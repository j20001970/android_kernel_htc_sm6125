// SPDX-License-Identifier: GPL-2.0
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/math64.h>
#include <linux/log2.h>
#include <linux/err.h>
#include <linux/module.h>

#include "qcom-vadc-common.h"

/* Voltage to temperature */
static const struct vadc_map_pt adcmap_100k_104ef_104fb[] = {
	{1758,	-40},
	{1742,	-35},
	{1719,	-30},
	{1691,	-25},
	{1654,	-20},
	{1608,	-15},
	{1551,	-10},
	{1483,	-5},
	{1404,	0},
	{1315,	5},
	{1218,	10},
	{1114,	15},
	{1007,	20},
	{900,	25},
	{795,	30},
	{696,	35},
	{605,	40},
	{522,	45},
	{448,	50},
	{383,	55},
	{327,	60},
	{278,	65},
	{237,	70},
	{202,	75},
	{172,	80},
	{146,	85},
	{125,	90},
	{107,	95},
	{92,	100},
	{79,	105},
	{68,	110},
	{59,	115},
	{51,	120},
	{44,	125}
};

/*
 * Voltage to temperature table for 100k pull up for NTCG104EF104 with
 * 1.875V reference.
 */
static const struct vadc_map_pt adcmap_100k_104ef_104fb_1875_vref[] = {
	{ 1831000,	-40000 },
	{ 1814000,	-35000 },
	{ 1791000,	-30000 },
	{ 1761000,	-25000 },
	{ 1723000,	-20000 },
	{ 1675000,	-15000 },
	{ 1616000,	-10000 },
	{ 1545000,	-5000 },
	{ 1463000,	0 },
	{ 1370000,	5000 },
	{ 1268000,	10000 },
	{ 1160000,	15000 },
	{ 1049000,	20000 },
	{ 937000,	25000 },
	{ 828000,	30000 },
	{ 726000,	35000 },
	{ 630000,	40000 },
	{ 544000,	45000 },
	{ 467000,	50000 },
	{ 399000,	55000 },
	{ 340000,	60000 },
	{ 290000,	65000 },
	{ 247000,	70000 },
	{ 209000,	75000 },
	{ 179000,	80000 },
	{ 153000,	85000 },
	{ 130000,	90000 },
	{ 112000,	95000 },
	{ 96000,	100000 },
	{ 82000,	105000 },
	{ 71000,	110000 },
	{ 62000,	115000 },
	{ 53000,	120000 },
	{ 46000,	125000 },
};

/*
 * Voltage to temperature table for 100k pull up for bat_therm with
 * Alium.
 */
static const struct vadc_map_pt adcmap_batt_therm_100k[] = {
#if 1 //100k 3435b
	{1802,	-400},
	{1793,	-380},
	{1783,	-360},
	{1772,	-340},
	{1759,	-320},
	{1746,	-300},
	{1732,	-280},
	{1716,	-260},
	{1699,	-240},
	{1680,	-220},
	{1661,	-200},
	{1640,	-180},
	{1617,	-160},
	{1594,	-140},
	{1568,	-120},
	{1542,	-100},
	{1514,	-80},
	{1485,	-60},
	{1455,	-40},
	{1423,	-20},
	{1391,	0},
	{1357,	20},
	{1323,	40},
	{1287,	60},
	{1251,	80},
	{1215,	100},
	{1178,	120},
	{1141,	140},
	{1104,	160},
	{1067,	180},
	{1029,	200},
	{992,	220},
	{956,	240},
	{919,	260},
	{884,	280},
	{849,	300},
	{814,	320},
	{781,	340},
	{748,	360},
	{716,	380},
	{685,	400},
	{655,	420},
	{626,	440},
	{598,	460},
	{571,	480},
	{545,	500},
	{520,	520},
	{496,	540},
	{474,	560},
	{452,	580},
	{431,	600},
	{410,	620},
	{391,	640},
	{373,	660},
	{356,	680},
	{339,	700},
	{323,	720},
	{308,	740},
	{294,	760},
	{280,	780},
	{267,	800},
	{255,	820},
	{243,	840},
	{232,	860},
	{222,	880},
	{212,	900},
	{202,	920},
	{193,	940},
	{184,	960},
	{176,	980}
#else
	{1840,	-400},
	{1835,	-380},
	{1828,	-360},
	{1821,	-340},
	{1813,	-320},
	{1803,	-300},
	{1793,	-280},
	{1781,	-260},
	{1768,	-240},
	{1753,	-220},
	{1737,	-200},
	{1719,	-180},
	{1700,	-160},
	{1679,	-140},
	{1655,	-120},
	{1630,	-100},
	{1603,	-80},
	{1574,	-60},
	{1543,	-40},
	{1510,	-20},
	{1475,	0},
	{1438,	20},
	{1400,	40},
	{1360,	60},
	{1318,	80},
	{1276,	100},
	{1232,	120},
	{1187,	140},
	{1142,	160},
	{1097,	180},
	{1051,	200},
	{1005,	220},
	{960,	240},
	{915,	260},
	{871,	280},
	{828,	300},
	{786,	320},
	{745,	340},
	{705,	360},
	{666,	380},
	{629,	400},
	{594,	420},
	{560,	440},
	{527,	460},
	{497,	480},
	{467,	500},
	{439,	520},
	{413,	540},
	{388,	560},
	{365,	580},
	{343,	600},
	{322,	620},
	{302,	640},
	{284,	660},
	{267,	680},
	{251,	700},
	{235,	720},
	{221,	740},
	{208,	760},
	{195,	780},
	{184,	800},
	{173,	820},
	{163,	840},
	{153,	860},
	{144,	880},
	{136,	900},
	{128,	920},
	{120,	940},
	{114,	960},
	{107,	980}

#endif
};

