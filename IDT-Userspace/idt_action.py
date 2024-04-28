import numpy as np
import random
import time

from idt_config import *
from mem_stat import *
from idt_state import *
from utils import *


class Action:
    """
    Change min_age by action.

    Action is setting min_age as  Q2, Q3, or MAX of current ages for each node.
    """
        
    action_apply_wait = 1
    max_action_apply_wait = 10
    action_num = 3

    idt_action = [IDT_Config.action_min] * IDT_Config.num_nodes

    # action enum
    MIN = 0
    Q1 = 1
    Q2 = 2
    Q3 = 3
    MAX = 4

    @staticmethod
    def __init__ ():
        pass

    @staticmethod
    def read ():
        """
        Read /proc/idt_action.

        Action for each node is obtained.
        """

        raw = None
        while raw is None:
            try:
                proc_action_file = open("/proc/idt_action", "r")
                raw = proc_action_file.read()
            except:
                continue

        # Parse /proc/idt_action
        tmp = list()
        for x in raw.split():
            try:
                x = int(x)
            except:
                continue
            tmp.append(x)
        Action.idt_action = tmp

        print(Action.idt_action)


    @staticmethod
    def write (nid, action):
        """Write /proc/idt_action

        :param nid: node id to write action
        :param action: action value
        """
        
        Action.idt_action[nid] = action

        action_str = ' '.join(map(str, Action.idt_action))

        done = False

        while not done:
            try:
                proc_action_file = open("/proc/idt_action", "w")
                proc_action_file.write(f"{action_str}\n")
                done = True
            except IOError:
                continue


    @staticmethod
    def nn_to_action (nn_out):
        """
        NN output to action.

        Find argmax of NN output node.
        NN output node corresponds to [q2, q3, max]
        apply_action shoulb be called to actually 'apply' action.

        :return: action [Q2|Q3|MAX]
        """

        idx = np.argmax(nn_out)

        #action = [Action.Q1, Action.Q2, Action.Q3]
        action = [Action.Q2, Action.Q3, Action.MAX]

        return action[idx]
        

    @staticmethod
    def apply_action (nid, action):
        """
        Apply action and update & apply min_age.

        When using NN output, call nn_to_action before invoking.
        
        :param nid: node id to apply action
        :param action: action to be applied
        """
        
        epsilon = 1

        if action == Action.MIN:
            action_val = State.idt_stat[nid].age_min + epsilon
        elif action == Action.Q1:
            action_val = State.idt_stat[nid].age_q1
        elif action == Action.Q2:
            action_val = State.idt_stat[nid].age_q2
        elif action == Action.Q3:
            action_val = State.idt_stat[nid].age_q3
        elif action == Action.MAX:
            action_val = State.idt_stat[nid].age_max

        action_val = max(action_val, IDT_Config.action_min)
        Action.write(nid, action_val)


    @staticmethod
    def random_action ():
        """
        Select random action.

        apply_action shoulb be called to actually 'apply' action.

        :param nid: node id to select random action
        :return: randomly select action.
        """

        action = random.randint(Action.Q2, Action.MAX)

        return action
    

    @staticmethod
    def print (nid, action):
        """Print current action of nid."""

        action_msg = ["Min", "Q1", "Q2", "Q3", "Max"]
        logging.info(f"[Action{nid}] {action_msg[action]} {Action.idt_action[nid]}\n")
