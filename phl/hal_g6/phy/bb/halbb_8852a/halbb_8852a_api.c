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
#include "../halbb_precomp.h"

#ifdef BB_8852A_CAV_SUPPORT

__iram_func__
void halbb_bb_reset_8852a(struct bb_info *bb)
{
	// === [PHY0] === //
	halbb_set_reg(bb, 0x804, 0xff, 0xff);
	halbb_set_reg(bb, 0x804, 0xff, 0);
	halbb_set_reg(bb, 0x804, 0xff, 0xff);
	// === [PHY1] === //
	halbb_set_reg(bb, 0x884, 0xff, 0xff);
	halbb_set_reg(bb, 0x884, 0xff, 0);
	halbb_set_reg(bb, 0x884, 0xff, 0xff);
}

__iram_func__
void halbb_rxevm_reset_8852a(struct bb_info *bb)
{
	u8 i = 0, j = 0;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < MAX_USER_NUM; j++) {
			bb->rxevm.rxevm_seg[i].rxevm_user[j].rxevm_ss_0 = 0;
			bb->rxevm.rxevm_seg[i].rxevm_user[j].rxevm_ss_1 = 0;
			bb->rxevm.rxevm_seg[i].rxevm_user[j].rxevm_ss_2 = 0;
			bb->rxevm.rxevm_seg[i].rxevm_user[j].rxevm_ss_3 = 0;
		}
	}	
}

__iram_func__
void halbb_adc_rst_8852a(struct bb_info *bb)
{
	halbb_set_reg(bb, 0x4cb8, BIT(30), 1);
	halbb_set_reg(bb, 0x4db8, BIT(30), 1);
	halbb_set_reg(bb, 0xb2c, BIT(30), 1);
	halbb_set_reg(bb, 0x4ccc, BIT(0), 0);
	halbb_set_reg(bb, 0x4dcc, BIT(0), 0);
	halbb_set_reg(bb, 0x4ccc, BIT(0), 1);
	halbb_set_reg(bb, 0x4dcc, BIT(0), 1);
	halbb_set_reg(bb, 0xb2c, BIT(30), 0);
	halbb_set_reg(bb, 0xb0c, BIT(27) | BIT(26), 0x3);
	halbb_set_reg(bb, 0xb0c, BIT(27) | BIT(26), 0);
	halbb_set_reg(bb, 0x4cb8, BIT(30), 0);
	halbb_set_reg(bb, 0x4db8, BIT(30), 0);
}

__iram_func__
u32 halbb_read_rf_reg_8852a(struct bb_info *bb, enum rf_path path, u32 reg_addr,
			    u32 bit_mask)
{
	u32 readback_value = 0, direct_addr = 0;
	u32 offset_read_rf[2] = {0xc000, 0xd000};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	/*==== Error handling ====*/
	if (path > RF_PATH_B) {
		BB_WARNING("[%s] Unsupported path (%d)\n", __func__, path);
		return INVALID_RF_DATA;
	}

	/*==== Calculate offset ====*/
	reg_addr &= 0xff;
	direct_addr = offset_read_rf[path] + (reg_addr << 2);

	/*==== RF register only has 20bits ====*/
	bit_mask &= RFREGOFFSETMASK;

	/*==== Read RF register directly ====*/
	readback_value = halbb_get_reg(bb, direct_addr, bit_mask);
	BB_DBG(bb, DBG_PHY_CONFIG, "RF-%d 0x%x = 0x%x, bit mask = 0x%x\n", path,
	       reg_addr, readback_value, bit_mask);
	return readback_value;
}

__iram_func__
u8 halbb_sco_mapping_8852a(struct bb_info *bb,  u8 central_ch)
{
	/*=== SCO compensate : (BIT(0) << 18) / central_ch ===*/
	if (central_ch == 1)
		/*=== 2G ===*/
		return 109;
	else if (central_ch >= 2 && central_ch <= 6)
		return 108;
	else if (central_ch >= 7 && central_ch <= 10)
		return 107;
	else if (central_ch >= 11 && central_ch <= 14)
		return 106;
	else if (central_ch == 36 || central_ch == 38)
		return 51;
	else if (central_ch >= 40 && central_ch <= 58)
		return 50;
	else if (central_ch >= 60 && central_ch <= 64)
		return 49;
	else if (central_ch == 100 || central_ch == 102)
		return 48;
	else if (central_ch >= 104 && central_ch <= 126)
		return 47;
	else if (central_ch >= 128 && central_ch <= 151)
		return 46;
	else if (central_ch >= 153 && central_ch <= 177)
		return 45;
	else
		return 0;
}

__iram_func__
bool halbb_ctrl_sco_cck_8852a(struct bb_info *bb, u8 central_ch, u8 pri_ch,
			      enum channel_width bw)
{
	u8 ch_element;
	u32 sco_barker_threshold[14] = {0x1cfea, 0x1d0e1, 0x1d1d7, 0x1d2cd,
					0x1d3c3, 0x1d4b9, 0x1d5b0, 0x1d6a6,
					0x1d79c, 0x1d892, 0x1d988, 0x1da7f,
					0x1db75, 0x1ddc4};
	u32 sco_cck_threshold[14] = {0x27de3, 0x27f35, 0x28088, 0x281da,
				     0x2832d, 0x2847f, 0x285d2, 0x28724,
				     0x28877, 0x289c9, 0x28b1c, 0x28c6e,
				     0x28dc1, 0x290ed};
	
	if (bw == CHANNEL_WIDTH_20) {
		ch_element = central_ch - 1;		
	} else if (bw == CHANNEL_WIDTH_40) {
		if (pri_ch == 1)
			ch_element = central_ch - 1 + 2;
		else
			ch_element = central_ch - 1 - 2;
	} else {
		BB_WARNING("Invalid BW:%d for CCK\n", bw);
		return false;
	}
	if (ch_element >=14) {
		BB_WARNING("Invalid center frequency :%d \n", ch_element);
		return false;
	}
	halbb_set_reg(bb, 0x78b0, 0x7ffff, sco_barker_threshold[ch_element]);
	halbb_set_reg(bb, 0x78b4, 0x7ffff, sco_cck_threshold[ch_element]);
	BB_DBG(bb, DBG_PHY_CONFIG, "[CCK SCO Success]");
	return true;
}

__iram_func__
bool halbb_write_rf_reg_8852a(struct bb_info *bb, enum rf_path path,
			      u32 reg_addr, u32 bit_mask, u32 data)
{
	u32 direct_addr = 0;
	u32 offset_write_rf[2] = {0xc000, 0xd000};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	/*==== Error handling ====*/
	if (path > RF_PATH_B) {
		BB_WARNING("[%s] Unsupported path (%d)\n", __func__, path);
		return false;
	}

	/*==== Calculate offset ====*/
	reg_addr &= 0xff;
	direct_addr = offset_write_rf[path] + (reg_addr << 2);

	/*==== RF register only has 20bits ====*/
	bit_mask &= RFREGOFFSETMASK;

	/*==== Write RF register directly ====*/
	halbb_set_reg(bb, direct_addr, bit_mask, data);

	halbb_delay_us(bb, 1);

	BB_DBG(bb, DBG_PHY_CONFIG, "RF-%d 0x%x = 0x%x , bit mask = 0x%x\n",
	       path, reg_addr, data, bit_mask);

	return true;
}

__iram_func__
bool halbb_bw_setting_8852a(struct bb_info *bb, enum channel_width bw,
			    enum rf_path path)
{
	u32 rf_reg18 = 0;
	u32 adc_sel[2] = {0x4cd0, 0x4dd0};
	u32 wbadc_sel[2] = {0x4cec, 0x4dec};

