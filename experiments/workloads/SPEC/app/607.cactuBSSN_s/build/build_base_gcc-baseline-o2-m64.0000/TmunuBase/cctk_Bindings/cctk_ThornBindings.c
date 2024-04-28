#include <stdio.h>

#include "cctki_ActiveThorns.h"

void CCTKi_BindingsThorn_TmunuBase(void);
void CCTKi_BindingsThorn_TmunuBase(void)
{
  const char *name[] = {"TmunuBase", 0};
  const char *implementation[] = {"TmunuBase", 0};
  const char *ancestors[] =
  {
    "ADMBASE",
    "COORDBASE",
    "GRID",
    "STATICCONFORMAL",
    0,
  };

  const char *friends[] =
  {
    "ADMCoupling",
    0,
  };

  const char *requires_thorns[] =
  {
    "ADMMacros",
    0,
  };

  /*
   * Should be able to do below with a constant initializer
   * but sr8000 compiler doesn't like it.
   * So have to laboriously assign values to each member of array.
   */
  struct iAttributeList attributes[6];

  attributes[0].attribute                = "name";
  attributes[0].AttributeData.StringList = name;
  attributes[1].attribute                = "implementation";
  attributes[1].AttributeData.StringList = implementation;
  attributes[2].attribute                = "ancestors";
  attributes[2].AttributeData.StringList = ancestors;
  attributes[3].attribute                = "friends";
  attributes[3].AttributeData.StringList = friends;
  attributes[4].attribute                = "requires thorns";
  attributes[4].AttributeData.StringList = requires_thorns;
  attributes[5].attribute                = 0;
  attributes[5].AttributeData.StringList = 0;

  CCTKi_RegisterThorn(attributes);
}
