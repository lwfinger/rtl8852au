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

void halrf_set_lo_8852a(struct rf_info *rf, bool is_on, enum rf_path path)
{
	if (is_on) {
		halrf_wrf(rf, path, 0x58, BIT(1), 0x1);
		halrf_rf_direct_cntrl_8852a(rf, path, false);
		halrf_wrf(rf, path, 0x1, 0x3f, 0x3f);
		halrf_wrf(rf, path, 0x0, MASKRFMODE, 0x2);
	} else {
		halrf_wrf(rf, path, 0x58, BIT(1), 0x0);
		halrf_rf_direct_cntrl_8852a(rf, path, true);
		halrf_wrf(rf, path, 0x0, MASKRF, 0x10000);
		halrf_wrf(rf, path, 0x0, MASKRFMODE, 0x3);
	}
}

void halrf_fcs_set_lo_8852a(struct rf_info *rf, bool is_on)
{
	if (is_on) {
		halrf_tssi_disable(rf, HW_PHY_0);
		halrf_wrf(rf, RF_PATH_A, 0x58, BIT(1), 0x1);
		halrf_rf_direct_cntrl_8852a(rf, RF_PATH_A, false);
		halrf_wrf(rf, RF_PATH_A, 0x1, 0xff, 0x2a);
		halrf_wrf(rf, RF_PATH_A, 0x0, MASKRFMODE, 0x2);

		halrf_wrf(rf, RF_PATH_B, 0x58, BIT(1), 0x1);
		halrf_rf_direct_cntrl_8852a(rf, RF_PATH_B, false);
		halrf_wrf(rf, RF_PATH_B, 0x1, 0xff, 0x2a);
		halrf_wrf(rf, RF_PATH_B, 0x0, MASKRFMODE, 0x2);
	} else {
		halrf_wrf(rf, RF_PATH_A, 0x58, BIT(1), 0x0);
		halrf_rf_direct_cntrl_8852a(rf, RF_PATH_A, true);
		halrf_wrf(rf, RF_PATH_A, 0x0, MASKRFMODE, 0x3);

		halrf_wrf(rf, RF_PATH_B, 0x58, BIT(1), 0x0);
		halrf_rf_direct_cntrl_8852a(rf, RF_PATH_B, true);
		halrf_wrf(rf, RF_PATH_B, 0x0, MASKRFMODE, 0x3);
		halrf_tssi_enable(rf, HW_PHY_0);
	}
}

void halrf_rf_direct_cntrl_8852a(struct rf_info *rf, enum rf_path path, bool is_bybb)
{
	if (is_bybb)
		halrf_wrf(rf, path, 0x5, BIT(0), 0x1);
	else
		halrf_wrf(rf, path, 0x5, BIT(0), 0x0);
}

void halrf_lo_test_8852a(struct rf_info *rf, bool is_on, enum rf_path path)
{
	switch (path) {
		case RF_PATH_A:
			halrf_set_lo_8852a(rf, is_on, RF_PATH_A);
			halrf_set_lo_8852a(rf, false, RF_PATH_B);
			break;
		case RF_PATH_B:
			halrf_set_lo_8852a(rf, false, RF_PATH_A);
			halrf_set_lo_8852a(rf, is_on, RF_PATH_B);
			break;
		case RF_PATH_AB:
			halrf_set_lo_8852a(rf, is_on, RF_PATH_A);
			halrf_set_lo_8852a(rf, is_on, RF_PATH_B);
			break;
		default:
			break;
	}
}

u8 halrf_kpath_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx) {

	RF_DBG(rf, DBG_RF_RFK, "[RFK]dbcc_en: %x,  PHY%d\n", rf->hal_com->dbcc_en, phy_idx);

	if (!rf->hal_com->dbcc_en) {
		return RF_AB;
	} else {
		if (phy_idx == HW_PHY_0)
			return RF_A;
		else
			return RF_B;
	}
}

void _rx_dck_info_8852a(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_afe)
{
	struct halrf_rx_dck_info *rx_dck = &rf->rx_dck;

	rx_dck->is_afe = is_afe;
	rx_dck->loc[path].cur_band = rf->hal_com->band[phy].cur_chandef.band;
	rx_dck->loc[path].cur_bw = rf->hal_com->band[phy].cur_chandef.bw;
	rx_dck->loc[path].cur_ch = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_RFK, "[RX_DCK] ==== S%d RX DCK (%s / CH%d / %s / by %s)====\n", path,
		rx_dck->loc[path].cur_band == 0 ? "2G" :
		(rx_dck->loc[path].cur_band == 1 ? "5G" : "6G"),
		rx_dck->loc[path].cur_ch,
	       rx_dck->loc[path].cur_bw == 0 ? "20M" :
	       (rx_dck->loc[path].cur_bw == 1 ? "40M" : "80M"),
	       	rx_dck->is_afe ? "AFE" : "RFC");	
}

void halrf_set_rx_dck_8852a(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_afe)
{
	u8 i, phy_map;
	u32 ori_val;

	phy_map = (BIT(phy) << 4) | BIT(path);

	_rx_dck_info_8852a(rf, phy, path, is_afe);

	if (rf->hal_com->cv == CAV)
		ori_val = halrf_rreg(rf, 0x4ca0 + (path << 8), MASKDWORD);
	else
		ori_val = halrf_rreg(rf, 0x12a0 + (path << 13), MASKDWORD);

	if (is_afe) {
		if (rf->hal_com->cv == CAV) {
			halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x1); /*debug en*/
			halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(19), 0x1);
			halrf_wreg(rf, 0x4ca0 + (path << 8), 0x00070000, 0x3); /*ADC 320M*/
			halrf_wreg(rf, 0x4cd8 + (path << 8), BIT(5) | BIT(4), 0x3); /*offset manual en*/
			halrf_wreg(rf, 0x4cd8 + (path << 8), BIT(7) | BIT(6), 0x3); /*avg 0:16; 1:32; 2:64; 3:128*/
			halrf_wreg(rf, 0x0b0c, 0x0f000000, 0x3); /*adc en*/
			halrf_wreg(rf, 0x0b2c, BIT(30), 0x0); /*adc clk*/
			halrf_wreg(rf, 0x0b2c, BIT(18) | BIT(17) | BIT(16), 0x1); /*connect with RXBB*/
		} else {
			halrf_wreg(rf, 0x12b8 + (path << 13), BIT(30), 0x1); /*debug en*/
			halrf_wreg(rf, 0x12a0 + (path << 13), BIT(19), 0x1);
			halrf_wreg(rf, 0x12a0 + (path << 13), 0x00070000, 0x3); /*ADC 320M*/
			halrf_wreg(rf, 0x12d8 + (path << 13), BIT(5) | BIT(4), 0x3); /*offset manual en*/
			halrf_wreg(rf, 0x12d8 + (path << 13), BIT(7) | BIT(6), 0x3); /*avg 0:16; 1:32; 2:64; 3:128*/
			halrf_wreg(rf, 0x030c, 0x0f000000, 0x3); /*adc en*/
			halrf_wreg(rf, 0x032c, BIT(30), 0x0); /*adc clk*/
			halrf_wreg(rf, 0x032c, BIT(22), 0x0); /*filter reset*/
			halrf_wreg(rf, 0x032c, BIT(22), 0x1); /*filter reset release*/
			halrf_wreg(rf, 0x032c, BIT(18) | BIT(17) | BIT(16), 0x1); /*connect with RXBB*/
		}
	}

	halrf_wrf(rf, path, 0x94, 0x000FC, 0x3f); /*[7:2] bk_cycle*/
	halrf_wrf(rf, path, 0x93, BIT(3), is_afe); /*0: from RFC; 1: from AFE*/

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_RXDCK, RFK_ONESHOT_START);

	halrf_wrf(rf, path, 0x92, BIT(0), 0x0);
	halrf_wrf(rf, path, 0x92, BIT(0), 0x1);

	for (i = 0; i < 30; i++) /*delay 600us*/
		halrf_delay_us(rf, 20);

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_RXDCK, RFK_ONESHOT_STOP);

	halrf_wrf(rf, path, 0x92, BIT(0), 0x0);

	if (is_afe) {
		if (rf->hal_com->cv == CAV) {
			halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x0);
			halrf_wreg(rf, 0x4ca0 + (path << 8), MASKDWORD, ori_val);
		} else {
			halrf_wreg(rf, 0x12b8 + (path << 13), BIT(30), 0x0);
			halrf_wreg(rf, 0x12a0 + (path << 13), MASKDWORD, ori_val);
		}
	}
}

