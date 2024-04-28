TUNE=base
LABEL=gcc-baseline-o2-m64
NUMBER=638
NAME=imagick_s
SOURCES= coders/tga.c filters/analyze.c magick/accelerate.c \
	 magick/animate.c magick/annotate.c magick/artifact.c magick/attribute.c \
	 magick/blob.c magick/cache-view.c magick/cache.c magick/channel.c \
	 magick/cipher.c magick/client.c magick/coder.c magick/color.c \
	 magick/colormap.c magick/colorspace.c magick/compare.c \
	 magick/composite.c magick/compress.c magick/configure.c \
	 magick/constitute.c magick/decorate.c magick/delegate.c magick/display.c \
	 magick/distort.c magick/distribute-cache.c magick/draw.c magick/effect.c \
	 magick/enhance.c magick/exception.c magick/feature.c magick/fourier.c \
	 magick/fx.c magick/gem.c magick/geometry.c magick/hashmap.c \
	 magick/histogram.c magick/identify.c magick/image.c magick/layer.c \
	 magick/list.c magick/locale.c magick/log.c magick/magic.c \
	 magick/magick.c magick/matrix.c magick/memory.c magick/mime.c \
	 magick/module.c magick/monitor.c magick/montage.c magick/morphology.c \
	 magick/option.c magick/paint.c magick/pixel.c magick/policy.c \
	 magick/prepress.c magick/profile.c magick/property.c magick/quantize.c \
	 magick/quantum-export.c magick/quantum-import.c magick/quantum.c \
	 magick/random.c magick/registry.c magick/resample.c magick/resize.c \
	 magick/resource.c magick/segment.c magick/semaphore.c magick/shear.c \
	 magick/signature.c magick/splay-tree.c magick/static.c \
	 magick/statistic.c magick/stream.c magick/string.c magick/threshold.c \
	 magick/timer.c magick/token.c magick/transform.c magick/type.c \
	 magick/utility.c magick/version.c magick/xml-tree.c utilities/convert.c \
	 wand/convert.c wand/drawing-wand.c wand/magick-image.c \
	 wand/magick-wand.c wand/mogrify.c wand/pixel-wand.c \
	 wand/magick-property.c wand/pixel-iterator.c wand/wand.c \
	 magick/deprecate.c
EXEBASE=imagick_s
NEED_MATH=yes
BENCHLANG=C

