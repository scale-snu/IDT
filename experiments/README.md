# IDT-Experiment

We provide experiment scripts and benchmarks used in our paper's evaluation section.

## Supported Schemes

The experiment scripts supports 5 different memory tiering schemes. 

* [IntelTiering 0.8](https://git.kernel.org/pub/scm/linux/kernel/git/vishal/tiering.git/)
* [[ASPLOS'23] TPP](https://lore.kernel.org/all/cover.1637778851.git.hasanalmaruf@fb.com/)
* [[ATC'21] AutoTiering](https://github.com/csl-ajou/AutoTiering?tab=readme-ov-file)
* AutoNUMA Tiering (MGLRU)
  * To test this scheme, you need Linux kernel v6.1.0 above. Note that IDT kernel is based on Linux kernel v6.0.19 and doesn't supports MGLRU.
  * Reconfiguring the demotion path to leverage remote DRAM over local DCPMM is also required.
* IDT
  * Please install IDT kernel and IDT-Userspace.

## Benchmark Lists

Note that all benchmark binaries are built in `workloads/` directory. There is no need for additional installation or compilations.

* graph500
* SPEC CPU 2017
  * 603.bwaves_s
  * 607.cactuBSSN_s
  * 631.deepsjeng_s
  * 638.imagick_s
  * 654.roms_s
  * 657.xz_s
* GAP Benchmark Suite with twitter graph
  * Breadth-First Search (BFS)
  * PageRank (PR)
  * Betweenness Centrality (BC)
  * To use GAPBS, please <ins>download</ins> `twitter.sg` <ins>used for GAPBS as</ins> `worklaods/GAPBS/gapbs-data/twitter.sg`.

## Description

* `artifact_result/` and `artifact_log/`: Raw data in the paper's evaluation section.
* `init_scripts/`: Initialization scripts before running benchmarks.
* `result/`: The benchmark execution time is saved in subdirectory with corresponding scheme name.
* `log/`: The benchmark execution log is saved in subdirectory with corresponding scheme name.
* `benches/`: Driver script for launching benchmark.
* `workloads/`: Benchmark binaries and code.
* `_config` files: Set up configuration for each supported scheme.
* `run.sh`: Main experiment driver script.

## Usage

Run experiment with `sudo ./run.sh`

1. Select scheme by uncommenting desired SCHEME in `run.sh`.

2. Select desired workload by uncommenting `run-core.sh`. For example, when running `xz` workload with large memory, uncomment

   ```bash
   bash run-core.sh --benchmark xz --wss large ${SCHEME} ${TIER}
   ```

3. Check benchmark exeuction time in `result/<SCHEME>` and execution logs in `log/<SCHEME>/`.
