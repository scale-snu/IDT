#!/bin/basu

# configure autonuma
function func_config_autonuma() {
    local NUMA_OPT=$1

    info "Config autonuma ${NUMA_OPT}..."

    case $NUMA_OPT in
        [Oo][Ff][Ff]) # AutoNUMA balancing OFF
            sudo sysctl kernel.numa_balancing=0
            ;;
        [Aa][Nn]) # AutoNUMA balancing ON
            sudo sysctl kernel.numa_balancing=1
            ;;
		[Mm][Tt]) # Memory tiering ON

			info "AutoNUMA memory tiering..."

			echo 15 | sudo tee /proc/sys/vm/zone_reclaim_mode
			echo 2 | sudo tee /proc/sys/kernel/numa_balancing
			echo 30 | sudo tee /proc/sys/kernel/numa_balancing_rate_limit_mbps
			;;
        *)
            err "Usage: {OFF|AN|MT} are available numa_balancing modes"
            func_err
            ;;
    esac
}
