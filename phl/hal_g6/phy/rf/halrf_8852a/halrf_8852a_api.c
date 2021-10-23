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
#include "../halrf_precomp.h"
#ifdef RF_8852A_SUPPORT

u8 halrf_get_thermal_8852a(struct rf_info *rf, enum rf_path rf_path)
{
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x1);
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x0);
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x1);

	halrf_delay_10us(rf, 20);

	return (u8)halrf_rrf(rf, rf_path, 0x42, 0x0007e);
}

bool halrf_set_dbcc_8852a(struct rf_info *rf, bool dbcc_en)
{
	u8 path = 0x0;

	if (dbcc_en == 1) {
		/*SYN0 on + SYN1 on*/
		halrf_wrf(rf, RF_PATH_A, 0xA0, (BIT(3) | BIT(2)), 0x3);
		halrf_wrf(rf, RF_PATH_B, 0xA0, (BIT(3) | BIT(2)), 0x3);
		halrf_by_rate_dpd_8852a(rf, HW_PHY_1);
	} else if (dbcc_en == 0) {
		/*SYN0 on + SYN1 off*/
		halrf_wrf(rf, RF_PATH_A, 0xA0, (BIT(3) | BIT(2)), 0x3);
		halrf_wrf(rf, RF_PATH_B, 0xA0, (BIT(3) | BIT(2)), 0x0);
	}
	path = 0x0;
#if 0
	/* IQK backup & restore*/	
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	for (path = 0; path < 2; path ++) {
		if (dbcc_en == 1) {
			halrf_iqk_tx_bypass(rf, path);		
			halrf_iqk_rx_bypass(rf, path);
		} else
			halrf_iqk_reload(rf , path);
	}
#endif
	return true;

#if 0
	/*SYN0 off + SYN1 on*/
	halrf_wrf(rf, RF_PATH_A, 0xA0, (BIT(3) | BIT(2)), 0x0);
	halrf_wrf(rf, RF_PATH_B, 0xA0, (BIT(3) | BIT(2)), 0x3);

	/*SYN0 off + SYN1 off*/
	halrf_wrf(rf, RF_PATH_A, 0xA0, (BIT(3) | BIT(2)), 0x0);
	halrf_wrf(rf, RF_PATH_B, 0xA0, (BIT(3) | BIT(2)), 0x0);
#endif
}

void halrf_trigger_thermal_8852a(struct rf_info *rf, enum rf_path rf_path)
{
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x1);
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x0);
	halrf_wrf(rf, rf_path, 0x42, BIT(19), 0x1);
}

u8 halrf_only_get_thermal_8852a(struct rf_info *rf, enum rf_path rf_path)
{
	return (u8)halrf_rrf(rf, rf_path, 0x42, 0x0007e);
}

