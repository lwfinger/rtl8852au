/******************************************************************************
 *
 * Copyright(c) 2007 - 2019 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#ifndef	__RTW_RF_H_
#define __RTW_RF_H_

#define NumRates	(13)
#define	B_MODE_RATE_NUM	(4)
#define	G_MODE_RATE_NUM	(8)
#define	G_MODE_BASIC_RATE_NUM	(3)
/* slot time for 11g */
#define SHORT_SLOT_TIME					9
#define NON_SHORT_SLOT_TIME				20

#define CENTER_CH_2G_40M_NUM	9
#define CENTER_CH_2G_NUM		14
#define CENTER_CH_5G_20M_NUM	28	/* 20M center channels */
#define CENTER_CH_5G_40M_NUM	14	/* 40M center channels */
#define CENTER_CH_5G_80M_NUM	7	/* 80M center channels */
#define CENTER_CH_5G_160M_NUM	3	/* 160M center channels */
#define CENTER_CH_5G_ALL_NUM	(CENTER_CH_5G_20M_NUM + CENTER_CH_5G_40M_NUM + CENTER_CH_5G_80M_NUM)

#define	MAX_CHANNEL_NUM_2G	CENTER_CH_2G_NUM
#define	MAX_CHANNEL_NUM_5G	CENTER_CH_5G_20M_NUM
#define	MAX_CHANNEL_NUM		(MAX_CHANNEL_NUM_2G + MAX_CHANNEL_NUM_5G)
#define MAX_CHANNEL_NUM_OF_BAND rtw_max(MAX_CHANNEL_NUM_2G, MAX_CHANNEL_NUM_5G)

extern u8 center_ch_2g[CENTER_CH_2G_NUM];
extern u8 center_ch_2g_40m[CENTER_CH_2G_40M_NUM];

u8 center_chs_2g_num(u8 bw);
u8 center_chs_2g(u8 bw, u8 id);

extern u8 center_ch_5g_20m[CENTER_CH_5G_20M_NUM];
extern u8 center_ch_5g_40m[CENTER_CH_5G_40M_NUM];
extern u8 center_ch_5g_20m_40m[CENTER_CH_5G_20M_NUM + CENTER_CH_5G_40M_NUM];
extern u8 center_ch_5g_80m[CENTER_CH_5G_80M_NUM];
extern u8 center_ch_5g_all[CENTER_CH_5G_ALL_NUM];

u8 center_chs_5g_num(u8 bw);
u8 center_chs_5g(u8 bw, u8 id);

u8 rtw_get_scch_by_cch_offset(u8 cch, u8 bw, u8 offset);

u8 rtw_get_op_chs_by_cch_bw(u8 cch, u8 bw, u8 **op_chs, u8 *op_ch_num);

u8 rtw_get_offset_by_chbw(u8 ch, u8 bw, u8 *r_offset);
u8 rtw_get_center_ch(u8 ch, u8 bw, u8 offset);

u8 rtw_get_ch_group(u8 ch, u8 *group, u8 *cck_group);

typedef enum _CAPABILITY {
	cESS			= 0x0001,
	cIBSS			= 0x0002,
	cPollable		= 0x0004,
	cPollReq			= 0x0008,
	cPrivacy		= 0x0010,
	cShortPreamble	= 0x0020,
	cPBCC			= 0x0040,
	cChannelAgility	= 0x0080,
	cSpectrumMgnt	= 0x0100,
	cQos			= 0x0200,	/* For HCCA, use with CF-Pollable and CF-PollReq */
	cShortSlotTime	= 0x0400,
	cAPSD			= 0x0800,
	cRM				= 0x1000,	/* RRM (Radio Request Measurement) */
	cDSSS_OFDM	= 0x2000,
	cDelayedBA		= 0x4000,
	cImmediateBA	= 0x8000,
} CAPABILITY, *PCAPABILITY;

enum	_REG_PREAMBLE_MODE {
	PREAMBLE_LONG	= 1,
	PREAMBLE_AUTO	= 2,
	PREAMBLE_SHORT	= 3,
};

#define rf_path_char(path) (((path) >= RF_PATH_MAX) ? 'X' : 'A' + (path))

#ifdef CONFIG_NARROWBAND_SUPPORTING
enum nb_config {
	RTW_NB_CONFIG_NONE		= 0,
	RTW_NB_CONFIG_WIDTH_5	= 5,
	RTW_NB_CONFIG_WIDTH_10	= 6,
};
#endif

