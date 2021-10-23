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
#ifndef __HALBB_8852A_2_H__
#define __HALBB_8852A_2_H__
#ifdef BB_8852A_2_SUPPORT

#include "../halbb_pmac_setting_ex.h"

#define BB_TDMA_PFD
#define BB_DYN_1R_CCA

enum bb_pfd_fmt_type {
	PFD_B_MODE	= 0,
	PFD_LEGACY	= 1,
	PFD_HT_MF	= 2,
	PFD_HT_GF	= 3,
	PFD_VHT		= 4,
	PFD_HE_SU	= 5,
	PFD_HE_ER_SU	= 6,
	PFD_HE_MU	= 7,
	PFD_HE_TB	= 8,
	PFD_ALL_FMT	= 0xff
};

struct bb_8852a_2_info {
#ifdef BB_DYN_1R_CCA
	bool			dyn_1r_cca_en;
	enum rf_path		dyn_1r_cca_cfg;
	u16			dyn_1r_cca_rssi_diff_th; /*RSSI  u(16,5)*/
	u16			dyn_1r_cca_rssi_min_th; /*RSSI  u(16,5)*/
#endif
	bool			tdma_pfd_en;
	u32			pfd_period_ms_0; /*Lgcy only period*/
	u32			pfd_period_ms_1; /*All mode period*/
	enum bb_pfd_fmt_type	pfd_fmt_curr;
	halbb_timer_list	pfd_timer;
};

struct bb_h2c_fw_cmw {
	u8 cmw_connect;
};

struct bb_info;
void halbb_dyn_1r_cca_cfg_8852a_2(struct bb_info *bb, enum rf_path rx_path);
void halbb_dyn_1r_cca_8852a_2(struct bb_info *bb);
void halbb_dyn_1r_cca_en_8852a_2(struct bb_info *bb, bool en);
void halbb_manual_pkt_fmt_sel_8852a_2(struct bb_info *bb, enum bb_pfd_fmt_type type);
#ifdef BB_TDMA_PFD
void halbb_pkt_fmt_sel_timers_8852a_2(struct bb_info *bb, enum bb_timer_state_t state);
void halbb_pkt_fmt_sel_main_8852a_2(struct bb_info *bb);
void halbb_pkt_fmt_sel_deinit_8852a_2(struct bb_info *bb);
#endif
bool halbb_chk_pkg_valid_8852a_2(struct bb_info *bb, u8 bb_ver, u8 rf_ver);
void halbb_set_pmac_tx_8852a_2(struct bb_info *bb, struct halbb_pmac_info *tx_info,
			     enum phl_phy_idx phy_idx);
void halbb_set_tmac_tx_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx);
void halbb_dpd_bypass_8852a_2(struct bb_info *bb, bool pdp_bypass,
			      enum phl_phy_idx phy_idx);
void halbb_ic_hw_setting_init_8852a_2(struct bb_info *bb);
void halbb_ic_hw_setting_8852a_2(struct bb_info *bb);

bool halbb_set_pd_lower_bound_8852a_2(struct bb_info *bb, u8 bound,
				      enum channel_width bw,
				      enum phl_phy_idx phy_idx);
u8 halbb_querry_pd_lower_bound_8852a_2(struct bb_info *bb, bool get_en_info,
				       enum phl_phy_idx phy_idx);
void halbb_pop_en_8852a_2(struct bb_info *bb, bool en, enum phl_phy_idx phy_idx);
bool halbb_querry_pop_en_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx);
u16 halbb_get_per_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx);
u8 halbb_get_losel_8852a_2(struct bb_info *bb);
bool halbb_chk_tx_idle_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx);

#endif
#endif /*  __HALBB_8852a_2_H__ */