	rf_reg18 = halbb_read_rf_reg_8852a(bb, path, 0x18, RFREGOFFSETMASK);
	/*==== [Error handling] ====*/
	if (rf_reg18 == INVALID_RF_DATA) {
		BB_WARNING("Invalid RF_0x18 for Path-%d\n", path);
		return false;
	}
	rf_reg18 &= ~(BIT(11) | BIT(10));
	/*==== [Switch bandwidth] ====*/
	switch (bw) {
	case CHANNEL_WIDTH_5:
	case CHANNEL_WIDTH_10:
	case CHANNEL_WIDTH_20:
		if (bw == CHANNEL_WIDTH_5) {
			/*ADC clock = 20M & WB ADC clock = 40M for BW5 */
			halbb_set_reg(bb, adc_sel[path], 0x6000, 0x1);
			halbb_set_reg(bb, wbadc_sel[path], 0x30, 0x0);
		} else if (bw == CHANNEL_WIDTH_10) {
			/*ADC clock = 40M & WB ADC clock = 80M for BW10 */
			halbb_set_reg(bb, adc_sel[path], 0x6000, 0x2);
			halbb_set_reg(bb, wbadc_sel[path], 0x30, 0x1);
		} else if (bw == CHANNEL_WIDTH_20) {
			/*ADC clock = 80M & WB ADC clock = 160M for BW20 */
			halbb_set_reg(bb, adc_sel[path], 0x6000, 0x0);
			halbb_set_reg(bb, wbadc_sel[path], 0x30, 0x2);
		}

		/*RF bandwidth */
		rf_reg18 |= (BIT(11) | BIT(10));

		break;
	case CHANNEL_WIDTH_40:
		/*ADC clock = 80M & WB ADC clock = 160M for BW40 */
		halbb_set_reg(bb, adc_sel[path], 0x6000, 0x0);
		halbb_set_reg(bb, wbadc_sel[path], 0x30, 0x2);

		/*RF bandwidth */
		rf_reg18 |= BIT(11);

		break;
	case CHANNEL_WIDTH_80:
		/*ADC clock = 160M & WB ADC clock = 160M for BW40 */
		halbb_set_reg(bb, adc_sel[path], 0x6000, 0x0);
		halbb_set_reg(bb, wbadc_sel[path], 0x30, 0x2);

		/*RF bandwidth */
		rf_reg18 |= BIT(10);

		break;
	default:
		BB_WARNING("Fail to set ADC\n");
	}

	/*==== [Write RF register] ====*/
	halbb_write_rf_reg_8852a(bb, path, 0x18, RFREGOFFSETMASK, rf_reg18);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success][bw_setting] ADC setting for Path-%d\n", path);
	return true;
}

__iram_func__
bool halbb_ctrl_bw_8852a(struct bb_info *bb, u8 pri_ch, enum channel_width bw,
			 enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

#if 0//def HALBB_DBCC_SUPPORT
	bb = halbb_get_curr_bb_pointer(bb, phy_idx);
#endif

	if (bb->is_disable_phy_api) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[%s] Disable PHY API\n", __func__);
		return true;
	}

	/*==== Error handling ====*/
	if (bw >= CHANNEL_WIDTH_MAX || (bw == CHANNEL_WIDTH_40 && pri_ch > 2) ||
	    (bw == CHANNEL_WIDTH_80 && pri_ch > 4)) {
		BB_WARNING("Fail to switch bw(bw:%d, pri ch:%d)\n", bw,
			   pri_ch);
		return false;
	}

	/*==== Switch bandwidth ====*/
	switch (bw) {
	case CHANNEL_WIDTH_5:
	case CHANNEL_WIDTH_10:
	case CHANNEL_WIDTH_20:
		if (bw == CHANNEL_WIDTH_5) {
			/*RF_BW:[31:30]=0x0 */
			halbb_set_reg_cmn(bb, 0x1878, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x1 */
			halbb_set_reg_cmn(bb, 0x187c, 0x3000, 0x1, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x187c, 0xf00, 0x0, phy_idx);
		} else if (bw == CHANNEL_WIDTH_10) {
			/*RF_BW:[31:30]=0x0 */
			halbb_set_reg_cmn(bb, 0x1878, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x2 */
			halbb_set_reg_cmn(bb, 0x187c, 0x3000, 0x2, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x187c, 0xf00, 0x0, phy_idx);
		} else if (bw == CHANNEL_WIDTH_20) {
			/*RF_BW:[31:30]=0x0 */
			halbb_set_reg_cmn(bb, 0x1878, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x0 */
			halbb_set_reg_cmn(bb, 0x187c, 0x3000, 0x0, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x187c, 0xf00, 0x0, phy_idx);
		}

		break;
	case CHANNEL_WIDTH_40:
		/*RF_BW:[31:30]=0x1 */
		halbb_set_reg_cmn(bb, 0x1878, 0xC0000000, 0x1, phy_idx);
		/*small BW:[13:12]=0x0 */
		halbb_set_reg_cmn(bb, 0x187c, 0x3000, 0x0, phy_idx);
		/*Pri ch:[11:8] */
		halbb_set_reg_cmn(bb, 0x187c, 0xf00, pri_ch, phy_idx);
		/*CCK primary channel */
		if (pri_ch == 1)
			halbb_set_reg(bb, 0x787c, BIT(0), 1);
		else
			halbb_set_reg(bb, 0x787c, BIT(0), 0);

		break;
	case CHANNEL_WIDTH_80:
		/*RF_BW:[31:30]=0x2 */
		halbb_set_reg_cmn(bb, 0x1878, 0xC0000000, 0x2, phy_idx);
		/*small BW:[13:12]=0x0 */
		halbb_set_reg_cmn(bb, 0x187c, 0x3000, 0x0, phy_idx);
		/*Pri ch:[11:8] */
		halbb_set_reg_cmn(bb, 0x187c, 0xf00, pri_ch, phy_idx);

		break;
	default:
		BB_WARNING("Fail to switch bw (bw:%d, pri ch:%d)\n", bw,
			   pri_ch);
	}

	if (phy_idx == HW_PHY_0) {
		/*============== [Path A] ==============*/
		halbb_bw_setting_8852a(bb, bw, RF_PATH_A);
		/*============== [Path B] ==============*/
		if (!bb->hal_com->dbcc_en)
			halbb_bw_setting_8852a(bb, bw, RF_PATH_B);
	} else {
		/*============== [Path B] ==============*/
		halbb_bw_setting_8852a(bb, bw, RF_PATH_B);
	}

	/*==== ADC reset ====*/
	if (bb->hal_com->cv == CAV)
		halbb_adc_rst_8852a(bb);

	BB_DBG(bb, DBG_PHY_CONFIG,
		  "[Switch BW Success] BW: %d for PHY%d\n", bw, phy_idx);

	return true;
}

__iram_func__
bool halbb_ch_setting_8852a(struct bb_info *bb, u8 central_ch, enum rf_path path,
			    bool *is_2g_ch)
{
	u32 rf_reg18 = 0;

	rf_reg18 = halbb_read_rf_reg_8852a(bb, path, 0x18, RFREGOFFSETMASK);
	/*==== [Error handling] ====*/
	if (rf_reg18 == INVALID_RF_DATA) {
		BB_WARNING("Invalid RF_0x18 for Path-%d\n", path);
		return false;
	}
	*is_2g_ch = (central_ch <= 14) ? true : false;
	/*==== [Set RF Reg 0x18] ====*/
	rf_reg18 &= ~0x303ff; /*[17:16],[9:8],[7:0]*/
	rf_reg18 |= central_ch; /* Channel*/
	/*==== [5G Setting] ====*/
	if (!*is_2g_ch)
		rf_reg18 |= (BIT(16) | BIT(8));
	halbb_write_rf_reg_8852a(bb, path, 0x18, RFREGOFFSETMASK, rf_reg18);

	halbb_delay_us(bb, 100);

	halbb_write_rf_reg_8852a(bb, path, 0xcf, BIT(0), 0);
	halbb_write_rf_reg_8852a(bb, path, 0xcf, BIT(0), 1);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Success][ch_setting] CH: %d for Path-%d\n",
	       central_ch, path);
	return true;
}

