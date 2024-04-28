TUNE=base
LABEL=gcc-baseline-o2-m64
NUMBER=607
NAME=cactuBSSN_s
SOURCES= gnu/getopt.c gnu/getopt1.c gnu/regex.c ADMBase/InitSymBound.c \
	 ADMBase/Initialisation.c ADMBase/ParamCheck.c ADMBase/Static.c \
	 ADMBase/cctk_Bindings/cctk_ThornBindings.c \
	 ADMCoupling/cctk_Bindings/cctk_ThornBindings.c \
	 ADMMacros/SetLocalSpatialOrder.c \
	 ADMMacros/cctk_Bindings/cctk_ThornBindings.c Boundary/Boundary.c \
	 Boundary/Check.c Boundary/CopyBoundary.c Boundary/FlatBoundary.c \
	 Boundary/NoneBoundary.c Boundary/RadiationBoundary.c Boundary/Register.c \
	 Boundary/RobinBoundary.c Boundary/ScalarBoundary.c \
	 Boundary/StaticBoundary.c Boundary/cctk_Bindings/cctk_ThornBindings.c \
	 Cactus/IO/FortranBindingsIO.c Cactus/IO/IOMethods.c \
	 Cactus/IO/OverloadIO.c Cactus/cctk_Bindings/cctk_ThornBindings.c \
	 Cactus/comm/CactusDefaultComm.c Cactus/comm/CactusSync.c \
	 Cactus/comm/Interp.c Cactus/comm/OverloadComm.c Cactus/comm/Reduction.c \
	 Cactus/main/ActiveThorns.c Cactus/main/Banner.c \
	 Cactus/main/CactusDefaultEvolve.c Cactus/main/CactusDefaultInitialise.c \
	 Cactus/main/CactusDefaultMainLoopIndex.c \
	 Cactus/main/CactusDefaultShutdown.c Cactus/main/CallStartupFunctions.c \
	 Cactus/main/CommandLine.c Cactus/main/Complex.c Cactus/main/ConfigData.c \
	 Cactus/main/Coord.c Cactus/main/DebugDefines.c \
	 Cactus/main/DefaultTimers.c Cactus/main/Dummies.c \
	 Cactus/main/FortranWrappers.c Cactus/main/GHExtensions.c \
	 Cactus/main/Groups.c Cactus/main/GroupsOnGH.c \
	 Cactus/main/InitialiseCactus.c Cactus/main/InitialiseDataStructures.c \
	 Cactus/main/MainUtils.c Cactus/main/Names.c Cactus/main/OverloadMain.c \
	 Cactus/main/Parameters.c Cactus/main/ProcessCommandLine.c \
	 Cactus/main/ProcessEnvironment.c Cactus/main/ProcessParameterDatabase.c \
	 Cactus/main/ScheduleInterface.c Cactus/main/SetParams.c \
	 Cactus/main/SetupCache.c Cactus/main/ShutdownCactus.c \
	 Cactus/main/Subsystems.c Cactus/main/Termination.c \
	 Cactus/main/Traverse.c Cactus/main/WarnLevel.c Cactus/main/flesh.cc \
	 Cactus/piraha/AutoGrammar.cc Cactus/piraha/Boundary.cc \
	 Cactus/piraha/Bracket.cc Cactus/piraha/Call.cc Cactus/piraha/Dot.cc \
	 Cactus/piraha/End.cc Cactus/piraha/Grammar.cc Cactus/piraha/Group.cc \
	 Cactus/piraha/ILiteral.cc Cactus/piraha/Literal.cc \
	 Cactus/piraha/Lookup.cc Cactus/piraha/Matcher.cc Cactus/piraha/Multi.cc \
	 Cactus/piraha/NegLookAhead.cc Cactus/piraha/Or.cc \
	 Cactus/piraha/ReParse.cc Cactus/piraha/Seq.cc Cactus/piraha/Start.cc \
	 Cactus/piraha/smart_ptr.cc Cactus/schedule/ScheduleCreater.c \
	 Cactus/schedule/ScheduleSorter.c Cactus/schedule/ScheduleTraverse.c \
	 Cactus/util/Cache.c Cactus/util/CactusTimers.c Cactus/util/Expression.c \
	 Cactus/util/File.c Cactus/util/Hash.c Cactus/util/Malloc.c \
	 Cactus/util/Math.c Cactus/util/Misc.c Cactus/util/Network.c \
	 Cactus/util/ParseFile.c Cactus/util/RegisterKeyedFunction.c \
	 Cactus/util/SKBinTree.c Cactus/util/StoreHandledData.c \
	 Cactus/util/StoreKeyedData.c Cactus/util/StoreNamedData.c \
	 Cactus/util/String.c Cactus/util/StringList.c Cactus/util/Table.c \
	 Cactus/util/Time.c Cactus/util/snprintf.c \
	 CactusBindings/Functions/ADMBase_Functions.c \
	 CactusBindings/Functions/AliasedFunctions.c \
	 CactusBindings/Functions/Boundary_Functions.c \
	 CactusBindings/Functions/CartGrid3D_Functions.c \
	 CactusBindings/Functions/CoordBase_Functions.c \
	 CactusBindings/Functions/GaugeWave_Functions.c \
	 CactusBindings/Functions/GenericFD_Functions.c \
	 CactusBindings/Functions/IOASCII_Functions.c \
	 CactusBindings/Functions/IOBasic_Functions.c \
	 CactusBindings/Functions/IOUtil_Functions.c \
	 CactusBindings/Functions/IsFunctionAliased.c \
	 CactusBindings/Functions/ML_BSSN_Functions.c \
	 CactusBindings/Functions/ML_BSSN_Helper_Functions.c \
	 CactusBindings/Functions/MoL_Functions.c \
	 CactusBindings/Functions/PUGHInterp_Functions.c \
	 CactusBindings/Functions/PUGHSlab_Functions.c \
	 CactusBindings/Functions/RegisterThornFunctions.c \
	 CactusBindings/Functions/StaticConformal_Functions.c \
	 CactusBindings/Functions/SymBase_Functions.c \
	 CactusBindings/Implementations/ImplementationBindings.c \
	 CactusBindings/Parameters/ADMBase_Parameters.c \
	 CactusBindings/Parameters/ADMCoupling_Parameters.c \
	 CactusBindings/Parameters/ADMMacros_Parameters.c \
	 CactusBindings/Parameters/BindingsParameters.c \
	 CactusBindings/Parameters/Boundary_Parameters.c \
	 CactusBindings/Parameters/Cactus_Parameters.c \
	 CactusBindings/Parameters/CartGrid3D_Parameters.c \
	 CactusBindings/Parameters/CoordBase_Parameters.c \
	 CactusBindings/Parameters/CoordGauge_Parameters.c \
	 CactusBindings/Parameters/Fortran_Parameters.c \
	 CactusBindings/Parameters/GaugeWave_Parameters.c \
	 CactusBindings/Parameters/GenericFD_Parameters.c \
	 CactusBindings/Parameters/Global.c \
	 CactusBindings/Parameters/IOASCII_Parameters.c \
	 CactusBindings/Parameters/IOBasic_Parameters.c \
	 CactusBindings/Parameters/IOUtil_Parameters.c \
	 CactusBindings/Parameters/LocalReduce_Parameters.c \
	 CactusBindings/Parameters/ML_BSSN_Helper_Parameters.c \
	 CactusBindings/Parameters/ML_BSSN_Parameters.c \
	 CactusBindings/Parameters/MoL_Parameters.c \
	 CactusBindings/Parameters/PUGHInterp_Parameters.c \
	 CactusBindings/Parameters/PUGHReduce_Parameters.c \
	 CactusBindings/Parameters/PUGHSlab_Parameters.c \
	 CactusBindings/Parameters/PUGH_Parameters.c \
	 CactusBindings/Parameters/StaticConformal_Parameters.c \
	 CactusBindings/Parameters/SymBase_Parameters.c \
	 CactusBindings/Parameters/Time_Parameters.c \
	 CactusBindings/Parameters/TmunuBase_Parameters.c \
	 CactusBindings/Schedule/BindingsParameterRecovery.c \
	 CactusBindings/Schedule/BindingsSchedule.c \
	 CactusBindings/Schedule/ScheduleADMBase.c \
	 CactusBindings/Schedule/ScheduleADMCoupling.c \
	 CactusBindings/Schedule/ScheduleADMMacros.c \
	 CactusBindings/Schedule/ScheduleBoundary.c \
	 CactusBindings/Schedule/ScheduleCactus.c \
	 CactusBindings/Schedule/ScheduleCartGrid3D.c \
	 CactusBindings/Schedule/ScheduleCoordBase.c \
	 CactusBindings/Schedule/ScheduleCoordGauge.c \
	 CactusBindings/Schedule/ScheduleFortran.c \
	 CactusBindings/Schedule/ScheduleGaugeWave.c \
	 CactusBindings/Schedule/ScheduleGenericFD.c \
	 CactusBindings/Schedule/ScheduleIOASCII.c \
	 CactusBindings/Schedule/ScheduleIOBasic.c \
	 CactusBindings/Schedule/ScheduleIOUtil.c \
	 CactusBindings/Schedule/ScheduleLocalReduce.c \
	 CactusBindings/Schedule/ScheduleML_BSSN.c \
	 CactusBindings/Schedule/ScheduleML_BSSN_Helper.c \
	 CactusBindings/Schedule/ScheduleMoL.c \
	 CactusBindings/Schedule/SchedulePUGH.c \
	 CactusBindings/Schedule/SchedulePUGHInterp.c \
	 CactusBindings/Schedule/SchedulePUGHReduce.c \
	 CactusBindings/Schedule/SchedulePUGHSlab.c \
	 CactusBindings/Schedule/ScheduleStaticConformal.c \
	 CactusBindings/Schedule/ScheduleSymBase.c \
	 CactusBindings/Schedule/ScheduleTime.c \
	 CactusBindings/Schedule/ScheduleTmunuBase.c \
	 CactusBindings/Variables/ADMBase.c \
	 CactusBindings/Variables/ADMCoupling.c \
	 CactusBindings/Variables/ADMMacros.c \
	 CactusBindings/Variables/BindingsVariables.c \
	 CactusBindings/Variables/Boundary.c CactusBindings/Variables/Cactus.c \
	 CactusBindings/Variables/CartGrid3D.c \
	 CactusBindings/Variables/CoordBase.c \
	 CactusBindings/Variables/CoordGauge.c CactusBindings/Variables/Fortran.c \
	 CactusBindings/Variables/GaugeWave.c \
	 CactusBindings/Variables/GenericFD.c CactusBindings/Variables/IOASCII.c \
	 CactusBindings/Variables/IOBasic.c CactusBindings/Variables/IOUtil.c \
	 CactusBindings/Variables/LocalReduce.c \
	 CactusBindings/Variables/ML_BSSN.c \
	 CactusBindings/Variables/ML_BSSN_Helper.c CactusBindings/Variables/MoL.c \
	 CactusBindings/Variables/PUGH.c CactusBindings/Variables/PUGHInterp.c \
	 CactusBindings/Variables/PUGHReduce.c \
	 CactusBindings/Variables/PUGHSlab.c \
	 CactusBindings/Variables/StaticConformal.c \
	 CactusBindings/Variables/SymBase.c CactusBindings/Variables/Time.c \
	 CactusBindings/Variables/TmunuBase.c CartGrid3D/CartGrid3D.c \
	 CartGrid3D/DecodeSymParameters.c CartGrid3D/GetSymmetry.c \
	 CartGrid3D/ParamCheck.c CartGrid3D/RegisterSymmetries.c \
	 CartGrid3D/SetSymmetry.c CartGrid3D/Startup.c CartGrid3D/Symmetry.c \
	 CartGrid3D/cctk_Bindings/cctk_ThornBindings.c CoordBase/CoordBase.c \
	 CoordBase/Domain.c CoordBase/GHExtension.c \
	 CoordBase/cctk_Bindings/cctk_ThornBindings.c CoordGauge/Slicing.c \
	 CoordGauge/cctk_Bindings/cctk_ThornBindings.c Fortran/cctk.f90 \
	 Fortran/cctk_ActiveThorns.f90 Fortran/cctk_Banner.f90 \
	 Fortran/cctk_Bindings/cctk_ThornBindings.c Fortran/cctk_Cache.f90 \
	 Fortran/cctk_Comm.f90 Fortran/cctk_CommandLine.f90 \
	 Fortran/cctk_Complex.f90 Fortran/cctk_Constants.f90 \
	 Fortran/cctk_Coord.f90 Fortran/cctk_DebugDefines.f90 \
	 Fortran/cctk_Faces.f90 Fortran/cctk_File.f90 Fortran/cctk_Flesh.f90 \
	 Fortran/cctk_FortranString1.f90 Fortran/cctk_FortranWrappers.f90 \
	 Fortran/cctk_GHExtensions.f90 Fortran/cctk_GNU.f90 \
	 Fortran/cctk_Groups.f90 Fortran/cctk_GroupsOnGH.f90 Fortran/cctk_IO.f90 \
	 Fortran/cctk_IOMethods.f90 Fortran/cctk_Interp.f90 Fortran/cctk_Main.f90 \
	 Fortran/cctk_Malloc1.f90 Fortran/cctk_Math.f90 Fortran/cctk_MemAlloc.f90 \
	 Fortran/cctk_Misc.f90 Fortran/cctk_ParamCheck.f90 \
	 Fortran/cctk_Parameter.f90 Fortran/cctk_Reduction.f90 \
	 Fortran/cctk_Schedule.f90 Fortran/cctk_Sync.f90 \
	 Fortran/cctk_Termination.f90 Fortran/cctk_Timers.f90 \
	 Fortran/cctk_Types.f90 Fortran/cctk_Version.f90 \
	 Fortran/cctk_WarnLevel.f90 Fortran/paramcheck.f90 Fortran/util_Table.f90 \
	 GaugeWave/Boundaries.cc GaugeWave/GaugeWave_always.cc \
	 GaugeWave/GaugeWave_initial.cc GaugeWave/ParamCheck.cc \
	 GaugeWave/RegisterMoL.cc GaugeWave/RegisterSymmetries.cc \
	 GaugeWave/Startup.cc GaugeWave/cctk_Bindings/cctk_ThornBindings.c \
	 GenericFD/GenericFD.c GenericFD/cctk_Bindings/cctk_ThornBindings.c \
	 IOASCII/ChooseOutput.c IOASCII/Output1D.c IOASCII/Output2D.c \
	 IOASCII/Output3D.c IOASCII/Startup.c IOASCII/Write1D.c IOASCII/Write2D.c \
	 IOASCII/Write3D.c IOASCII/cctk_Bindings/cctk_ThornBindings.c \
	 IOBasic/OutputInfo.c IOBasic/OutputScalar.c IOBasic/Startup.c \
	 IOBasic/WriteInfo.c IOBasic/WriteScalar.c \
	 IOBasic/cctk_Bindings/cctk_ThornBindings.c IOUtil/AdvertisedFiles.c \
	 IOUtil/CheckpointRecovery.c IOUtil/Startup.c IOUtil/Utils.c \
	 IOUtil/cctk_Bindings/cctk_ThornBindings.c LocalReduce/AvgFunctions.c \
	 LocalReduce/CountFunctions.c LocalReduce/MaxFunctions.c \
	 LocalReduce/MinFunctions.c LocalReduce/Norm1Functions.c \
	 LocalReduce/Norm2Functions.c LocalReduce/Norm3Functions.c \
	 LocalReduce/Norm4Functions.c LocalReduce/NormInfFunctions.c \
	 LocalReduce/Reduction.c LocalReduce/ReductionAvg.c \
	 LocalReduce/ReductionCount.c LocalReduce/ReductionMax.c \
	 LocalReduce/ReductionMin.c LocalReduce/ReductionNorm1.c \
	 LocalReduce/ReductionNorm2.c LocalReduce/ReductionNorm3.c \
	 LocalReduce/ReductionNorm4.c LocalReduce/ReductionNormInf.c \
	 LocalReduce/ReductionSum.c LocalReduce/Startup.c \
	 LocalReduce/SumFunctions.c \
	 LocalReduce/cctk_Bindings/cctk_ThornBindings.c ML_BSSN/Boundaries.cc \
	 ML_BSSN/ML_BSSN_Advect.cc ML_BSSN/ML_BSSN_Dissipation.cc \
	 ML_BSSN/ML_BSSN_InitGamma.cc ML_BSSN/ML_BSSN_InitRHS.cc \
	 ML_BSSN/ML_BSSN_Minkowski.cc ML_BSSN/ML_BSSN_RHS.cc \
	 ML_BSSN/ML_BSSN_RHSStaticBoundary.cc ML_BSSN/ML_BSSN_boundary.cc \
	 ML_BSSN/ML_BSSN_constraints.cc ML_BSSN/ML_BSSN_convertFromADMBase.cc \
	 ML_BSSN/ML_BSSN_convertFromADMBaseGamma.cc \
	 ML_BSSN/ML_BSSN_convertToADMBase.cc \
	 ML_BSSN/ML_BSSN_convertToADMBaseDtLapseShift.cc \
	 ML_BSSN/ML_BSSN_convertToADMBaseDtLapseShiftBoundary.cc \
	 ML_BSSN/ML_BSSN_convertToADMBaseFakeDtLapseShift.cc \
	 ML_BSSN/ML_BSSN_enforce.cc ML_BSSN/RegisterMoL.cc \
	 ML_BSSN/RegisterSymmetries.cc ML_BSSN/Startup.cc \
	 ML_BSSN/cctk_Bindings/cctk_ThornBindings.c \
	 ML_BSSN_Helper/ExtrapolateGammas.c ML_BSSN_Helper/NewRad.c \
	 ML_BSSN_Helper/RegisterConstrained.c ML_BSSN_Helper/RegisterSlicing.c \
	 ML_BSSN_Helper/SelectBCsADMBase.c ML_BSSN_Helper/SetGroupTags.c \
	 ML_BSSN_Helper/cctk_Bindings/cctk_ThornBindings.c MoL/AB.c \
	 MoL/ChangeType.c MoL/Counter.c MoL/Euler.c MoL/GenericRK.c MoL/ICN.c \
	 MoL/IndexArrays.c MoL/InitialCopy.c MoL/Operators.c MoL/ParamCheck.c \
	 MoL/RHSNaNCheck.c MoL/RK2-MR-2_1.c MoL/RK2.c MoL/RK3.c MoL/RK4-MR-2_1.c \
	 MoL/RK4-RK2.c MoL/RK4.c MoL/RK45.c MoL/RK65.c MoL/RK87.c \
	 MoL/RKCoefficients.c MoL/Registration.c MoL/SandR.c MoL/SetTime.c \
	 MoL/Startup.c MoL/StepSize.c MoL/cctk_Bindings/cctk_ThornBindings.c \
	 PUGH/Comm.c PUGH/Evolve.c PUGH/FinishReceiveGA.c PUGH/GHExtension.c \
	 PUGH/LoadAware.c PUGH/Overloadables.c PUGH/P2LMappings.c \
	 PUGH/PostReceiveGA.c PUGH/PostSendGA.c PUGH/PughUtils.c \
	 PUGH/Registration.c PUGH/SetupGroup.c PUGH/SetupPGH.c PUGH/SetupPGV.c \
	 PUGH/Startup.c PUGH/Storage.c PUGH/Topology.c PUGH/Vectors.cc \
	 PUGH/cctk_Bindings/cctk_ThornBindings.c PUGHInterp/InterpGridArrays.c \
	 PUGHInterp/Startup.c PUGHInterp/cctk_Bindings/cctk_ThornBindings.c \
	 PUGHReduce/ReduceArraysGlobally.c PUGHReduce/ReduceGA.c \
	 PUGHReduce/Reduction.c PUGHReduce/ReductionAvg.c \
	 PUGHReduce/ReductionCount.c PUGHReduce/ReductionMax.c \
	 PUGHReduce/ReductionMin.c PUGHReduce/ReductionNorm1.c \
	 PUGHReduce/ReductionNorm2.c PUGHReduce/ReductionNorm3.c \
	 PUGHReduce/ReductionNorm4.c PUGHReduce/ReductionNormInf.c \
	 PUGHReduce/ReductionSum.c PUGHReduce/Startup.c \
	 PUGHReduce/cctk_Bindings/cctk_ThornBindings.c \
	 PUGHSlab/DatatypeConversion.c PUGHSlab/GetHyperslab.c PUGHSlab/Mapping.c \
	 PUGHSlab/cctk_Bindings/cctk_ThornBindings.c StaticConformal/ConfPhys.c \
	 StaticConformal/Initialise.c \
	 StaticConformal/cctk_Bindings/cctk_ThornBindings.c SymBase/Check.c \
	 SymBase/Faces.c SymBase/Handles.c SymBase/Interpolation.c \
	 SymBase/Startup.c SymBase/Statistics.c SymBase/Table.c \
	 SymBase/cctk_Bindings/cctk_ThornBindings.c Time/Courant.c Time/Given.c \
	 Time/Initialise.c Time/Simple.c Time/cctk_Bindings/cctk_ThornBindings.c \
	 TmunuBase/CopyTmunu.f90 TmunuBase/SetStressEnergyState.f90 \
	 TmunuBase/SetTmunu.f90 TmunuBase/ZeroTmunu.f90 \
	 TmunuBase/cctk_Bindings/cctk_ThornBindings.c datestamp.c
