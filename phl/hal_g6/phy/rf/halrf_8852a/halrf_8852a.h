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
#ifndef __HALRF_8852A_H__
#define __HALRF_8852A_H__
#ifdef RF_8852A_SUPPORT

#define RXDCK_VER_8852A 0xe
#define RCK_VER_8852A 0x5
#define FCS_TIMEOUT 100000
#define FCS_THER_STEP 8

void halrf_lo_test_8852a(struct rf_info *rf, bool is_on, enum rf_path path);
u8 halrf_kpath_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx);
void halrf_set_rx_dck_8852a(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_afe);
void halrf_rx_dck_onoff_8852a(struct rf_info *rf, bool is_enable);
void halrf_rx_dck_8852a(struct rf_info *rf, enum phl_phy_idx phy, bool is_afe);
void halrf_rck_8852a(struct rf_info *rf, enum rf_path path);
void halrf_rf_direct_cntrl_8852a(struct rf_info *rf, enum rf_path path, bool is_bybb);
void halrf_bf_config_rf_8852a(struct rf_info *rf);
void halrf_rfk_reg_backup_8852a(struct rf_info *rf);
void halrf_rfc_reg_backup_8852a(struct rf_info *rf);
bool halrf_rfk_reg_check_fail_8852a(struct rf_info *rf);
bool halrf_rfc_reg_check_fail_8852a(struct rf_info *rf);
bool halrf_rfk_chl_thermal_8852a(struct rf_info *rf, u8 chl_idx, u8 ther_idx);
void halrf_rfk_recovery_chl_thermal_8852a(struct rf_info *rf, u8 chl_idx);
u8 halrf_thermal_index_cal_8852a(struct rf_info *rf);
void halrf_fcs_init_8852a(struct rf_info *rf);
void halrf_lck_check_a_8852a(struct rf_info *rf);
void halrf_lck_check_b_8852a(struct rf_info *rf);
extern struct rfk_iqk_info rf_iqk_hwspec_8852a;

bool halrf_check_mcc_ch_8852a(struct rf_info *rf, struct rtw_chan_def *chandef);
void halrf_disconnect_notify_8852a(struct rf_info *rf, struct rtw_chan_def *chandef);
void halrf_fw_ntfy_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx);
void halrf_by_rate_dpd_8852a(struct rf_info *rf, enum phl_phy_idx phy);
void halrf_dpk_init_8852a(struct rf_info *rf);
void halrf_rxbb_bw_8852a(struct rf_info *rf, enum phl_phy_idx phy, enum channel_width bw);
void halrf_2g_rxant_8852a(struct rf_info *rf, enum halrf_ant ant);
void halrf_quick_check_rfrx_8852a(struct rf_info *rf);

#endif
#endif /*  __HALRF_8852A_H__ */
