#include <Arduino.h>

const PROGMEM uint16_t exptable[512] {
0,
2,
3,
5,
6,
8,
10,
11,
13,
15,
17,
18,
20,
22,
24,
26,
28,
30,
32,
33,
35,
37,
40,
42,
44,
46,
48,
50,
52,
54,
57,
59,
61,
64,
66,
68,
71,
73,
76,
78,
81,
83,
86,
89,
91,
94,
97,
99,
102,
105,
108,
111,
114,
117,
120,
123,
126,
129,
132,
135,
139,
142,
145,
148,
152,
155,
159,
162,
166,
170,
173,
177,
181,
184,
188,
192,
196,
200,
204,
208,
212,
217,
221,
225,
229,
234,
238,
243,
247,
252,
257,
261,
266,
271,
276,
281,
286,
291,
296,
301,
307,
312,
317,
323,
328,
334,
340,
345,
351,
357,
363,
369,
375,
381,
388,
394,
400,
407,
413,
420,
427,
434,
440,
447,
455,
462,
469,
476,
484,
491,
499,
506,
514,
522,
530,
538,
546,
555,
563,
572,
580,
589,
598,
607,
616,
625,
634,
643,
653,
662,
672,
682,
692,
702,
712,
723,
733,
744,
754,
765,
776,
787,
798,
810,
821,
833,
845,
857,
869,
881,
894,
906,
919,
932,
945,
958,
971,
985,
998,
1012,
1026,
1040,
1055,
1069,
1084,
1099,
1114,
1129,
1145,
1160,
1176,
1192,
1208,
1225,
1241,
1258,
1275,
1293,
1310,
1328,
1346,
1364,
1382,
1401,
1419,
1438,
1458,
1477,
1497,
1517,
1537,
1557,
1578,
1599,
1620,
1642,
1663,
1685,
1708,
1730,
1753,
1776,
1800,
1823,
1847,
1872,
1896,
1921,
1946,
1972,
1997,
2023,
2050,
2077,
2104,
2131,
2159,
2187,
2215,
2244,
2273,
2303,
2333,
2363,
2394,
2425,
2456,
2488,
2520,
2552,
2585,
2619,
2652,
2686,
2721,
2756,
2791,
2827,
2864,
2900,
2938,
2975,
3013,
3052,
3091,
3131,
3171,
3211,
3252,
3294,
3336,
3378,
3421,
3465,
3509,
3554,
3599,
3645,
3691,
3738,
3785,
3833,
3882,
3931,
3981,
4032,
4083,
4135,
4187,
4240,
4294,
4348,
4403,
4459,
4515,
4572,
4630,
4688,
4747,
4807,
4868,
4929,
4991,
5054,
5118,
5182,
5247,
5313,
5380,
5448,
5516,
5586,
5656,
5727,
5799,
5872,
5945,
6020,
6096,
6172,
6249,
6328,
6407,
6487,
6569,
6651,
6734,
6818,
6904,
6990,
7077,
7166,
7256,
7346,
7438,
7531,
7625,
7720,
7817,
7914,
8013,
8113,
8214,
8317,
8421,
8526,
8632,
8739,
8848,
8959,
9070,
9183,
9298,
9413,
9531,
9649,
9769,
9891,
10014,
10139,
10265,
10392,
10522,
10652,
10785,
10919,
11054,
11192,
11331,
11472,
11614,
11758,
11904,
12052,
12202,
12353,
12506,
12662,
12819,
12978,
13139,
13302,
13467,
13634,
13803,
13974,
14147,
14322,
14500,
14680,
14861,
15046,
15232,
15421,
15612,
15805,
16001,
16199,
16399,
16602,
16808,
17016,
17226,
17440,
17655,
17874,
18095,
18319,
18545,
18775,
19007,
19242,
19480,
19721,
19964,
20211,
20461,
20714,
20970,
21229,
21491,
21757,
22026,
22298,
22573,
22852,
23134,
23420,
23709,
24002,
24299,
24599,
24902,
25210,
25521,
25836,
26155,
26478,
26805,
27136,
27470,
27809,
28153,
28500,
28852,
29207,
29568,
29933,
30302,
30676,
31054,
31437,
31825,
32217,
32614,
33017,
33424,
33836,
34253,
34675,
35103,
35536,
35974,
36417,
36866,
37320,
37780,
38246,
38717,
39194,
39677,
40166,
40661,
41162,
41669,
42183,
42703,
43229,
43761,
44300,
44846,
45398,
45958,
46524,
47097,
47677,
48264,
48858,
49460,
50069,
50686,
51310,
51942,
52581,
53229,
53884,
54547,
55219,
55899,
56587,
57284,
57989,
58703,
59425,
60157,
60897,
61647,
62406,
63174,
63951,
64738,
65535,
};