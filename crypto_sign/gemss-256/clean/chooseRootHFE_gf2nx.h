#ifndef _CHOOSEROOTHFE_H
#define _CHOOSEROOTHFE_H
#include "choice_crypto.h"
#include "config_HFE.h"
#include "gf2nx.h"
#include "tools_gf2n.h"



/* Choose the method to choose the root */
#define FIRST_ROOT 0
#define DETERMINIST_ROOT 1
#define QUARTZ_ROOT 0



int PQCLEAN_GEMSS256_CLEAN_chooseRootHFE_gf2nx(gf2n root,
        complete_sparse_monic_gf2nx F,
        cst_gf2n U);


#endif