extern const char *const _band_str[];
#define band_str(band) (((band) >= BAND_MAX) ? _rtw_band_str[BAND_MAX] : _rtw_band_str[(band)])

extern const u8 _band_to_band_cap[];
#define band_to_band_cap(band) (((band) >= BAND_MAX) ? _band_to_band_cap[BAND_MAX] : _band_to_band_cap[(band)])


extern const char *const _ch_width_str[];
#define ch_width_str(bw) (((bw) < CHANNEL_WIDTH_MAX) ? _ch_width_str[(bw)] : "CHANNEL_WIDTH_MAX")

extern const u8 _ch_width_to_bw_cap[];
#define ch_width_to_bw_cap(bw) (((bw) < CHANNEL_WIDTH_MAX) ? _ch_width_to_bw_cap[(bw)] : 0)

enum opc_bw {
	OPC_BW20		= 0,
	OPC_BW40PLUS	= 1,
	OPC_BW40MINUS	= 2,
	OPC_BW80		= 3,
	OPC_BW160		= 4,
	OPC_BW80P80		= 5,
	OPC_BW_NUM,
};

extern const char *const _opc_bw_str[OPC_BW_NUM];
#define opc_bw_str(bw) (((bw) < OPC_BW_NUM) ? _opc_bw_str[(bw)] : "N/A")

extern const u8 _opc_bw_to_ch_width[OPC_BW_NUM];
#define opc_bw_to_ch_width(bw) (((bw) < OPC_BW_NUM) ? _opc_bw_to_ch_width[(bw)] : CHANNEL_WIDTH_MAX)

/* global op class APIs */
bool is_valid_global_op_class_id(u8 gid);
s16 get_sub_op_class(u8 gid, u8 ch);
void dump_global_op_class(void *sel);
u8 rtw_get_op_class_by_chbw(u8 ch, u8 bw, u8 offset);
u8 rtw_get_bw_offset_by_op_class_ch(u8 gid, u8 ch, u8 *bw, u8 *offset);

struct op_ch_t {
	u8 ch;
	u8 static_non_op:1; /* not in channel list */
	u8 no_ir:1;
	s16 max_txpwr; /* mBm */
};

struct op_class_pref_t {
	u8 class_id;
	enum band_type band;
	enum opc_bw bw;
	u8 ch_num; /* number of chs */
	u8 op_ch_num; /* channel number which is not static non operable */
	u8 ir_ch_num; /* channel number which can init radiation */
	struct op_ch_t chs[MAX_CHANNEL_NUM_OF_BAND]; /* zero(ch) terminated array */
};

int op_class_pref_init(_adapter *adapter);
void op_class_pref_deinit(_adapter *adapter);

#define REG_BEACON_HINT		0
#define REG_TXPWR_CHANGE	1
#define REG_CHANGE			2

void op_class_pref_apply_regulatory(_adapter *adapter, u8 reason);

struct rf_ctl_t;
void dump_cap_spt_op_class_ch(void *sel, struct rf_ctl_t *rfctl, bool detail);
void dump_reg_spt_op_class_ch(void *sel, struct rf_ctl_t *rfctl, bool detail);
void dump_cur_spt_op_class_ch(void *sel, struct rf_ctl_t *rfctl, bool detail);

typedef enum _VHT_DATA_SC {
	VHT_DATA_SC_DONOT_CARE = 0,
	VHT_DATA_SC_20_UPPER_OF_80MHZ = 1,
	VHT_DATA_SC_20_LOWER_OF_80MHZ = 2,
	VHT_DATA_SC_20_UPPERST_OF_80MHZ = 3,
	VHT_DATA_SC_20_LOWEST_OF_80MHZ = 4,
	VHT_DATA_SC_20_RECV1 = 5,
	VHT_DATA_SC_20_RECV2 = 6,
	VHT_DATA_SC_20_RECV3 = 7,
	VHT_DATA_SC_20_RECV4 = 8,
	VHT_DATA_SC_40_UPPER_OF_80MHZ = 9,
	VHT_DATA_SC_40_LOWER_OF_80MHZ = 10,
} VHT_DATA_SC, *PVHT_DATA_SC_E;

