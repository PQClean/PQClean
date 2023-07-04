#include "ntt_consts.h"

#define PADDING 0

const int16_t qmvq[8] = {FALCON_Q, FALCON_QINV,
                         FALCON_MONT, FALCON_NINV_MONT,
                         FALCON_V, 0,
                         FALCON_MONT_BR, FALCON_NINV_MONT_BR};


const int16_t ntt_br[] = {
 PADDING,    -1479,    -5146,     4043,  PADDING,  PADDING,  PADDING,  PADDING, 
   -1305,     3542,    -3504,    -4821,     2639,    -2625,     -949,     2319, 
   -1170,     -955,     -790,    -3201,     3014,     5086,    -1326,  PADDING, 
    1260,     1260,     1260,     1260,     4632,     4632,     4632,     4632, 
    2426,     2426,     2426,     2426,     1428,     1428,     1428,     1428, 
    2013,     2013,     2013,     2013,      729,      729,      729,      729, 
    2881,     2881,     2881,     2881,    -5092,    -5092,    -5092,    -5092, 
    4388,     4388,     4388,     4388,    -5755,    -5755,    -5755,    -5755, 
     334,      334,      334,      334,     1696,     1696,     1696,     1696, 
   -3289,    -3289,    -3289,    -3289,     3241,     3241,     3241,     3241, 
    3284,     3284,     3284,     3284,    -2089,    -2089,    -2089,    -2089, 
    2401,      442,    -5101,    -1067,      390,      773,    -3833,     3778, 
     354,     4861,    -2912,     5698,     5012,    -2481,     2859,    -1045, 
    1017,    -4885,     1632,    -5084,       27,    -3066,    -3763,    -1440, 
    1537,      242,     4714,    -4143,    -2678,     3704,     5019,     -545, 
      49,     5915,    -2500,    -1583,     1512,    -1815,     5369,    -3202, 
   -2738,    -5735,    -3009,      174,    -1975,      347,    -3315,     1858, 
    3030,     2361,     2908,     3434,     3963,     6142,     1954,    -2882, 
    3991,    -2767,     2281,    -2031,     3772,     5908,     5429,    -4737, 
    1263,     1483,    -1489,    -5942,      350,     5383,    -2057,     4493, 
   -5868,     2655,     1693,      723,    -3757,     2925,     -426,     4754, 
    4115,    -1843,      218,    -3529,      576,    -2447,    -2051,    -1805, 
   -3969,      156,     5876,     5333,      418,     -453,    -4774,     1293, 
     722,    -2545,     3621,     -563,    -2975,    -3006,    -2744,     4846, 
   -2747,    -3135,     3712,     4805,    -3553,    -1062,    -2294,  PADDING, 
   -3694,    -3694,    -3694,    -3694,    -1759,    -1759,    -1759,    -1759, 
    3382,     3382,     3382,     3382,    -2548,    -2548,    -2548,    -2548, 
    3637,     3637,     3637,     3637,      145,      145,      145,      145, 
   -2731,    -2731,    -2731,    -2731,    -4890,    -4890,    -4890,    -4890, 
   -5179,    -5179,    -5179,    -5179,    -3707,    -3707,    -3707,    -3707, 
    -355,     -355,     -355,     -355,    -4231,    -4231,    -4231,    -4231, 
    3459,     3459,     3459,     3459,    -5542,    -5542,    -5542,    -5542, 
   -3932,    -3932,    -3932,    -3932,    -5911,    -5911,    -5911,    -5911, 
    1002,     5011,     5088,    -4284,    -4976,    -1607,    -3780,     -875, 
   -2437,     3646,     6022,     2987,    -2566,    -2187,    -6039,    -2422, 
   -1065,     2143,     -404,    -4645,     1168,     5277,    -1207,     3248, 
     493,    -4096,    -5444,     2381,    -4337,     -435,     1378,     1912, 
     295,     5766,    -4016,    -3762,      325,    -1146,     5990,    -3728, 
    3329,     -168,     5961,    -1962,    -6122,    -5184,     1360,    -6119, 
   -4079,      922,     1958,     1112,     4046,    -3150,     4240,    -6065, 
    2459,     3656,    -1566,    -2948,    -3123,    -3054,    -4433,     3834, 
    6099,      652,     4077,    -2919,    -1404,     -948,     1159,    -4049, 
    4298,     2692,    -5106,     1594,    -2555,    -1200,     3956,     5297, 
   -1058,      441,     4322,     2078,      709,     1319,    -3570,     -835, 
     683,      -64,     5782,    -2503,    -1747,    -5486,    -5919,    -5257, 
    5736,    -1646,     1212,     5728,    -4591,     5023,     5828,     3091, 
     -81,    -4320,    -1000,    -2963,    -4896,    -3051,     2366,  PADDING, 
   -2842,    -2842,    -2842,    -2842,     1022,     1022,     1022,     1022, 
   -2468,    -2468,    -2468,    -2468,     5791,     5791,     5791,     5791, 
   -1673,    -1673,    -1673,    -1673,    -5331,    -5331,    -5331,    -5331, 
   -4177,    -4177,    -4177,    -4177,     1381,     1381,     1381,     1381, 
     480,      480,      480,      480,        9,        9,        9,        9, 
     339,      339,      339,      339,      544,      544,      544,      544, 
    4278,     4278,     4278,     4278,    -4989,    -4989,    -4989,    -4989, 
   -3584,    -3584,    -3584,    -3584,    -2525,    -2525,    -2525,    -2525, 
    2166,     3915,     -113,    -4919,     -160,     3149,       -3,     4437, 
    3636,     4938,     5291,     2704,    -1426,    -4654,     1663,    -1777, 
    3364,     1689,     4057,    -3271,    -2847,    -4414,     2174,     4372, 
   -5042,    -2305,     4053,     2645,     5195,    -2780,    -4895,     1484, 
   -5241,    -4169,    -5468,    -3482,     5057,     4780,     -192,     4912, 
     677,    -6055,     1323,      -52,     1579,    -2505,     3957,      151, 
     -58,     3532,     1956,     -885,     3477,      142,    -2844,     -975, 
   -3029,     4782,    -4213,     2302,     -421,     3602,    -3600,     6077, 
   -2920,    -3127,     1010,      787,     4698,    -3445,     1321,    -2049, 
   -5874,    -3336,    -2766,     3174,     -431,     5906,    -2839,    -2127, 
    -241,    -1003,    -5009,    -6008,    -5681,    -1105,     3438,     4212, 
   -5594,     5886,      504,     -605,    -4080,     6068,     3263,    -4624, 
   -4134,     3195,     5860,    -3328,    -5777,    -4978,     1351,    -1177, 
   -4255,    -1635,    -2768,     -140,    -1853,    -4611,     -726,  PADDING, 
    -953,     -953,     -953,     -953,      827,      827,      827,      827, 
    2476,     2476,     2476,     2476,     2197,     2197,     2197,     2197, 
    3949,     3949,     3949,     3949,     4452,     4452,     4452,     4452, 
   -4354,    -4354,    -4354,    -4354,     2837,     2837,     2837,     2837, 
   -3748,    -3748,    -3748,    -3748,     5767,     5767,     5767,     5767, 
     118,      118,      118,      118,    -5067,    -5067,    -5067,    -5067, 
   -3296,    -3296,    -3296,    -3296,     2396,     2396,     2396,     2396, 
     130,      130,      130,      130,    -5374,    -5374,    -5374,    -5374, 
   -3247,    -2686,    -3978,    -2969,    -2370,     2865,     5332,     3510, 
    1630,    -2126,     5407,     3186,    -1153,    -2884,    -2249,    -4048, 
   -2399,    -3400,    -5191,    -3136,    -3000,      671,     3016,      243, 
   -5559,      420,    -2178,     1544,     3985,     4905,     3531,      476, 
   -4467,    -5537,     4449,     -147,     6118,     1190,     3860,    -4536, 
    5079,     2169,    -4324,    -4075,    -1278,     1973,    -3514,     5925, 
     654,     1702,    -5529,     3199,     6136,    -5415,     4948,      400, 
    5339,     3710,      468,      316,    -2033,     3879,    -1359,      973, 
   -4789,     4749,    -5456,    -3789,    -3818,    -2683,     5445,    -1050, 
   -3262,     -522,     4916,     5315,    -2344,    -5574,    -1041,    -1018, 
    3565,     1987,     5206,      -56,    -5862,    -3643,    -6137,    -1728, 
    5446,     6093,    -3988,     -382,    -3998,     1922,    -5435,    -1254, 
}; // 512->712

