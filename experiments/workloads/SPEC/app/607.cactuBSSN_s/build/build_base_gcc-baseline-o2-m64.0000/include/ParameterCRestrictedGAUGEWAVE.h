#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL amp;
  CCTK_REAL boostx;
  CCTK_REAL boosty;
  CCTK_REAL boostz;
  CCTK_REAL lapsefactor;
  CCTK_REAL period;
  CCTK_REAL phi;
  CCTK_REAL positionx;
  CCTK_REAL positiony;
  CCTK_REAL positionz;
  CCTK_REAL psi;
  CCTK_REAL shiftaddx;
  CCTK_REAL shiftaddy;
  CCTK_REAL shiftaddz;
  CCTK_REAL theta;
  CCTK_REAL timeoffset;
  CCTK_INT GaugeWave_MaxNumArrayEvolvedVars;
  CCTK_INT GaugeWave_MaxNumEvolvedVars;
  CCTK_INT GaugeWave_always_calc_every;
  CCTK_INT GaugeWave_always_calc_offset;
  CCTK_INT GaugeWave_initial_calc_every;
  CCTK_INT GaugeWave_initial_calc_offset;
  CCTK_INT other_timelevels;
  CCTK_INT rhs_timelevels;
  CCTK_INT timelevels;
  CCTK_INT verbose;
} RESTRICTED_GAUGEWAVE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_GAUGEWAVE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, amp, CCTK_PARAMETER__GAUGEWAVE__amp); \
  CCTK_DECLARE_INIT (CCTK_REAL const, boostx, CCTK_PARAMETER__GAUGEWAVE__boostx); \
  CCTK_DECLARE_INIT (CCTK_REAL const, boosty, CCTK_PARAMETER__GAUGEWAVE__boosty); \
  CCTK_DECLARE_INIT (CCTK_REAL const, boostz, CCTK_PARAMETER__GAUGEWAVE__boostz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, lapsefactor, CCTK_PARAMETER__GAUGEWAVE__lapsefactor); \
  CCTK_DECLARE_INIT (CCTK_REAL const, period, CCTK_PARAMETER__GAUGEWAVE__period); \
  CCTK_DECLARE_INIT (CCTK_REAL const, phi, CCTK_PARAMETER__GAUGEWAVE__phi); \
  CCTK_DECLARE_INIT (CCTK_REAL const, positionx, CCTK_PARAMETER__GAUGEWAVE__positionx); \
  CCTK_DECLARE_INIT (CCTK_REAL const, positiony, CCTK_PARAMETER__GAUGEWAVE__positiony); \
  CCTK_DECLARE_INIT (CCTK_REAL const, positionz, CCTK_PARAMETER__GAUGEWAVE__positionz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, psi, CCTK_PARAMETER__GAUGEWAVE__psi); \
  CCTK_DECLARE_INIT (CCTK_REAL const, shiftaddx, CCTK_PARAMETER__GAUGEWAVE__shiftaddx); \
  CCTK_DECLARE_INIT (CCTK_REAL const, shiftaddy, CCTK_PARAMETER__GAUGEWAVE__shiftaddy); \
  CCTK_DECLARE_INIT (CCTK_REAL const, shiftaddz, CCTK_PARAMETER__GAUGEWAVE__shiftaddz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, theta, CCTK_PARAMETER__GAUGEWAVE__theta); \
  CCTK_DECLARE_INIT (CCTK_REAL const, timeoffset, CCTK_PARAMETER__GAUGEWAVE__timeoffset); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_MaxNumArrayEvolvedVars, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumArrayEvolvedVars); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_MaxNumEvolvedVars, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumEvolvedVars); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_always_calc_every, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_always_calc_offset, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_offset); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_initial_calc_every, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, GaugeWave_initial_calc_offset, CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_offset); \
  CCTK_DECLARE_INIT (CCTK_INT const, other_timelevels, CCTK_PARAMETER__GAUGEWAVE__other_timelevels); \
  CCTK_DECLARE_INIT (CCTK_INT const, rhs_timelevels, CCTK_PARAMETER__GAUGEWAVE__rhs_timelevels); \
  CCTK_DECLARE_INIT (CCTK_INT const, timelevels, CCTK_PARAMETER__GAUGEWAVE__timelevels); \
  CCTK_DECLARE_INIT (CCTK_INT const, verbose, CCTK_PARAMETER__GAUGEWAVE__verbose); \


