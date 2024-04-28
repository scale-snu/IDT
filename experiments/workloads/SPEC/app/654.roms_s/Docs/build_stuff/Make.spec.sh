#!/bin/ksh
#cp -p netcdf/netcdf.f90  netcdf.F90
#cp -p netcdf/typeSizes.f90 typeSizes.F90
gmake depends -f Makefile.depends
ddd=`cat Srcfiles_F90 | sed "s'\.F90'.d'"`
cat $ddd | \
(  read name modu
 while [ "x$name" != "x" ]; do
# The first part is because src xx.F90 has module name xx_mod.mod
  base=`echo $modu | sed "s'_mod\.o'.o'" | sed "s'\.o''"`
  rbas=`grep -i "^$base.F90" Srcfiles_F90`
  if [ "x$rbas" != "x" ]; then
        modu=`echo $rbas | sed "s'\.F90'.o'"`
  fi
  printf "%s %s\n" $name $modu
  read name modu;
  done
) | \
sed "s't3dbc_mod\.o't3dbc_im.o'g" | \
sed "s'tkebc_mod\.o'tkebc_im.o'g" | \
sed "s'u2dbc_mod\.o'u2dbc_im.o'g" | \
sed "s'u3dbc_mod\.o'u3dbc_im.o'g" | \
sed "s'v2dbc_mod\.o'v2dbc_im.o'g" | \
sed "s'v3dbc_mod\.o'v3dbc_im.o'g" | \
awk -f mergedepspec.awk -F':' - | \
    sed "s/' /'/"  > newDepends
#rm netcdf.F90 typeSizes.F90
#    sed "s' netcdf\.F90' netcdf/netcdf.f90'g" | \
#    sed "s'typeSizes.F90'netcdf/typeSizes.f90'g" | \
#rm *.d