/*
 * Voltage to temperature table for 100k pull up for bat_therm with
 * MLP356477.
 */
static const struct vadc_map_pt adcmap_batt_therm_100k_6125[] = {
#if 1
	{1802,	-400},
	{1793,	-380},
	{1783,	-360},
	{1772,	-340},
	{1759,	-320},
	{1746,	-300},
	{1732,	-280},
	{1716,	-260},
	{1699,	-240},
	{1680,	-220},
	{1661,	-200},
	{1640,	-180},
	{1617,	-160},
	{1594,	-140},
	{1568,	-120},
	{1542,	-100},
	{1514,	-80},
	{1485,	-60},
	{1455,	-40},
	{1423,	-20},
	{1391,	0},
	{1357,	20},
	{1323,	40},
	{1287,	60},
	{1251,	80},
	{1215,	100},
	{1178,	120},
	{1141,	140},
	{1104,	160},
	{1067,	180},
	{1029,	200},
	{992,	220},
	{956,	240},
	{919,	260},
	{884,	280},
	{849,	300},
	{814,	320},
	{781,	340},
	{748,	360},
	{716,	380},
	{685,	400},
	{655,	420},
	{626,	440},
	{598,	460},
	{571,	480},
	{545,	500},
	{520,	520},
	{496,	540},
	{474,	560},
	{452,	580},
	{431,	600},
	{410,	620},
	{391,	640},
	{373,	660},
	{356,	680},
	{339,	700},
	{323,	720},
	{308,	740},
	{294,	760},
	{280,	780},
	{267,	800},
	{255,	820},
	{243,	840},
	{232,	860},
	{222,	880},
	{212,	900},
	{202,	920},
	{193,	940},
	{184,	960},
	{176,	980}
#else
	{1770,	-400},
	{1757,	-380},
	{1743,	-360},
	{1727,	-340},
	{1710,	-320},
	{1691,	-300},
	{1671,	-280},
	{1650,	-260},
	{1627,	-240},
	{1602,	-220},
	{1576,	-200},
	{1548,	-180},
	{1519,	-160},
	{1488,	-140},
	{1456,	-120},
	{1423,	-100},
	{1388,	-80},
	{1353,	-60},
	{1316,	-40},
	{1278,	-20},
	{1240,	0},
	{1201,	20},
	{1162,	40},
	{1122,	60},
	{1082,	80},
	{1042,	100},
	{1003,	120},
	{964,	140},
	{925,	160},
	{887,	180},
	{849,	200},
	{812,	220},
	{777,	240},
	{742,	260},
	{708,	280},
	{675,	300},
	{643,	320},
	{613,	340},
	{583,	360},
	{555,	380},
	{528,	400},
	{502,	420},
	{477,	440},
	{453,	460},
	{430,	480},
	{409,	500},
	{388,	520},
	{369,	540},
	{350,	560},
	{333,	580},
	{316,	600},
	{300,	620},
	{285,	640},
	{271,	660},
	{257,	680},
	{245,	700},
	{233,	720},
	{221,	740},
	{210,	760},
	{200,	780},
	{190,	800},
	{181,	820},
	{173,	840},
	{164,	860},
	{157,	880},
	{149,	900},
	{142,	920},
	{136,	940},
	{129,	960},
	{124,	980},
#endif
};