typedef enum _PROTECTION_MODE {
	PROTECTION_MODE_AUTO = 0,
	PROTECTION_MODE_FORCE_ENABLE = 1,
	PROTECTION_MODE_FORCE_DISABLE = 2,
} PROTECTION_MODE, *PPROTECTION_MODE;

typedef enum _RF_TX_NUM {
	RF_1TX = 0,
	RF_2TX,
	RF_3TX,
	RF_4TX,
	RF_MAX_TX_NUM,
	RF_TX_NUM_NONIMPLEMENT,
} RF_TX_NUM;

#define RF_TYPE_VALID(rf_type) (rf_type < RF_TYPE_MAX)

extern const u8 _rf_type_to_rf_tx_cnt[];
#define rf_type_to_rf_tx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_tx_cnt[rf_type] : 0)

extern const u8 _rf_type_to_rf_rx_cnt[];
#define rf_type_to_rf_rx_cnt(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rf_rx_cnt[rf_type] : 0)

extern const char *const _rf_type_to_rfpath_str[];
#define rf_type_to_rfpath_str(rf_type) (RF_TYPE_VALID(rf_type) ? _rf_type_to_rfpath_str[rf_type] : "UNKNOWN")

enum rf_type trx_num_to_rf_type(u8 tx_num, u8 rx_num);
enum rf_type trx_bmp_to_rf_type(u8 tx_bmp, u8 rx_bmp);
bool rf_type_is_a_in_b(enum rf_type a, enum rf_type b);
u8 rtw_restrict_trx_path_bmp_by_rftype(u8 trx_path_bmp, enum rf_type type, u8 *tx_num, u8 *rx_num);


int rtw_ch2freq(int chan);
int rtw_freq2ch(int freq);
bool rtw_chbw_to_freq_range(u8 ch, u8 bw, u8 offset, u32 *hi, u32 *lo);

struct rf_ctl_t;

typedef enum _REGULATION_TXPWR_LMT {
	TXPWR_LMT_NONE = 0, /* no limit */
	TXPWR_LMT_FCC = 1,
	TXPWR_LMT_MKK = 2,
	TXPWR_LMT_ETSI = 3,
	TXPWR_LMT_IC = 4,
	TXPWR_LMT_KCC = 5,
	TXPWR_LMT_NCC = 6,
	TXPWR_LMT_ACMA = 7,
	TXPWR_LMT_CHILE = 8,
	TXPWR_LMT_UKRAINE = 9,
	TXPWR_LMT_MEXICO = 10,
	TXPWR_LMT_CN = 11,
	TXPWR_LMT_WW, /* smallest of all available limit, keep last */
} REGULATION_TXPWR_LMT;

extern const char *const _regd_str[];
#define regd_str(regd) (((regd) > TXPWR_LMT_WW) ? _regd_str[TXPWR_LMT_WW] : _regd_str[(regd)])

void txpwr_idx_get_dbm_str(s8 idx, u8 txgi_max, u8 txgi_pdbm, SIZE_T cwidth, char dbm_str[], u8 dbm_str_len);

#define MBM_PDBM 100
#define UNSPECIFIED_MBM 32767 /* maximum of s16 */

void txpwr_mbm_get_dbm_str(s16 mbm, SIZE_T cwidth, char dbm_str[], u8 dbm_str_len);
s16 mb_of_ntx(u8 ntx);

#if CONFIG_TXPWR_LIMIT
struct regd_exc_ent {
	_list list;
	char country[2];
	u8 domain;
	char regd_name[];
};

void dump_regd_exc_list(void *sel, struct rf_ctl_t *rfctl);
void rtw_regd_exc_add_with_nlen(struct rf_ctl_t *rfctl, const char *country, u8 domain, const char *regd_name, u32 nlen);
void rtw_regd_exc_add(struct rf_ctl_t *rfctl, const char *country, u8 domain, const char *regd_name);
struct regd_exc_ent *_rtw_regd_exc_search(struct rf_ctl_t *rfctl, const char *country, u8 domain);
struct regd_exc_ent *rtw_regd_exc_search(struct rf_ctl_t *rfctl, const char *country, u8 domain);
void rtw_regd_exc_list_free(struct rf_ctl_t *rfctl);


void rtw_txpwr_lmt_add_with_nlen(struct rf_ctl_t *rfctl, const char *regd_name, u32 nlen
	, u8 band, u8 bw, u8 tlrs, u8 ntx_idx, u8 ch_idx, s8 lmt);
