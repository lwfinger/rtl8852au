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

#ifdef BB_8852A_2_SUPPORT

bool halbb_set_pwr_ul_tb_ofst_8852a_2(struct bb_info *bb,
				      s16 pw_ofst, enum phl_phy_idx phy_idx)
{
	/*S(5,0) for 8852A/8852B*/
	if (pw_ofst < -16 || pw_ofst > 15) {
		BB_WARNING("[%s] ofst=%d\n", __func__, pw_ofst);
		return false;
	}

	/*ECO en*/
	rtw_hal_mac_write_msk_pwr_reg(bb->hal_com, (u8)phy_idx, 0xD288, BIT31, 1);

	/*1 TX*/
	rtw_hal_mac_write_msk_pwr_reg(bb->hal_com, (u8)phy_idx, 0xD28c, 0x1f, pw_ofst);

	/*2 TX*/
	if (pw_ofst < -13)
		pw_ofst = -13;
	rtw_hal_mac_write_msk_pwr_reg(bb->hal_com, (u8)phy_idx, 0xD290, 0x1f, pw_ofst - 3);

	return true;
}

void halbb_tx_triangular_shap_cfg_8852a_2(struct bb_info *bb, u8 shape_idx,
					  enum phl_phy_idx phy_idx) {

	halbb_set_reg(bb, 0x4494, BIT(24) | BIT(25), shape_idx);
	/*0:0dB 1:-4dB 2:-5dB 3:-6dB*/
}

void halbb_bb_reset_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);
}

void halbb_bb_reset_all_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);
}

void halbb_bb_reset_en_8852a_2(struct bb_info *bb, bool en, enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);

	if (en)
		halbb_set_reg_cmn(bb, 0x704, BIT(1), 1, phy_idx);
	else
		halbb_set_reg_cmn(bb, 0x704, BIT(1), 0, phy_idx);
}


u32 halbb_read_rf_reg_8852a_2(struct bb_info *bb, enum rf_path path, u32 reg_addr,
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

void halbb_dfs_en_8852a_2(struct bb_info *bb, bool en)
{
	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);

	if (en)
		halbb_set_reg(bb, 0x0, BIT(31), 1);
	else
		halbb_set_reg(bb, 0x0, BIT(31), 0);
}

void halbb_adc_en_8852a_2(struct bb_info *bb, bool en)
{
	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);

	if (en)
		halbb_set_reg(bb, 0x20fc, 0xff000000, 0x0);
	else
		halbb_set_reg(bb, 0x20fc, 0xff000000, 0xf);
}

void halbb_tssi_cont_en_8852a_2(struct bb_info *bb, bool en, enum rf_path path)
{
	u32 tssi_trk_man[2] = {0x5818, 0x7818};

	BB_DBG(bb, DBG_DBG_API, "%s\n", __func__);

	if (en)
		halbb_set_reg(bb, tssi_trk_man[path], BIT(30), 0x0);
	else
		halbb_set_reg(bb, tssi_trk_man[path], BIT(30), 0x1);
}

void halbb_5m_mask_8852a_2(struct bb_info *bb, u8 pri_ch, enum channel_width bw,
			   enum phl_phy_idx phy_idx)
{
	bool mask_5m_low = false;
	bool mask_5m_en = false;

	switch (bw) {
		case CHANNEL_WIDTH_40:
			/* Prich=1 : Mask 5M High
			   Prich=2 : Mask 5M Low */
			mask_5m_en = true;
			mask_5m_low = pri_ch == 2 ? true : false;
			break;
		case CHANNEL_WIDTH_80:
			/* Prich=3 : Mask 5M High
			   Prich=4 : Mask 5M Low
			   Else    : Mask 5M Disable */
			mask_5m_en = ((pri_ch == 3) || (pri_ch == 4)) ? true : false;
			mask_5m_low = pri_ch == 4 ? true : false;
			break;
		default:
			mask_5m_en = false;
			break;
	}

	BB_DBG(bb, DBG_PHY_CONFIG, "[5M Mask] pri_ch = %d, bw = %d", pri_ch, bw);

	if (!mask_5m_en) {
		halbb_set_reg(bb, 0x46b0, BIT(12), 0x0);
		halbb_set_reg(bb, 0x4784, BIT(12), 0x0);
		halbb_set_reg_cmn(bb, 0x4450, BIT(24), 0x0, phy_idx);
	} else {
		if (mask_5m_low) {
			halbb_set_reg(bb, 0x46b0, 0x3f, 0x4);
			halbb_set_reg(bb, 0x46b0, BIT(12), 0x1);
			halbb_set_reg(bb, 0x46b0, BIT(8), 0x0);
			halbb_set_reg(bb, 0x46b0, BIT(6), 0x1);
			halbb_set_reg(bb, 0x4784, 0x3f, 0x4);
			halbb_set_reg(bb, 0x4784, BIT(12), 0x1);
			halbb_set_reg(bb, 0x4784, BIT(8), 0x0);
			halbb_set_reg(bb, 0x4784, BIT(6), 0x1);
		} else {
			halbb_set_reg(bb, 0x46b0, 0x3f, 0x4);
			halbb_set_reg(bb, 0x46b0, BIT(12), 0x1);
			halbb_set_reg(bb, 0x46b0, BIT(8), 0x1);
			halbb_set_reg(bb, 0x46b0, BIT(6), 0x0);
			halbb_set_reg(bb, 0x4784, 0x3f, 0x4);
			halbb_set_reg(bb, 0x4784, BIT(12), 0x1);
			halbb_set_reg(bb, 0x4784, BIT(8), 0x1);
			halbb_set_reg(bb, 0x4784, BIT(6), 0x0);
		}
		halbb_set_reg_cmn(bb, 0x4450, BIT(24), 0x1, phy_idx);
	}
}

u8 halbb_sco_mapping_8852a_2(struct bb_info *bb,  u8 central_ch)
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

bool halbb_ctrl_sco_cck_8852a_2(struct bb_info *bb, u8 pri_ch)
{
	u32 sco_barker_threshold[14] = {0x1cfea, 0x1d0e1, 0x1d1d7, 0x1d2cd,
					0x1d3c3, 0x1d4b9, 0x1d5b0, 0x1d6a6,
					0x1d79c, 0x1d892, 0x1d988, 0x1da7f,
					0x1db75, 0x1ddc4};
	u32 sco_cck_threshold[14] = {0x27de3, 0x27f35, 0x28088, 0x281da,
				     0x2832d, 0x2847f, 0x285d2, 0x28724,
				     0x28877, 0x289c9, 0x28b1c, 0x28c6e,
				     0x28dc1, 0x290ed};

	if (pri_ch > 14) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[CCK SCO Fail]");
		return false;
	}

	halbb_set_reg(bb, 0x23b0, 0x7ffff, sco_barker_threshold[pri_ch - 1]);
	halbb_set_reg(bb, 0x23b4, 0x7ffff, sco_cck_threshold[pri_ch - 1]);
	BB_DBG(bb, DBG_PHY_CONFIG, "[CCK SCO Success]");
	return true;
}

bool halbb_write_rf_reg_8852a_2(struct bb_info *bb, enum rf_path path,
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

void halbb_ctrl_btg_8852a_2(struct bb_info *bb, bool btg)
{
	struct rtw_phl_com_t *phl = bb->phl_com;
	struct dev_cap_t *dev = &phl->dev_cap;

	if (dev->rfe_type >= 50)
		return;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (btg) {
		// Path A
		halbb_set_reg(bb, 0x466c, BIT(18) | BIT(17), 0x1);
		// Path B
		halbb_set_reg(bb, 0x4740, BIT(18) | BIT(17), 0x3);
		BB_DBG(bb, DBG_PHY_CONFIG, "[BT] Apply BTG Setting\n");
		// Apply Grant BT by TMAC Setting
		halbb_set_reg(bb, 0x980, 0x1e0000, 0x0);
		BB_DBG(bb, DBG_PHY_CONFIG, "[BT] Apply Grant BT by TMAC Setting\n");
		// Add BT share
		halbb_set_reg(bb, 0x4978, BIT(14), 0x1);
		halbb_set_reg(bb, 0x4974, 0x3c00000, 0x2);
		/* To avoid abnormal 1R CCA without BT, set rtl only 0xc6c[21] = 0x1 */
		halbb_set_reg(bb, 0x441c, BIT(31), 0x1);
		halbb_set_reg(bb, 0xc6c, BIT(21), 0x1);
	} else {
		// Path A
		halbb_set_reg(bb, 0x466c, BIT(18) | BIT(17), 0x0);
		// Path B
		halbb_set_reg(bb, 0x4740, BIT(18) | BIT(17), 0x0);
		BB_DBG(bb, DBG_PHY_CONFIG, "[BT] Disable BTG Setting\n");
		// Ignore Grant BT by PMAC Setting
		halbb_set_reg(bb, 0x980, 0x1e0000, 0xf);
		halbb_set_reg(bb, 0x980, 0x3c000000, 0x4);
		BB_DBG(bb, DBG_PHY_CONFIG, "[BT] Ignore Grant BT by PMAC Setting\n");
		// Reset BT share
		halbb_set_reg(bb, 0x4978, BIT(14), 0x0);
		halbb_set_reg(bb, 0x4974, 0x3c00000, 0x0);
		/* To avoid abnormal 1R CCA without BT, set rtl only 0xc6c[21] = 0x1 */
		halbb_set_reg(bb, 0x441c, BIT(31), 0x0);
		halbb_set_reg(bb, 0xc6c, BIT(21), 0x0);
	}
}

bool halbb_bw_setting_8852a_2(struct bb_info *bb, enum channel_width bw,
			    enum rf_path path)
{
	u32 rf_reg18 = 0;
	u32 adc_sel[2] = {0x12d0, 0x32d0};
	u32 wbadc_sel[2] = {0x12ec, 0x32ec};

	rf_reg18 = halbb_read_rf_reg_8852a_2(bb, path, 0x18, RFREGOFFSETMASK);
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
	halbb_write_rf_reg_8852a_2(bb, path, 0x18, RFREGOFFSETMASK, rf_reg18);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success][bw_setting] ADC setting for Path-%d\n", path);
	return true;
}

bool halbb_ctrl_bw_8852a_2(struct bb_info *bb, u8 pri_ch, enum channel_width bw,
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
			halbb_set_reg_cmn(bb, 0x4974, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x1 */
			halbb_set_reg_cmn(bb, 0x4978, 0x3000, 0x1, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x4978, 0xf00, 0x0, phy_idx);
		} else if (bw == CHANNEL_WIDTH_10) {
			/*RF_BW:[31:30]=0x0 */
			halbb_set_reg_cmn(bb, 0x4974, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x2 */
			halbb_set_reg_cmn(bb, 0x4978, 0x3000, 0x2, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x4978, 0xf00, 0x0, phy_idx);
		} else if (bw == CHANNEL_WIDTH_20) {
			/*RF_BW:[31:30]=0x0 */
			halbb_set_reg_cmn(bb, 0x4974, 0xC0000000, 0x0,
					  phy_idx);
			/*small BW:[13:12]=0x0 */
			halbb_set_reg_cmn(bb, 0x4978, 0x3000, 0x0, phy_idx);
			/*Pri ch:[11:8]=0x0 */
			halbb_set_reg_cmn(bb, 0x4978, 0xf00, 0x0, phy_idx);
		}

		break;
	case CHANNEL_WIDTH_40:
		/*RF_BW:[31:30]=0x1 */
		halbb_set_reg_cmn(bb, 0x4974, 0xC0000000, 0x1, phy_idx);
		/*small BW:[13:12]=0x0 */
		halbb_set_reg_cmn(bb, 0x4978, 0x3000, 0x0, phy_idx);
		/*Pri ch:[11:8] */
		halbb_set_reg_cmn(bb, 0x4978, 0xf00, pri_ch, phy_idx);
		/*CCK primary channel */
		if (pri_ch == 1)
			halbb_set_reg(bb, 0x237c, BIT(0), 1);
		else
			halbb_set_reg(bb, 0x237c, BIT(0), 0);

		break;
	case CHANNEL_WIDTH_80:
		/*RF_BW:[31:30]=0x2 */
		halbb_set_reg_cmn(bb, 0x4974, 0xC0000000, 0x2, phy_idx);
		/*small BW:[13:12]=0x0 */
		halbb_set_reg_cmn(bb, 0x4978, 0x3000, 0x0, phy_idx);
		/*Pri ch:[11:8] */
		halbb_set_reg_cmn(bb, 0x4978, 0xf00, pri_ch, phy_idx);

		break;
	default:
		BB_WARNING("Fail to switch bw (bw:%d, pri ch:%d)\n", bw,
			   pri_ch);
	}

	if (phy_idx == HW_PHY_0) {
		/*============== [Path A] ==============*/
		halbb_bw_setting_8852a_2(bb, bw, RF_PATH_A);
		/*============== [Path B] ==============*/
		if (!bb->hal_com->dbcc_en)
			halbb_bw_setting_8852a_2(bb, bw, RF_PATH_B);
	} else {
		/*============== [Path B] ==============*/
		halbb_bw_setting_8852a_2(bb, bw, RF_PATH_B);
	}

	BB_DBG(bb, DBG_PHY_CONFIG,
		  "[Switch BW Success] BW: %d for PHY%d\n", bw, phy_idx);

	return true;
}

