open_project sDTW_HLS
set_top subseek_dtw
add_files src/subseek_dtw.cpp
add_files -tb src/top_subseek_dtw.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution0"
set_part {xc7z020-clg484-1}
create_clock -period 8.0 -name default
# csim_design -clean
csynth_design
export_design
close_project

# open_project ref_loader
# set_top reference_loader
# add_files src/reference_bram.cpp
# open_solution "solution0"
# set_part {xc7z020-clg484-1}
# create_clock -period 8.0 -name default
# csynth_design
# export_design
# close_project
exit