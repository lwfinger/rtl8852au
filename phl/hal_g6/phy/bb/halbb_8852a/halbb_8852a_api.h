/******************************************************************************
 *
 * Copyright(c) 2007 - 2020  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
#ifndef __HALBB_8852A_API_H__
#define __HALBB_8852A_API_H__
#ifdef BB_8852A_CAV_SUPPORT
#ifdef HALBB_CONFIG_RUN_IN_DRV
#include "../halbb_api.h"
#endif

/*@--------------------------[Define]-------------------------------------*/
#define INVALID_RF_DATA 0xffffffff

/*@--------------------------[Enum]------------------------------------------*/
enum bb_efuse_length {
	LEN_BOARD_OPTION		= 1,
	LEN_BB_SWING_2G			= 1,
	LEN_BB_SWING_5G			= 1,
	LEN_CCK_GAIN_GAP_2G		= 2,
	LEN_OFDM_GAIN_GAP_2G		= 2,
	LEN_LOWBAND_5G_GAIN_GAP		= 2,
	LEN_MIDBAND_5G_GAIN_GAP		= 2,
	LEN_HIGHBAND_5G_GAIN_GAP	= 2
};

enum bb_efuse_ofst {
	OFST_BOARD_OPTION		= 0xc1,
	OFST_BB_SWING_2G		= 0xc6,
	OFST_BB_SWING_5G		= 0xc7,
	OFST_CCK_GAIN_GAP_2G		= 0xd6,
	OFST_OFDM_GAIN_GAP_2G		= 0xd4,
	OFST_LOWBAND_5G_GAIN_GAP	= 0xd8,
	OFST_MIDBAND_5G_GAIN_GAP	= 0xda,
	OFST_HIGHBAND_5G_GAIN_GAP	= 0xdc
};

enum bb_efuse_init {
	INIT_BOARD_OPTION		= 0x0,
	INIT_BB_SWING_2G		= 0x0,
	INIT_BB_SWING_5G		= 0x0,
	INIT_CCK_GAIN_GAP_2G		= 0x0,
	INIT_OFDM_GAIN_GAP_2G		= 0x0,
	INIT_LOWBAND_5G_GAIN_GAP	= 0x0,
	INIT_MIDBAND_5G_GAIN_GAP	= 0x0,
	INIT_HIGHBAND_5G_GAIN_GAP	= 0x0
};

/*@--------------------------[Prptotype]-------------------------------------*/
struct bb_info;

bool halbb_ctrl_bw_ch_8852a(struct bb_info *bb, u8 pri_ch, u8 central_ch,
			    enum channel_width bw, enum phl_phy_idx phy_idx);

bool halbb_ctrl_cck_rx_path_8852a(struct bb_info *bb, enum rf_path rx_path);

bool halbb_ctrl_rx_path_8852a(struct bb_info *bb, enum rf_path rx_path);

bool halbb_ctrl_tx_path_8852a(struct bb_info *bb, enum rf_path tx_path);

#ifdef HALBB_CONFIG_RUN_IN_DRV
void halbb_plcp_gen_homologation_8852a(struct bb_info *bb, void *in_void);
u32 halbb_phy0_to_phy1_ofst_8852a(struct bb_info *bb, u32 addr);
void halbb_gpio_ctrl_dump_8852a(struct bb_info *bb);

void halbb_gpio_rfm_8852a(struct bb_info *bb, enum bb_path path,
			  enum bb_rfe_src_sel src, bool dis_tx_gnt_wl,
			  bool active_tx_opt, bool act_bt_en, u8 rfm_output_val);

void halbb_gpio_trsw_table_8852a(struct bb_info *bb, enum bb_path path,
				 bool path_en, bool trsw_tx,
				 bool trsw_rx, bool trsw, bool trsw_b);

void halbb_gpio_setting_8852a(struct bb_info *bb, u8 gpio_idx,
			      enum bb_path path, bool inv,
			      enum bb_rfe_src_sel src);

void halbb_gpio_setting_all_8852a(struct bb_info *bb, u8 rfe_idx);

void halbb_gpio_setting_init_8852a(struct bb_info *bb);

void halbb_bb_reset_8852a(struct bb_info *bb);

void halbb_adc_rst_8852a(struct bb_info *bb);

u32 halbb_read_rf_reg_8852a(struct bb_info *bb, enum rf_path path, u32 reg_addr,
			    u32 bit_mask);

bool halbb_write_rf_reg_8852a(struct bb_info *bb, enum rf_path path, u32 reg_addr,
			      u32 bit_mask, u32 data);