bool halbb_ch_setting_8852a_2(struct bb_info *bb, u8 central_ch,
			      enum rf_path path, enum band_type band)
{
	u32 rf_reg18 = 0;

	rf_reg18 = halbb_read_rf_reg_8852a_2(bb, path, 0x18, RFREGOFFSETMASK);
	/*==== [Error handling] ====*/
	if (rf_reg18 == INVALID_RF_DATA) {
		BB_WARNING("Invalid RF_0x18 for Path-%d\n", path);
		return false;
	}
	/*==== [Set RF Reg 0x18] ====*/
	rf_reg18 &= ~0x303ff; /*[17:16],[9:8],[7:0]*/
	rf_reg18 |= central_ch; /* Channel*/
	/*==== [5G Setting] ====*/
	if (band == BAND_ON_5G)
		rf_reg18 |= (BIT(16) | BIT(8));
	else if (band == BAND_ON_6G)
		rf_reg18 |= (BIT(17) | BIT(16));
	halbb_write_rf_reg_8852a_2(bb, path, 0x18, RFREGOFFSETMASK, rf_reg18);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Success][ch_setting] CH: %d for Path-%d\n",
	       central_ch, path);
	return true;
}

bool halbb_ctrl_ch_8852a_2(struct bb_info *bb, u8 central_ch, enum band_type band,
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
	if (band != BAND_ON_6G) {
		if ((central_ch > 14 && central_ch < 36) ||
		    (central_ch > 64 && central_ch < 100) ||
		    (central_ch > 144 && central_ch < 149) ||
		    central_ch > 177 ||
		    central_ch== 0) {
			BB_WARNING("Invalid 2/5G CH:%d for PHY%d\n", central_ch,
				   phy_idx);
			return false;
		}
	} else {
		if (central_ch > 253) {
			BB_WARNING("Invalid 6G CH:%d for PHY%d\n", central_ch,
				   phy_idx);
			return false;
		}
	}

	is_2g_ch = (band == BAND_ON_24G) ? true : false;

	if (phy_idx == HW_PHY_0) {
		/*============== [Path A] ==============*/
		halbb_ch_setting_8852a_2(bb, central_ch, RF_PATH_A, band);
		//------------- [Mode Sel - Path A] ------------//
		if (is_2g_ch)
			halbb_set_reg_cmn(bb, 0x4644, BIT(31) | BIT(30), 1,
					  phy_idx);
		else
			halbb_set_reg_cmn(bb, 0x4644, BIT(31) | BIT(30), 0,
					  phy_idx);

		/*============== [Path B] ==============*/
		if (!bb->hal_com->dbcc_en) {
			halbb_ch_setting_8852a_2(bb, central_ch, RF_PATH_B,
						 band);
			//------------- [Mode Sel - Path B] ------------//
			if (is_2g_ch)
				halbb_set_reg_cmn(bb, 0x4718, BIT(31) | BIT(30),
						  1, phy_idx);
			else
				halbb_set_reg_cmn(bb, 0x4718, BIT(31) | BIT(30),
						  0, phy_idx);
			halbb_set_reg(bb, 0x4970, BIT(1), 0);
		} else { /*==== [Phy0 config at 2/5G] ====*/
			if (is_2g_ch)
				halbb_set_reg(bb, 0x4970, BIT(1), 0);
			else
				halbb_set_reg(bb, 0x4970, BIT(1), 1);
		}
		/*==== [SCO compensate fc setting] ====*/
		sco_comp = halbb_sco_mapping_8852a_2(bb, central_ch);
		halbb_set_reg_cmn(bb, 0x4974, 0x7f, sco_comp, phy_idx);
	} else {
		/*============== [Path B] ==============*/
		halbb_ch_setting_8852a_2(bb, central_ch, RF_PATH_B, band);

		//------------- [Mode Sel - Path B] ------------//
		if (is_2g_ch)
			halbb_set_reg_cmn(bb, 0x4718, BIT(31) | BIT(30), 1,
					  phy_idx);
		else
			halbb_set_reg_cmn(bb, 0x4718, BIT(31) | BIT(30), 0,
					  phy_idx);

		/*==== [SCO compensate fc setting] ====*/
		sco_comp = halbb_sco_mapping_8852a_2(bb, central_ch);
		halbb_set_reg_cmn(bb, 0x4974, 0x7f, sco_comp, phy_idx);
	}

	/* === Bandedge ===*/
	if (is_2g_ch)
		halbb_set_reg_cmn(bb, 0x4498, BIT(30), 1, phy_idx);
	else
		halbb_set_reg_cmn(bb, 0x4498, BIT(30), 0, phy_idx);
	/* === CCK Parameters === */
	if (central_ch == 14) {
		halbb_set_reg(bb, 0x2300, 0xffffff, 0x3b13ff);
		halbb_set_reg(bb, 0x2304, 0xffffff, 0x1c42de);
		halbb_set_reg(bb, 0x2308, 0xffffff, 0xfdb0ad);
		halbb_set_reg(bb, 0x230c, 0xffffff, 0xf60f6e);
		halbb_set_reg(bb, 0x2310, 0xffffff, 0xfd8f92);
		halbb_set_reg(bb, 0x2314, 0xffffff, 0x2d011);
		halbb_set_reg(bb, 0x2318, 0xffffff, 0x1c02c);
		halbb_set_reg(bb, 0x231c, 0xffffff, 0xfff00a);
	} else {
		halbb_set_reg(bb, 0x2300, 0xffffff, 0x3d23ff);
		halbb_set_reg(bb, 0x2304, 0xffffff, 0x29b354);
		halbb_set_reg(bb, 0x2308, 0xffffff, 0xfc1c8);
		halbb_set_reg(bb, 0x230c, 0xffffff, 0xfdb053);
		halbb_set_reg(bb, 0x2310, 0xffffff, 0xf86f9a);
		halbb_set_reg(bb, 0x2314, 0xffffff, 0xfaef92);
		halbb_set_reg(bb, 0x2318, 0xffffff, 0xfe5fcc);
		halbb_set_reg(bb, 0x231c, 0xffffff, 0xffdff5);
	}
	if (band != BAND_ON_6G) {
		/* === Set Gain Error === */
		halbb_set_gain_error_8852a_2(bb, central_ch);
		/* === Set Efuse === */
		halbb_set_efuse_8852a_2(bb, central_ch, bb->rx_path, phy_idx);
	}
	/* === Set Ch idx report in phy-sts === */
	halbb_set_reg_cmn(bb, 0x0734, 0x0ff0000, central_ch, phy_idx);
	bb->bb_ch_i.rf_central_ch_cfg = central_ch;

	BB_DBG(bb, DBG_PHY_CONFIG, "[Switch CH Success] CH: %d for PHY%d\n",
	       central_ch, phy_idx);
	return true;
}

void halbb_ctrl_cck_en_8852a_2(struct bb_info *bb, bool cck_en,
			     enum phl_phy_idx phy_idx)
{
	if (cck_en) {
		//halbb_set_reg(bb, 0x2300, BIT(27), 0);
		halbb_set_reg(bb, 0x2344, BIT(31), 0);
	} else {
		//halbb_set_reg(bb, 0x2300, BIT(27), 1);
		halbb_set_reg(bb, 0x2344, BIT(31), 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[CCK Enable for PHY%d]\n", phy_idx);
}

bool halbb_ctrl_bw_ch_8852a_2(struct bb_info *bb, u8 pri_ch, u8 central_ch,
			      enum channel_width bw, enum band_type band,
			      enum phl_phy_idx phy_idx)
{
	bool rpt = true;
	bool cck_en = false;
	bool is_2g_ch;
	u8 pri_ch_idx = 0;

	is_2g_ch = (band == BAND_ON_24G) ? true : false;
	/*==== [Set pri_ch idx] ====*/
	if (is_2g_ch) {
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
		rpt &= halbb_ctrl_sco_cck_8852a_2(bb, pri_ch);

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
	rpt &= halbb_ctrl_ch_8852a_2(bb, central_ch, band, phy_idx);
	/*==== [Switch BW] ====*/
	rpt &= halbb_ctrl_bw_8852a_2(bb, pri_ch_idx, bw, phy_idx);
	/*==== [CCK Enable / Disable] ====*/
	halbb_ctrl_cck_en_8852a_2(bb, cck_en, phy_idx);

	if (band == BAND_ON_5G) {
		/*==== [Spur elimination] ====*/
		if (central_ch == 153) {
			halbb_set_reg(bb, 0x469c, 0xfff, 0x210);
			halbb_set_reg(bb, 0x4770, 0xfff, 0x210);
			halbb_set_reg(bb, 0x42ac, 0xfff, 0x7c0);
			halbb_set_reg(bb, 0x469c, BIT(12), 0x1);
			halbb_set_reg(bb, 0x4770, BIT(12), 0x1);
			halbb_set_reg(bb, 0x42c4, BIT(23), 0x1);
		} else if (central_ch == 151) {
			halbb_set_reg(bb, 0x469c, 0xfff, 0x210);
			halbb_set_reg(bb, 0x4770, 0xfff, 0x210);
			halbb_set_reg(bb, 0x42ac, 0xfff, 0x40);
			halbb_set_reg(bb, 0x469c, BIT(12), 0x1);
			halbb_set_reg(bb, 0x4770, BIT(12), 0x1);
			halbb_set_reg(bb, 0x42c4, BIT(23), 0x1);
		} else if (central_ch == 155) {
			halbb_set_reg(bb, 0x469c, 0xfff, 0x2d0);
			halbb_set_reg(bb, 0x4770, 0xfff, 0x2d0);
			halbb_set_reg(bb, 0x42ac, 0xfff, 0x740);
			halbb_set_reg(bb, 0x469c, BIT(12), 0x1);
			halbb_set_reg(bb, 0x4770, BIT(12), 0x1);
			halbb_set_reg(bb, 0x42c4, BIT(23), 0x1);
		} else {
			halbb_set_reg(bb, 0x469c, BIT(12), 0x0);
			halbb_set_reg(bb, 0x4770, BIT(12), 0x0);
			halbb_set_reg(bb, 0x42c4, BIT(23), 0x0);
		}
	}

	if (is_2g_ch && ((bb->rx_path == RF_PATH_B) || (bb->rx_path == RF_PATH_AB)))
		halbb_ctrl_btg_8852a_2(bb, true);
	else
		halbb_ctrl_btg_8852a_2(bb, false);

	/* Dynamic 5M Mask Setting */
	halbb_5m_mask_8852a_2(bb, pri_ch, bw, phy_idx);

	/*==== [TSSI reset] ====*/
	if (!bb->hal_com->dbcc_en) {
		// Path A
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x3);
		// Path B
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x3);
	} else {
		if (phy_idx == HW_PHY_0) {
			// Path A
			halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x1);
			halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x3);
		} else {
			// Path B
			halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x1);
			halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x3);
		}
	}

	return rpt;
}

