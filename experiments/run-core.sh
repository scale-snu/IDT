#!/bin/bash

export SCRIPTDIR=$(pwd)

# set print style
source $SCRIPTDIR/_message.sh

# bin set
TIME="/usr/bin/time"
NUMA_EXEC="numactl --cpunodebind=0 --physcpubind=0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46"
export NUMA_EXEC

# config set 
DATE=$(date +%Y%m%d%H%M)
MACHINE=$(uname -n)

# result files
RESULT_FILE=""
LOG_FILE=""

export RESULT_FILE
export LOG_FILE

# global variable set
TEST="default"
CONFIG_AUTONUMA="no"
AUTONUMA="off"
CONFIG_INTELTIERING="no"
CONFIG_AUTOTIERING="no"
CONFIG_TPP="no"
CONFIG_IDT="no"
CONFIG_NVM=no
CONFIG_MULTITIER="no"
CONFIG_MGLRU="no"
CONFIG_LAST="no"

export CONFIG_AUTONUMA
export CONFIG_MEMTIERING
export CONFIG_INTELTIERING
export CONFIG_AUTOTIERING
export CONFIG_IDT
export CONFIG_NVM
export CONFIG_MULTITIER
export CONFIG_MGLRU
export CONFIG_LAST

# import scripts
source $SCRIPTDIR/_config_inteltiering.sh
source $SCRIPTDIR/_config_autotiering.sh
source $SCRIPTDIR/_config_autonuma.sh
source $SCRIPTDIR/_config_tpp.sh
source $SCRIPTDIR/_config_idt.sh
source $SCRIPTDIR/_config_mglru.sh
source $SCRIPTDIR/_utils.sh

function func_run_bench() {
    ${TIME} -f "execution_time %e (s)" ${NUMA_EXEC} ${BENCH_RUN} 2>&1 | tee ${LOG_FILE}
}

function func_prepare() {
    info "Preparing benchmark start..."

    # set nthreads
    func_set_nthreads

    # MSR
    sudo modprobe msr
    sleep 1

    info "Disable prefetcher..."
    sudo ./init_scripts/prefetchers.sh disable > /dev/null

    info "Disable turbo-boost..."
    sudo ./init_scripts/turbo-boost.sh disable > /dev/null

    info "Set max CPU frequency..."
    sudo ./init_scripts/set_max_freq.sh > /dev/null

    sleep 1

    # config default linux autonuma
    func_config_autonuma ${AUTONUMA} 
    sleep 1

    # config default MGLRU
    func_config_mglru
    sleep 1

    # config autotiering
    func_config_autotiering
    sleep 1

    # config intel tiering
    func_config_inteltiering
    sleep 1

	# config tpp
	func_config_tpp
	sleep 1

    # config idt
    func_config_idt
    sleep 1

    # Drop page cache
    func_clean_page_cache
    sleep 1

    export BENCH_SIZE
    export BENCH_NAME

    # The source will bring ${BENCH_RUN}
    if [[ -e ./benches/_${BENCH_NAME}.sh ]]; then
        source ./benches/_${BENCH_NAME}.sh
    else
        err "_${BENCH_NAME}.sh does not exist"
        func_err
    fi
}

function func_main() {
    info "Run benchmark main..."

    info "Bechmark: ${BENCH_NAME} with ${BENCH_SIZE}"
    info "Start date $(date)"

    func_set_files
    
    func_run_bench &
    WAIT_PID=$!
    
    # wait unitl find pid
    while [ $(pidof ${BENCH_NAME} | wc | cut -c7) == "0" ]; do
        sleep 1
    done
    PID=$(pidof ${BENCH_NAME})
	echo $BENCH_NAME > pid.log
	echo $PID >> pid.log
    export PID
    info "Benchmark pid: ${PID}"

    # configure idt pid (register to DAMON)
    func_config_pid_idt

    # wait untill bench end
    wait ${WAIT_PID}

    func_generate_result

    func_finish

    info "End date $(date)"
}

