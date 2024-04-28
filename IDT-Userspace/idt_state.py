import numpy as np
from dataclasses import dataclass
from sklearn.decomposition import PCA

from idt_config import *
from idt_action import *
from mem_stat import *
from utils import *

@dataclass
class IDT_Stat:
    nr_regions: int = 0
    age_min: int = 0
    age_q1: int = 0
    age_q2: int = 0
    age_q3: int = 0
    age_max: int = 0
    demoted: int = 0
    promoted: int = 0
    demotion_enabled: bool = False

@dataclass
class Reward_State:
    demoted:    int = 0
    promoted:   int = 0


class State:
    """
    Get the IDT region state.

    Currently used observations are demoted_pages, promoted_pages, and age statistics.
    """

    idt_stat = [IDT_Stat()] * IDT_Config.num_nodes
    cumulative_idt_stat = np.empty((0, 0))
    last_total_promoted = [0] * IDT_Config.num_nodes
    last_total_demoted = [0] * IDT_Config.num_nodes
    
    reward_state = [Reward_State()] * IDT_Config.num_nodes

    proc_param_file = None

    state_num = 5
    state_window = 32
    input_state_num = 9

    target_started = False

    @staticmethod
    def __init__ ():
        try:
            State.proc_param_file = open("/proc/idt_state", "r")
        except IOError:
            print("OS error occurred trying to open /proc/idt_state")
            exit()
        
        State.read()
        State.cumulative_idt_stat = np.empty((0, State.state_num))

    @staticmethod
    def is_target_terminated ():
        """Check if target program is terminated"""

        if not State.target_started:
            return False
        
        for node_idx in range(IDT_Config.num_nodes):
            if  (State.idt_stat[node_idx].nr_regions != 0 or State.idt_stat[node_idx].age_min != 0 or \
                State.idt_stat[node_idx].age_q1 != 0 or State.idt_stat[node_idx].age_q2 != 0 or \
                State.idt_stat[node_idx].age_q3 != 0 or State.idt_stat[node_idx].age_max != 0 or \
                State.idt_stat[node_idx].demoted != 0 or State.idt_stat[node_idx].promoted != 0 or \
                State.idt_stat[node_idx].demotion_enabled != 0):
                return False

        return True

    @staticmethod
    def is_target_started ():
        if State.target_started:
            return True

        for node_idx in range(IDT_Config.num_nodes):
            if State.idt_stat[node_idx].nr_regions != 0:
                return True

        return False

   
    @staticmethod
    def read ():
        """
        Read /proc/idt_state.

        /proc/idt_state is: nr_regions, age_[min|q1|q2|q3|max], total_[promoted|demoted]_pages.
        """
        
        raw = None
        while raw is None:
            try:
                raw = State.proc_param_file.read()
            except:
                State.proc_param_file = open("/proc/idt_state", "r")

        # Parse /proc/idt_state
        tmp = list()
        for x in raw.split():
            try:
                x = int(x)
            except:
                continue
            tmp.append(x)
        raw = tmp
        
        i = 0
        for node_idx in range(IDT_Config.num_nodes):
            demoted = raw[i+6] - State.last_total_demoted[node_idx]
            promoted = raw[i+7] - State.last_total_promoted[node_idx]
            State.idt_stat[node_idx] = IDT_Stat(raw[i], raw[i+1], raw[i+2], raw[i+3], raw[i+4], raw[i+5], demoted, promoted, bool(raw[i+8]))

            State.last_total_demoted[node_idx] = raw[i+6]
            State.last_total_promoted[node_idx] = raw[i+7]
            i += State.input_state_num

        if State.is_target_terminated():
            logging.info("\nTarget program terminated")
            exit()
        elif State.is_target_started():
            State.target_started = True

        # set file_pos=start for next read
        State.proc_param_file.seek(0) 


    @staticmethod
    def get_state (nid):
        """
        Get current state of node nid.

        Current state is obtained from previous reward.
        State is [demoted_pages, promoted_pages].

        :return: np array state
        """

        cur_state = np.array([State.idt_stat[nid].age_min, State.idt_stat[nid].age_q1, State.idt_stat[nid].age_q2, State.idt_stat[nid].age_q3, State.idt_stat[nid].age_max], dtype=np.float32)

        # Save cumulatvie reward (windowed state)
        State.cumulative_idt_stat = np.vstack([State.cumulative_idt_stat, cur_state])

        if State.cumulative_idt_stat.shape[0] >= State.state_window:
            State.cumulative_idt_stat = State.cumulative_idt_stat[-State.state_window:]

        # Return current state if not enough variance
        if np.var(State.cumulative_idt_stat) < 1e-10 or State.cumulative_idt_stat.shape[0] <= State.state_num:
            return cur_state

        pca = PCA(n_components=State.state_num)
        pca_data = pca.fit_transform(State.cumulative_idt_stat)
        state = np.mean(pca_data, axis=0)

        return state

    @staticmethod
    def is_demotion_enabled (nid):
        """
        Check if demotion enabled for node nid.

        Always call State.read() before invoking.
          
        :param nid: node id to be checked
        :return: boolean
        """

        return State.idt_stat[nid].demotion_enabled

    @staticmethod
    def is_stable (nid):
        """
        Check if node nid is stable.

        Stable means there are no demoted/promoted pages.

        :param nid: node id to be checked
        :return: boolean
        """

        return (State.idt_stat[nid].demoted <= 0 and State.idt_stat[nid].promoted <= 0)


    @staticmethod
    def print (nid):
        """Print current state of nid."""

        logging.info(f"[State{nid}] nr_regions: {State.idt_stat[nid].nr_regions}")
        logging.info(f"[State{nid}] Min: {State.idt_stat[nid].age_min}, Q1: {State.idt_stat[nid].age_q1}, Q2: {State.idt_stat[nid].age_q2}, Q3: {State.idt_stat[nid].age_q3}, Max: {State.idt_stat[nid].age_max}")
        logging.info(f"[State{nid}] Demoted: {State.reward_state[nid].demoted}, Promoted: {State.reward_state[nid].promoted}, Demotion enabled: {State.idt_stat[nid].demotion_enabled}\n")