bool halrf_rx_dck_check_8852a(struct rf_info *rf, enum rf_path path)
{
	u8 addr;
	bool is_fail = false;

	if (halrf_rreg(rf, 0xc400 + path * 0x1000, 0xF0000) == 0x0)
		 return is_fail = true;
	else if (halrf_rreg(rf, 0xc400 + path * 0x1000, 0x0F000) == 0x0)
		return is_fail = true;
	else if (halrf_rreg(rf, 0xc440 + path * 0x1000, 0xF0000) == 0x0)
		return is_fail = true;
	else if (halrf_rreg(rf, 0xc440 + path * 0x1000, 0x0F000) == 0x0)
		return is_fail = true;
	else {
		for (addr = 0x0; addr < 0x20; addr++) {
			if (halrf_rreg(rf, 0xc400 + path * 0x1000 + addr * 4, 0x00FC0) == 0x0)
				return is_fail = true;
		}

		for (addr = 0x0; addr < 0x20; addr++) {
			if (halrf_rreg(rf, 0xc400 + path * 0x1000 + addr * 4, 0x0003F) == 0x0)
				return is_fail = true;
		}
	}

	return is_fail;
}

void halrf_rx_dck_8852a(struct rf_info *rf, enum phl_phy_idx phy, bool is_afe) 
{
	u8 path, kpath, dck_tune;
	u32 rf_reg5;

	RF_DBG(rf, DBG_RF_RXDCK, "[RX_DCK] ****** RXDCK Start (Ver: 0x%x, Cv: %d) ******\n",
		RXDCK_VER_8852A, rf->hal_com->cv);

	kpath = halrf_kpath_8852a(rf, phy);

	for (path = 0; path < 2; path++) {
		rf_reg5 = halrf_rrf(rf, path, 0x5, MASKRF);
		dck_tune = (u8)halrf_rrf(rf, path, 0x92, BIT(1));
		if (kpath & BIT(path)) {
			if (rf->is_tssi_mode[path]) {
				if (rf->hal_com->cv == CAV)
					halrf_wreg(rf, 0x7c18 + (path << 8), BIT(30), 0x1); /*TSSI pause*/
				else
					halrf_wreg(rf, 0x5818 + (path << 13), BIT(30), 0x1); /*TSSI pause*/
			}

			halrf_wrf(rf, path, 0x5, BIT(0), 0x0);
			halrf_wrf(rf, path, 0x92, BIT(1), 0x0);
//			halrf_wrf(rf, path, 0x8f, BIT(11) | BIT(10), 0x1); /*EN_TIA_IDAC_LSB[1:0]*/
			halrf_wrf(rf, path, 0x00, MASKRFMODE, RF_RX);
			halrf_set_rx_dck_8852a(rf, phy, path, is_afe);
#if 0
			if (halrf_rx_dck_check_8852a(rf, path)) {
				RF_DBG(rf, DBG_RF_RFK, "[RX_DCK] S%d RX_DCK value = 0 happen!!!\n", path);
				halrf_wrf(rf, path, 0x8f, BIT(11) | BIT(10), 0x2); /*EN_TIA_IDAC_LSB[1:0]*/
				halrf_set_rx_dck_8852a(rf, phy, path, is_afe);
			}
#endif
			halrf_wrf(rf, path, 0x92, BIT(1), dck_tune);
			halrf_wrf(rf, path, 0x5, MASKRF, rf_reg5);

			if (rf->is_tssi_mode[path]) {
				if (rf->hal_com->cv == CAV)
					halrf_wreg(rf, 0x7c18 + (path << 8), BIT(30), 0x0); /*TSSI resume*/
				else
					halrf_wreg(rf, 0x5818 + (path << 13), BIT(30), 0x0); /*TSSI resume*/
			}
		}
	}
}

void halrf_rx_dck_onoff_8852a(struct rf_info *rf, bool is_enable)
{
	u8 path;

	for (path = 0; path < 2; path++) {
		halrf_wrf(rf, path, 0x93, BIT(0), !is_enable);
		if (!is_enable) {
			halrf_wrf(rf, path, 0x92, 0xFFC00, 0x220); /*[19:10]*/
			halrf_wrf(rf, path, 0x93, 0xFFC00, 0x220); /*[19:10]*/
		}
	}
}

void halrf_rck_8852a(struct rf_info *rf, enum rf_path path)
{
	u8 cnt = 0;
	u32 rf_reg5, rck_val = 0;

	RF_DBG(rf, DBG_RF_RFK, "[RCK] ====== S%d RCK ======\n", path);

	rf_reg5 = halrf_rrf(rf, path, 0x5, MASKRF);

	halrf_wrf(rf, path, 0x5, BIT(0), 0x0);
	halrf_wrf(rf, path, 0x00, MASKRFMODE, RF_RX);

	RF_DBG(rf, DBG_RF_RFK, "[RCK] RF0x00 = 0x%x\n", halrf_rrf(rf, path, 0x00, MASKRF));

	/*RCK trigger*/
	//halrf_wrf(rf, path, 0x1b, BIT(6), 0x0);
	halrf_wrf(rf, path, 0x1b, MASKRF, 0x00240);

	while ((halrf_rrf(rf, path, 0x1c, BIT(3)) == 0x00) && (cnt < 10)) {
		halrf_delay_us(rf, 2);
	cnt++;
	}

	rck_val = halrf_rrf(rf, path, 0x1b, 0x07C00); /*[14:10]*/
	halrf_wrf(rf, path, 0x1b, MASKRF, rck_val);

	/*RCK_ADC_OFFSET*/
	halrf_wrf(rf, path, 0x1d, 0x03E00, 0x4); /*RCK_ADC_OFFSET[4:0]*/
	//halrf_wrf(rf, path, 0x1b, BIT(6), 0x0);  /*avoid LPS re-K*/

	halrf_wrf(rf, path, 0xf0, BIT(1), 0x1);
	halrf_wrf(rf, path, 0xf0, BIT(1), 0x0);

	halrf_wrf(rf, path, 0x5, MASKRF, rf_reg5);

	RF_DBG(rf, DBG_RF_RFK, "[RCK] RF 0x1b / 0x1c / 0x1d = 0x%x / 0x%x / 0x%x\n",
	       halrf_rrf(rf, path, 0x1b, MASKRF),
	       halrf_rrf(rf, path, 0x1c, MASKRF),
	       halrf_rrf(rf, path, 0x1d, MASKRF));
}