__iram_func__
bool halbb_ctrl_ch_8852a(struct bb_info *bb, u8 central_ch,
			 enum phl_phy_idx phy_idx)
{
	u8 sco_comp;
	bool is_2g_ch;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bb->is_disable_phy_api) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[%s] Disable PHY API\n", __func__);
		return true;
	}
	/*==== Error handling ====*/
	if ((central_ch > 14 && central_ch < 36) ||
		    (central_ch > 64 && central_ch < 100) ||
		    (central_ch > 144 && central_ch < 149) ||
		central_ch > 177 ) {
		BB_WARNING("Invalid CH:%d for PHY%d\n", central_ch,
			   phy_idx);
		return false;
	}

	if (phy_idx == HW_PHY_0) {
		/*============== [Path A] ==============*/
		halbb_ch_setting_8852a(bb, central_ch, RF_PATH_A, &is_2g_ch);
		//------------- [Mode Sel - Path A] ------------//
		if (is_2g_ch)
			halbb_set_reg_cmn(bb, 0x156c, BIT(31) | BIT(30), 1,
					  phy_idx);
		else
			halbb_set_reg_cmn(bb, 0x156c, BIT(31) | BIT(30), 0,
					  phy_idx);

		/*============== [Path B] ==============*/
		if (!bb->hal_com->dbcc_en) {
			halbb_ch_setting_8852a(bb, central_ch, RF_PATH_B,
					       &is_2g_ch);
			//------------- [Mode Sel - Path B] ------------//
			if (is_2g_ch)
				halbb_set_reg_cmn(bb, 0x162c, BIT(31) | BIT(30),
						  1, phy_idx);
			else
				halbb_set_reg_cmn(bb, 0x162c, BIT(31) | BIT(30),
						  0, phy_idx);
		} else { /*==== [Phy0 config at 2/5G] ====*/
			if (is_2g_ch)
				halbb_set_reg(bb, 0x1874, BIT(1), 0);
			else
				halbb_set_reg(bb, 0x1874, BIT(1), 1);
		}
		/*==== [SCO compensate fc setting] ====*/
		sco_comp = halbb_sco_mapping_8852a(bb, central_ch);
		halbb_set_reg_cmn(bb, 0x1878, 0x7f, sco_comp, phy_idx);
	} else {
		/*============== [Path B] ==============*/
		halbb_ch_setting_8852a(bb, central_ch, RF_PATH_B, &is_2g_ch);

		//------------- [Mode Sel - Path B] ------------//
		if (is_2g_ch)
			halbb_set_reg_cmn(bb, 0x162c, BIT(31) | BIT(30), 1,
					  phy_idx);
		else
			halbb_set_reg_cmn(bb, 0x162c, BIT(31) | BIT(30), 0,
					  phy_idx);

		/*==== [SCO compensate fc setting] ====*/
		sco_comp = halbb_sco_mapping_8852a(bb, central_ch);
		halbb_set_reg_cmn(bb, 0x1878, 0x7f, sco_comp, phy_idx);
	}

	/*==== ADC reset ====*/
	if (bb->hal_com->cv == CAV)
		halbb_adc_rst_8852a(bb);
	/* === Bandedge ===*/
	halbb_set_reg_cmn(bb, 0x1390, BIT(30), 1, phy_idx);
	/* === CCK Parameters === */
	if (central_ch == 14) {
		halbb_set_reg(bb, 0x7800, 0xffffff, 0x3b13ff);
		halbb_set_reg(bb, 0x7804, 0xffffff, 0x1c42de);
		halbb_set_reg(bb, 0x7808, 0xffffff, 0xfdb0ad);
		halbb_set_reg(bb, 0x780c, 0xffffff, 0xf60f6e);
		halbb_set_reg(bb, 0x7810, 0xffffff, 0xfd8f92);
		halbb_set_reg(bb, 0x7814, 0xffffff, 0x2d011);
		halbb_set_reg(bb, 0x7818, 0xffffff, 0x1c02c);
		halbb_set_reg(bb, 0x781c, 0xffffff, 0xfff00a);
	} else {	
		halbb_set_reg(bb, 0x7800, 0xffffff, 0x3d23ff);
		halbb_set_reg(bb, 0x7804, 0xffffff, 0x29b354);
		halbb_set_reg(bb, 0x7808, 0xffffff, 0xfc1c8);
		halbb_set_reg(bb, 0x780c, 0xffffff, 0xfdb053);
		halbb_set_reg(bb, 0x7810, 0xffffff, 0xf86f9a);
		halbb_set_reg(bb, 0x7814, 0xffffff, 0xfaef92);
		halbb_set_reg(bb, 0x7818, 0xffffff, 0xfe5fcc);
		halbb_set_reg(bb, 0x781c, 0xffffff, 0xffdff5);
	}

	BB_DBG(bb, DBG_PHY_CONFIG, "[Switch CH Success] CH: %d for PHY%d\n",
	       central_ch, phy_idx);
	return true;
}

__iram_func__
void halbb_ctrl_cck_en_8852a(struct bb_info *bb, bool cck_en,
			     enum phl_phy_idx phy_idx)
{
	if (cck_en) {
		halbb_set_reg(bb, 0x7800, BIT(27), 0);
		halbb_set_reg(bb, 0x7844, BIT(31), 0);		
	} else {
		halbb_set_reg(bb, 0x7800, BIT(27), 1);
		halbb_set_reg(bb, 0x7844, BIT(31), 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[CCK Enable for PHY%d]\n", phy_idx);
}

__iram_func__
bool halbb_ctrl_bw_ch_8852a(struct bb_info *bb, u8 pri_ch, u8 central_ch,
			    enum channel_width bw, enum phl_phy_idx phy_idx)
{
	bool rpt = true;
	bool cck_en = false;
	u8 pri_ch_idx = 0;

	/*==== [Set pri_ch idx] ====*/
	if (central_ch <= 14) {
		// === 2G === //
		switch (bw) {
		case CHANNEL_WIDTH_20:
			break;

		case CHANNEL_WIDTH_40:
			pri_ch_idx = pri_ch > central_ch ? 1 : 2;
			break;

		default:
			break;
		}

		/*==== [CCK SCO Compesate] ====*/
		rpt &= halbb_ctrl_sco_cck_8852a(bb, central_ch, pri_ch, bw);

		cck_en = true;
	} else {
		// === 5G === //
		switch (bw) {
		case CHANNEL_WIDTH_20:
			break;

		case CHANNEL_WIDTH_40:
		case CHANNEL_WIDTH_80:
			if (pri_ch > central_ch)
				pri_ch_idx = (pri_ch - central_ch) >> 1;
			else
				pri_ch_idx = ((central_ch - pri_ch) >> 1) + 1;
			break;

		default:
			break;
		}
		cck_en = false;
	}

	/*==== [Switch CH] ====*/
	rpt &= halbb_ctrl_ch_8852a(bb, central_ch, phy_idx);
	/*==== [Switch BW] ====*/
	rpt &= halbb_ctrl_bw_8852a(bb, pri_ch_idx, bw, phy_idx);
	/*==== [CCK Enable / Disable] ====*/
	halbb_ctrl_cck_en_8852a(bb, cck_en, phy_idx);
	/*==== [BB reset] ====*/
	halbb_bb_reset_8852a(bb);
	/*==== RxEVM reset ====*/
	halbb_rxevm_reset_8852a(bb);

	return rpt;
}

__iram_func__
bool halbb_ctrl_rx_path_8852a(struct bb_info *bb, enum rf_path rx_path)
{
	u32 ofdm_rx = 0x0;

	ofdm_rx = (u32)rx_path;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	/*==== Rx Path Backup (For 52A CAV)====*/
	bb->rx_path = rx_path;

	if (bb->hal_com->dbcc_en) {
		halbb_set_reg(bb, 0x187c, 0xf, 0x1);
		halbb_set_reg(bb, 0x587c, 0xf, 0x2);
		/*==== [PHY0] 1rcca ====*/
		halbb_set_reg(bb, 0x1878, 0x3C000, 1);
		halbb_set_reg(bb, 0x1878, 0x3C0000, 1);
		/*==== [PHY1] 1rcca ====*/
		halbb_set_reg(bb, 0x5878, 0x3C000, 2);
		halbb_set_reg(bb, 0x5878, 0x3C0000, 2);
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 0);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0x4580, 0x3fc0, 4);
		halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23), 0);
		/*==== [PHY1] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0x4618, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0x4618, BIT(22) | BIT(21), 0);
		/*==== [PHY1] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0x4680, 0x3fc0, 4);
		halbb_set_reg(bb, 0x4680, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0x4680, BIT(25) | BIT(24) | BIT(23), 0);
	} else {
		if (ofdm_rx == RF_PATH_A) {
			halbb_set_reg(bb, 0x187c, 0xf, 0x1);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x1878, 0x3C000, 1);
			halbb_set_reg(bb, 0x1878, 0x3C0000, 1);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 0);
			halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 0);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0x4580, 0x3fc0, 4);
			halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14),
				      0);
			halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23),
				      0);
		} else if (ofdm_rx == RF_PATH_B) {
			halbb_set_reg(bb, 0x187c, 0xf, 0x2);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x1878, 0x3C000, 0x2);
			halbb_set_reg(bb, 0x1878, 0x3C0000, 0x2);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 0);
			halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 0);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0x4580, 0x3fc0, 4);
			halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14),
				      0);
			halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23),
				      0);
		} else if (ofdm_rx == RF_PATH_AB) {
			halbb_set_reg(bb, 0x187c, 0xf, 0x3);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x1878, 0x3C000, 0x3);
			halbb_set_reg(bb, 0x1878, 0x3C0000, 0x3);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 1);
			halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 1);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0x4580, 0x3fc0, 4);
			halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14),
				      1);
			halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23),
				      1);
		}
	}
	/*==== [BB reset] ====*/
	halbb_bb_reset_8852a(bb);
	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx Success]RX_en=%x\n", rx_path);
	return true;

}