void halbb_1r_cca_cr_cfg_8852a_2(struct bb_info *bb, enum rf_path rx_path)
{
	if (rx_path == RF_PATH_A) {
		/*path-A CCA only*/
		halbb_set_reg(bb, 0x32b8, 0x1E0000, 1);
		halbb_set_reg(bb, 0x4974, 0x3C000, 1);
		halbb_set_reg(bb, 0x4444, BIT(31), 1);
	} else if (rx_path == RF_PATH_B) {
		/*path-B CCA only*/
		halbb_set_reg(bb, 0x32b8, 0x1E0000, 0);
		halbb_set_reg(bb, 0x4974, 0x3C000, 2);
		halbb_set_reg(bb, 0x4444, BIT(31), 1);
	} else {
		/*path-AB CCA*/
		halbb_set_reg(bb, 0x32b8, 0x1E0000, 0);
		halbb_set_reg(bb, 0x4974, 0x3C000, 3);
		halbb_set_reg(bb, 0x4444, BIT(31), 0);
	}
}

bool halbb_ctrl_rx_path_8852a_2(struct bb_info *bb, enum rf_path rx_path)
{
	u32 ofdm_rx = 0x0;

	ofdm_rx = (u32)rx_path;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	bb->rx_path = rx_path;

	if (bb->hal_com->dbcc_en) {
		halbb_set_reg(bb, 0x4978, 0xf, 0x1);
		halbb_set_reg(bb, 0x6978, 0xf, 0x2);
		/*==== [PHY0] 1rcca ====*/
		halbb_set_reg(bb, 0x4974, 0x3C000, 1);
		halbb_set_reg(bb, 0x4974, 0x3C0000, 1);
		/*==== [PHY1] 1rcca ====*/
		halbb_set_reg(bb, 0x6974, 0x3C000, 2);
		halbb_set_reg(bb, 0x6974, 0x3C0000, 2);
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 0);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0xd80, 0x3fc0, 4);
		halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23), 0);
		/*==== [PHY1] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0x2d18, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0x2d18, BIT(22) | BIT(21), 0);
		/*==== [PHY1] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0x2d80, 0x3fc0, 1);
		halbb_set_reg(bb, 0x2d80, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0x2d80, BIT(25) | BIT(24) | BIT(23), 0);
		/* === Set Efuse === */
		halbb_set_efuse_8852a_2(bb,bb->hal_com->band[0].cur_chandef.center_ch, RF_PATH_A, HW_PHY_0);
		halbb_set_efuse_8852a_2(bb,bb->hal_com->band[1].cur_chandef.center_ch, RF_PATH_B, HW_PHY_1);
	} else {
		if (ofdm_rx == RF_PATH_A) {
			halbb_set_reg(bb, 0x4978, 0xf, 0x1);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x4974, 0x3C000, 1);
			halbb_set_reg(bb, 0x4974, 0x3C0000, 1);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 0);
			halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 0);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0xd80, 0x3fc0, 4);
			halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14),
				      0);
			halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23),
				      0);
		} else if (ofdm_rx == RF_PATH_B) {
			halbb_set_reg(bb, 0x4978, 0xf, 0x2);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x4974, 0x3C000, 0x2);
			halbb_set_reg(bb, 0x4974, 0x3C0000, 0x2);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 0);
			halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 0);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0xd80, 0x3fc0, 4);
			halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14),
				      0);
			halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23),
				      0);
		} else if (ofdm_rx == RF_PATH_AB) {
			halbb_set_reg(bb, 0x4978, 0xf, 0x3);
			/*==== 1rcca ====*/
			halbb_set_reg(bb, 0x4974, 0x3C000, 0x3);
			halbb_set_reg(bb, 0x4974, 0x3C0000, 0x3);
			/*==== Rx HT nss_limit / mcs_limit ====*/
			halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 1);
			halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 1);
			/*==== Rx HE n_user_max / tb_max_nss ====*/
			halbb_set_reg(bb, 0xd80, 0x3fc0, 4);
			halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14),
				      1);
			halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23),
				      1);
		}
		/* === Set Efuse === */
		halbb_set_efuse_8852a_2(bb,bb->hal_com->band[0].cur_chandef.center_ch, rx_path, HW_PHY_0);
	}

	/* === [BTG setting] === */
	if ((bb->hal_com->band[0].cur_chandef.band == BAND_ON_24G) && ((rx_path == RF_PATH_B) || (rx_path == RF_PATH_AB)))
		halbb_ctrl_btg_8852a_2(bb, true);
	else
		halbb_ctrl_btg_8852a_2(bb, false);

	/*==== [TSSI reset] ====*/
	if (rx_path == RF_PATH_A) {
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x3);
	} else {
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x3);
	}

	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx Success]RX_en=%x\n", rx_path);
	return true;

}

bool halbb_ctrl_tx_path_8852a_2(struct bb_info *bb, enum rf_path tx_path)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);
	/*P-MAC TX path control in MP driver*/
	/*No effect in True-MAC used by normal driver*/

	bb->tx_path = tx_path;

	/*==== [P-MAC] Path & Path_map Enable ====*/
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x7, HW_PHY_0);
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x7, HW_PHY_1);

	if (!bb->hal_com->dbcc_en) {
		if (tx_path == RF_PATH_A) {
			halbb_set_reg(bb, 0x458C, 0xf0000000, 0x1);
			halbb_set_reg(bb, 0x45B4, 0x1e0000, 0x0);
		} else if (tx_path == RF_PATH_B) {
			halbb_set_reg(bb, 0x458C, 0xf0000000, 0x2);
			halbb_set_reg(bb, 0x45B4, 0x1e0000, 0x0);
		} else if (tx_path == RF_PATH_AB) {
			halbb_set_reg(bb, 0x458C, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x45B4, 0x1e0000, 0x4);
		} else {
			BB_WARNING("Invalid Tx Path\n");
			return false;
		}
	} else {
		halbb_set_reg(bb, 0x458C, 0xf0000000, 0x1);
		halbb_set_reg_cmn(bb, 0x458C, 0xf0000000, 0x2, HW_PHY_1);
		halbb_set_reg(bb, 0x45B4, 0x1e0000, 0x0);
		halbb_set_reg_cmn(bb, 0x45B4, 0x1e0000, 0x4, HW_PHY_1);
	}

	/*==== [TSSI reset] ====*/
	if (tx_path == RF_PATH_A) {
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x58dc, BIT(31) | BIT(30), 0x3);
	} else {
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x1);
		halbb_set_reg(bb, 0x78dc, BIT(31) | BIT(30), 0x3);
	}

	return true;
}

#if 0
bool halbb_cfg_rx_path_8852a_2(struct bb_info *bb, enum rf_path rx_path,
			       u8 rx_nss, enum phl_phy_idx phy_idx)
{
	u32 ofdm_rx = 0x0;
	ofdm_rx = (u32)rx_path;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	/*==== [Config Rx path] ====*/
	if (ofdm_rx == RF_PATH_A) {
		halbb_set_reg_cmn(bb, 0x4978, 0xf, 0x1, phy_idx);
		/*==== 1rcca ====*/
		halbb_set_reg_cmn(bb, 0x4974, 0x3C000, 1, phy_idx);
		halbb_set_reg_cmn(bb, 0x4974, 0x3C0000, 1, phy_idx);
	} else if (ofdm_rx == RF_PATH_B) {
		halbb_set_reg_cmn(bb, 0x4978, 0xf, 0x2, phy_idx);
		/*==== 1rcca ====*/
		halbb_set_reg_cmn(bb, 0x4974, 0x3C000, 0x2, phy_idx);
		halbb_set_reg_cmn(bb, 0x4974, 0x3C0000, 0x2, phy_idx);
	} else if (ofdm_rx == RF_PATH_AB) {
		if (phy_idx == HW_PHY_1) {
			BB_WARNING("Invalid Path!!\n");
			return false;
		}
		halbb_set_reg_cmn(bb, 0x4978, 0xf, 0x3, phy_idx);
		/*==== 1rcca ====*/
		halbb_set_reg_cmn(bb, 0x4974, 0x3C000, 0x3, phy_idx);
		halbb_set_reg_cmn(bb, 0x4974, 0x3C0000, 0x3, phy_idx);
	} else {
		BB_WARNING("Invalid Rx Path!!\n");
		return false;
	}

	/*==== [Config Rx Nss] ====*/
	if (rx_nss == 1) {
		/*==== Rx ht_mcs_limit / vht_nss_limit ====*/
		halbb_set_reg_cmn(bb, 0xd18, BIT(9) | BIT(8), 0, phy_idx);
		halbb_set_reg_cmn(bb, 0xd18, BIT(22) | BIT(21), 0, phy_idx);
		/*==== Rx he_max_nss / tb_max_nss ====*/
		halbb_set_reg_cmn(bb, 0xd80, BIT(16) | BIT(15) | BIT(14),
				  0, phy_idx);
		halbb_set_reg_cmn(bb, 0xd80, BIT(25) | BIT(24) | BIT(23),
				  0, phy_idx);
	} else if (rx_nss == 2) {
		/*==== Rx ht_mcs_limit / vht_nss_limit ====*/
		halbb_set_reg_cmn(bb, 0xd18, BIT(9) | BIT(8), 1, phy_idx);
		halbb_set_reg_cmn(bb, 0xd18, BIT(22) | BIT(21), 1, phy_idx);
		/*==== Rx he_max_nss / tb_max_nss ====*/
		halbb_set_reg_cmn(bb, 0xd80, BIT(16) | BIT(15) | BIT(14),
				  1, phy_idx);
		halbb_set_reg_cmn(bb, 0xd80, BIT(25) | BIT(24) | BIT(23),
				  1, phy_idx);
	} else {
		BB_WARNING("Invalid Rx Nss!!\n");
		return false;
	}

	/*==== [Config Rx Nss] ====*/
	if (phy_idx == HW_PHY_0) {
		/*==== Rx HE n_user_max ====*/
		halbb_set_reg_cmn(bb, 0xd80, 0x3fc0, 4, phy_idx);
	} else if (phy_idx == HW_PHY_1) {
		/*==== Rx HE n_user_max ====*/
		halbb_set_reg_cmn(bb, 0xd80, 0x3fc0, 1, phy_idx);
	} else {
		BB_WARNING("Invalid PHY Index!!\n");
		return false;
	}
	/*==== [BB reset] ====*/
	halbb_bb_reset_8852a_2(bb, phy_idx);

	return true;
}