void iqk_backup_8852a(struct rf_info *rf, enum rf_path path) 
{
	return;
}

void halrf_bf_config_rf_8852a(struct rf_info *rf)
{
	u8 i;

	for (i = 0; i < 2; i++) {
		halrf_wrf(rf, (enum rf_path)i, 0xef, BIT(19), 0x1);
		halrf_wrf(rf, (enum rf_path)i, 0x33, 0xf, 0x1);
		halrf_wrf(rf, (enum rf_path)i, 0x3e, MASKRF, 0x00001);
		halrf_wrf(rf, (enum rf_path)i, 0x3f, MASKRF, 0xb2120);
		halrf_wrf(rf, (enum rf_path)i, 0x33, 0xf, 0x2);
		halrf_wrf(rf, (enum rf_path)i, 0x3e, MASKRF, 0x00001);
		halrf_wrf(rf, (enum rf_path)i, 0x3f, MASKRF, 0xfe124);
		halrf_wrf(rf, (enum rf_path)i, 0x33, 0xf, 0x3);
		halrf_wrf(rf, (enum rf_path)i, 0x3e, MASKRF, 0x00001);
		halrf_wrf(rf, (enum rf_path)i, 0x3f, MASKRF, 0x30d7c);
		halrf_wrf(rf, (enum rf_path)i, 0x33, 0xf, 0xa);
		halrf_wrf(rf, (enum rf_path)i, 0x3e, MASKRF, 0x00001);
		halrf_wrf(rf, (enum rf_path)i, 0x3f, MASKRF, 0x30d7e);
		halrf_wrf(rf, (enum rf_path)i, 0x33, 0xf, 0xb);
		halrf_wrf(rf, (enum rf_path)i, 0x3e, MASKRF, 0x00001);
		halrf_wrf(rf, (enum rf_path)i, 0x3f, MASKRF, 0x30d7d);
		halrf_wrf(rf, (enum rf_path)i, 0xef, BIT(19), 0x0);
	}
}

bool halrf_rfk_reg_check_8852a(struct rf_info *rf)
{
	u32 i, reg, temp;
	bool fail = false;

	for (i = 0; i < 2048; i++) {
		reg = 0x8000 + i*4;
		if (((reg >= 0x8000 && reg < 0x8300) || 
			(reg >= 0x8500 && reg < 0x90c0) || 
			(reg >= 0x9100 && reg < 0x94c0) ||
			(reg >= 0x9500 && reg < 0x9640) ||
			(reg >= 0x9700 && reg < 0x9840) ||
			(reg >= 0x9f00)) &&
			((reg != 0x80fc) && (reg != 0x81fc) &&
			(reg != 0x82fc) && (reg != 0x9ffc) &&
			(reg != 0x81b4) && (reg != 0x82b4))) {
			temp = halrf_rreg(rf, reg, MASKDWORD);
			if (rf->rfk_reg[i] != temp) {
				RF_DBG(rf, DBG_RF_RFK,
					"[RFK]reg 0x%x b 0x%x/a 0x%x\n",
					reg,
					rf->rfk_reg[i],
					temp);
#if 0
				for (t =0; t < 10; t++) {
					temp = halrf_rreg(rf, reg, MASKDWORD);
					RF_DBG(rf, DBG_RF_RFK, "[RFK]reg 0x%x mismatch: before 0x%x/after 0x%x\n", reg, rf->rfk_reg[i], temp);
					halrf_delay_ms(rf, 1);
				}
#endif
				fail = true;
				rf->rfk_check_fail_count++;
			}
		}
	}
	return fail;
}

#if 0
bool halrf_rfk_reg_restore_check_8852a(struct rf_info *rf)
{
	u32 cnt = 0, c, reg, i;
	bool fail = false;

	/*restore*/
	rf->rfk_check_fail_count = 0;

#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, false, HW_PHY_0);
#else
	PHL_INFO("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif

	for (c = 0; c < 100; c++) {
		while (cnt < 1000) {
			cnt++;
			halrf_wreg(rf, 0x8080, BIT(2), 0x1);
			if (halrf_rreg(rf, 0x8080, BIT(2)) == 0x1)
				break;
	    	}	

		for (i = 0; i < 2048; i++) {
			reg = 0x8000 + i*4;
			halrf_wreg(rf, reg, MASKDWORD, rf->rfk_reg[i]);
		}

	  	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000000);	
	  	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00010003);
	  	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
	  	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000000);	
	  	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00010003);	
	  	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	  	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000004);
	  	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);	
	  	halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000000);
	  	halrf_wreg(rf, 0x0a70, 0x3e0, 0x0);
		/*check*/
		RF_DBG(rf, DBG_RF_RFK, "[RFK]c = %d\n", c);
		halrf_wreg(rf, 0x0a70, 0x3e0, 0x1f);
		halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000007);	
		halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000110);
		halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00010001);
		halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00010001);
		if (halrf_rfk_reg_check_8852a(rf) == true) {
			fail = true;
		}
		halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000000);	
		halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00010003);
		halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
		halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000000);	
		halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00010003);	
		halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
		halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000004);
		halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);	
		halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000000);
		halrf_wreg(rf, 0x0a70, 0x3e0, 0x0);
	}
	RF_DBG(rf, DBG_RF_RFK, "[RFK]fail count = %d\n", rf->rfk_check_fail_count);
#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, true, HW_PHY_0);
#else
	PHL_INFO("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	return fail;
}
#endif

void halrf_rfk_reg_backup_8852a(struct rf_info *rf)
{
	u32 i, reg;

#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, false, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	halrf_wreg(rf, 0x0a70, 0x3e0, 0x1f);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000005);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000110);
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00010001);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00010001);
	for (i = 0; i < 2048; i++) {
		reg = 0x8000 + i*4;
		rf->rfk_reg[i] = halrf_rreg(rf, reg, MASKDWORD);
		RF_DBG(rf, DBG_RF_RFK,
			"[RFK]bk reg 0x%x = 0x%x\n",
			reg,
			rf->rfk_reg[i]);
	}
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00010003);
	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00010003);
	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000004);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x0a70, 0x3e0, 0x0);