bool halrf_wl_tx_power_control_8852a(struct rf_info *rf, u32 tx_power_val)
{
	struct rtw_hal_com_t *hal = rf->hal_com;
	struct halrf_pwr_info *pwr = &rf->pwr_info;
	u32 tmp = 0, result; 
	s32 tmp_pwr;
	u8 phy = 0;
	u32 all_time_control = 0;
	u32 gnt_bt_control = 0;

	RF_DBG(rf, DBG_RF_POWER, "=======>%s\n", __func__);

	all_time_control = tx_power_val & 0xffff;
	gnt_bt_control = (tx_power_val & 0xffff0000) >> 16;

	RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl]tx_power_val=0x%x   all_time_control=0x%x   gnt_bt_control=0x%x\n",
		tx_power_val, all_time_control, gnt_bt_control);

	if (gnt_bt_control == 0xffff) {
		if (all_time_control == 0xffff) {
			/*Coex Disable*/
			pwr->coex_pwr_ctl_enable = false;
			pwr->coex_pwr = 0;	/*30dBm*/
			RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] Coex Disable all_time_control=0xffff!!!\n");
		} else if (all_time_control == 0xeeee) {
			/*DPK Disable*/
			pwr->dpk_pwr_ctl_enable = false;
			pwr->dpk_pwr = 0;	/*30dBm*/
			RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] DPK Disable all_time_control=0xeeee\n");
		} else {
			if (all_time_control & BIT(15)) {
				/*DPK*/
				if (all_time_control & BIT(8))
					tmp_pwr = 0xfffffe00 | (all_time_control & 0x1ff);
				else
					tmp_pwr = all_time_control & ~ BIT(15);

				pwr->dpk_pwr_ctl_enable = true;
				pwr->dpk_pwr = tmp_pwr;

				RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] DPK Enable Set pwr->dpk_pwr = %d\n",
					pwr->dpk_pwr);
			} else {
				/*Coex*/
				if (all_time_control & BIT(8))
					tmp_pwr = 0xfffffe00 | (all_time_control & 0x1ff);
				else
					tmp_pwr = all_time_control;

				pwr->coex_pwr_ctl_enable = true;
				pwr->coex_pwr = tmp_pwr;

				RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] Coex Enable Set pwr->coex_pwr = %d\n",
					pwr->coex_pwr);
			}
		}

		if (pwr->coex_pwr_ctl_enable == true && pwr->dpk_pwr_ctl_enable == false) {
			tmp_pwr = pwr->coex_pwr;

			RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] coex_pwr_ctl_enable=true dpk_pwr_ctl_enable=false tmp_pwr=%d\n",
				tmp_pwr);
		} else if (pwr->coex_pwr_ctl_enable == false && pwr->dpk_pwr_ctl_enable == true) {
			tmp_pwr = pwr->dpk_pwr;

			RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] coex_pwr_ctl_enable=false dpk_pwr_ctl_enable=true tmp_pwr=%d\n",
				tmp_pwr);
		} else if (pwr->coex_pwr_ctl_enable == true && pwr->dpk_pwr_ctl_enable == true) {
			if (pwr->coex_pwr > pwr->dpk_pwr)
				tmp_pwr = pwr->dpk_pwr;
			else
				tmp_pwr = pwr->coex_pwr;

			RF_DBG(rf, DBG_RF_POWER, "[Pwr Ctrl] coex_pwr_ctl_enable=true dpk_pwr_ctl_enable=true tmp_pwr=%d\n",
				tmp_pwr);
		} else
			tmp_pwr = 0;

		if (pwr->coex_pwr_ctl_enable == false && pwr->dpk_pwr_ctl_enable == false) {
			/*all-time control Disable*/
			result = rtw_hal_mac_get_pwr_reg(hal, phy, 0xd200, &tmp);
			if (result) {
				RF_WARNING("=======>%s Get MAC(0xd200) fail, error code=%d\n",
					__func__, result);
				return false;
			} else
				RF_DBG(rf, DBG_RF_POWER, "Get MAC(0xd200) ok!!! 0x%08x\n", tmp);

			tmp = tmp & 0xfffffdff;

			result = rtw_hal_mac_set_pwr_reg(hal, phy, 0xd200, tmp);
			if (result) {
				RF_WARNING("=======>%s Set MAC(0xd200) fail, error code=%d\n",
					__func__, result);
				return false;
			} else {
				RF_DBG(rf, DBG_RF_POWER, "Set MAC(0xd200) ok!!! 0x%08x\n", tmp);
				rf->is_coex = false;
				halrf_set_power_8852a(rf, 0, (PWR_LIMIT | PWR_LIMIT_RU));
			}
		} else {
			/*all-time control*/
			result = rtw_hal_mac_get_pwr_reg(hal, phy, 0xd200, &tmp);
			if (result) {
				RF_WARNING("=======>%s Get MAC(0xd200) fail, error code=%d\n",
					__func__, result);
				return false;
			} else
				RF_DBG(rf, DBG_RF_POWER, "Get MAC(0xd200) ok!!! 0x%08x\n", tmp);

			tmp = tmp | BIT(9);
			tmp = tmp & 0xfffffe00;
			tmp = tmp | (tmp_pwr & 0x1ff);

			result = rtw_hal_mac_set_pwr_reg(hal, phy, 0xd200, tmp);
			if (result) {
				RF_WARNING("=======>%s Set MAC(0xd200) fail, error code=%d\n",
					__func__, result);
				return false;
			} else {
				RF_DBG(rf, DBG_RF_POWER, "Set MAC(0xd200) ok!!! 0x%08x\n", tmp);
				rf->is_coex = true;
			}
		}
	}

	if (gnt_bt_control == 0xffff) {
		/*GNT_BT control*/

		RF_DBG(rf, DBG_RF_POWER, "=======>%s   gnt_bt_control = 0x%x\n",
			__func__, gnt_bt_control);
		
		result = rtw_hal_mac_get_pwr_reg(hal, phy, 0xd220, &tmp);
		if (result) {
			RF_WARNING("=======>%s Get MAC(0xd220) fail, error code=%d\n",
				__func__, result);
			return false;
		} else
			RF_DBG(rf, DBG_RF_POWER, "Get MAC(0xd220) ok!!! 0x%08x\n", tmp);

		tmp = tmp & 0xfffffffd;

		result = rtw_hal_mac_set_pwr_reg(hal, phy, 0xd220, tmp);
		if (result) {
			RF_WARNING("=======>%s Set MAC(0xd220) fail, error code=%d\n",
				__func__, result);
			return false;
		} else {
			RF_DBG(rf, DBG_RF_POWER, "Set MAC(0xd220) ok!!! 0x%08x\n", tmp);
			rf->is_coex = false;
			halrf_set_power_8852a(rf, 0, (PWR_LIMIT | PWR_LIMIT_RU));
		}
	} else {
		/*GNT_BT control*/

		RF_DBG(rf, DBG_RF_POWER, "=======>%s   gnt_bt_control = 0x%x\n",
			__func__, gnt_bt_control);

		result = rtw_hal_mac_get_pwr_reg(hal, phy, 0xd220, &tmp);
		if (result) {
			RF_WARNING("=======>%s Get MAC(0xd220) fail, error code=%d\n",
				__func__, result);
			return false;
		} else
			RF_DBG(rf, DBG_RF_POWER, "Get MAC(0xd220) ok!!! 0x%08x\n", tmp);

		tmp = tmp | BIT(1);
		tmp = tmp & 0xfffff007;
		tmp = tmp | (gnt_bt_control & 0x1ff) << 3;

		result = rtw_hal_mac_set_pwr_reg(hal, phy, 0xd220, tmp);
		if (result) {
			RF_WARNING("=======>%s Set MAC(0xd220) fail, error code=%d\n",
				__func__, result);
			return false;
		} else {
			RF_DBG(rf, DBG_RF_POWER, "Set MAC(0xd220) ok!!! 0x%08x\n", tmp);
			rf->is_coex = true;
		}
	}

	return true;
}

