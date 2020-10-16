#include <immintrin.h>

static void transpose_n1(__m256i *M)
{
	//int i;
	register __m256i r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
	register __m256i temp, temp0, temp1, temp2;

	//for(i=0; i<8; i=i+1)
	//{
		r0 = _mm256_unpacklo_epi16(M[0], M[1]); 
		r1 = _mm256_unpacklo_epi16(M[2], M[3]); 
		r2 = _mm256_unpacklo_epi16(M[4], M[5]); 
		r3 = _mm256_unpacklo_epi16(M[6], M[7]);
		r4 = _mm256_unpacklo_epi16(M[8], M[9]); 
		r5 = _mm256_unpacklo_epi16(M[10], M[11]);
		r6 = _mm256_unpacklo_epi16(M[12], M[13]); 
		r7 = _mm256_unpacklo_epi16(M[14], M[15]); 


		temp = _mm256_unpacklo_epi32(r0, r1); 
		temp0 = _mm256_unpacklo_epi32(r2, r3); 
		temp1 = _mm256_unpacklo_epi32(r4, r5); 
		temp2 = _mm256_unpacklo_epi32(r6, r7); 

		r8 = _mm256_unpackhi_epi32(r0, r1); 
		r9 = _mm256_unpackhi_epi32(r2, r3); 
		r10 = _mm256_unpackhi_epi32(r4, r5); 
		r11 = _mm256_unpackhi_epi32(r6, r7);

		r0 = _mm256_unpacklo_epi64(temp, temp0); 
		r2 = _mm256_unpackhi_epi64(temp, temp0); 

		r1 = _mm256_unpacklo_epi64(temp1, temp2); 
		r3 = _mm256_unpackhi_epi64(temp1, temp2);

		temp = _mm256_unpackhi_epi16(M[0], M[1]); 
		temp0 = _mm256_unpackhi_epi16(M[2], M[3]); 
		temp1 = _mm256_unpackhi_epi16(M[4], M[5]); 
		temp2 = _mm256_unpackhi_epi16(M[6], M[7]); 
		r4 = _mm256_unpackhi_epi16(M[8], M[9]); 

		M[0] = _mm256_permute2f128_si256(r0, r1, 0x20);
		M[8] = _mm256_permute2f128_si256(r0, r1, 0x31);
		M[1] = _mm256_permute2f128_si256(r2, r3, 0x20);
		M[9] = _mm256_permute2f128_si256(r2, r3, 0x31);


		r5 = _mm256_unpackhi_epi16(M[10], M[11]); 
		r6 = _mm256_unpackhi_epi16(M[12], M[13]); 
		r7 = _mm256_unpackhi_epi16(M[14], M[15]); 



		r0 = _mm256_unpacklo_epi64(r8, r9); 
		r1 = _mm256_unpacklo_epi64(r10, r11); 

		r2 = _mm256_unpackhi_epi64(r8, r9); 
		r3 = _mm256_unpackhi_epi64(r10, r11); 



		M[3] = _mm256_permute2f128_si256(r2, r3, 0x20);
		M[11] = _mm256_permute2f128_si256(r2, r3, 0x31);
		M[2] = _mm256_permute2f128_si256(r0, r1, 0x20);
		M[10] = _mm256_permute2f128_si256(r0, r1, 0x31);


	//for(i=0; i<4; i=i+1)
	//{
		r0 = _mm256_unpacklo_epi32(temp, temp0); 
		r1 = _mm256_unpacklo_epi32(temp1, temp2);
		r2 = _mm256_unpacklo_epi32(r4, r5); 
		r3 = _mm256_unpacklo_epi32(r6, r7); 

	//}


	//for(i=0; i<2; i=i+1)
	//{
		r8 = _mm256_unpacklo_epi64(r0, r1); 
		r10 = _mm256_unpackhi_epi64(r0, r1); 

		r9 = _mm256_unpacklo_epi64(r2, r3); 
		r11 = _mm256_unpackhi_epi64(r2, r3); 

		M[4] = _mm256_permute2f128_si256(r8, r9, 0x20);
		M[12] = _mm256_permute2f128_si256(r8, r9, 0x31);
		M[5] = _mm256_permute2f128_si256(r10, r11, 0x20);
		M[13] = _mm256_permute2f128_si256(r10, r11, 0x31);

		r0 = _mm256_unpackhi_epi32(temp, temp0); 
		r1 = _mm256_unpackhi_epi32(temp1, temp2); 
		r2 = _mm256_unpackhi_epi32(r4, r5); 
		r3 = _mm256_unpackhi_epi32(r6, r7); 

	//}
//	for(i=0; i<2; i=i+1)
//	{
		r4 = _mm256_unpacklo_epi64(r0, r1); 
		r6 = _mm256_unpackhi_epi64(r0, r1); 

		r5 = _mm256_unpacklo_epi64(r2, r3); 
		r7 = _mm256_unpackhi_epi64(r2, r3); 

//	}

	//-------------------------------------------------------

	M[6] = _mm256_permute2f128_si256(r4, r5, 0x20);
	M[14] = _mm256_permute2f128_si256(r4, r5, 0x31);
	M[7] = _mm256_permute2f128_si256(r6, r7, 0x20);
	M[15] = _mm256_permute2f128_si256(r6, r7, 0x31);
}