#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, true, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
}

bool halrf_rfk_reg_check_fail_8852a(struct rf_info *rf)
{
	bool fail = false;

#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, false, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	rf->rfk_check_fail_count = 0;
	halrf_wreg(rf, 0x0a70, 0x3e0, 0x1f);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000005);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000110);
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00010001);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00010001);
	fail = halrf_rfk_reg_check_8852a(rf);
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00010003);	
	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00010003);
	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000004);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x0a70, 0x3e0, 0x0);
	RF_DBG(rf, DBG_RF_RFK,
		"[RFK]fail count = %d\n",
		rf->rfk_check_fail_count);
#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, true, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	return fail;
}

static u32 r_d[] = {0x18, 0xb2, 0xc5};
static u32 r_m[] = {0xfffff, 0xfffff, 0x8000};

void halrf_rfc_reg_backup_8852a(struct rf_info *rf)
{
	u8 i;

	for (i = 0; i < 3; i++) {
		rf->rfc_reg[0][i] = halrf_rrf(rf, RF_PATH_A, r_d[i], r_m[i]);
		rf->rfc_reg[1][i] = halrf_rrf(rf, RF_PATH_B, r_d[i], r_m[i]);  
	}
}

bool halrf_rfc_reg_check_fail_8852a(struct rf_info *rf)
{
	u8 i;
	u32 temp;
	bool fail = false;

	for (i = 0; i < 3; i++) {
		temp = halrf_rrf(rf, RF_PATH_A, r_d[i], r_m[i]);
		if (rf->rfc_reg[0][i] !=temp) {
			RF_DBG(rf, DBG_RF_RFK,
				"[RFK]S0 0x%x mask=0x%x, old=0x%x, new=0x%x\n",
				r_d[i],
				r_m[i],
				rf->rfc_reg[0][i],
				temp);
			fail = true;
		}
	}
	for (i = 0; i < 3; i++) {
		temp = halrf_rrf(rf, RF_PATH_B, r_d[i], r_m[i]);
		if (rf->rfc_reg[1][i] !=temp) {
			RF_DBG(rf, DBG_RF_RFK,
				"[RFK]S1 0x%x mask=0x%x, old=0x%x, new=0x%x\n",
				r_d[i],
				r_m[i],
				rf->rfc_reg[1][i],
				temp);
			fail = true;
		}
	}
	return fail;
}

void halrf_disconnect_notify_8852a(struct rf_info *rf, struct rtw_chan_def *chandef  ) {

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 path, ch;
	
	RF_DBG(rf, DBG_RF_RFK, "[IQK]===>%s\n", __func__);
	/*[IQK disconnect]*/
	for (ch = 0; ch < 2; ch++) {
		for (path = 0; path < KPATH; path++) {
			if (iqk_info->iqk_mcc_ch[ch][path] == chandef->center_ch)
				iqk_info->iqk_mcc_ch[ch][path] = 0x0;
		}

	}
}

bool halrf_check_mcc_ch_8852a(struct rf_info *rf, struct rtw_chan_def *chandef) {

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 path, ch;

	bool check = false;
	RF_DBG(rf, DBG_RF_RFK, "[IQK]===>%s, center_ch(%d)\n", __func__, chandef->center_ch);
	/*[IQK check_mcc_ch]*/
	for (ch = 0; ch < 2; ch++) {
		for (path = 0; path < KPATH; path++) {
			if (iqk_info->iqk_mcc_ch[ch][path] == chandef->center_ch) {
				check = true;
				return check;
			}
		}
	}
	return check;
}

void halrf_fw_ntfy_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx) {
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 i = 0x0;
	u32 data_to_fw[5] = {0};
	u16 len = (u16) (sizeof(data_to_fw) / sizeof(u32))*4;
	
	data_to_fw[0] = (u32) iqk_info->iqk_mcc_ch[0][0];
	data_to_fw[1] = (u32) iqk_info->iqk_mcc_ch[0][1];
	data_to_fw[2] = (u32) iqk_info->iqk_mcc_ch[1][0];
	data_to_fw[3] = (u32) iqk_info->iqk_mcc_ch[1][1];
	data_to_fw[4] = rf->hal_com->band[phy_idx].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_RFK, "[IQK] len = 0x%x\n", len);
	for (i =0; i < 5; i++)
		RF_DBG(rf, DBG_RF_RFK, "[IQK] data_to_fw[%x] = 0x%x\n", i, data_to_fw[i]);

	halrf_fill_h2c_cmd(rf, len, FWCMD_H2C_GET_MCCCH, 0xa, H2CB_TYPE_DATA, (u32 *) data_to_fw);	

	return;
}


static u32 backup_mac_reg_8852a[] = {0x0};
static u32 backup_bb_reg_8852a[] = {0x2344};
static u32 backup_rf_reg_8852a[] = {0xef, 0xde, 0x0, 0x1e, 0x2, 0x85, 0x90, 0x5};
//static u32 backup_rf_reg_8852a[] = {0xde,0x5};

static struct halrf_iqk_ops iqk_ops= {
    .iqk_kpath = halrf_kpath_8852a,
    .iqk_mcc_page_sel = iqk_mcc_page_sel_8852ab,
    .iqk_get_ch_info = iqk_get_ch_info_8852ab,
    .iqk_preset = iqk_preset_8852ab,
    .iqk_macbb_setting = iqk_macbb_setting_8852ab,
    .iqk_start_iqk = iqk_start_iqk_8852ab,
    .iqk_restore = iqk_restore_8852ab,
    .iqk_afebb_restore = iqk_afebb_restore_8852ab,
};

struct rfk_iqk_info rf_iqk_hwspec_8852a = {
  	.rf_iqk_ops = &iqk_ops,
	.rf_max_path_num = 2,
	.rf_iqk_version = iqk_version,
	.rf_iqk_ch_num = 2,
	.rf_iqk_path_num = 2,

#if 0
	.backup_mac_reg = backup_mac_reg_8852a,
	.backup_mac_reg_num = ARRAY_SIZE(backup_mac_reg_8852a),
#else
	.backup_mac_reg = backup_mac_reg_8852a,
	.backup_mac_reg_num = 0,
#endif

    	.backup_bb_reg = backup_bb_reg_8852a,
    	.backup_bb_reg_num = ARRAY_SIZE(backup_bb_reg_8852a),
    	.backup_rf_reg = backup_rf_reg_8852a,
    	.backup_rf_reg_num = ARRAY_SIZE(backup_rf_reg_8852a),
};

void halrf_fcs_init_8852a(struct rf_info *rf)
{
	rf->pre_chl_idx = 6;
	rf->pre_ther_idx = 4;
}

