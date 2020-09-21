#ifndef _EVALMQSV_GF2_H
#define _EVALMQSV_GF2_H
#include "MQS_gf2.h"
#include "arch.h"
#include "matrix_gf2.h"
#include "parameters_HFE.h"



/* n equations, v variables */

void PQCLEAN_GEMSSBLUE256_CLEAN_evalMQSv_classical_gf2(vecm_gf2 c, cst_vecnv_gf2 m,
        cst_mqsnv_gf2m pk);


/* Choose the best function */
#define evalMQSv_gf2 PQCLEAN_GEMSSBLUE256_CLEAN_evalMQSv_classical_gf2


#endif