/*
 * Voltage to temperature table for 30k pull up for bat_therm with
 * Alium.
 */
static const struct vadc_map_pt adcmap_batt_therm_30k[] = {
#if 1 //pu30k 3380beta
	{1663,	-400},
	{1639,	-380},
	{1613,	-360},
	{1585,	-340},
	{1555,	-320},
	{1523,	-300},
	{1490,	-280},
	{1454,	-260},
	{1418,	-240},
	{1379,	-220},
	{1339,	-200},
	{1298,	-180},
	{1257,	-160},
	{1214,	-140},
	{1170,	-120},
	{1127,	-100},
	{1083,	-80},
	{1039,	-60},
	{995,	-40},
	{952,	-20},
	{909,	0},
	{867,	20},
	{826,	40},
	{786,	60},
	{747,	80},
	{709,	100},
	{672,	120},
	{637,	140},
	{603,	160},
	{571,	180},
	{540,	200},
	{510,	220},
	{482,	240},
	{456,	260},
	{430,	280},
	{406,	300},
	{383,	320},
	{362,	340},
	{341,	360},
	{322,	380},
	{304,	400},
	{287,	420},
	{271,	440},
	{256,	460},
	{242,	480},
	{228,	500},
	{216,	520},
	{204,	540},
	{193,	560},
	{182,	580},
	{172,	600},
	{163,	620},
	{155,	640},
	{146,	660},
	{139,	680},
	{131,	700},
	{125,	720},
	{118,	740},
	{112,	760},
	{107,	780},
	{101,	800},
	{96,	820},
	{91,	840},
	{87,	860},
	{83,	880},
	{79,	900},
	{75,	920},
	{71,	940},
	{68,	960},
	{65,	980}
#else //pu30k 3435beta
	{1673,	-400},
	{1649,	-380},
	{1623,	-360},
	{1596,	-340},
	{1566,	-320},
	{1535,	-300},
	{1502,	-280},
	{1467,	-260},
	{1430,	-240},
	{1392,	-220},
	{1352,	-200},
	{1311,	-180},
	{1269,	-160},
	{1226,	-140},
	{1182,	-120},
	{1138,	-100},
	{1093,	-80},
	{1049,	-60},
	{1004,	-40},
	{960,	-20},
	{917,	0},
	{874,	20},
	{832,	40},
	{791,	60},
	{752,	80},
	{713,	100},
	{676,	120},
	{640,	140},
	{606,	160},
	{573,	180},
	{541,	200},
	{511,	220},
	{483,	240},
	{455,	260},
	{430,	280},
	{405,	300},
	{382,	320},
	{360,	340},
	{340,	360},
	{320,	380},
	{302,	400},
	{285,	420},
	{269,	440},
	{253,	460},
	{239,	480},
	{225,	500},
	{213,	520},
	{201,	540},
	{190,	560},
	{179,	580},
	{169,	600},
	{160,	620},
	{152,	640},
	{143,	660},
	{136,	680},
	{128,	700},
	{122,	720},
	{115,	740},
	{109,	760},
	{104,	780},
	{98,	800},
	{93,	820},
	{89,	840},
	{84,	860},
	{80,	880},
	{76,	900},
	{73,	920},
	{69,	940},
	{66,	960},
	{63,	980}
#endif
};

/*
 * Voltage to temperature table for 30k pull up for bat_therm with
 * MLP356477.
 */