#define fcs_hw_tx_cnt 1000
void halrf_fcs_hw_tx_8852a(struct rf_info *rf, bool enable)
{
	struct rf_pmac_tx_info tx_info = {0};

	RF_DBG(rf, DBG_RF_RFK, "======> %s\n", __func__);

	tx_info.ppdu = T_HT_MF;
	tx_info.mcs = 7;
	tx_info.bw = 0;
	tx_info.nss = 1;
	tx_info.gi = 1;
	tx_info.txagc_cw = 0;
	tx_info.dbm = 64;
	tx_info.cnt = fcs_hw_tx_cnt;
	tx_info.time = 20; 
	tx_info.period = 100;
	tx_info.length = 3000; /*0*/

	halrf_set_pmac_tx(rf, HW_PHY_0, RF_PATH_AB, &tx_info, enable, false);
}

u8 halrf_thermal_index_cal_8852a(struct rf_info *rf)
{
	u8 ther_now, ther_index = 0;
	u8 i;

	ther_now = halrf_get_thermal_8852a(rf, RF_PATH_A);
	for (i = 0; i < 4; i ++) {
		if (ther_now < (rf->ther_init + (FCS_THER_STEP * i))) {
			ther_index = i;
			break;
		}
	}

	if (i == 4)
		ther_index = 3;

	return ther_index;
}

