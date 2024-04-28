#include <stdio.h>

#include "cctki_ActiveThorns.h"

void CCTKi_BindingsThorn_IOASCII(void);
void CCTKi_BindingsThorn_IOASCII(void)
{
  const char *name[] = {"IOASCII", 0};
  const char *implementation[] = {"IOASCII", 0};
  const char *requires_thorns[] =
  {
    "IOUtil",
    0,
  };

  const char *activates_thorns[] =
  {
    "IOUtil",
    0,
  };

  /*
   * Should be able to do below with a constant initializer
   * but sr8000 compiler doesn't like it.
   * So have to laboriously assign values to each member of array.
   */
  struct iAttributeList attributes[5];

  attributes[0].attribute                = "name";
  attributes[0].AttributeData.StringList = name;
  attributes[1].attribute                = "implementation";
  attributes[1].AttributeData.StringList = implementation;
  attributes[2].attribute                = "requires thorns";
  attributes[2].AttributeData.StringList = requires_thorns;
  attributes[3].attribute                = "activates thorns";
  attributes[3].AttributeData.StringList = activates_thorns;
  attributes[4].attribute                = 0;
  attributes[4].AttributeData.StringList = 0;

  CCTKi_RegisterThorn(attributes);
}
