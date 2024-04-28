import random

from idt_config import *
from rl_config import *
from idt_state import *
from idt_action import *
from idt_reward import *
from utils import *
from experience import *
from train import *


class Infer:
    """
    Infer using trained RL model.
    
    Infer by the trained RL model. Infer module collects the state and applies action online. (Recall that training utilizes experience buffer data) 
    Step 1. Collect current state.
    Step 2. Use trained model to determine action for current state.
    Step 3. Apply action.
    Step 4. Wait for Action.action_apply_wait seconds. This is to wait for action to be 'fully' applied.
    Step 5. Calculate reward.
    Step 6. Save to state, action, reward to the experience buffer.
    Step 7. Later, train module trains from the experience buffer.
    """

    # probability of choosing random action instead of using trained model
    rand_p = 0.05

    def __init__ (self, chkpt_file="./pre-trained/checkpoint", select_env="damon-v0"):
        logging.info("Infer initializing.")

        rl_config = RL_Config()
        rl_config.rllib_init(select_env)
        self.agent = rl_config.rllib_agent_config(1, select_env)
        self.agent.restore(chkpt_file)

        self.policy = self.agent.get_policy()
        self.model = self.policy.model

        Action.__init__()
        State.__init__()

        logging.info("Infer initialize complete.")


    def __infer (self, nid):
        """
        Infer by using trained model or randomly selecting action for node nid.
        
        Step 1. Get state.
        Step 2. For p < rand_p, choose random action and get reward by Reward.get_reward.
        Step 3. Apply action by RL model.
        Step 4. Get reward by actually write action to /proc/idt_action and calculate reward function by Reward.get_reward.
        Step 5. Save to the expereince buffer.

        :param nid: node id to be inferred
        """
        
        timer = Timer()
        timer.start()

        state = State.get_state(nid)
        State.print(nid)

        # random action
        if random.random() < Infer.rand_p:
            action = Action.random_action()
            Action.apply_action(nid, action)
        else:
            nn_out = self.agent.compute_single_action(state)
            action = Action.nn_to_action(nn_out)
            Action.apply_action(nid, action)
        
        Action.print(nid, action)

        timer.end()
        timer.print()

        return (state, action)

    
    def infer (self):
        """Infer all nodes.

        1. Check node with demotion enabled.
        2. Run infer to those nodes.
        """

        logging.info("Infer start\n")
        
        done = False
        
        # While loop is to wait unitl there is enabled demotion
        # and action demotes/promotes pages
        while not done:
            infers = []
            infer_nodes = []
            waited_nodes = []
        
            State.read()

            for idx in range(IDT_Config.num_nodes):
                if State.is_demotion_enabled(idx):
                    state, action = self.__infer(idx)
                    infers.append((idx, state, action))
                    infer_nodes.append(idx)

            if not infer_nodes:
                # logging.info("No demotionable node. Retrying Infer.\n")
                time.sleep(IDT_Config.demotion_node_wait)
                continue

            # Wait unitl change in demoted/promoted in any node
            # Get nodes with change in demoted/promoted in any node
            waited_nodes = Reward.wait_for_reward(infer_nodes)

            if not waited_nodes:
                # logging.info("No action works. Retrying Infer.\n")
                infers = []
                infer_nodes = []


            for nid, state, action in infers:
                # Get reward from waited node
                if nid not in waited_nodes:
                    continue

                reward = Reward.get_reward(nid)

                # Save experience only if change in demoted/promoted (just double checking)
                if reward is None:
                    continue

                Experience.save(state, reward, action)

                done = True

        logging.info("Infer finish\n\n")


    def reset (self, chkpt_file):
        """
        Reset by loading the newly trained model and reset the experience buffer.

        :param chkpt_file: model to be restored
        """

        logging.info(f"Model restored from {chkpt_file}.")
        self.agent.restore(chkpt_file)
        Experience.reset()


if __name__ == "__main__":
    infer = Infer()

    infer.infer()
