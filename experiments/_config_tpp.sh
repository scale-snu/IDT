#!/bin/basu

# configure IntelTiering
function func_config_tpp() {
    info "Config tpp ${CONFIG_TPP}..."
    
    case $CONFIG_TPP in
        [Nn][Oo])
            ;;
        [Yy][Ee][Ss]) # TPP On
						sudo bash -c "echo true > /sys/kernel/mm/numa/demotion_enabled"
						sudo sysctl kernel.numa_balancing=2

						# should change once before setting var
						sudo sysctl vm.demote_scale_factor=300

						# TPP-2
						sudo sysctl vm.demote_scale_factor=200

						# TPP-50
						#sudo sysctl vm.demote_scale_factor=5000
						
						info "TPP demotion path"
						cat /proc/zoneinfo | grep toptier
            ;;
        *)
            err "Usage: invalid parameter $CONFIG_TPP for TPP"
            func_err
            ;;
    esac
}
