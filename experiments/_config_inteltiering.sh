#!/bin/basu

# configure IntelTiering
function func_config_inteltiering() {
    info "Config intel tiering ${CONFIG_INTELTIERING}..."
    
    case $CONFIG_INTELTIERING in
        [Nn][Oo]) # Memory tiering OFF
            ;;
        [Yy][Ee][Ss]) # Memory tiering ON
            echo 1 > /sys/kernel/mm/numa/demotion_enabled
            echo 2 | sudo tee /proc/sys/kernel/numa_balancing
            echo 30 | sudo tee /proc/sys/kernel/numa_balancing_rate_limit_mbps

            ;;
        *)
            err "Usage: invalid parameter $CONFIG_INTELTIERING for Intel Tiering"
            func_err
            ;;
    esac
}
