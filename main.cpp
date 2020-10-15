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
	}else if (asm_opt.mode_readset_kmer_count){
		hamt_printout_ha_count(&asm_opt, &R_INF);
		return 0;
	}else if (asm_opt.is_dump_read_selection){
		hamt_dump_read_selection_mask(&asm_opt, &R_INF);
		return 0;
	}


	// main
	ret = ha_assemble();
    destory_opt(&asm_opt);
	fprintf(stderr, "[M::%s] Version: %s\n", __func__, HA_VERSION);
	fprintf(stderr, "[M::%s] CMD:", __func__);
	for (i = 0; i < argc; ++i)
		fprintf(stderr, " %s", argv[i]);
	fprintf(stderr, "\n[M::%s] Real time: %.3f sec; CPU: %.3f sec; Peak RSS: %.3f GB\n", __func__, yak_realtime(), yak_cputime(), yak_peakrss_in_gb());
    return ret;
}