u32 halrf_mac_get_pwr_reg_8852a(struct rf_info *rf, enum phl_phy_idx phy,
	u32 addr, u32 mask)
{
	struct rtw_hal_com_t *hal = rf->hal_com;
	u32 result, ori_val, bit_shift, reg_val;

	result = rtw_hal_mac_get_pwr_reg(hal, phy, addr, &ori_val);
	if (result)
		RF_WARNING("=======>%s Get MAC(0x%x) fail, error code=%d\n",
			__func__, addr, result);
	else
		RF_DBG(rf, DBG_RF_POWER, "Get MAC(0x%x) ok!!! 0x%08x\n",
			addr, ori_val);

	bit_shift = halrf_cal_bit_shift(mask);
	reg_val = (ori_val & mask) >> bit_shift;

	return reg_val;
}

bool halrf_mac_set_pwr_reg_8852a(struct rf_info *rf, enum phl_phy_idx phy,
	u32 addr, u32 mask, u32 val)
{
	struct rtw_hal_com_t *hal = rf->hal_com;
	u32 result, ori_val, bit_shift;

	if (mask != MASKDWORD) {
		result = rtw_hal_mac_get_pwr_reg(hal, phy, addr, &ori_val);
		if (result) {
			RF_WARNING("=======>%s Get MAC(0x%x) fail, error code=%d\n",
				__func__, addr, result);
			return false;
		}
		else
			RF_DBG(rf, DBG_RF_POWER, "Get MAC(0x%x) ok!!! 0x%08x\n",
				addr, ori_val);

		bit_shift = halrf_cal_bit_shift(mask);
		val = ((ori_val) & (~mask)) | (((val << bit_shift)) & mask);
	}

	result = rtw_hal_mac_set_pwr_reg(hal, phy, addr, val);
	if (result) {
		RF_WARNING("=======>%s Set MAC(0x%x) fail, error code=%d\n",
			__func__, addr, result);
		return false;
	} else
		RF_DBG(rf, DBG_RF_POWER, "Set MAC(0x%x) ok!!! 0x%08x\n",
			addr, val);

	return true;
}

u8 halrf_get_default_rfe_type_8852a(struct rf_info *rf)
{
	return 1;
}

u8 halrf_get_default_xtal_8852a(struct rf_info *rf)
{
	return 0x50;
}

