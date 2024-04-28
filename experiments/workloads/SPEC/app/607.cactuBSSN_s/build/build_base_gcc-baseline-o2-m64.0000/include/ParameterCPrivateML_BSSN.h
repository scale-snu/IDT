#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL A_bound_limit;
  CCTK_REAL A_bound_scalar;
  CCTK_REAL A_bound_speed;
  CCTK_REAL At11_bound_limit;
  CCTK_REAL At11_bound_scalar;
  CCTK_REAL At11_bound_speed;
  CCTK_REAL At12_bound_limit;
  CCTK_REAL At12_bound_scalar;
  CCTK_REAL At12_bound_speed;
  CCTK_REAL At13_bound_limit;
  CCTK_REAL At13_bound_scalar;
  CCTK_REAL At13_bound_speed;
  CCTK_REAL At22_bound_limit;
  CCTK_REAL At22_bound_scalar;
  CCTK_REAL At22_bound_speed;
  CCTK_REAL At23_bound_limit;
  CCTK_REAL At23_bound_scalar;
  CCTK_REAL At23_bound_speed;
  CCTK_REAL At33_bound_limit;
  CCTK_REAL At33_bound_scalar;
  CCTK_REAL At33_bound_speed;
  CCTK_REAL B1_bound_limit;
  CCTK_REAL B1_bound_scalar;
  CCTK_REAL B1_bound_speed;
  CCTK_REAL B2_bound_limit;
  CCTK_REAL B2_bound_scalar;
  CCTK_REAL B2_bound_speed;
  CCTK_REAL B3_bound_limit;
  CCTK_REAL B3_bound_scalar;
  CCTK_REAL B3_bound_speed;
  CCTK_REAL ML_Gamma_bound_limit;
  CCTK_REAL ML_Gamma_bound_scalar;
  CCTK_REAL ML_Gamma_bound_speed;
  CCTK_REAL ML_curv_bound_limit;
  CCTK_REAL ML_curv_bound_scalar;
  CCTK_REAL ML_curv_bound_speed;
  CCTK_REAL ML_dtlapse_bound_limit;
  CCTK_REAL ML_dtlapse_bound_scalar;
  CCTK_REAL ML_dtlapse_bound_speed;
  CCTK_REAL ML_dtshift_bound_limit;
  CCTK_REAL ML_dtshift_bound_scalar;
  CCTK_REAL ML_dtshift_bound_speed;
  CCTK_REAL ML_lapse_bound_limit;
  CCTK_REAL ML_lapse_bound_scalar;
  CCTK_REAL ML_lapse_bound_speed;
  CCTK_REAL ML_log_confac_bound_limit;
  CCTK_REAL ML_log_confac_bound_scalar;
  CCTK_REAL ML_log_confac_bound_speed;
  CCTK_REAL ML_metric_bound_limit;
  CCTK_REAL ML_metric_bound_scalar;
  CCTK_REAL ML_metric_bound_speed;
  CCTK_REAL ML_shift_bound_limit;
  CCTK_REAL ML_shift_bound_scalar;
  CCTK_REAL ML_shift_bound_speed;
  CCTK_REAL ML_trace_curv_bound_limit;
  CCTK_REAL ML_trace_curv_bound_scalar;
  CCTK_REAL ML_trace_curv_bound_speed;
  CCTK_REAL Xt1_bound_limit;
  CCTK_REAL Xt1_bound_scalar;
  CCTK_REAL Xt1_bound_speed;
  CCTK_REAL Xt2_bound_limit;
  CCTK_REAL Xt2_bound_scalar;
  CCTK_REAL Xt2_bound_speed;
  CCTK_REAL Xt3_bound_limit;
  CCTK_REAL Xt3_bound_scalar;
  CCTK_REAL Xt3_bound_speed;
  CCTK_REAL alpha_bound_limit;
  CCTK_REAL alpha_bound_scalar;
  CCTK_REAL alpha_bound_speed;
  CCTK_REAL beta1_bound_limit;
  CCTK_REAL beta1_bound_scalar;
  CCTK_REAL beta1_bound_speed;
  CCTK_REAL beta2_bound_limit;
  CCTK_REAL beta2_bound_scalar;
  CCTK_REAL beta2_bound_speed;
  CCTK_REAL beta3_bound_limit;
  CCTK_REAL beta3_bound_scalar;
  CCTK_REAL beta3_bound_speed;
  CCTK_REAL gt11_bound_limit;
  CCTK_REAL gt11_bound_scalar;
  CCTK_REAL gt11_bound_speed;
  CCTK_REAL gt12_bound_limit;
  CCTK_REAL gt12_bound_scalar;
  CCTK_REAL gt12_bound_speed;
  CCTK_REAL gt13_bound_limit;
  CCTK_REAL gt13_bound_scalar;
  CCTK_REAL gt13_bound_speed;
  CCTK_REAL gt22_bound_limit;
  CCTK_REAL gt22_bound_scalar;
  CCTK_REAL gt22_bound_speed;
  CCTK_REAL gt23_bound_limit;
  CCTK_REAL gt23_bound_scalar;
  CCTK_REAL gt23_bound_speed;
  CCTK_REAL gt33_bound_limit;
  CCTK_REAL gt33_bound_scalar;
  CCTK_REAL gt33_bound_speed;
  CCTK_REAL phi_bound_limit;
  CCTK_REAL phi_bound_scalar;
  CCTK_REAL phi_bound_speed;
  CCTK_REAL trK_bound_limit;
  CCTK_REAL trK_bound_scalar;
  CCTK_REAL trK_bound_speed;
  const char * A_bound;
  const char * At11_bound;
  const char * At12_bound;
  const char * At13_bound;
  const char * At22_bound;
  const char * At23_bound;
  const char * At33_bound;
  const char * B1_bound;
  const char * B2_bound;
  const char * B3_bound;
  const char * ML_Gamma_bound;
  const char * ML_curv_bound;
  const char * ML_dtlapse_bound;
  const char * ML_dtshift_bound;
  const char * ML_lapse_bound;
  const char * ML_log_confac_bound;
  const char * ML_metric_bound;
  const char * ML_shift_bound;
  const char * ML_trace_curv_bound;
  const char * Xt1_bound;
  const char * Xt2_bound;
  const char * Xt3_bound;
  const char * alpha_bound;
  const char * apply_dissipation;
  const char * beta1_bound;
  const char * beta2_bound;
  const char * beta3_bound;
  const char * dt_lapse_shift_method;
  const char * gt11_bound;
  const char * gt12_bound;
  const char * gt13_bound;
  const char * gt22_bound;
  const char * gt23_bound;
  const char * gt33_bound;
  const char * my_boundary_condition;
  const char * my_initial_data;
  const char * phi_bound;
  const char * trK_bound;
} PRIVATE_ML_BSSN_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_ML_BSSN_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, A_bound_limit, CCTK_PARAMETER__ML_BSSN__A_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, A_bound_scalar, CCTK_PARAMETER__ML_BSSN__A_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, A_bound_speed, CCTK_PARAMETER__ML_BSSN__A_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At11_bound_limit, CCTK_PARAMETER__ML_BSSN__At11_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At11_bound_scalar, CCTK_PARAMETER__ML_BSSN__At11_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At11_bound_speed, CCTK_PARAMETER__ML_BSSN__At11_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At12_bound_limit, CCTK_PARAMETER__ML_BSSN__At12_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At12_bound_scalar, CCTK_PARAMETER__ML_BSSN__At12_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At12_bound_speed, CCTK_PARAMETER__ML_BSSN__At12_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At13_bound_limit, CCTK_PARAMETER__ML_BSSN__At13_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At13_bound_scalar, CCTK_PARAMETER__ML_BSSN__At13_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At13_bound_speed, CCTK_PARAMETER__ML_BSSN__At13_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At22_bound_limit, CCTK_PARAMETER__ML_BSSN__At22_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At22_bound_scalar, CCTK_PARAMETER__ML_BSSN__At22_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At22_bound_speed, CCTK_PARAMETER__ML_BSSN__At22_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At23_bound_limit, CCTK_PARAMETER__ML_BSSN__At23_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At23_bound_scalar, CCTK_PARAMETER__ML_BSSN__At23_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At23_bound_speed, CCTK_PARAMETER__ML_BSSN__At23_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At33_bound_limit, CCTK_PARAMETER__ML_BSSN__At33_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At33_bound_scalar, CCTK_PARAMETER__ML_BSSN__At33_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, At33_bound_speed, CCTK_PARAMETER__ML_BSSN__At33_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B1_bound_limit, CCTK_PARAMETER__ML_BSSN__B1_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B1_bound_scalar, CCTK_PARAMETER__ML_BSSN__B1_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B1_bound_speed, CCTK_PARAMETER__ML_BSSN__B1_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B2_bound_limit, CCTK_PARAMETER__ML_BSSN__B2_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B2_bound_scalar, CCTK_PARAMETER__ML_BSSN__B2_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B2_bound_speed, CCTK_PARAMETER__ML_BSSN__B2_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B3_bound_limit, CCTK_PARAMETER__ML_BSSN__B3_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B3_bound_scalar, CCTK_PARAMETER__ML_BSSN__B3_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, B3_bound_speed, CCTK_PARAMETER__ML_BSSN__B3_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_Gamma_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_Gamma_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_Gamma_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_curv_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_curv_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_curv_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_curv_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_curv_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_curv_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtlapse_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtlapse_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtlapse_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtshift_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtshift_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_dtshift_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_lapse_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_lapse_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_lapse_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_log_confac_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_log_confac_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_log_confac_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_metric_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_metric_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_metric_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_metric_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_metric_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_metric_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_shift_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_shift_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_shift_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_shift_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_shift_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_shift_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_trace_curv_bound_limit, CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_trace_curv_bound_scalar, CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ML_trace_curv_bound_speed, CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt1_bound_limit, CCTK_PARAMETER__ML_BSSN__Xt1_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt1_bound_scalar, CCTK_PARAMETER__ML_BSSN__Xt1_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt1_bound_speed, CCTK_PARAMETER__ML_BSSN__Xt1_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt2_bound_limit, CCTK_PARAMETER__ML_BSSN__Xt2_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt2_bound_scalar, CCTK_PARAMETER__ML_BSSN__Xt2_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt2_bound_speed, CCTK_PARAMETER__ML_BSSN__Xt2_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt3_bound_limit, CCTK_PARAMETER__ML_BSSN__Xt3_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt3_bound_scalar, CCTK_PARAMETER__ML_BSSN__Xt3_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, Xt3_bound_speed, CCTK_PARAMETER__ML_BSSN__Xt3_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, alpha_bound_limit, CCTK_PARAMETER__ML_BSSN__alpha_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, alpha_bound_scalar, CCTK_PARAMETER__ML_BSSN__alpha_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, alpha_bound_speed, CCTK_PARAMETER__ML_BSSN__alpha_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta1_bound_limit, CCTK_PARAMETER__ML_BSSN__beta1_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta1_bound_scalar, CCTK_PARAMETER__ML_BSSN__beta1_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta1_bound_speed, CCTK_PARAMETER__ML_BSSN__beta1_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta2_bound_limit, CCTK_PARAMETER__ML_BSSN__beta2_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta2_bound_scalar, CCTK_PARAMETER__ML_BSSN__beta2_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta2_bound_speed, CCTK_PARAMETER__ML_BSSN__beta2_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta3_bound_limit, CCTK_PARAMETER__ML_BSSN__beta3_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta3_bound_scalar, CCTK_PARAMETER__ML_BSSN__beta3_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, beta3_bound_speed, CCTK_PARAMETER__ML_BSSN__beta3_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt11_bound_limit, CCTK_PARAMETER__ML_BSSN__gt11_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt11_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt11_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt11_bound_speed, CCTK_PARAMETER__ML_BSSN__gt11_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt12_bound_limit, CCTK_PARAMETER__ML_BSSN__gt12_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt12_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt12_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt12_bound_speed, CCTK_PARAMETER__ML_BSSN__gt12_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt13_bound_limit, CCTK_PARAMETER__ML_BSSN__gt13_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt13_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt13_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt13_bound_speed, CCTK_PARAMETER__ML_BSSN__gt13_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt22_bound_limit, CCTK_PARAMETER__ML_BSSN__gt22_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt22_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt22_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt22_bound_speed, CCTK_PARAMETER__ML_BSSN__gt22_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt23_bound_limit, CCTK_PARAMETER__ML_BSSN__gt23_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt23_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt23_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt23_bound_speed, CCTK_PARAMETER__ML_BSSN__gt23_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt33_bound_limit, CCTK_PARAMETER__ML_BSSN__gt33_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt33_bound_scalar, CCTK_PARAMETER__ML_BSSN__gt33_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, gt33_bound_speed, CCTK_PARAMETER__ML_BSSN__gt33_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, phi_bound_limit, CCTK_PARAMETER__ML_BSSN__phi_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, phi_bound_scalar, CCTK_PARAMETER__ML_BSSN__phi_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, phi_bound_speed, CCTK_PARAMETER__ML_BSSN__phi_bound_speed); \
  CCTK_DECLARE_INIT (CCTK_REAL const, trK_bound_limit, CCTK_PARAMETER__ML_BSSN__trK_bound_limit); \
  CCTK_DECLARE_INIT (CCTK_REAL const, trK_bound_scalar, CCTK_PARAMETER__ML_BSSN__trK_bound_scalar); \
  CCTK_DECLARE_INIT (CCTK_REAL const, trK_bound_speed, CCTK_PARAMETER__ML_BSSN__trK_bound_speed); \
  CCTK_DECLARE_INIT (const char * const, A_bound, CCTK_PARAMETER__ML_BSSN__A_bound); \
  CCTK_DECLARE_INIT (const char * const, At11_bound, CCTK_PARAMETER__ML_BSSN__At11_bound); \
  CCTK_DECLARE_INIT (const char * const, At12_bound, CCTK_PARAMETER__ML_BSSN__At12_bound); \
  CCTK_DECLARE_INIT (const char * const, At13_bound, CCTK_PARAMETER__ML_BSSN__At13_bound); \
  CCTK_DECLARE_INIT (const char * const, At22_bound, CCTK_PARAMETER__ML_BSSN__At22_bound); \
  CCTK_DECLARE_INIT (const char * const, At23_bound, CCTK_PARAMETER__ML_BSSN__At23_bound); \
  CCTK_DECLARE_INIT (const char * const, At33_bound, CCTK_PARAMETER__ML_BSSN__At33_bound); \
  CCTK_DECLARE_INIT (const char * const, B1_bound, CCTK_PARAMETER__ML_BSSN__B1_bound); \
  CCTK_DECLARE_INIT (const char * const, B2_bound, CCTK_PARAMETER__ML_BSSN__B2_bound); \
  CCTK_DECLARE_INIT (const char * const, B3_bound, CCTK_PARAMETER__ML_BSSN__B3_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_Gamma_bound, CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_curv_bound, CCTK_PARAMETER__ML_BSSN__ML_curv_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_dtlapse_bound, CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_dtshift_bound, CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_lapse_bound, CCTK_PARAMETER__ML_BSSN__ML_lapse_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_log_confac_bound, CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_metric_bound, CCTK_PARAMETER__ML_BSSN__ML_metric_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_shift_bound, CCTK_PARAMETER__ML_BSSN__ML_shift_bound); \
  CCTK_DECLARE_INIT (const char * const, ML_trace_curv_bound, CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound); \
  CCTK_DECLARE_INIT (const char * const, Xt1_bound, CCTK_PARAMETER__ML_BSSN__Xt1_bound); \
  CCTK_DECLARE_INIT (const char * const, Xt2_bound, CCTK_PARAMETER__ML_BSSN__Xt2_bound); \
  CCTK_DECLARE_INIT (const char * const, Xt3_bound, CCTK_PARAMETER__ML_BSSN__Xt3_bound); \
  CCTK_DECLARE_INIT (const char * const, alpha_bound, CCTK_PARAMETER__ML_BSSN__alpha_bound); \
  CCTK_DECLARE_INIT (const char * const, apply_dissipation, CCTK_PARAMETER__ML_BSSN__apply_dissipation); \
  CCTK_DECLARE_INIT (const char * const, beta1_bound, CCTK_PARAMETER__ML_BSSN__beta1_bound); \
  CCTK_DECLARE_INIT (const char * const, beta2_bound, CCTK_PARAMETER__ML_BSSN__beta2_bound); \
  CCTK_DECLARE_INIT (const char * const, beta3_bound, CCTK_PARAMETER__ML_BSSN__beta3_bound); \
  CCTK_DECLARE_INIT (const char * const, dt_lapse_shift_method, CCTK_PARAMETER__ML_BSSN__dt_lapse_shift_method); \
  CCTK_DECLARE_INIT (const char * const, gt11_bound, CCTK_PARAMETER__ML_BSSN__gt11_bound); \
  CCTK_DECLARE_INIT (const char * const, gt12_bound, CCTK_PARAMETER__ML_BSSN__gt12_bound); \
  CCTK_DECLARE_INIT (const char * const, gt13_bound, CCTK_PARAMETER__ML_BSSN__gt13_bound); \
  CCTK_DECLARE_INIT (const char * const, gt22_bound, CCTK_PARAMETER__ML_BSSN__gt22_bound); \
  CCTK_DECLARE_INIT (const char * const, gt23_bound, CCTK_PARAMETER__ML_BSSN__gt23_bound); \
  CCTK_DECLARE_INIT (const char * const, gt33_bound, CCTK_PARAMETER__ML_BSSN__gt33_bound); \
  CCTK_DECLARE_INIT (const char * const, my_boundary_condition, CCTK_PARAMETER__ML_BSSN__my_boundary_condition); \
  CCTK_DECLARE_INIT (const char * const, my_initial_data, CCTK_PARAMETER__ML_BSSN__my_initial_data); \
  CCTK_DECLARE_INIT (const char * const, phi_bound, CCTK_PARAMETER__ML_BSSN__phi_bound); \
  CCTK_DECLARE_INIT (const char * const, trK_bound, CCTK_PARAMETER__ML_BSSN__trK_bound); \


