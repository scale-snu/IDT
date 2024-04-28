# Artifact for "IDT: Intelligent Data Placement for Multi-tiered Main Memory with Reinforcement Learning", HPDC 2024.

This repository contains source code and experiment scripts for the paper presented in the HPDC 2024 by J. Chang et al.

## Contents
This repository contains the RL model, Linux kernel, and experiment scripts. Please refer to the README.md in each subdirectory for more details.

* `IDT-Userspace/`: RL model for autotuning demotion policy.
* `linux/`: Linux kernel for region-granularity memory access monitoring and supporting demotion/promotion. <ins>The provided linux kernel should be built and installed.</ins>
* `experiments/`: Experiment scripts used in the paper.

## Hardware prerequisites
We have tested on:

### CPU
24-core Intel Xeon Platinum 8260 processor for each socket

### Memory
For each socket
* 32GB DDR4 DRAM (fast memory)
* 256GB Intel Optane DCPMM (slow memory), App-direct mode

## Contact

Juneseo Chang ([jschang0215@snu.ac.kr](mailto:jschang0215@snu.ac.kr))
