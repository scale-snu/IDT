from gym.envs.registration import register

register(
    id="damon-v0",
    entry_point="rl_env.envs:DAMON_v0",
    max_episode_steps=5,
)
