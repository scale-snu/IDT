#!/bin/bash

if [ $EUID -ne 0 ]; then
    echo "Please run as root or sudo"
    exit -1
fi

NUM_ITER=1

SCHEME="--autonuma"
#SCHEME="--memtiering"
#SCHEME="--memtiering-MGLRU"
#SCHEME="--autotiering"
#SCHEME="--tpp"
#SCHEME="--inteltiering"
#SCHEME="--idt"

TIER="--multitier"
#TIER="--twotier"

for ITER in $(seq 1 ${NUM_ITER}); do
	#bash run-core.sh --benchmark graph500 --wss 30GB ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark imagick --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark xz --wss tiny ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark roms --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bc --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bfs --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark pr --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark deepsjeng --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bwaves --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark cactuBSSN --wss tiny ${SCHEME} ${TIER}

	#bash run-core.sh --benchmark gups-pebs --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark gups-random --wss small ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark gups-instantaneous --wss small ${SCHEME} ${TIER}


	#bash run-core.sh --benchmark graph500 --wss 96GB ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark imagick --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark xz --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark roms --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bc --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bfs --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark pr --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark deepsjeng --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark bwaves --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark cactuBSSN --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark gups-pebs --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark gups-random --wss large ${SCHEME} ${TIER}
	#bash run-core.sh --benchmark gups-instantaneous --wss large ${SCHEME} ${TIER}
done