bool halbb_cfg_tx_path_8852a_2(struct bb_info *bb, enum rf_path tx_path,
			       u8 tx_nss, enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	/*==== [P-MAC] Path & Path_map Enable ====*/
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x7, phy_idx);

	if (tx_path == RF_PATH_A) {
		halbb_set_reg_cmn(bb, 0x458C, 0xf0000000, 0x1, phy_idx);
		halbb_set_reg_cmn(bb, 0x45B4, 0x1e0000, 0x0, phy_idx);
		if (tx_nss > 1) {
			BB_WARNING("Invalid Tx Nss!!\n");
			return false;
		}
	} else if (tx_path == RF_PATH_B) {
		halbb_set_reg_cmn(bb, 0x458C, 0xf0000000, 0x2, phy_idx);
		halbb_set_reg_cmn(bb, 0x45B4, 0x1e0000, 0x0, phy_idx);
		if (tx_nss > 1) {
			BB_WARNING("Invalid Tx Nss!!\n");
			return false;
		}
	} else if (tx_path == RF_PATH_AB) {
		halbb_set_reg_cmn(bb, 0x458C, 0xf0000000, 0x3, phy_idx);
		halbb_set_reg_cmn(bb, 0x45B4, 0x1e0000, 0x4, phy_idx);
	} else {
		BB_WARNING("Invalid Tx Path\n");
		return false;
	}
	/*==== [BB reset] ====*/
	halbb_bb_reset_8852a_2(bb, phy_idx);

	return true;
}
#endif

#ifdef HALBB_DBCC_SUPPORT
u32 halbb_phy0_to_phy1_ofst_8852a_2(struct bb_info *bb, u32 addr)
{
	u32 phy_page = addr >> 8;
	u32 ofst = 0;

	if ((phy_page >= 0x40) && (phy_page <= 0x4f))
		return 0x2000;

	switch (phy_page) {
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xa:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0x19:
	case 0x1a:
	case 0x1b:
		ofst = 0x2000;
		break;
	default:
		ofst = 0; /*phy1 or warning case*/
		break;
	}

	return ofst;
}
#endif

void halbb_gpio_ctrl_dump_8852a_2(struct bb_info *bb)
{
	u32 rfe_cr_table[] = {0x0334, 0x0338, 0x033c,
			      0x5868, 0x5894, 0x7868, 0x7894,
			      0x5890, 0x7890, 0x5880, 0x5884, 0x7880, 0x7884};
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

void halbb_gpio_rfm_8852a_2(struct bb_info *bb, enum bb_path path,
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
		path_cr_base = 0x5800;
	} else {
		path_cr_base = 0x7800;
	}

	cr_tmp = path_cr_base | 0x94;
	val_tmp = (u8)dis_tx_gnt_wl << 7 | (u8)active_tx_opt << 6 |
		  (u8)act_bt_en << 5 | rfm_output_val;
	mask = 0xff << mask_ofst;

	halbb_set_reg(bb, cr_tmp, mask, val_tmp);

	BB_DBG(bb, DBG_DBG_API, "0x%x[0x%x]=%d\n",cr_tmp, mask, val_tmp);
}

void halbb_gpio_trsw_table_8852a_2(struct bb_info *bb, enum bb_path path,
				 bool tx_path_en, bool trsw_tx,
				 bool trsw_rx, bool trsw, bool trsw_b)
{
	u32 path_cr_base = 0;
	u32 cr_tmp = 0;
	u32 val_tmp = 0;
	u32 mask_ofst = 16;

	BB_DBG(bb, DBG_DBG_API,
	       "[path %d]{tx_path_en:%d, trsw_tx:%d, trsw_rx:%d] = {trsw:%d, trsw_b:%d}\n",
	       path, tx_path_en, trsw_tx, trsw_rx, trsw, trsw_b);

	if (path == BB_PATH_A) {
		path_cr_base = 0x5800;
	} else {
		path_cr_base = 0x7800;
	}

	cr_tmp = path_cr_base | 0x68;
	mask_ofst += ((u8)tx_path_en << 2 | (u8)trsw_tx << 1 | (u8)trsw_rx) << 1;
	val_tmp = (u8)trsw << 1 | (u8)trsw_b;

	halbb_set_reg(bb, cr_tmp, 0x3 << mask_ofst, val_tmp);

	BB_DBG(bb, DBG_DBG_API, "0x%x[0x%x]=%d\n",cr_tmp, mask_ofst, val_tmp);
}

void halbb_gpio_setting_8852a_2(struct bb_info *bb, u8 gpio_idx,
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
		path_cr_base = 0x5800;
	} else {
		path_sel = 1;
		path_cr_base = 0x7800;
	}

	/*[Path_mux_sel]*/
	if (gpio_idx <= 15) {
		cr_tmp = 0x33c;
		mask_tmp = ((u32)0x3) << (gpio_idx << 1);
	} else {
		cr_tmp = 0x340;
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

void halbb_gpio_setting_all_8852a_2(struct bb_info *bb, u8 rfe_idx)
{

	if (rfe_idx == 51) {
		halbb_gpio_setting_8852a_2(bb, 11, BB_PATH_A, false, PAPE_RFM);
		halbb_gpio_setting_8852a_2(bb, 7, BB_PATH_A, true, TRSW_RFM);
		halbb_gpio_setting_8852a_2(bb, 6, BB_PATH_A, false, LNAON_RFM);

		halbb_gpio_setting_8852a_2(bb, 1, BB_PATH_B, false, PAPE_RFM);
		halbb_gpio_setting_8852a_2(bb, 2, BB_PATH_B, true, TRSW_RFM);
		halbb_gpio_setting_8852a_2(bb, 3, BB_PATH_B, false, LNAON_RFM);

	} else if (rfe_idx == 53) {
		halbb_gpio_setting_8852a_2(bb, 11, BB_PATH_A, false, PAPE_RFM);
		halbb_gpio_setting_8852a_2(bb, 7, BB_PATH_A, true, TRSW_RFM);
		halbb_gpio_setting_8852a_2(bb, 6, BB_PATH_A, true, LNAON_RFM);

		halbb_gpio_setting_8852a_2(bb, 1, BB_PATH_B, false, PAPE_RFM);
		halbb_gpio_setting_8852a_2(bb, 2, BB_PATH_B, true, TRSW_RFM);
		halbb_gpio_setting_8852a_2(bb, 3, BB_PATH_B, true, LNAON_RFM);
	} else {
		BB_DBG(bb, DBG_DBG_API, "Not Support RFE=%d\n", rfe_idx);
	}

}

void halbb_gpio_setting_init_8852a_2(struct bb_info *bb)
{
	u32 path_cr_base = 0;
	u8 i = 0;

	/*[Delay]*/
	for (i = 0; i < 2; i++) {
		if (i == 0) {
			path_cr_base = 0x5800;
		} else {
			path_cr_base = 0x7800;
		}
		halbb_set_reg(bb, (path_cr_base | 0x68), BIT(1), 1); /*	r_tx_ant_sel*/
		halbb_set_reg(bb, (path_cr_base | 0x68), BIT(2), 0);
		halbb_set_reg(bb, (path_cr_base | 0x68), 0xe0, 0);/*convert to TRSW look-up-table*/
		halbb_set_reg(bb, (path_cr_base | 0x80), MASKDWORD, 0x77777777);
		halbb_set_reg(bb, (path_cr_base | 0x84), MASKDWORD, 0x77777777);
	}

	halbb_set_reg(bb, 0x334, MASKDWORD, 0xffffffff); /*output mode[31:0]*/
	halbb_set_reg(bb, 0x338, MASKDWORD, 0);	/*non dbg_gpio mode[31:0]*/
	halbb_set_reg(bb, 0x33c, MASKDWORD, 0);	/*path_sel[15:0]*/
	halbb_set_reg(bb, 0x340, MASKDWORD, 0);	/*path_sel[16:31]*/

	/*[TRSW Table]*/
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 0, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 0, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 0, 1, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 0, 1, 1, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 1, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 1, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 1, 1, 0, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_A, 1, 1, 1, 1, 0);

	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 0, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 0, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 0, 1, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 0, 1, 1, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 1, 0, 0, 0, 1);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 1, 0, 1, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 1, 1, 0, 1, 0);
	halbb_gpio_trsw_table_8852a_2(bb, BB_PATH_B, 1, 1, 1, 1, 0);

	/*RFM Setting*/
	halbb_gpio_rfm_8852a_2(bb, BB_PATH_A, PAPE_RFM, 0, 0, 0, 0x0);
	halbb_gpio_rfm_8852a_2(bb, BB_PATH_A, LNAON_RFM, 0, 0, 0, 0x4);
	halbb_gpio_rfm_8852a_2(bb, BB_PATH_A, TRSW_RFM, 0, 0, 0, 0xa);

	halbb_gpio_rfm_8852a_2(bb, BB_PATH_B, PAPE_RFM, 0, 0, 0, 0x0);
	halbb_gpio_rfm_8852a_2(bb, BB_PATH_B, LNAON_RFM, 0, 0, 0, 0x4);
	halbb_gpio_rfm_8852a_2(bb, BB_PATH_B, TRSW_RFM, 0, 0, 0, 0xa);
}

void halbb_ctrl_rf_mode_8852a_2(struct bb_info *bb,  enum phl_rf_mode mode)
{
	if (mode == RF_MODE_STANDBY) {
		halbb_set_reg(bb, 0x12ac, 0xfffffff0, 0x1111111);
		halbb_set_reg(bb, 0x12b0, 0xfff, 0x111);
		halbb_set_reg(bb, 0x32ac, 0xfffffff0, 0x1111111);
		halbb_set_reg(bb, 0x32b0, 0xfff, 0x111);
	} else if (mode == RF_MODE_SHUTDOWN) {
		halbb_set_reg(bb, 0x12ac, 0xfffffff0, 0x0);
		halbb_set_reg(bb, 0x12b0, 0xfff, 0x0);
		halbb_set_reg(bb, 0x32ac, 0xfffffff0, 0x0);
		halbb_set_reg(bb, 0x32b0, 0xfff, 0x0);
	} else {
		halbb_set_reg(bb, 0x12ac, 0xfffffff0, 0x1233312);
		halbb_set_reg(bb, 0x12b0, 0xfff, 0x333);
		halbb_set_reg(bb, 0x32ac, 0xfffffff0, 0x1233312);
		halbb_set_reg(bb, 0x32b0, 0xfff, 0x333);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[RF Mode] Mode = %d", mode);
}

u16 halbb_cfg_cmac_tx_ant_8852a_2(struct bb_info *bb, enum rf_path tx_path)
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

void halbb_ctrl_trx_path_8852a_2(struct bb_info *bb, enum rf_path tx_path,
			       u8 tx_nss, enum rf_path rx_path, u8 rx_nss)
{
	/*normal driver use only*/
	// Rx Config
	halbb_ctrl_rx_path_8852a_2(bb, rx_path);

	if ((rx_nss > 2) || (tx_nss > 2)) {
		BB_WARNING("[Invalid Nss]Tx Nss: %d, Rx Nss: %d\n", tx_nss,
			   rx_nss);
		return;
	}

	if (rx_nss == 1) {
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 0);
		halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 0);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14), 0);
		halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23), 0);
	} else {
		/*==== [PHY0] Rx HT nss_limit / mcs_limit ====*/
		halbb_set_reg(bb, 0xd18, BIT(9) | BIT(8), 1);
		halbb_set_reg(bb, 0xd18, BIT(22) | BIT(21), 1);
		/*==== [PHY0] Rx HE n_user_max / tb_max_nss ====*/
		halbb_set_reg(bb, 0xd80, BIT(16) | BIT(15) | BIT(14), 1);
		halbb_set_reg(bb, 0xd80, BIT(25) | BIT(24) | BIT(23), 1);
	}

	// Tx Config (to do)
	// Need to Add MP flag for Tx_path API since Normal Drv will also call this function
	// ==== [T-MAC] Path & Path_map Enable ==== //
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x0, HW_PHY_0); /*T-MAC TX*/
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x0, HW_PHY_1);
}
#if 0
void halbb_cfg_trx_path_8852a_2(struct bb_info *bb, enum rf_path tx_path,
				u8 tx_nss, enum rf_path rx_path, u8 rx_nss,
				enum phl_phy_idx phy_idx, bool tmac_en)
{
	if ((rx_nss > 2) || (tx_nss > 2)) {
		BB_WARNING("[Invalid Nss]Tx Nss: %d, Rx Nss: %d\n", tx_nss,
			   rx_nss);
		return;
	}
	// ==== Rx Config ==== //
	halbb_cfg_rx_path_8852a_2(bb, rx_path, rx_nss, phy_idx);
	// ==== Tx Config ==== //
	if (tmac_en)
		halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0x0, phy_idx);
	else
		halbb_cfg_tx_path_8852a_2(bb, tx_path, tx_nss, phy_idx);
}
#endif
void halbb_tssi_bb_reset_8852a_2(struct bb_info *bb)
{

}

