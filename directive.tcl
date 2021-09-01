set_directive_interface -mode ap_memory "subseek_dtw" x
set_directive_interface -mode ap_memory "subseek_dtw" y
set_directive_array_partition -type complete -dim 1 "subseek_dtw" cost
set_directive_unroll "subseek_dtw/cost_init_loop"
set_directive_pipeline "subseek_dtw/seq_y_loop"
