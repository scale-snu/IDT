#
# File fdepends.awk
#
# Take an .F90 file and generate Makefile dependencies for
# each module "use", "#include", and "include"
#
# Example:
# POP.o: io.o
#
# Predefined variables     Typical values
#     NAME                    POP
#     SUF                     .F90
#

BEGIN { IGNORECASE=1
        PRLINE = NAME".o: "
        print NAME".o: " NAME SUF
      }


#
# awk reads each line of the filename argument $2 until it finds 
# a "use" or "#include"
# 


/^[ \t]*use[ \t]+/ {

        # Ignore any "use" line that contains comment "_EXTERNAL"
	# As we use this where everything is in one directory, netcdf included, dont drop these lines
        # if ( $0 ~ /_EXTERNAL/ ) next

        # Assume the second field is the F90 module name,
        # remove any comma at the end of the second field (due to 
        # ONLY or rename), and print it in a dependency line.
	# We can also have use module,only (no spaces)
	# and there are use module,only: as well
        sub(/,$/,"",$2)
	sub(/,only$/,"",$2)
	sub(/,only:$/,"",$2)
        print PRLINE $2".o"
       }


# This will match include lines (either cpp or fortran style)
#     #include "myinclude.inc"
#     #include <myinclude.inc>
#     INCLUDE 'MYINCLUDE.INC'

/^[ \t]*#?include[ \t]/ {

         # Ignore any "#include" line that contains comment "_EXTERNAL"
	 # Keep this, the external include files are mpif.h which we have, and pnetcdf.inc
	 # which we dont have
         if ( $0 ~ /_EXTERNAL/ ) next

         # Remove starting or ending quote or angle bracket
         sub(/^["<']/,"",$2) 
         sub(/[">']$/,"",$2) 
         print PRLINE $2
 
      }
