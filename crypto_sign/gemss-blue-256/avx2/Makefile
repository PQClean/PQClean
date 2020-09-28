# This Makefile can be used with GNU Make or BSD Make

LIB=libgemss-blue-256_avx2.a
HEADERS=add_gf2n.h add_gf2nx.h add_gf2x.h api.h arch.h bit.h changeVariablesMQS_gf2.h choice_crypto.h chooseRootHFE_gf2nx.h config_gf2n.h config_HFE.h conv_gf2nx.h convMQ_gf2.h convMQS_gf2.h determinantn_gf2.h determinantnv_gf2.h div_gf2nx.h dotProduct_gf2.h dotProduct_gf2n.h evalMQnocst_gf2.h evalMQShybrid_gf2.h evalMQSnocst8_gf2.h evalMQSnocst8_quo_gf2.h evalMQSv_gf2.h findRoots_gf2nx.h findRootsSplit_gf2nx.h frobeniusMap_gf2nx.h gcd_gf2nx.h genCanonicalBasis_gf2n.h genSecretMQS_gf2.h gf2.h gf2nx.h gf2x.h hash.h init.h initListDifferences_gf2nx.h initMatrixId_gf2.h inv_gf2n.h invMatrixn_gf2.h invMatrixnv_gf2.h ITMIA_addchains.h loadMask.h macro.h matrix_gf2.h matrix_gf2n.h mixEquationsMQS_gf2.h MQ_gf2.h MQS_gf2.h mul_gf2n.h mul_gf2x.h parameters_HFE.h predicate.h rand_gf2n.h rand_gf2nx.h randMatrix_gf2.h rem5_gf2n.h rem_gf2n.h signHFE.h sign_keypairHFE.h sign_openHFE.h simd.h simd_intel.h sizes_crypto.h sizes_HFE.h sort_gf2n.h sqr_gf2n.h sqr_gf2nx.h sqr_gf2x.h tools_gf2m.h tools_gf2n.h tools_gf2nv.h tools_gf2v.h vecMatProduct_gf2.h xor.h 
OBJECTS=add_gf2n.o changeVariablesMQS_gf2.o chooseRootHFE_gf2nx.o conv_gf2nx.o convMQ_gf2.o convMQS_gf2.o determinantn_gf2.o determinantnv_gf2.o div_gf2nx.o dotProduct_gf2n.o evalMQnocst_gf2.o evalMQShybrid_gf2.o evalMQSnocst8_gf2.o evalMQSnocst8_quo_gf2.o evalMQSv_gf2.o findRoots_gf2nx.o findRootsSplit_gf2nx.o frobeniusMap_gf2nx.o gcd_gf2nx.o genCanonicalBasis_gf2n.o genSecretMQS_gf2.o initListDifferences_gf2nx.o initMatrixId_gf2.o inv_gf2n.o invMatrixn_gf2.o invMatrixnv_gf2.o mixEquationsMQS_gf2.o mul_gf2n.o mul_gf2x.o predicate.o rand_gf2nx.o randMatrix_gf2.o rem_gf2n.o sign.o signHFE.o sign_keypairHFE.o sign_openHFE.o sort_gf2n.o sqr_gf2n.o sqr_gf2nx.o sqr_gf2x.o vecMatProduct_gf2.o 

CFLAGS=-O3 -mavx2 -mbmi -mpclmul -Wall -Wextra -Wpedantic -Wvla -Werror -Wredundant-decls -Wmissing-prototypes -std=c99 -I../../../common $(EXTRAFLAGS)

all: $(LIB)

%.o: %.s $(HEADERS)
	$(AS) -o $@ $<

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJECTS)
	$(AR) -r $@ $(OBJECTS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB)