static const struct vadc_map_pt adcmap_batt_therm_30k_6125[] = {
#if 1 //pu30k 3380beta
	{1663,	-400},
	{1639,	-380},
	{1613,	-360},
	{1585,	-340},
	{1555,	-320},
	{1523,	-300},
	{1490,	-280},
	{1454,	-260},
	{1418,	-240},
	{1379,	-220},
	{1339,	-200},
	{1298,	-180},
	{1257,	-160},
	{1214,	-140},
	{1170,	-120},
	{1127,	-100},
	{1083,	-80},
	{1039,	-60},
	{995,	-40},
	{952,	-20},
	{909,	0},
	{867,	20},
	{826,	40},
	{786,	60},
	{747,	80},
	{709,	100},
	{672,	120},
	{637,	140},
	{603,	160},
	{571,	180},
	{540,	200},
	{510,	220},
	{482,	240},
	{456,	260},
	{430,	280},
	{406,	300},
	{383,	320},
	{362,	340},
	{341,	360},
	{322,	380},
	{304,	400},
	{287,	420},
	{271,	440},
	{256,	460},
	{242,	480},
	{228,	500},
	{216,	520},
	{204,	540},
	{193,	560},
	{182,	580},
	{172,	600},
	{163,	620},
	{155,	640},
	{146,	660},
	{139,	680},
	{131,	700},
	{125,	720},
	{118,	740},
	{112,	760},
	{107,	780},
	{101,	800},
	{96,	820},
	{91,	840},
	{87,	860},
	{83,	880},
	{79,	900},
	{75,	920},
	{71,	940},
	{68,	960},
	{65,	980}
#else //pu30k 3435beta
	{1673,	-400},
	{1649,	-380},
	{1623,	-360},
	{1596,	-340},
	{1566,	-320},
	{1535,	-300},
	{1502,	-280},
	{1467,	-260},
	{1430,	-240},
	{1392,	-220},
	{1352,	-200},
	{1311,	-180},
	{1269,	-160},
	{1226,	-140},
	{1182,	-120},
	{1138,	-100},
	{1093,	-80},
	{1049,	-60},
	{1004,	-40},
	{960,	-20},
	{917,	0},
	{874,	20},
	{832,	40},
	{791,	60},
	{752,	80},
	{713,	100},
	{676,	120},
	{640,	140},
	{606,	160},
	{573,	180},
	{541,	200},
	{511,	220},
	{483,	240},
	{455,	260},
	{430,	280},
	{405,	300},
	{382,	320},
	{360,	340},
	{340,	360},
	{320,	380},
	{302,	400},
	{285,	420},
	{269,	440},
	{253,	460},
	{239,	480},
	{225,	500},
	{213,	520},
	{201,	540},
	{190,	560},
	{179,	580},
	{169,	600},
	{160,	620},
	{152,	640},
	{143,	660},
	{136,	680},
	{128,	700},
	{122,	720},
	{115,	740},
	{109,	760},
	{104,	780},
	{98,	800},
	{93,	820},
	{89,	840},
	{84,	860},
	{80,	880},
	{76,	900},
	{73,	920},
	{69,	940},
	{66,	960},
	{63,	980}
#endif
};

/*
 * Voltage to temperature table for 400k pull up for bat_therm with
 * Alium.
 */
static const struct vadc_map_pt adcmap_batt_therm_400k[] = {
	{1744,	-400},
	{1724,	-380},
	{1701,	-360},
	{1676,	-340},
	{1648,	-320},
	{1618,	-300},
	{1584,	-280},
	{1548,	-260},
	{1509,	-240},
	{1468,	-220},
	{1423,	-200},
	{1377,	-180},
	{1328,	-160},
	{1277,	-140},
	{1225,	-120},
	{1171,	-100},
	{1117,	-80},
	{1062,	-60},
	{1007,	-40},
	{953,	-20},
	{899,	0},
	{847,	20},
	{795,	40},
	{745,	60},
	{697,	80},
	{651,	100},
	{607,	120},
	{565,	140},
	{526,	160},
	{488,	180},
	{453,	200},
	{420,	220},
	{390,	240},
	{361,	260},
	{334,	280},
	{309,	300},
	{286,	320},
	{265,	340},
	{245,	360},
	{227,	380},
	{210,	400},
	{195,	420},
	{180,	440},
	{167,	460},
	{155,	480},
	{144,	500},
	{133,	520},
	{124,	540},
	{115,	560},
	{107,	580},
	{99,	600},
	{92,	620},
	{86,	640},
	{80,	660},
	{75,	680},
	{70,	700},
	{65,	720},
	{61,	740},
	{57,	760},
	{53,	780},
	{50,	800},
	{46,	820},
	{43,	840},
	{41,	860},
	{38,	880},
	{36,	900},
	{34,	920},
	{32,	940},
	{30,	960},
	{28,	980}
};