void halbb_ctrl_dbcc_8852a_2(struct bb_info *bb, bool dbcc_en)
{
	if (dbcc_en)
		halbb_set_reg(bb, 0x4970, BIT(0), 1);
	else
		halbb_set_reg(bb, 0x4970, BIT(0), 0);
	BB_DBG(bb, DBG_PHY_CONFIG, "[DBCC Enable = %d]\n", dbcc_en);
}

void halbb_cfg_dbcc_8852a_2(struct bb_info *bb, bool dbcc_en)
{
	if (dbcc_en) {
		// === BB rst === //
		halbb_set_reg(bb, 0x704, BIT(1), 0);
		halbb_set_reg(bb, 0x2704, BIT(1), 0);
		// === DBCC En === //
		halbb_ctrl_dbcc_8852a_2(bb, 1);
		// === Rx settings === //
		halbb_ctrl_rx_path_8852a_2(bb, RF_PATH_AB);
		// === BB rst === //
		halbb_set_reg(bb, 0x704, BIT(1), 1);
		halbb_set_reg(bb, 0x2704, BIT(1), 1);
		// === LO Select === //
		halbb_set_reg(bb, 0x35c, BIT(11) | BIT(10), 3);
 	} else {
		// === BB rst === //
		halbb_set_reg(bb, 0x704, BIT(1), 0);
		halbb_set_reg(bb, 0x2704, BIT(1), 0);
		// === DBCC En === //
		halbb_ctrl_dbcc_8852a_2(bb, 0);
		// === Rx settings === //
		halbb_ctrl_rx_path_8852a_2(bb, RF_PATH_AB);
		// === BB rst === //
		halbb_set_reg(bb, 0x704, BIT(1), 1);
		halbb_set_reg(bb, 0x2704, BIT(1), 1);
		// === LO Select === //
		halbb_set_reg(bb, 0x35c, BIT(11) | BIT(10), 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[DBCC cfg] Status = %d\n", dbcc_en);
}

void halbb_ctrl_rx_cca_8852a_2(struct bb_info *bb, bool cca_en, enum phl_phy_idx phy_idx)
{
	bool band_2g;

	band_2g = (bb->hal_com->band[phy_idx].cur_chandef.band == BAND_ON_24G) ?
		  true : false;

	if (cca_en) {
		halbb_set_reg_cmn(bb, 0xc3c, BIT(9), 0, phy_idx);
		if (band_2g)
			halbb_set_reg(bb, 0x2344, BIT(31), 0);
	} else {
		halbb_set_reg_cmn(bb, 0xc3c, BIT(9), 1, phy_idx);
		if (band_2g)
			halbb_set_reg(bb, 0x2344, BIT(31), 1);
		// BB reset
		halbb_bb_reset_en_8852a_2(bb, false, phy_idx);
		halbb_bb_reset_en_8852a_2(bb, true, phy_idx);
		// Delay 1us
		halbb_delay_us(bb, 1);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx CCA] CCA_EN = %d\n", cca_en);
}



void halbb_ctrl_ofdm_en_8852a_2(struct bb_info *bb, bool ofdm_en,
			      enum phl_phy_idx phy_idx)
{
	if (ofdm_en)
		halbb_set_reg_cmn(bb, 0x700, BIT(4), 1, phy_idx);
	else
		halbb_set_reg_cmn(bb, 0x700, BIT(4), 0, phy_idx);

	BB_DBG(bb, DBG_PHY_CONFIG, "[OFDM Enable for PHY%d]\n", phy_idx);
}



// =================== [Power Module] =================== //
bool halbb_set_txpwr_dbm_8852a_2(struct bb_info *bb, s16 power_dbm,
			       enum phl_phy_idx phy_idx)
{
	bool tmp = false;

	power_dbm &= 0x1ff;
	halbb_set_reg_cmn(bb, 0x09a4, BIT(16), 1, phy_idx);
	halbb_set_reg_cmn(bb, 0x4594, 0x7fc00000, power_dbm, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Set Tx pwr(dBm) for [PHY-%d] : %d\n", phy_idx,
	       power_dbm);
	tmp = true;
	return tmp;
}

s16 halbb_get_txpwr_dbm_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	u32 txpwr_dbm;
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	txpwr_dbm = halbb_get_reg_cmn(bb, 0x4594, 0x7fc00000, phy_idx);
	output = (s16)halbb_cnvrt_2_sign(txpwr_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Get Tx pwr(dBm) for [PHY-%d] : %d\n", phy_idx,
	       output);
	return output;
}

s16 halbb_get_txinfo_txpwr_dbm_8852a_2(struct bb_info *bb)
{
	u32 txpwr_dbm;
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	txpwr_dbm = halbb_get_reg(bb, 0x1804, 0x7FC0000);
	output = (s16)halbb_cnvrt_2_sign(txpwr_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] Get TxInfo pwr(dBm) : %d\n", output);
	return output;
}

bool halbb_set_cck_txpwr_idx_8852a_2(struct bb_info *bb, u16 power_idx,
				   enum rf_path tx_path)
{
	u32 pwr_idx_addr[2] = {0x5808, 0x7808};

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

u16 halbb_get_cck_txpwr_idx_8852a_2(struct bb_info *bb, enum rf_path tx_path)
{
	u16 cck_pwr_idx;
	u32 pwr_idx_addr[2] = {0x5808, 0x7808};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	cck_pwr_idx = (u16)halbb_get_reg(bb, pwr_idx_addr[tx_path], 0x3fe00);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [CCK] Get Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, cck_pwr_idx);
	return cck_pwr_idx;
}

s16 halbb_get_cck_ref_dbm_8852a_2(struct bb_info *bb, enum rf_path tx_path)
{
	u32 cck_ref_dbm;
	u32 pwr_ref_addr[2] = {0x5808, 0x7808};
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	cck_ref_dbm = halbb_get_reg(bb, pwr_ref_addr[tx_path], 0x1ff);
	output = (s16)halbb_cnvrt_2_sign(cck_ref_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [CCK] Get Tx ref pwr(dBm) for [Path-%d] : %d\n",
	       tx_path, output);
	return output;
}

bool halbb_set_ofdm_txpwr_idx_8852a_2(struct bb_info *bb, u16 power_idx,
				    enum rf_path tx_path)
{
	u32 pwr_idx_addr[2] = {0x5804, 0x7804};

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

u16 halbb_get_ofdm_txpwr_idx_8852a_2(struct bb_info *bb, enum rf_path tx_path)
{
	u16 ofdm_pwr_idx;
	u32 pwr_idx_addr[2] = {0x5804, 0x7804};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	ofdm_pwr_idx = (u16)halbb_get_reg(bb, pwr_idx_addr[tx_path], 0x3fe00);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [OFDM] Get Tx pwr idx for [Path-%d] : %x\n",
	       tx_path, ofdm_pwr_idx);
	return ofdm_pwr_idx;
}

s16 halbb_get_ofdm_ref_dbm_8852a_2(struct bb_info *bb, enum rf_path tx_path)
{
	u32 ofdm_ref_dbm;
	u32 pwr_ref_addr[2] = {0x5804, 0x7804};
	s16 output;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	ofdm_ref_dbm = halbb_get_reg(bb, pwr_ref_addr[tx_path], 0x1ff);
	output = (s16)halbb_cnvrt_2_sign(ofdm_ref_dbm, 9);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [OFDM] Get Tx ref pwr(dBm) for [Path-%d] : %d\n",
	       tx_path, output);
	return output;
}

void halbb_reset_bb_hw_cnt_8852a_2(struct bb_info *bb)
{

	/*@ Reset all counter*/
	halbb_set_reg_phy0_1(bb, 0x730, BIT(0), 1);
	halbb_set_reg_phy0_1(bb, 0x730, BIT(0), 0);

}

void halbb_backup_info_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	/*==== This Backup info is for RF TSSI calibration =====*/
	bb->bb_cmn_backup_i.cur_tx_path = (u8)halbb_get_reg_cmn(bb, 0x458c, 0xf0000000, phy_idx);
	bb->bb_cmn_backup_i.cur_tx_pwr = halbb_get_txpwr_dbm_8852a_2(bb, phy_idx);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Backup Info] [PHY%d] Tx path = %x\n", phy_idx, bb->bb_cmn_backup_i.cur_tx_path);
	BB_DBG(bb, DBG_PHY_CONFIG, "[Backup Info] [PHY%d] Tx pwr = %x\n", phy_idx, (u16)bb->bb_cmn_backup_i.cur_tx_pwr);
}

void halbb_restore_info_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	/*==== This Restore info is for RF TSSI calibration =====*/
	halbb_set_reg_cmn(bb, 0x458c, 0xf0000000, bb->bb_cmn_backup_i.cur_tx_path, phy_idx);
	halbb_set_txpwr_dbm_8852a_2(bb, bb->bb_cmn_backup_i.cur_tx_pwr, phy_idx);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Restore Info] [PHY%d] Tx path = %x\n", phy_idx, bb->bb_cmn_backup_i.cur_tx_path);
	BB_DBG(bb, DBG_PHY_CONFIG, "[Restore Info] [PHY%d] Tx pwr = %x\n", phy_idx, (u16)bb->bb_cmn_backup_i.cur_tx_pwr);
}

bool halbb_set_txsc_8852a_2(struct bb_info *bb, u8 txsc, enum phl_phy_idx phy_idx)
{
	/*==== txsc Check ====*/
	if ((txsc & ~0xf) != 0) {
		BB_WARNING("TXSC: %x\n", txsc);
		return false;
	}
	halbb_set_reg_cmn(bb, 0x45ac, 0x7800000, txsc, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] TXSC for [PHY-%d] : %x\n", phy_idx,
	       txsc);
	return true;
}

void halbb_ctrl_btc_preagc_8852a_2(struct bb_info *bb, bool bt_en)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bt_en) {
		halbb_set_reg(bb, 0x4624, 0x1fc000, 0x40);
		halbb_set_reg(bb, 0x46f8, 0x1fc000, 0x40);
		halbb_set_reg(bb, 0x4674, BIT(20) | BIT(19), 0x2);
		halbb_set_reg(bb, 0x4748, BIT(20) | BIT(19), 0x2);
		halbb_set_reg(bb, 0x4650, 0x7c00, 0x18);
		halbb_set_reg(bb, 0x4724, 0x7c00, 0x18);
		halbb_set_reg(bb, 0x4688, BIT(1) | BIT(0), 0x3);
		halbb_set_reg(bb, 0x475c, BIT(1) | BIT(0), 0x3);
	} else {
		halbb_set_reg(bb, 0x4624, 0x1fc000, 0x1a);
		halbb_set_reg(bb, 0x46f8, 0x1fc000, 0x1a);
		halbb_set_reg(bb, 0x4674, BIT(20) | BIT(19), 0x1);
		halbb_set_reg(bb, 0x4748, BIT(20) | BIT(19), 0x1);
		halbb_set_reg(bb, 0x4650, 0x7c00, 0x12);
		halbb_set_reg(bb, 0x4724, 0x7c00, 0x12);
		halbb_set_reg(bb, 0x4688, BIT(1) | BIT(0), 0x0);
		halbb_set_reg(bb, 0x475c, BIT(1) | BIT(0), 0x0);
	}
}