__iram_func__
bool halbb_ctrl_cck_rx_path_8852a(struct bb_info *bb, enum rf_path rx_path)
{
	u32 ofdm_rx = 0x0;

	ofdm_rx = (u32)rx_path;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	/*==== Rx Path Backup (For 52A CAV)====*/
	bb->rx_path = rx_path;

	if (bb->hal_com->dbcc_en) {
		halbb_set_reg(bb, 0x187c, 0xf, 0x1);
		halbb_set_reg(bb, 0x587c, 0xf, 0x2);
	} else {
		if (ofdm_rx == RF_PATH_A)
			halbb_set_reg(bb, 0x187c, 0xf, 0x1);
		else if (ofdm_rx == RF_PATH_B)
			halbb_set_reg(bb, 0x187c, 0xf, 0x2);
		else if (ofdm_rx == RF_PATH_AB)
			halbb_set_reg(bb, 0x187c, 0xf, 0x3);
	}

	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx Success]RX_en=%x\n", rx_path);
	return true;
}

__iram_func__
bool halbb_ctrl_tx_path_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	/*==== Rx Path Backup (For 52A CAV)====*/
	bb->bb_cmn_backup_i.cur_tx_path = tx_path;

	/*==== [P-MAC] Path & Path_map Enable ====*/
	halbb_set_reg_cmn(bb, 0x0da4, 0x1c, 0x7, HW_PHY_0);
	halbb_set_reg_cmn(bb, 0x0da4, 0x1c, 0x7, HW_PHY_1);

	if (!bb->hal_com->dbcc_en) {
		if (tx_path == RF_PATH_A) {
			halbb_set_reg(bb, 0x147c, 0xf0000000, 0x1);
			halbb_set_reg(bb, 0x14a4, 0x1e00000, 0x0);
		} else if (tx_path == RF_PATH_B) {
			halbb_set_reg(bb, 0x147c, 0xf0000000, 0x2);
			halbb_set_reg(bb, 0x14a4, 0x1e00000, 0x0);
		} else if (tx_path == RF_PATH_AB) {
			halbb_set_reg(bb, 0x147c, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x14a4, 0x1e00000, 0x4);
		} else {
			BB_WARNING("Invalid Tx Path\n");
			return false;
		}
	} else {
		halbb_set_reg(bb, 0x147c, 0xf0000000, 0x1);
		halbb_set_reg_cmn(bb, 0x147c, 0xf0000000, 0x2, HW_PHY_1);
		halbb_set_reg(bb, 0x14a4, 0x1e00000, 0x0);
		halbb_set_reg_cmn(bb, 0x14a4, 0x1e00000, 0x4, HW_PHY_1);
	}
	/*==== [BB reset] ====*/
	halbb_bb_reset_8852a(bb);
	BB_DBG(bb, DBG_PHY_CONFIG, "[Success] [P-MAC] Tx Path Config\n");
	return true;
}

#ifdef HALBB_CONFIG_RUN_IN_DRV

void halbb_cr_init_rfe51_53_8852a(struct bb_info *bb)
{
	if (bb->phl_com->dev_cap.rfe_type != 51 &&
	    bb->phl_com->dev_cap.rfe_type != 53)
		return;

	halbb_set_reg(bb, 0x1594, MASKDWORD, 0x5CE6C576);
	halbb_set_reg(bb, 0x1654, MASKDWORD, 0x5CE6C576);
	halbb_set_reg(bb, 0x7C04, MASKDWORD, 0x04231040);
	halbb_set_reg(bb, 0x7D04, MASKDWORD, 0x04231040);
	halbb_set_reg(bb, 0x7C08, MASKDWORD, 0x04231040);
	halbb_set_reg(bb, 0x7D08, MASKDWORD, 0x04231040);
	halbb_set_reg(bb, 0x7C6C, MASKDWORD, 0x00D00000);
	halbb_set_reg(bb, 0x7D6C, MASKDWORD, 0x00D00000);
}

void halbb_plcp_gen_homologation_8852a(struct bb_info *bb, void *in_void)
{
	struct halbb_plcp_info *in = (struct halbb_plcp_info *)in_void;

#ifdef SPF_PHL_BB_007_01_HOMO
	if (bb->phl_com->drv_mode != RTW_DRV_MODE_HOMOLOGATION)
		return;
#endif
	if (bb->ic_type != BB_RTL8852AA)
		return;

	switch(in->ppdu_type) {
	case B_MODE_FMT: //CCK
		break;
	case LEGACY_FMT: //Legacy
		halbb_set_reg(bb, 0x1470, 0x7f, 0xd);
		halbb_set_reg(bb, 0x142c, MASKDWORD, 0x57);
		break;
	case HT_MF_FMT: //HT_MF
		if (in->dbw == 0) {
			halbb_set_reg(bb, 0x1470, 0x7f, 0x75);
			halbb_set_reg(bb, 0x142c, MASKDWORD, 0x56);
		} else {
			halbb_set_reg(bb, 0x1470, 0x7f, 0xf);
			halbb_set_reg(bb, 0x142c, MASKDWORD, 0x77);
		}
		break;
	case VHT_FMT: //VHT
		if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_24G) {
			if (in->dbw == 0) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x2b);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0xe8);
			} else {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x7b);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0xed);
			}
		} else if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_5G) {
			if (in->dbw == 0) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0xa);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0x28);
			} else if (in->dbw == 1) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x58);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0xd);
			} else {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x15);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0x54);
			}
		}
		break;
	case HE_SU_FMT: //HE_SU
		if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_24G) {
			if (in->dbw == 0) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x3a);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0xac);
			} else {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x42);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0x1e);
			}
		} else if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_5G) {
			if (in->dbw == 0) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x5a);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0xfe);
			} else if (in->dbw == 1) {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x64);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0x12);
			} else {
				halbb_set_reg(bb, 0x1470, 0x7f, 0x3d);
				halbb_set_reg(bb, 0x142c, MASKDWORD, 0x5d);
			}
		}
		halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
		break;
	case HE_TB_FMT: //HE_TB
		if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_24G) {
			if (in->dbw == 0) {
				switch (in->usr[0].ru_alloc) {
				case 0:
					halbb_set_reg(bb, 0x1470, 0x7f, 0x69);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x4a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (4 << 1):
				case (8 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x65);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x52);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (37 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x18);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xb);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (39 << 1):
				case (40 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x4);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xf);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (53 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x36);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xbd);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (54 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x2f);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x69);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (61 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x47);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x70);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				default:
					break;
				}
			} else {
				switch (in->usr[0].ru_alloc) {
				case 0:
				case (9 << 1):
				case (17 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x65);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x52);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (37 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x18);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xb);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (41 << 1):
				case (44 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x4);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xf);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (53 << 1):
				case (55 << 1):
				case (56 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x44);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0xa);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (61 << 1):
				case (62 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x16);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x39);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (65 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x66);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x32);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				default:
					break;
				}
			}
		} else if (bb->hal_com->band[0].cur_chandef.band == BAND_ON_5G) {
			if (in->dbw == 0) {
				switch (in->usr[0].ru_alloc) {
				case 0:
					halbb_set_reg(bb, 0x1470, 0x7f, 0x69);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x4a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (4 << 1):
				case (8 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x65);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x52);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (37 << 1):
				case (39 << 1):
				case (40 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7e);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x3e);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (53 << 1):
				case (54 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7b);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x6a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (61 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x5c);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x99);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				default:
					break;
				}
			} else if (in->dbw == 1) {
				switch (in->usr[0].ru_alloc) {
				case 0:
					halbb_set_reg(bb, 0x1470, 0x7f, 0x69);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x4a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (9 << 1):
				case (17 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x65);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x52);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (37 << 1):
				case (41 << 1):
				case (44 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7e);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x3e);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (53 << 1):
				case (55 << 1):
				case (56 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7b);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x6a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (61 << 1):
				case (62 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x5c);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x99);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (65 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x4f);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x74);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				default:
					break;
				}
			} else {
				switch (in->usr[0].ru_alloc) {
				case 0:
				case (36 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x28);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x9);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (18 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x65);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x52);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (37 << 1):
				case (45 << 1):
				case (52 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7e);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x3e);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (53 << 1):
				case (57 << 1):
				case (60 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x7b);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x6a);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (61 << 1):
				case (64 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x5c);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x99);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (63 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x23);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x5b);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (65 << 1):
				case (66 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x20);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x35);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				case (67 << 1):
					halbb_set_reg(bb, 0x1470, 0x7f, 0x6);
					halbb_set_reg(bb, 0x142c, MASKDWORD, 0x2f);
					halbb_set_reg(bb, 0x14a0, 0x7, 0x0);
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