#ifndef CCTK_PARAMETER__GAUGEWAVE__amp
#  define CCTK_PARAMETER__GAUGEWAVE__amp RESTRICTED_GAUGEWAVE_STRUCT.amp
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__boostx
#  define CCTK_PARAMETER__GAUGEWAVE__boostx RESTRICTED_GAUGEWAVE_STRUCT.boostx
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__boosty
#  define CCTK_PARAMETER__GAUGEWAVE__boosty RESTRICTED_GAUGEWAVE_STRUCT.boosty
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__boostz
#  define CCTK_PARAMETER__GAUGEWAVE__boostz RESTRICTED_GAUGEWAVE_STRUCT.boostz
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__lapsefactor
#  define CCTK_PARAMETER__GAUGEWAVE__lapsefactor RESTRICTED_GAUGEWAVE_STRUCT.lapsefactor
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__period
#  define CCTK_PARAMETER__GAUGEWAVE__period RESTRICTED_GAUGEWAVE_STRUCT.period
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__phi
#  define CCTK_PARAMETER__GAUGEWAVE__phi RESTRICTED_GAUGEWAVE_STRUCT.phi
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__positionx
#  define CCTK_PARAMETER__GAUGEWAVE__positionx RESTRICTED_GAUGEWAVE_STRUCT.positionx
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__positiony
#  define CCTK_PARAMETER__GAUGEWAVE__positiony RESTRICTED_GAUGEWAVE_STRUCT.positiony
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__positionz
#  define CCTK_PARAMETER__GAUGEWAVE__positionz RESTRICTED_GAUGEWAVE_STRUCT.positionz
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__psi
#  define CCTK_PARAMETER__GAUGEWAVE__psi RESTRICTED_GAUGEWAVE_STRUCT.psi
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__shiftaddx
#  define CCTK_PARAMETER__GAUGEWAVE__shiftaddx RESTRICTED_GAUGEWAVE_STRUCT.shiftaddx
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__shiftaddy
#  define CCTK_PARAMETER__GAUGEWAVE__shiftaddy RESTRICTED_GAUGEWAVE_STRUCT.shiftaddy
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__shiftaddz
#  define CCTK_PARAMETER__GAUGEWAVE__shiftaddz RESTRICTED_GAUGEWAVE_STRUCT.shiftaddz
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__theta
#  define CCTK_PARAMETER__GAUGEWAVE__theta RESTRICTED_GAUGEWAVE_STRUCT.theta
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__timeoffset
#  define CCTK_PARAMETER__GAUGEWAVE__timeoffset RESTRICTED_GAUGEWAVE_STRUCT.timeoffset
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumArrayEvolvedVars
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumArrayEvolvedVars RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_MaxNumArrayEvolvedVars
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumEvolvedVars
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_MaxNumEvolvedVars RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_MaxNumEvolvedVars
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_every
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_every RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_always_calc_every
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_offset
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_always_calc_offset RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_always_calc_offset
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_every
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_every RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_initial_calc_every
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_offset
#  define CCTK_PARAMETER__GAUGEWAVE__GaugeWave_initial_calc_offset RESTRICTED_GAUGEWAVE_STRUCT.GaugeWave_initial_calc_offset
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__other_timelevels
#  define CCTK_PARAMETER__GAUGEWAVE__other_timelevels RESTRICTED_GAUGEWAVE_STRUCT.other_timelevels
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__rhs_timelevels
#  define CCTK_PARAMETER__GAUGEWAVE__rhs_timelevels RESTRICTED_GAUGEWAVE_STRUCT.rhs_timelevels
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__timelevels
#  define CCTK_PARAMETER__GAUGEWAVE__timelevels RESTRICTED_GAUGEWAVE_STRUCT.timelevels
#endif
#ifndef CCTK_PARAMETER__GAUGEWAVE__verbose
#  define CCTK_PARAMETER__GAUGEWAVE__verbose RESTRICTED_GAUGEWAVE_STRUCT.verbose
#endif