bool halrf_rfk_chl_thermal_8852a(struct rf_info *rf, u8 chl_idx, u8 ther_idx)
{
	u8 ther_now = 0, ther_th;
//	u8 i;
	u32 c = 0, cc = 0, t, tx_cnt, temp;
	bool fail = true;
	static bool firstrun = true;
	u32 start_time = 0;
	u32 temp0c3c, temp2344;


	if ((chl_idx > 5) || (ther_idx > 3))
		return false;

	temp0c3c = halrf_rreg(rf, 0x0c3c, MASKDWORD);
	temp2344 = halrf_rreg(rf, 0x2344, MASKDWORD);
	RF_DBG(rf, DBG_RF_RFK, "[FCS]start cold boot calibration!!!!\n");
	RF_DBG(rf, DBG_RF_RFK, "[FCS]chl_index = %d, ther_index =%d\n", chl_idx, ther_idx);
	RF_DBG(rf, DBG_RF_RFK, "[FCS]RF0x18 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x18, MASKRF));
//	if (ther_idx == 0 && chl_idx == 0) {
	if (firstrun == true) {
		firstrun = false;
		rf->ther_init = halrf_get_thermal_8852a(rf, RF_PATH_A);
	}
	RF_DBG(rf, DBG_RF_RFK, "[FCS]ther_init =%d\n", rf->ther_init);
	if (ther_idx != 0) {
		ther_th = rf->ther_init + ther_idx * FCS_THER_STEP;
#if 0
		halrf_fcs_set_lo_8852a( rf, true);
		start_time = _os_get_cur_time_ms();
//		while (c < FCS_TIMEOUT) {
		while ((_os_get_cur_time_ms() - start_time) < 10000) {

			c++;
			ther_now = halrf_get_thermal_8852a(rf, RF_PATH_A);
			if (ther_now > ther_th) {
				fail = false;
				break;
			}

			for (i =0; i < 10; i++)
				halrf_delay_us(rf, 10);


		}
		halrf_fcs_set_lo_8852a( rf, false);
#endif

#if 1
		start_time = _os_get_cur_time_ms();
		halrf_tmac_tx_pause(rf, HW_PHY_0, true);
		halrf_wreg(rf, 0x0c3c, BIT(9), 0x1);
		halrf_wreg(rf, 0x2344, BIT(31), 0x1);
		//bb reset to prevent CCAing
		halrf_wreg(rf, 0x0704, BIT(1), 0x0);
		halrf_wreg(rf, 0x0704, BIT(1), 0x1);
		halrf_delay_ms(rf, 15);
		while ((_os_get_cur_time_ms() - start_time) < 40000) {
			c++;
			//reset tx_cnt
			halrf_wreg(rf, 0x0730, BIT(0), 0x1);
			halrf_wreg(rf, 0x0730, BIT(0), 0x0);
			tx_cnt= halrf_rreg(rf, 0x1a40, 0xffff);
			halrf_fcs_hw_tx_8852a(rf, true);
			halrf_delay_ms(rf, 15);
			cc = 0;
//			while (halrf_rreg(rf, 0x1a40, 0xffff) - tx_cnt < fcs_hw_tx_cnt) {
			while (cc < 1000) {
				cc++;
				temp = halrf_rreg(rf, 0x1a40, 0xffff);
				if ((temp == 0) || (temp == tx_cnt))
					break;
				tx_cnt = temp;
				halrf_delay_ms(rf, 15);
			}
			RF_DBG(rf, DBG_RF_RFK, "[FCS]tx_cnt =%d, cc = %d\n", tx_cnt, cc);
			halrf_fcs_hw_tx_8852a(rf, false);

			ther_now = halrf_get_thermal_8852a(rf, RF_PATH_A);
			RF_DBG(rf, DBG_RF_RFK, "[FCS]ther_now = %d, ther_th =%d\n", ther_now, ther_th);
			if (ther_now > ther_th) {
				fail = false;
				break;
			}
		}
		halrf_tx_mode_switch(rf, HW_PHY_0, 0);
		halrf_tmac_tx_pause(rf, HW_PHY_0, false);
		halrf_wreg(rf, 0x0c3c, MASKDWORD, temp0c3c);
		halrf_wreg(rf, 0x2344, MASKDWORD, temp2344);
#endif
		RF_DBG(rf, DBG_RF_RFK, "[FCS]ther_now = %d, ther_th =%d\n", ther_now, ther_th);
		RF_DBG(rf, DBG_RF_RFK, "[FCS]c =%d\n", c);

//		if (c == FCS_TIMEOUT)
//			fail = true;
	} else {
		fail = false;
	}

	if (fail) {
		RF_DBG(rf, DBG_RF_RFK, "[FCS]T is limited\n");	
	} else {
		halrf_chl_rfk_trigger(rf,HW_PHY_0, true);
		halrf_fast_chl_sw_backup(rf, chl_idx, ther_idx);
		t = chl_idx*4 +ther_idx;
		rf->fcs_rfk_ok_map = (rf->fcs_rfk_ok_map & (~BIT(t))) |(1<< t);
		RF_DBG(rf, DBG_RF_RFK, "[FCS]fcs_rfk_ok_map = 0x%x\n", rf->fcs_rfk_ok_map);
		RF_DBG(rf, DBG_RF_RFK, "[FCS]ther after chl_rfk = %d\n", halrf_get_thermal_8852a(rf, RF_PATH_A));
	}
	return fail;
}

void halrf_rfk_recovery_chl_thermal_8852a(struct rf_info *rf, u8 chl_idx)
{
	u8 ther_idx;
	u32 t;
	u8 c = 0;
	RF_DBG(rf, DBG_RF_RFK, "[FCS]start recover!!!!\n");
	RF_DBG(rf, DBG_RF_RFK, "[FCS]fcs_rfk_ok_map = 0x%x\n", rf->fcs_rfk_ok_map);
	ther_idx = halrf_thermal_index_cal_8852a(rf);
	RF_DBG(rf, DBG_RF_RFK, "[FCS]pre_chl_idx = %d, pre_ther_idx = %d\n", rf->pre_chl_idx, rf->pre_ther_idx);	
	RF_DBG(rf, DBG_RF_RFK, "[FCS]chl_idx = %d, ther_idx= %d\n", chl_idx, ther_idx);	
	t = chl_idx*4 + ther_idx;
	while (((rf->fcs_rfk_ok_map & (1 << t)) == 0) && (c < 5)) {
		RF_DBG(rf, DBG_RF_RFK, "[FCS]can not find valid RFK value, ther_idx= %d\n", ther_idx);
		if (ther_idx !=0)
			ther_idx -= 1;
		t = chl_idx*4 + ther_idx;
		c++;
	}

	if ((chl_idx == rf->pre_chl_idx) && (ther_idx == rf->pre_ther_idx)) {
		RF_DBG(rf, DBG_RF_RFK, "[FCS]No recover because of the same condition\n");
	} else {
		halrf_fast_chl_sw_reload(rf, chl_idx, ther_idx);
		rf->pre_chl_idx = chl_idx;
		rf->pre_ther_idx = ther_idx;
		RF_DBG(rf, DBG_RF_RFK, "[FCS]recover RFK, chl_idx = %d, ther_idx= %d\n", chl_idx, ther_idx);
	}
}

void halrf_lck_check_a_8852a(struct rf_info *rf)
{
	u8 c = 0;
	u8 i = 0;
	u32 tmp18;

	tmp18 = halrf_rrf(rf, RF_PATH_A, 0x18, MASKRF);
	/*check SYN*/
	for (i = 0; i < 4; i++)
		halrf_delay_us(rf, 10);
	if (halrf_rrf(rf, RF_PATH_A, 0xa0, 0xc) == 0x3) {
		while (c < 3) {
			c++;
			if (halrf_rrf(rf, RF_PATH_A, 0xc5, BIT(15)) == 0x1) {
				break;
			} else {
				halrf_wrf(rf, RF_PATH_A, 0xd3, BIT(8), 0x1);
				halrf_wrf(rf, RF_PATH_A, 0xa0, MASKRF, 0x4c3);
				halrf_wrf(rf, RF_PATH_A, 0xa0, MASKRF, 0x4cf);
				for (i = 0; i < 4; i++)
					halrf_delay_us(rf, 10);
				halrf_wrf(rf, RF_PATH_A, 0x18, MASKRF, tmp18);
				for (i = 0; i < 20; i++)
					halrf_delay_us(rf, 10);
				halrf_wrf(rf, RF_PATH_A, 0xd3, BIT(8), 0x0);
            			}
		}
		if (c == 3)
			RF_DBG(rf, DBG_RF_RFK, "check LCK\n");
	}
}

void halrf_lck_check_b_8852a(struct rf_info *rf)
{
	u8 c = 0;
	u8 i = 0;
	u32 tmp18;

	tmp18 = halrf_rrf(rf, RF_PATH_B, 0x18, MASKRF);
	/*check SYN*/
	for (i = 0; i < 4; i++)
		halrf_delay_us(rf, 10);
	if (halrf_rrf(rf, RF_PATH_B, 0xa0, 0xc) == 0x3) {
		while (c < 3) {
			c++;
			if (halrf_rrf(rf, RF_PATH_B, 0xc5, BIT(15)) == 0x1) {
				break;
			} else {
				halrf_wrf(rf, RF_PATH_B, 0xd3, BIT(8), 0x1);
				halrf_wrf(rf, RF_PATH_B, 0xa0, MASKRF, 0x4c3);
				halrf_wrf(rf, RF_PATH_B, 0xa0, MASKRF, 0x4cf); 
				for (i = 0; i < 4; i++)
					halrf_delay_us(rf, 10);
				halrf_wrf(rf, RF_PATH_B, 0x18, MASKRF, tmp18);
				for (i = 0; i < 20; i++)
					halrf_delay_us(rf, 10);
				halrf_wrf(rf, RF_PATH_B, 0xd3, BIT(8), 0x0);
			}
		}
		if (c == 3)
			RF_DBG(rf, DBG_RF_RFK, "check LCK\n");
	}
}

void halrf_by_rate_dpd_8852a(struct rf_info *rf, enum phl_phy_idx phy)
{
	u8 legacy_th = 6; /*bypass <= 36M*/
	u8 ht_th = 5;	/* bypass <= MCS4*/
	u8 vht_th = 5;	/* bypass <= MCS4*/
	u8 he_th = 5;	/* bypass <= MCS4*/

	u8 legacy_val;
	u8 ht_val;
	u8 vht_val;
	u8 he_val;

	legacy_val = 0x20 | (4 + legacy_th);
	ht_val = 0x20 | ht_th;
	vht_val = 0x20 | vht_th;
	he_val = 0x20 | he_th;

	halrf_mac_set_pwr_reg_8852a(rf, phy, 0xd22c + (phy << 13), 0x1F800000, legacy_val); /*[28:23]*/
	halrf_mac_set_pwr_reg_8852a(rf, phy, 0xd230 + (phy << 13), 0x0003FFFF,
		ht_val | (vht_val << 6) | (he_val << 12)); /*[17:0]*/
}

void halrf_set_dpd_backoff_8852a(struct rf_info *rf, enum phl_phy_idx phy)
{
	struct halrf_dpk_info *dpk = &rf->dpk;
	u8 tx_scale, ofdm_bkof, path, kpath;

	kpath = halrf_kpath_8852a(rf, phy);

	ofdm_bkof = (u8)halrf_rreg(rf, 0x44a0 + (phy << 13), 0x0001F000); /*[16:12]*/
	tx_scale = (u8)halrf_rreg(rf, 0x44a0 + (phy << 13), 0x0000007F); /*[6:0]*/

	if ((ofdm_bkof + tx_scale) >= 44) { /*move dpd backoff to bb, and set dpd backoff to 0*/
		dpk->dpk_gs[phy] = 0x7f;
		for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
			if (kpath & BIT(path)) {
				halrf_wreg(rf, 0x81bc + (path << 8), 0x007FFFFF, 0x7f7f7f); /*[22:0]*/
				RF_DBG(rf, DBG_RF_RFK, "[RFK] Set S%d DPD backoff to 0dB\n", path);
			}
		}
	} else
		dpk->dpk_gs[phy] = 0x5b;
}

void halrf_dpk_init_8852a(struct rf_info *rf)
{
	/*by rate dpd*/
	halrf_by_rate_dpd_8852a(rf, HW_PHY_0);
	halrf_set_dpd_backoff_8852a(rf, HW_PHY_0);

	if (rf->hal_com->dbcc_en == true) {
		halrf_by_rate_dpd_8852a(rf, HW_PHY_1);
		halrf_set_dpd_backoff_8852a(rf, HW_PHY_1);
	}
}

