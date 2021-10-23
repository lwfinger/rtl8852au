/******************************************************************************
 *
 * Copyright(c) 2019 Realtek Corporation.
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
#ifndef _HALRF_SET_PWR_TABLE_8852A_H_
#define _HALRF_SET_PWR_TABLE_8852A_H_

/*@--------------------------Define Parameters-------------------------------*/
#define MAX_TX_PATH	2
#define TPU_SIZE_RUA	3 /*{26, 52, 106}*/
#define TPU_SIZE_BW20_SC  8 /*8 * 20M = 160M*/

/*@-----------------------End Define Parameters-----------------------*/

#define R_AX_PWR_RATE_CTRL_8852A 0xD200
#define R_AX_PWR_RATE_CTRL_C1_8852A 0xF200

#define PWR_BY_RATE_LGCY_OFFSET_8852A  0XC0
#define PWR_BY_RATE_OFFSET_8852A  0XCC

#define R_AX_PWR_LMT_CTRL_8852A 0xD208
#define R_AX_PWR_LMT_CTRL_C1_8852A 0xF208

#define PWR_LMT_CCK_OFFSET_8852A 0XEC
#define PWR_LMT_LGCY_OFFSET_8852A 0XF0
#define PWR_LMT_TBL2_OFFSET_8852A 0XF4
#define PWR_LMT_TBL5_OFFSET_8852A 0X100
#define PWR_LMT_TBL6_OFFSET_8852A 0X104
#define PWR_LMT_TBL7_OFFSET_8852A 0X108
#define PWR_LMT_TBL8_OFFSET_8852A 0X10C
#define PWR_LMT_TBL9_OFFSET_8852A 0X110

#define PWR_LMT_TBL_UNIT_8852A 0X28

#define R_AX_PWR_RU_LMT_TABLE0_8852A 0xD33C
#define R_AX_PWR_RU_LMT_TABLE0_C1_8852A 0xF33C

bool halrf_set_ref_power_8852a(struct rf_info *rf, enum phl_phy_idx phy);

bool halrf_set_power_by_rate_to_mac_8852a(struct rf_info *rf,
						enum phl_phy_idx phy);

bool halrf_set_power_limit_to_mac_8852a(struct rf_info *rf,
	enum phl_phy_idx phy);

void halrf_set_ref_power_minus_dbm_to_struct_8852a(struct rf_info *rf,
	enum phl_phy_idx phy, s16 dbm);

void halrf_set_ref_power_to_struct_8852a(struct rf_info *rf,
						enum phl_phy_idx phy);

bool halrf_set_power_8852a(struct rf_info *rf, enum phl_phy_idx phy,
	enum phl_pwr_table pwr_table);

void halrf_pwr_by_rate_info_8852a(struct rf_info *rf,
		char input[][16], u32 *_used, char *output, u32 *_out_len);

void halrf_pwr_limit_info_8852a(struct rf_info *rf,
		char input[][16], u32 *_used, char *output, u32 *_out_len);

void halrf_pwr_limit_ru_info_8852a(struct rf_info *rf,
		char input[][16], u32 *_used, char *output, u32 *_out_len);

#endif	/*_HALRF_SET_PWR_TABLE_8852A_H_*/