bool halbb_set_bss_color_8852a_2(struct bb_info *bb, u8 bss_color, 
			       enum phl_phy_idx phy_idx)
{
	/*==== BSS color Check ====*/
	if ((bss_color & ~0x3f) != 0) {
		BB_WARNING("BSS color: %x\n", bss_color);
		return false;
	}
	//=== [Enable BSS color mapping] ===//
	halbb_set_reg_cmn(bb, 0x43ac, BIT(28), 0x1, phy_idx);
	halbb_set_reg_cmn(bb, 0x43ac, 0xfc00000, bss_color, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] BSS Color for [PHY-%d] : %x\n", phy_idx,
	       bss_color);
	return true;
}

bool halbb_set_sta_id_8852a_2(struct bb_info *bb, u16 sta_id,
			    enum phl_phy_idx phy_idx)
{
	/*==== Station ID Check ====*/
	if ((sta_id & ~0x7ff) != 0) {
		BB_WARNING("Station ID: %x\n", sta_id);
		return false;
	}
	//=== [Set Station ID] ===//
	halbb_set_reg_cmn(bb, 0x43ac, 0x3ff800, sta_id, phy_idx);
	BB_DBG(bb, DBG_PHY_CONFIG,
	       "[Success] [P-MAC] Station ID for [PHY-%d] : %x\n", phy_idx,
	       sta_id);

	return true;
}

void halbb_set_igi_8852a_2(struct bb_info *bb, u8 lna_idx, bool tia_idx,
			u8 rxbb_idx, enum rf_path path)
{
	u8 lna = 0;
	bool tia = 0;
	u8 rxbb = 0;

	u32 lna_addr[2] = {0x4668, 0x473c};
	u32 tia_addr[2] = {0x4674, 0x4748};
	u32 rxbb_addr[2] = {0x4658, 0x472C};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	halbb_set_reg(bb, lna_addr[path], BIT(26) | BIT(25) | BIT(24), lna_idx);
	halbb_set_reg(bb, tia_addr[path], BIT(17), tia_idx);
	halbb_set_reg(bb, rxbb_addr[path], 0x3e0, rxbb_idx);

	lna = (u8)halbb_get_reg(bb, lna_addr[path], BIT(26) | BIT(25) | BIT(24));
	tia = (bool)halbb_get_reg(bb, tia_addr[path], BIT(17));
	rxbb = (u8)halbb_get_reg(bb, rxbb_addr[path], 0x3e0);

	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] LNA for [Path-%d] : %d\n", path, lna);
	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] TIA for [Path-%d] : %d\n", path, tia);
	BB_DBG(bb, DBG_PHY_CONFIG, "[IGI] RxBB for [Path-%d] : %d\n", path, rxbb);
}

void halbb_set_tx_pow_ref_8852a_2(struct bb_info *bb, s16 pw_dbm_ofdm, /*s(9,2)*/
				  s16 pw_dbm_cck, s8 ofst,
				  u8 base_cw_0db, u16 tssi_16dBm_cw,
				  u16 *ofdm_cw, u16 *cck_cw,
				  enum phl_phy_idx phy_idx)
{
	s16 rf_pw_cw = 0;
	u32 pw_cw = 0;
	u32 val_ofdm = 0, val_cck = 0;
	s16 pw_s10_3 = 0;
	u32 tssi_ofst_cw = 0;
	bool cfg_path_a = true, cfg_path_b = true;

	/*==== [OFDM] =======================================================*/
	pw_s10_3 = (pw_dbm_ofdm * 2) + (s16)(ofst) + (s16)(base_cw_0db * 8);


	pw_cw = pw_s10_3;
	rf_pw_cw = (pw_s10_3 & 0x1F8) >> 3;

	if (rf_pw_cw > 63)
		pw_cw = (63 << 3) | (pw_s10_3 & 0x7); /*upper bound (+24dBm)*/
	else if (rf_pw_cw < 15)
		pw_cw = (15 << 3) | (pw_s10_3 & 0x7); /*lower bound (-24dBm)*/

	/* ===[Set TSSI Offset]===*/
	/*
	172 = 300 -  (55 - 39) * 8;
	tssi_ofst_cw = tssi_16dBm_cw -  (tx_pow_16dBm_ref_cw - tx_pow_ref_cw) * 8;
	             = tssi_16dBm_cw + tx_pow_ref * 8  - tx_pow_16dBm_ref * 8
	*/
	tssi_ofst_cw = (u32)((s16)tssi_16dBm_cw + (pw_dbm_ofdm * 2) - (16 * 8));

	BB_DBG(bb, DBG_DBG_API, "[OFDM]tssi_ofst_cw=%d, rf_cw=0x%x, bb_cw=0x%x\n", tssi_ofst_cw, pw_cw >> 3, pw_cw & 0x7);

	*ofdm_cw = (u16)pw_cw;
	val_ofdm = tssi_ofst_cw << 18 | pw_cw << 9 | (u32)(pw_dbm_ofdm & 0x1ff);

	/*==== [CCK] =========================================================*/
	pw_s10_3 = (pw_dbm_cck * 2) + (s16)(ofst) + (s16)(base_cw_0db * 8);

	pw_cw = pw_s10_3;
	rf_pw_cw = (pw_s10_3 & 0x1F8) >> 3;

	if (rf_pw_cw > 63)
		pw_cw = (63 << 3) | (pw_s10_3 & 0x7); /*upper bound (+24dBm)*/
	else if (rf_pw_cw < 15)
		pw_cw = (15 << 3) | (pw_s10_3 & 0x7); /*lower bound (-24dBm)*/

	/* ===[Set TSSI Offset]===*/
	/*
	172 = 300 -  (55 - 39) * 8;
	tssi_ofst_cw = tssi_16dBm_cw -  (tx_pow_16dBm_ref_cw - tx_pow_ref_cw) * 8;
	             = tssi_16dBm_cw + tx_pow_ref * 8  - tx_pow_16dBm_ref * 8
	*/
	tssi_ofst_cw = (u32)((s16)tssi_16dBm_cw + (pw_dbm_cck * 2) - (16 * 8));

	BB_DBG(bb, DBG_DBG_API, "[CCK] tssi_ofst_cw=%d, rf_cw=0x%x, bb_cw=0x%x\n", tssi_ofst_cw, pw_cw >> 3, pw_cw & 0x7);

	*cck_cw = (u16)pw_cw;
	val_cck = tssi_ofst_cw << 18 | pw_cw << 9 | (u32)(pw_dbm_cck & 0x1ff);

	/*==== [SET BB CR] ===================================================*/
#ifdef HALBB_DBCC_SUPPORT
	if (bb->hal_com->dbcc_en) {
		if (phy_idx == HW_PHY_0)
			cfg_path_b = false; /*path-A only for phy-0, no need to set path-B*/
		else if (phy_idx == HW_PHY_1)
			cfg_path_a = false; /*path-B only for phy-1, no need to set path-A*/
	}
#endif
	/*path-A*/
	if (cfg_path_a) {
		halbb_set_reg(bb, 0x5804, 0x7FFFFFF, val_ofdm);
		halbb_set_reg(bb, 0x5808, 0x7FFFFFF, val_cck);
	}
	/*path-B*/
	if (cfg_path_b) {
		halbb_set_reg(bb, 0x7804, 0x7FFFFFF, val_ofdm);	
		halbb_set_reg(bb, 0x7808, 0x7FFFFFF, val_cck);
	}
}

void halbb_dump_bb_reg_8852a_2(struct bb_info *bb, u32 *_used, char *output,
			       u32 *_out_len)
{
	u32 i = 0, addr = 0;
	u32 cr_start = 0, cr_end = 0;
	u32 dump_cr_table[][2] = {{0x0000, 0x04FC},
				  {0x0600, 0x0DFC},
				  {0x1000, 0x10FC},
				  {0x1200, 0x13FC},
				  {0x1700, 0x20FC},
				  {0x2200, 0x24FC},
				  {0x2600, 0x2DFC},
				  {0x3000, 0x30FC},
				  {0x3200, 0x33FC},
				  {0x3900, 0x4FFC},
				  {0x5800, 0x6FFC},
				  {0x7800, 0x7FFC}};
	u32 table_len = sizeof(dump_cr_table)/(sizeof(u32) * 2);

	BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "BBCR: dump all ==>\n");

	if (bb->hal_com->dbcc_en) {
		BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "BBCR: [WARNING] Will simultaneously dump phy-0/1 reg. ==>\n");
		BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "BBCR: PHY-0 ==>\n");
	}
	BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "table_len=%d\n", table_len);

	for (i = 0; i < table_len; i ++) {
		BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "[%02d] Reg[0x%04x ~ 0x%04x]\n\n",
			    i, dump_cr_table[i][0], dump_cr_table[i][1]);
	}

	for (i = 0; i < table_len; i ++) {
		cr_start = dump_cr_table[i][0];
		cr_end = dump_cr_table[i][1];
		for (addr = cr_start; addr <= cr_end; addr += 4)
			BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
				    "0x%04x 0x%08x\n",
				    addr, halbb_get_reg(bb, addr, MASKDWORD));
	}

	if (bb->hal_com->dbcc_en) {
		BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "BBCR: PHY-1 ==>\n");

		for (i = 0; i < table_len; i ++) {
			cr_start = dump_cr_table[i][0];
			cr_end = dump_cr_table[i][1];
			for (addr = cr_start; addr <= cr_end; addr += 4)
				BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
					    "0x%04x 0x%08x\n",
					    addr, halbb_get_reg_cmn(bb, addr, MASKDWORD, HW_PHY_1));
		}
	}
}

void halbb_dbgport_dump_all_8852a_2(struct bb_info *bb, u32 *_used, char *output,
				    u32 *_out_len)
{
	const u32 dump_dbgport_table[][3] = {
					{DBGPORT_IP_TD, 0x001, 0x026},
					{DBGPORT_IP_TD, 0xb01, 0xb27},
					{DBGPORT_IP_RX_INNER, 0x0, 0x29},
					{DBGPORT_IP_TX_INNER, 0x0, 0x8},
					{DBGPORT_IP_OUTER, 0x0, 0xaa},
					{DBGPORT_IP_OUTER, 0xc0, 0xc4},
					{DBGPORT_IP_INTF, 0x0, 0x40},
					{DBGPORT_IP_CCK, 0x0, 0x3e},
					{DBGPORT_IP_BF, 0x0, 0x59},
					{DBGPORT_IP_RX_OUTER, 0x00, 0x63},
					{DBGPORT_IP_RX_OUTER, 0x90, 0x98},
					{DBGPORT_IP_RX_OUTER, 0xc0, 0xc3},
					{DBGPORT_IP_RX_OUTER, 0xe0, 0xe3}};
	u32 table_len;
	u32 dp = 0; /*debug port value*/
	u8 i;
	u32 j;
	u32 dbg_start = 0, dbg_end = 0;

	table_len = sizeof(dump_dbgport_table) / (sizeof(u32) * 3);

	BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "BB DBG Port: dump all ==>\n");

	for (i = 0; i < table_len; i++) {
		BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "[%02d][IP=%02d] Dbgport[0x%03x ~ 0x%03x]\n",
			    i, dump_dbgport_table[i][0],
			    dump_dbgport_table[i][1], dump_dbgport_table[i][2]);
	}

	for (i = 0; i < table_len; i++) {
		halbb_set_bb_dbg_port_ip(bb, dump_dbgport_table[i][0]);
		dbg_start = dump_dbgport_table[i][1];
		dbg_end = dump_dbgport_table[i][2];
		for (j = dbg_start; j <= dbg_end; j ++) {
			halbb_set_bb_dbg_port(bb, j);
			dp = halbb_get_bb_dbg_port_val(bb);
			BB_DBG_VAST(*_out_len, *_used, output + *_used, *_out_len - *_used,
				    "[0x%02x, 0x%03x] = 0x%08x\n",
				    dump_dbgport_table[i][0], j, dp);
		}
	}
	halbb_release_bb_dbg_port(bb);
}

