#!/bin/bash 


ndctl disable-namespace namespace1.0

ndctl disable-namespace namespace0.0

ndctl destroy-namespace namespace0.0

ndctl destroy-namespace namespace1.0

ndctl disable-region all

daxctl migrate-device-model

lsmod | grep dax 


modprobe -r dax_pmem_compat

lsmod | grep dax

ndctl enable-region all

ipmctl show -region


ndctl create-namespace --mode devdax --map mem 
ndctl create-namespace --mode devdax --map mem 

daxctl reconfigure-device --mode=system-ram --region=0 all

daxctl reconfigure-device --mode=system-ram --region=1 all

numactl -H