bool halbb_ctrl_bw_8852a(struct bb_info *bb, u8 pri_ch, enum channel_width bw,
			 enum phl_phy_idx phy_idx);

bool halbb_ctrl_ch_8852a(struct bb_info *bb, u8 central_ch,
			 enum phl_phy_idx phy_idx);

void halbb_ctrl_cck_en_8852a(struct bb_info *bb, bool cck_en,
			     enum phl_phy_idx phy_idx);

void halbb_ctrl_trx_path_8852a(struct bb_info *bb, enum rf_path tx_path,
			       u8 tx_nss, enum rf_path rx_path, u8 rx_nss);

bool halbb_efuse_len_chk_8852a(struct bb_info *bb, enum rtw_efuse_info id,
			       u8 length);

bool halbb_get_efuse_info_8852a(struct bb_info *bb, u8 *efuse_map,
				enum rtw_efuse_info id, void *value, u8 length);

u8 halbb_sco_mapping_8852a(struct bb_info *bb,  u8 central_ch);

bool halbb_ctrl_sco_cck_8852a(struct bb_info *bb, u8 central_ch, u8 pri_ch,
			      enum channel_width bw);

bool halbb_bw_setting_8852a(struct bb_info *bb, enum channel_width bw,
			    enum rf_path path);

bool halbb_ch_setting_8852a(struct bb_info *bb, u8 central_ch, enum rf_path path,
			    bool *is_2g_ch);

void halbb_ctrl_rf_mode_8852a(struct bb_info *bb,  enum phl_rf_mode mode);

bool halbb_ctrl_ch2_80p80_8852a(struct bb_info *bb, u8 central_ch);

void halbb_ctrl_dbcc_8852a(struct bb_info *bb, bool dbcc_en);

void halbb_cfg_dbcc_8852a(struct bb_info *bb, bool dbcc_en);

void halbb_ctrl_rx_cca_8852a(struct bb_info *bb, bool cca_en, enum phl_phy_idx phy_idx);

void halbb_ctrl_ofdm_en_8852a(struct bb_info *bb, bool ofdm_en,
			      enum phl_phy_idx phy_idx);
bool halbb_set_txpwr_dbm_8852a(struct bb_info *bb, s16 power_dbm,
			       enum phl_phy_idx phy_idx);

s16 halbb_get_txpwr_dbm_8852a(struct bb_info *bb, enum phl_phy_idx phy_idx);

s16 halbb_get_txinfo_txpwr_dbm_8852a(struct bb_info *bb);

bool halbb_set_cck_txpwr_idx_8852a(struct bb_info *bb, u16 power_idx,
				   enum rf_path tx_path);
u16 halbb_get_cck_txpwr_idx_8852a(struct bb_info *bb, enum rf_path tx_path);

s16 halbb_get_cck_ref_dbm_8852a(struct bb_info *bb, enum rf_path tx_path);

bool halbb_set_ofdm_txpwr_idx_8852a(struct bb_info *bb, u16 power_idx,
				    enum rf_path tx_path);

u16 halbb_get_ofdm_txpwr_idx_8852a(struct bb_info *bb, enum rf_path tx_path);

s16 halbb_get_ofdm_ref_dbm_8852a(struct bb_info *bb, enum rf_path tx_path);

bool halbb_set_txsc_8852a(struct bb_info *bb, u8 txsc, enum phl_phy_idx phy_idx);

bool halbb_set_bss_color_8852a(struct bb_info *bb, u8 bss_color, 
			       enum phl_phy_idx phy_idx);

bool halbb_set_sta_id_8852a(struct bb_info *bb, u16 sta_id,
			    enum phl_phy_idx phy_idx);

void halbb_set_igi_8852a(struct bb_info *bb, u8 lna_idx, bool tia_idx,
			u8 rxbb_idx, enum rf_path path);

bool halbb_lps_8852a(struct bb_info *bb, bool enable_lps);

void halbb_reset_bb_hw_cnt_8852a(struct bb_info *bb);

void halbb_dump_bb_reg_8852a(struct bb_info *bb, u32 *_used, char *output,
			     u32 *_out_len);
void halbb_dbgport_dump_all_8852a(struct bb_info *bb, u32 *_used, char *output,
				  u32 *_out_len);
void halbb_pre_agc_en_8852a(struct bb_info *bb, bool enable);
#endif
#endif
#endif /*  __INC_PHYDM_API_H_8852A__ */