const int16_t ntt_qinv_br[] = {
 PADDING,    -3943,   -13721,    10780,  PADDING,  PADDING,  PADDING,  PADDING, 
   -3479,     9444,    -9343,   -12854,     7036,    -6999,    -2530,     6183, 
   -3119,    -2546,    -2106,    -8535,     8036,    13561,    -3535,  PADDING, 
    3359,     3359,     3359,     3359,    12350,    12350,    12350,    12350, 
    6468,     6468,     6468,     6468,     3807,     3807,     3807,     3807, 
    5367,     5367,     5367,     5367,     1943,     1943,     1943,     1943, 
    7682,     7682,     7682,     7682,   -13577,   -13577,   -13577,   -13577, 
   11700,    11700,    11700,    11700,   -15345,   -15345,   -15345,   -15345, 
     890,      890,      890,      890,     4522,     4522,     4522,     4522, 
   -8769,    -8769,    -8769,    -8769,     8641,     8641,     8641,     8641, 
    8756,     8756,     8756,     8756,    -5570,    -5570,    -5570,    -5570, 
    6402,     1178,   -13601,    -2845,     1039,     2061,   -10220,    10073, 
     943,    12961,    -7764,    15193,    13364,    -6615,     7623,    -2786, 
    2711,   -13025,     4351,   -13556,       71,    -8175,   -10033,    -3839, 
    4098,      645,    12569,   -11047,    -7140,     9876,    13382,    -1453, 
     130,    15772,    -6666,    -4220,     4031,    -4839,    14316,    -8537, 
   -7300,   -15292,    -8023,      463,    -5266,      925,    -8839,     4954, 
    8079,     6295,     7754,     9156,    10567,    16377,     5210,    -7684, 
   10641,    -7378,     6082,    -5415,    10057,    15753,    14476,   -12630, 
    3367,     3954,    -3970,   -15844,      933,    14353,    -5484,    11980, 
  -15646,     7079,     4514,     1927,   -10017,     7799,    -1135,    12676, 
   10972,    -4914,      581,    -9409,     1535,    -6524,    -5468,    -4812, 
  -10583,      415,    15668,    14220,     1114,    -1207,   -12729,     3447, 
    1925,    -6786,     9655,    -1501,    -7932,    -8015,    -7316,    12921, 
   -7324,    -8359,     9897,    12812,    -9473,    -2831,    -6116,  PADDING, 
   -9849,    -9849,    -9849,    -9849,    -4690,    -4690,    -4690,    -4690, 
    9017,     9017,     9017,     9017,    -6794,    -6794,    -6794,    -6794, 
    9697,     9697,     9697,     9697,      386,      386,      386,      386, 
   -7282,    -7282,    -7282,    -7282,   -13038,   -13038,   -13038,   -13038, 
  -13809,   -13809,   -13809,   -13809,    -9884,    -9884,    -9884,    -9884, 
    -946,     -946,     -946,     -946,   -11281,   -11281,   -11281,   -11281, 
    9223,     9223,     9223,     9223,   -14777,   -14777,   -14777,   -14777, 
  -10484,   -10484,   -10484,   -10484,   -15761,   -15761,   -15761,   -15761, 
    2671,    13361,    13566,   -11423,   -13268,    -4284,   -10079,    -2333, 
   -6498,     9721,    16057,     7964,    -6842,    -5831,   -16102,    -6458, 
   -2839,     5714,    -1077,   -12385,     3114,    14070,    -3218,     8660, 
    1314,   -10921,   -14516,     6348,   -11564,    -1159,     3674,     5098, 
     786,    15374,   -10708,   -10031,      866,    -3055,    15972,    -9940, 
    8876,     -447,    15894,    -5231,   -16324,   -13822,     3626,   -16316, 
  -10876,     2458,     5220,     2965,    10788,    -8399,    11305,   -16172, 
    6556,     9748,    -4175,    -7860,    -8327,    -8143,   -11820,    10223, 
   16262,     1738,    10871,    -7783,    -3743,    -2527,     3090,   -10796, 
   11460,     7178,   -13614,     4250,    -6812,    -3199,    10548,    14124, 
   -2821,     1175,    11524,     5540,     1890,     3517,    -9519,    -2226, 
    1821,     -170,    15417,    -6674,    -4658,   -14628,   -15782,   -14017, 
   15294,    -4388,     3231,    15273,   -12241,    13393,    15540,     8241, 
    -215,   -11519,    -2666,    -7900,   -13054,    -8135,     6308,  PADDING, 
   -7578,    -7578,    -7578,    -7578,     2725,     2725,     2725,     2725, 
   -6580,    -6580,    -6580,    -6580,    15441,    15441,    15441,    15441, 
   -4460,    -4460,    -4460,    -4460,   -14214,   -14214,   -14214,   -14214, 
  -11137,   -11137,   -11137,   -11137,     3682,     3682,     3682,     3682, 
    1279,     1279,     1279,     1279,       23,       23,       23,       23, 
     903,      903,      903,      903,     1450,     1450,     1450,     1450, 
   11407,    11407,    11407,    11407,   -13302,   -13302,   -13302,   -13302, 
   -9556,    -9556,    -9556,    -9556,    -6732,    -6732,    -6732,    -6732, 
    5775,    10439,     -301,   -13116,     -426,     8396,       -7,    11831, 
    9695,    13166,    14108,     7210,    -3802,   -12409,     4434,    -4738, 
    8969,     4503,    10817,    -8721,    -7591,   -11769,     5796,    11657, 
  -13444,    -6146,    10807,     7052,    13852,    -7412,   -13052,     3957, 
  -13974,   -11116,   -14580,    -9284,    13484,    12745,     -511,    13097, 
    1805,   -16145,     3527,     -138,     4210,    -6679,    10551,      402, 
    -154,     9417,     5215,    -2359,     9271,      378,    -7583,    -2599, 
   -8076,    12750,   -11233,     6138,    -1122,     9604,    -9599,    16204, 
   -7786,    -8337,     2693,     2098,    12526,    -9185,     3522,    -5463, 
  -15662,    -8895,    -7375,     8463,    -1149,    15748,    -7570,    -5671, 
    -642,    -2674,   -13356,   -16020,   -15148,    -2946,     9167,    11231, 
  -14916,    15694,     1343,    -1613,   -10879,    16180,     8700,   -12329, 
  -11023,     8519,    15625,    -8873,   -15404,   -13273,     3602,    -3138, 
  -11345,    -4359,    -7380,     -373,    -4940,   -12294,    -1935,  PADDING, 
   -2541,    -2541,    -2541,    -2541,     2205,     2205,     2205,     2205, 
    6602,     6602,     6602,     6602,     5858,     5858,     5858,     5858, 
   10529,    10529,    10529,    10529,    11871,    11871,    11871,    11871, 
  -11609,   -11609,   -11609,   -11609,     7564,     7564,     7564,     7564, 
   -9993,    -9993,    -9993,    -9993,    15377,    15377,    15377,    15377, 
     314,      314,      314,      314,   -13510,   -13510,   -13510,   -13510, 
   -8788,    -8788,    -8788,    -8788,     6388,     6388,     6388,     6388, 
     346,      346,      346,      346,   -14329,   -14329,   -14329,   -14329, 
   -8657,    -7162,   -10607,    -7916,    -6319,     7639,    14217,     9359, 
    4346,    -5668,    14417,     8495,    -3074,    -7690,    -5996,   -10793, 
   -6396,    -9065,   -13841,    -8361,    -7999,     1789,     8042,      647, 
  -14822,     1119,    -5807,     4116,    10625,    13078,     9415,     1269, 
  -11911,   -14764,    11863,     -391,    16313,     3173,    10292,   -12095, 
   13542,     5783,   -11529,   -10865,    -3407,     5260,    -9369,    15798, 
    1743,     4538,   -14742,     8529,    16361,   -14438,    13193,     1066, 
   14236,     9892,     1247,      842,    -5420,    10343,    -3623,     2594, 
  -12769,    12662,   -14548,   -10103,   -10180,    -7154,    14518,    -2799, 
   -8697,    -1391,    13108,    14172,    -6250,   -14862,    -2775,    -2714, 
    9505,     5298,    13881,     -149,   -15630,    -9713,   -16364,    -4607, 
   14521,    16246,   -10633,    -1018,   -10660,     5124,   -14492,    -3343, 
}; // 712
const int16_t invntt_br[] = {
    1254,     5435,    -1922,     3998,      382,     3988,    -6093,    -5446, 
    1728,     6137,     3643,     5862,       56,    -5206,    -1987,    -3565, 
    1018,     1041,     5574,     2344,    -5315,    -4916,      522,     3262, 
    1050,    -5445,     2683,     3818,     3789,     5456,    -4749,     4789, 
    -973,     1359,    -3879,     2033,     -316,     -468,    -3710,    -5339, 
    -400,    -4948,     5415,    -6136,    -3199,     5529,    -1702,     -654, 
   -5925,     3514,    -1973,     1278,     4075,     4324,    -2169,    -5079, 
    4536,    -3860,    -1190,    -6118,      147,    -4449,     5537,     4467, 
    -476,    -3531,    -4905,    -3985,    -1544,     2178,     -420,     5559, 
    -243,    -3016,     -671,     3000,     3136,     5191,     3400,     2399, 
    4048,     2249,     2884,     1153,    -3186,    -5407,     2126,    -1630, 
   -3510,    -5332,    -2865,     2370,     2969,     3978,     2686,     3247, 
    5374,     5374,     5374,     5374,     -130,     -130,     -130,     -130, 
   -2396,    -2396,    -2396,    -2396,     3296,     3296,     3296,     3296, 
    5067,     5067,     5067,     5067,     -118,     -118,     -118,     -118, 
   -5767,    -5767,    -5767,    -5767,     3748,     3748,     3748,     3748, 
   -2837,    -2837,    -2837,    -2837,     4354,     4354,     4354,     4354, 
   -4452,    -4452,    -4452,    -4452,    -3949,    -3949,    -3949,    -3949, 
   -2197,    -2197,    -2197,    -2197,    -2476,    -2476,    -2476,    -2476, 
    -827,     -827,     -827,     -827,      953,      953,      953,      953, 
     726,     4611,     1853,      140,     2768,     1635,     4255,     1177, 
   -1351,     4978,     5777,     3328,    -5860,    -3195,     4134,  PADDING, 
    4624,    -3263,    -6068,     4080,      605,     -504,    -5886,     5594, 
   -4212,    -3438,     1105,     5681,     6008,     5009,     1003,      241, 
    2127,     2839,    -5906,      431,    -3174,     2766,     3336,     5874, 
    2049,    -1321,     3445,    -4698,     -787,    -1010,     3127,     2920, 
   -6077,     3600,    -3602,      421,    -2302,     4213,    -4782,     3029, 
     975,     2844,     -142,    -3477,      885,    -1956,    -3532,       58, 
    -151,    -3957,     2505,    -1579,       52,    -1323,     6055,     -677, 
   -4912,      192,    -4780,    -5057,     3482,     5468,     4169,     5241, 
   -1484,     4895,     2780,    -5195,    -2645,    -4053,     2305,     5042, 
   -4372,    -2174,     4414,     2847,     3271,    -4057,    -1689,    -3364, 
    1777,    -1663,     4654,     1426,    -2704,    -5291,    -4938,    -3636, 
   -4437,        3,    -3149,      160,     4919,      113,    -3915,    -2166, 
    2525,     2525,     2525,     2525,     3584,     3584,     3584,     3584, 
    4989,     4989,     4989,     4989,    -4278,    -4278,    -4278,    -4278, 
    -544,     -544,     -544,     -544,     -339,     -339,     -339,     -339, 
      -9,       -9,       -9,       -9,     -480,     -480,     -480,     -480, 
   -1381,    -1381,    -1381,    -1381,     4177,     4177,     4177,     4177, 
    5331,     5331,     5331,     5331,     1673,     1673,     1673,     1673, 
   -5791,    -5791,    -5791,    -5791,     2468,     2468,     2468,     2468, 
   -1022,    -1022,    -1022,    -1022,     2842,     2842,     2842,     2842, 
   -2366,     3051,     4896,     2963,     1000,     4320,       81,    -3091, 
   -5828,    -5023,     4591,    -5728,    -1212,     1646,    -5736,  PADDING, 
    5257,     5919,     5486,     1747,     2503,    -5782,       64,     -683, 
     835,     3570,    -1319,     -709,    -2078,    -4322,     -441,     1058, 
   -5297,    -3956,     1200,     2555,    -1594,     5106,    -2692,    -4298, 
    4049,    -1159,      948,     1404,     2919,    -4077,     -652,    -6099, 
   -3834,     4433,     3054,     3123,     2948,     1566,    -3656,    -2459, 
    6065,    -4240,     3150,    -4046,    -1112,    -1958,     -922,     4079, 
    6119,    -1360,     5184,     6122,     1962,    -5961,      168,    -3329, 
    3728,    -5990,     1146,     -325,     3762,     4016,    -5766,     -295, 
   -1912,    -1378,      435,     4337,    -2381,     5444,     4096,     -493, 
   -3248,     1207,    -5277,    -1168,     4645,      404,    -2143,     1065, 
    2422,     6039,     2187,     2566,    -2987,    -6022,    -3646,     2437, 
     875,     3780,     1607,     4976,     4284,    -5088,    -5011,    -1002, 
    5911,     5911,     5911,     5911,     3932,     3932,     3932,     3932, 
    5542,     5542,     5542,     5542,    -3459,    -3459,    -3459,    -3459, 
    4231,     4231,     4231,     4231,      355,      355,      355,      355, 
    3707,     3707,     3707,     3707,     5179,     5179,     5179,     5179, 
    4890,     4890,     4890,     4890,     2731,     2731,     2731,     2731, 
    -145,     -145,     -145,     -145,    -3637,    -3637,    -3637,    -3637, 
    2548,     2548,     2548,     2548,    -3382,    -3382,    -3382,    -3382, 
    1759,     1759,     1759,     1759,     3694,     3694,     3694,     3694, 
    2294,     1062,     3553,    -4805,    -3712,     3135,     2747,    -4846, 
    2744,     3006,     2975,      563,    -3621,     2545,     -722,  PADDING, 
   -1293,     4774,      453,     -418,    -5333,    -5876,     -156,     3969, 
    1805,     2051,     2447,     -576,     3529,     -218,     1843,    -4115, 
   -4754,      426,    -2925,     3757,     -723,    -1693,    -2655,     5868, 
   -4493,     2057,    -5383,     -350,     5942,     1489,    -1483,    -1263, 
    4737,    -5429,    -5908,    -3772,     2031,    -2281,     2767,    -3991, 
    2882,    -1954,    -6142,    -3963,    -3434,    -2908,    -2361,    -3030, 
   -1858,     3315,     -347,     1975,     -174,     3009,     5735,     2738, 
    3202,    -5369,     1815,    -1512,     1583,     2500,    -5915,      -49, 
     545,    -5019,    -3704,     2678,     4143,    -4714,     -242,    -1537, 
    1440,     3763,     3066,      -27,     5084,    -1632,     4885,    -1017, 
    1045,    -2859,     2481,    -5012,    -5698,     2912,    -4861,     -354, 
   -3778,     3833,     -773,     -390,     1067,     5101,     -442,    -2401, 
    2089,     2089,     2089,     2089,    -3284,    -3284,    -3284,    -3284, 
   -3241,    -3241,    -3241,    -3241,     3289,     3289,     3289,     3289, 
   -1696,    -1696,    -1696,    -1696,     -334,     -334,     -334,     -334, 
    5755,     5755,     5755,     5755,    -4388,    -4388,    -4388,    -4388, 
    5092,     5092,     5092,     5092,    -2881,    -2881,    -2881,    -2881, 
    -729,     -729,     -729,     -729,    -2013,    -2013,    -2013,    -2013, 
   -1428,    -1428,    -1428,    -1428,    -2426,    -2426,    -2426,    -2426, 
   -4632,    -4632,    -4632,    -4632,    -1260,    -1260,    -1260,    -1260, 
    1326,    -5086,    -3014,     3201,      790,      955,     1170,    -2319, 
     949,     2625,    -2639,     4821,     3504,    -3542,     1305,  PADDING, 
   -4043,     5146,     1371,    12265,     1479,  PADDING,  PADDING,  PADDING, 
}; // 712

