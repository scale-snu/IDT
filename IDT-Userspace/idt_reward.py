import math
import random
import time
from dataclasses import dataclass

from idt_config import *
from idt_action import *
from idt_state import *
from mem_stat import *
from utils import *


class Reward:
    """
    Calculate reward.

    Action should be applied before invoking `get_reward`.
    Reward is calculated after waiting for `IDT_Config.apply_wait` to 'fully' apply action.
    """

    epsilon = 1
    base = 1

            
    @staticmethod
    def wait_for_reward (nids):
        """
        Wait unitl there is demoted_pages or promoted_pages for all nodes.

        Step 1. Sleep for `IDT_Config.apply_wait` seconds.
        Step 2. If there is demoted or promoted pages in any node, return.
        Step 3. If there is still no demoted or promoted pages, wait another  `IDT_Config.apply_wait` seconds.
        Step 4. If total waited time is `IDT_Config.wait_max`, return.

        :param nids: nodes that would be monitored for wait
        :return: nodes such that there is demoted or promoted pages
        """

        done_nids = []

        time.sleep(IDT_Config.apply_wait)
        wait_time = IDT_Config.apply_wait
        State.read()

        # Check if program terminated
        if State.is_target_terminated():
            logging.info("\nTarget program terminated")
            exit()

        # Wait another action_apply_wait time
        while wait_time < IDT_Config.wait_max and (not done_nids):
            for idx in nids:
                if not State.is_stable(idx):
                    done_nids.append(idx)

            time.sleep(IDT_Config.apply_wait)
            State.read()
            wait_time += IDT_Config.apply_wait

        return done_nids


    @staticmethod
    def get_reward (nid):
        """
        Get reward when action is applied.

        Always call wait_for_reward before invoking.

        Step 1. Wait by invoking `__wait()`.
        Step 2. Calculate reward function.

        :param nid: node id to get reward
        :return: reward; return None if no demoted and promoted pages
        """

        if State.is_stable(nid):
            return None

        reward = math.log( (State.idt_stat[nid].demoted + Reward.epsilon) / (State.idt_stat[nid].promoted + Reward.epsilon) ) - Reward.base

        State.reward_state[nid] = Reward_State(State.idt_stat[nid].demoted, State.idt_stat[nid].promoted)

        logging.info(f"[Reward{nid}] demoted {State.idt_stat[nid].demoted}, promoted {State.idt_stat[nid].promoted}, reward: {reward}\n")

        return reward
