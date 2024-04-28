import shutil

from utils import *
from rl_config import *


class Train:
    """Train the RL model via experience buffer."""

    def __init__ (self, select_env="damon-v0"):
        logging.info("Train initializing.")

        self.rl_config = RL_Config()
        self.rl_config.rllib_init(select_env)
        shutil.rmtree(self.rl_config.ray_results, ignore_errors=True, onerror=None)

        self.agent = self.rl_config.rllib_agent_config(1, select_env)
        
        self.n_iter = 1

        logging.info("Train initialize complete.")


    def train (self):
        """
        Train RL model.

        Trained model is saved as a checkpoint.

        :return: saved checkpoint file path
        """
        
        # init directory in which to save checkpoints
        shutil.rmtree(self.rl_config.chkpt_root, ignore_errors=True, onerror=None)

        status = "{:2d}: Reward: {:6.2f} / {:6.2f}"

        for itr in range(self.n_iter):
            res = self.agent.train()
            print(status.format(itr, res["episode_reward_mean"], res["episode_reward_max"]))

        saved_chkpt = self.agent.save(self.rl_config.chkpt_root)
        policy = self.agent.get_policy()
        model = policy.model
        #print(model.base_model.summary())
        
        return saved_chkpt


if __name__ == "__main__":
    RL_Train = Train()
    
    timer = Timer()
    logging.info("Train running...")
    timer.start()
    RL_Train.train()
    timer.end()
    logging.info("Train end.")
    timer.print()