/*
 * Voltage to temperature table for 400k pull up for bat_therm with
 * MLP356477.
 */
static const struct vadc_map_pt adcmap_batt_therm_400k_6125[] = {
	{1516,	-400},
	{1478,	-380},
	{1438,	-360},
	{1396,	-340},
	{1353,	-320},
	{1307,	-300},
	{1261,	-280},
	{1213,	-260},
	{1164,	-240},
	{1115,	-220},
	{1066,	-200},
	{1017,	-180},
	{968,	-160},
	{920,	-140},
	{872,	-120},
	{826,	-100},
	{781,	-80},
	{737,	-60},
	{694,	-40},
	{654,	-20},
	{615,	0},
	{578,	20},
	{542,	40},
	{509,	60},
	{477,	80},
	{447,	100},
	{419,	120},
	{392,	140},
	{367,	160},
	{343,	180},
	{321,	200},
	{301,	220},
	{282,	240},
	{264,	260},
	{247,	280},
	{231,	300},
	{216,	320},
	{203,	340},
	{190,	360},
	{178,	380},
	{167,	400},
	{157,	420},
	{147,	440},
	{138,	460},
	{130,	480},
	{122,	500},
	{115,	520},
	{108,	540},
	{102,	560},
	{96,	580},
	{90,	600},
	{85,	620},
	{80,	640},
	{76,	660},
	{72,	680},
	{68,	700},
	{64,	720},
	{61,	740},
	{57,	760},
	{54,	780},
	{52,	800},
	{49,	820},
	{46,	840},
	{44,	860},
	{42,	880},
	{40,	900},
	{38,	920},
	{36,	940},
	{34,	960},
	{32,	980},
};

struct lut_table {
	const struct vadc_map_pt *table;
	u32 tablesize;
};

static const struct lut_table lut_table_30[] = {
	{adcmap_batt_therm_30k,	ARRAY_SIZE(adcmap_batt_therm_30k)},
	{adcmap_batt_therm_30k_6125, ARRAY_SIZE(adcmap_batt_therm_30k_6125)},
};

static const struct lut_table lut_table_100[] = {
	{adcmap_batt_therm_100k, ARRAY_SIZE(adcmap_batt_therm_100k)},
	{adcmap_batt_therm_100k_6125, ARRAY_SIZE(adcmap_batt_therm_100k_6125)},
};

static const struct lut_table lut_table_400[] = {
	{adcmap_batt_therm_400k, ARRAY_SIZE(adcmap_batt_therm_400k)},
	{adcmap_batt_therm_400k_6125, ARRAY_SIZE(adcmap_batt_therm_400k_6125)},
};

static int qcom_vadc_map_voltage_temp(const struct vadc_map_pt *pts,
				      u32 tablesize, s32 input, s64 *output)
{
	bool descending = 1;
	u32 i = 0;

	if (!pts)
		return -EINVAL;

	/* Check if table is descending or ascending */
	if (tablesize > 1) {
		if (pts[0].x < pts[1].x)
			descending = 0;
	}

	while (i < tablesize) {
		if ((descending) && (pts[i].x < input)) {
			/* table entry is less than measured*/
			 /* value and table is descending, stop */
			break;
		} else if ((!descending) &&
				(pts[i].x > input)) {
			/* table entry is greater than measured*/
			/*value and table is ascending, stop */
			break;
		}
		i++;
	}

	if (i == 0) {
		*output = pts[0].y;
	} else if (i == tablesize) {
		*output = pts[tablesize - 1].y;
	} else {
		/* result is between search_index and search_index-1 */
		/* interpolate linearly */
		*output = (((s32)((pts[i].y - pts[i - 1].y) *
			(input - pts[i - 1].x)) /
			(pts[i].x - pts[i - 1].x)) +
			pts[i - 1].y);
	}

	return 0;
}

