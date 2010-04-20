#!/bin/sh
#
# Arguments: <simics> <mips-binary>
#

SIMICS=$1
# export SIMICS
MIPS_BINARY=$2

HERE=$PWD
cd $SIMICS
./simics -fast -e "\$mips_binary=$HERE/$MIPS_BINARY" -x $HERE/scripts/run.simics


