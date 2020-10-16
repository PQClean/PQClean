//#define SCM_SIZE 16

//#pragma STDC FP_CONTRACT ON

#include <immintrin.h>

static inline __m256i mul_add(__m256i a, __m256i b, __m256i c) { 
    return _mm256_add_epi16(_mm256_mullo_epi16(a, b), c);
}


static void schoolbook_avx_new3_acc(__m256i* a, __m256i* b, __m256i* c_avx) ////8 coefficients of a and b has been prefetched
									      //the c_avx are added cummulatively
{

	register __m256i a0, a1, a2, a3, a4, a5, a6, a7, b0, b1, b2, b3, b4, b5, b6, b7;
	register __m256i temp;


	a0=a[0];
	a1=a[1];
	a2=a[2];
	a3=a[3];
	a4=a[4];
	a5=a[5];
	a6=a[6];
	a7=a[7];

	b0=b[0];
	b1=b[1];
	b2=b[2];
	b3=b[3];
	b4=b[4];
	b5=b[5];
	b6=b[6];
	b7=b[7];

	// New Unrolled first triangle

	//otherwise accumulate
	c_avx[0] = mul_add(a0, b0, c_avx[0]);
	

	temp = _mm256_mullo_epi16 (a0, b1);
	temp=mul_add(a1, b0, temp);
	c_avx[1] = _mm256_add_epi16(temp, c_avx[1]);


	temp = _mm256_mullo_epi16 (a0, b2);
	temp = mul_add(a1, b1, temp);
	temp=mul_add(a2, b0, temp);
	c_avx[2] = _mm256_add_epi16(temp, c_avx[2]);
	

	temp = _mm256_mullo_epi16 (a0, b3);
	temp = mul_add(a1, b2, temp);
	temp = mul_add(a2, b1, temp);
	temp=mul_add(a3, b0, temp);
	c_avx[3] = _mm256_add_epi16(temp, c_avx[3]);

	temp = _mm256_mullo_epi16 (a0, b4);
	temp = mul_add(a1, b3, temp);
	temp = mul_add(a3, b1, temp);
	temp = mul_add(a4, b0, temp);
	temp=mul_add(a2, b2, temp);
	c_avx[4] = _mm256_add_epi16(temp, c_avx[4]);


	temp = _mm256_mullo_epi16 (a0, b5);
	temp = mul_add(a1, b4 , temp);
	temp = mul_add(a2, b3, temp);
	temp = mul_add(a3, b2, temp);
	temp = mul_add( a4, b1, temp);
	temp=mul_add(a5, b0, temp);
	c_avx[5] = _mm256_add_epi16(temp, c_avx[5]);
	
	temp = _mm256_mullo_epi16 (a0, b6);
	temp = mul_add(a1, b5, temp);
	temp = mul_add(a5, b1, temp);
	temp = mul_add(a6, b0, temp);
	temp = mul_add(a2, b4, temp);
	temp = mul_add(a3, b3, temp);
	temp=mul_add(a4, b2, temp);
	c_avx[6] = _mm256_add_epi16(temp, c_avx[6]);


	temp = _mm256_mullo_epi16 (a0, b7);
	temp = mul_add(a1, b6, temp);
	temp = mul_add (a6, b1, temp);
	temp = mul_add (a7, b0, temp);
	temp = mul_add(a2, b5, temp);
	temp = mul_add (a3, b4, temp);
	temp = mul_add (a4, b3, temp);
	temp=mul_add(a5, b2, temp);
	c_avx[7] = _mm256_add_epi16(temp, c_avx[7]);

	temp = _mm256_mullo_epi16 (a0, b[8]);
	temp = mul_add (a1, b7, temp);
	temp = mul_add (a7, b1, temp);
	temp = mul_add (a[8], b0, temp);
	temp = mul_add (a2, b6,temp);
	temp = mul_add(a3, b5, temp);
	temp = mul_add (a4, b4,temp);
	temp = mul_add (a5, b3, temp);
	
		temp=mul_add(a6, b2, temp);
		c_avx[8] = _mm256_add_epi16(temp, c_avx[8]);


	temp = _mm256_mullo_epi16 (a0, b[9]);
	temp = mul_add (a1, b[8], temp);
	temp = mul_add (a[8], b1, temp);
	temp = mul_add (a[9], b0, temp);
	temp = mul_add (a2, b7, temp);
	temp = mul_add (a3, b6, temp);
	temp = mul_add (a4, b5, temp);
	temp = mul_add (a5, b4, temp);
	temp = mul_add (a6, b3, temp);
		temp=mul_add(a7, b2, temp);
		c_avx[9] = _mm256_add_epi16(temp, c_avx[9]);


	temp= _mm256_mullo_epi16 (a0, b[10]);
	temp = mul_add (a1, b[9], temp);
	temp = mul_add (a[9], b1, temp);
	temp = mul_add (a[10], b0, temp);
	temp = mul_add (a2, b[8], temp);
	temp = mul_add (a3, b7, temp);
	temp = mul_add (a4, b6, temp);
	temp = mul_add (a5, b5, temp);
	temp = mul_add (a6, b4, temp);
	temp = mul_add (a7, b3, temp);
		temp=mul_add(a[8], b2, temp);
		c_avx[10] = _mm256_add_epi16(temp, c_avx[10]);


	temp = _mm256_mullo_epi16 (a0, b[11]);
	temp = mul_add (a1, b[10], temp );
	temp = mul_add (a[10], b1, temp );
	temp = mul_add (a[11], b0, temp );
	temp = mul_add (a2, b[9], temp );
	temp = mul_add (a3, b[8], temp );
	temp = mul_add (a4, b7, temp );
	temp = mul_add (a5, b6, temp );
	temp = mul_add (a6, b5, temp );
	temp = mul_add (a7, b4, temp );
	temp = mul_add (a[8], b3, temp );
		temp=mul_add(a[9], b2, temp);
		c_avx[11] = _mm256_add_epi16(temp, c_avx[11]);


	temp = _mm256_mullo_epi16 (a0, b[12]);
	temp = mul_add (a1, b[11], temp);
	temp = mul_add (a[11], b1, temp);
	temp = mul_add (a[12], b0, temp);
	temp = mul_add (a2, b[10], temp);
	temp = mul_add (a3, b[9], temp);
	temp = mul_add (a4, b[8], temp);
	temp = mul_add (a5, b7, temp);
	temp = mul_add (a6, b6, temp);
	temp = mul_add (a7, b5, temp);
	temp = mul_add (a[8], b4, temp);
	temp = mul_add (a[9], b3, temp);
		temp=mul_add(a[10], b2, temp);
		c_avx[12] = _mm256_add_epi16(temp, c_avx[12]);


	temp = _mm256_mullo_epi16 (a0, b[13]);
	temp = mul_add (a1, b[12], temp );
	temp = mul_add (a[12], b1, temp );
	temp = mul_add (a[13], b0, temp );
	temp = mul_add (a2, b[11], temp );
	temp = mul_add (a3, b[10], temp );
	temp = mul_add (a4, b[9], temp );
	temp = mul_add (a5, b[8], temp );
	temp = mul_add (a6, b7, temp );
	temp = mul_add (a7, b6, temp );
	temp = mul_add (a[8], b5, temp );
	temp = mul_add (a[9], b4, temp );
	temp = mul_add (a[10], b3, temp );
		temp=mul_add(a[11], b2, temp);
		c_avx[13] = _mm256_add_epi16(temp, c_avx[13]);



	temp = _mm256_mullo_epi16 (a0, b[14]);
	temp = mul_add (a1, b[13], temp );
	temp = mul_add (a[13], b1, temp );
	temp = mul_add (a[14], b0, temp );
	temp = mul_add (a2, b[12], temp );
	temp = mul_add (a3, b[11], temp );
	temp = mul_add (a4, b[10], temp );
	temp = mul_add (a5, b[9], temp );
	temp = mul_add (a6, b[8], temp );
	temp = mul_add (a7, b7, temp );
	temp = mul_add (a[8], b6, temp );
	temp = mul_add (a[9], b5, temp );
	temp = mul_add (a[10], b4, temp );
	temp = mul_add (a[11], b3, temp );
		temp=mul_add(a[12], b2, temp);
		c_avx[14] = _mm256_add_epi16(temp, c_avx[14]);


	temp = _mm256_mullo_epi16 (a0, b[15]);
	temp = mul_add (a1, b[14], temp );
	temp = mul_add (a[14], b1, temp );
	temp = mul_add (a[15], b0, temp );
	temp = mul_add (a2, b[13], temp );
	temp = mul_add (a3, b[12], temp );
	temp = mul_add (a4, b[11], temp );
	temp = mul_add (a5, b[10], temp );
	temp = mul_add (a6, b[9], temp );
	temp = mul_add (a7, b[8], temp );
	temp = mul_add (a[8], b7, temp );
	temp = mul_add (a[9], b6, temp );
	temp = mul_add (a[10], b5, temp );
	temp = mul_add (a[11], b4, temp );
	temp = mul_add (a[12], b3, temp );
		temp=mul_add(a[13], b2, temp);
		c_avx[15] = _mm256_add_epi16(temp, c_avx[15]);


	// unrolled second triangle
	a0=a[14];
	a1=a[15];
	a2=a[13];
	a3=a[12];
	a4=a[11];
	a5=a[10];
	a6=a[9];
	a7=a[8];

	b0=b[14];
	b1=b[15];
	b2=b[13];
	b3=b[12];
	b4=b[11];
	b5=b[10];
	b6=b[9];
	b7=b[8];

	temp = _mm256_mullo_epi16 (a[1], b1);
	temp = mul_add (a[2], b0, temp );
	temp = mul_add (a[3], b2, temp );
	temp = mul_add (a[4], b3, temp );
	temp = mul_add (a[5], b4, temp );
	temp = mul_add (a[6], b5, temp );
	temp = mul_add (a[7], b6, temp );
	temp = mul_add (a7, b7, temp );
	temp = mul_add (a6, b[7], temp );
	temp = mul_add (a5, b[6], temp );
	temp = mul_add (a4, b[5], temp );
	temp = mul_add (a3, b[4], temp );
	temp = mul_add (a2, b[3], temp );
	temp = mul_add (a0, b[2], temp );
		temp=mul_add(a1, b[1], temp);
		c_avx[16] = _mm256_add_epi16(temp, c_avx[16]);


	temp = _mm256_mullo_epi16 (a[2], b1);
	temp = mul_add (a[3], b0, temp );
	temp = mul_add (a[4], b2, temp );
	temp = mul_add (a[5], b3, temp );
	temp = mul_add (a[6], b4, temp );
	temp = mul_add (a[7], b5, temp );
	temp = mul_add (a7, b6, temp );
	temp = mul_add (a6, b7, temp );
	temp = mul_add (a5, b[7], temp );
	temp = mul_add (a4, b[6], temp );
	temp = mul_add (a3, b[5], temp );
	temp = mul_add (a2, b[4], temp );
	temp = mul_add (a0, b[3], temp );
		temp=mul_add(a1, b[2], temp);
		c_avx[17] = _mm256_add_epi16(temp, c_avx[17]);


	temp = _mm256_mullo_epi16 (a[3], b1);
	temp = mul_add (a[4], b0, temp );
	temp = mul_add (a[5], b2, temp );
	temp = mul_add (a[6], b3, temp );
	temp = mul_add (a[7], b4, temp );
	temp = mul_add (a7, b5, temp );
	temp = mul_add (a6, b6, temp );
	temp = mul_add (a5, b7, temp );
	temp = mul_add (a4, b[7], temp );
	temp = mul_add (a3, b[6], temp );
	temp = mul_add (a2, b[5], temp );
	temp = mul_add (a0, b[4], temp );
		temp=mul_add(a1, b[3], temp);
		c_avx[18] = _mm256_add_epi16(temp, c_avx[18]);


	temp = _mm256_mullo_epi16 (a[4], b1);
	temp = mul_add (a[5], b0, temp );
	temp = mul_add (a[6], b2, temp );
	temp = mul_add (a[7], b3, temp );
	temp = mul_add (a7, b4, temp );
	temp = mul_add (a6, b5, temp );
	temp = mul_add (a5, b6, temp );
	temp = mul_add (a4, b7, temp );
	temp = mul_add (a3, b[7], temp );
	temp = mul_add (a2, b[6], temp );
	temp = mul_add (a0, b[5], temp );
		temp=mul_add(a1, b[4], temp);
		c_avx[19] = _mm256_add_epi16(temp, c_avx[19]);


	temp = _mm256_mullo_epi16 (a[5], b1);
	temp = mul_add (a[6], b0, temp );
	temp = mul_add (a[7], b2, temp );
	temp = mul_add (a7, b3, temp );
	temp = mul_add (a6, b4, temp );
	temp = mul_add (a5, b5, temp );
	temp = mul_add (a4, b6, temp );
	temp = mul_add (a3, b7, temp );
	temp = mul_add (a2, b[7], temp );
	temp = mul_add (a0, b[6], temp );
		temp=mul_add(a1, b[5], temp);
		c_avx[20] = _mm256_add_epi16(temp, c_avx[20]);


	temp = _mm256_mullo_epi16 (a[6], b1);
	temp = mul_add (a[7], b0, temp );
	temp = mul_add (a7, b2, temp );
	temp = mul_add (a6, b3, temp );
	temp = mul_add (a5, b4, temp );
	temp = mul_add (a4, b5, temp );
	temp = mul_add (a3, b6, temp );
	temp = mul_add (a2, b7, temp );
	temp = mul_add (a0, b[7], temp );
		temp=mul_add(a1, b[6], temp);
		c_avx[21] = _mm256_add_epi16(temp, c_avx[21]);


	temp = _mm256_mullo_epi16 (a[7], b1);
	temp = mul_add (a7, b0, temp );
	temp = mul_add (a6, b2, temp );
	temp = mul_add (a5, b3, temp );
	temp = mul_add (a4, b4, temp );
	temp = mul_add (a3, b5, temp );
	temp = mul_add (a2, b6, temp );
	temp = mul_add (a0, b7, temp );
		temp=mul_add(a1, b[7], temp);
		c_avx[22] = _mm256_add_epi16(temp, c_avx[22]);


	temp = _mm256_mullo_epi16 (a7, b1);
	temp = mul_add (a6, b0, temp );
	temp = mul_add (a5, b2, temp );
	temp = mul_add (a4, b3, temp );
	temp = mul_add (a3, b4, temp );
	temp = mul_add (a2, b5, temp );
	temp = mul_add (a0, b6, temp );
		temp=mul_add(a1, b7, temp);
		c_avx[23] = _mm256_add_epi16(temp, c_avx[23]);


	temp = _mm256_mullo_epi16 (a6, b1);
	temp = mul_add (a5, b0, temp );
	temp = mul_add (a4, b2, temp );
	temp = mul_add (a3, b3, temp );
	temp = mul_add (a2, b4, temp );
	temp = mul_add (a0, b5, temp );
		temp=mul_add(a1, b6, temp);
		c_avx[24] = _mm256_add_epi16(temp, c_avx[24]);


	temp = _mm256_mullo_epi16 (a5, b1);
	temp = mul_add (a4, b0, temp );
	temp = mul_add (a3, b2, temp );
	temp = mul_add (a2, b3, temp );
	temp = mul_add (a0, b4, temp );
		temp=mul_add(a1, b5, temp);
		c_avx[25] = _mm256_add_epi16(temp, c_avx[25]);


	temp = _mm256_mullo_epi16 (a4, b1);
	temp = mul_add (a3, b0, temp );
	temp = mul_add (a2, b2, temp );
	temp = mul_add (a0, b3, temp );
		temp=mul_add(a1, b4, temp);
		c_avx[26] = _mm256_add_epi16(temp, c_avx[26]);


	temp = _mm256_mullo_epi16 (a3, b1);
	temp = mul_add (a2, b0, temp );
	temp = mul_add (a0, b2, temp );
		temp=mul_add(a1, b3, temp);
		c_avx[27] = _mm256_add_epi16(temp, c_avx[27]);


	temp = _mm256_mullo_epi16 (a2, b1);
	temp = mul_add (a0, b0, temp );
		temp=mul_add(a1, b2, temp);
		c_avx[28] = _mm256_add_epi16(temp, c_avx[28]);


	temp = _mm256_mullo_epi16 (a0, b1);
		temp=mul_add(a1, b0, temp);
		c_avx[29] = _mm256_add_epi16(temp, c_avx[29]);


		c_avx[30] = mul_add(a1, b1, c_avx[30]);



	c_avx[2*SCM_SIZE-1] = _mm256_set_epi64x(0, 0, 0, 0);


}