u32 halbb_phy0_to_phy1_ofst_8852a(struct bb_info *bb, u32 addr)
{
	u32 phy_page = addr >> 8;
	u32 ofst = 0;
	u8 addr_frag = 0;

#ifdef HALBB_DBCC_SUPPORT

	switch (phy_page) {
	case 0x2:
	case 0x41:
		ofst = 0x100;
		break;
	case 0x45:
		ofst = 0x100;
		break;

	case 0x8:
	case 0xa:
	case 0x49:
		addr_frag = (addr & 0xf0) >> 4;
		if (addr_frag < 0x8)
			ofst = 0x80;

		break;

	case 0xd:
		addr_frag = (addr & 0xf0) >> 4;
		if (addr_frag < 0x4 || (addr_frag >= 0x8 && addr_frag < 0xc))
			ofst = 0x40;

		break;
	case 0x47:
	case 0x48:
	case 0xe:
	case 0xf:
		addr_frag = (addr & 0xf0) >> 4;
		if (addr_frag >= 0x8 && addr_frag < 0xc)
			ofst = 0x40;

		break;
	default:
		ofst = 0; /*warning case*/
		break;
	}

	if ((phy_page >= 0x10) && (phy_page <= 0x2f))
		ofst = 0x4000;

#endif
	return ofst;
}

void halbb_gpio_ctrl_dump_8852a(struct bb_info *bb)
{
	u32 rfe_cr_table[] = {0x0b34, 0x0b3c, 0x7c68, 0x7c94, 0x7d68, 0x7d94, 
			      0x7c90, 0x7d90, 0x7c80, 0x7c84, 0x7d80, 0x7d84};
	u8 i = 0;
	u8 rfe_cr_len = sizeof(rfe_cr_table) / sizeof(u32);
	u32 cr_tmp, val;
	BB_DBG(bb, DBG_DBG_API, "[%s] ==================> \n", __func__);
	for (i = 0; i < rfe_cr_len; i++) {
		cr_tmp = rfe_cr_table[i];
		val = halbb_get_reg(bb, cr_tmp, MASKDWORD);
		BB_DBG(bb, DBG_DBG_API, "Reg[0x%04x] = 0x%08x\n", cr_tmp, val);
	}
}

void halbb_gpio_rfm_8852a(struct bb_info *bb, enum bb_path path,
			  enum bb_rfe_src_sel src, bool dis_tx_gnt_wl,
			  bool active_tx_opt, bool act_bt_en, u8 rfm_output_val)
{
	u32 path_cr_base = 0;
	u32 cr_tmp = 0;
	u32 val_tmp = 0;
	u32 mask = 0xff, mask_ofst = 0;

	BB_DBG(bb, DBG_DBG_API,
	       "[src:%d]{dis_tx_gnt_wl:%d, active_tx_opt:%d, act_bt_en:%d] = 0x%x\n",
	       src, dis_tx_gnt_wl, active_tx_opt, act_bt_en, rfm_output_val);

	if (src == PAPE_RFM)
		mask_ofst = 0;
	else if (src == LNAON_RFM)
		mask_ofst = 8;
	else if (src == TRSW_RFM)
		mask_ofst = 16;
	else
		return; /*invalid case*/

	if (path == BB_PATH_A) {
		path_cr_base = 0x7c00;
	} else {
		path_cr_base = 0x7d00;
	}

	cr_tmp = path_cr_base | 0x94;
	val_tmp = (u8)dis_tx_gnt_wl << 7 | (u8)active_tx_opt << 6 |
		  (u8)act_bt_en << 5 | rfm_output_val;
	mask = 0xff << mask_ofst;
		
	halbb_set_reg(bb, cr_tmp, mask, val_tmp);

	BB_DBG(bb, DBG_DBG_API, "0x%x[0x%x]=%d\n",cr_tmp, mask, val_tmp);
}

void halbb_gpio_trsw_table_8852a(struct bb_info *bb, enum bb_path path,
				 bool path_en, bool trsw_tx,
				 bool trsw_rx, bool trsw, bool trsw_b)
{
	u32 path_cr_base = 0;
	u32 cr_tmp = 0;
	u32 val_tmp = 0;
	u32 mask_ofst = 16;

	BB_DBG(bb, DBG_DBG_API,
	       "[path %d]{path_en:%d, trsw_tx:%d, trsw_rx:%d] = {trsw:%d, trsw_b:%d}\n",
	       path, path_en, trsw_tx, trsw_rx, trsw, trsw_b);

	if (path == BB_PATH_A) {
		path_cr_base = 0x7c00;
	} else {
		path_cr_base = 0x7d00;
	}

	cr_tmp = path_cr_base | 0x68;
	mask_ofst += ((u8)path_en << 2 | (u8)trsw_tx << 1 | (u8)trsw_rx) << 1;
	val_tmp = (u8)trsw << 1 | (u8)trsw_b;

	halbb_set_reg(bb, cr_tmp, 0x3 << mask_ofst, val_tmp);

	BB_DBG(bb, DBG_DBG_API, "0x%x[0x%x]=%d\n",cr_tmp, mask_ofst, val_tmp);
}

void halbb_gpio_setting_8852a(struct bb_info *bb, u8 gpio_idx,
			      enum bb_path path, bool inv,
			      enum bb_rfe_src_sel src)
{
	u8 path_sel = 0;
	u32 path_cr_base = 0;
	u32 cr_tmp = 0;
	u32 mask_tmp = 0;

	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);
	BB_DBG(bb, DBG_DBG_API, "gpio_idx[%d]:{Path=%d}{inv=%d}{src=%d}\n",
	       gpio_idx, path, inv, src);

	if (gpio_idx > 31 || path >= BB_PATH_C) {
		BB_WARNING("Wrong gpio_idx=%d,path=%d}\n", gpio_idx, path);
		return;
	}

	if (path == BB_PATH_A) {
		path_sel = 0;
		path_cr_base = 0x7c00;
	} else {
		path_sel = 1;
		path_cr_base = 0x7d00;
	}
		
	/*[Path_mux_sel]*/
	if (gpio_idx <= 15) {
		cr_tmp = 0xb3c;
		mask_tmp = ((u32)0x3) << (gpio_idx << 1);
	} else {
		cr_tmp = 0xb40;
		mask_tmp = ((u32)0x3) << ((gpio_idx - 16) << 1);
	}

	halbb_set_reg(bb, cr_tmp, mask_tmp, path_sel);
	BB_DBG(bb, DBG_DBG_API, "Path: 0x%x[0x%x]=%d\n",
	       cr_tmp, mask_tmp, path_sel);

	/*[inv]*/
	halbb_set_reg(bb, (path_cr_base | 0x90), BIT(gpio_idx), (u32)inv);
	BB_DBG(bb, DBG_DBG_API, "Inv: 0x%x[0x%x]=%d\n",
	       (path_cr_base | 0x90), (u32)BIT(gpio_idx), inv);

	/*[Output Source Signal]*/
	cr_tmp = path_cr_base | 0x80 | ((gpio_idx >> 1) & 0xfc);
	mask_tmp = ((u32)0xf) << ((gpio_idx % 8) << 2);
	halbb_set_reg(bb, cr_tmp, mask_tmp, (u32)src);
	BB_DBG(bb, DBG_DBG_API, "src: 0x%x[0x%x]=%d\n", cr_tmp, mask_tmp, src);
}

void halbb_gpio_setting_all_8852a(struct bb_info *bb, u8 rfe_idx)
{

	if (rfe_idx == 51) {
		halbb_gpio_setting_8852a(bb, 11, BB_PATH_A, false, PAPE_RFM);
		halbb_gpio_setting_8852a(bb, 7, BB_PATH_A, true, TRSW_RFM);
		halbb_gpio_setting_8852a(bb, 6, BB_PATH_A, false, LNAON_RFM);
		
		halbb_gpio_setting_8852a(bb, 1, BB_PATH_B, false, PAPE_RFM);
		halbb_gpio_setting_8852a(bb, 2, BB_PATH_B, true, TRSW_RFM);
		halbb_gpio_setting_8852a(bb, 3, BB_PATH_B, false, LNAON_RFM);

	} else if (rfe_idx == 53) {
		halbb_gpio_setting_8852a(bb, 11, BB_PATH_A, false, PAPE_RFM);
		halbb_gpio_setting_8852a(bb, 7, BB_PATH_A, true, TRSW_RFM);
		halbb_gpio_setting_8852a(bb, 6, BB_PATH_A, true, LNAON_RFM);
		
		halbb_gpio_setting_8852a(bb, 1, BB_PATH_B, false, PAPE_RFM);
		halbb_gpio_setting_8852a(bb, 2, BB_PATH_B, true, TRSW_RFM);
		halbb_gpio_setting_8852a(bb, 3, BB_PATH_B, true, LNAON_RFM);
	} else {
		BB_DBG(bb, DBG_DBG_API, "Not Support RFE=%d\n", rfe_idx);
	}

}