function func_finish() {
    # finish for each test
    case $TEST in
        "tiering")
            info "Get demoted and promoted pages..."
            grep -H -E 'pgpromote|pgdemote' /sys/devices/system/node/node*/vmstat >> ${RESULT_FILE}
            ;;
        "autonuma")
			func_initialize_migration_path
            ;;
        "memtiering")
			func_initialize_migration_path
            ;;
        "memtiering-mglru")
			func_initialize_migration_path
            ;;
        "autotiering")
			func_initialize_migration_path
            ;;
		"tpp")
			info "Get demoted and promoted pages..."
			cat /proc/vmstat | grep -E "promote|demote|migrate" >> ${RESULT_FILE}
			;;
        "idt")
            info "Disable demotion..."
            bash /scale/cal/home-interns/jschang0215/linux-damon/mm/damon/disable_demotion.sh

            info "Copy trained model to pre-trained path..."
            cd /scale/cal/home-interns/jschang0215/linux-damon/mm/damon/rl
            rm -r -f pre-trained/checkpoint.old
            mv pre-trained/checkpoint pre-trained/checkpoint.old
            cp -r -f chkpt/* pre-trained/checkpoint
            cd /scale/cal/home-interns/jschang0215/experiment

            ;;
        *)
            err "Usage: invalid experiment $TEST"
            func_err
            ;;
    esac

    # Initialization to linux default configuration
    sudo sysctl kernel.numa_balancing=1
    sleep 1

    func_clean_page_cache
    sleep 1

    # Post-processing of specific benchmarks
    source ./benches/finish.sh
    sleep 1
}

function func_pebs_analysis() {
	bash pebs-analysis.sh
}

ARGS=`getopt -o b:w: --long benchmark:,wss:,autonuma,memtiering-MGLRU,memtiering,inteltiering,autotiering,tpp,idt,multitier,twotier,last -n run-bench.sh -- "$@"`
if [ $? -ne 0 ]; then
    echo "Terminating..." >&2
    exit -1
fi

eval set -- "${ARGS}"

# parse argument
while true; do
    case "$1" in
        -b|--benchmark)
            BENCH_NAME+=( "$2" )
            shift 2
            ;;
        -w|--wss)
            BENCH_SIZE="$2"
            shift 2
            ;;
        --autonuma)
            TEST="autonuma"
            CONFIG_AUTONUMA="yes"
            AUTONUMA=AN
            shift 1
            ;;
		--memtiering-MGLRU)
            TEST="memtiering-mglru"
			CONFIG_MGLRU="yes"
            CONFIG_MEMTIERING="yes"
            AUTONUMA=MT
            shift 1
            ;;
		--memtiering)
            TEST="memtiering"
            CONFIG_MEMTIERING="yes"
            AUTONUMA=MT
            shift 1
            ;;
        --inteltiering)
            TEST="inteltiering"
            CONFIG_INTELTIERING="yes"
            shift 1
            ;;
        --autotiering)
            TEST="autotiering"
            CONFIG_AUTOTIERING="yes"

            # autonuma should be enabled
            AUTONUMA=AN
            shift 1
            ;;
		--tpp)
			TEST="tpp"
			CONFIG_TPP="yes"
			shift 1
			;;
        --idt)
            TEST="idt"
            CONFIG_IDT="yes"
            shift 1
            ;;
		--multitier)
			CONFIG_MULTITIER="yes"
			shift 1
			;;
		--twotier)
			CONFIG_MULTITIER="no"
			shift 1
			;;
		--last)
			CONFIG_LAST="yes"
			shift 1
			;;
        --)
            break
            ;;
        *)
            err "Unrecognized option $1"
            exit -1
            ;;
    esac
done

if [ -z  "${BENCH_NAME}" ]; then
    err "Benchmark name parameter must be specified"
    func_usage
    exit -1
fi

func_prepare
func_main