EXEBASE=cactuBSSN_s
NEED_MATH=yes
BENCHLANG=CXX C F

BENCH_CFLAGS     = -Iinclude -DCCODE
BENCH_CXXFLAGS   = -Iinclude -DCCODE -DCCTK_DISABLE_RESTRICT=1
BENCH_FFLAGS     = -Iinclude -DFCODE
CC               = $(SPECLANG)gcc-7   -m64   -std=c11
CC_VERSION_OPTION = -v
CPORTABILITY     = 
CXX              = $(SPECLANG)g++-7   -m64
CXX_VERSION_OPTION = -v
EXTRA_OPTIMIZE   = -fopenmp -DSPEC_OPENMP
EXTRA_PORTABILITY = -DSPEC_LP64
FC               = $(SPECLANG)gfortran-7    -m64
FC_VERSION_OPTION = -v
FPORTABILITY     = 
OPTIMIZE         = -g -O2 -fopt-info
OS               = unix
SPECLANG         = /usr/bin/
absolutely_no_locking = 0
action           = build
allow_label_override = 0
backup_config    = 1
baseexe          = cactuBSSN_s
basepeak         = 0
benchdir         = benchspec
benchmark        = 607.cactuBSSN_s
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
commandexe       = cactuBSSN_s_base.gcc-baseline-o2-m64
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
configpath       = /home/ybmoon/cpu2017/config/gcc-baseline.cfg
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
floatcompare     = 1
force_monitor    = 0
from_runcpu      = 2
hostname         = scal22
http_proxy       = 
http_timeout     = 30
hw_cpu_name      = Intel Xeon Platinum 8260
hw_disk          = 886 GB  add more disk info here
hw_memory001     = 247.437 GB fixme: If using DDR3, format is:
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
logfile          = /home/ybmoon/cpu2017/tmp/CPU2017.001/templogs/preenv.fpspeed.001.1
logname          = /home/ybmoon/cpu2017/tmp/CPU2017.001/templogs/preenv.fpspeed.001.1
lognum           = 001.1
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
name             = cactuBSSN_s
nansupport       = 
need_math        = yes
no_input_handler = close
no_monitor       = 
noratios         = 0
note_preenv      = 1
notes_plat_sysinfo_000 =  Sysinfo program /home/ybmoon/cpu2017/bin/sysinfo
notes_plat_sysinfo_005 =  Rev: r5797 of 2017-06-14 96c45e4568ad54c135fd618bcc091c0f
notes_plat_sysinfo_010 =  running on scal22 Thu Nov  4 23:33:35 2021
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
notes_plat_sysinfo_140 =       NUMA node(s):          2
notes_plat_sysinfo_145 =       Vendor ID:             GenuineIntel
notes_plat_sysinfo_150 =       CPU family:            6
notes_plat_sysinfo_155 =       Model:                 85
notes_plat_sysinfo_160 =       Model name:            Intel(R) Xeon(R) Platinum 8260 CPU @ 2.40GHz
notes_plat_sysinfo_165 =       Stepping:              7
notes_plat_sysinfo_170 =       CPU MHz:               999.807
notes_plat_sysinfo_175 =       CPU max MHz:           3900.0000
notes_plat_sysinfo_180 =       CPU min MHz:           1000.0000
notes_plat_sysinfo_185 =       BogoMIPS:              4801.39
notes_plat_sysinfo_190 =       Virtualization:        VT-x
notes_plat_sysinfo_195 =       L1d cache:             32K
notes_plat_sysinfo_200 =       L1i cache:             32K
notes_plat_sysinfo_205 =       L2 cache:              1024K
notes_plat_sysinfo_210 =       L3 cache:              36608K
notes_plat_sysinfo_215 =       NUMA node0 CPU(s):
notes_plat_sysinfo_220 =       0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46
notes_plat_sysinfo_225 =       NUMA node1 CPU(s):
notes_plat_sysinfo_230 =       1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47
notes_plat_sysinfo_235 =       Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov
notes_plat_sysinfo_240 =       pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp
notes_plat_sysinfo_245 =       lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid
notes_plat_sysinfo_250 =       aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16
notes_plat_sysinfo_255 =       xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave
notes_plat_sysinfo_260 =       avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3
notes_plat_sysinfo_265 =       invpcid_single intel_ppin ssbd mba ibrs ibpb stibp ibrs_enhanced tpr_shadow vnmi
notes_plat_sysinfo_270 =       flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms
notes_plat_sysinfo_275 =       invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb intel_pt
notes_plat_sysinfo_280 =       avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc
notes_plat_sysinfo_285 =       cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts pku ospke avx512_vnni md_clear
notes_plat_sysinfo_290 =       flush_l1d arch_capabilities
notes_plat_sysinfo_295 = 
notes_plat_sysinfo_300 =  /proc/cpuinfo cache data
notes_plat_sysinfo_305 =     cache size : 36608 KB
notes_plat_sysinfo_310 = 
notes_plat_sysinfo_315 =  From numactl --hardware  WARNING: a numactl 'node' might or might not correspond to a
notes_plat_sysinfo_320 =  physical chip.
notes_plat_sysinfo_325 =    available: 2 nodes (0-1)
notes_plat_sysinfo_330 =    node 0 cpus: 0 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46
notes_plat_sysinfo_335 =    node 0 size: 126397 MB
notes_plat_sysinfo_340 =    node 0 free: 120477 MB
notes_plat_sysinfo_345 =    node 1 cpus: 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47
notes_plat_sysinfo_350 =    node 1 size: 126977 MB
notes_plat_sysinfo_355 =    node 1 free: 122681 MB
notes_plat_sysinfo_360 =    node distances:
notes_plat_sysinfo_365 =    node   0   1
notes_plat_sysinfo_370 =      0:  10  21
notes_plat_sysinfo_375 =      1:  21  10
notes_plat_sysinfo_380 = 
notes_plat_sysinfo_385 =  From /proc/meminfo
notes_plat_sysinfo_390 =     MemTotal:       259456300 kB
notes_plat_sysinfo_395 =     HugePages_Total:       0
notes_plat_sysinfo_400 =     Hugepagesize:       2048 kB
notes_plat_sysinfo_405 = 
notes_plat_sysinfo_410 =  /usr/bin/lsb_release -d
notes_plat_sysinfo_415 =     Ubuntu 16.04.6 LTS
notes_plat_sysinfo_420 = 
notes_plat_sysinfo_425 =  From /etc/*release* /etc/*version*
notes_plat_sysinfo_430 =     debian_version: stretch/sid
notes_plat_sysinfo_435 =     os-release:
notes_plat_sysinfo_440 =        NAME="Ubuntu"
notes_plat_sysinfo_445 =        VERSION="16.04.6 LTS (Xenial Xerus)"
notes_plat_sysinfo_450 =        ID=ubuntu
notes_plat_sysinfo_455 =        ID_LIKE=debian
notes_plat_sysinfo_460 =        PRETTY_NAME="Ubuntu 16.04.6 LTS"
notes_plat_sysinfo_465 =        VERSION_ID="16.04"
notes_plat_sysinfo_470 =        HOME_URL="http://www.ubuntu.com/"
notes_plat_sysinfo_475 =        SUPPORT_URL="http://help.ubuntu.com/"
notes_plat_sysinfo_480 = 
notes_plat_sysinfo_485 =  uname -a:
notes_plat_sysinfo_490 =     Linux scal22 5.3.0-autotiering \#1 SMP Tue Oct 26 14:46:15 KST 2021 x86_64 x86_64
notes_plat_sysinfo_495 =     x86_64 GNU/Linux
notes_plat_sysinfo_500 = 
notes_plat_sysinfo_505 =  run-level 5 Nov 4 23:08
notes_plat_sysinfo_510 = 
notes_plat_sysinfo_515 =  SPEC is set to: /home/ybmoon/cpu2017
notes_plat_sysinfo_520 =     Filesystem           Type  Size  Used Avail Use% Mounted on
notes_plat_sysinfo_525 =     /dev/mapper/lvm-root ext4  886G  513G  329G  61% /
notes_plat_sysinfo_530 = 
notes_plat_sysinfo_535 =  Additional information from dmidecode follows.  WARNING: Use caution when you interpret
notes_plat_sysinfo_540 =  this section. The 'dmidecode' program reads system data which is "intended to allow
notes_plat_sysinfo_545 =  hardware to be accurately determined", but the intent may not be met, as there are
notes_plat_sysinfo_550 =  frequent changes to hardware, firmware, and the "DMTF SMBIOS" standard.
notes_plat_sysinfo_555 = 
notes_plat_sysinfo_560 =  (End of data from sysinfo program)
notes_wrap_columns = 0
notes_wrap_indent =   
num              = 607
obiwan           = 
oldhash          = 
os_exe_ext       = 
output_format    = txt,html,cfg,pdf,csv
output_root      = 
outputdir        = output
parallel_test    = 0
parallel_test_submit = 0
parallel_test_workloads = 
path             = /home/ybmoon/cpu2017/benchspec/CPU/607.cactuBSSN_s
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
reportable       = 0
resultdir        = result
review           = 0
run              = all
runcpu           = /home/ybmoon/cpu2017/bin/harness/runcpu --configfile gcc-baseline --action build --noreportable --nopower --runmode speed --tune base --size refspeed fpspeed --nopreenv --note-preenv --logfile /home/ybmoon/cpu2017/tmp/CPU2017.001/templogs/preenv.fpspeed.001.1 --lognum 001.1 --from_runcpu 2
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
srcsource        = /home/ybmoon/cpu2017/benchspec/CPU/507.cactuBSSN_r/src
stagger          = 10
strict_rundir_verify = 0
sw_base_ptrsize  = 64-bit
sw_compiler001   = C/C++/Fortran: Version 6.2.0 of GCC, the
sw_compiler002   = GNU Compiler Collection
sw_file          = ext4
sw_os001         = Ubuntu 16.04.6 LTS
sw_os002         = 5.3.0-autotiering
sw_peak_ptrsize  = 64-bit
sw_state         = Run level 5 (add definition here)
sysinfo_hash_bits = 256
sysinfo_program  = specperl /home/ybmoon/cpu2017/bin/sysinfo
sysinfo_program_hash = sysinfo:SHA:ecd2bef08f316af97f5a7768b641e2a3307c1b4b68efb5a57fa76367d790d233
table            = 1
teeout           = 0
test_date        = Nov-2021
test_sponsor     = SCALable Computer Architecture Laboratory at Seoul National University
tester           = Deok-Jae Oh
threads          = 1
top              = /home/ybmoon/cpu2017
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
OUTPUT_RMFILES   = gxx.xl gxy.xl spec_train.out