void halbb_gpio_setting_init_8852a(struct bb_info *bb)
{
	u32 path_cr_base = 0;
	u8 i = 0;

	/*[Delay]*/
	for (i = 0; i < 2; i++) {
		if (i == 0) {
			path_cr_base = 0x7c00;
		} else {
			path_cr_base = 0x7d00;
		}
		halbb_set_reg(bb, (path_cr_base | 0x68), BIT(1), 1); /*	r_tx_ant_sel*/
		halbb_set_reg(bb, (path_cr_base | 0x68), BIT(2), 0);
		halbb_set_reg(bb, (path_cr_base | 0x68), 0xe0, 0);/*convert to TRSW look-up-table*/
		halbb_set_reg(bb, (path_cr_base | 0x80), MASKDWORD, 0x77777777);
		halbb_set_reg(bb, (path_cr_base | 0x84), MASKDWORD, 0x77777777);
	}

	halbb_set_reg(bb, 0xb34, MASKDWORD, 0xffffffff); /*output mode[31:0]*/
	halbb_set_reg(bb, 0xb38, MASKDWORD, 0);	/*non dbg_gpio mode[31:0]*/
	halbb_set_reg(bb, 0xb3c, MASKDWORD, 0);	/*path_sel[15:0]*/
	halbb_set_reg(bb, 0xb40, MASKDWORD, 0);	/*path_sel[16:31]*/

	/*[TRSW Table]*/
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 0, 0, 0, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 0, 0, 1, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 0, 1, 0, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 0, 1, 1, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 1, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 1, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 1, 1, 0, 1, 0);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_A, 1, 1, 1, 1, 0);

	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 0, 0, 0, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 0, 0, 1, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 0, 1, 0, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 0, 1, 1, 1, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 1, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 1, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 1, 1, 0, 1, 0);
	halbb_gpio_trsw_table_8852a(bb, BB_PATH_B, 1, 1, 1, 1, 0);

	/*RFM Setting*/
	halbb_gpio_rfm_8852a(bb, BB_PATH_A, PAPE_RFM, 0, 0, 0, 0x0);
	halbb_gpio_rfm_8852a(bb, BB_PATH_A, LNAON_RFM, 0, 0, 0, 0x4);
	halbb_gpio_rfm_8852a(bb, BB_PATH_A, TRSW_RFM, 0, 0, 0, 0xa);

	halbb_gpio_rfm_8852a(bb, BB_PATH_B, PAPE_RFM, 0, 0, 0, 0x0);
	halbb_gpio_rfm_8852a(bb, BB_PATH_B, LNAON_RFM, 0, 0, 0, 0x4);
	halbb_gpio_rfm_8852a(bb, BB_PATH_B, TRSW_RFM, 0, 0, 0, 0xa);
}


bool halbb_efuse_len_chk_8852a(struct bb_info *bb, enum rtw_efuse_info id,
			       u8 length)
{
	u8 idle_len = 0;

	switch (id) {
	case EFUSE_INFO_BB_ANTDIV:
		idle_len = LEN_BOARD_OPTION;
		break;
	default:
		return false;
	}
	if (length != idle_len || idle_len == 0)
		return false;
	else
		return true;
}

bool halbb_get_efuse_info_8852a(struct bb_info *bb, u8 *efuse_map,
				enum rtw_efuse_info id, void *value, u8 length)
{
	u32 offset = 0;
	u8 default_value = 0;

	// === Efuse Length Check === //
	if (halbb_efuse_len_chk_8852a(bb, id, length))
		return false;

	switch (id) {
	case EFUSE_INFO_BB_ANTDIV:
		offset = OFST_BOARD_OPTION;
		//default_value = INIT_BOARD_OPTION;
		break;
	default:
		return false;
	}

	halbb_mem_cpy(bb, value, (efuse_map + offset), length);
	//value = (*value & 0x8) >> 3;

	return true;
}

void halbb_tssi_bb_reset_8852a(struct bb_info *bb)
{
	// === [TSSI protect on] === //
	halbb_set_reg(bb, 0x7cdc, BIT(30), 0x1);
	halbb_set_reg(bb, 0x7c18, BIT(30), 0x1);
	halbb_set_reg(bb, 0x7ddc, BIT(30), 0x1);
	halbb_set_reg(bb, 0x7d18, BIT(30), 0x1);
	// === [PHY0] === //
	halbb_set_reg(bb, 0x804, 0xff, 0xff);
	halbb_set_reg(bb, 0x804, 0xff, 0);
	halbb_set_reg(bb, 0x804, 0xff, 0xff);
	// === [PHY1] === //
	halbb_set_reg(bb, 0x884, 0xff, 0xff);
	halbb_set_reg(bb, 0x884, 0xff, 0);
	halbb_set_reg(bb, 0x884, 0xff, 0xff);
	// === [TSSI protect off] === //
	halbb_set_reg(bb, 0x7cdc, BIT(30), 0x0);
	halbb_set_reg(bb, 0x7c18, BIT(30), 0x0);
	halbb_set_reg(bb, 0x7ddc, BIT(30), 0x0);
	halbb_set_reg(bb, 0x7d18, BIT(30), 0x0);
}

void halbb_reset_bb_hw_cnt_8852a(struct bb_info *bb)
{

	/*@ Reset all counter*/
	halbb_set_reg_phy0_1(bb, 0x830, BIT(0), 1);
	halbb_set_reg_phy0_1(bb, 0x830, BIT(0), 0);

}

void halbb_ctrl_rf_mode_8852a(struct bb_info *bb,  enum phl_rf_mode mode)
{
	if (mode == RF_MODE_STANDBY) {
		halbb_set_reg(bb, 0x4cac, 0xfffffff0, 0x1111111);
		halbb_set_reg(bb, 0x4cb0, 0xfff, 0x111);
		halbb_set_reg(bb, 0x4dac, 0xfffffff0, 0x1111111);
		halbb_set_reg(bb, 0x4db0, 0xfff, 0x111);
	} else if (mode == RF_MODE_SHUTDOWN) {
		halbb_set_reg(bb, 0x4cac, 0xfffffff0, 0x0);
		halbb_set_reg(bb, 0x4cb0, 0xfff, 0x0);
		halbb_set_reg(bb, 0x4dac, 0xfffffff0, 0x0);
		halbb_set_reg(bb, 0x4db0, 0xfff, 0x0);
	} else {
		halbb_set_reg(bb, 0x4cac, 0xfffffff0, 0x233302);
		halbb_set_reg(bb, 0x4cb0, 0xfff, 0x333);
		halbb_set_reg(bb, 0x4dac, 0xfffffff0, 0x233302);
		halbb_set_reg(bb, 0x4db0, 0xfff, 0x333);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[RF Mode] Mode = %d", mode);
}

u16 halbb_cfg_cmac_tx_ant_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	// Return CMAC [OFST 20] Tx settings //
	/* [19:16] path_en[3:0] ||
	|| [21:20] map_a[1:0]   ||
	|| [23:22] map_b[1:0]   ||
	|| [25:24] map_c[1:0]   ||
	|| [27:26] map_d[1:0]   ||
	|| [28] ant_sel_a[0]    ||
	|| [29] ant_sel_b[0]    ||
	|| [30] ant_sel_c[0]    ||
	|| [31] ant_sel_d[0]    */
	u16 cmac_tx_info = 0;

	if (tx_path == RF_PATH_A) {
		cmac_tx_info = 0x1;
	} else if (tx_path == RF_PATH_B) {
		cmac_tx_info = 0x2;
	} else if (tx_path == RF_PATH_AB) {
		cmac_tx_info = 0x43;
	} else {
		cmac_tx_info = 0xffff;
		BB_WARNING("Invalid Tx Path: %d\n", tx_path);
	}
	return cmac_tx_info;

}

void halbb_ctrl_trx_path_8852a(struct bb_info *bb, enum rf_path tx_path,
			       u8 tx_nss, enum rf_path rx_path, u8 rx_nss)
{
	// Rx Config
	halbb_ctrl_rx_path_8852a(bb, rx_path);

	if ((rx_nss > 2) || (tx_nss > 2)) {
		BB_WARNING("[Invalid Nss]Tx Nss: %d, Rx Nss: %d\n", tx_nss,
			   rx_nss);
		return;
	}

	if (rx_nss == 1) {
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 0);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23), 0);
	} else {
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0x4518, BIT(9) | BIT(8), 1);
		halbb_set_reg(bb, 0x4518, BIT(22) | BIT(21), 1);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0x4580, BIT(16) | BIT(15) | BIT(14), 1);
		halbb_set_reg(bb, 0x4580, BIT(25) | BIT(24) | BIT(23), 1);
	}

	// Tx Config (Tx path_en config by BB reg)
	// Need to Add MP flag for Tx_path API since Normal Drv will also call this function
	halbb_ctrl_tx_path_8852a(bb, tx_path);
}

