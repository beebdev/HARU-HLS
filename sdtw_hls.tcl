set part {xc7z020-clg484-1}

##################################################################################################
# sDTW Project
##################################################################################################
open_project sDTW_HLS

set_top subseek_dtw

add_files src/subseek_dtw.c
add_files -tb src/top_subseek_dtw.c -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"


###############################################################################
# Baseline solution
###############################################################################
open_solution "baseline"
set_part $part
create_clock -period 10
csim_design -clean

###############################################################################
# Baseline solution
###############################################################################
open_solution "accelerated"
set_part $part
create_clock -period 10
source "./directive.tcl"
csynth_design
export_design -format ip_catalog

exit