#!/bin/bash

DEMOTION_WATERMARK=10
PROMOTION_WATERMARK=1

SAMPL=10000
AGGR=1000000
RG_UP=100000000
MIN_RG=1000
MAX_RG=10000

# configure idt
function func_config_idt() {
    info "Config IDT ${CONFIG_IDT}..."
    
    case $CONFIG_IDT in
        [Nn][Oo]) # IDT OFF
            ;;
        [Yy][Ee][Ss]) # IDT ON
            info "Enable_demotion..."
            sudo bash -c "echo 'true' >> /sys/kernel/mm/numa/demotion_enabled"

            # set damon attrs
            func_set_damon_attrs

			#echo 15 | sudo tee /proc/sys/vm/zone_reclaim_mode
			#echo 2 | sudo tee /proc/sys/kernel/numa_balancing
			#echo 30 | sudo tee /proc/sys/kernel/numa_balancing_rate_limit_mbps
            
            info "Clear damon param..."
            sudo bash -c "echo '0 0 0 0 0 0 0 0' > /proc/idt_state"

			# set memory tiering
			if [[ $CONFIG_MULTITIER == "yes" ]]; then
				func_config_idt_multitier
			else
				func_config_idt_twotier
			fi

            ;;
        *)
            err "Usage: invalid parameter $CONFIG_IDT for IDT"
            func_err
            ;;
    esac
}

function func_set_damon_attrs() {
    info "Set DAMON attributes..."
    # sampling interval, aggregation interval, region update interval, # min regions, # max regions
    # micro second unit

	info "Sampl: ${SAMPL} us, Aggr: ${AGGR} us"
    info "RgUp: ${RG_UP}, MinRg: ${MIN_RG}, MaxRg: ${MAX_RG}" 
    sudo bash -c "echo '${SAMPL} ${AGGR} ${RG_UP} ${MIN_RG} ${MAX_RG}' > /sys/kernel/debug/damon/attrs"
    sleep 1

	info "Set idt demotion numa node..."
	NODE0_NEXT_NODE=1
	NODE1_NEXT_NODE=2
	NODE2_NEXT_NODE=3
	NODE3_NEXT_NODE=-1
	echo "${NODE0_NEXT_NODE} ${NODE1_NEXT_NODE} ${NODE2_NEXT_NODE} ${NODE3_NEXT_NODE}" > /proc/idt_node
	echo "${NODE0_NEXT_NODE} ${NODE1_NEXT_NODE} ${NODE2_NEXT_NODE} ${NODE3_NEXT_NODE}" > /proc/idt_node
	cat /proc/idt_node
	sleep 1

    info "Set default DAMOS demote scheme..."
    NODE0_MIN_AGE=10
    NODE1_MIN_AGE=10
    NODE2_MIN_AGE=10
    NODE3_MIN_AGE=10
    echo "${NODE0_MIN_AGE} ${NODE1_MIN_AGE} ${NODE2_MIN_AGE} ${NODE3_MIN_AGE}" > /proc/idt_action
    echo "${NODE0_MIN_AGE} ${NODE1_MIN_AGE} ${NODE2_MIN_AGE} ${NODE3_MIN_AGE}" > /proc/idt_action
	cat /proc/idt_action
    sleep 1
}

# configure idt after run
function func_config_pid_idt() {
    info "Config IDT pid ${CONFIG_IDT}..."
    
    case $CONFIG_IDT in
        [Nn][Oo]) # IDT OFF
            ;;
        [Yy][Ee][Ss]) # IDT ON
            # register to damon
            info "Register ${PID} to damon/target_ids..."
            sudo bash -c "echo '${PID}' > /sys/kernel/debug/damon/target_ids"
            sudo bash -c "echo 'on' > /sys/kernel/debug/damon/monitor_on"
			#echo ${PID} >> multi_pid.log
			#func_config_last_program
            ;;
        *)
            err "Usage: invalid parameter $CONFIG_IDT for IDT"
            func_err
            ;;
    esac
}

# configure two-tiered idt
function func_config_idt_twotier() {
	echo 1 -1 -1 -1 > /proc/idt_node
	echo 2 ${DEMOTION_WATERMARK} ${PROMOTION_WATERMARK} > /proc/idt_config
	
	info "Set idt demotion numa node..."
	cat /prot/idt_node
}

# configure multi-tiered idt
function func_config_idt_multitier() {
	echo 1 2 3 -1 > /proc/idt_node
	echo 4 ${DEMOTION_WATERMARK} ${PROMOTION_WATERMARK} > /proc/idt_config

	info "Set idt demotion numa node..."
	cat /prot/idt_node
}

# config idt only for the last program
function func_config_last_program() {
	case $CONFIG_LAST in
        [Nn][Oo]) # IDT Last OFF
			;;
        [Yy][Ee][Ss]) # IDT Last ON
			info "Register to damon..."
			cd /scale/cal/home-interns/jschang0215/IDT-Experiment
			tr '\n' ' ' < multi_pid.log > multi_pid_modified.log;
			sudo bash -c "cat multi_pid_modified.log > /sys/kernel/debug/damon/target_ids";
			sudo bash -c "echo 'on' > /sys/kernel/debug/damon/monitor_on";
            ;;
        *)
            func_err
            ;;
    esac
}