void halrf_set_gpio_8852a(struct rf_info *rf, enum phl_phy_idx phy, u8 band)
{
#ifndef RF_8852B_SUPPORT
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	RF_DBG(rf, DBG_RF_INIT, "===>%s   rfe_type=%d   band=%d   phy=%d\n",
		__func__, rfe_type, band, phy);

	if (rfe_type == 37 || rfe_type == 38) {
		if (band == BAND_ON_5G) {
			RF_DBG(rf, DBG_RF_INIT, "Set BB GPIO 5G Setting\n");
			/*Set BB GPIO Setting*/
			/*5G PathA*/
			halrf_gpio_setting(rf, 6, 0x1, false, HALRF_PAPE_RFM);
			halrf_gpio_setting(rf, 5, 0x1, false, HALRF_PAPE_RFM);
			/*5G PathB*/
			halrf_gpio_setting(rf, 17, 0x2, false, HALRF_PAPE_RFM);
			halrf_gpio_setting(rf, 4, 0x2, false, HALRF_PAPE_RFM);
		} else if (band == BAND_ON_6G) {
			RF_DBG(rf, DBG_RF_INIT, "Set BB GPIO 6G Setting\n");
			/*6G PathA*/
			halrf_gpio_setting(rf, 6, 0x1, false, HALRF_ZERO);
			halrf_gpio_setting(rf, 5, 0x1, false, HALRF_PAPE_RFM);
			/*6G PathB*/
			halrf_gpio_setting(rf, 17, 0x2, false, HALRF_ZERO);
			halrf_gpio_setting(rf, 4, 0x2, false, HALRF_PAPE_RFM);
		} else {
			/*Set GPIO Low PathA*/
			halrf_gpio_setting(rf, 6, 0x1, true, HALRF_ZERO);
			halrf_gpio_setting(rf, 5, 0x1, false, HALRF_ZERO);
			/*Set GPIO Low PathB*/
			halrf_gpio_setting(rf, 17, 0x2, true, HALRF_ZERO);
			halrf_gpio_setting(rf, 4, 0x2, false, HALRF_ZERO);
		}
		
		/*Set MAC GPIO Setting*/
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 0);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 1);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 2);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 3);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 4);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 5);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 6);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 7);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 11);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 14);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 17);

	} else if (rfe_type == 51 || rfe_type == 52) {
		/*Set BB GPIO Setting*/
		/*PathA=0x1*/
		halrf_gpio_setting(rf, 11, 0x1, false, HALRF_PAPE_RFM);
		halrf_gpio_setting(rf, 7, 0x1, true, HALRF_TRSW_RFM);
		halrf_gpio_setting(rf, 6, 0x1, false, HALRF_LNAON_RFM);
		/*PathB=0x2*/
		halrf_gpio_setting(rf, 1, 0x2, false, HALRF_PAPE_RFM);
		halrf_gpio_setting(rf, 2, 0x2, true, HALRF_TRSW_RFM);
		halrf_gpio_setting(rf, 3, 0x2, false, HALRF_LNAON_RFM);

		/*Set MAC GPIO Setting*/
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 0);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 1);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 2);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 3);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 6);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 7);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 11);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 14);
	} else if (rfe_type == 53 || rfe_type == 54) {
		/*Set BB GPIO Setting*/
		/*PathA=0x1*/
		halrf_gpio_setting(rf, 11, 0x1, false, HALRF_PAPE_RFM);
		halrf_gpio_setting(rf, 7, 0x1, true, HALRF_TRSW_RFM);
		halrf_gpio_setting(rf, 6, 0x1, true, HALRF_LNAON_RFM);
		/*PathB=0x2*/
		halrf_gpio_setting(rf, 1, 0x2, false, HALRF_PAPE_RFM);
		halrf_gpio_setting(rf, 2, 0x2, true, HALRF_TRSW_RFM);
		halrf_gpio_setting(rf, 3, 0x2, true, HALRF_LNAON_RFM);

		/*Set MAC GPIO Setting*/
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 0);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 1);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 2);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 3);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 6);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 7);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 11);
		halrf_set_gpio_func(rf, RTW_MAC_GPIO_WL_RFE_CTRL, 14);
	}
#endif
}

