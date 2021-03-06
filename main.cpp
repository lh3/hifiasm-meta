#include <stdio.h>
#include <stdlib.h>
#include "CommandLines.h"
#include "Process_Read.h"
#include "Assembly.h"
#include "Levenshtein_distance.h"
#include "htab.h"
#include "meta_util_debug.h"

int main(int argc, char *argv[])
{
	int i, ret;
	yak_reset_realtime();
    init_opt(&asm_opt);
    if (!CommandLine_process(argc, argv, &asm_opt)) return 1;

	// debug modules
	if (asm_opt.mode_read_kmer_profile){
		hamt_read_kmer_profile(&asm_opt, &R_INF);
		return 0;
	}
	else if (asm_opt.is_dump_read_mask){
		hamt_dump_read_selection_mask(&asm_opt, &R_INF);
		return 0;
	}
	else if (asm_opt.is_dump_read_names){
		hamt_dump_selected_read_names(&asm_opt, &R_INF);
		return 0;
	}
	else if (asm_opt.is_dump_ovec_error_count){
		hamt_dump_ovec_read_error_count_and_kmerinfo(&asm_opt, &R_INF);
		return 0;
	}


	// main
	if (asm_opt.is_use_exp_graph_cleaning){
		ret = hamt_assemble();
		fprintf(stderr, "[M::%s] Hifiasm code base version: %s\n", __func__, HA_VERSION);
		fprintf(stderr, "[M::%s] Hifiasm_meta version: %s\n", __func__, HAMT_VERSION);
	}else{
		ret = ha_assemble();
		fprintf(stderr, "[M::%s] Hifiasm %s\n", __func__, HA_VERSION);
	}
    destory_opt(&asm_opt);
	
	fprintf(stderr, "[M::%s] CMD:", __func__);
	for (i = 0; i < argc; ++i)
		fprintf(stderr, " %s", argv[i]);
	fprintf(stderr, "\n[M::%s] Real time: %.3f sec; CPU: %.3f sec; Peak RSS: %.3f GB\n", __func__, yak_realtime(), yak_cputime(), yak_peakrss_in_gb());
    return ret;
}