static void qcom_vadc_scale_calib(const struct vadc_linear_graph *calib_graph,
				  u16 adc_code,
				  bool absolute,
				  s64 *scale_voltage)
{
	*scale_voltage = (adc_code - calib_graph->gnd);
	*scale_voltage *= calib_graph->dx;
	*scale_voltage = div64_s64(*scale_voltage, calib_graph->dy);
	if (absolute)
		*scale_voltage += calib_graph->dx;

	if (*scale_voltage < 0)
		*scale_voltage = 0;
}

static int qcom_vadc_scale_volt(const struct vadc_linear_graph *calib_graph,
				const struct vadc_prescale_ratio *prescale,
				bool absolute, u16 adc_code,
				int *result_uv)
{
	s64 voltage = 0, result = 0;

	qcom_vadc_scale_calib(calib_graph, adc_code, absolute, &voltage);

	voltage = voltage * prescale->den;
	result = div64_s64(voltage, prescale->num);
	*result_uv = result;

	return 0;
}

static int qcom_vadc_scale_therm(const struct vadc_linear_graph *calib_graph,
				 const struct vadc_prescale_ratio *prescale,
				 bool absolute, u16 adc_code,
				 int *result_mdec)
{
	s64 voltage = 0, result = 0;
	int ret;
	pr_err("hzn::adcmap_100k_104ef_104fb-----------\n");

	qcom_vadc_scale_calib(calib_graph, adc_code, absolute, &voltage);

	if (absolute)
		voltage = div64_s64(voltage, 1000);

	ret = qcom_vadc_map_voltage_temp(adcmap_100k_104ef_104fb,
					 ARRAY_SIZE(adcmap_100k_104ef_104fb),
					 voltage, &result);
	if (ret)
		return ret;

	result *= 1000;
	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_die_temp(const struct vadc_linear_graph *calib_graph,
				    const struct vadc_prescale_ratio *prescale,
				    bool absolute,
				    u16 adc_code, int *result_mdec)
{
	s64 voltage = 0;
	u64 temp; /* Temporary variable for do_div */

	qcom_vadc_scale_calib(calib_graph, adc_code, absolute, &voltage);

	if (voltage > 0) {
		temp = voltage * prescale->den;
		do_div(temp, prescale->num * 2);
		voltage = temp;
	} else {
		voltage = 0;
	}

	voltage -= KELVINMIL_CELSIUSMIL;
	*result_mdec = voltage;

	return 0;
}

static int qcom_vadc_scale_chg_temp(const struct vadc_linear_graph *calib_graph,
				    const struct vadc_prescale_ratio *prescale,
				    bool absolute,
				    u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, result = 0;

	qcom_vadc_scale_calib(calib_graph, adc_code, absolute, &voltage);

	voltage = voltage * prescale->den;
	voltage = div64_s64(voltage, prescale->num);
	voltage = ((PMI_CHG_SCALE_1) * (voltage * 2));
	voltage = (voltage + PMI_CHG_SCALE_2);
	result =  div64_s64(voltage, 1000000);
	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_volt(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_uv)
{
	s64 voltage = 0, result = 0, adc_vdd_ref_mv = 1875;

	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, data->full_scale_code_volt);
	voltage = voltage * prescale->den;
	result = div64_s64(voltage, prescale->num);
	*result_uv = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_therm(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, result = 0;
	int ret;
	//pr_err("hzn::adcmap_100k_104ef_104fb_1875_vref-----------\n");
	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * ADC_HC_VDD_REF * 1000;
	voltage = div64_s64(voltage, (data->full_scale_code_volt
								* 1000));
	ret = qcom_vadc_map_voltage_temp(adcmap_100k_104ef_104fb_1875_vref,
				 ARRAY_SIZE(adcmap_100k_104ef_104fb_1875_vref),
				 voltage, &result);
	if (ret)
		return ret;

	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_batt_therm_100(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				unsigned int lut_index,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, result = 0, adc_vdd_ref_mv = 1875;
	int ret;
	u32 size;
	const struct vadc_map_pt *lut;
	pr_err("hzn::lut_table_100-----------\n");

	if (lut_index >= ARRAY_SIZE(lut_table_100)) {
		pr_err("LUT index out of range\n");
		return -EINVAL;
	}
	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, (data->full_scale_code_volt
								* 1000));

	lut = lut_table_100[lut_index].table;
	size = lut_table_100[lut_index].tablesize;

	ret = qcom_vadc_map_voltage_temp(lut, size, voltage, &result);

	if (ret)
		return ret;

	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_batt_therm_30(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				unsigned int lut_index,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, result = 0, adc_vdd_ref_mv = 1875;
	int ret;
	u32 size;
	const struct vadc_map_pt *lut;
	//pr_err("hzn::lut_table_30: lut_index = %d-----------\n", lut_index);

	if (lut_index >= ARRAY_SIZE(lut_table_30)) {
		pr_err("LUT index out of range\n");
		return -EINVAL;
	}
	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, (data->full_scale_code_volt
								* 1000));

	lut = lut_table_30[lut_index].table;
	size = lut_table_30[lut_index].tablesize;

	ret = qcom_vadc_map_voltage_temp(lut, size, voltage, &result);

	if (ret)
		return ret;

	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_batt_therm_400(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				unsigned int lut_index,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, result = 0, adc_vdd_ref_mv = 1875;
	int ret;
	u32 size;
	const struct vadc_map_pt *lut;
	pr_err("hzn::lut_table_400-----------\n");

	if (lut_index >= ARRAY_SIZE(lut_table_400)) {
		pr_err("LUT index out of range\n");
		return -EINVAL;
	}
	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, (data->full_scale_code_volt
								* 1000));

	lut = lut_table_400[lut_index].table;
	size = lut_table_400[lut_index].tablesize;

	ret = qcom_vadc_map_voltage_temp(lut, size, voltage, &result);

	if (ret)
		return ret;

	*result_mdec = result;

	return 0;
}

static int qcom_vadc_scale_hw_calib_die_temp(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, adc_vdd_ref_mv = 1875;
	u64 temp; /* Temporary variable for do_div */

	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, data->full_scale_code_volt);
	if (voltage > 0) {
		temp = voltage * prescale->den;
		do_div(temp, prescale->num * 2);
		voltage = temp;
	} else {
		voltage = 0;
	}

	voltage -= KELVINMIL_CELSIUSMIL;
	*result_mdec = voltage;

	return 0;
}

static int qcom_vadc_scale_hw_smb_temp(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, adc_vdd_ref_mv = 1875;
	u64 temp;

	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, data->full_scale_code_volt);
	if (voltage > 0) {
		temp = voltage * prescale->den;
		temp *= 100;
		do_div(temp, prescale->num * PMIC5_SMB_TEMP_SCALE_FACTOR);
		voltage = temp;
	} else {
		voltage = 0;
	}

	voltage = PMIC5_SMB_TEMP_CONSTANT - voltage;
	*result_mdec = voltage;

	return 0;
}