bool halrf_check_efem_8852a(struct rf_info *rf, enum phl_phy_idx phy)
{
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	bool chcenk_efem = false;

	/*2G FEM check*/
	if (rfe_type == 11 || rfe_type == 12 || rfe_type == 17 ||
	    rfe_type == 18 || rfe_type == 51 || rfe_type == 52 ||
	    rfe_type == 53 || rfe_type == 54) {
		chcenk_efem = true;
	}

	/*5G FEM check*/
	if (rfe_type == 9 || rfe_type == 10 || rfe_type == 11 ||
	    rfe_type == 12 || rfe_type == 15 || rfe_type == 16 ||
	    rfe_type == 17 || rfe_type == 18 || rfe_type == 37 || 
	    rfe_type == 38 || rfe_type == 51 || rfe_type == 52 ||
	    rfe_type == 53 || rfe_type == 54) {
		chcenk_efem = true;
	}

	/*6G FEM check*/
	if (rfe_type == 13 || rfe_type == 14 || rfe_type == 15 ||
	    rfe_type == 16 || rfe_type == 17 || rfe_type == 18 ||
	    rfe_type == 37 || rfe_type == 38 || rfe_type == 51 ||
	    rfe_type == 52 || rfe_type == 53 ||
	    rfe_type == 54) {
		chcenk_efem = true;
	}

	return chcenk_efem;
}

s8 halrf_get_ther_protected_threshold_8852a(struct rf_info *rf)
{
	u8 tmp_a, tmp_b, tmp;

	tmp_a = halrf_get_thermal(rf, RF_PATH_A);
	tmp_b = halrf_get_thermal(rf, RF_PATH_B);

	if (tmp_a > tmp_b)
		tmp = tmp_a;
	else
		tmp = tmp_b;

	if (tmp > 0x32)
		return -1;	/*Tx duty reduce*/
	else if (tmp < 0x31)
		return 1;	/*Tx duty up*/
	else 
		return 0;	/*Tx duty the same*/
}

s8 halrf_xtal_tracking_offset_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_xtal_info *xtal_trk = &rf->xtal_track;
	u8 thermal_a = 0xff, thermal_b = 0xff;
	u8 tmp_a, tmp_b, tmp;
	s8 xtal_ofst = 0;

	RF_DBG(rf, DBG_RF_XTAL_TRACK, "======>%s   phy=%d\n",
		__func__, phy);

	tmp_a = halrf_get_thermal(rf, RF_PATH_A);
	tmp_b = halrf_get_thermal(rf, RF_PATH_B);
	halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_A, &thermal_a, 1);
	halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_B, &thermal_b, 1);

	if (thermal_a == 0xff || thermal_b == 0xff ||
		thermal_a == 0x0 || thermal_b == 0x0) {
		RF_DBG(rf, DBG_RF_XTAL_TRACK, "======>%s PG ThermalA=%d ThermalB=%d\n",
			__func__, thermal_a, thermal_b);
		return 0;
	}

	if (tmp_a > tmp_b) {
		if (tmp_a > thermal_a) {
			tmp = tmp_a - thermal_a;
			if (tmp > DELTA_SWINGIDX_SIZE)
				tmp = DELTA_SWINGIDX_SIZE - 1;
			xtal_ofst = xtal_trk->delta_swing_xtal_table_idx_p[tmp];
		} else {
			tmp = thermal_a - tmp_a;
			if (tmp > DELTA_SWINGIDX_SIZE)
				tmp = DELTA_SWINGIDX_SIZE - 1;
			xtal_ofst = xtal_trk->delta_swing_xtal_table_idx_n[tmp];
		}
	} else {
		if (tmp_b > thermal_b) {
			tmp = tmp_b - thermal_b;
			if (tmp > DELTA_SWINGIDX_SIZE)
				tmp = DELTA_SWINGIDX_SIZE - 1;
			xtal_ofst = xtal_trk->delta_swing_xtal_table_idx_p[tmp];
		} else {
			tmp = thermal_b - tmp_b;
			if (tmp > DELTA_SWINGIDX_SIZE)
				tmp = DELTA_SWINGIDX_SIZE - 1;
			xtal_ofst = xtal_trk->delta_swing_xtal_table_idx_n[tmp];
		}
	}

	RF_DBG(rf, DBG_RF_XTAL_TRACK, "PG ThermalA=%d   ThermalA=%d\n",
		thermal_a, tmp_a);

	RF_DBG(rf, DBG_RF_XTAL_TRACK, "PG ThermalB=%d   ThermalB=%d\n",
		thermal_b, tmp_b);

	RF_DBG(rf, DBG_RF_XTAL_TRACK, "xtal_ofst[%d]=%d\n",
		tmp, xtal_ofst);

	return xtal_ofst;
}

#endif