#ifndef CCTK_PARAMETER__ML_BSSN__A_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__A_bound_limit PRIVATE_ML_BSSN_STRUCT.A_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__A_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__A_bound_scalar PRIVATE_ML_BSSN_STRUCT.A_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__A_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__A_bound_speed PRIVATE_ML_BSSN_STRUCT.A_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At11_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At11_bound_limit PRIVATE_ML_BSSN_STRUCT.At11_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At11_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At11_bound_scalar PRIVATE_ML_BSSN_STRUCT.At11_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At11_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At11_bound_speed PRIVATE_ML_BSSN_STRUCT.At11_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At12_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At12_bound_limit PRIVATE_ML_BSSN_STRUCT.At12_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At12_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At12_bound_scalar PRIVATE_ML_BSSN_STRUCT.At12_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At12_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At12_bound_speed PRIVATE_ML_BSSN_STRUCT.At12_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At13_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At13_bound_limit PRIVATE_ML_BSSN_STRUCT.At13_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At13_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At13_bound_scalar PRIVATE_ML_BSSN_STRUCT.At13_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At13_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At13_bound_speed PRIVATE_ML_BSSN_STRUCT.At13_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At22_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At22_bound_limit PRIVATE_ML_BSSN_STRUCT.At22_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At22_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At22_bound_scalar PRIVATE_ML_BSSN_STRUCT.At22_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At22_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At22_bound_speed PRIVATE_ML_BSSN_STRUCT.At22_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At23_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At23_bound_limit PRIVATE_ML_BSSN_STRUCT.At23_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At23_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At23_bound_scalar PRIVATE_ML_BSSN_STRUCT.At23_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At23_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At23_bound_speed PRIVATE_ML_BSSN_STRUCT.At23_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At33_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__At33_bound_limit PRIVATE_ML_BSSN_STRUCT.At33_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At33_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__At33_bound_scalar PRIVATE_ML_BSSN_STRUCT.At33_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At33_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__At33_bound_speed PRIVATE_ML_BSSN_STRUCT.At33_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B1_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__B1_bound_limit PRIVATE_ML_BSSN_STRUCT.B1_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B1_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__B1_bound_scalar PRIVATE_ML_BSSN_STRUCT.B1_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B1_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__B1_bound_speed PRIVATE_ML_BSSN_STRUCT.B1_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B2_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__B2_bound_limit PRIVATE_ML_BSSN_STRUCT.B2_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B2_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__B2_bound_scalar PRIVATE_ML_BSSN_STRUCT.B2_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B2_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__B2_bound_speed PRIVATE_ML_BSSN_STRUCT.B2_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B3_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__B3_bound_limit PRIVATE_ML_BSSN_STRUCT.B3_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B3_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__B3_bound_scalar PRIVATE_ML_BSSN_STRUCT.B3_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B3_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__B3_bound_speed PRIVATE_ML_BSSN_STRUCT.B3_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_Gamma_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_Gamma_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_Gamma_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_curv_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_curv_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_curv_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_curv_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_curv_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_curv_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_curv_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_curv_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_curv_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_dtlapse_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_dtlapse_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_dtlapse_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_dtshift_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_dtshift_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_dtshift_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_lapse_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_lapse_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_lapse_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_lapse_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_log_confac_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_log_confac_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_log_confac_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_metric_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_metric_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_metric_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_metric_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_metric_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_metric_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_metric_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_metric_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_metric_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_shift_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_shift_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_shift_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_shift_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_shift_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_shift_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_shift_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_shift_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_shift_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_limit PRIVATE_ML_BSSN_STRUCT.ML_trace_curv_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_scalar PRIVATE_ML_BSSN_STRUCT.ML_trace_curv_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound_speed PRIVATE_ML_BSSN_STRUCT.ML_trace_curv_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt1_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__Xt1_bound_limit PRIVATE_ML_BSSN_STRUCT.Xt1_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt1_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__Xt1_bound_scalar PRIVATE_ML_BSSN_STRUCT.Xt1_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt1_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__Xt1_bound_speed PRIVATE_ML_BSSN_STRUCT.Xt1_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt2_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__Xt2_bound_limit PRIVATE_ML_BSSN_STRUCT.Xt2_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt2_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__Xt2_bound_scalar PRIVATE_ML_BSSN_STRUCT.Xt2_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt2_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__Xt2_bound_speed PRIVATE_ML_BSSN_STRUCT.Xt2_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt3_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__Xt3_bound_limit PRIVATE_ML_BSSN_STRUCT.Xt3_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt3_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__Xt3_bound_scalar PRIVATE_ML_BSSN_STRUCT.Xt3_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt3_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__Xt3_bound_speed PRIVATE_ML_BSSN_STRUCT.Xt3_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__alpha_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__alpha_bound_limit PRIVATE_ML_BSSN_STRUCT.alpha_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__alpha_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__alpha_bound_scalar PRIVATE_ML_BSSN_STRUCT.alpha_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__alpha_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__alpha_bound_speed PRIVATE_ML_BSSN_STRUCT.alpha_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta1_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__beta1_bound_limit PRIVATE_ML_BSSN_STRUCT.beta1_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta1_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__beta1_bound_scalar PRIVATE_ML_BSSN_STRUCT.beta1_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta1_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__beta1_bound_speed PRIVATE_ML_BSSN_STRUCT.beta1_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta2_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__beta2_bound_limit PRIVATE_ML_BSSN_STRUCT.beta2_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta2_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__beta2_bound_scalar PRIVATE_ML_BSSN_STRUCT.beta2_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta2_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__beta2_bound_speed PRIVATE_ML_BSSN_STRUCT.beta2_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta3_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__beta3_bound_limit PRIVATE_ML_BSSN_STRUCT.beta3_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta3_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__beta3_bound_scalar PRIVATE_ML_BSSN_STRUCT.beta3_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta3_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__beta3_bound_speed PRIVATE_ML_BSSN_STRUCT.beta3_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt11_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt11_bound_limit PRIVATE_ML_BSSN_STRUCT.gt11_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt11_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt11_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt11_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt11_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt11_bound_speed PRIVATE_ML_BSSN_STRUCT.gt11_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt12_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt12_bound_limit PRIVATE_ML_BSSN_STRUCT.gt12_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt12_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt12_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt12_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt12_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt12_bound_speed PRIVATE_ML_BSSN_STRUCT.gt12_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt13_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt13_bound_limit PRIVATE_ML_BSSN_STRUCT.gt13_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt13_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt13_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt13_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt13_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt13_bound_speed PRIVATE_ML_BSSN_STRUCT.gt13_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt22_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt22_bound_limit PRIVATE_ML_BSSN_STRUCT.gt22_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt22_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt22_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt22_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt22_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt22_bound_speed PRIVATE_ML_BSSN_STRUCT.gt22_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt23_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt23_bound_limit PRIVATE_ML_BSSN_STRUCT.gt23_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt23_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt23_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt23_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt23_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt23_bound_speed PRIVATE_ML_BSSN_STRUCT.gt23_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt33_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__gt33_bound_limit PRIVATE_ML_BSSN_STRUCT.gt33_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt33_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__gt33_bound_scalar PRIVATE_ML_BSSN_STRUCT.gt33_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt33_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__gt33_bound_speed PRIVATE_ML_BSSN_STRUCT.gt33_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__phi_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__phi_bound_limit PRIVATE_ML_BSSN_STRUCT.phi_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__phi_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__phi_bound_scalar PRIVATE_ML_BSSN_STRUCT.phi_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__phi_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__phi_bound_speed PRIVATE_ML_BSSN_STRUCT.phi_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__trK_bound_limit
#  define CCTK_PARAMETER__ML_BSSN__trK_bound_limit PRIVATE_ML_BSSN_STRUCT.trK_bound_limit
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__trK_bound_scalar
#  define CCTK_PARAMETER__ML_BSSN__trK_bound_scalar PRIVATE_ML_BSSN_STRUCT.trK_bound_scalar
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__trK_bound_speed
#  define CCTK_PARAMETER__ML_BSSN__trK_bound_speed PRIVATE_ML_BSSN_STRUCT.trK_bound_speed
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__A_bound
#  define CCTK_PARAMETER__ML_BSSN__A_bound PRIVATE_ML_BSSN_STRUCT.A_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At11_bound
#  define CCTK_PARAMETER__ML_BSSN__At11_bound PRIVATE_ML_BSSN_STRUCT.At11_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At12_bound
#  define CCTK_PARAMETER__ML_BSSN__At12_bound PRIVATE_ML_BSSN_STRUCT.At12_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At13_bound
#  define CCTK_PARAMETER__ML_BSSN__At13_bound PRIVATE_ML_BSSN_STRUCT.At13_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At22_bound
#  define CCTK_PARAMETER__ML_BSSN__At22_bound PRIVATE_ML_BSSN_STRUCT.At22_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At23_bound
#  define CCTK_PARAMETER__ML_BSSN__At23_bound PRIVATE_ML_BSSN_STRUCT.At23_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__At33_bound
#  define CCTK_PARAMETER__ML_BSSN__At33_bound PRIVATE_ML_BSSN_STRUCT.At33_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B1_bound
#  define CCTK_PARAMETER__ML_BSSN__B1_bound PRIVATE_ML_BSSN_STRUCT.B1_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B2_bound
#  define CCTK_PARAMETER__ML_BSSN__B2_bound PRIVATE_ML_BSSN_STRUCT.B2_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__B3_bound
#  define CCTK_PARAMETER__ML_BSSN__B3_bound PRIVATE_ML_BSSN_STRUCT.B3_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_Gamma_bound PRIVATE_ML_BSSN_STRUCT.ML_Gamma_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_curv_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_curv_bound PRIVATE_ML_BSSN_STRUCT.ML_curv_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_dtlapse_bound PRIVATE_ML_BSSN_STRUCT.ML_dtlapse_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_dtshift_bound PRIVATE_ML_BSSN_STRUCT.ML_dtshift_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_lapse_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_lapse_bound PRIVATE_ML_BSSN_STRUCT.ML_lapse_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_log_confac_bound PRIVATE_ML_BSSN_STRUCT.ML_log_confac_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_metric_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_metric_bound PRIVATE_ML_BSSN_STRUCT.ML_metric_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_shift_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_shift_bound PRIVATE_ML_BSSN_STRUCT.ML_shift_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound
#  define CCTK_PARAMETER__ML_BSSN__ML_trace_curv_bound PRIVATE_ML_BSSN_STRUCT.ML_trace_curv_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt1_bound
#  define CCTK_PARAMETER__ML_BSSN__Xt1_bound PRIVATE_ML_BSSN_STRUCT.Xt1_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt2_bound
#  define CCTK_PARAMETER__ML_BSSN__Xt2_bound PRIVATE_ML_BSSN_STRUCT.Xt2_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__Xt3_bound
#  define CCTK_PARAMETER__ML_BSSN__Xt3_bound PRIVATE_ML_BSSN_STRUCT.Xt3_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__alpha_bound
#  define CCTK_PARAMETER__ML_BSSN__alpha_bound PRIVATE_ML_BSSN_STRUCT.alpha_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__apply_dissipation
#  define CCTK_PARAMETER__ML_BSSN__apply_dissipation PRIVATE_ML_BSSN_STRUCT.apply_dissipation
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta1_bound
#  define CCTK_PARAMETER__ML_BSSN__beta1_bound PRIVATE_ML_BSSN_STRUCT.beta1_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta2_bound
#  define CCTK_PARAMETER__ML_BSSN__beta2_bound PRIVATE_ML_BSSN_STRUCT.beta2_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__beta3_bound
#  define CCTK_PARAMETER__ML_BSSN__beta3_bound PRIVATE_ML_BSSN_STRUCT.beta3_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__dt_lapse_shift_method
#  define CCTK_PARAMETER__ML_BSSN__dt_lapse_shift_method PRIVATE_ML_BSSN_STRUCT.dt_lapse_shift_method
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt11_bound
#  define CCTK_PARAMETER__ML_BSSN__gt11_bound PRIVATE_ML_BSSN_STRUCT.gt11_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt12_bound
#  define CCTK_PARAMETER__ML_BSSN__gt12_bound PRIVATE_ML_BSSN_STRUCT.gt12_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt13_bound
#  define CCTK_PARAMETER__ML_BSSN__gt13_bound PRIVATE_ML_BSSN_STRUCT.gt13_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt22_bound
#  define CCTK_PARAMETER__ML_BSSN__gt22_bound PRIVATE_ML_BSSN_STRUCT.gt22_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt23_bound
#  define CCTK_PARAMETER__ML_BSSN__gt23_bound PRIVATE_ML_BSSN_STRUCT.gt23_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__gt33_bound
#  define CCTK_PARAMETER__ML_BSSN__gt33_bound PRIVATE_ML_BSSN_STRUCT.gt33_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__my_boundary_condition
#  define CCTK_PARAMETER__ML_BSSN__my_boundary_condition PRIVATE_ML_BSSN_STRUCT.my_boundary_condition
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__my_initial_data
#  define CCTK_PARAMETER__ML_BSSN__my_initial_data PRIVATE_ML_BSSN_STRUCT.my_initial_data
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__phi_bound
#  define CCTK_PARAMETER__ML_BSSN__phi_bound PRIVATE_ML_BSSN_STRUCT.phi_bound
#endif
#ifndef CCTK_PARAMETER__ML_BSSN__trK_bound
#  define CCTK_PARAMETER__ML_BSSN__trK_bound PRIVATE_ML_BSSN_STRUCT.trK_bound
#endif