bool halbb_ctrl_ch2_80p80_8852a(struct bb_info *bb, u8 central_ch)
{
	u8 sco_comp;
	u32 rf_18 = 0;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bb->is_disable_phy_api) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[%s] Disable PHY API\n", __func__);
		return true;
	}

	/*==== Error handling ====*/
	if (central_ch < 36 || (central_ch > 64 && central_ch < 100) ||
		    (central_ch > 144 && central_ch < 149) || central_ch > 177 ) {
		BB_WARNING("Invalid CH:%d for 80p80\n", central_ch);
		return false;
	}

	rf_18 = halbb_read_rf_reg_8852a(bb, RF_PATH_B, 0x18, RFREGOFFSETMASK);

	/*==== [Set RF Reg 0x18] ====*/
	rf_18 &= ~0x303ff; /*[17:16],[9:8],[7:0]*/
	rf_18 |= central_ch; /* Channel*/
	/*==== [5G Setting] ====*/
	rf_18 |= (BIT(16) | BIT(8));

	halbb_write_rf_reg_8852a(bb, RF_PATH_B, 0x18, RFREGOFFSETMASK, rf_18);

	/*==== [SCO compensate fc setting] ====*/
	sco_comp = halbb_sco_mapping_8852a(bb, central_ch);
	halbb_set_reg(bb, 0x1878, 0x3f80, sco_comp);
	/*==== [AGC Settings] ====*/
	//halbb_cfg_agc_8852a(bb, central_ch);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Switch CH Success] CH: %d for 80p80\n",
	       central_ch);
	return true;
}

void halbb_ctrl_dbcc_8852a(struct bb_info *bb, bool dbcc_en)
{
	if (dbcc_en)
		halbb_set_reg(bb, 0x1874, BIT(0), 1);
	else
		halbb_set_reg(bb, 0x1874, BIT(0), 0);
	BB_DBG(bb, DBG_PHY_CONFIG, "[DBCC Enable = %d]\n", dbcc_en);
}

void halbb_cfg_dbcc_8852a(struct bb_info *bb, bool dbcc_en)
{
	if (dbcc_en) {
		// === BB rst === //
		halbb_set_reg(bb, 0x804, BIT(1), 0);
		halbb_set_reg(bb, 0x884, BIT(1), 0);
		// === DBCC En === //
		halbb_ctrl_dbcc_8852a(bb, 1);
		// === Rx settings === //
		halbb_ctrl_rx_path_8852a(bb, RF_PATH_AB);
		// === BB rst === //
		halbb_set_reg(bb, 0x804, BIT(1), 1);
		halbb_set_reg(bb, 0x884, BIT(1), 1);
		// === LO Select === //
		halbb_set_reg(bb, 0xb5c, BIT(11) | BIT(10), 3);
 	} else {
		// === BB rst === //
		halbb_set_reg(bb, 0x804, BIT(1), 0);
		halbb_set_reg(bb, 0x884, BIT(1), 0);
		// === DBCC En === //
		halbb_ctrl_dbcc_8852a(bb, 0);
		// === Rx settings === //
		halbb_ctrl_rx_path_8852a(bb, RF_PATH_AB);
		// === BB rst === //
		halbb_set_reg(bb, 0x804, BIT(1), 1);
		halbb_set_reg(bb, 0x884, BIT(1), 1);
		// === LO Select === //
		halbb_set_reg(bb, 0xb5c, BIT(11) | BIT(10), 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[DBCC cfg] Status = %d\n", dbcc_en);
}

void halbb_ctrl_rx_cca_8852a(struct bb_info *bb, bool cca_en, enum phl_phy_idx phy_idx)
{
	if (cca_en) {
		halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 0, phy_idx);
		halbb_set_reg(bb, 0x7844, BIT(31), 0);
	} else {
		halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 1, phy_idx);
		halbb_set_reg(bb, 0x7844, BIT(31), 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx CCA] CCA_EN = %d\n", cca_en);
}



void halbb_ctrl_ofdm_en_8852a(struct bb_info *bb, bool ofdm_en,
			      enum phl_phy_idx phy_idx)
{
	if (ofdm_en)
		halbb_set_reg_cmn(bb, 0x800, BIT(4), 1, phy_idx);
	else
		halbb_set_reg_cmn(bb, 0x800, BIT(4), 0, phy_idx);

	BB_DBG(bb, DBG_PHY_CONFIG, "[OFDM Enable for PHY%d]\n", phy_idx);
}



// =================== [Power Module] =================== //
bool halbb_set_txpwr_dbm_8852a(struct bb_info *bb, s16 power_dbm,
			       enum phl_phy_idx phy_idx)
{
	bool tmp = false;
	
	power_dbm &= 0x1ff;
	halbb_set_reg_cmn(bb, 0x0da4, BIT(16), 1, phy_idx);
	halbb_set_reg_cmn(bb, 0x1484, 0x7fc00000, power_dbm, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Set Tx pwr(dBm) for [PHY-%d] : %d\n", phy_idx,
	       power_dbm);
	tmp = true;
	return tmp;
}

s16 halbb_get_txpwr_dbm_8852a(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	u32 txpwr_dbm;
	s16 output;
	
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	
	txpwr_dbm = halbb_get_reg_cmn(bb, 0x1484, 0x7fc00000, phy_idx);
	output = (s16)halbb_cnvrt_2_sign(txpwr_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Get Tx pwr(dBm) for [PHY-%d] : %d\n", phy_idx,
	       output);
	return output;
}

s16 halbb_get_txinfo_txpwr_dbm_8852a(struct bb_info *bb)
{
	u32 txpwr_dbm;
	s16 output;
	
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	
	txpwr_dbm = halbb_get_reg(bb, 0x4704, 0x7FC0000);
	output = (s16)halbb_cnvrt_2_sign(txpwr_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Get TxInfo pwr(dBm) : %d\n", output);
	return output;
}

bool halbb_set_cck_txpwr_idx_8852a(struct bb_info *bb, u16 power_idx,
				   enum rf_path tx_path)
{
	u32 pwr_idx_addr[2] = {0x7c08, 0x7d08};
	
	/*==== Power index Check ====*/
	if ((power_idx & ~0x1ff) != 0) {
		BB_WARNING("Power Idx: %x\n", power_idx);
		return false;
	}
	/*==== Tx Path Check ====*/
	if (tx_path > RF_PATH_B) {
		BB_WARNING("Invalid Tx Path for CCK Txpwr_idx setting (52A)\n");
		return false;
	}
	halbb_set_reg(bb, pwr_idx_addr[tx_path], 0x3fe00, power_idx);

	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [CCK] Set Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, power_idx);
	return true;
}

u16 halbb_get_cck_txpwr_idx_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	u16 cck_pwr_idx;
	u32 pwr_idx_addr[2] = {0x7c08, 0x7d08};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	cck_pwr_idx = (u16)halbb_get_reg(bb, pwr_idx_addr[tx_path], 0x3fe00);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [CCK] Get Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, cck_pwr_idx);
	return cck_pwr_idx;
}

s16 halbb_get_cck_ref_dbm_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	u32 cck_ref_dbm;
	u32 pwr_ref_addr[2] = {0x7c08, 0x7d08};
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	cck_ref_dbm = halbb_get_reg(bb, pwr_ref_addr[tx_path], 0x1ff);
	output = (s16)halbb_cnvrt_2_sign(cck_ref_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [CCK] Get Tx ref pwr(dBm) for [Path-%d] : %d\n",
	       tx_path, output);
	return output;
}

bool halbb_set_ofdm_txpwr_idx_8852a(struct bb_info *bb, u16 power_idx,
				    enum rf_path tx_path)
{
	u32 pwr_idx_addr[2] = {0x7c04, 0x7d04};
	
	/*==== Power index Check ====*/
	if ((power_idx & ~0x1ff) != 0) {
		BB_WARNING("Power Idx: %x\n", power_idx);
		return false;
	}
	/*==== Tx Path Check ====*/
	if (tx_path > RF_PATH_B) {
		BB_WARNING("Invalid Tx Path for CCK Txpwr_idx setting (52A)\n");
		return false;
	}

	halbb_set_reg(bb, pwr_idx_addr[tx_path], 0x3fe00, power_idx);

	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [OFDM] Set Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, power_idx);
	return true;
}

u16 halbb_get_ofdm_txpwr_idx_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	u16 ofdm_pwr_idx;
	u32 pwr_idx_addr[2] = {0x7c04, 0x7d04};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	ofdm_pwr_idx = (u16)halbb_get_reg(bb, pwr_idx_addr[tx_path], 0x3fe00);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [OFDM] Get Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, ofdm_pwr_idx);
	return ofdm_pwr_idx;
}

