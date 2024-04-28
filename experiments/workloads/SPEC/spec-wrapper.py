#!/usr/bin/env python

import math
import sys
import subprocess
import os
import getopt

cur_path = os.getcwd()

second_wrapper = cur_path + "/workloads/SPEC/second_wrapper"

app_path_base = cur_path + "/workloads/SPEC/app/"

spec_config="gcc-baseline-o2"
spec_tune="base"
spec_apps={"603": "603.bwaves_s", "607": "607.cactuBSSN_s", "631": "631.deepsjeng_s", "638": "638.imagick_s", "654": "654.roms_s", "657": "657.xz_s"}
spec_args={"603": " $ bwaves_1.in ", "607": " spec_ref.par ", "631": " ref.txt ", "638": " -limit disk 0 refspeed_input.tga -resize 817% -rotate -2.76 -shave 540x375 -alpha remove -auto-level -contrast-stretch 1x1% -colorspace Lab -channel R -equalize +channel -colorspace sRGB -define histogram:unique-colors=false -adaptive-blur 0x5 -despeckle -auto-gamma -adaptive-sharpen 55 -enhance -brightness-contrast 10x10 -resize 30% refspeed_output.tga ", "654": " $ ocean_benchmark3.in.x ", "657": " cpu2006docs.tar.xz 6643 055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae 1036078272 1111795472 4 "}
spec_bins={"603": "speed_bwaves", "607": "cactuBSSN_s", "631": "deepsjeng_s", "638": "imagick_s", "654": "sroms", "657": "xz_s"}
apppath=""
inputarg=""
apparg=""
appbin=""
appcmd=""


## Edit following parameters
# # # of programs to execute
# rate="8"
# # Memory allocation node - DRAM0: 0, DRAM1: 1, DCPMM0: 2, DCPMM1: 3
# memory_node = "0"
# # Application index in SPEC2017 (speed only)
# appIndex = ["631"]

def main(argv):
    appIndex = sys.argv[1]
    memorynode = sys.argv[2]
    rate = sys.argv[3]
    full_cmd = "" 
    apppath = app_path_base+spec_apps[appIndex]+"/run/run_base_refspeed_"+spec_config+"-m64.0000/"
    appbin = apppath+spec_bins[appIndex]+"_"+spec_tune+"."+spec_config+"-m64"
    full_cmd = second_wrapper+" "+memorynode+" "+rate+" "+apppath+" "+appbin+" "+spec_args[appIndex]
    print("Full Command: ",full_cmd)
    full_cmd = full_cmd.split()
    subprocess.call(full_cmd) # run converting


if __name__ == "__main__":
    main(sys.argv[1:])