void halrf_set_rxbb_bw_8852a(struct rf_info *rf, enum channel_width bw, enum rf_path path)
{
	halrf_wrf(rf, path, 0xee, BIT(2), 0x1);
	halrf_wrf(rf, path, 0x33, 0x0001F, 0xe); /*[4:0]*/

	if (bw == CHANNEL_WIDTH_20)
		halrf_wrf(rf, path, 0x3f, 0x0003F, 0x1b); /*[5:0]*/
	else if (bw == CHANNEL_WIDTH_40)
		halrf_wrf(rf, path, 0x3f, 0x0003F, 0x13); /*[5:0]*/
	else if (bw == CHANNEL_WIDTH_80)
		halrf_wrf(rf, path, 0x3f, 0x0003F, 0xb); /*[5:0]*/
	else
		halrf_wrf(rf, path, 0x3f, 0x0003F, 0x3); /*[5:0]*/

	RF_DBG(rf, DBG_RF_RFK, "[RFK] RXBB BW set 0x3F = 0x%x\n",
		halrf_rrf(rf, path, 0x3f, 0x0003F));

	halrf_wrf(rf, path, 0xee, BIT(2), 0x0);
}

void halrf_rxbb_bw_8852a(struct rf_info *rf, enum phl_phy_idx phy, enum channel_width bw)
{
	u8 kpath, path;

	kpath = halrf_kpath_8852a(rf, phy);
	
	for (path = 0; path < 2; path++) {
		if (kpath & BIT(path))
			halrf_set_rxbb_bw_8852a(rf, bw, path);
	}
}

void halrf_2g_rxant_8852a(struct rf_info *rf, enum halrf_ant ant)
{
	if (ant == RF_MAIN_ANT) {
		halrf_wrf(rf, RF_PATH_A, 0xef, MASKRF, 0x04000);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00005);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0xB0600);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00004);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x00400);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00003);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x38B00);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00002);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x00B00);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00001);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x01A00);
		halrf_wrf(rf, RF_PATH_A, 0xef, MASKRF, 0x00000);

		halrf_wrf(rf, RF_PATH_B, 0xef, MASKRF, 0x04000);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00005);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0xB0600);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00004);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x00400);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00003);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x38B00);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00002);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x00B00);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00001);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x01A00);
		halrf_wrf(rf, RF_PATH_B, 0xef, MASKRF, 0x00000);
                } else if (ant == RF_AUX_ANT) {
		halrf_wrf(rf, RF_PATH_A, 0xef, MASKRF, 0x04000);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00005);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x90600);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00004);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0xD0500);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00003);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0xD0400);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00002);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x10B00);
		halrf_wrf(rf, RF_PATH_A, 0x33, MASKRF, 0x00001);
		halrf_wrf(rf, RF_PATH_A, 0x3f, MASKRF, 0x02A00);
		halrf_wrf(rf, RF_PATH_A, 0xef, MASKRF, 0x00000);

		halrf_wrf(rf, RF_PATH_B, 0xef, MASKRF, 0x04000);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00005);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x90600);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00004);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0xD0500);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00003);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0xD0400);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00002);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x10B00);
		halrf_wrf(rf, RF_PATH_B, 0x33, MASKRF, 0x00001);
		halrf_wrf(rf, RF_PATH_B, 0x3f, MASKRF, 0x02A00);
		halrf_wrf(rf, RF_PATH_B, 0xef, MASKRF, 0x00000);
	}
}

