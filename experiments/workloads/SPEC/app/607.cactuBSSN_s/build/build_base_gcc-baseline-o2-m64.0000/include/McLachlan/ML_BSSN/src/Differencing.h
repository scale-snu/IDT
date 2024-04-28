#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder21(u) ((-KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0))*p1o2dx)
#else
#  define PDstandardNthfdOrder21(u) (PDstandardNthfdOrder21_impl(u,p1o2dx,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0))*p1o2dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder22(u) ((-KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0))*p1o2dy)
#else
#  define PDstandardNthfdOrder22(u) (PDstandardNthfdOrder22_impl(u,p1o2dy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0))*p1o2dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder23(u) ((-KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1))*p1o2dz)
#else
#  define PDstandardNthfdOrder23(u) (PDstandardNthfdOrder23_impl(u,p1o2dz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o2dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder22_impl(u, p1o2dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder41(u) ((-8*KRANC_GFOFFSET3D(u,-1,0,0) + 8*KRANC_GFOFFSET3D(u,1,0,0) + KRANC_GFOFFSET3D(u,-2,0,0) - KRANC_GFOFFSET3D(u,2,0,0))*p1o12dx)
#else
#  define PDstandardNthfdOrder41(u) (PDstandardNthfdOrder41_impl(u,p1o12dx,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-8*KRANC_GFOFFSET3D(u,-1,0,0) + 8*KRANC_GFOFFSET3D(u,1,0,0) + KRANC_GFOFFSET3D(u,-2,0,0) - KRANC_GFOFFSET3D(u,2,0,0))*p1o12dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder42(u) ((-8*KRANC_GFOFFSET3D(u,0,-1,0) + 8*KRANC_GFOFFSET3D(u,0,1,0) + KRANC_GFOFFSET3D(u,0,-2,0) - KRANC_GFOFFSET3D(u,0,2,0))*p1o12dy)
#else
#  define PDstandardNthfdOrder42(u) (PDstandardNthfdOrder42_impl(u,p1o12dy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-8*KRANC_GFOFFSET3D(u,0,-1,0) + 8*KRANC_GFOFFSET3D(u,0,1,0) + KRANC_GFOFFSET3D(u,0,-2,0) - KRANC_GFOFFSET3D(u,0,2,0))*p1o12dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder43(u) ((-8*KRANC_GFOFFSET3D(u,0,0,-1) + 8*KRANC_GFOFFSET3D(u,0,0,1) + KRANC_GFOFFSET3D(u,0,0,-2) - KRANC_GFOFFSET3D(u,0,0,2))*p1o12dz)
#else
#  define PDstandardNthfdOrder43(u) (PDstandardNthfdOrder43_impl(u,p1o12dz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder42_impl(u, p1o12dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder61(u) ((-45*KRANC_GFOFFSET3D(u,-1,0,0) + 45*KRANC_GFOFFSET3D(u,1,0,0) + 9*KRANC_GFOFFSET3D(u,-2,0,0) - 9*KRANC_GFOFFSET3D(u,2,0,0) - KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o60dx)
#else
#  define PDstandardNthfdOrder61(u) (PDstandardNthfdOrder61_impl(u,p1o60dx,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-45*KRANC_GFOFFSET3D(u,-1,0,0) + 45*KRANC_GFOFFSET3D(u,1,0,0) + 9*KRANC_GFOFFSET3D(u,-2,0,0) - 9*KRANC_GFOFFSET3D(u,2,0,0) - KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o60dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder62(u) ((-45*KRANC_GFOFFSET3D(u,0,-1,0) + 45*KRANC_GFOFFSET3D(u,0,1,0) + 9*KRANC_GFOFFSET3D(u,0,-2,0) - 9*KRANC_GFOFFSET3D(u,0,2,0) - KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o60dy)
#else
#  define PDstandardNthfdOrder62(u) (PDstandardNthfdOrder62_impl(u,p1o60dy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-45*KRANC_GFOFFSET3D(u,0,-1,0) + 45*KRANC_GFOFFSET3D(u,0,1,0) + 9*KRANC_GFOFFSET3D(u,0,-2,0) - 9*KRANC_GFOFFSET3D(u,0,2,0) - KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o60dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder63(u) ((-45*KRANC_GFOFFSET3D(u,0,0,-1) + 45*KRANC_GFOFFSET3D(u,0,0,1) + 9*KRANC_GFOFFSET3D(u,0,0,-2) - 9*KRANC_GFOFFSET3D(u,0,0,2) - KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3))*p1o60dz)
#else
#  define PDstandardNthfdOrder63(u) (PDstandardNthfdOrder63_impl(u,p1o60dz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o60dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder62_impl(u, p1o60dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder81(u) ((-672*KRANC_GFOFFSET3D(u,-1,0,0) + 672*KRANC_GFOFFSET3D(u,1,0,0) + 168*KRANC_GFOFFSET3D(u,-2,0,0) - 168*KRANC_GFOFFSET3D(u,2,0,0) - 32*KRANC_GFOFFSET3D(u,-3,0,0) + 32*KRANC_GFOFFSET3D(u,3,0,0) + 3*KRANC_GFOFFSET3D(u,-4,0,0) - 3*KRANC_GFOFFSET3D(u,4,0,0))*p1o840dx)
#else
#  define PDstandardNthfdOrder81(u) (PDstandardNthfdOrder81_impl(u,p1o840dx,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-672*KRANC_GFOFFSET3D(u,-1,0,0) + 672*KRANC_GFOFFSET3D(u,1,0,0) + 168*KRANC_GFOFFSET3D(u,-2,0,0) - 168*KRANC_GFOFFSET3D(u,2,0,0) - 32*KRANC_GFOFFSET3D(u,-3,0,0) + 32*KRANC_GFOFFSET3D(u,3,0,0) + 3*KRANC_GFOFFSET3D(u,-4,0,0) - 3*KRANC_GFOFFSET3D(u,4,0,0))*p1o840dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder82(u) ((-672*KRANC_GFOFFSET3D(u,0,-1,0) + 672*KRANC_GFOFFSET3D(u,0,1,0) + 168*KRANC_GFOFFSET3D(u,0,-2,0) - 168*KRANC_GFOFFSET3D(u,0,2,0) - 32*KRANC_GFOFFSET3D(u,0,-3,0) + 32*KRANC_GFOFFSET3D(u,0,3,0) + 3*KRANC_GFOFFSET3D(u,0,-4,0) - 3*KRANC_GFOFFSET3D(u,0,4,0))*p1o840dy)
#else
#  define PDstandardNthfdOrder82(u) (PDstandardNthfdOrder82_impl(u,p1o840dy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-672*KRANC_GFOFFSET3D(u,0,-1,0) + 672*KRANC_GFOFFSET3D(u,0,1,0) + 168*KRANC_GFOFFSET3D(u,0,-2,0) - 168*KRANC_GFOFFSET3D(u,0,2,0) - 32*KRANC_GFOFFSET3D(u,0,-3,0) + 32*KRANC_GFOFFSET3D(u,0,3,0) + 3*KRANC_GFOFFSET3D(u,0,-4,0) - 3*KRANC_GFOFFSET3D(u,0,4,0))*p1o840dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder83(u) ((-672*KRANC_GFOFFSET3D(u,0,0,-1) + 672*KRANC_GFOFFSET3D(u,0,0,1) + 168*KRANC_GFOFFSET3D(u,0,0,-2) - 168*KRANC_GFOFFSET3D(u,0,0,2) - 32*KRANC_GFOFFSET3D(u,0,0,-3) + 32*KRANC_GFOFFSET3D(u,0,0,3) + 3*KRANC_GFOFFSET3D(u,0,0,-4) - 3*KRANC_GFOFFSET3D(u,0,0,4))*p1o840dz)
#else
#  define PDstandardNthfdOrder83(u) (PDstandardNthfdOrder83_impl(u,p1o840dz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o840dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder82_impl(u, p1o840dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder211(u) ((-2*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0))*p1odx2)
#else
#  define PDstandardNthfdOrder211(u) (PDstandardNthfdOrder211_impl(u,p1odx2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder211_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odx2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder211_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odx2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-2*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0))*p1odx2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder222(u) ((-2*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0))*p1ody2)
#else
#  define PDstandardNthfdOrder222(u) (PDstandardNthfdOrder222_impl(u,p1ody2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder222_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1ody2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder222_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1ody2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-2*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0))*p1ody2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder233(u) ((-2*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1))*p1odz2)
#else
#  define PDstandardNthfdOrder233(u) (PDstandardNthfdOrder233_impl(u,p1odz2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder233_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odz2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder233_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odz2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder222_impl(u, p1odz2, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder411(u) ((30*KRANC_GFOFFSET3D(u,0,0,0) - 16*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o12dx2)
#else
#  define PDstandardNthfdOrder411(u) (PDstandardNthfdOrder411_impl(u,pm1o12dx2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder411_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dx2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder411_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dx2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (30*KRANC_GFOFFSET3D(u,0,0,0) - 16*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o12dx2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder422(u) ((30*KRANC_GFOFFSET3D(u,0,0,0) - 16*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o12dy2)
#else
#  define PDstandardNthfdOrder422(u) (PDstandardNthfdOrder422_impl(u,pm1o12dy2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder422_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dy2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder422_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dy2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (30*KRANC_GFOFFSET3D(u,0,0,0) - 16*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o12dy2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder433(u) ((30*KRANC_GFOFFSET3D(u,0,0,0) - 16*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) + KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2))*pm1o12dz2)
#else
#  define PDstandardNthfdOrder433(u) (PDstandardNthfdOrder433_impl(u,pm1o12dz2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder433_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dz2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder433_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o12dz2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder422_impl(u, pm1o12dz2, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder611(u) ((-490*KRANC_GFOFFSET3D(u,0,0,0) + 270*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 27*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 2*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)))*p1o180dx2)
#else
#  define PDstandardNthfdOrder611(u) (PDstandardNthfdOrder611_impl(u,p1o180dx2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder611_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dx2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder611_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dx2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-490*KRANC_GFOFFSET3D(u,0,0,0) + 270*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 27*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 2*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)))*p1o180dx2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder622(u) ((-490*KRANC_GFOFFSET3D(u,0,0,0) + 270*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 27*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 2*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)))*p1o180dy2)
#else
#  define PDstandardNthfdOrder622(u) (PDstandardNthfdOrder622_impl(u,p1o180dy2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder622_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dy2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder622_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dy2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-490*KRANC_GFOFFSET3D(u,0,0,0) + 270*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 27*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 2*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)))*p1o180dy2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder633(u) ((-490*KRANC_GFOFFSET3D(u,0,0,0) + 270*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 27*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + 2*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)))*p1o180dz2)
#else
#  define PDstandardNthfdOrder633(u) (PDstandardNthfdOrder633_impl(u,p1o180dz2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder633_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dz2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder633_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o180dz2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder622_impl(u, p1o180dz2, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder811(u) ((-14350*KRANC_GFOFFSET3D(u,0,0,0) + 8064*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 1008*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 128*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 9*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)))*p1o5040dx2)
#else
#  define PDstandardNthfdOrder811(u) (PDstandardNthfdOrder811_impl(u,p1o5040dx2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder811_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dx2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder811_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dx2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-14350*KRANC_GFOFFSET3D(u,0,0,0) + 8064*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 1008*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 128*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 9*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)))*p1o5040dx2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder822(u) ((-14350*KRANC_GFOFFSET3D(u,0,0,0) + 8064*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 1008*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 128*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 9*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)))*p1o5040dy2)
#else
#  define PDstandardNthfdOrder822(u) (PDstandardNthfdOrder822_impl(u,p1o5040dy2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder822_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dy2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder822_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dy2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-14350*KRANC_GFOFFSET3D(u,0,0,0) + 8064*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 1008*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 128*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 9*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)))*p1o5040dy2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder833(u) ((-14350*KRANC_GFOFFSET3D(u,0,0,0) + 8064*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 1008*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + 128*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)) - 9*(KRANC_GFOFFSET3D(u,0,0,-4) + KRANC_GFOFFSET3D(u,0,0,4)))*p1o5040dz2)
#else
#  define PDstandardNthfdOrder833(u) (PDstandardNthfdOrder833_impl(u,p1o5040dz2,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder833_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dz2, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder833_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o5040dz2, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder822_impl(u, p1o5040dz2, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder212(u) ((KRANC_GFOFFSET3D(u,-1,-1,0) - KRANC_GFOFFSET3D(u,-1,1,0) - KRANC_GFOFFSET3D(u,1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0))*p1o4dxdy)
#else
#  define PDstandardNthfdOrder212(u) (PDstandardNthfdOrder212_impl(u,p1o4dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder212_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder212_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (KRANC_GFOFFSET3D(u,-1,-1,0) - KRANC_GFOFFSET3D(u,-1,1,0) - KRANC_GFOFFSET3D(u,1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0))*p1o4dxdy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder213(u) ((KRANC_GFOFFSET3D(u,-1,0,-1) - KRANC_GFOFFSET3D(u,-1,0,1) - KRANC_GFOFFSET3D(u,1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1))*p1o4dxdz)
#else
#  define PDstandardNthfdOrder213(u) (PDstandardNthfdOrder213_impl(u,p1o4dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder213_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder213_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder212_impl(u, p1o4dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder221(u) ((KRANC_GFOFFSET3D(u,-1,-1,0) - KRANC_GFOFFSET3D(u,-1,1,0) - KRANC_GFOFFSET3D(u,1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0))*p1o4dxdy)
#else
#  define PDstandardNthfdOrder221(u) (PDstandardNthfdOrder221_impl(u,p1o4dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder221_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder221_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder212_impl(u, p1o4dxdy, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder223(u) ((KRANC_GFOFFSET3D(u,0,-1,-1) - KRANC_GFOFFSET3D(u,0,-1,1) - KRANC_GFOFFSET3D(u,0,1,-1) + KRANC_GFOFFSET3D(u,0,1,1))*p1o4dydz)
#else
#  define PDstandardNthfdOrder223(u) (PDstandardNthfdOrder223_impl(u,p1o4dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder223_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder223_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (KRANC_GFOFFSET3D(u,0,-1,-1) - KRANC_GFOFFSET3D(u,0,-1,1) - KRANC_GFOFFSET3D(u,0,1,-1) + KRANC_GFOFFSET3D(u,0,1,1))*p1o4dydz;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder231(u) ((KRANC_GFOFFSET3D(u,-1,0,-1) - KRANC_GFOFFSET3D(u,-1,0,1) - KRANC_GFOFFSET3D(u,1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1))*p1o4dxdz)
#else
#  define PDstandardNthfdOrder231(u) (PDstandardNthfdOrder231_impl(u,p1o4dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder231_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder231_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder212_impl(u, p1o4dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder232(u) ((KRANC_GFOFFSET3D(u,0,-1,-1) - KRANC_GFOFFSET3D(u,0,-1,1) - KRANC_GFOFFSET3D(u,0,1,-1) + KRANC_GFOFFSET3D(u,0,1,1))*p1o4dydz)
#else
#  define PDstandardNthfdOrder232(u) (PDstandardNthfdOrder232_impl(u,p1o4dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder232_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder232_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder223_impl(u, p1o4dydz, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder412(u) ((-64*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 64*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 8*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 8*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) + KRANC_GFOFFSET3D(u,-2,-2,0) - KRANC_GFOFFSET3D(u,-2,2,0) - KRANC_GFOFFSET3D(u,2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0))*p1o144dxdy)
#else
#  define PDstandardNthfdOrder412(u) (PDstandardNthfdOrder412_impl(u,p1o144dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder412_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder412_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-64*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 64*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 8*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 8*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) + KRANC_GFOFFSET3D(u,-2,-2,0) - KRANC_GFOFFSET3D(u,-2,2,0) - KRANC_GFOFFSET3D(u,2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0))*p1o144dxdy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder413(u) ((-64*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 64*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 8*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 8*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) + KRANC_GFOFFSET3D(u,-2,0,-2) - KRANC_GFOFFSET3D(u,-2,0,2) - KRANC_GFOFFSET3D(u,2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2))*p1o144dxdz)
#else
#  define PDstandardNthfdOrder413(u) (PDstandardNthfdOrder413_impl(u,p1o144dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder413_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder413_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder412_impl(u, p1o144dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder421(u) ((-64*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 64*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 8*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 8*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) + KRANC_GFOFFSET3D(u,-2,-2,0) - KRANC_GFOFFSET3D(u,-2,2,0) - KRANC_GFOFFSET3D(u,2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0))*p1o144dxdy)
#else
#  define PDstandardNthfdOrder421(u) (PDstandardNthfdOrder421_impl(u,p1o144dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder421_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder421_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder412_impl(u, p1o144dxdy, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder423(u) ((-64*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 64*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 8*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 8*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) + KRANC_GFOFFSET3D(u,0,-2,-2) - KRANC_GFOFFSET3D(u,0,-2,2) - KRANC_GFOFFSET3D(u,0,2,-2) + KRANC_GFOFFSET3D(u,0,2,2))*p1o144dydz)
#else
#  define PDstandardNthfdOrder423(u) (PDstandardNthfdOrder423_impl(u,p1o144dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder423_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder423_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-64*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 64*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 8*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 8*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) + KRANC_GFOFFSET3D(u,0,-2,-2) - KRANC_GFOFFSET3D(u,0,-2,2) - KRANC_GFOFFSET3D(u,0,2,-2) + KRANC_GFOFFSET3D(u,0,2,2))*p1o144dydz;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder431(u) ((-64*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 64*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 8*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 8*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) + KRANC_GFOFFSET3D(u,-2,0,-2) - KRANC_GFOFFSET3D(u,-2,0,2) - KRANC_GFOFFSET3D(u,2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2))*p1o144dxdz)
#else
#  define PDstandardNthfdOrder431(u) (PDstandardNthfdOrder431_impl(u,p1o144dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder431_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder431_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder412_impl(u, p1o144dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder432(u) ((-64*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 64*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 8*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 8*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) + KRANC_GFOFFSET3D(u,0,-2,-2) - KRANC_GFOFFSET3D(u,0,-2,2) - KRANC_GFOFFSET3D(u,0,2,-2) + KRANC_GFOFFSET3D(u,0,2,2))*p1o144dydz)
#else
#  define PDstandardNthfdOrder432(u) (PDstandardNthfdOrder432_impl(u,p1o144dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder432_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder432_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o144dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder423_impl(u, p1o144dydz, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder612(u) ((-2025*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 2025*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 405*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 405*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 81*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 81*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 45*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 45*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 9*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 9*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) + KRANC_GFOFFSET3D(u,-3,-3,0) - KRANC_GFOFFSET3D(u,-3,3,0) - KRANC_GFOFFSET3D(u,3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0))*p1o3600dxdy)
#else
#  define PDstandardNthfdOrder612(u) (PDstandardNthfdOrder612_impl(u,p1o3600dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder612_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder612_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-2025*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 2025*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 405*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 405*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 81*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 81*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 45*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 45*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 9*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 9*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) + KRANC_GFOFFSET3D(u,-3,-3,0) - KRANC_GFOFFSET3D(u,-3,3,0) - KRANC_GFOFFSET3D(u,3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0))*p1o3600dxdy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder613(u) ((-2025*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 2025*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 405*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 405*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) - 81*(KRANC_GFOFFSET3D(u,-2,0,2) + KRANC_GFOFFSET3D(u,2,0,-2)) + 81*(KRANC_GFOFFSET3D(u,-2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2)) - 45*(KRANC_GFOFFSET3D(u,-1,0,3) + KRANC_GFOFFSET3D(u,1,0,-3) + KRANC_GFOFFSET3D(u,-3,0,1) + KRANC_GFOFFSET3D(u,3,0,-1)) + 45*(KRANC_GFOFFSET3D(u,-1,0,-3) + KRANC_GFOFFSET3D(u,1,0,3) + KRANC_GFOFFSET3D(u,-3,0,-1) + KRANC_GFOFFSET3D(u,3,0,1)) + 9*(KRANC_GFOFFSET3D(u,-2,0,3) + KRANC_GFOFFSET3D(u,2,0,-3) + KRANC_GFOFFSET3D(u,-3,0,2) + KRANC_GFOFFSET3D(u,3,0,-2)) - 9*(KRANC_GFOFFSET3D(u,-2,0,-3) + KRANC_GFOFFSET3D(u,2,0,3) + KRANC_GFOFFSET3D(u,-3,0,-2) + KRANC_GFOFFSET3D(u,3,0,2)) + KRANC_GFOFFSET3D(u,-3,0,-3) - KRANC_GFOFFSET3D(u,-3,0,3) - KRANC_GFOFFSET3D(u,3,0,-3) + KRANC_GFOFFSET3D(u,3,0,3))*p1o3600dxdz)
#else
#  define PDstandardNthfdOrder613(u) (PDstandardNthfdOrder613_impl(u,p1o3600dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder613_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder613_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder612_impl(u, p1o3600dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder621(u) ((-2025*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 2025*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 405*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 405*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 81*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 81*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 45*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 45*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 9*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 9*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) + KRANC_GFOFFSET3D(u,-3,-3,0) - KRANC_GFOFFSET3D(u,-3,3,0) - KRANC_GFOFFSET3D(u,3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0))*p1o3600dxdy)
#else
#  define PDstandardNthfdOrder621(u) (PDstandardNthfdOrder621_impl(u,p1o3600dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder621_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder621_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder612_impl(u, p1o3600dxdy, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder623(u) ((-2025*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 2025*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 405*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 405*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 81*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 81*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 45*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 45*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 9*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 9*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) + KRANC_GFOFFSET3D(u,0,-3,-3) - KRANC_GFOFFSET3D(u,0,-3,3) - KRANC_GFOFFSET3D(u,0,3,-3) + KRANC_GFOFFSET3D(u,0,3,3))*p1o3600dydz)
#else
#  define PDstandardNthfdOrder623(u) (PDstandardNthfdOrder623_impl(u,p1o3600dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder623_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder623_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-2025*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 2025*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 405*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 405*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 81*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 81*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 45*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 45*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 9*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 9*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) + KRANC_GFOFFSET3D(u,0,-3,-3) - KRANC_GFOFFSET3D(u,0,-3,3) - KRANC_GFOFFSET3D(u,0,3,-3) + KRANC_GFOFFSET3D(u,0,3,3))*p1o3600dydz;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder631(u) ((-2025*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 2025*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 405*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 405*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) - 81*(KRANC_GFOFFSET3D(u,-2,0,2) + KRANC_GFOFFSET3D(u,2,0,-2)) + 81*(KRANC_GFOFFSET3D(u,-2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2)) - 45*(KRANC_GFOFFSET3D(u,-1,0,3) + KRANC_GFOFFSET3D(u,1,0,-3) + KRANC_GFOFFSET3D(u,-3,0,1) + KRANC_GFOFFSET3D(u,3,0,-1)) + 45*(KRANC_GFOFFSET3D(u,-1,0,-3) + KRANC_GFOFFSET3D(u,1,0,3) + KRANC_GFOFFSET3D(u,-3,0,-1) + KRANC_GFOFFSET3D(u,3,0,1)) + 9*(KRANC_GFOFFSET3D(u,-2,0,3) + KRANC_GFOFFSET3D(u,2,0,-3) + KRANC_GFOFFSET3D(u,-3,0,2) + KRANC_GFOFFSET3D(u,3,0,-2)) - 9*(KRANC_GFOFFSET3D(u,-2,0,-3) + KRANC_GFOFFSET3D(u,2,0,3) + KRANC_GFOFFSET3D(u,-3,0,-2) + KRANC_GFOFFSET3D(u,3,0,2)) + KRANC_GFOFFSET3D(u,-3,0,-3) - KRANC_GFOFFSET3D(u,-3,0,3) - KRANC_GFOFFSET3D(u,3,0,-3) + KRANC_GFOFFSET3D(u,3,0,3))*p1o3600dxdz)
#else
#  define PDstandardNthfdOrder631(u) (PDstandardNthfdOrder631_impl(u,p1o3600dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder631_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder631_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder612_impl(u, p1o3600dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder632(u) ((-2025*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 2025*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 405*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 405*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 81*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 81*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 45*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 45*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 9*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 9*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) + KRANC_GFOFFSET3D(u,0,-3,-3) - KRANC_GFOFFSET3D(u,0,-3,3) - KRANC_GFOFFSET3D(u,0,3,-3) + KRANC_GFOFFSET3D(u,0,3,3))*p1o3600dydz)
#else
#  define PDstandardNthfdOrder632(u) (PDstandardNthfdOrder632_impl(u,p1o3600dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder632_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder632_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o3600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder623_impl(u, p1o3600dydz, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder812(u) ((-451584*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 451584*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 112896*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 112896*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 28224*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 28224*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 21504*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 21504*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 5376*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 5376*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) - 1024*(KRANC_GFOFFSET3D(u,-3,3,0) + KRANC_GFOFFSET3D(u,3,-3,0)) + 1024*(KRANC_GFOFFSET3D(u,-3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0)) + 2016*(KRANC_GFOFFSET3D(u,-1,4,0) + KRANC_GFOFFSET3D(u,1,-4,0) + KRANC_GFOFFSET3D(u,-4,1,0) + KRANC_GFOFFSET3D(u,4,-1,0)) - 2016*(KRANC_GFOFFSET3D(u,-1,-4,0) + KRANC_GFOFFSET3D(u,1,4,0) + KRANC_GFOFFSET3D(u,-4,-1,0) + KRANC_GFOFFSET3D(u,4,1,0)) - 504*(KRANC_GFOFFSET3D(u,-2,4,0) + KRANC_GFOFFSET3D(u,2,-4,0) + KRANC_GFOFFSET3D(u,-4,2,0) + KRANC_GFOFFSET3D(u,4,-2,0)) + 504*(KRANC_GFOFFSET3D(u,-2,-4,0) + KRANC_GFOFFSET3D(u,2,4,0) + KRANC_GFOFFSET3D(u,-4,-2,0) + KRANC_GFOFFSET3D(u,4,2,0)) + 96*(KRANC_GFOFFSET3D(u,-3,4,0) + KRANC_GFOFFSET3D(u,3,-4,0) + KRANC_GFOFFSET3D(u,-4,3,0) + KRANC_GFOFFSET3D(u,4,-3,0)) - 96*(KRANC_GFOFFSET3D(u,-3,-4,0) + KRANC_GFOFFSET3D(u,3,4,0) + KRANC_GFOFFSET3D(u,-4,-3,0) + KRANC_GFOFFSET3D(u,4,3,0)) - 9*(KRANC_GFOFFSET3D(u,-4,4,0) + KRANC_GFOFFSET3D(u,4,-4,0)) + 9*(KRANC_GFOFFSET3D(u,-4,-4,0) + KRANC_GFOFFSET3D(u,4,4,0)))*p1o705600dxdy)
#else
#  define PDstandardNthfdOrder812(u) (PDstandardNthfdOrder812_impl(u,p1o705600dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder812_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder812_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-451584*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 451584*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 112896*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 112896*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 28224*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 28224*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 21504*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 21504*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 5376*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 5376*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) - 1024*(KRANC_GFOFFSET3D(u,-3,3,0) + KRANC_GFOFFSET3D(u,3,-3,0)) + 1024*(KRANC_GFOFFSET3D(u,-3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0)) + 2016*(KRANC_GFOFFSET3D(u,-1,4,0) + KRANC_GFOFFSET3D(u,1,-4,0) + KRANC_GFOFFSET3D(u,-4,1,0) + KRANC_GFOFFSET3D(u,4,-1,0)) - 2016*(KRANC_GFOFFSET3D(u,-1,-4,0) + KRANC_GFOFFSET3D(u,1,4,0) + KRANC_GFOFFSET3D(u,-4,-1,0) + KRANC_GFOFFSET3D(u,4,1,0)) - 504*(KRANC_GFOFFSET3D(u,-2,4,0) + KRANC_GFOFFSET3D(u,2,-4,0) + KRANC_GFOFFSET3D(u,-4,2,0) + KRANC_GFOFFSET3D(u,4,-2,0)) + 504*(KRANC_GFOFFSET3D(u,-2,-4,0) + KRANC_GFOFFSET3D(u,2,4,0) + KRANC_GFOFFSET3D(u,-4,-2,0) + KRANC_GFOFFSET3D(u,4,2,0)) + 96*(KRANC_GFOFFSET3D(u,-3,4,0) + KRANC_GFOFFSET3D(u,3,-4,0) + KRANC_GFOFFSET3D(u,-4,3,0) + KRANC_GFOFFSET3D(u,4,-3,0)) - 96*(KRANC_GFOFFSET3D(u,-3,-4,0) + KRANC_GFOFFSET3D(u,3,4,0) + KRANC_GFOFFSET3D(u,-4,-3,0) + KRANC_GFOFFSET3D(u,4,3,0)) - 9*(KRANC_GFOFFSET3D(u,-4,4,0) + KRANC_GFOFFSET3D(u,4,-4,0)) + 9*(KRANC_GFOFFSET3D(u,-4,-4,0) + KRANC_GFOFFSET3D(u,4,4,0)))*p1o705600dxdy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder813(u) ((-451584*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 451584*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 112896*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 112896*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) - 28224*(KRANC_GFOFFSET3D(u,-2,0,2) + KRANC_GFOFFSET3D(u,2,0,-2)) + 28224*(KRANC_GFOFFSET3D(u,-2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2)) - 21504*(KRANC_GFOFFSET3D(u,-1,0,3) + KRANC_GFOFFSET3D(u,1,0,-3) + KRANC_GFOFFSET3D(u,-3,0,1) + KRANC_GFOFFSET3D(u,3,0,-1)) + 21504*(KRANC_GFOFFSET3D(u,-1,0,-3) + KRANC_GFOFFSET3D(u,1,0,3) + KRANC_GFOFFSET3D(u,-3,0,-1) + KRANC_GFOFFSET3D(u,3,0,1)) + 5376*(KRANC_GFOFFSET3D(u,-2,0,3) + KRANC_GFOFFSET3D(u,2,0,-3) + KRANC_GFOFFSET3D(u,-3,0,2) + KRANC_GFOFFSET3D(u,3,0,-2)) - 5376*(KRANC_GFOFFSET3D(u,-2,0,-3) + KRANC_GFOFFSET3D(u,2,0,3) + KRANC_GFOFFSET3D(u,-3,0,-2) + KRANC_GFOFFSET3D(u,3,0,2)) - 1024*(KRANC_GFOFFSET3D(u,-3,0,3) + KRANC_GFOFFSET3D(u,3,0,-3)) + 1024*(KRANC_GFOFFSET3D(u,-3,0,-3) + KRANC_GFOFFSET3D(u,3,0,3)) + 2016*(KRANC_GFOFFSET3D(u,-1,0,4) + KRANC_GFOFFSET3D(u,1,0,-4) + KRANC_GFOFFSET3D(u,-4,0,1) + KRANC_GFOFFSET3D(u,4,0,-1)) - 2016*(KRANC_GFOFFSET3D(u,-1,0,-4) + KRANC_GFOFFSET3D(u,1,0,4) + KRANC_GFOFFSET3D(u,-4,0,-1) + KRANC_GFOFFSET3D(u,4,0,1)) - 504*(KRANC_GFOFFSET3D(u,-2,0,4) + KRANC_GFOFFSET3D(u,2,0,-4) + KRANC_GFOFFSET3D(u,-4,0,2) + KRANC_GFOFFSET3D(u,4,0,-2)) + 504*(KRANC_GFOFFSET3D(u,-2,0,-4) + KRANC_GFOFFSET3D(u,2,0,4) + KRANC_GFOFFSET3D(u,-4,0,-2) + KRANC_GFOFFSET3D(u,4,0,2)) + 96*(KRANC_GFOFFSET3D(u,-3,0,4) + KRANC_GFOFFSET3D(u,3,0,-4) + KRANC_GFOFFSET3D(u,-4,0,3) + KRANC_GFOFFSET3D(u,4,0,-3)) - 96*(KRANC_GFOFFSET3D(u,-3,0,-4) + KRANC_GFOFFSET3D(u,3,0,4) + KRANC_GFOFFSET3D(u,-4,0,-3) + KRANC_GFOFFSET3D(u,4,0,3)) - 9*(KRANC_GFOFFSET3D(u,-4,0,4) + KRANC_GFOFFSET3D(u,4,0,-4)) + 9*(KRANC_GFOFFSET3D(u,-4,0,-4) + KRANC_GFOFFSET3D(u,4,0,4)))*p1o705600dxdz)
#else
#  define PDstandardNthfdOrder813(u) (PDstandardNthfdOrder813_impl(u,p1o705600dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder813_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder813_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder812_impl(u, p1o705600dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder821(u) ((-451584*(KRANC_GFOFFSET3D(u,-1,1,0) + KRANC_GFOFFSET3D(u,1,-1,0)) + 451584*(KRANC_GFOFFSET3D(u,-1,-1,0) + KRANC_GFOFFSET3D(u,1,1,0)) + 112896*(KRANC_GFOFFSET3D(u,-1,2,0) + KRANC_GFOFFSET3D(u,1,-2,0) + KRANC_GFOFFSET3D(u,-2,1,0) + KRANC_GFOFFSET3D(u,2,-1,0)) - 112896*(KRANC_GFOFFSET3D(u,-1,-2,0) + KRANC_GFOFFSET3D(u,1,2,0) + KRANC_GFOFFSET3D(u,-2,-1,0) + KRANC_GFOFFSET3D(u,2,1,0)) - 28224*(KRANC_GFOFFSET3D(u,-2,2,0) + KRANC_GFOFFSET3D(u,2,-2,0)) + 28224*(KRANC_GFOFFSET3D(u,-2,-2,0) + KRANC_GFOFFSET3D(u,2,2,0)) - 21504*(KRANC_GFOFFSET3D(u,-1,3,0) + KRANC_GFOFFSET3D(u,1,-3,0) + KRANC_GFOFFSET3D(u,-3,1,0) + KRANC_GFOFFSET3D(u,3,-1,0)) + 21504*(KRANC_GFOFFSET3D(u,-1,-3,0) + KRANC_GFOFFSET3D(u,1,3,0) + KRANC_GFOFFSET3D(u,-3,-1,0) + KRANC_GFOFFSET3D(u,3,1,0)) + 5376*(KRANC_GFOFFSET3D(u,-2,3,0) + KRANC_GFOFFSET3D(u,2,-3,0) + KRANC_GFOFFSET3D(u,-3,2,0) + KRANC_GFOFFSET3D(u,3,-2,0)) - 5376*(KRANC_GFOFFSET3D(u,-2,-3,0) + KRANC_GFOFFSET3D(u,2,3,0) + KRANC_GFOFFSET3D(u,-3,-2,0) + KRANC_GFOFFSET3D(u,3,2,0)) - 1024*(KRANC_GFOFFSET3D(u,-3,3,0) + KRANC_GFOFFSET3D(u,3,-3,0)) + 1024*(KRANC_GFOFFSET3D(u,-3,-3,0) + KRANC_GFOFFSET3D(u,3,3,0)) + 2016*(KRANC_GFOFFSET3D(u,-1,4,0) + KRANC_GFOFFSET3D(u,1,-4,0) + KRANC_GFOFFSET3D(u,-4,1,0) + KRANC_GFOFFSET3D(u,4,-1,0)) - 2016*(KRANC_GFOFFSET3D(u,-1,-4,0) + KRANC_GFOFFSET3D(u,1,4,0) + KRANC_GFOFFSET3D(u,-4,-1,0) + KRANC_GFOFFSET3D(u,4,1,0)) - 504*(KRANC_GFOFFSET3D(u,-2,4,0) + KRANC_GFOFFSET3D(u,2,-4,0) + KRANC_GFOFFSET3D(u,-4,2,0) + KRANC_GFOFFSET3D(u,4,-2,0)) + 504*(KRANC_GFOFFSET3D(u,-2,-4,0) + KRANC_GFOFFSET3D(u,2,4,0) + KRANC_GFOFFSET3D(u,-4,-2,0) + KRANC_GFOFFSET3D(u,4,2,0)) + 96*(KRANC_GFOFFSET3D(u,-3,4,0) + KRANC_GFOFFSET3D(u,3,-4,0) + KRANC_GFOFFSET3D(u,-4,3,0) + KRANC_GFOFFSET3D(u,4,-3,0)) - 96*(KRANC_GFOFFSET3D(u,-3,-4,0) + KRANC_GFOFFSET3D(u,3,4,0) + KRANC_GFOFFSET3D(u,-4,-3,0) + KRANC_GFOFFSET3D(u,4,3,0)) - 9*(KRANC_GFOFFSET3D(u,-4,4,0) + KRANC_GFOFFSET3D(u,4,-4,0)) + 9*(KRANC_GFOFFSET3D(u,-4,-4,0) + KRANC_GFOFFSET3D(u,4,4,0)))*p1o705600dxdy)
#else
#  define PDstandardNthfdOrder821(u) (PDstandardNthfdOrder821_impl(u,p1o705600dxdy,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder821_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder821_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder812_impl(u, p1o705600dxdy, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder823(u) ((-451584*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 451584*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 112896*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 112896*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 28224*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 28224*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 21504*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 21504*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 5376*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 5376*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) - 1024*(KRANC_GFOFFSET3D(u,0,-3,3) + KRANC_GFOFFSET3D(u,0,3,-3)) + 1024*(KRANC_GFOFFSET3D(u,0,-3,-3) + KRANC_GFOFFSET3D(u,0,3,3)) + 2016*(KRANC_GFOFFSET3D(u,0,-1,4) + KRANC_GFOFFSET3D(u,0,1,-4) + KRANC_GFOFFSET3D(u,0,-4,1) + KRANC_GFOFFSET3D(u,0,4,-1)) - 2016*(KRANC_GFOFFSET3D(u,0,-1,-4) + KRANC_GFOFFSET3D(u,0,1,4) + KRANC_GFOFFSET3D(u,0,-4,-1) + KRANC_GFOFFSET3D(u,0,4,1)) - 504*(KRANC_GFOFFSET3D(u,0,-2,4) + KRANC_GFOFFSET3D(u,0,2,-4) + KRANC_GFOFFSET3D(u,0,-4,2) + KRANC_GFOFFSET3D(u,0,4,-2)) + 504*(KRANC_GFOFFSET3D(u,0,-2,-4) + KRANC_GFOFFSET3D(u,0,2,4) + KRANC_GFOFFSET3D(u,0,-4,-2) + KRANC_GFOFFSET3D(u,0,4,2)) + 96*(KRANC_GFOFFSET3D(u,0,-3,4) + KRANC_GFOFFSET3D(u,0,3,-4) + KRANC_GFOFFSET3D(u,0,-4,3) + KRANC_GFOFFSET3D(u,0,4,-3)) - 96*(KRANC_GFOFFSET3D(u,0,-3,-4) + KRANC_GFOFFSET3D(u,0,3,4) + KRANC_GFOFFSET3D(u,0,-4,-3) + KRANC_GFOFFSET3D(u,0,4,3)) - 9*(KRANC_GFOFFSET3D(u,0,-4,4) + KRANC_GFOFFSET3D(u,0,4,-4)) + 9*(KRANC_GFOFFSET3D(u,0,-4,-4) + KRANC_GFOFFSET3D(u,0,4,4)))*p1o705600dydz)
#else
#  define PDstandardNthfdOrder823(u) (PDstandardNthfdOrder823_impl(u,p1o705600dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder823_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder823_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-451584*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 451584*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 112896*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 112896*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 28224*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 28224*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 21504*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 21504*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 5376*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 5376*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) - 1024*(KRANC_GFOFFSET3D(u,0,-3,3) + KRANC_GFOFFSET3D(u,0,3,-3)) + 1024*(KRANC_GFOFFSET3D(u,0,-3,-3) + KRANC_GFOFFSET3D(u,0,3,3)) + 2016*(KRANC_GFOFFSET3D(u,0,-1,4) + KRANC_GFOFFSET3D(u,0,1,-4) + KRANC_GFOFFSET3D(u,0,-4,1) + KRANC_GFOFFSET3D(u,0,4,-1)) - 2016*(KRANC_GFOFFSET3D(u,0,-1,-4) + KRANC_GFOFFSET3D(u,0,1,4) + KRANC_GFOFFSET3D(u,0,-4,-1) + KRANC_GFOFFSET3D(u,0,4,1)) - 504*(KRANC_GFOFFSET3D(u,0,-2,4) + KRANC_GFOFFSET3D(u,0,2,-4) + KRANC_GFOFFSET3D(u,0,-4,2) + KRANC_GFOFFSET3D(u,0,4,-2)) + 504*(KRANC_GFOFFSET3D(u,0,-2,-4) + KRANC_GFOFFSET3D(u,0,2,4) + KRANC_GFOFFSET3D(u,0,-4,-2) + KRANC_GFOFFSET3D(u,0,4,2)) + 96*(KRANC_GFOFFSET3D(u,0,-3,4) + KRANC_GFOFFSET3D(u,0,3,-4) + KRANC_GFOFFSET3D(u,0,-4,3) + KRANC_GFOFFSET3D(u,0,4,-3)) - 96*(KRANC_GFOFFSET3D(u,0,-3,-4) + KRANC_GFOFFSET3D(u,0,3,4) + KRANC_GFOFFSET3D(u,0,-4,-3) + KRANC_GFOFFSET3D(u,0,4,3)) - 9*(KRANC_GFOFFSET3D(u,0,-4,4) + KRANC_GFOFFSET3D(u,0,4,-4)) + 9*(KRANC_GFOFFSET3D(u,0,-4,-4) + KRANC_GFOFFSET3D(u,0,4,4)))*p1o705600dydz;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder831(u) ((-451584*(KRANC_GFOFFSET3D(u,-1,0,1) + KRANC_GFOFFSET3D(u,1,0,-1)) + 451584*(KRANC_GFOFFSET3D(u,-1,0,-1) + KRANC_GFOFFSET3D(u,1,0,1)) + 112896*(KRANC_GFOFFSET3D(u,-1,0,2) + KRANC_GFOFFSET3D(u,1,0,-2) + KRANC_GFOFFSET3D(u,-2,0,1) + KRANC_GFOFFSET3D(u,2,0,-1)) - 112896*(KRANC_GFOFFSET3D(u,-1,0,-2) + KRANC_GFOFFSET3D(u,1,0,2) + KRANC_GFOFFSET3D(u,-2,0,-1) + KRANC_GFOFFSET3D(u,2,0,1)) - 28224*(KRANC_GFOFFSET3D(u,-2,0,2) + KRANC_GFOFFSET3D(u,2,0,-2)) + 28224*(KRANC_GFOFFSET3D(u,-2,0,-2) + KRANC_GFOFFSET3D(u,2,0,2)) - 21504*(KRANC_GFOFFSET3D(u,-1,0,3) + KRANC_GFOFFSET3D(u,1,0,-3) + KRANC_GFOFFSET3D(u,-3,0,1) + KRANC_GFOFFSET3D(u,3,0,-1)) + 21504*(KRANC_GFOFFSET3D(u,-1,0,-3) + KRANC_GFOFFSET3D(u,1,0,3) + KRANC_GFOFFSET3D(u,-3,0,-1) + KRANC_GFOFFSET3D(u,3,0,1)) + 5376*(KRANC_GFOFFSET3D(u,-2,0,3) + KRANC_GFOFFSET3D(u,2,0,-3) + KRANC_GFOFFSET3D(u,-3,0,2) + KRANC_GFOFFSET3D(u,3,0,-2)) - 5376*(KRANC_GFOFFSET3D(u,-2,0,-3) + KRANC_GFOFFSET3D(u,2,0,3) + KRANC_GFOFFSET3D(u,-3,0,-2) + KRANC_GFOFFSET3D(u,3,0,2)) - 1024*(KRANC_GFOFFSET3D(u,-3,0,3) + KRANC_GFOFFSET3D(u,3,0,-3)) + 1024*(KRANC_GFOFFSET3D(u,-3,0,-3) + KRANC_GFOFFSET3D(u,3,0,3)) + 2016*(KRANC_GFOFFSET3D(u,-1,0,4) + KRANC_GFOFFSET3D(u,1,0,-4) + KRANC_GFOFFSET3D(u,-4,0,1) + KRANC_GFOFFSET3D(u,4,0,-1)) - 2016*(KRANC_GFOFFSET3D(u,-1,0,-4) + KRANC_GFOFFSET3D(u,1,0,4) + KRANC_GFOFFSET3D(u,-4,0,-1) + KRANC_GFOFFSET3D(u,4,0,1)) - 504*(KRANC_GFOFFSET3D(u,-2,0,4) + KRANC_GFOFFSET3D(u,2,0,-4) + KRANC_GFOFFSET3D(u,-4,0,2) + KRANC_GFOFFSET3D(u,4,0,-2)) + 504*(KRANC_GFOFFSET3D(u,-2,0,-4) + KRANC_GFOFFSET3D(u,2,0,4) + KRANC_GFOFFSET3D(u,-4,0,-2) + KRANC_GFOFFSET3D(u,4,0,2)) + 96*(KRANC_GFOFFSET3D(u,-3,0,4) + KRANC_GFOFFSET3D(u,3,0,-4) + KRANC_GFOFFSET3D(u,-4,0,3) + KRANC_GFOFFSET3D(u,4,0,-3)) - 96*(KRANC_GFOFFSET3D(u,-3,0,-4) + KRANC_GFOFFSET3D(u,3,0,4) + KRANC_GFOFFSET3D(u,-4,0,-3) + KRANC_GFOFFSET3D(u,4,0,3)) - 9*(KRANC_GFOFFSET3D(u,-4,0,4) + KRANC_GFOFFSET3D(u,4,0,-4)) + 9*(KRANC_GFOFFSET3D(u,-4,0,-4) + KRANC_GFOFFSET3D(u,4,0,4)))*p1o705600dxdz)
#else
#  define PDstandardNthfdOrder831(u) (PDstandardNthfdOrder831_impl(u,p1o705600dxdz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder831_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder831_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dxdz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder812_impl(u, p1o705600dxdz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDstandardNthfdOrder832(u) ((-451584*(KRANC_GFOFFSET3D(u,0,-1,1) + KRANC_GFOFFSET3D(u,0,1,-1)) + 451584*(KRANC_GFOFFSET3D(u,0,-1,-1) + KRANC_GFOFFSET3D(u,0,1,1)) + 112896*(KRANC_GFOFFSET3D(u,0,-1,2) + KRANC_GFOFFSET3D(u,0,1,-2) + KRANC_GFOFFSET3D(u,0,-2,1) + KRANC_GFOFFSET3D(u,0,2,-1)) - 112896*(KRANC_GFOFFSET3D(u,0,-1,-2) + KRANC_GFOFFSET3D(u,0,1,2) + KRANC_GFOFFSET3D(u,0,-2,-1) + KRANC_GFOFFSET3D(u,0,2,1)) - 28224*(KRANC_GFOFFSET3D(u,0,-2,2) + KRANC_GFOFFSET3D(u,0,2,-2)) + 28224*(KRANC_GFOFFSET3D(u,0,-2,-2) + KRANC_GFOFFSET3D(u,0,2,2)) - 21504*(KRANC_GFOFFSET3D(u,0,-1,3) + KRANC_GFOFFSET3D(u,0,1,-3) + KRANC_GFOFFSET3D(u,0,-3,1) + KRANC_GFOFFSET3D(u,0,3,-1)) + 21504*(KRANC_GFOFFSET3D(u,0,-1,-3) + KRANC_GFOFFSET3D(u,0,1,3) + KRANC_GFOFFSET3D(u,0,-3,-1) + KRANC_GFOFFSET3D(u,0,3,1)) + 5376*(KRANC_GFOFFSET3D(u,0,-2,3) + KRANC_GFOFFSET3D(u,0,2,-3) + KRANC_GFOFFSET3D(u,0,-3,2) + KRANC_GFOFFSET3D(u,0,3,-2)) - 5376*(KRANC_GFOFFSET3D(u,0,-2,-3) + KRANC_GFOFFSET3D(u,0,2,3) + KRANC_GFOFFSET3D(u,0,-3,-2) + KRANC_GFOFFSET3D(u,0,3,2)) - 1024*(KRANC_GFOFFSET3D(u,0,-3,3) + KRANC_GFOFFSET3D(u,0,3,-3)) + 1024*(KRANC_GFOFFSET3D(u,0,-3,-3) + KRANC_GFOFFSET3D(u,0,3,3)) + 2016*(KRANC_GFOFFSET3D(u,0,-1,4) + KRANC_GFOFFSET3D(u,0,1,-4) + KRANC_GFOFFSET3D(u,0,-4,1) + KRANC_GFOFFSET3D(u,0,4,-1)) - 2016*(KRANC_GFOFFSET3D(u,0,-1,-4) + KRANC_GFOFFSET3D(u,0,1,4) + KRANC_GFOFFSET3D(u,0,-4,-1) + KRANC_GFOFFSET3D(u,0,4,1)) - 504*(KRANC_GFOFFSET3D(u,0,-2,4) + KRANC_GFOFFSET3D(u,0,2,-4) + KRANC_GFOFFSET3D(u,0,-4,2) + KRANC_GFOFFSET3D(u,0,4,-2)) + 504*(KRANC_GFOFFSET3D(u,0,-2,-4) + KRANC_GFOFFSET3D(u,0,2,4) + KRANC_GFOFFSET3D(u,0,-4,-2) + KRANC_GFOFFSET3D(u,0,4,2)) + 96*(KRANC_GFOFFSET3D(u,0,-3,4) + KRANC_GFOFFSET3D(u,0,3,-4) + KRANC_GFOFFSET3D(u,0,-4,3) + KRANC_GFOFFSET3D(u,0,4,-3)) - 96*(KRANC_GFOFFSET3D(u,0,-3,-4) + KRANC_GFOFFSET3D(u,0,3,4) + KRANC_GFOFFSET3D(u,0,-4,-3) + KRANC_GFOFFSET3D(u,0,4,3)) - 9*(KRANC_GFOFFSET3D(u,0,-4,4) + KRANC_GFOFFSET3D(u,0,4,-4)) + 9*(KRANC_GFOFFSET3D(u,0,-4,-4) + KRANC_GFOFFSET3D(u,0,4,4)))*p1o705600dydz)
#else
#  define PDstandardNthfdOrder832(u) (PDstandardNthfdOrder832_impl(u,p1o705600dydz,cdj,cdk))
static CCTK_REAL PDstandardNthfdOrder832_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDstandardNthfdOrder832_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o705600dydz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDstandardNthfdOrder823_impl(u, p1o705600dydz, cdj, cdk);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder21(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o16dx)
#else
#  define PDdissipationNthfdOrder21(u) (PDdissipationNthfdOrder21_impl(u,pm1o16dx,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o16dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder22(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o16dy)
#else
#  define PDdissipationNthfdOrder22(u) (PDdissipationNthfdOrder22_impl(u,pm1o16dy,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o16dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder23(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) + KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2))*pm1o16dz)
#else
#  define PDdissipationNthfdOrder23(u) (PDdissipationNthfdOrder23_impl(u,pm1o16dz,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o16dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDdissipationNthfdOrder22_impl(u, pm1o16dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder41(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 6*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o64dx)
#else
#  define PDdissipationNthfdOrder41(u) (PDdissipationNthfdOrder41_impl(u,p1o64dx,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 6*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o64dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder42(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 6*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o64dy)
#else
#  define PDdissipationNthfdOrder42(u) (PDdissipationNthfdOrder42_impl(u,p1o64dy,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 6*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o64dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder43(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 6*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3))*p1o64dz)
#else
#  define PDdissipationNthfdOrder43(u) (PDdissipationNthfdOrder43_impl(u,p1o64dz,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o64dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDdissipationNthfdOrder42_impl(u, p1o64dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder61(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + 28*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) - 8*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) + KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0))*pm1o256dx)
#else
#  define PDdissipationNthfdOrder61(u) (PDdissipationNthfdOrder61_impl(u,pm1o256dx,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + 28*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) - 8*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) + KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0))*pm1o256dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder62(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + 28*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) - 8*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) + KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0))*pm1o256dy)
#else
#  define PDdissipationNthfdOrder62(u) (PDdissipationNthfdOrder62_impl(u,pm1o256dy,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + 28*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) - 8*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) + KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0))*pm1o256dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder63(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) + 28*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) - 8*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)) + KRANC_GFOFFSET3D(u,0,0,-4) + KRANC_GFOFFSET3D(u,0,0,4))*pm1o256dz)
#else
#  define PDdissipationNthfdOrder63(u) (PDdissipationNthfdOrder63_impl(u,pm1o256dz,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o256dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDdissipationNthfdOrder62_impl(u, pm1o256dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder81(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 120*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 45*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 10*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)) + KRANC_GFOFFSET3D(u,-5,0,0) + KRANC_GFOFFSET3D(u,5,0,0))*p1o1024dx)
#else
#  define PDdissipationNthfdOrder81(u) (PDdissipationNthfdOrder81_impl(u,p1o1024dx,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 120*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 45*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 10*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)) + KRANC_GFOFFSET3D(u,-5,0,0) + KRANC_GFOFFSET3D(u,5,0,0))*p1o1024dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder82(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 120*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 45*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 10*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)) + KRANC_GFOFFSET3D(u,0,-5,0) + KRANC_GFOFFSET3D(u,0,5,0))*p1o1024dy)
#else
#  define PDdissipationNthfdOrder82(u) (PDdissipationNthfdOrder82_impl(u,p1o1024dy,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 120*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 45*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 10*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)) + KRANC_GFOFFSET3D(u,0,-5,0) + KRANC_GFOFFSET3D(u,0,5,0))*p1o1024dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDdissipationNthfdOrder83(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 120*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + 45*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)) - 10*(KRANC_GFOFFSET3D(u,0,0,-4) + KRANC_GFOFFSET3D(u,0,0,4)) + KRANC_GFOFFSET3D(u,0,0,-5) + KRANC_GFOFFSET3D(u,0,0,5))*p1o1024dz)
#else
#  define PDdissipationNthfdOrder83(u) (PDdissipationNthfdOrder83_impl(u,p1o1024dz,cdj,cdk))
static CCTK_REAL PDdissipationNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDdissipationNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1024dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDdissipationNthfdOrder82_impl(u, p1o1024dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder21(u) ((3*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,2*dir1,0,0) - 4*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o2dx*dir1)
#else
#  define PDupwindNthfdOrder21(u) (PDupwindNthfdOrder21_impl(u,pm1o2dx,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (3*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,2*dir1,0,0) - 4*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o2dx*dir1;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder41(u) ((-10*KRANC_GFOFFSET3D(u,0,0,0) - 6*KRANC_GFOFFSET3D(u,2*dir1,0,0) + KRANC_GFOFFSET3D(u,3*dir1,0,0) - 3*KRANC_GFOFFSET3D(u,-dir1,0,0) + 18*KRANC_GFOFFSET3D(u,dir1,0,0))*p1o12dx*dir1)
#else
#  define PDupwindNthfdOrder41(u) (PDupwindNthfdOrder41_impl(u,p1o12dx,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-10*KRANC_GFOFFSET3D(u,0,0,0) - 6*KRANC_GFOFFSET3D(u,2*dir1,0,0) + KRANC_GFOFFSET3D(u,3*dir1,0,0) - 3*KRANC_GFOFFSET3D(u,-dir1,0,0) + 18*KRANC_GFOFFSET3D(u,dir1,0,0))*p1o12dx*dir1;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder61(u) ((35*KRANC_GFOFFSET3D(u,0,0,0) - 2*KRANC_GFOFFSET3D(u,-2*dir1,0,0) + 30*KRANC_GFOFFSET3D(u,2*dir1,0,0) - 8*KRANC_GFOFFSET3D(u,3*dir1,0,0) + KRANC_GFOFFSET3D(u,4*dir1,0,0) + 24*KRANC_GFOFFSET3D(u,-dir1,0,0) - 80*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o60dx*dir1)
#else
#  define PDupwindNthfdOrder61(u) (PDupwindNthfdOrder61_impl(u,pm1o60dx,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (35*KRANC_GFOFFSET3D(u,0,0,0) - 2*KRANC_GFOFFSET3D(u,-2*dir1,0,0) + 30*KRANC_GFOFFSET3D(u,2*dir1,0,0) - 8*KRANC_GFOFFSET3D(u,3*dir1,0,0) + KRANC_GFOFFSET3D(u,4*dir1,0,0) + 24*KRANC_GFOFFSET3D(u,-dir1,0,0) - 80*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o60dx*dir1;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder81(u) ((378*KRANC_GFOFFSET3D(u,0,0,0) - 60*KRANC_GFOFFSET3D(u,-2*dir1,0,0) + 5*KRANC_GFOFFSET3D(u,-3*dir1,0,0) - 140*KRANC_GFOFFSET3D(u,3*dir1,0,0) + 30*KRANC_GFOFFSET3D(u,4*dir1,0,0) - 3*KRANC_GFOFFSET3D(u,5*dir1,0,0) + 420*(KRANC_GFOFFSET3D(u,2*dir1,0,0) + KRANC_GFOFFSET3D(u,-dir1,0,0)) - 1050*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o840dx*dir1)
#else
#  define PDupwindNthfdOrder81(u) (PDupwindNthfdOrder81_impl(u,pm1o840dx,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (378*KRANC_GFOFFSET3D(u,0,0,0) - 60*KRANC_GFOFFSET3D(u,-2*dir1,0,0) + 5*KRANC_GFOFFSET3D(u,-3*dir1,0,0) - 140*KRANC_GFOFFSET3D(u,3*dir1,0,0) + 30*KRANC_GFOFFSET3D(u,4*dir1,0,0) - 3*KRANC_GFOFFSET3D(u,5*dir1,0,0) + 420*(KRANC_GFOFFSET3D(u,2*dir1,0,0) + KRANC_GFOFFSET3D(u,-dir1,0,0)) - 1050*KRANC_GFOFFSET3D(u,dir1,0,0))*pm1o840dx*dir1;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder21(u) ((-4*KRANC_GFOFFSET3D(u,-1,0,0) + 4*KRANC_GFOFFSET3D(u,1,0,0) + KRANC_GFOFFSET3D(u,-2,0,0) - KRANC_GFOFFSET3D(u,2,0,0))*p1o4dx)
#else
#  define PDupwindNthAntifdOrder21(u) (PDupwindNthAntifdOrder21_impl(u,p1o4dx,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-4*KRANC_GFOFFSET3D(u,-1,0,0) + 4*KRANC_GFOFFSET3D(u,1,0,0) + KRANC_GFOFFSET3D(u,-2,0,0) - KRANC_GFOFFSET3D(u,2,0,0))*p1o4dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder41(u) ((-21*KRANC_GFOFFSET3D(u,-1,0,0) + 21*KRANC_GFOFFSET3D(u,1,0,0) + 6*KRANC_GFOFFSET3D(u,-2,0,0) - 6*KRANC_GFOFFSET3D(u,2,0,0) - KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o24dx)
#else
#  define PDupwindNthAntifdOrder41(u) (PDupwindNthAntifdOrder41_impl(u,p1o24dx,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-21*KRANC_GFOFFSET3D(u,-1,0,0) + 21*KRANC_GFOFFSET3D(u,1,0,0) + 6*KRANC_GFOFFSET3D(u,-2,0,0) - 6*KRANC_GFOFFSET3D(u,2,0,0) - KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o24dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder61(u) ((-104*KRANC_GFOFFSET3D(u,-1,0,0) + 104*KRANC_GFOFFSET3D(u,1,0,0) + 32*KRANC_GFOFFSET3D(u,-2,0,0) - 32*KRANC_GFOFFSET3D(u,2,0,0) - 8*KRANC_GFOFFSET3D(u,-3,0,0) + 8*KRANC_GFOFFSET3D(u,3,0,0) + KRANC_GFOFFSET3D(u,-4,0,0) - KRANC_GFOFFSET3D(u,4,0,0))*p1o120dx)
#else
#  define PDupwindNthAntifdOrder61(u) (PDupwindNthAntifdOrder61_impl(u,p1o120dx,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-104*KRANC_GFOFFSET3D(u,-1,0,0) + 104*KRANC_GFOFFSET3D(u,1,0,0) + 32*KRANC_GFOFFSET3D(u,-2,0,0) - 32*KRANC_GFOFFSET3D(u,2,0,0) - 8*KRANC_GFOFFSET3D(u,-3,0,0) + 8*KRANC_GFOFFSET3D(u,3,0,0) + KRANC_GFOFFSET3D(u,-4,0,0) - KRANC_GFOFFSET3D(u,4,0,0))*p1o120dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder81(u) ((-1470*KRANC_GFOFFSET3D(u,-1,0,0) + 1470*KRANC_GFOFFSET3D(u,1,0,0) + 480*KRANC_GFOFFSET3D(u,-2,0,0) - 480*KRANC_GFOFFSET3D(u,2,0,0) - 145*KRANC_GFOFFSET3D(u,-3,0,0) + 145*KRANC_GFOFFSET3D(u,3,0,0) + 30*KRANC_GFOFFSET3D(u,-4,0,0) - 30*KRANC_GFOFFSET3D(u,4,0,0) - 3*KRANC_GFOFFSET3D(u,-5,0,0) + 3*KRANC_GFOFFSET3D(u,5,0,0))*p1o1680dx)
#else
#  define PDupwindNthAntifdOrder81(u) (PDupwindNthAntifdOrder81_impl(u,p1o1680dx,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-1470*KRANC_GFOFFSET3D(u,-1,0,0) + 1470*KRANC_GFOFFSET3D(u,1,0,0) + 480*KRANC_GFOFFSET3D(u,-2,0,0) - 480*KRANC_GFOFFSET3D(u,2,0,0) - 145*KRANC_GFOFFSET3D(u,-3,0,0) + 145*KRANC_GFOFFSET3D(u,3,0,0) + 30*KRANC_GFOFFSET3D(u,-4,0,0) - 30*KRANC_GFOFFSET3D(u,4,0,0) - 3*KRANC_GFOFFSET3D(u,-5,0,0) + 3*KRANC_GFOFFSET3D(u,5,0,0))*p1o1680dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder21(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o4dx)
#else
#  define PDupwindNthSymmfdOrder21(u) (PDupwindNthSymmfdOrder21_impl(u,pm1o4dx,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder21_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0))*pm1o4dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder41(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 6*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o24dx)
#else
#  define PDupwindNthSymmfdOrder41(u) (PDupwindNthSymmfdOrder41_impl(u,p1o24dx,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder41_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 6*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0))*p1o24dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder61(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + 28*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) - 8*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) + KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0))*pm1o120dx)
#else
#  define PDupwindNthSymmfdOrder61(u) (PDupwindNthSymmfdOrder61_impl(u,pm1o120dx,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder61_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) + 28*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) - 8*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) + KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0))*pm1o120dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder81(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 120*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 45*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 10*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)) + KRANC_GFOFFSET3D(u,-5,0,0) + KRANC_GFOFFSET3D(u,5,0,0))*p1o560dx)
#else
#  define PDupwindNthSymmfdOrder81(u) (PDupwindNthSymmfdOrder81_impl(u,p1o560dx,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dx, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder81_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dx, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,-1,0,0) + KRANC_GFOFFSET3D(u,1,0,0)) - 120*(KRANC_GFOFFSET3D(u,-2,0,0) + KRANC_GFOFFSET3D(u,2,0,0)) + 45*(KRANC_GFOFFSET3D(u,-3,0,0) + KRANC_GFOFFSET3D(u,3,0,0)) - 10*(KRANC_GFOFFSET3D(u,-4,0,0) + KRANC_GFOFFSET3D(u,4,0,0)) + KRANC_GFOFFSET3D(u,-5,0,0) + KRANC_GFOFFSET3D(u,5,0,0))*p1o560dx;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDonesided1(u) ((-KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,dir1,0,0))*p1odx*dir1)
#else
#  define PDonesided1(u) (PDonesided1_impl(u,p1odx,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDonesided1_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDonesided1_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odx, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,dir1,0,0))*p1odx*dir1;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder22(u) ((3*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,2*dir2,0) - 4*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o2dy*dir2)
#else
#  define PDupwindNthfdOrder22(u) (PDupwindNthfdOrder22_impl(u,pm1o2dy,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (3*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,2*dir2,0) - 4*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o2dy*dir2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder42(u) ((-10*KRANC_GFOFFSET3D(u,0,0,0) - 6*KRANC_GFOFFSET3D(u,0,2*dir2,0) + KRANC_GFOFFSET3D(u,0,3*dir2,0) - 3*KRANC_GFOFFSET3D(u,0,-dir2,0) + 18*KRANC_GFOFFSET3D(u,0,dir2,0))*p1o12dy*dir2)
#else
#  define PDupwindNthfdOrder42(u) (PDupwindNthfdOrder42_impl(u,p1o12dy,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-10*KRANC_GFOFFSET3D(u,0,0,0) - 6*KRANC_GFOFFSET3D(u,0,2*dir2,0) + KRANC_GFOFFSET3D(u,0,3*dir2,0) - 3*KRANC_GFOFFSET3D(u,0,-dir2,0) + 18*KRANC_GFOFFSET3D(u,0,dir2,0))*p1o12dy*dir2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder62(u) ((35*KRANC_GFOFFSET3D(u,0,0,0) - 2*KRANC_GFOFFSET3D(u,0,-2*dir2,0) + 30*KRANC_GFOFFSET3D(u,0,2*dir2,0) - 8*KRANC_GFOFFSET3D(u,0,3*dir2,0) + KRANC_GFOFFSET3D(u,0,4*dir2,0) + 24*KRANC_GFOFFSET3D(u,0,-dir2,0) - 80*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o60dy*dir2)
#else
#  define PDupwindNthfdOrder62(u) (PDupwindNthfdOrder62_impl(u,pm1o60dy,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (35*KRANC_GFOFFSET3D(u,0,0,0) - 2*KRANC_GFOFFSET3D(u,0,-2*dir2,0) + 30*KRANC_GFOFFSET3D(u,0,2*dir2,0) - 8*KRANC_GFOFFSET3D(u,0,3*dir2,0) + KRANC_GFOFFSET3D(u,0,4*dir2,0) + 24*KRANC_GFOFFSET3D(u,0,-dir2,0) - 80*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o60dy*dir2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder82(u) ((378*KRANC_GFOFFSET3D(u,0,0,0) - 60*KRANC_GFOFFSET3D(u,0,-2*dir2,0) + 5*KRANC_GFOFFSET3D(u,0,-3*dir2,0) - 140*KRANC_GFOFFSET3D(u,0,3*dir2,0) + 30*KRANC_GFOFFSET3D(u,0,4*dir2,0) - 3*KRANC_GFOFFSET3D(u,0,5*dir2,0) + 420*(KRANC_GFOFFSET3D(u,0,2*dir2,0) + KRANC_GFOFFSET3D(u,0,-dir2,0)) - 1050*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o840dy*dir2)
#else
#  define PDupwindNthfdOrder82(u) (PDupwindNthfdOrder82_impl(u,pm1o840dy,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dy, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (378*KRANC_GFOFFSET3D(u,0,0,0) - 60*KRANC_GFOFFSET3D(u,0,-2*dir2,0) + 5*KRANC_GFOFFSET3D(u,0,-3*dir2,0) - 140*KRANC_GFOFFSET3D(u,0,3*dir2,0) + 30*KRANC_GFOFFSET3D(u,0,4*dir2,0) - 3*KRANC_GFOFFSET3D(u,0,5*dir2,0) + 420*(KRANC_GFOFFSET3D(u,0,2*dir2,0) + KRANC_GFOFFSET3D(u,0,-dir2,0)) - 1050*KRANC_GFOFFSET3D(u,0,dir2,0))*pm1o840dy*dir2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder22(u) ((-4*KRANC_GFOFFSET3D(u,0,-1,0) + 4*KRANC_GFOFFSET3D(u,0,1,0) + KRANC_GFOFFSET3D(u,0,-2,0) - KRANC_GFOFFSET3D(u,0,2,0))*p1o4dy)
#else
#  define PDupwindNthAntifdOrder22(u) (PDupwindNthAntifdOrder22_impl(u,p1o4dy,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-4*KRANC_GFOFFSET3D(u,0,-1,0) + 4*KRANC_GFOFFSET3D(u,0,1,0) + KRANC_GFOFFSET3D(u,0,-2,0) - KRANC_GFOFFSET3D(u,0,2,0))*p1o4dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder42(u) ((-21*KRANC_GFOFFSET3D(u,0,-1,0) + 21*KRANC_GFOFFSET3D(u,0,1,0) + 6*KRANC_GFOFFSET3D(u,0,-2,0) - 6*KRANC_GFOFFSET3D(u,0,2,0) - KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o24dy)
#else
#  define PDupwindNthAntifdOrder42(u) (PDupwindNthAntifdOrder42_impl(u,p1o24dy,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-21*KRANC_GFOFFSET3D(u,0,-1,0) + 21*KRANC_GFOFFSET3D(u,0,1,0) + 6*KRANC_GFOFFSET3D(u,0,-2,0) - 6*KRANC_GFOFFSET3D(u,0,2,0) - KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o24dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder62(u) ((-104*KRANC_GFOFFSET3D(u,0,-1,0) + 104*KRANC_GFOFFSET3D(u,0,1,0) + 32*KRANC_GFOFFSET3D(u,0,-2,0) - 32*KRANC_GFOFFSET3D(u,0,2,0) - 8*KRANC_GFOFFSET3D(u,0,-3,0) + 8*KRANC_GFOFFSET3D(u,0,3,0) + KRANC_GFOFFSET3D(u,0,-4,0) - KRANC_GFOFFSET3D(u,0,4,0))*p1o120dy)
#else
#  define PDupwindNthAntifdOrder62(u) (PDupwindNthAntifdOrder62_impl(u,p1o120dy,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-104*KRANC_GFOFFSET3D(u,0,-1,0) + 104*KRANC_GFOFFSET3D(u,0,1,0) + 32*KRANC_GFOFFSET3D(u,0,-2,0) - 32*KRANC_GFOFFSET3D(u,0,2,0) - 8*KRANC_GFOFFSET3D(u,0,-3,0) + 8*KRANC_GFOFFSET3D(u,0,3,0) + KRANC_GFOFFSET3D(u,0,-4,0) - KRANC_GFOFFSET3D(u,0,4,0))*p1o120dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder82(u) ((-1470*KRANC_GFOFFSET3D(u,0,-1,0) + 1470*KRANC_GFOFFSET3D(u,0,1,0) + 480*KRANC_GFOFFSET3D(u,0,-2,0) - 480*KRANC_GFOFFSET3D(u,0,2,0) - 145*KRANC_GFOFFSET3D(u,0,-3,0) + 145*KRANC_GFOFFSET3D(u,0,3,0) + 30*KRANC_GFOFFSET3D(u,0,-4,0) - 30*KRANC_GFOFFSET3D(u,0,4,0) - 3*KRANC_GFOFFSET3D(u,0,-5,0) + 3*KRANC_GFOFFSET3D(u,0,5,0))*p1o1680dy)
#else
#  define PDupwindNthAntifdOrder82(u) (PDupwindNthAntifdOrder82_impl(u,p1o1680dy,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-1470*KRANC_GFOFFSET3D(u,0,-1,0) + 1470*KRANC_GFOFFSET3D(u,0,1,0) + 480*KRANC_GFOFFSET3D(u,0,-2,0) - 480*KRANC_GFOFFSET3D(u,0,2,0) - 145*KRANC_GFOFFSET3D(u,0,-3,0) + 145*KRANC_GFOFFSET3D(u,0,3,0) + 30*KRANC_GFOFFSET3D(u,0,-4,0) - 30*KRANC_GFOFFSET3D(u,0,4,0) - 3*KRANC_GFOFFSET3D(u,0,-5,0) + 3*KRANC_GFOFFSET3D(u,0,5,0))*p1o1680dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder22(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o4dy)
#else
#  define PDupwindNthSymmfdOrder22(u) (PDupwindNthSymmfdOrder22_impl(u,pm1o4dy,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder22_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0))*pm1o4dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder42(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 6*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o24dy)
#else
#  define PDupwindNthSymmfdOrder42(u) (PDupwindNthSymmfdOrder42_impl(u,p1o24dy,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder42_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 6*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0))*p1o24dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder62(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + 28*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) - 8*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) + KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0))*pm1o120dy)
#else
#  define PDupwindNthSymmfdOrder62(u) (PDupwindNthSymmfdOrder62_impl(u,pm1o120dy,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder62_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) + 28*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) - 8*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) + KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0))*pm1o120dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder82(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 120*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 45*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 10*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)) + KRANC_GFOFFSET3D(u,0,-5,0) + KRANC_GFOFFSET3D(u,0,5,0))*p1o560dy)
#else
#  define PDupwindNthSymmfdOrder82(u) (PDupwindNthSymmfdOrder82_impl(u,p1o560dy,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dy, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder82_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dy, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,-1,0) + KRANC_GFOFFSET3D(u,0,1,0)) - 120*(KRANC_GFOFFSET3D(u,0,-2,0) + KRANC_GFOFFSET3D(u,0,2,0)) + 45*(KRANC_GFOFFSET3D(u,0,-3,0) + KRANC_GFOFFSET3D(u,0,3,0)) - 10*(KRANC_GFOFFSET3D(u,0,-4,0) + KRANC_GFOFFSET3D(u,0,4,0)) + KRANC_GFOFFSET3D(u,0,-5,0) + KRANC_GFOFFSET3D(u,0,5,0))*p1o560dy;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDonesided2(u) ((-KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,dir2,0))*p1ody*dir2)
#else
#  define PDonesided2(u) (PDonesided2_impl(u,p1ody,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDonesided2_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1ody, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDonesided2_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1ody, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return (-KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,dir2,0))*p1ody*dir2;
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder23(u) ((3*KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,0,2*dir3) - 4*KRANC_GFOFFSET3D(u,0,0,dir3))*pm1o2dz*dir3)
#else
#  define PDupwindNthfdOrder23(u) (PDupwindNthfdOrder23_impl(u,pm1o2dz,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o2dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthfdOrder22_impl(u, pm1o2dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder43(u) ((-10*KRANC_GFOFFSET3D(u,0,0,0) - 6*KRANC_GFOFFSET3D(u,0,0,2*dir3) + KRANC_GFOFFSET3D(u,0,0,3*dir3) - 3*KRANC_GFOFFSET3D(u,0,0,-dir3) + 18*KRANC_GFOFFSET3D(u,0,0,dir3))*p1o12dz*dir3)
#else
#  define PDupwindNthfdOrder43(u) (PDupwindNthfdOrder43_impl(u,p1o12dz,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o12dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthfdOrder42_impl(u, p1o12dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder63(u) ((35*KRANC_GFOFFSET3D(u,0,0,0) - 2*KRANC_GFOFFSET3D(u,0,0,-2*dir3) + 30*KRANC_GFOFFSET3D(u,0,0,2*dir3) - 8*KRANC_GFOFFSET3D(u,0,0,3*dir3) + KRANC_GFOFFSET3D(u,0,0,4*dir3) + 24*KRANC_GFOFFSET3D(u,0,0,-dir3) - 80*KRANC_GFOFFSET3D(u,0,0,dir3))*pm1o60dz*dir3)
#else
#  define PDupwindNthfdOrder63(u) (PDupwindNthfdOrder63_impl(u,pm1o60dz,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o60dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthfdOrder62_impl(u, pm1o60dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthfdOrder83(u) ((378*KRANC_GFOFFSET3D(u,0,0,0) - 60*KRANC_GFOFFSET3D(u,0,0,-2*dir3) + 5*KRANC_GFOFFSET3D(u,0,0,-3*dir3) - 140*KRANC_GFOFFSET3D(u,0,0,3*dir3) + 30*KRANC_GFOFFSET3D(u,0,0,4*dir3) - 3*KRANC_GFOFFSET3D(u,0,0,5*dir3) + 420*(KRANC_GFOFFSET3D(u,0,0,2*dir3) + KRANC_GFOFFSET3D(u,0,0,-dir3)) - 1050*KRANC_GFOFFSET3D(u,0,0,dir3))*pm1o840dz*dir3)
#else
#  define PDupwindNthfdOrder83(u) (PDupwindNthfdOrder83_impl(u,pm1o840dz,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDupwindNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o840dz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthfdOrder82_impl(u, pm1o840dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder23(u) ((-4*KRANC_GFOFFSET3D(u,0,0,-1) + 4*KRANC_GFOFFSET3D(u,0,0,1) + KRANC_GFOFFSET3D(u,0,0,-2) - KRANC_GFOFFSET3D(u,0,0,2))*p1o4dz)
#else
#  define PDupwindNthAntifdOrder23(u) (PDupwindNthAntifdOrder23_impl(u,p1o4dz,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o4dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthAntifdOrder22_impl(u, p1o4dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder43(u) ((-21*KRANC_GFOFFSET3D(u,0,0,-1) + 21*KRANC_GFOFFSET3D(u,0,0,1) + 6*KRANC_GFOFFSET3D(u,0,0,-2) - 6*KRANC_GFOFFSET3D(u,0,0,2) - KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3))*p1o24dz)
#else
#  define PDupwindNthAntifdOrder43(u) (PDupwindNthAntifdOrder43_impl(u,p1o24dz,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthAntifdOrder42_impl(u, p1o24dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder63(u) ((-104*KRANC_GFOFFSET3D(u,0,0,-1) + 104*KRANC_GFOFFSET3D(u,0,0,1) + 32*KRANC_GFOFFSET3D(u,0,0,-2) - 32*KRANC_GFOFFSET3D(u,0,0,2) - 8*KRANC_GFOFFSET3D(u,0,0,-3) + 8*KRANC_GFOFFSET3D(u,0,0,3) + KRANC_GFOFFSET3D(u,0,0,-4) - KRANC_GFOFFSET3D(u,0,0,4))*p1o120dz)
#else
#  define PDupwindNthAntifdOrder63(u) (PDupwindNthAntifdOrder63_impl(u,p1o120dz,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o120dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthAntifdOrder62_impl(u, p1o120dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthAntifdOrder83(u) ((-1470*KRANC_GFOFFSET3D(u,0,0,-1) + 1470*KRANC_GFOFFSET3D(u,0,0,1) + 480*KRANC_GFOFFSET3D(u,0,0,-2) - 480*KRANC_GFOFFSET3D(u,0,0,2) - 145*KRANC_GFOFFSET3D(u,0,0,-3) + 145*KRANC_GFOFFSET3D(u,0,0,3) + 30*KRANC_GFOFFSET3D(u,0,0,-4) - 30*KRANC_GFOFFSET3D(u,0,0,4) - 3*KRANC_GFOFFSET3D(u,0,0,-5) + 3*KRANC_GFOFFSET3D(u,0,0,5))*p1o1680dz)
#else
#  define PDupwindNthAntifdOrder83(u) (PDupwindNthAntifdOrder83_impl(u,p1o1680dz,cdj,cdk))
static CCTK_REAL PDupwindNthAntifdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthAntifdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o1680dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthAntifdOrder82_impl(u, p1o1680dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder23(u) ((6*KRANC_GFOFFSET3D(u,0,0,0) - 4*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) + KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2))*pm1o4dz)
#else
#  define PDupwindNthSymmfdOrder23(u) (PDupwindNthSymmfdOrder23_impl(u,pm1o4dz,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder23_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o4dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthSymmfdOrder22_impl(u, pm1o4dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder43(u) ((-20*KRANC_GFOFFSET3D(u,0,0,0) + 15*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 6*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3))*p1o24dz)
#else
#  define PDupwindNthSymmfdOrder43(u) (PDupwindNthSymmfdOrder43_impl(u,p1o24dz,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder43_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o24dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthSymmfdOrder42_impl(u, p1o24dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder63(u) ((70*KRANC_GFOFFSET3D(u,0,0,0) - 56*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) + 28*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) - 8*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)) + KRANC_GFOFFSET3D(u,0,0,-4) + KRANC_GFOFFSET3D(u,0,0,4))*pm1o120dz)
#else
#  define PDupwindNthSymmfdOrder63(u) (PDupwindNthSymmfdOrder63_impl(u,pm1o120dz,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder63_impl(const CCTK_REAL* restrict const u, const CCTK_REAL pm1o120dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthSymmfdOrder62_impl(u, pm1o120dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDupwindNthSymmfdOrder83(u) ((-252*KRANC_GFOFFSET3D(u,0,0,0) + 210*(KRANC_GFOFFSET3D(u,0,0,-1) + KRANC_GFOFFSET3D(u,0,0,1)) - 120*(KRANC_GFOFFSET3D(u,0,0,-2) + KRANC_GFOFFSET3D(u,0,0,2)) + 45*(KRANC_GFOFFSET3D(u,0,0,-3) + KRANC_GFOFFSET3D(u,0,0,3)) - 10*(KRANC_GFOFFSET3D(u,0,0,-4) + KRANC_GFOFFSET3D(u,0,0,4)) + KRANC_GFOFFSET3D(u,0,0,-5) + KRANC_GFOFFSET3D(u,0,0,5))*p1o560dz)
#else
#  define PDupwindNthSymmfdOrder83(u) (PDupwindNthSymmfdOrder83_impl(u,p1o560dz,cdj,cdk))
static CCTK_REAL PDupwindNthSymmfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dz, const ptrdiff_t cdj, const ptrdiff_t cdk) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDupwindNthSymmfdOrder83_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1o560dz, const ptrdiff_t cdj, const ptrdiff_t cdk)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDupwindNthSymmfdOrder82_impl(u, p1o560dz, cdk, cdj);
}
#endif

#ifndef KRANC_DIFF_FUNCTIONS
#  define PDonesided3(u) ((-KRANC_GFOFFSET3D(u,0,0,0) + KRANC_GFOFFSET3D(u,0,0,dir3))*p1odz*dir3)
#else
#  define PDonesided3(u) (PDonesided3_impl(u,p1odz,cdj,cdk,dir1,dir2,dir3))
static CCTK_REAL PDonesided3_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3) CCTK_ATTRIBUTE_NOINLINE CCTK_ATTRIBUTE_UNUSED;
static CCTK_REAL PDonesided3_impl(const CCTK_REAL* restrict const u, const CCTK_REAL p1odz, const ptrdiff_t cdj, const ptrdiff_t cdk, const ptrdiff_t dir1, const ptrdiff_t dir2, const ptrdiff_t dir3)
{
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL);
  return PDonesided2_impl(u, p1odz, cdk, cdj);
}
#endif