void halbb_physts_brk_fail_pkt_rpt_8852a_2(struct bb_info *bb, bool enable,
					   enum phl_phy_idx phy_idx)
{
	u32 val32 = (enable) ? 0 : 0x3;

	halbb_set_reg_cmn(bb, 0x0738, 0xC, val32, phy_idx);
}

bool halbb_rf_write_bb_reg_8852a_2(struct bb_info *bb, u32 addr, u32 mask, u32 data)
{
	u32 page = (addr & 0xff00) >> 8;

	if (page != 0x0c && page != 0x20 && page != 0x2c &&
	    page != 0x58 && page != 0x78 &&
	    addr != 0x0700 && addr != 0x12a0 && addr != 0x12b8 &&
	    addr != 0x2320 && addr != 0x2700 &&
	    addr != 0x32a0 && addr != 0x32b8) {
		return false;
	}

	halbb_set_reg(bb, addr, mask, data);
	return true;
}

void halbb_pre_agc_en_8852a_2(struct bb_info *bb, bool enable)
{
	u8 en = 0;

	en = (enable == true) ? 1 : 0;
	halbb_set_reg(bb, 0x4670, BIT(20), en);
	halbb_set_reg(bb, 0x4744, BIT(20), en);
	BB_DBG(bb, DBG_DBG_API, "PreAGC en: 0x4670[20]=(0x%x)\n", en);
}

s8 halbb_efuse_exchange_8852a_2(struct bb_info *bb, u8 value,
				enum efuse_bit_mask mask)
{
	s8 tmp = 0;

	if (mask == LOW_MASK) {
		tmp = value & 0xf;

		if (tmp & BIT(3))
			tmp = tmp | 0xf0;
	} else {
		tmp = (value & 0xf0) >> 4;

		if (tmp & BIT(3))
			tmp = tmp | 0xf0;
	}

	return tmp;
}

void halbb_get_normal_efuse_init_8852a_2(struct bb_info *bb)
{
	struct bb_efuse_info *gain = &bb->bb_efuse_i;
	u8 tmp;
	u8 tmp_ofst;
	u32 check_tmp = 0, i, j;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	tmp_ofst = (u8)halbb_get_reg(bb, 0x494C, 0xf8000000);
	bb->bb_efuse_i.efuse_ofst = (s8)halbb_cnvrt_2_sign(tmp_ofst, 5);
	tmp_ofst = (u8)halbb_get_reg(bb, 0x4964, 0xfe00000);
	bb->bb_efuse_i.efuse_ofst_tb = (s8)halbb_cnvrt_2_sign(tmp_ofst, 7);

	halbb_efuse_get_info(bb, EFUSE_INFO_RF_RX_GAIN_K_A_2G_CCK, &tmp, 1);
	gain->gain_offset[RF_PATH_A][0] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_offset[RF_PATH_B][0] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_efuse_get_info(bb, EFUSE_INFO_RF_RX_GAIN_K_A_2G_OFMD, &tmp, 1);
	gain->gain_offset[RF_PATH_A][1] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_offset[RF_PATH_B][1] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_efuse_get_info(bb, EFUSE_INFO_RF_RX_GAIN_K_A_5GL, &tmp, 1);
	gain->gain_offset[RF_PATH_A][2] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_offset[RF_PATH_B][2] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_efuse_get_info(bb, EFUSE_INFO_RF_RX_GAIN_K_A_5GM, &tmp, 1);
	gain->gain_offset[RF_PATH_A][3] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_offset[RF_PATH_B][3] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_efuse_get_info(bb, EFUSE_INFO_RF_RX_GAIN_K_A_5GH, &tmp, 1);
	gain->gain_offset[RF_PATH_A][4] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_offset[RF_PATH_B][4] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	for (i = 0; i < HALBB_MAX_PATH; i++) {
		for (j = 0; j < BB_EFUSE_BAND_NUM; j++) {
			BB_DBG(bb, DBG_INIT, "[Efuse]gain->gain_offset[%d][%d]=0x%x\n", i, j, gain->gain_offset[i][j]);
			if ((gain->gain_offset[i][j] & 0xf) == 0xf)
				check_tmp++;
		}
	}

	BB_DBG(bb, DBG_INIT, "[Efuse]check_tmp = %d\n", check_tmp);
	BB_DBG(bb, DBG_INIT, "[Efuse]HALBB_MAX_PATH * BB_EFUSE_BAND_NUM = %d\n", HALBB_MAX_PATH * BB_EFUSE_BAND_NUM);

	if (check_tmp == HALBB_MAX_PATH * BB_EFUSE_BAND_NUM)
		bb->bb_efuse_i.normal_efuse_check = false;
	else
		bb->bb_efuse_i.normal_efuse_check = true;
/*
	BB_DBG(bb, DBG_INIT,
	       "[Efuse][Gain 2G][CCK] Path-A: %d, Path-B: %d\n",
	       gain->gain_offset[RF_PATH_A][0], gain->gain_offset[RF_PATH_B][0]);
	BB_DBG(bb, DBG_INIT,
	       "[Efuse][Gain 2G][OFDM] Path-A: %d, Path-B: %d\n",
	       gain->gain_offset[RF_PATH_A][1], gain->gain_offset[RF_PATH_B][1]);
	BB_DBG(bb, DBG_INIT,
	       "[Efuse][Gain 5GL] Path-A: %d, Path-B: %d\n",
	       gain->gain_offset[RF_PATH_A][2], gain->gain_offset[RF_PATH_B][2]);
	BB_DBG(bb, DBG_INIT,
	       "[Efuse][Gain 5GM] Path-A: %d, Path-B: %d\n",
	       gain->gain_offset[RF_PATH_A][3], gain->gain_offset[RF_PATH_B][3]);
	BB_DBG(bb, DBG_INIT,
	       "[Efuse][Gain 5GH] Path-A: %d, Path-B: %d\n",
	       gain->gain_offset[RF_PATH_A][4], gain->gain_offset[RF_PATH_B][4]);
*/
}


void halbb_get_hidden_efuse_init_8852a_2(struct bb_info *bb)
{
	struct bb_efuse_info *gain = &bb->bb_efuse_i;
	u8 tmp;
	u32 check_tmp = 0, i, j;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_A_2G_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_A][0] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_A][0] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_A_5GL_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_A][1] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_A][1] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_A_5GM_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_A][2] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_A][2] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_A_5GH_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_A][3] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_A][3] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_B_2G_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_B][0] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_B][0] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_B_5GL_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_B][1] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_B][1] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_B_5GM_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_B][2] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_B][2] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	halbb_phy_efuse_get_info(bb, GAIN_HIDE_EFUSE_B_5GH_8852A, 1, &tmp);
	gain->gain_cs[RF_PATH_B][3] = halbb_efuse_exchange_8852a_2(bb, tmp, HIGH_MASK);
	gain->gain_cg[RF_PATH_B][3] = halbb_efuse_exchange_8852a_2(bb, tmp, LOW_MASK);

	for (i = 0; i < HALBB_MAX_PATH; i++) {
		for (j = 0; j < BB_GAIN_BAND_NUM; j++) {
			BB_DBG(bb, DBG_INIT, "[Efuse]gain->gain_cs[%d][%d]=0x%x\n", i, j, gain->gain_cg[i][j]);
			if ((gain->gain_cg[i][j] & 0xf) == 0xf)
				check_tmp++;
		}
	}

	BB_DBG(bb, DBG_INIT, "[Efuse]check_tmp = %d\n", check_tmp);
	BB_DBG(bb, DBG_INIT, "[Efuse]HALBB_MAX_PATH * BB_GAIN_BAND_NUM = %d\n", HALBB_MAX_PATH * BB_GAIN_BAND_NUM);

	if (check_tmp == HALBB_MAX_PATH * BB_GAIN_BAND_NUM)
		bb->bb_efuse_i.hidden_efuse_check = false;
	else
		bb->bb_efuse_i.hidden_efuse_check = true;

/*
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 2G][Path-A] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_A][0], gain->gain_cg[RF_PATH_A][0]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GL][Path-A] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_A][1], gain->gain_cg[RF_PATH_A][1]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GM][Path-A] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_A][2], gain->gain_cg[RF_PATH_A][2]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GH][Path-A] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_A][3], gain->gain_cg[RF_PATH_A][3]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 2G][Path-B] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_B][0], gain->gain_cg[RF_PATH_B][0]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GL][Path-B] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_B][1], gain->gain_cg[RF_PATH_B][1]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GM][Path-B] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_B][2], gain->gain_cg[RF_PATH_B][2]);
	BB_DBG(bb, DBG_INIT,
	       "[Hidden Efuse][Gain 5GH][Path-B] CS: %d, CG: %d\n",
	       gain->gain_cs[RF_PATH_B][3], gain->gain_cg[RF_PATH_B][3]);
*/
}

