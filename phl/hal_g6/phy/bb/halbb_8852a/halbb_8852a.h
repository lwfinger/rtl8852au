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
#ifndef __HALBB_8852A_H__
#define __HALBB_8852A_H__
#ifdef BB_8852A_CAV_SUPPORT

#ifdef HALBB_CONFIG_RUN_IN_DRV
#include "../halbb_pmac_setting_ex.h"
#endif

struct bb_info;

struct bb_8852a_info{
	/* Backup / Restore Tx Info for RF TSSI settings */
	u8 tx_path;
	s16 pwr_dbm;
};

void halbb_set_pmac_tx_8852a(struct bb_info *bb, struct halbb_pmac_info *tx_info,
			     enum phl_phy_idx phy_idx);

#ifdef HALBB_CONFIG_RUN_IN_DRV
void halbb_set_tmac_tx_8852a(struct bb_info *bb, enum phl_phy_idx phy_idx);
void halbb_rx_setting_8852a(struct bb_info *bb, u8 patch_idx);
void halbb_ic_hw_setting_init_8852a(struct bb_info *bb);
bool halbb_chk_pkg_valid_8852a(struct bb_info *bb, u8 bb_ver, u8 rf_ver);
#endif

#endif
#endif /*  __INC_PHYDM_API_H_8852A__ */
