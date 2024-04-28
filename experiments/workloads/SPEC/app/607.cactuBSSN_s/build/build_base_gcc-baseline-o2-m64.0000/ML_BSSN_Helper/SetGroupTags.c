#ifdef SPEC 
#include "ML_BSSN_Helper/cctk.h" 
#define THORN_DIR "ML_BSSN_Helper" 
#else 
#include <cctk.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN_Helper/cctk_Parameters.h" 
#else 
#include <cctk_Parameters.h>
#endif /* END SPEC */ 
#include <util_Table.h>

#include <assert.h>

#define THORN_ML_BSSN           // "ML_BSSN" will be replaced
#ifdef THORN_ML_CCZ4
#  define HAVE_THETA
#endif

static void
set_group_tags (int const checkpoint,
                int const persistent,
                int const prolongate,
                char const * restrict const gn);

int
ML_BSSN_SetGroupTags (void)
{
  DECLARE_CCTK_PARAMETERS
  
  int const checkpoint = timelevels > 1;
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::metric");
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::curv");
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::lapse");
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::shift");
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::dtlapse");
  set_group_tags (checkpoint, checkpoint, 1, "ADMBase::dtshift");
  
  set_group_tags (checkpoint, checkpoint, 0, "ML_BSSN::ML_cons_detg");
  set_group_tags (checkpoint, checkpoint, 0, "ML_BSSN::ML_cons_Gamma");
  set_group_tags (checkpoint, checkpoint, 0, "ML_BSSN::ML_cons_traceA");
  set_group_tags (checkpoint, checkpoint, 0, "ML_BSSN::ML_Ham");
  set_group_tags (checkpoint, checkpoint, 0, "ML_BSSN::ML_mom");
  
  int const rhs_checkpoint = rhs_timelevels > 1;
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_log_confacrhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_metricrhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_Gammarhs");
  
#ifdef HAVE_THETA
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_Thetarhs");
#endif
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_trace_curvrhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_curvrhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_lapserhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_dtlapserhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_shiftrhs");
  set_group_tags (rhs_checkpoint, rhs_checkpoint, 0, "ML_BSSN::ML_dtshiftrhs");
  
  return 0;
}

static void
set_group_tags (int const checkpoint,
                int const persistent,
                int const prolongate,
                char const * restrict const gn)
{
  assert (gn);
  
  int const gi = CCTK_GroupIndex (gn);
  assert (gi >= 0);
  
  int const table = CCTK_GroupTagsTableI (gi);
  assert (table >= 0);
  
  if (! checkpoint) {
    int const ierr = Util_TableSetString (table, "no", "Checkpoint");
    assert (! ierr);
  }
  
  if (! persistent) {
    int const ierr = Util_TableSetString (table, "no", "Persistent");
    assert (! ierr);
  }
  
  if (! prolongate) {
    int const iret = Util_TableDeleteKey (table, "ProlongationParameter");
    assert (iret == 0 || iret == UTIL_ERROR_TABLE_NO_SUCH_KEY);
    int const ierr = Util_TableSetString (table, "none", "Prolongation");
    assert (ierr == 0 || ierr == 1); /* 0 means key did not exist before.  1 means key existed before and has now been reset */
  }
}