BENCH_FLAGS      = -I.
CC               = $(SPECLANG)gcc   -m64   -std=c11
CC_VERSION_OPTION = -v
CPORTABILITY     = 
CXX              = $(SPECLANG)g++  -m64
CXX_VERSION_OPTION = -v
EXTRA_OPTIMIZE   = -fopenmp -DSPEC_OPENMP
EXTRA_PORTABILITY = -DSPEC_LP64
FC               = $(SPECLANG)gfortran    -m64
FC_VERSION_OPTION = -v
OPTIMIZE         = -g -O2 -fopt-info
OS               = unix
SPECLANG         = /usr/bin/
absolutely_no_locking = 0
abstol           = 
action           = build
allow_label_override = 0
backup_config    = 1
baseexe          = imagick_s
basepeak         = 0
benchdir         = benchspec
benchmark        = 638.imagick_s
binary           = 
bindir           = exe
builddir         = build
bundleaction     = 
bundlename       = 
calctol          = 1
changedhash      = 0
check_version    = 0
clean_between_builds = no
command_add_redirect = 1
commanderrfile   = speccmds.err
commandexe       = imagick_s_base.gcc-baseline-o2-m64
commandfile      = speccmds.cmd
commandoutfile   = speccmds.out
commandstdoutfile = speccmds.stdout
comparedir       = compare
compareerrfile   = compare.err
comparefile      = compare.cmd
compareoutfile   = compare.out
comparestdoutfile = compare.stdout
compile_error    = 0
compwhite        = 
configdir        = config
configfile       = gcc-baseline
configpath       = /home/ybmoon/workloads/cpu2017/config/gcc-baseline.cfg
copies           = 1
current_range    = 
datadir          = data
default_size     = ref
default_submit   = $command
delay            = 0
deletebinaries   = 0
deletework       = 0
dependent_workloads = 0
device           = 
difflines        = 10
dirprot          = 511
discard_power_samples = 0
enable_monitor   = 1
endian           = 12345678
env_vars         = 0
expand_notes     = 0
expid            = 
exthash_bits     = 256
failflags        = 0
fake             = 0
feedback         = 1
flag_url_base    = https://www.spec.org/auto/cpu2017/Docs/benchmarks/flags/
floatcompare     = 
force_monitor    = 0
from_runcpu      = 2
hostname         = scal22
http_proxy       = 
http_timeout     = 30
hw_cpu_name      = Intel Xeon Platinum 8260
hw_disk          = 886 GB  add more disk info here
hw_memory001     = 625.370 GB fixme: If using DDR3, format is:
hw_memory002     = 'N GB (M x N GB nRxn PCn-nnnnnR-n, ECC)'
hw_nchips        = 2
hw_vendor        = SCALable Computer Architecture Laboratory at Seoul National University
idle_current_range = 
idledelay        = 10
idleduration     = 60
ignore_errors    = 1
ignore_sigint    = 0
ignorecase       = 
info_wrap_columns = 50
inputdir         = input
inputgenerrfile  = inputgen.err
inputgenfile     = inputgen.cmd
inputgenoutfile  = inputgen.out
inputgenstdoutfile = inputgen.stdout
iteration        = -1
iterations       = 1
keeptmp          = 0
label            = gcc-baseline-o2-m64
line_width       = 1020
link_input_files = 1
locking          = 1
log              = CPU2017
log_line_width   = 1020
log_timestamp    = 0
logfile          = /home/ybmoon/workloads/cpu2017/tmp/CPU2017.013/templogs/preenv.fpspeed.013.0
logname          = /home/ybmoon/workloads/cpu2017/tmp/CPU2017.013/templogs/preenv.fpspeed.013.0
lognum           = 013.0
mail_reports     = all
mailcompress     = 0
mailmethod       = smtp
mailport         = 25
mailserver       = 127.0.0.1
mailto           = 
make             = specmake
make_no_clobber  = 0
makefile_template = Makefile.YYYtArGeTYYYspec
makeflags        = --jobs=16
max_average_uncertainty = 1
max_hum_limit    = 0
max_report_runs  = 3
max_unknown_uncertainty = 1
mean_anyway      = 1
meter_connect_timeout = 30
meter_errors_default = 5
meter_errors_percentage = 5
min_report_runs  = 2
min_temp_limit   = 20
minimize_builddirs = 0
minimize_rundirs = 0
name             = imagick_s
nansupport       = 
need_math        = yes
no_input_handler = close
no_monitor       = 
noratios         = 0
note_preenv      = 1
notes_plat_sysinfo_000 =  Sysinfo program /home/ybmoon/workloads/cpu2017/bin/sysinfo
notes_plat_sysinfo_005 =  Rev: r5797 of 2017-06-14 96c45e4568ad54c135fd618bcc091c0f
notes_plat_sysinfo_010 =  running on scal22 Wed Jan 11 13:24:46 2023
notes_plat_sysinfo_015 = 
notes_plat_sysinfo_020 =  SUT (System Under Test) info as seen by some common utilities.
notes_plat_sysinfo_025 =  For more information on this section, see
notes_plat_sysinfo_030 =     https://www.spec.org/cpu2017/Docs/config.html\#sysinfo
notes_plat_sysinfo_035 = 
notes_plat_sysinfo_040 =  From /proc/cpuinfo
notes_plat_sysinfo_045 =     model name : Intel(R) Xeon(R) Platinum 8260 CPU @ 2.40GHz
notes_plat_sysinfo_050 =        2  "physical id"s (chips)
notes_plat_sysinfo_055 =        48 "processors"
notes_plat_sysinfo_060 =     cores, siblings (Caution: counting these is hw and system dependent. The following
notes_plat_sysinfo_065 =     excerpts from /proc/cpuinfo might not be reliable.  Use with caution.)
notes_plat_sysinfo_070 =        cpu cores : 24
notes_plat_sysinfo_075 =        siblings  : 24
notes_plat_sysinfo_080 =        physical 0: cores 0 1 2 3 4 5 6 8 10 11 12 13 16 17 18 19 20 21 24 25 26 27 28 29
notes_plat_sysinfo_085 =        physical 1: cores 0 1 2 3 4 5 6 8 9 10 11 12 13 16 17 18 19 20 21 25 26 27 28 29
notes_plat_sysinfo_090 = 
notes_plat_sysinfo_095 =  From lscpu:
notes_plat_sysinfo_100 =       Architecture:          x86_64
notes_plat_sysinfo_105 =       CPU op-mode(s):        32-bit, 64-bit
notes_plat_sysinfo_110 =       Byte Order:            Little Endian
notes_plat_sysinfo_115 =       CPU(s):                48
notes_plat_sysinfo_120 =       On-line CPU(s) list:   0-47
notes_plat_sysinfo_125 =       Thread(s) per core:    1
notes_plat_sysinfo_130 =       Core(s) per socket:    24
notes_plat_sysinfo_135 =       Socket(s):             2
notes_plat_sysinfo_140 =       NUMA node(s):          4
notes_plat_sysinfo_145 =       Vendor ID:             GenuineIntel
notes_plat_sysinfo_150 =       CPU family:            6
notes_plat_sysinfo_155 =       Model:                 85
notes_plat_sysinfo_160 =       Model name:            Intel(R) Xeon(R) Platinum 8260 CPU @ 2.40GHz
notes_plat_sysinfo_165 =       Stepping:              7
notes_plat_sysinfo_170 =       CPU MHz:               1000.000
notes_plat_sysinfo_175 =       CPU max MHz:           3900.0000
notes_plat_sysinfo_180 =       CPU min MHz:           1000.0000
notes_plat_sysinfo_185 =       BogoMIPS:              4801.60
notes_plat_sysinfo_190 =       L1d cache:             32K
notes_plat_sysinfo_195 =       L1i cache:             32K
notes_plat_sysinfo_200 =       L2 cache:              1024K
notes_plat_sysinfo_205 =       L3 cache:              36608K
notes_plat_sysinfo_210 =       NUMA node0 CPU(s):
notes_plat_sysinfo_215 =       0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46
notes_plat_sysinfo_220 =       NUMA node1 CPU(s):
notes_plat_sysinfo_225 =       1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47
notes_plat_sysinfo_230 =       NUMA node2 CPU(s):
notes_plat_sysinfo_235 =       NUMA node3 CPU(s):
notes_plat_sysinfo_240 =       Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov
notes_plat_sysinfo_245 =       pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp
notes_plat_sysinfo_250 =       lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid
notes_plat_sysinfo_255 =       aperfmperf pni pclmulqdq dtes64 monitor ds_cpl smx est tm2 ssse3 sdbg fma cx16 xtpr
notes_plat_sysinfo_260 =       pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx
notes_plat_sysinfo_265 =       f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 invpcid_single
notes_plat_sysinfo_270 =       intel_ppin ssbd mba ibrs ibpb stibp ibrs_enhanced fsgsbase tsc_adjust bmi1 hle avx2
notes_plat_sysinfo_275 =       smep bmi2 erms invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt
notes_plat_sysinfo_280 =       clwb intel_pt avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc
notes_plat_sysinfo_285 =       cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts pku ospke
notes_plat_sysinfo_290 =       avx512_vnni md_clear flush_l1d arch_capabilities
notes_plat_sysinfo_295 = 
notes_plat_sysinfo_300 =  /proc/cpuinfo cache data
notes_plat_sysinfo_305 =     cache size : 36608 KB
notes_plat_sysinfo_310 = 
notes_plat_sysinfo_315 =  From numactl --hardware  WARNING: a numactl 'node' might or might not correspond to a
notes_plat_sysinfo_320 =  physical chip.
notes_plat_sysinfo_325 =    available: 4 nodes (0-3)
notes_plat_sysinfo_330 =    node 0 cpus: 0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46
notes_plat_sysinfo_335 =    node 0 size: 63901 MB
notes_plat_sysinfo_340 =    node 0 free: 55581 MB
notes_plat_sysinfo_345 =    node 1 cpus: 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47
notes_plat_sysinfo_350 =    node 1 size: 64476 MB
notes_plat_sysinfo_355 =    node 1 free: 60123 MB
notes_plat_sysinfo_360 =    node 2 cpus:
notes_plat_sysinfo_365 =    node 2 size: 256000 MB
notes_plat_sysinfo_370 =    node 2 free: 256000 MB
notes_plat_sysinfo_375 =    node 3 cpus:
notes_plat_sysinfo_380 =    node 3 size: 256000 MB
notes_plat_sysinfo_385 =    node 3 free: 256000 MB
notes_plat_sysinfo_390 =    node distances:
notes_plat_sysinfo_395 =    node   0   1   2   3
notes_plat_sysinfo_400 =      0:  10  21  17  28
notes_plat_sysinfo_405 =      1:  21  10  28  17
notes_plat_sysinfo_410 =      2:  17  28  10  28
notes_plat_sysinfo_415 =      3:  28  17  28  10
notes_plat_sysinfo_420 = 
notes_plat_sysinfo_425 =  From /proc/meminfo
notes_plat_sysinfo_430 =     MemTotal:       655747608 kB
notes_plat_sysinfo_435 =     HugePages_Total:       0
notes_plat_sysinfo_440 =     Hugepagesize:       2048 kB
notes_plat_sysinfo_445 = 
notes_plat_sysinfo_450 =  /usr/bin/lsb_release -d
notes_plat_sysinfo_455 =     Ubuntu 16.04.6 LTS
notes_plat_sysinfo_460 = 
notes_plat_sysinfo_465 =  From /etc/*release* /etc/*version*
notes_plat_sysinfo_470 =     debian_version: stretch/sid
notes_plat_sysinfo_475 =     os-release:
notes_plat_sysinfo_480 =        NAME="Ubuntu"
notes_plat_sysinfo_485 =        VERSION="16.04.6 LTS (Xenial Xerus)"
notes_plat_sysinfo_490 =        ID=ubuntu
notes_plat_sysinfo_495 =        ID_LIKE=debian
notes_plat_sysinfo_500 =        PRETTY_NAME="Ubuntu 16.04.6 LTS"
notes_plat_sysinfo_505 =        VERSION_ID="16.04"
notes_plat_sysinfo_510 =        HOME_URL="http://www.ubuntu.com/"
notes_plat_sysinfo_515 =        SUPPORT_URL="http://help.ubuntu.com/"
notes_plat_sysinfo_520 = 
notes_plat_sysinfo_525 =  uname -a:
notes_plat_sysinfo_530 =     Linux scal22 5.13.0-ADTPP \#7 SMP Mon Jan 9 15:32:58 KST 2023 x86_64 x86_64 x86_64
notes_plat_sysinfo_535 =     GNU/Linux
notes_plat_sysinfo_540 = 
notes_plat_sysinfo_545 =  run-level 5 Jan 9 16:18
notes_plat_sysinfo_550 = 
notes_plat_sysinfo_555 =  SPEC is set to: /home/ybmoon/workloads/cpu2017
notes_plat_sysinfo_560 =     Filesystem           Type  Size  Used Avail Use% Mounted on
notes_plat_sysinfo_565 =     /dev/mapper/lvm-root ext4  886G  723G  119G  86% /
notes_plat_sysinfo_570 = 
notes_plat_sysinfo_575 =  Additional information from dmidecode follows.  WARNING: Use caution when you interpret
notes_plat_sysinfo_580 =  this section. The 'dmidecode' program reads system data which is "intended to allow
notes_plat_sysinfo_585 =  hardware to be accurately determined", but the intent may not be met, as there are
notes_plat_sysinfo_590 =  frequent changes to hardware, firmware, and the "DMTF SMBIOS" standard.
notes_plat_sysinfo_595 = 
notes_plat_sysinfo_600 =  (End of data from sysinfo program)
notes_wrap_columns = 0
notes_wrap_indent =   
num              = 638
obiwan           = 
os_exe_ext       = 
output_format    = txt,html,cfg,pdf,csv
output_root      = 
outputdir        = output
parallel_test    = 0
parallel_test_submit = 0
parallel_test_workloads = 
path             = /home/ybmoon/workloads/cpu2017/benchspec/CPU/638.imagick_s
plain_train      = 1
platform         = 
power            = 0
preENV_LD_LIBRARY_PATH = %{gcc_dir}/lib64/:%{gcc_dir}/lib/:/lib64
preENV_OMP_STACKSIZE = 120M
preenv           = 0
prefix           = 
prepared_by      = ybmoon  (is never output, only tags rawfile)
ranks            = 1
rawhash_bits     = 256
rebuild          = 0
reftime          = reftime
reltol           = 0.01
reportable       = 0
resultdir        = result
review           = 0
run              = all
runcpu           = /home/ybmoon/workloads/cpu2017/bin/harness/runcpu --action build --configfile gcc-baseline --noreportable --nopower --runmode speed --tune base --size refspeed 638.imagick_s --nopreenv --note-preenv --logfile /home/ybmoon/workloads/cpu2017/tmp/CPU2017.013/templogs/preenv.fpspeed.013.0 --lognum 013.0 --from_runcpu 2
rundir           = run
runmode          = speed
safe_eval        = 1
save_build_files = 
section_specifier_fatal = 1
setprocgroup     = 1
setup_error      = 0
sigint           = 2
size             = refspeed
size_class       = ref
skipabstol       = 
skipobiwan       = 
skipreltol       = 
skiptol          = 
smarttune        = base
specdiff         = specdiff
specrun          = specinvoke
srcalt           = 
srcdir           = src
srcsource        = /home/ybmoon/workloads/cpu2017/benchspec/CPU/538.imagick_r/src
stagger          = 10
strict_rundir_verify = 0
sw_base_ptrsize  = 64-bit
sw_compiler001   = C/C++/Fortran: Version 6.2.0 of GCC, the
sw_compiler002   = GNU Compiler Collection
sw_file          = ext4
sw_os001         = Ubuntu 16.04.6 LTS
sw_os002         = 5.13.0-ADTPP
sw_peak_ptrsize  = 64-bit
sw_state         = Run level 5 (add definition here)
sysinfo_hash_bits = 256
sysinfo_program  = specperl /home/ybmoon/workloads/cpu2017/bin/sysinfo
sysinfo_program_hash = sysinfo:SHA:ecd2bef08f316af97f5a7768b641e2a3307c1b4b68efb5a57fa76367d790d233
table            = 1
teeout           = 0
test_date        = Jan-2023
test_sponsor     = SCALable Computer Architecture Laboratory at Seoul National University
tester           = Deok-Jae Oh
threads          = 1
top              = /home/ybmoon/workloads/cpu2017
train_single_thread = 0
train_with       = train
tune             = base
uid              = 1022
unbuffer         = 1
uncertainty_exception = 5
update           = 0
update_url       = http://www.spec.org/auto/cpu2017/updates/
use_submit_for_compare = 0
use_submit_for_speed = 0
username         = ybmoon
verbose          = 5
verify_binaries  = 1
version          = 0.903000
version_url      = http://www.spec.org/auto/cpu2017/current_version
voltage_range    = 
worklist         = list
OUTPUT_RMFILES   = train_validate.out
