#!/bin/bash
#
# You may need to add following configuration
# GRUB_CMDLINE_LINUX_DEFAULT="intel_pstate=disable intel_idle.max_cstate=0 processor.max_cstate=1"
#
NUM_CPUS=`grep -c ^processor /proc/cpuinfo`
MAX_FREQ=`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq`

echo "Setting the CPU frequency to $MAX_FREQ"

for (( c=0; c<$NUM_CPUS; c++ ))
do
     echo $MAX_FREQ | sudo tee /sys/devices/system/cpu/cpu$c/cpufreq/scaling_min_freq
done