void halbb_set_efuse_8852a_2(struct bb_info *bb, u8 central_ch, enum rf_path path, enum phl_phy_idx phy_idx)
{
	u8 band;
	//u8 path = 0;
	u8 upper_bound = 60; // S(7,4): 3.75
	u8 lower_bound = 64; // S(7,4): -4
	s32 hidden_efuse = 0, normal_efuse = 0, normal_efuse_cck = 0;
	s32 efuse_tmp = 0, efuse_ofst_tmp = 0;
	s32 tmp = 0;
	u32 efuse_5g[BB_PATH_MAX_8852A] = {0x4624, 0x46f8};
	u32 efuse_5g_mask = 0x07e00000;
	u32 efuse_2g[BB_PATH_MAX_8852A] = {0x4628, 0x46fc};
	u32 efuse_2g_mask = 0x0000003f;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	// 2G Band: (0)
	// 5G Band: (1):Low, (2): Mid, (3):High
	if (central_ch >= 0 && central_ch <= 14)
		band = 0;
	else if (central_ch >= 36 && central_ch <= 64)
		band = 1;
	else if (central_ch >= 100 && central_ch <= 144)
		band = 2;
	else if (central_ch >= 149 && central_ch <= 177)
		band = 3;
	else
		band = 0;

	// === [Set hidden efuse] === //
	if (bb->bb_efuse_i.hidden_efuse_check) {
		for (path = RF_PATH_A; path < BB_PATH_MAX_8852A; path++) {
			if (central_ch >= 0 && central_ch <= 14) {
				efuse_tmp = halbb_sign_conversion((s32)(bb->bb_efuse_i.gain_cg[path][band] << 2), 6);
				efuse_ofst_tmp = halbb_sign_conversion((s32)bb->bb_gain_i.efuse_ofst[band][path][0], 6);

				hidden_efuse = efuse_tmp + efuse_ofst_tmp;
				// Lower bound -8 for s(6,2)
				hidden_efuse = hidden_efuse < -32 ? -32 : hidden_efuse;
				// Upper bound +7.75 for s(6,2)
				hidden_efuse = hidden_efuse > 31 ? 31 : hidden_efuse;

				halbb_set_reg(bb, efuse_2g[path], efuse_2g_mask, (hidden_efuse & 0x3f));
			} else {
				hidden_efuse = (bb->bb_efuse_i.gain_cg[path][band] << 2);
				halbb_set_reg(bb, efuse_5g[path], efuse_5g_mask, (hidden_efuse & 0x3f));
			}
		}
		BB_DBG(bb, DBG_INIT, "[Efuse] Hidden efuse dynamic setting!!\n");
	} else {
		BB_DBG(bb, DBG_INIT, "[Efuse] Values of hidden efuse are all 0xff, bypass dynamic setting!!\n");
	}

	// === [Set normal efuse] === //
	if (bb->bb_efuse_i.normal_efuse_check) {
		if ((bb->rx_path == RF_PATH_A) || (bb->rx_path == RF_PATH_AB)) {
			normal_efuse = bb->bb_efuse_i.gain_offset[RF_PATH_A][band + 1];
			normal_efuse_cck = bb->bb_efuse_i.gain_offset[RF_PATH_A][0];
		} else if (bb->rx_path == RF_PATH_B) {
			normal_efuse = bb->bb_efuse_i.gain_offset[RF_PATH_B][band + 1];
			normal_efuse_cck = bb->bb_efuse_i.gain_offset[RF_PATH_B][0];
		}
		normal_efuse *= (-1);
		normal_efuse_cck *= (-1);

		// OFDM normal efuse
		if (normal_efuse > 3) {
			tmp = (normal_efuse << 4) + (bb->bb_efuse_i.efuse_ofst << 2) - upper_bound;
			// r_1_rpl_bias_comp
			halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, ((tmp >> 2) & 0x1f), phy_idx);
			// r_tb_rssi_bias_comp
			halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (tmp & 0x7f), phy_idx);
			// r_1_rpl_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, (upper_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x7f, (upper_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x3f80, (upper_bound & 0x7f), phy_idx);
			// r_tb_rssi_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4968, 0x3f80, (upper_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, (upper_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, (upper_bound & 0x7f), phy_idx);
		} else if (normal_efuse < -4) {
			tmp = (normal_efuse << 4) + (bb->bb_efuse_i.efuse_ofst << 2) + lower_bound;
			// r_1_rpl_bias_comp
			halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, ((tmp >> 2) & 0x1f), phy_idx);
			// r_tb_rssi_bias_comp
			halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (tmp & 0x7f), phy_idx);
			// r_1_rpl_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, (lower_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x7f, (lower_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x3f80, (lower_bound & 0x7f), phy_idx);
			// r_tb_rssi_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4968, 0x3f80, (lower_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, (lower_bound & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, (lower_bound & 0x7f), phy_idx);
		} else {
			// r_1_rpl_bias_comp
			halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, (bb->bb_efuse_i.efuse_ofst & 0x1f), phy_idx);
			// r_tb_rssi_bias_comp
			halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (bb->bb_efuse_i.efuse_ofst_tb & 0x7f), phy_idx);
			// r_1_rpl_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, ((normal_efuse << 4) & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x7f, ((normal_efuse << 4) & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4964, 0x3f80, ((normal_efuse << 4) & 0x7f), phy_idx);
			// r_tb_rssi_bias_comp by BW
			halbb_set_reg_cmn(bb, 0x4968, 0x3f80, ((normal_efuse << 4) & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, ((normal_efuse << 4) & 0x7f), phy_idx);
			halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, ((normal_efuse << 4) & 0x7f), phy_idx);
		}

		// CCK normal efuse
		if (band == 0) {
			tmp = normal_efuse_cck << 3;
			halbb_set_reg(bb, 0x23ac, 0x7f, (tmp & 0x7f));
		}

		BB_DBG(bb, DBG_INIT, "[Efuse] Normal efuse dynamic setting!!\n");
	} else {
		BB_DBG(bb, DBG_INIT, "[Efuse] Values of normal efuse are all 0xff, bypass dynamic setting!!\n");
	}
}

void halbb_set_gain_error_8852a_2(struct bb_info *bb, u8 central_ch)
{
	u8 band;
	u8 path = 0, lna_idx = 0, tia_idx = 0;
	s32 tmp = 0;
	u32 lna_err_a_5g[BB_PATH_MAX_8852A][7] = {{0x462c, 0x462c, 0x4630,
						   0x4634, 0x4634, 0x4638,
						   0x4638}, {0x4700, 0x4700,
						   0x4704, 0x4708, 0x4708,
						   0x470c, 0x470c}};
	u32 lna_err_a_5g_mask[7] = {0x00000fc0, 0x3f000000, 0x0003f000,
				    0x0000003f, 0x00fc0000, 0x00000fc0,
				    0x3f000000};
	u32 lna_err_a_2g[BB_PATH_MAX_8852A][7] = {{0x462c, 0x4630, 0x4630,
						   0x4634, 0x4634, 0x4638,
						   0x463c}, {0x4700, 0x4704,
						   0x4704, 0x4708, 0x4708,
						   0x470c, 0x4710}};
	u32 lna_err_a_2g_mask[7] = {0x0003f000, 0x0000003f, 0x00fc0000,
				    0x00000fc0, 0x3f000000, 0x0003f000,
				    0x0000003f};
	u32 tia_err_a_5g[BB_PATH_MAX_8852A][2] = {{0x4640, 0x4644}, {0x4714,
						   0x4718}};
	u32 tia_err_a_5g_mask[2] = {0x0003f000, 0x0000003f};
	u32 tia_err_a_2g[BB_PATH_MAX_8852A][2] = {{0x4640, 0x4644}, {0x4714,
						   0x4718}};
	u32 tia_err_a_2g_mask[2] = {0x00fc0000, 0x00000fc0};

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	// 2G Band: (0)
	// 5G Band: (1):Low, (2): Mid, (3):High
	if (central_ch >= 0 && central_ch <= 14)
		band = 0;
	else if (central_ch >= 36 && central_ch <= 64)
		band = 1;
	else if (central_ch >= 100 && central_ch <= 144)
		band = 2;
	else if (central_ch >= 149 && central_ch <= 177)
		band = 3;
	else
		band = 0;

	if (central_ch >= 0 && central_ch <= 14) {
		for (path = RF_PATH_A; path < BB_PATH_MAX_8852A; path++) {
			// Set 2G LNA Gain Err
			for (lna_idx = 0; lna_idx < 7; lna_idx++) {
				tmp = bb->bb_gain_i.lna_gain[band][path][lna_idx];

				halbb_set_reg(bb, lna_err_a_2g[path][lna_idx],
					      lna_err_a_2g_mask[lna_idx], (tmp & 0x3f));

/*				BB_DBG(bb, DBG_PHY_CONFIG, "[2g]==================================================\n");
				BB_DBG(bb, DBG_PHY_CONFIG, "[Gain err] LNA gain = 0x%x\n",
					bb->bb_gain_i.lna_gain[band][path][lna_idx]);
				BB_DBG(bb, DBG_PHY_CONFIG, "[Gain err] Reg 0x%x, Bitmap 0x%x, Value = 0x%x\n",
					lna_err_a_2g[path][lna_idx], lna_err_a_2g_mask[lna_idx], (tmp & 0x3f));
*/			}
			// Set 2G TIA Gain Err
			for (tia_idx = 0; tia_idx < 2; tia_idx++) {
				tmp = bb->bb_gain_i.tia_gain[band][path][tia_idx];
				halbb_set_reg(bb, tia_err_a_2g[path][tia_idx],
					      tia_err_a_2g_mask[tia_idx], (tmp & 0x3f));
			}
		}
	} else {
		for (path = RF_PATH_A; path < BB_PATH_MAX_8852A; path++) {
			// Set 5G LNA Gain Err
			for (lna_idx = 0; lna_idx < 7; lna_idx++) {
				tmp = bb->bb_gain_i.lna_gain[band][path][lna_idx];

				halbb_set_reg(bb, lna_err_a_5g[path][lna_idx],
					      lna_err_a_5g_mask[lna_idx], (tmp & 0x3f));
			}
			// Set 5G TIA Gain Err
			for (tia_idx = 0; tia_idx < 2; tia_idx++) {
				tmp = bb->bb_gain_i.tia_gain[band][path][tia_idx];
				halbb_set_reg(bb, tia_err_a_5g[path][tia_idx],
					      tia_err_a_5g_mask[tia_idx], (tmp & 0x3f));
			}
		}
	}
}

void halbb_normal_efuse_verify_8852a_2(struct bb_info *bb, s8 rx_gain_offset,
				       enum rf_path rx_path,
				       enum phl_phy_idx phy_idx)
{
	s32 normal_efuse = 0;
	s32 tmp = 0;
	u8 upper_bound = 60; // S(7,4): 3.75
	u8 lower_bound = 64; // S(7,4): -4

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] rx_gain_offset = %d\n",
		rx_gain_offset);

	rx_gain_offset *= (-1);

	// === [Set normal efuse] === //
	if (rx_gain_offset > 3) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] Gain ofst > 3\n");
		tmp = (rx_gain_offset << 4) + (bb->bb_efuse_i.efuse_ofst << 2) - upper_bound;
		// r_1_rpl_bias_comp
		halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, ((tmp >> 2) & 0x1f), phy_idx);
		// r_tb_rssi_bias_comp
		halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (tmp & 0x7f), phy_idx);
		// r_1_rpl_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, (upper_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x7f, (upper_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x3f80, (upper_bound & 0x7f), phy_idx);
		// r_tb_rssi_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4968, 0x3f80, (upper_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, (upper_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, (upper_bound & 0x7f), phy_idx);
	} else if (rx_gain_offset < -4) {
		BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] Gain ofst < -4\n");
		tmp = (rx_gain_offset << 4) + (bb->bb_efuse_i.efuse_ofst << 2) + lower_bound;
		// r_1_rpl_bias_comp
		halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, ((tmp >> 2) & 0x1f), phy_idx);
		// r_tb_rssi_bias_comp
		halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (tmp & 0x7f), phy_idx);
		// r_1_rpl_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, (lower_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x7f, (lower_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x3f80, (lower_bound & 0x7f), phy_idx);
		// r_tb_rssi_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4968, 0x3f80, (lower_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, (lower_bound & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, (lower_bound & 0x7f), phy_idx);
	} else {
		BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] -4 <= Gain ofst <= 3\n");
		// r_1_rpl_bias_comp
		halbb_set_reg_cmn(bb, 0x494c, 0xf8000000, (bb->bb_efuse_i.efuse_ofst & 0x1f), phy_idx);
		// r_tb_rssi_bias_comp
		halbb_set_reg_cmn(bb, 0x4964, 0xfe00000, (bb->bb_efuse_i.efuse_ofst_tb & 0x7f), phy_idx);
		// r_1_rpl_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4960, 0xfe00000, ((rx_gain_offset << 4) & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x7f, ((rx_gain_offset << 4) & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4964, 0x3f80, ((rx_gain_offset << 4) & 0x7f), phy_idx);
		// r_tb_rssi_bias_comp by BW
		halbb_set_reg_cmn(bb, 0x4968, 0x3f80, ((rx_gain_offset << 4) & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0x1fc000, ((rx_gain_offset << 4) & 0x7f), phy_idx);
		halbb_set_reg_cmn(bb, 0x4968, 0xfe00000, ((rx_gain_offset << 4) & 0x7f), phy_idx);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x494c[31:27] = 0x%x\n", halbb_get_reg(bb, 0x494c, 0xf8000000));
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x4964[27:21] = 0x%x\n", halbb_get_reg(bb, 0x4964, 0xfe00000));
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x4960[27:21] = 0x%x\n", halbb_get_reg(bb, 0x4960, 0xfe00000));
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x4964[6:0] = 0x%x\n", halbb_get_reg(bb, 0x4964, 0x7f));
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x4964[13:7] = 0x%x\n", halbb_get_reg(bb, 0x4964, 0x3f80));
}

void halbb_normal_efuse_verify_cck_8852a_2(struct bb_info *bb, s8 rx_gain_offset,
					   enum rf_path rx_path,
					   enum phl_phy_idx phy_idx)
{
	s32 tmp = 0;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	rx_gain_offset *= (-1);
	tmp = rx_gain_offset << 3;
	halbb_set_reg(bb, 0x23ac, 0x7f, (tmp & 0x7f));

	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] gain ofst = 0x%x\n", rx_gain_offset);
	BB_DBG(bb, DBG_PHY_CONFIG, "[Normal Efuse] 0x23ac[6:0] = 0x%x\n", halbb_get_reg(bb, 0x23ac, 0x7f));
}

#endif