static int qcom_vadc_scale_hw_smb1398_temp(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, adc_vdd_ref_mv = 1875;
	u64 temp;

	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, data->full_scale_code_volt);
	if (voltage > 0) {
		temp = voltage * prescale->den;
		temp *= 100;
		do_div(temp, prescale->num * PMIC5_SMB1398_TEMP_SCALE_FACTOR);
		voltage = temp;
	} else {
		voltage = 0;
	}

	voltage = voltage - PMIC5_SMB1398_TEMP_CONSTANT;
	*result_mdec = voltage;

	return 0;
}

static int qcom_vadc_scale_hw_chg5_temp(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_mdec)
{
	s64 voltage = 0, adc_vdd_ref_mv = 1875;
	u64 temp;

	if (adc_code > VADC5_MAX_CODE)
		adc_code = 0;

	/* (ADC code * vref_vadc (1.875V)) / full_scale_code */
	voltage = (s64) adc_code * adc_vdd_ref_mv * 1000;
	voltage = div64_s64(voltage, data->full_scale_code_volt);
	if (voltage > 0) {
		temp = voltage * prescale->den;
		do_div(temp, prescale->num * 4);
		voltage = temp;
	} else {
		voltage = 0;
	}

	voltage = PMIC5_CHG_TEMP_SCALE_FACTOR - voltage;
	*result_mdec = voltage;

	return 0;
}