static void schoolbook_avx_new2(__m256i* a, __m256i* b, __m256i* c_avx) ////8 coefficients of a and b has been prefetched
									      //the c_avx are not added cummulatively
{

	__m256i a0, a1, a2, a3, a4, a5, a6, a7, b0, b1, b2, b3, b4, b5, b6, b7;
	__m256i temp;


	a0=a[0];
	a1=a[1];
	a2=a[2];
	a3=a[3];
	a4=a[4];
	a5=a[5];
	a6=a[6];
	a7=a[7];

	b0=b[0];
	b1=b[1];
	b2=b[2];
	b3=b[3];
	b4=b[4];
	b5=b[5];
	b6=b[6];
	b7=b[7];

	// New Unrolled first triangle
	c_avx[0] = _mm256_mullo_epi16 (a0, b0);

	temp = _mm256_mullo_epi16 (a0, b1);
	c_avx[1]=mul_add(a1, b0, temp);

	temp = _mm256_mullo_epi16 (a0, b2);

	temp = mul_add(a1, b1, temp);
	c_avx[2]= mul_add(a2, b0, temp);

	temp = _mm256_mullo_epi16 (a0, b3);
	temp = mul_add(a1, b2, temp);
	temp = mul_add(a2, b1, temp);
	c_avx[3]= mul_add(a3, b0, temp);

	temp = _mm256_mullo_epi16 (a0, b4);
	temp = mul_add(a1, b3, temp);
	temp = mul_add(a3, b1, temp);
	temp = mul_add(a4, b0, temp);
	c_avx[4]= mul_add(a2, b2, temp);

	temp = _mm256_mullo_epi16 (a0, b5);
	temp = mul_add(a1, b4 , temp);
	temp = mul_add(a2, b3, temp);
	temp = mul_add(a3, b2, temp);
	temp = mul_add( a4, b1, temp);
	c_avx[5] = mul_add(a5, b0, temp);
	
	temp = _mm256_mullo_epi16 (a0, b6);
	temp = mul_add(a1, b5, temp);
	temp = mul_add(a5, b1, temp);
	temp = mul_add(a6, b0, temp);
	temp = mul_add(a2, b4, temp);
	temp = mul_add(a3, b3, temp);
	c_avx[6] = mul_add(a4, b2, temp);

	temp = _mm256_mullo_epi16 (a0, b7);
	temp = mul_add(a1, b6, temp);
	temp = mul_add (a6, b1, temp);
	temp = mul_add (a7, b0, temp);
	temp = mul_add(a2, b5, temp);
	temp = mul_add (a3, b4, temp);
	temp = mul_add (a4, b3, temp);
	c_avx[7] = mul_add (a5, b2, temp);

	temp = _mm256_mullo_epi16 (a0, b[8]);
	temp = mul_add (a1, b7, temp);
	temp = mul_add (a7, b1, temp);
	temp = mul_add (a[8], b0, temp);
	temp = mul_add (a2, b6,temp);
	temp = mul_add(a3, b5, temp);
	temp = mul_add (a4, b4,temp);
	temp = mul_add (a5, b3, temp);
	c_avx[8] = mul_add (a6, b2, temp);

	temp = _mm256_mullo_epi16 (a0, b[9]);
	temp = mul_add (a1, b[8], temp);
	temp = mul_add (a[8], b1, temp);
	temp = mul_add (a[9], b0, temp);
	temp = mul_add (a2, b7, temp);
	temp = mul_add (a3, b6, temp);
	temp = mul_add (a4, b5, temp);
	temp = mul_add (a5, b4, temp);
	temp = mul_add (a6, b3, temp);
	c_avx[9] = mul_add (a7, b2, temp);

	temp= _mm256_mullo_epi16 (a0, b[10]);
	temp = mul_add (a1, b[9], temp);
	temp = mul_add (a[9], b1, temp);
	temp = mul_add (a[10], b0, temp);
	temp = mul_add (a2, b[8], temp);
	temp = mul_add (a3, b7, temp);
	temp = mul_add (a4, b6, temp);
	temp = mul_add (a5, b5, temp);
	temp = mul_add (a6, b4, temp);
	temp = mul_add (a7, b3, temp);
	c_avx[10] = mul_add (a[8], b2, temp);

	temp = _mm256_mullo_epi16 (a0, b[11]);
	temp = mul_add (a1, b[10], temp );
	temp = mul_add (a[10], b1, temp );
	temp = mul_add (a[11], b0, temp );
	temp = mul_add (a2, b[9], temp );
	temp = mul_add (a3, b[8], temp );
	temp = mul_add (a4, b7, temp );
	temp = mul_add (a5, b6, temp );
	temp = mul_add (a6, b5, temp );
	temp = mul_add (a7, b4, temp );
	temp = mul_add (a[8], b3, temp );
	c_avx[11] = mul_add (a[9], b2, temp );

	temp = _mm256_mullo_epi16 (a0, b[12]);
	temp = mul_add (a1, b[11], temp);
	temp = mul_add (a[11], b1, temp);
	temp = mul_add (a[12], b0, temp);
	temp = mul_add (a2, b[10], temp);
	temp = mul_add (a3, b[9], temp);
	temp = mul_add (a4, b[8], temp);
	temp = mul_add (a5, b7, temp);
	temp = mul_add (a6, b6, temp);
	temp = mul_add (a7, b5, temp);
	temp = mul_add (a[8], b4, temp);
	temp = mul_add (a[9], b3, temp);
	c_avx[12] = mul_add (a[10], b2, temp);

	temp = _mm256_mullo_epi16 (a0, b[13]);
	temp = mul_add (a1, b[12], temp );
	temp = mul_add (a[12], b1, temp );
	temp = mul_add (a[13], b0, temp );
	temp = mul_add (a2, b[11], temp );
	temp = mul_add (a3, b[10], temp );
	temp = mul_add (a4, b[9], temp );
	temp = mul_add (a5, b[8], temp );
	temp = mul_add (a6, b7, temp );
	temp = mul_add (a7, b6, temp );
	temp = mul_add (a[8], b5, temp );
	temp = mul_add (a[9], b4, temp );
	temp = mul_add (a[10], b3, temp );
	c_avx[13] = mul_add (a[11], b2, temp );

	temp = _mm256_mullo_epi16 (a0, b[14]);
	temp = mul_add (a1, b[13], temp );
	temp = mul_add (a[13], b1, temp );
	temp = mul_add (a[14], b0, temp );
	temp = mul_add (a2, b[12], temp );
	temp = mul_add (a3, b[11], temp );
	temp = mul_add (a4, b[10], temp );
	temp = mul_add (a5, b[9], temp );
	temp = mul_add (a6, b[8], temp );
	temp = mul_add (a7, b7, temp );
	temp = mul_add (a[8], b6, temp );
	temp = mul_add (a[9], b5, temp );
	temp = mul_add (a[10], b4, temp );
	temp = mul_add (a[11], b3, temp );
	c_avx[14] = mul_add (a[12], b2, temp );

	temp = _mm256_mullo_epi16 (a0, b[15]);
	temp = mul_add (a1, b[14], temp );
	temp = mul_add (a[14], b1, temp );
	temp = mul_add (a[15], b0, temp );
	temp = mul_add (a2, b[13], temp );
	temp = mul_add (a3, b[12], temp );
	temp = mul_add (a4, b[11], temp );
	temp = mul_add (a5, b[10], temp );
	temp = mul_add (a6, b[9], temp );
	temp = mul_add (a7, b[8], temp );
	temp = mul_add (a[8], b7, temp );
	temp = mul_add (a[9], b6, temp );
	temp = mul_add (a[10], b5, temp );
	temp = mul_add (a[11], b4, temp );
	temp = mul_add (a[12], b3, temp );
	c_avx[15] = mul_add (a[13], b2, temp );


	// unrolled second triangle
	a0=a[14];
	a1=a[15];
	a2=a[13];
	a3=a[12];
	a4=a[11];
	a5=a[10];
	a6=a[9];
	a7=a[8];

	b0=b[14];
	b1=b[15];
	b2=b[13];
	b3=b[12];
	b4=b[11];
	b5=b[10];
	b6=b[9];
	b7=b[8];
	

	temp = _mm256_mullo_epi16 (a[1], b1);
	temp = mul_add (a[2], b0, temp );
	temp = mul_add (a[3], b2, temp );
	temp = mul_add (a[4], b3, temp );
	temp = mul_add (a[5], b4, temp );
	temp = mul_add (a[6], b5, temp );
	temp = mul_add (a[7], b6, temp );
	temp = mul_add (a7, b7, temp );
	temp = mul_add (a6, b[7], temp );
	temp = mul_add (a5, b[6], temp );
	temp = mul_add (a4, b[5], temp );
	temp = mul_add (a3, b[4], temp );
	temp = mul_add (a2, b[3], temp );
	temp = mul_add (a0, b[2], temp );
	c_avx[16] = mul_add (a1, b[1], temp );

	temp = _mm256_mullo_epi16 (a[2], b1);
	temp = mul_add (a[3], b0, temp );
	temp = mul_add (a[4], b2, temp );
	temp = mul_add (a[5], b3, temp );
	temp = mul_add (a[6], b4, temp );
	temp = mul_add (a[7], b5, temp );
	temp = mul_add (a7, b6, temp );
	temp = mul_add (a6, b7, temp );
	temp = mul_add (a5, b[7], temp );
	temp = mul_add (a4, b[6], temp );
	temp = mul_add (a3, b[5], temp );
	temp = mul_add (a2, b[4], temp );
	temp = mul_add (a0, b[3], temp );
	c_avx[17] = mul_add (a1, b[2], temp );

	temp = _mm256_mullo_epi16 (a[3], b1);
	temp = mul_add (a[4], b0, temp );
	temp = mul_add (a[5], b2, temp );
	temp = mul_add (a[6], b3, temp );
	temp = mul_add (a[7], b4, temp );
	temp = mul_add (a7, b5, temp );
	temp = mul_add (a6, b6, temp );
	temp = mul_add (a5, b7, temp );
	temp = mul_add (a4, b[7], temp );
	temp = mul_add (a3, b[6], temp );
	temp = mul_add (a2, b[5], temp );
	temp = mul_add (a0, b[4], temp );
	c_avx[18] = mul_add (a1, b[3], temp );

	temp = _mm256_mullo_epi16 (a[4], b1);
	temp = mul_add (a[5], b0, temp );
	temp = mul_add (a[6], b2, temp );
	temp = mul_add (a[7], b3, temp );
	temp = mul_add (a7, b4, temp );
	temp = mul_add (a6, b5, temp );
	temp = mul_add (a5, b6, temp );
	temp = mul_add (a4, b7, temp );
	temp = mul_add (a3, b[7], temp );
	temp = mul_add (a2, b[6], temp );
	temp = mul_add (a0, b[5], temp );
	c_avx[19] = mul_add (a1, b[4], temp );

	temp = _mm256_mullo_epi16 (a[5], b1);
	temp = mul_add (a[6], b0, temp );
	temp = mul_add (a[7], b2, temp );
	temp = mul_add (a7, b3, temp );
	temp = mul_add (a6, b4, temp );
	temp = mul_add (a5, b5, temp );
	temp = mul_add (a4, b6, temp );
	temp = mul_add (a3, b7, temp );
	temp = mul_add (a2, b[7], temp );
	temp = mul_add (a0, b[6], temp );
	c_avx[20] = mul_add (a1, b[5], temp );

	temp = _mm256_mullo_epi16 (a[6], b1);
	temp = mul_add (a[7], b0, temp );
	temp = mul_add (a7, b2, temp );
	temp = mul_add (a6, b3, temp );
	temp = mul_add (a5, b4, temp );
	temp = mul_add (a4, b5, temp );
	temp = mul_add (a3, b6, temp );
	temp = mul_add (a2, b7, temp );
	temp = mul_add (a0, b[7], temp );
	c_avx[21] = mul_add (a1, b[6], temp );

	temp = _mm256_mullo_epi16 (a[7], b1);
	temp = mul_add (a7, b0, temp );
	temp = mul_add (a6, b2, temp );
	temp = mul_add (a5, b3, temp );
	temp = mul_add (a4, b4, temp );
	temp = mul_add (a3, b5, temp );
	temp = mul_add (a2, b6, temp );
	temp = mul_add (a0, b7, temp );
	c_avx[22] = mul_add (a1, b[7], temp );

	temp = _mm256_mullo_epi16 (a7, b1);
	temp = mul_add (a6, b0, temp );
	temp = mul_add (a5, b2, temp );
	temp = mul_add (a4, b3, temp );
	temp = mul_add (a3, b4, temp );
	temp = mul_add (a2, b5, temp );
	temp = mul_add (a0, b6, temp );
	c_avx[23] = mul_add (a1, b7, temp );

	temp = _mm256_mullo_epi16 (a6, b1);
	temp = mul_add (a5, b0, temp );
	temp = mul_add (a4, b2, temp );
	temp = mul_add (a3, b3, temp );
	temp = mul_add (a2, b4, temp );
	temp = mul_add (a0, b5, temp );
	c_avx[24] = mul_add (a1, b6, temp );

	temp = _mm256_mullo_epi16 (a5, b1);
	temp = mul_add (a4, b0, temp );
	temp = mul_add (a3, b2, temp );
	temp = mul_add (a2, b3, temp );
	temp = mul_add (a0, b4, temp );
	c_avx[25] = mul_add (a1, b5, temp );

	temp = _mm256_mullo_epi16 (a4, b1);
	temp = mul_add (a3, b0, temp );
	temp = mul_add (a2, b2, temp );
	temp = mul_add (a0, b3, temp );
	c_avx[26] = mul_add (a1, b4, temp );

	temp = _mm256_mullo_epi16 (a3, b1);
	temp = mul_add (a2, b0, temp );
	temp = mul_add (a0, b2, temp );
	c_avx[27] = mul_add (a1, b3, temp );

	temp = _mm256_mullo_epi16 (a2, b1);
	temp = mul_add (a0, b0, temp );
	c_avx[28] = mul_add (a1, b2, temp );

	temp = _mm256_mullo_epi16 (a0, b1);
	c_avx[29] = mul_add (a1, b0, temp);

	c_avx[30] = _mm256_mullo_epi16 (a1, b1);


	c_avx[2*SCM_SIZE-1] = _mm256_set_epi64x(0, 0, 0, 0);

}
