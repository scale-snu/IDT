#!/bin/bash

BENCH_BIN=./workloads/SPEC/spec-wrapper.py

export SKIP_VALIDATION=1
export VERBOSE=1
export KMP_LIBRARY=throughput
export KMP_BLOCKTIME=infinite
export OMP_DYNAMIC=FALSE

MAX_THREADS=$(grep -c processor /proc/cpuinfo)
BENCH_RUN=""

export KMP_LIBRARY=throughput
export OMP_NUM_THREADS=${NTHREADS}
if [[ $CONFIG_PINNED == "yes" ]]; then
	export KMP_AFFINITY="compact"
elif [[ $NTHREADS -eq $MAX_THREADS ]]; then
	export KMP_AFFINITY="proclist=[0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,46],explicit" #,verbose
else
	export KMP_AFFINITY="compact,verbose"
	BENCH_RUN+="taskset -c 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,46 "
fi

if [[ "${BENCH_SIZE}" == "small" ]]; then
	# set nthreads to 1
	NTHREADS=1
	export OMP_NUM_THREADS=$NTHREADS

	BENCH_RUN+="${BENCH_BIN} 638 0 15"
elif [[ "${BENCH_SIZE}" == "large" ]]; then
	# set nthreads to 1
	NTHREADS=1
	export OMP_NUM_THREADS=1

	BENCH_RUN+="${BENCH_BIN} 638 0 20"
else
	err "ERROR: Retry with available SIZE. refer to benches/_imagick.sh"
	exit -1
fi

export BENCH_RUN
export BENCH_NAME="imagick_s_base.gcc-baseline-o2-m64"