void halrf_quick_check_rftx_8852a(struct rf_info *rf)
{
	/*check RFC*/
	RF_DBG(rf, DBG_RF_CHK, "======RFC======\n");
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x67 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x67, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x0 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x1 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x1, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x5 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x5, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x8 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x8, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x53 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x53, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x58 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x58, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x63 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x63, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x6e = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x6e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x6f = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x6f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x7e = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x7e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x7f = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x7f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x80 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x80, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x81 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x81, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xa0 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xa0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xb2 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xb2, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xc5 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xc5, MASKRF));

	RF_DBG(rf, DBG_RF_CHK, "RF1 0x67 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x67, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x0 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x1 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x1, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x5 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x5, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x8 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x8, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x53 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x53, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x58 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x58, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x63 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x63, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x6e = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x6e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x6f = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x6f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x7e = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x7e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x7f = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x7f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x80 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x80, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x81 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x81, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xa0 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xa0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xb2 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xb2, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xc5 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xc5, MASKRF));

	/*check DACK*/
	RF_DBG(rf, DBG_RF_CHK, "======DACK======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x5e00 = 0x%x\n", halrf_rreg(rf, 0x5e00, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5e14 = 0x%x\n", halrf_rreg(rf, 0x5e14, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5e44 = 0x%x\n", halrf_rreg(rf, 0x5e44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5e50 = 0x%x\n", halrf_rreg(rf, 0x5e50, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5e64 = 0x%x\n", halrf_rreg(rf, 0x5e64, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5e94 = 0x%x\n", halrf_rreg(rf, 0x5e94, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e00 = 0x%x\n", halrf_rreg(rf, 0x7e00, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e14 = 0x%x\n", halrf_rreg(rf, 0x7e14, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e44 = 0x%x\n", halrf_rreg(rf, 0x7e44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e50 = 0x%x\n", halrf_rreg(rf, 0x7e50, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e64 = 0x%x\n", halrf_rreg(rf, 0x7e64, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7e94 = 0x%x\n", halrf_rreg(rf, 0x7e94, MASKDWORD));

	/*check IQK*/
	RF_DBG(rf, DBG_RF_CHK, "======IQK======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x8000 = 0x%x\n", halrf_rreg(rf, 0x8000, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8004 = 0x%x\n", halrf_rreg(rf, 0x8004, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8008 = 0x%x\n", halrf_rreg(rf, 0x8008, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8080 = 0x%x\n", halrf_rreg(rf, 0x8080, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8124 = 0x%x\n", halrf_rreg(rf, 0x8124, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8138 = 0x%x\n", halrf_rreg(rf, 0x8138, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x813c = 0x%x\n", halrf_rreg(rf, 0x813c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8224 = 0x%x\n", halrf_rreg(rf, 0x8224, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8238 = 0x%x\n", halrf_rreg(rf, 0x8238, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x823c = 0x%x\n", halrf_rreg(rf, 0x823c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe0 = 0x%x\n", halrf_rreg(rf, 0x9fe0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe4 = 0x%x\n", halrf_rreg(rf, 0x9fe4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe8 = 0x%x\n", halrf_rreg(rf, 0x9fe8, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fec = 0x%x\n", halrf_rreg(rf, 0x9fec, MASKDWORD));

	/*check DPK*/
	RF_DBG(rf, DBG_RF_CHK, "======DPK======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x80b0 = 0x%x\n", halrf_rreg(rf, 0x80b0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x81bc = 0x%x\n", halrf_rreg(rf, 0x81bc, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x82bc = 0x%x\n", halrf_rreg(rf, 0x82bc, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x81b4 = 0x%x\n", halrf_rreg(rf, 0x81b4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x82b4 = 0x%x\n", halrf_rreg(rf, 0x82b4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x81c4 = 0x%x\n", halrf_rreg(rf, 0x81c4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x82c4 = 0x%x\n", halrf_rreg(rf, 0x82c4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x81c8 = 0x%x\n", halrf_rreg(rf, 0x81c8, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x82c8 = 0x%x\n", halrf_rreg(rf, 0x82c8, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x58d4 = 0x%x\n", halrf_rreg(rf, 0x58d4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x78d4 = 0x%x\n", halrf_rreg(rf, 0x78d4, MASKDWORD));

	/*check TSSI*/
	RF_DBG(rf, DBG_RF_CHK, "======TSSI======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x5818 = 0x%x\n", halrf_rreg(rf, 0x5818, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5820 = 0x%x\n", halrf_rreg(rf, 0x5820, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c60 = 0x%x\n", halrf_rreg(rf, 0x1c60, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c44 = 0x%x\n", halrf_rreg(rf, 0x1c44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5838 = 0x%x\n", halrf_rreg(rf, 0x5838, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x12c0 = 0x%x\n", halrf_rreg(rf, 0x12c0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x120c = 0x%x\n", halrf_rreg(rf, 0x120c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c04 = 0x%x\n", halrf_rreg(rf, 0x1c04, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c0c = 0x%x\n", halrf_rreg(rf, 0x1c0c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c18 = 0x%x\n", halrf_rreg(rf, 0x1c18, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7818 = 0x%x\n", halrf_rreg(rf, 0x7818, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7820 = 0x%x\n", halrf_rreg(rf, 0x7820, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c60 = 0x%x\n", halrf_rreg(rf, 0x3c60, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c44 = 0x%x\n", halrf_rreg(rf, 0x3c44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7838 = 0x%x\n", halrf_rreg(rf, 0x7838, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x32c0 = 0x%x\n", halrf_rreg(rf, 0x32c0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x320c = 0x%x\n", halrf_rreg(rf, 0x320c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c04 = 0x%x\n", halrf_rreg(rf, 0x3c04, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c0c = 0x%x\n", halrf_rreg(rf, 0x3c0c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c18 = 0x%x\n", halrf_rreg(rf, 0x3c18, MASKDWORD));
}


void halrf_quick_check_rfrx_8852a(struct rf_info *rf)
{
	/*check RFC*/
	RF_DBG(rf, DBG_RF_CHK, "======RFC======\n");
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x67 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x67, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x0 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x1 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x1, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x5 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x5, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x53 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x53, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x63 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x63, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x6e = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x6e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x6f = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x6f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x7e = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x7e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x7f = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x7f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x80 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x80, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x81 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x81, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x8d = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x8d, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x8f = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x8f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x90 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x90, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x92 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x92, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0x93 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0x93, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xa0 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xa0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xb2 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xb2, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF0 0xc5 = 0x%x\n", halrf_rrf(rf, RF_PATH_A, 0xc5, MASKRF));

	RF_DBG(rf, DBG_RF_CHK, "RF1 0x67 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x67, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x0 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x1 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x1, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x5 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x5, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x53 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x53, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x63 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x63, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x6e = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x6e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x6f = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x6f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x7e = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x7e, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x7f = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x7f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x80 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x80, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x81 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x81, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x8d = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x8d, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x8f = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x8f, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x90 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x90, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x92 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x92, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0x93 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0x93, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xa0 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xa0, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xb2 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xb2, MASKRF));
	RF_DBG(rf, DBG_RF_CHK, "RF1 0xc5 = 0x%x\n", halrf_rrf(rf, RF_PATH_B, 0xc5, MASKRF));

	/*check DACK*/
	RF_DBG(rf, DBG_RF_CHK, "======ADDCK======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x12d4 = 0x%x\n", halrf_rreg(rf, 0x12d4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x12d8 = 0x%x\n", halrf_rreg(rf, 0x12d8, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x32d4 = 0x%x\n", halrf_rreg(rf, 0x32d4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x32d8 = 0x%x\n", halrf_rreg(rf, 0x32d8, MASKDWORD));

	/*check IQK*/
	RF_DBG(rf, DBG_RF_CHK, "======IQK======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x8000 = 0x%x\n", halrf_rreg(rf, 0x8000, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8004 = 0x%x\n", halrf_rreg(rf, 0x8004, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8008 = 0x%x\n", halrf_rreg(rf, 0x8008, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8080 = 0x%x\n", halrf_rreg(rf, 0x8080, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8124 = 0x%x\n", halrf_rreg(rf, 0x8124, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8138 = 0x%x\n", halrf_rreg(rf, 0x8138, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x813c = 0x%x\n", halrf_rreg(rf, 0x813c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8224 = 0x%x\n", halrf_rreg(rf, 0x8224, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x8238 = 0x%x\n", halrf_rreg(rf, 0x8238, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x823c = 0x%x\n", halrf_rreg(rf, 0x823c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe0 = 0x%x\n", halrf_rreg(rf, 0x9fe0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe4 = 0x%x\n", halrf_rreg(rf, 0x9fe4, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fe8 = 0x%x\n", halrf_rreg(rf, 0x9fe8, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x9fec = 0x%x\n", halrf_rreg(rf, 0x9fec, MASKDWORD));

	/*check TSSI*/
	RF_DBG(rf, DBG_RF_CHK, "======TSSI======\n");
	RF_DBG(rf, DBG_RF_CHK, "0x5818 = 0x%x\n", halrf_rreg(rf, 0x5818, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5820 = 0x%x\n", halrf_rreg(rf, 0x5820, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c60 = 0x%x\n", halrf_rreg(rf, 0x1c60, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c44 = 0x%x\n", halrf_rreg(rf, 0x1c44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x5838 = 0x%x\n", halrf_rreg(rf, 0x5838, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x12c0 = 0x%x\n", halrf_rreg(rf, 0x12c0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x120c = 0x%x\n", halrf_rreg(rf, 0x120c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c04 = 0x%x\n", halrf_rreg(rf, 0x1c04, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c0c = 0x%x\n", halrf_rreg(rf, 0x1c0c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x1c18 = 0x%x\n", halrf_rreg(rf, 0x1c18, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7818 = 0x%x\n", halrf_rreg(rf, 0x7818, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7820 = 0x%x\n", halrf_rreg(rf, 0x7820, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c60 = 0x%x\n", halrf_rreg(rf, 0x3c60, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c44 = 0x%x\n", halrf_rreg(rf, 0x3c44, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x7838 = 0x%x\n", halrf_rreg(rf, 0x7838, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x32c0 = 0x%x\n", halrf_rreg(rf, 0x32c0, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x320c = 0x%x\n", halrf_rreg(rf, 0x320c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c04 = 0x%x\n", halrf_rreg(rf, 0x3c04, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c0c = 0x%x\n", halrf_rreg(rf, 0x3c0c, MASKDWORD));
	RF_DBG(rf, DBG_RF_CHK, "0x3c18 = 0x%x\n", halrf_rreg(rf, 0x3c18, MASKDWORD));

}
#endif