void rtw_txpwr_lmt_add(struct rf_ctl_t *rfctl, const char *regd_name
	, u8 band, u8 bw, u8 tlrs, u8 ntx_idx, u8 ch_idx, s8 lmt);
struct txpwr_lmt_ent *_rtw_txpwr_lmt_get_by_name(struct rf_ctl_t *rfctl, const char *regd_name);
struct txpwr_lmt_ent *rtw_txpwr_lmt_get_by_name(struct rf_ctl_t *rfctl, const char *regd_name);
void rtw_txpwr_lmt_list_free(struct rf_ctl_t *rfctl);
#endif /* CONFIG_TXPWR_LIMIT */

/* only check channel ranges */
#define rtw_is_2g_ch(ch) (ch >= 1 && ch <= 14)
#define rtw_is_5g_ch(ch) ((ch) >= 36 && (ch) <= 177)
#define rtw_is_same_band(a, b) \
	((rtw_is_2g_ch(a) && rtw_is_2g_ch(b)) \
	|| (rtw_is_5g_ch(a) && rtw_is_5g_ch(b)))

#define rtw_is_5g_band1(ch) ((ch) >= 36 && (ch) <= 48)
#define rtw_is_5g_band2(ch) ((ch) >= 52 && (ch) <= 64)
#define rtw_is_5g_band3(ch) ((ch) >= 100 && (ch) <= 144)
#define rtw_is_5g_band4(ch) ((ch) >= 149 && (ch) <= 177)
#define rtw_is_same_5g_band(a, b) \
	((rtw_is_5g_band1(a) && rtw_is_5g_band1(b)) \
	|| (rtw_is_5g_band2(a) && rtw_is_5g_band2(b)) \
	|| (rtw_is_5g_band3(a) && rtw_is_5g_band3(b)) \
	|| (rtw_is_5g_band4(a) && rtw_is_5g_band4(b)))

bool rtw_is_long_cac_range(u32 hi, u32 lo, u8 dfs_region);
bool rtw_is_long_cac_ch(u8 ch, u8 bw, u8 offset, u8 dfs_region);



/* Tx Power Limit Table Size */
#define MAX_REGULATION_NUM				4
#define MAX_RF_PATH_NUM_IN_POWER_LIMIT_TABLE		4
#define MAX_2_4G_BANDWIDTH_NUM				2
#define MAX_RATE_SECTION_NUM				10
#define MAX_5G_BANDWIDTH_NUM				4

#define NUM_OF_TARGET_TXPWR_2G	10 /* CCK:1, OFDM:1, HT:4, VHT:4 */
#define NUM_OF_TARGET_TXPWR_5G	9 /* OFDM:1, HT:4, VHT:4 */

#define TXPWR_LMT_RS_CCK	0
#define TXPWR_LMT_RS_OFDM	1
#define TXPWR_LMT_RS_HT	2
#define TXPWR_LMT_RS_VHT	3
#define TXPWR_LMT_RS_NUM	4

#define TXPWR_LMT_RS_NUM_2G	4 /* CCK, OFDM, HT, VHT */
#define TXPWR_LMT_RS_NUM_5G	3 /* OFDM, HT, VHT */
#define	MAX_TX_COUNT		4

#if CONFIG_TXPWR_LIMIT
extern const char *const _txpwr_lmt_rs_str[];
#define txpwr_lmt_rs_str(rs) (((rs) >= TXPWR_LMT_RS_NUM) ? _txpwr_lmt_rs_str[TXPWR_LMT_RS_NUM] : _txpwr_lmt_rs_str[(rs)])

struct txpwr_lmt_ent {
	_list list;

	s8 lmt_2g[MAX_2_4G_BANDWIDTH_NUM]
		[TXPWR_LMT_RS_NUM_2G]
		[CENTER_CH_2G_NUM]
		[MAX_TX_COUNT];

#if CONFIG_IEEE80211_BAND_5GHZ
	s8 lmt_5g[MAX_5G_BANDWIDTH_NUM]
		[TXPWR_LMT_RS_NUM_5G]
		[CENTER_CH_5G_ALL_NUM]
		[MAX_TX_COUNT];
#endif

	char regd_name[];
};
#endif /* CONFIG_TXPWR_LIMIT */

#endif /* _RTL8711_RF_H_ */
