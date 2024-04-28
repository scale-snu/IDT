#!/bin/basu

# configure autotiering (opm)
function func_config_autotiering() {
    info "Config autotiering (OPM-X) ${CONFIG_AUTOTIERING}..."
    
    if [[ $CONFIG_AUTOTIERING == "yes" ]]; then
		CHECK_EXCH=$(ls /proc/sys/kernel/numa_balancing* | grep extended)
		if [[ -z ${CHECK_EXCH} ]]; then
			err "Numa_balancing extended is not supported!"
			func_err
		fi

		if [[ $AUTONUMA == "OFF" ]]; then
			err "Autotiering works only with AN enabled"
			func_err
		fi
		mode=0

		sudo sysctl kernel.perf_event_max_sample_rate=1

		info "Set autotiering OPM..."
    	sudo sysctl kernel.numa_balancing_scan_period_max_ms=1000
    	sudo sysctl kernel.numa_balancing_scan_size_mb=2048
		mode=$(($mode | 1<<1))

    	info "Set OPM (OPM-FG)..."
    	echo 0 | sudo tee /sys/kernel/mm/page_balancing/background_demotion > /dev/null
    	echo 0 | sudo tee /sys/kernel/mm/page_balancing/batch_demotion > /dev/null

    	# use OPM-X; since OPM-BD crashes...
    	info "Set Exchange (OPM-X)..."
		mode=$(($mode | 1<<2))

    	info "Set extended numa balancing..."
    	sudo sysctl kernel.numa_balancing_extended=$mode

		if [[ $CONFIG_MULTITIER == "yes" ]]; then
			func_config_autotiering_multitier
		else
			func_config_autotiering_twotier
		fi
    fi
}

# configure two-tiered autotiering
function func_config_autotiering_twotier() {
	info "Config autotiering two-tier..."

	sudo bash -c "echo -1 > /sys/devices/system/node/node0/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node0/promotion_path"
	sudo bash -c "echo 2 > /sys/devices/system/node/node0/demotion_path"

	sudo bash -c "echo -1 > /sys/devices/system/node/node1/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node1/promotion_path"
	sudo bash -c "echo 3 > /sys/devices/system/node/node1/demotion_path"
	
	sudo bash -c "echo -1 > /sys/devices/system/node/node2/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node2/promotion_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node2/demotion_path"
	
	sudo bash -c "echo -1 > /sys/devices/system/node/node3/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node3/promotion_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node3/demotion_path"
}

# configure multi-tiered autotiering
function func_config_autotiering_multitier() {
	info "Config autotiering multi-tier..."

	sudo bash -c "echo 1 > /sys/devices/system/node/node0/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node0/promotion_path"
	sudo bash -c "echo 2 > /sys/devices/system/node/node0/demotion_path"

	sudo bash -c "echo 0 > /sys/devices/system/node/node1/migration_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node1/promotion_path"
	sudo bash -c "echo 3 > /sys/devices/system/node/node1/demotion_path"
	
	sudo bash -c "echo -1 > /sys/devices/system/node/node2/migration_path"
	sudo bash -c "echo 0 > /sys/devices/system/node/node2/promotion_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node2/demotion_path"
	
	sudo bash -c "echo -1 > /sys/devices/system/node/node3/migration_path"
	sudo bash -c "echo 1 > /sys/devices/system/node/node3/promotion_path"
	sudo bash -c "echo -1 > /sys/devices/system/node/node3/demotion_path"
}