const int16_t invntt_qinv_br[] = {
    3343,    14492,    -5124,    10660,     1018,    10633,   -16246,   -14521, 
    4607,    16364,     9713,    15630,      149,   -13881,    -5298,    -9505, 
    2714,     2775,    14862,     6250,   -14172,   -13108,     1391,     8697, 
    2799,   -14518,     7154,    10180,    10103,    14548,   -12662,    12769, 
   -2594,     3623,   -10343,     5420,     -842,    -1247,    -9892,   -14236, 
   -1066,   -13193,    14438,   -16361,    -8529,    14742,    -4538,    -1743, 
  -15798,     9369,    -5260,     3407,    10865,    11529,    -5783,   -13542, 
   12095,   -10292,    -3173,   -16313,      391,   -11863,    14764,    11911, 
   -1269,    -9415,   -13078,   -10625,    -4116,     5807,    -1119,    14822, 
    -647,    -8042,    -1789,     7999,     8361,    13841,     9065,     6396, 
   10793,     5996,     7690,     3074,    -8495,   -14417,     5668,    -4346, 
   -9359,   -14217,    -7639,     6319,     7916,    10607,     7162,     8657, 
   14329,    14329,    14329,    14329,     -346,     -346,     -346,     -346, 
   -6388,    -6388,    -6388,    -6388,     8788,     8788,     8788,     8788, 
   13510,    13510,    13510,    13510,     -314,     -314,     -314,     -314, 
  -15377,   -15377,   -15377,   -15377,     9993,     9993,     9993,     9993, 
   -7564,    -7564,    -7564,    -7564,    11609,    11609,    11609,    11609, 
  -11871,   -11871,   -11871,   -11871,   -10529,   -10529,   -10529,   -10529, 
   -5858,    -5858,    -5858,    -5858,    -6602,    -6602,    -6602,    -6602, 
   -2205,    -2205,    -2205,    -2205,     2541,     2541,     2541,     2541, 
    1935,    12294,     4940,      373,     7380,     4359,    11345,     3138, 
   -3602,    13273,    15404,     8873,   -15625,    -8519,    11023,  PADDING, 
   12329,    -8700,   -16180,    10879,     1613,    -1343,   -15694,    14916, 
  -11231,    -9167,     2946,    15148,    16020,    13356,     2674,      642, 
    5671,     7570,   -15748,     1149,    -8463,     7375,     8895,    15662, 
    5463,    -3522,     9185,   -12526,    -2098,    -2693,     8337,     7786, 
  -16204,     9599,    -9604,     1122,    -6138,    11233,   -12750,     8076, 
    2599,     7583,     -378,    -9271,     2359,    -5215,    -9417,      154, 
    -402,   -10551,     6679,    -4210,      138,    -3527,    16145,    -1805, 
  -13097,      511,   -12745,   -13484,     9284,    14580,    11116,    13974, 
   -3957,    13052,     7412,   -13852,    -7052,   -10807,     6146,    13444, 
  -11657,    -5796,    11769,     7591,     8721,   -10817,    -4503,    -8969, 
    4738,    -4434,    12409,     3802,    -7210,   -14108,   -13166,    -9695, 
  -11831,        7,    -8396,      426,    13116,      301,   -10439,    -5775, 
    6732,     6732,     6732,     6732,     9556,     9556,     9556,     9556, 
   13302,    13302,    13302,    13302,   -11407,   -11407,   -11407,   -11407, 
   -1450,    -1450,    -1450,    -1450,     -903,     -903,     -903,     -903, 
     -23,      -23,      -23,      -23,    -1279,    -1279,    -1279,    -1279, 
   -3682,    -3682,    -3682,    -3682,    11137,    11137,    11137,    11137, 
   14214,    14214,    14214,    14214,     4460,     4460,     4460,     4460, 
  -15441,   -15441,   -15441,   -15441,     6580,     6580,     6580,     6580, 
   -2725,    -2725,    -2725,    -2725,     7578,     7578,     7578,     7578, 
   -6308,     8135,    13054,     7900,     2666,    11519,      215,    -8241, 
  -15540,   -13393,    12241,   -15273,    -3231,     4388,   -15294,  PADDING, 
   14017,    15782,    14628,     4658,     6674,   -15417,      170,    -1821, 
    2226,     9519,    -3517,    -1890,    -5540,   -11524,    -1175,     2821, 
  -14124,   -10548,     3199,     6812,    -4250,    13614,    -7178,   -11460, 
   10796,    -3090,     2527,     3743,     7783,   -10871,    -1738,   -16262, 
  -10223,    11820,     8143,     8327,     7860,     4175,    -9748,    -6556, 
   16172,   -11305,     8399,   -10788,    -2965,    -5220,    -2458,    10876, 
   16316,    -3626,    13822,    16324,     5231,   -15894,      447,    -8876, 
    9940,   -15972,     3055,     -866,    10031,    10708,   -15374,     -786, 
   -5098,    -3674,     1159,    11564,    -6348,    14516,    10921,    -1314, 
   -8660,     3218,   -14070,    -3114,    12385,     1077,    -5714,     2839, 
    6458,    16102,     5831,     6842,    -7964,   -16057,    -9721,     6498, 
    2333,    10079,     4284,    13268,    11423,   -13566,   -13361,    -2671, 
   15761,    15761,    15761,    15761,    10484,    10484,    10484,    10484, 
   14777,    14777,    14777,    14777,    -9223,    -9223,    -9223,    -9223, 
   11281,    11281,    11281,    11281,      946,      946,      946,      946, 
    9884,     9884,     9884,     9884,    13809,    13809,    13809,    13809, 
   13038,    13038,    13038,    13038,     7282,     7282,     7282,     7282, 
    -386,     -386,     -386,     -386,    -9697,    -9697,    -9697,    -9697, 
    6794,     6794,     6794,     6794,    -9017,    -9017,    -9017,    -9017, 
    4690,     4690,     4690,     4690,     9849,     9849,     9849,     9849, 
    6116,     2831,     9473,   -12812,    -9897,     8359,     7324,   -12921, 
    7316,     8015,     7932,     1501,    -9655,     6786,    -1925,  PADDING, 
   -3447,    12729,     1207,    -1114,   -14220,   -15668,     -415,    10583, 
    4812,     5468,     6524,    -1535,     9409,     -581,     4914,   -10972, 
  -12676,     1135,    -7799,    10017,    -1927,    -4514,    -7079,    15646, 
  -11980,     5484,   -14353,     -933,    15844,     3970,    -3954,    -3367, 
   12630,   -14476,   -15753,   -10057,     5415,    -6082,     7378,   -10641, 
    7684,    -5210,   -16377,   -10567,    -9156,    -7754,    -6295,    -8079, 
   -4954,     8839,     -925,     5266,     -463,     8023,    15292,     7300, 
    8537,   -14316,     4839,    -4031,     4220,     6666,   -15772,     -130, 
    1453,   -13382,    -9876,     7140,    11047,   -12569,     -645,    -4098, 
    3839,    10033,     8175,      -71,    13556,    -4351,    13025,    -2711, 
    2786,    -7623,     6615,   -13364,   -15193,     7764,   -12961,     -943, 
  -10073,    10220,    -2061,    -1039,     2845,    13601,    -1178,    -6402, 
    5570,     5570,     5570,     5570,    -8756,    -8756,    -8756,    -8756, 
   -8641,    -8641,    -8641,    -8641,     8769,     8769,     8769,     8769, 
   -4522,    -4522,    -4522,    -4522,     -890,     -890,     -890,     -890, 
   15345,    15345,    15345,    15345,   -11700,   -11700,   -11700,   -11700, 
   13577,    13577,    13577,    13577,    -7682,    -7682,    -7682,    -7682, 
   -1943,    -1943,    -1943,    -1943,    -5367,    -5367,    -5367,    -5367, 
   -3807,    -3807,    -3807,    -3807,    -6468,    -6468,    -6468,    -6468, 
  -12350,   -12350,   -12350,   -12350,    -3359,    -3359,    -3359,    -3359, 
    3535,   -13561,    -8036,     8535,     2106,     2546,     3119,    -6183, 
    2530,     6999,    -7036,    12854,     9343,    -9444,     3479,  PADDING, 
  -10780,    13721,     3655,    32704,     3943,  PADDING,  PADDING,  PADDING, 
}; // 712