/*
void transpose_unrolled(__m256i *M)
{
	int i;
	__m256i tL[8], tH[8];
	__m256i bL[4], bH[4], cL[4], cH[4];
	__m256i dL[2], dH[2], eL[2], eH[2], fL[2], fH[2], gL[2], gH[2];

	__m256i r0, r1, r2, r3, r4, r5, r6, r7;

	//for(i=0; i<8; i=i+1)
	//{
		tL[0] = _mm256_unpacklo_epi16(M[0], M[1]); 
		tH[0] = _mm256_unpackhi_epi16(M[0], M[1]); 

		tL[1] = _mm256_unpacklo_epi16(M[2], M[3]); 
		tH[1] = _mm256_unpackhi_epi16(M[2], M[3]); 

		tL[2] = _mm256_unpacklo_epi16(M[4], M[5]); 
		tH[2] = _mm256_unpackhi_epi16(M[4], M[5]); 

		tL[3] = _mm256_unpacklo_epi16(M[6], M[7]); 
		tH[3] = _mm256_unpackhi_epi16(M[6], M[7]); 

		tL[4] = _mm256_unpacklo_epi16(M[8], M[9]); 
		tH[4] = _mm256_unpackhi_epi16(M[8], M[9]); 

		tL[5] = _mm256_unpacklo_epi16(M[10], M[11]); 
		tH[5] = _mm256_unpackhi_epi16(M[10], M[11]); 

		tL[6] = _mm256_unpacklo_epi16(M[12], M[13]); 
		tH[6] = _mm256_unpackhi_epi16(M[12], M[13]); 

		tL[7] = _mm256_unpacklo_epi16(M[14], M[15]); 
		tH[7] = _mm256_unpackhi_epi16(M[14], M[15]); 

	//}

	//-------------------------------------------------------
	//for(i=0; i<4; i=i+1)
	//{
		bL[0] = _mm256_unpacklo_epi32(tL[0], tL[1]); 
		bH[0] = _mm256_unpackhi_epi32(tL[0], tL[1]); 

		bL[1] = _mm256_unpacklo_epi32(tL[2], tL[3]); 
		bH[1] = _mm256_unpackhi_epi32(tL[2], tL[3]); 

		bL[2] = _mm256_unpacklo_epi32(tL[4], tL[5]); 
		bH[2] = _mm256_unpackhi_epi32(tL[4], tL[5]); 

		bL[3] = _mm256_unpacklo_epi32(tL[6], tL[7]); 
		bH[3] = _mm256_unpackhi_epi32(tL[6], tL[7]); 

	//}

	//for(i=0; i<2; i=i+1)
	//{
		dL[0] = _mm256_unpacklo_epi64(bL[0], bL[1]); 
		dH[0] = _mm256_unpackhi_epi64(bL[0], bL[1]); 

		dL[1] = _mm256_unpacklo_epi64(bL[2], bL[3]); 
		dH[1] = _mm256_unpackhi_epi64(bL[2], bL[3]);

		M[0] = _mm256_permute2f128_si256(dL[0], dL[1], 0x20);
		M[8] = _mm256_permute2f128_si256(dL[0], dL[1], 0x31);
		M[1] = _mm256_permute2f128_si256(dH[0], dH[1], 0x20);
		M[9] = _mm256_permute2f128_si256(dH[0], dH[1], 0x31);

	//}
	//for(i=0; i<2; i=i+1)
	//{
		eL[0] = _mm256_unpacklo_epi64(bH[0], bH[1]); 
		eH[0] = _mm256_unpackhi_epi64(bH[0], bH[1]); 

		eL[1] = _mm256_unpacklo_epi64(bH[2], bH[3]); 
		eH[1] = _mm256_unpackhi_epi64(bH[2], bH[3]); 

	//}

	//-------------------------------------------------------

	//-------------------------------------------------------
	for(i=0; i<4; i=i+1)
	{
		cL[i] = _mm256_unpacklo_epi32(tH[2*i], tH[2*i+1]); 
		cH[i] = _mm256_unpackhi_epi32(tH[2*i], tH[2*i+1]); 
	}


	for(i=0; i<2; i=i+1)
	{
		fL[i] = _mm256_unpacklo_epi64(cL[2*i], cL[2*i+1]); 
		fH[i] = _mm256_unpackhi_epi64(cL[2*i], cL[2*i+1]); 
	}
	for(i=0; i<2; i=i+1)
	{
		gL[i] = _mm256_unpacklo_epi64(cH[2*i], cH[2*i+1]); 
		gH[i] = _mm256_unpackhi_epi64(cH[2*i], cH[2*i+1]); 
	}

	//-------------------------------------------------------



	M[2] = _mm256_permute2f128_si256(eL[0], eL[1], 0x20);
	M[10] = _mm256_permute2f128_si256(eL[0], eL[1], 0x31);
	M[3] = _mm256_permute2f128_si256(eH[0], eH[1], 0x20);
	M[11] = _mm256_permute2f128_si256(eH[0], eH[1], 0x31);

	M[4] = _mm256_permute2f128_si256(fL[0], fL[1], 0x20);
	M[12] = _mm256_permute2f128_si256(fL[0], fL[1], 0x31);
	M[5] = _mm256_permute2f128_si256(fH[0], fH[1], 0x20);
	M[13] = _mm256_permute2f128_si256(fH[0], fH[1], 0x31);

	M[6] = _mm256_permute2f128_si256(gL[0], gL[1], 0x20);
	M[14] = _mm256_permute2f128_si256(gL[0], gL[1], 0x31);
	M[7] = _mm256_permute2f128_si256(gH[0], gH[1], 0x20);
	M[15] = _mm256_permute2f128_si256(gH[0], gH[1], 0x31);
}


void transpose1(__m256i *M)
{
	int i;
	__m256i tL[8], tH[8];
	__m256i bL[4], bH[4], cL[4], cH[4];
	__m256i dL[2], dH[2], eL[2], eH[2], fL[2], fH[2], gL[2], gH[2];

	for(i=0; i<8; i=i+1)
	{
		tL[i] = _mm256_unpacklo_epi16(M[2*i], M[2*i+1]); 
		tH[i] = _mm256_unpackhi_epi16(M[2*i], M[2*i+1]); 
	}

	for(i=0; i<4; i=i+1)
	{
		bL[i] = _mm256_unpacklo_epi32(tL[2*i], tL[2*i+1]); 
		bH[i] = _mm256_unpackhi_epi32(tL[2*i], tL[2*i+1]); 
	}
	for(i=0; i<4; i=i+1)
	{
		cL[i] = _mm256_unpacklo_epi32(tH[2*i], tH[2*i+1]); 
		cH[i] = _mm256_unpackhi_epi32(tH[2*i], tH[2*i+1]); 
	}

	for(i=0; i<2; i=i+1)
	{
		dL[i] = _mm256_unpacklo_epi64(bL[2*i], bL[2*i+1]); 
		dH[i] = _mm256_unpackhi_epi64(bL[2*i], bL[2*i+1]); 
	}
	for(i=0; i<2; i=i+1)
	{
		eL[i] = _mm256_unpacklo_epi64(bH[2*i], bH[2*i+1]); 
		eH[i] = _mm256_unpackhi_epi64(bH[2*i], bH[2*i+1]); 
	}

	for(i=0; i<2; i=i+1)
	{
		fL[i] = _mm256_unpacklo_epi64(cL[2*i], cL[2*i+1]); 
		fH[i] = _mm256_unpackhi_epi64(cL[2*i], cL[2*i+1]); 
	}
	for(i=0; i<2; i=i+1)
	{
		gL[i] = _mm256_unpacklo_epi64(cH[2*i], cH[2*i+1]); 
		gH[i] = _mm256_unpackhi_epi64(cH[2*i], cH[2*i+1]); 
	}

	M[0] = _mm256_permute2f128_si256(dL[0], dL[1], 0x20);
	M[8] = _mm256_permute2f128_si256(dL[0], dL[1], 0x31);
	M[1] = _mm256_permute2f128_si256(dH[0], dH[1], 0x20);
	M[9] = _mm256_permute2f128_si256(dH[0], dH[1], 0x31);

	M[2] = _mm256_permute2f128_si256(eL[0], eL[1], 0x20);
	M[10] = _mm256_permute2f128_si256(eL[0], eL[1], 0x31);
	M[3] = _mm256_permute2f128_si256(eH[0], eH[1], 0x20);
	M[11] = _mm256_permute2f128_si256(eH[0], eH[1], 0x31);

	M[4] = _mm256_permute2f128_si256(fL[0], fL[1], 0x20);
	M[12] = _mm256_permute2f128_si256(fL[0], fL[1], 0x31);
	M[5] = _mm256_permute2f128_si256(fH[0], fH[1], 0x20);
	M[13] = _mm256_permute2f128_si256(fH[0], fH[1], 0x31);

	M[6] = _mm256_permute2f128_si256(gL[0], gL[1], 0x20);
	M[14] = _mm256_permute2f128_si256(gL[0], gL[1], 0x31);
	M[7] = _mm256_permute2f128_si256(gH[0], gH[1], 0x20);
	M[15] = _mm256_permute2f128_si256(gH[0], gH[1], 0x31);
}
*/
