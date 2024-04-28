#!/bin/bash

export SCRIPTDIR=$(pwd)

MEM_NODES=($(ls /sys/devices/system/node | grep node | awk -F 'node' '{print $NF}'))

RESULT_PATH="${SCRIPTDIR}/result"
LOG_PATH="${SCRIPTDIR}/log"

function func_err() {
	exit -1
}

function func_set_nthreads() {
	info "Set nthreads..."
	if [ -z $NTHREADS ];then
		NTHREADS=$(grep -c processor /proc/cpuinfo)
	fi
	info "nthreads: ${NTHREADS}"
	export NTHREADS
}

# drop cache
function func_clean_page_cache() {
	info "Drop page cache and stop background services..."
	DROPCACHE_SCRIPT="sudo /scale/cal/home/wjdoh/init.sh"
	${DROPCACHE_SCRIPT} > /dev/null
	
	# clear dmesg
	# sudo dmesg -c > /dev/null
}

function func_set_files() {
	RESULT_FILE="${RESULT_PATH}/${TEST}/${BENCH_NAME}_${BENCH_SIZE}_${TEST}_${DATE}.txt"
	export RESULT_FILE
	LOG_FILE="${LOG_PATH}/${TEST}/${BENCH_NAME}_${BENCH_SIZE}_${TEST}_${DATE}.log"
	export LOG_FILE
}

function func_generate_result() {
	info "Generate result file..."
	cat ${LOG_FILE} | grep "execution_time" 2>&1 | tee ${RESULT_FILE}
	sleep 1
}

function func_kswapd() {
	ONOFF=$1
	nodes=($(ls /sys/devices/system/node/ | grep node))
	case $ONOFF in
		[Oo][Nn])
			FAILURE_NUM=0
			;;
		[Oo][Ff][Ff])
			FAILURE_NUM=16
			;;
		*)
			echo "usage: $0 [on|off]"
			exit -1
			;;
	esac

	for node in ${nodes[@]}; do
		if [[ ! -e /sys/devices/system/node/$node/kswapd_failures ]]; then
			err "kswapd_failure interface is not supported"
			continue
		fi
		echo $FAILURE_NUM | sudo tee /sys/devices/system/node/$node/kswapd_failures > /dev/null
		info "Set kswapd failure number: $node: $FAILURE_NUM"
	done
}

function func_config_demotion_path() {
	node=$1
	path=$2
	FILE_PATH=/sys/devices/system/node/node$1/demotion_path

	if [[ -e ${FILE_PATH} ]]; then
		echo $path | sudo tee ${FILE_PATH} > /dev/null
		info "Set demotion_path  [${node} --> ${path}]"
	else
		err "The demotion_path [$node --> $path] does not exist. You should execute AutoTiering kernel"
	fi
}

function func_config_promotion_path() {
	node=$1
	path=$2
	FILE_PATH=/sys/devices/system/node/node$1/promotion_path

	if [[ -e ${FILE_PATH} ]]; then
		echo $path | sudo tee ${FILE_PATH} > /dev/null
		info "Set promotion_path [${node} --> ${path}]"
	else
		err "The promotion_path [$node --> $path] does not exist. You should execute AutoTiering kernel"
	fi
}

function func_config_migration_path() {
	node=$1
	path=$2
	FILE_PATH=/sys/devices/system/node/node$1/migration_path

	if [ $CONFIG_NVM == "no" ];then
		if [[ $node -ne 0 && $node -ne 1 ]];then
			return
		fi
	fi
	if [[ -e ${FILE_PATH} ]]; then
		echo $path | sudo tee ${FILE_PATH} > /dev/null
		info "Set migration_path [${node} --> ${path}]"
	else
		err "The migration_path [$node --> $path] does not exist. You should execute AutoTiering kernel"
	fi
}

function func_initialize_migration_path() {
	for node in ${MEM_NODES[@]}; do
		func_config_demotion_path  $node -1
		func_config_promotion_path $node -1
		func_config_migration_path $node -1
	done
}

function func_prepare_migration_path() {
	for node in ${MEM_NODES[@]}; do
		case $node in
			0)
				func_config_migration_path $node 1
				func_config_promotion_path $node -1
				func_config_demotion_path  $node 2
				;;
			1)
				func_config_migration_path $node 0
				func_config_promotion_path $node -1
				func_config_demotion_path  $node 3
				;;
			2)
				func_config_migration_path $node 3
				func_config_promotion_path $node 0
				func_config_demotion_path  $node -1
				;;
			3)
				func_config_migration_path $node 2
				func_config_promotion_path $node 1
				func_config_demotion_path  $node -1
				;;
			*)
				err "Add NUMA node$node interfaces"
				;;
		esac
	done
}