s16 halbb_get_ofdm_ref_dbm_8852a(struct bb_info *bb, enum rf_path tx_path)
{
	u32 ofdm_ref_dbm;
	u32 pwr_ref_addr[2] = {0x7c04, 0x7d04};
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	ofdm_ref_dbm = halbb_get_reg(bb, pwr_ref_addr[tx_path], 0x1ff);
	output = (s16)halbb_cnvrt_2_sign(ofdm_ref_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [OFDM] Get Tx ref pwr(dBm) for [Path-%d] : %d\n",
	       tx_path, output);
	return output;
}

bool halbb_set_txsc_8852a(struct bb_info *bb, u8 txsc, enum phl_phy_idx phy_idx)
{
	/*==== txsc Check ====*/
	if ((txsc & ~0xf) != 0) {
		BB_WARNING("TXSC: %x\n", txsc);
		return false;
	}
	halbb_set_reg_cmn(bb, 0x149c, 0xf0000000, txsc, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] TXSC for [PHY-%d] : %x\n", phy_idx,
	       txsc);
	return true;
}

bool halbb_set_bss_color_8852a(struct bb_info *bb, u8 bss_color, 
			       enum phl_phy_idx phy_idx)
{
	/*==== BSS color Check ====*/
	if ((bss_color & ~0x3f) != 0) {
		BB_WARNING("BSS color: %x\n", bss_color);
		return false;
	}
	//=== [Enable BSS color mapping] ===//
	halbb_set_reg_cmn(bb, 0x12b8, BIT(28), 0x1, phy_idx);
	halbb_set_reg_cmn(bb, 0x12b8, 0xfc00000, bss_color, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] BSS Color for [PHY-%d] : %x\n", phy_idx,
	       bss_color);
	return true;
}

bool halbb_set_sta_id_8852a(struct bb_info *bb, u16 sta_id,
			    enum phl_phy_idx phy_idx)
{
	/*==== Station ID Check ====*/
	if ((sta_id & ~0x7ff) != 0) {
		BB_WARNING("Station ID: %x\n", sta_id);
		return false;
	}
	//=== [Set Station ID] ===//
	halbb_set_reg_cmn(bb, 0x12b8, 0x3ff800, sta_id, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] Station ID for [PHY-%d] : %x\n", phy_idx,
	       sta_id);

	/*==== HE40 ltf 2x Setting ====*/
	if ((bb->phl_sta_info[0]->chandef.bw == CHANNEL_WIDTH_40) &&
	    (bb->phl_sta_info[0]->wmode & WLAN_MD_11AX)) {
		halbb_set_reg(bb, 0x1068, BIT(31), 1);
		halbb_set_reg(bb, 0x1148, MASKDWORD, 0x6DB60010);
		halbb_set_reg(bb, 0x114c, MASKDWORD, 0x11a203);
		BB_DBG(bb, DBG_PHY_CONFIG, "[HE40] AP cap HE / BW40!!\n");
	} else {
		halbb_set_reg(bb, 0x1068, BIT(31), 0);
		BB_DBG(bb, DBG_PHY_CONFIG, "[HE40] Not AP cap HE / BW40!!\n");
	}

	return true;
}

void halbb_set_igi_8852a(struct bb_info *bb, u8 lna_idx, bool tia_idx,
			u8 rxbb_idx, enum rf_path path)
{
	u8 lna = 0;
	bool tia = 0;
	u8 rxbb = 0;

	u32 lna_addr[2] = {0x1590, 0x1650};
	u32 tia_addr[2] = {0x1598, 0x1658};
	u32 rxbb_addr[2] = {0x1580, 0x1640};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	halbb_set_reg(bb, lna_addr[path], BIT(14) | BIT(13) | BIT(12), lna_idx);
	halbb_set_reg(bb, tia_addr[path], BIT(20), tia_idx);
	halbb_set_reg(bb, rxbb_addr[path], 0x3e0, rxbb_idx);

	lna = (u8)halbb_get_reg(bb, lna_addr[path], BIT(14) | BIT(13) | BIT(12));
	tia = (bool)halbb_get_reg(bb, tia_addr[path], BIT(20));
	rxbb = (u8)halbb_get_reg(bb, rxbb_addr[path], 0x3e0);

	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] LNA for [Path-%d] : %d\n", path, lna);
	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] TIA for [Path-%d] : %d\n", path, tia);
	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] RxBB for [Path-%d] : %d\n", path, rxbb);
}

void halbb_info_backup_8852a(struct bb_info *bb, bool is_backup,
			     enum phl_phy_idx phy_idx)
{
	// === Backup / Restore Tx path and pwr_dbm for RF TSSI settings === //
	if (is_backup) {
		bb->bb_8852a_i.tx_path = (u8)halbb_get_reg_cmn(bb,0x147c,
							       0xf0000000,
							       phy_idx);
		bb->bb_8852a_i.pwr_dbm = halbb_get_txpwr_dbm_8852a(bb, phy_idx);
	} else {
		halbb_ctrl_tx_path_8852a(bb, bb->bb_8852a_i.tx_path);
		halbb_set_txpwr_dbm_8852a(bb, bb->bb_8852a_i.pwr_dbm, phy_idx);
	}
}

bool
halbb_ctrl_lps_8852a(struct bb_info *bb, bool lps_en)
{
	bool ret_val = false;
#if 0
	if (lps_en == true) {
		/* backup RF reg0x0
		SysMib.Wlan.PS.PSParm.RxGainPathA = halbb_read_rf_reg_8852a(bb, RF_PATH_A, RF_0x0, RFREGOFFSETMASK);
		SysMib.Wlan.PS.PSParm.RxGainPathB = halbb_read_rf_reg_8852a(bb, RF_PATH_B, RF_0x0, RFREGOFFSETMASK);*/

		/* Reset BB reg & BB clk gating*/
		halbb_set_reg(bb, 0x804, BIT(1), 0);

		ret_val = true;
	} else {
		/* Reset BB reg & release BB clk*/
		halbb_set_reg(bb, 0x804, BIT(1), 1);
		halbb_set_reg(bb, 0x804, BIT(1), 0);
		halbb_set_reg(bb, 0x804, BIT(1), 1);

		ret_val = true;
	}
#endif
	return ret_val;
}

void halbb_dump_bb_reg_8852a(struct bb_info *bb, u32 *_used, char *output,
			     u32 *_out_len)
{
	u32 addr = 0;
	u32 used = *_used;
	u32 out_len = *_out_len;

	BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "BBCR: dump all ==>\n");

	for (addr = 0x0000; addr < 0x0fff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	for (addr = 0x1000; addr < 0x1fff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	for (addr = 0x3800; addr < 0x3bff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	for (addr = 0x4000; addr < 0x4fff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	if (bb->hal_com->dbcc_en) {
		for (addr = 0x5000; addr < 0x5fff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	}

	for (addr = 0x7800; addr < 0x78ff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	for (addr = 0x7a00; addr < 0x7cff; addr += 4)
		BB_DBG_VAST(out_len, used, output + used, out_len - used,
			    "0x%04x 0x%08x\n",
			    addr, halbb_get_reg(bb, addr, MASKDWORD));

	*_used = used;
	*_out_len = out_len;
}

void halbb_dbgport_dump_all_8852a(struct bb_info *bb, u32 *_used, char *output,
				  u32 *_out_len)
{
	const enum bb_dbg_port_ip_t ip_all[] = {
		DBGPORT_IP_TD, DBGPORT_IP_RX_INNER, DBGPORT_IP_TX_INNER,
		DBGPORT_IP_OUTER, DBGPORT_IP_INTF, DBGPORT_IP_CCK,
		DBGPORT_IP_BF, DBGPORT_IP_RX_OUTER};
	u32 ip_num;
	u32 dp = 0; /*debug port value*/
	u32 dp_idx;
	u8 i;

	ip_num = sizeof(ip_all) / sizeof(enum bb_dbg_port_ip_t);

	BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "BB DBG Port: dump all ==>\n");

	for (i = 0; i < ip_num; i++) {
		halbb_set_bb_dbg_port_ip(bb, ip_all[i]);
		for (dp_idx = 0; dp_idx < 0x2e0; dp_idx += 4) {
			halbb_set_bb_dbg_port(bb, dp_idx);
			dp = halbb_get_bb_dbg_port_val(bb);
			BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
				    "[0x%02x, 0x%03x] = 0x%08x\n",
				    ip_all[i], dp_idx, dp);
		}
	}
	halbb_release_bb_dbg_port(bb);
}

void halbb_pre_agc_en_8852a(struct bb_info *bb, bool enable)
{
	u8 en = 0;

	en = (enable == true) ? 1 : 0;
	halbb_set_reg(bb, 0x1594, BIT(28), en);
	halbb_set_reg(bb, 0x1654, BIT(28), en);
	BB_DBG(bb, DBG_DBG_API, "PreAGC en: 0x1594[28]=(0x%x)\n", en);
}

#endif

#endif
