#!/usr/bin/env python

import math
import sys
import subprocess
import os
import getopt

cur_path = os.getcwd()

second_wrapper = cur_path  + "/workloads/GAPBS/second_wrapper"
app_path_base = cur_path + "/workloads/GAPBS/bin/"
input_path_base = cur_path + "/workloads/GAPBS/gapbs-data/"

gapbs_apps={"bc": "bc", "cc": "cc", "pr": "pr", "bfs": "bfs", "sssp": "sssp", "bfs": "bfs", "sssp": "sssp", "tc": "tc", "malloc_sort": "malloc_sort"}
gapbs_args={"bc": "-i4 -n16", "cc": "-n16", "pr": "-i1000 -t1e-4 -n16", "bfs": "-n64", "sssp": "-d2", "tc": "-n3", "malloc_sort": ""}
apppath=""
inputarg=""
apparg=""
appbin=""
appcmd=""

## Edit following parameters
# # of programs to execute
#rate="4"
# Memory Allocation Node - DRAM0: 0, DRAM1: 1, DCPMM0: 2, DCPMM1: 3
#memory_node = "0"
# App index to execute. bc/cc/pr/bfs...
#appIndex = ["cc"]
# Input graph to calculate. kron/twitter/web...

def main(argv):
    appIndex = sys.argv[1]
    inputIndex = sys.argv[2]
    memorynode = sys.argv[3]
    rate = sys.argv[4]
    full_cmd = "" 
    inputarg = input_path_base+inputIndex+".sg"
    apppath = app_path_base
    appbin = app_path_base+gapbs_apps[appIndex]
    apparg = "-f "+inputarg+" "+gapbs_args[appIndex]
    full_cmd = second_wrapper+" "+memorynode+" "+rate+" "+apppath+" "+appbin+" "+apparg
    print("Full Command: ",full_cmd)
    full_cmd = full_cmd.split()
    subprocess.call(full_cmd) # run converting


if __name__ == "__main__":
    main(sys.argv[1:])