static int qcom_adc_scale_hw_calib_cur(
				const struct vadc_prescale_ratio *prescale,
				const struct adc_data *data,
				u16 adc_code, int *result_uamps)
{
	s64 voltage = 0, result = 0;

	if ((adc_code & ADC_USR_DATA_CHECK) == 0) {
		voltage = (s64) adc_code * data->full_scale_code_cur * 1000;
		voltage = div64_s64(voltage, VADC5_MAX_CODE);
		voltage = voltage * prescale->den;
		result = div64_s64(voltage, prescale->num);
		*result_uamps = result;
	} else {
		adc_code = ~adc_code + 1;
		voltage = (s64) adc_code;
		voltage = (s64) adc_code * data->full_scale_code_cur * 1000;
		voltage = div64_s64(voltage, VADC5_MAX_CODE);
		voltage = voltage * prescale->den;
		result = div64_s64(voltage, prescale->num);
		*result_uamps = -result;
	}

	return 0;
}

int qcom_vadc_scale(enum vadc_scale_fn_type scaletype,
		    const struct vadc_linear_graph *calib_graph,
		    const struct vadc_prescale_ratio *prescale,
		    bool absolute,
		    u16 adc_code, int *result)
{
	switch (scaletype) {
	case SCALE_DEFAULT:
		return qcom_vadc_scale_volt(calib_graph, prescale,
					    absolute, adc_code,
					    result);
	case SCALE_THERM_100K_PULLUP:
	case SCALE_XOTHERM:
		return qcom_vadc_scale_therm(calib_graph, prescale,
					     absolute, adc_code,
					     result);
	case SCALE_PMIC_THERM:
		return qcom_vadc_scale_die_temp(calib_graph, prescale,
						absolute, adc_code,
						result);
	case SCALE_PMI_CHG_TEMP:
		return qcom_vadc_scale_chg_temp(calib_graph, prescale,
						absolute, adc_code,
						result);
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL(qcom_vadc_scale);

int qcom_vadc_hw_scale(enum vadc_scale_fn_type scaletype,
		    const struct vadc_prescale_ratio *prescale,
		    const struct adc_data *data, unsigned int lut_index,
		    u16 adc_code, int *result)
{
	switch (scaletype) {
	case SCALE_HW_CALIB_DEFAULT:
		return qcom_vadc_scale_hw_calib_volt(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_THERM_100K_PULLUP:
	case SCALE_HW_CALIB_XOTHERM:
		return qcom_vadc_scale_hw_calib_therm(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_BATT_THERM_100K:
		return qcom_vadc_scale_hw_calib_batt_therm_100(prescale,
					data, lut_index, adc_code, result);
	case SCALE_HW_CALIB_BATT_THERM_30K:
		return qcom_vadc_scale_hw_calib_batt_therm_30(prescale,
					data, lut_index, adc_code, result);
	case SCALE_HW_CALIB_BATT_THERM_400K:
		return qcom_vadc_scale_hw_calib_batt_therm_400(prescale,
					data, lut_index, adc_code, result);
	case SCALE_HW_CALIB_PMIC_THERM:
		return qcom_vadc_scale_hw_calib_die_temp(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_CUR:
		return qcom_adc_scale_hw_calib_cur(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_PM5_CHG_TEMP:
		return qcom_vadc_scale_hw_chg5_temp(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_PM5_SMB_TEMP:
		return qcom_vadc_scale_hw_smb_temp(prescale, data,
						adc_code, result);
	case SCALE_HW_CALIB_PM5_SMB1398_TEMP:
		return qcom_vadc_scale_hw_smb1398_temp(prescale, data,
						adc_code, result);
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL(qcom_vadc_hw_scale);

int qcom_vadc_decimation_from_dt(u32 value)
{
	if (!is_power_of_2(value) || value < VADC_DECIMATION_MIN ||
	    value > VADC_DECIMATION_MAX)
		return -EINVAL;

	return __ffs64(value / VADC_DECIMATION_MIN);
}
EXPORT_SYMBOL(qcom_vadc_decimation_from_dt);

int qcom_adc5_decimation_from_dt(u32 value, const unsigned int *decimation)
{
	uint32_t i;

	for (i = 0; i < ADC_DECIMATION_SAMPLES_MAX; i++) {
		if (value == decimation[i])
			return i;
	}

	return -EINVAL;
}
EXPORT_SYMBOL(qcom_adc5_decimation_from_dt);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Qualcomm ADC common functionality");
