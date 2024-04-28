#!/bin/bash
# Simple build script, potentially useful if building a GPL-redistributable
# version on a system *without* SPEC CPU2017.  See:
#
#     https://www.spec.org/cpu2017/Docs/licenses.html#GPLtarballs
#
# Note: if the benchmark uses "specpp", that is the freely-redistributable
#       preprocessor "filepp".   It is described at 
#         https://www.spec.org/cpu2017/Docs/utility.html#specpp  
#       and will need to be in your path.  You can find filepp sources as
#       modified by SPEC in redistributable_sources/modified/tools.src/harness-spec.tar.xz 
#---------------------------------------------------------------------------

specmake: Nothing to be done for 'all'.
