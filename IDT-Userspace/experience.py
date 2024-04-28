import numpy as np
import shutil
import pickle

from utils import *


class Experience:
    
    buf_file = "./data/experience.buf"
    buf = dict()

    
    @staticmethod
    def save (state, reward, action):
        """
        Save experience to experience buffer.

        Experience buffer entry is stored as (state, reward, action).
        Indexed by state.

        :param state: state of experience
        :param reward: reward of experience
        :param action: action of experience
        """

        Experience.buf[state.tobytes()] = (state, reward, action)


    @staticmethod
    def lookup (state):
        """
        Find entry of state in experience buffer.

        :param state: state to be found
        """

        key = state.tobytes()

        if key in Experience.buf:
            logging.info("[Experience] in buffer")
            return Experience.buf[key]
        else:
            return None


    @staticmethod
    def write ():
        """
        Save experience buffer to file as object.

        Write before end/reset experience buffer.
        """

        with open(Experience.buf_file, "wb") as _buf_file:
            pickle.dump(Experience.buf, _buf_file)


    @staticmethod
    def read ():
        """
        Read experience buffer file.

        Read before starting to use experience buffer.

        :return: experience buffer (dict object)
        """

        try:
            _buf_file = open(Experience.buf_file, "rb")
            return pickle.load(_buf_file)
        except IOError:
            return None


    @staticmethod
    def reset ():
        """Reset (Clear) experience buffer."""

        del Experience.buf
        Experience.buf = dict()


    @staticmethod
    def get_state (experience):
        """Get state from experience buffer entry."""

        return experience[0]

    
    @staticmethod
    def get_reward (experience):
        """Get reward from experience buffer entry."""

        return experience[1]


    @staticmethod
    def get_action (experience):
        """Get action from experience buffer entry."""

        return experience[2]
