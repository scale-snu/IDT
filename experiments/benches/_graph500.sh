#!/bin/bash
BENCH_BIN=./workloads/graph500/omp-csr

export SKIP_VALIDATION=1
export VERBOSE=1
export KMP_LIBRARY=throughput
export KMP_BLOCKTIME=infinite
export OMP_DYNAMIC=FALSE

MAX_THREADS=$(grep -c processor /proc/cpuinfo)
BENCH_RUN=""

export KMP_LIBRARY=throughput

NTHREADS=${MAX_THREADS} # enable max threads for graph500
export OMP_NUM_THREADS=${NTHREADS}

if [[ $CONFIG_PINNED == "yes" ]]; then
	export KMP_AFFINITY="compact"
elif [[ $NTHREADS -eq $MAX_THREADS ]]; then
	export KMP_AFFINITY="proclist=[0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,46],explicit" #,verbose
else
	export KMP_AFFINITY="compact,verbose"
	BENCH_RUN+="taskset -c 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,46 "
fi

if [[ "${BENCH_SIZE}" == "8GB" ]]; then
	BENCH_RUN+="${BENCH_BIN}/omp-csr -s 24 -e 16 -V"
elif [[ "${BENCH_SIZE}" == "30GB" ]]; then
	NTHREADS=12
	export OMP_NUM_THREADS=${NTHREADS}
	BENCH_RUN+="${BENCH_BIN}/omp-csr -s 25 -e 28"
elif [[ "${BENCH_SIZE}" == "80GB" ]]; then
	BENCH_RUN+="${BENCH_BIN}/omp-csr -s 27 -e 19"
elif [[ "${BENCH_SIZE}" == "96GB" ]]; then
	BENCH_RUN+="${BENCH_BIN}/omp-csr -s 27 -e 23"
else
	err "ERROR: Retry with available SIZE. refer to benches/_graph500.sh"
	exit -1
fi

export BENCH_RUN
export BENCH_NAME="omp-csr"
