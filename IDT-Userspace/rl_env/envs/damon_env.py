import gym
import numpy as np
import random

from idt_action import *
from idt_state import *
from idt_reward import *
from mem_stat import *
from experience import *


class DAMON_v0 (gym.Env):
    """Environment for RL training. Set 'select_env=damon-v0' to use.

    DAMON_v0 gets state, reward, and action from the experience buffer.
    The values are calculated when storing in the experience buffer.
    """

    metadata = { "render.modes": ["human"] }

    def __init__ (self):
        # Action [scale1, scale2, scale3]
        self.action_space = gym.spaces.Box(
                                low=0.0,
                                high=np.inf,
                                shape=(Action.action_num,),
                                dtype=np.float32)

        # Observation [demoted_pages, promoted_pages]
        self.observation_space = gym.spaces.Box(
                                    low=-np.inf,
                                    high=np.inf,
                                    shape=(State.state_num,),
                                    dtype=np.float32)
        self.reset()


    def __get_state (self, experience):
        return Experience.get_state(experience)


    def __get_reward (self, experience):
        return Experience.get_reward(experience)


    def reset (self):
        """Reset environment.
        Configured as OpenAI Gym doc.

        :return: state
        """

        # Reset env state & reward
        self.state = np.array([0.0] * State.state_num, dtype=np.float32)
        self.reward = 0
        self.done = False
        self.info = {}

        self.experience_buf = Experience.read() 

        return self.state


    def step (self, action):
        """Step action.
        Configured as OpenAI Gym doc.

        Step 1. Read random entry from experience_buf
        Step 2. Obtain state, reward from the entry
        Step 3. Delete read entry

        :return: [state, reward, done, info]
        """

        if self.experience_buf:
            key, experience = random.choice(list(self.experience_buf.items()))
            self.state = self.__get_state(experience)
            self.reward = self.__get_reward(experience)
        else:
            self.state = np.array([0.0] * State.state_num, dtype=np.float32)
            self.reward = 0

        return [self.state, self.reward, self.done, self.info]


    def close (self):
        pass
