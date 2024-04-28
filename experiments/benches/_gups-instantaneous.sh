#!/bin/bash

THREADS=16
UPDATES=1000000000
DATASIZE=8

GUPSPATH="/scale/cal/home-interns/jschang0215/IDT-Experiment/workloads/GUPS"

if [[ "${BENCH_SIZE}" == "small" ]]; then
	BENCH_RUN+="numactl -N0 -c0 -- "$GUPSPATH/gups-hotset-move" $THREADS $UPDATES 34 $DATASIZE 33"
elif [[ "${BENCH_SIZE}" == "large" ]]; then
	BENCH_RUN+="numactl -N0 -c0 -- "$GUPSPATH/gups-hotset-move" $THREADS $UPDATES 37 $DATASIZE 36"
else
	err "ERROR: Retry with available SIZE. refer to benches/_gups-instanteneous.sh"
	exit -1
fi

export BENCH_RUN
export BENCH_NAME="gups-hotset-move"
