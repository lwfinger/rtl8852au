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

/*8852A DPK ver:0x1a 20200105*/

void _dpk_bkup_kip_8852a(
	struct rf_info *rf,
	u32 *reg,
	u32 reg_bkup[][DPK_KIP_REG_NUM_8852A],
	u8 path)
{
	u8 i;

	for (i = 0; i < DPK_KIP_REG_NUM_8852A; i++) {
		reg_bkup[path][i] = halrf_rreg(rf, reg[i] + (path << 8), MASKDWORD);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Backup 0x%x = %x\n", reg[i]+ (path << 8), reg_bkup[path][i]);
	}
}

void _dpk_bkup_bb_8852a(
	struct rf_info *rf,
	u32 *reg,
	u32 reg_bkup[DPK_BB_REG_NUM_8852A])
{
	u8 i;

	for (i = 0; i < DPK_BB_REG_NUM_8852A; i++) {
		reg_bkup[i] = halrf_rreg(rf, reg[i], MASKDWORD);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Backup 0x%x = %x\n", reg[i], reg_bkup[i]);
	}
}

void _dpk_bkup_rf_8852a(
	struct rf_info *rf,
	u32 *rf_reg,
	u32 rf_bkup[][DPK_RF_REG_NUM_8852A],
	u8 path)
{
	u8 i;

	for (i = 0; i < DPK_RF_REG_NUM_8852A; i++) {
		rf_bkup[path][i] = halrf_rrf(rf, path, rf_reg[i], MASKRF);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Backup RF S%d 0x%x = %x\n",
				path, rf_reg[i], rf_bkup[path][i]);
	}
}

void _dpk_reload_kip_8852a(
	struct rf_info *rf,
	u32 *reg,
	u32 reg_bkup[][DPK_KIP_REG_NUM_8852A],
	u8 path) 
{
	u8 i;

	for (i = 0; i < DPK_KIP_REG_NUM_8852A; i++) {
		halrf_wreg(rf, reg[i] + (path << 8), MASKDWORD, reg_bkup[path][i]);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Reload 0x%x = %x\n", reg[i] + (path << 8),
				   reg_bkup[path][i]);
	}
}

void _dpk_reload_bb_8852a(
	struct rf_info *rf,
	u32 *reg,
	u32 reg_bkup[DPK_BB_REG_NUM_8852A]) 
{
	u8 i;

	for (i = 0; i < DPK_BB_REG_NUM_8852A; i++) {
		halrf_wreg(rf, reg[i], MASKDWORD, reg_bkup[i]);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Reload 0x%x = %x\n", reg[i],
				   reg_bkup[i]);
	}
}

void _dpk_reload_rf_8852a(
	struct rf_info *rf,
	u32 *rf_reg,
	u32 rf_bkup[][DPK_RF_REG_NUM_8852A],
	u8 path)
{
	u8 i;

	for (i = 0; i < DPK_RF_REG_NUM_8852A; i++) {
		halrf_wrf(rf, path, rf_reg[i], MASKRF, rf_bkup[path][i]);
		if (DPK_REG_DBG)
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Reload RF S%d 0x%x = %x\n",
				path, rf_reg[i], rf_bkup[path][i]);
	}
}

u8 _dpk_one_shot_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	enum dpk_id id)
{
	u8 r_bff8 = 0x0, phy_map;
	u16 dpk_cmd = 0x0, count = 0;

	phy_map = (BIT(phy) << 4) | BIT(path);

	dpk_cmd = (u16)((id << 8) | (0x19 + (path << 4)));

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DPK, RFK_ONESHOT_START);

	halrf_wreg(rf, 0x8000, MASKDWORD, dpk_cmd);
	/*halrf_wreg(rf, 0x8000, MASKDWORD, dpk_cmd + 1);*/
#if 0
	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x3800, BIT(4), 0x0); /*DAC FIFO reset*/
		halrf_wreg(rf, 0x3800, BIT(4), 0x1);
	} else {
		halrf_wreg(rf, 0x3880, BIT(4), 0x0); /*DAC FIFO reset*/
		halrf_wreg(rf, 0x3880, BIT(4), 0x1);
	}
#endif		
	halrf_wreg(rf, 0x80b0, BIT(28), 0x1);

	r_bff8 = (u8)halrf_rreg(rf, 0xbff8, MASKBYTE0);
	while (r_bff8 != 0x55 && count < 2000) {
		halrf_delay_us(rf, 10);
		r_bff8 = (u8)halrf_rreg(rf, 0xbff8, MASKBYTE0);
		count++;
	}

	halrf_wreg(rf, 0x8010, MASKBYTE0, 0x0);

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DPK, RFK_ONESHOT_STOP);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] one-shot for %s = 0x%x (count=%d)\n",
	       id == 0x06 ? "LBK_RXIQK" : (id == 0x10 ? "SYNC" :
	       (id == 0x11 ? "MDPK_IDL" : (id == 0x12 ? "MDPK_MPA" :
	        (id == 0x13 ? "GAIN_LOSS" : "PWR_CAL")))),
	       dpk_cmd, count);

	if (count == 2000) {
		RF_DBG(rf, DBG_RF_DPK, "[DPK] one-shot over 20ms!!!!\n");
		return 1;
	} else
		return 0;
}

void _dpk_rx_dck_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	halrf_wrf(rf, path, 0x8f, BIT(11) | BIT(10), 0x3); /*EN_TIA_IDAC_LSB*/

	halrf_set_rx_dck_8852a(rf, phy, path, false);

}

void _dpk_information_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u8 kidx = dpk->cur_idx[path];

	dpk->bp[path][kidx].band = rf->hal_com->band[phy].cur_chandef.band;
	dpk->bp[path][kidx].ch = rf->hal_com->band[phy].cur_chandef.center_ch;
	dpk->bp[path][kidx].bw = rf->hal_com->band[phy].cur_chandef.bw;

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d[%d] (PHY%d): TSSI %s/ DBCC %s/ %s/ CH%d/ %s\n",
	       path, dpk->cur_idx[path], phy, rf->is_tssi_mode[path] ? "on" : "off",
	       rf->hal_com->dbcc_en ? "on" : "off",
	       dpk->bp[path][kidx].band == 0 ? "2G" : (dpk->bp[path][kidx].band == 1 ? "5G" : "6G"),
	       dpk->bp[path][kidx].ch,
	       dpk->bp[path][kidx].bw == 0 ? "20M" : (dpk->bp[path][kidx].bw == 1 ? "40M" : "80M"));
}

void _dpk_bb_afe_setting_a_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	if (phy == HW_PHY_0) {
		/*adda fifo rst*/
		halrf_wreg(rf, 0x0804, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x0810, MASKHWORD, 0x0000);

		/*tx iqk goes through bb*/
		halrf_wreg(rf, 0x7c64 + (path << 8), BIT(28) | BIT(27), 0x3);

		/*set anapar dbg mode*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x1);
		/*acd clk off*/
		halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);
		/*rst adc digital filter*/
		halrf_wreg(rf, 0x4ccc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4ccc, BIT(0), 0x1);
		halrf_wreg(rf, 0x4dcc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4dcc, BIT(0), 0x1);

		/*acd clk on*/
		halrf_wreg(rf, 0x0b2c, BIT(30), 0x0);
		/*adda pwr on*/
		halrf_wreg(rf, 0x0b0c, MASKBYTE3, 0x1f);
		halrf_delay_us(rf, 10);

		/*wait cca*/
		halrf_wreg(rf, 0x0b2c, MASKHWORD, 0x0001);
		/*adda pwr on*/
		halrf_wreg(rf, 0x0b0c, MASKBYTE3, 0x13);
		/*no reset afe*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(28), 0x1);
		/*adda fifo clk on*/
		halrf_wreg(rf, 0x7cc8 + (path << 8), BIT(24), 0x1);

		/*tx chain clk on*/
		halrf_wreg(rf, 0x7c64 + (path << 8), 0xc0000000, 0x3);
		halrf_wreg(rf, 0x0908, 0x01FFFFFF, 0x1ffffff);
		/*bbrx_td_cken*/
		halrf_wreg(rf, 0x0a1c, BIT(2), 0x1);
		/*td_upd_gen_force*/
		halrf_wreg(rf, 0x0800, BIT(27), 0x1);
		/*iqk_com_tx_rx_en*/
		halrf_wreg(rf, 0x0a70, 0x000003FF, 0x3ff);
		/*iqk_clk_on*/
		halrf_wreg(rf, 0x0a60, BIT(1) | BIT(0), 0x3);
		/*iqk_com_rst*/
		halrf_wreg(rf, 0x0a6c, BIT(0), 0x0);
		halrf_wreg(rf, 0x0a6c, BIT(0), 0x1);
		/*iqk_path_rst*/
		halrf_wreg(rf, 0x7cac + (path << 8), BIT(27), 0x0);
		halrf_wreg(rf, 0x7cac + (path << 8), BIT(27), 0x1);
		/*block ofdm cca*/
		halrf_wreg(rf, 0x0a3c, BIT(9), 0x1);
		/*block cck cca*/
		halrf_wreg(rf, 0x7844, 0x80000000, 0x1);
		/*set dac clk*/
		halrf_wreg(rf, 0x1388, 0x80000000, 0x1); /*0/1:480/960*/
		/*txck_force_val*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(14) | BIT(13) |BIT(12), 0x7); /*6/7:480/960*/
		/*txck_force_on*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(15), 0x1);
		/*rxck_force_val*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(18) | BIT(17) |BIT(16), 0x3); /*1/2/3:80/160/320*/
		/*rxck_force_on*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(19), 0x1);
		/*bb_adc_force_on*/
		halrf_wreg(rf, 0x0800, BIT(24), 0x1);
		/*bb_adc_force_val*/
		halrf_wreg(rf, 0x0800, BIT(26) | BIT(25), 0x2); /*0/1/2:80/160/320*/
		/*adc fifo release*/
		halrf_wreg(rf, 0x0804, MASKHWORD, 0x601f);
		halrf_wreg(rf, 0x0810, MASKHWORD, 0xe181);
	}else {
		/*adda fifo rst*/
		halrf_wreg(rf, 0x0884, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x0890, MASKHWORD, 0x0000);

		/*tx iqk goes through bb*/
		halrf_wreg(rf, 0x7c64 + (path << 8), BIT(28) | BIT(27), 0x3);

		/*set anapar dbg mode*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x1);
		/*acd clk off*/
		halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);
		/*rst adc digital filter*/
		halrf_wreg(rf, 0x4ccc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4ccc, BIT(0), 0x1);
		halrf_wreg(rf, 0x4dcc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4dcc, BIT(0), 0x1);

		/*acd clk on*/
		halrf_wreg(rf, 0x0b2c, BIT(30), 0x0);
		/*adda pwr on*/
		halrf_wreg(rf, 0x0b0c, MASKBYTE3, 0x1f);
		halrf_delay_us(rf, 20);

		/*wait cca*/
		halrf_wreg(rf, 0x0b2c, MASKHWORD, 0x0001);
		/*adda pwr on*/
		halrf_wreg(rf, 0x0b0c, MASKBYTE3, 0x13);
		/*no reset afe*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(28), 0x1);
		/*adda fifo clk on*/
		halrf_wreg(rf, 0x7cc8 + (path << 8), BIT(24), 0x1);

		/*tx chain clk on*/
		halrf_wreg(rf, 0x7c64 + (path << 8), 0xc0000000, 0x3);
		halrf_wreg(rf, 0x0908, 0x01FFFFFF, 0x1ffffff);
		/*bbrx_td_cken*/
		halrf_wreg(rf, 0x0a9c, BIT(2), 0x1);
		/*td_upd_gen_force*/
		halrf_wreg(rf, 0x0880, BIT(27), 0x1);
		/*iqk_com_tx_rx_en*/
		halrf_wreg(rf, 0x0a70, 0x000003FF, 0x3ff);
		/*iqk_clk_on*/
		halrf_wreg(rf, 0x0a60, BIT(1) | BIT(0), 0x3);
		/*iqk_com_rst*/
		halrf_wreg(rf, 0x0a6c, BIT(0), 0x0);
		halrf_wreg(rf, 0x0a6c, BIT(0), 0x1);
		/*iqk_path_rst*/
		halrf_wreg(rf, 0x7cac + (path << 8), BIT(27), 0x0);
		halrf_wreg(rf, 0x7cac + (path << 8), BIT(27), 0x1);
		/*block ofdm cca*/
		halrf_wreg(rf, 0x0abc, BIT(9), 0x1);
		/*block cck cca*/
		halrf_wreg(rf, 0x7844, 0x80000000, 0x1);
		/*set dac clk*/
		halrf_wreg(rf, 0x5388, 0x80000000, 0x1); /*0/1:480/960*/
		/*txck_force_val*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(14) | BIT(13) |BIT(12), 0x7); /*6/7:480/960*/
		/*txck_force_on*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(15), 0x1);
		/*rxck_force_val*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(18) | BIT(17) |BIT(16), 0x3); /*1/2/3:80/160/320*/
		/*rxck_force_on*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), BIT(19), 0x1);
		/*bb_adc_force_on*/
		halrf_wreg(rf, 0x0800, BIT(24), 0x1);
		/*bb_adc_force_val*/
		halrf_wreg(rf, 0x0800, BIT(26) | BIT(25), 0x2); /*0/1/2:80/160/320*/
		/*adc fifo release*/
		halrf_wreg(rf, 0x0884, MASKHWORD, 0x601f);
		halrf_wreg(rf, 0x0890, MASKHWORD, 0xe181);
	}
	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d Set BB/AFE (PHY%d)!!\n", path, phy);
}

void _dpk_bb_afe_setting_b_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kpath)
{
#if 1
	switch (kpath) {
	case RF_A: /*DBCC PHY0*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0101);
		halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
		//halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
		halrf_wreg(rf, 0x030c, MASKBYTE3, 0x13);
		halrf_wreg(rf, 0x032c, MASKHWORD, 0x0041);
		halrf_wreg(rf, 0x12b8, BIT(28), 0x1);
		halrf_wreg(rf, 0x58c8, BIT(24), 0x1);
		halrf_wreg(rf, 0x5864, BIT(31) | BIT(30), 0x3);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x1ffffff);
		halrf_wreg(rf, 0x0c1c, BIT(2), 0x1);
		halrf_wreg(rf, 0x0700, BIT(27), 0x1);
		halrf_wreg(rf, 0x0c70, 0x000003FF, 0x3ff);
		halrf_wreg(rf, 0x0c60, BIT(1) | BIT(0), 0x3);
		halrf_wreg(rf, 0x0c6c, BIT(0), 0x1);
		halrf_wreg(rf, 0x58ac, BIT(27), 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
		halrf_bb_ctrl_rx_cca(rf, false, phy);
#else
		halrf_wreg(rf, 0x0c3c, BIT(9), 0x1); /*block OFDM CCA*/
		if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x0) /*CCK @PHY0*/
			halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/

		halrf_wreg(rf, 0x0704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x0704, BIT(1), 0x1);
		halrf_delay_us(rf, 1);
#endif
		halrf_wreg(rf, 0x4490, BIT(31), 0x1);
		//halrf_wreg(rf, 0x12a0, BIT(14) | BIT(13) | BIT(12), 0x7);
		//halrf_wreg(rf, 0x12a0, BIT(15), 0x1);
		//halrf_wreg(rf, 0x12a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x12a0, BIT(19), 0x1);
		halrf_wreg(rf, 0x12a0, 0x000FF000, 0xbf); /*[19:12]*/
		//halrf_wreg(rf, 0x32a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x32a0, BIT(19), 0x1);
		//halrf_wreg(rf, 0x0700, BIT(24), 0x1);
		//halrf_wreg(rf, 0x0700, BIT(26) | BIT(25), 0x2);
		halrf_wreg(rf, 0x0700, BIT(26) | BIT(25) | BIT(24), 0x5);
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x1111);

		halrf_wreg(rf, 0x58f0, BIT(19), 0x0); /*gain_scale post dpd off*/
		break;
	case RF_B: /*DBCC PHY1*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0202);
		//halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
		halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
		halrf_wreg(rf, 0x030c, MASKBYTE3, 0x13);
		halrf_wreg(rf, 0x032c, MASKHWORD, 0x0041);
		halrf_wreg(rf, 0x32b8, BIT(28), 0x1);
		halrf_wreg(rf, 0x78c8, BIT(24), 0x1);
		halrf_wreg(rf, 0x7864, BIT(31) | BIT(30), 0x3);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x1ffffff);
		halrf_wreg(rf, 0x2c1c, BIT(2), 0x1);
		halrf_wreg(rf, 0x2700, BIT(27), 0x1);
		halrf_wreg(rf, 0x0c70, 0x000003FF, 0x3ff);
		halrf_wreg(rf, 0x0c60, BIT(1) | BIT(0), 0x3);
		halrf_wreg(rf, 0x0c6c, BIT(0), 0x1);
		halrf_wreg(rf, 0x78ac, BIT(27), 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
		halrf_bb_ctrl_rx_cca(rf, false, phy);
#else
		halrf_wreg(rf, 0x2c3c, BIT(9), 0x1); /*block OFDM CCA*/
		if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x1) /*CCK @PHY1*/
			halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/

		halrf_wreg(rf, 0x2704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x2704, BIT(1), 0x1);
		halrf_delay_us(rf, 1);
#endif
		halrf_wreg(rf, 0x6490, BIT(31), 0x1);
		//halrf_wreg(rf, 0x32a0, BIT(14) | BIT(13) | BIT(12), 0x7);
		//halrf_wreg(rf, 0x32a0, BIT(15), 0x1);
		//halrf_wreg(rf, 0x32a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x32a0, BIT(19), 0x1);
		halrf_wreg(rf, 0x32a0, 0x000FF000, 0xbf); /*[19:12]*/
		//halrf_wreg(rf, 0x32a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x32a0, BIT(19), 0x1);
		//halrf_wreg(rf, 0x2700, BIT(24), 0x1);
		//halrf_wreg(rf, 0x2700, BIT(26) | BIT(25), 0x2);
		halrf_wreg(rf, 0x2700, BIT(26) | BIT(25) | BIT(24), 0x5);
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x2222);

		halrf_wreg(rf, 0x78f0, BIT(19), 0x0); /*gain_scale post dpd off*/
		break;
	case RF_AB: /*nonDBCC*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0303);
		halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
		halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
		halrf_wreg(rf, 0x030c, MASKBYTE3, 0x13);
		halrf_wreg(rf, 0x032c, MASKHWORD, 0x0041);
		halrf_wreg(rf, 0x12b8, BIT(28), 0x1);
		halrf_wreg(rf, 0x58c8, BIT(24), 0x1);
		halrf_wreg(rf, 0x78c8, BIT(24), 0x1);
		halrf_wreg(rf, 0x5864, BIT(31) | BIT(30), 0x3);
		halrf_wreg(rf, 0x7864, BIT(31) | BIT(30), 0x3);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x1ffffff);
		halrf_wreg(rf, 0x0c1c, BIT(2), 0x1);
		halrf_wreg(rf, 0x0700, BIT(27), 0x1);
		halrf_wreg(rf, 0x0c70, 0x000003FF, 0x3ff);
		halrf_wreg(rf, 0x0c60, BIT(1) | BIT(0), 0x3);
		halrf_wreg(rf, 0x0c6c, BIT(0), 0x1);
		halrf_wreg(rf, 0x58ac, BIT(27), 0x1);
		halrf_wreg(rf, 0x78ac, BIT(27), 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
		halrf_bb_ctrl_rx_cca(rf, false, phy);
#else		
		halrf_wreg(rf, 0x0c3c, BIT(9), 0x1); /*block OFDM CCA*/
		halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/

		halrf_wreg(rf, 0x0704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x0704, BIT(1), 0x1);
		halrf_delay_us(rf, 1);
#endif		
		halrf_wreg(rf, 0x4490, BIT(31), 0x1);
		//halrf_wreg(rf, 0x12a0, BIT(14) | BIT(13) | BIT(12), 0x7);
		//halrf_wreg(rf, 0x12a0, BIT(15), 0x1);
		//halrf_wreg(rf, 0x12a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x12a0, BIT(19), 0x1);
		halrf_wreg(rf, 0x12a0, 0x000FF000, 0xbf); /*[19:12]*/
		//halrf_wreg(rf, 0x32a0, BIT(18) | BIT(17) | BIT(16), 0x3);
		//halrf_wreg(rf, 0x32a0, BIT(19), 0x1);
		halrf_wreg(rf, 0x32a0, BIT(19) | BIT(18) | BIT(17) | BIT(16), 0xb);
		//halrf_wreg(rf, 0x0700, BIT(24), 0x1);
		//halrf_wreg(rf, 0x0700, BIT(26) | BIT(25), 0x2);
		halrf_wreg(rf, 0x0700, BIT(26) | BIT(25) | BIT(24), 0x5);
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x3333);

		halrf_wreg(rf, 0x58f0, BIT(19), 0x0); /*gain_scale post dpd off*/
		halrf_wreg(rf, 0x78f0, BIT(19), 0x0); /*gain_scale post dpd off*/
		break;
	default :
		break;
	}
	RF_DBG(rf, DBG_RF_DPK, "[DPK] Set BB/AFE for PHY%d (kpath=%d)\n", phy, kpath);
#else
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0303);
	//halrf_wreg(rf, 0x5864 + (path << 13), BIT(28) | BIT(27), 0x3);
	halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x030c, MASKBYTE3, 0x13);
	halrf_wreg(rf, 0x032c, MASKHWORD, 0x0041);
	halrf_wreg(rf, 0x12b8, BIT(28), 0x1);
	halrf_wreg(rf, 0x58c8 + (path << 13), BIT(24), 0x1);
	halrf_wreg(rf, 0x5864 + (path << 13), BIT(31) | BIT(30), 0x3);
	halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x1ffffff);
	halrf_wreg(rf, 0x0c1c, BIT(2), 0x1);
	halrf_wreg(rf, 0x0700, BIT(27), 0x1);
	halrf_wreg(rf, 0x0c70, 0x000003FF, 0x3ff);
	halrf_wreg(rf, 0x0c60, BIT(1) | BIT(0), 0x3);
	halrf_wreg(rf, 0x0c6c, BIT(0), 0x1);
	halrf_wreg(rf, 0x58ac + (path << 13), BIT(27), 0x1);
	halrf_wreg(rf, 0x0c3c, BIT(9), 0x1); /*block OFDM CCA*/
	halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/
	halrf_wreg(rf, 0x4490, BIT(31), 0x1);
	halrf_wreg(rf, 0x12a0, BIT(14) | BIT(13) | BIT(12), 0x7);
	halrf_wreg(rf, 0x12a0, BIT(15), 0x1);
	halrf_wreg(rf, 0x12a0, BIT(18) | BIT(17) | BIT(16), 0x3);
	halrf_wreg(rf, 0x12a0, BIT(19), 0x1);
	halrf_wreg(rf, 0x32a0, BIT(18) | BIT(17) | BIT(16), 0x3);
	halrf_wreg(rf, 0x32a0, BIT(19), 0x1);
	halrf_wreg(rf, 0x0700, BIT(24), 0x1);
	halrf_wreg(rf, 0x0700, BIT(26) | BIT(25), 0x2);
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x3333);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d Set BB/AFE (PHY%d)!!\n", path, phy);
#endif
}

void _dpk_bb_afe_setting_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kpath)
{
	if (rf->hal_com->cv == CAV)
		_dpk_bb_afe_setting_a_8852a(rf, phy, path);
	else
		_dpk_bb_afe_setting_b_8852a(rf, phy, path, kpath);
}

void _dpk_bb_afe_restore_a_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	if (phy == HW_PHY_0) {
		/*adda fifo rst*/
		halrf_wreg(rf, 0x0804, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x0810, MASKHWORD, 0x0000);
		/*leave anapar dbg mode*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x0);
		/*tx-chain clk force off*/
		halrf_wreg(rf, 0x7c64 + (path << 8), 0xc0000000, 0x0);
		halrf_wreg(rf, 0x0908, 0x01FFFFFF, 0x00000000);
		/*bbrx_td_cken*/
		halrf_wreg(rf, 0x0a1c, BIT(2), 0x0);
		/*td_upd_gen_force*/
		halrf_wreg(rf, 0x0800, BIT(27), 0x0);
		/*iqk_com_tx_en*/
		halrf_wreg(rf, 0x0a70, 0x0000001F, 0x03); /*[4:0]*/
		/*iqk_com_rx_en*/
		halrf_wreg(rf, 0x0a70, 0x000003E0, 0x03); /*[9:5]*/
		/*set dac clk*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), 0x000FF000, 0x00); /*[19:12]*/
		/*set adc clk&upd_adc*/
		halrf_wreg(rf, 0x0800, BIT(26) | BIT(25) | BIT(24), 0x0);
		/*block ofdm cca*/
		halrf_wreg(rf, 0x0a3c, BIT(9), 0x0);
		/*block cck cca*/
		halrf_wreg(rf, 0x7844, 0x80000000, 0x0);
		/*adc fifo release*/
		halrf_wreg(rf, 0x0804, MASKHWORD, 0x601c);
		halrf_wreg(rf, 0x0810, MASKHWORD, 0xe181);
		/*adda_fifo_clk_force_on*/
		halrf_wreg(rf, 0x7cc8 + (path << 8), BIT(24), 0x0);
	}else {
		/*adda fifo rst*/
		halrf_wreg(rf, 0x0884, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x0890, MASKHWORD, 0x0000);
		/*leave anapar dbg mode*/
		halrf_wreg(rf, 0x4cb8 + (path << 8), BIT(30), 0x0);
		/*tx-chain clk force off*/
		halrf_wreg(rf, 0x7c64 + (path << 8), 0xc0000000, 0x0);
		halrf_wreg(rf, 0x0908, 0x01FFFFFF, 0x00000000);
		/*bbrx_td_cken*/
		halrf_wreg(rf, 0x0a9c, BIT(2), 0x0);
		/*td_upd_gen_force*/
		halrf_wreg(rf, 0x0880, BIT(27), 0x0);
		/*iqk_com_tx_en*/
		halrf_wreg(rf, 0x0a70, 0x0000001F, 0x03); /*[4:0]*/
		/*iqk_com_rx_en*/
		halrf_wreg(rf, 0x0a70, 0x000003E0, 0x03); /*[9:5]*/
		/*set dac clk*/
		halrf_wreg(rf, 0x4ca0 + (path << 8), 0x000FF000, 0x00); /*[19:12]*/
		/*set adc clk&upd_adc*/
		halrf_wreg(rf, 0x0880, BIT(26) | BIT(25) | BIT(24), 0x0);
		/*block ofdm cca*/
		halrf_wreg(rf, 0x0abc, BIT(9), 0x0);
		/*block cck cca*/
		halrf_wreg(rf, 0x7844, 0x80000000, 0x0);
		/*adc fifo release*/
		halrf_wreg(rf, 0x0884, MASKHWORD, 0x601c);
		halrf_wreg(rf, 0x0890, MASKHWORD, 0xe181);
		/*adda_fifo_clk_force_on*/
		halrf_wreg(rf, 0x7cc8 + (path << 8), BIT(24), 0x0);
	}

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d restore BB/AFE (PHY%d)!!\n", path, phy);
}

void _dpk_bb_afe_restore_b_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kpath)
{
#if 1
	switch (kpath) {
	case RF_A: /*DBCC PHY0*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0101);
		halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
		//halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
		halrf_wreg(rf, 0x5864, BIT(31) | BIT(30), 0x0);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x0);
		halrf_wreg(rf, 0x0c1c, BIT(2), 0x0);
		halrf_wreg(rf, 0x0700, BIT(27), 0x0);
		halrf_wreg(rf, 0x0c70, 0x0000001F, 0x03);
		halrf_wreg(rf, 0x0c70, 0x000003E0, 0x03);
		halrf_wreg(rf, 0x12a0, 0x000FF000, 0x00);
		//halrf_wreg(rf, 0x32a0, 0x000FF000, 0x00);
		halrf_wreg(rf, 0x0700, BIT(26) | BIT(25) | BIT(24), 0x0);
		halrf_wreg(rf, 0x5864, BIT(29), 0x0);
		halrf_wreg(rf, 0x0c3c, BIT(9), 0x0); /*block OFDM CCA*/
		//halrf_wreg(rf, 0x2344, BIT(31), 0x0); /*block CCK CCA*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x58c8, BIT(24), 0x0);
		break;
	case RF_B: /*DBCC PHY1*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0202);
		//halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
		halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
		halrf_wreg(rf, 0x7864, BIT(31) | BIT(30), 0x0);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x0);
		halrf_wreg(rf, 0x2c1c, BIT(2), 0x0);
		halrf_wreg(rf, 0x2700, BIT(27), 0x0);
		halrf_wreg(rf, 0x0c70, 0x0000001F, 0x03);
		halrf_wreg(rf, 0x0c70, 0x000003E0, 0x03);
		//halrf_wreg(rf, 0x12a0, 0x000FF000, 0x00);
		halrf_wreg(rf, 0x32a0, 0x000FF000, 0x00);
		halrf_wreg(rf, 0x2700, BIT(26) | BIT(25) | BIT(24), 0x0);
		halrf_wreg(rf, 0x7864, BIT(29), 0x0);
		halrf_wreg(rf, 0x2c3c, BIT(9), 0x0); /*block OFDM CCA*/
		//halrf_wreg(rf, 0x2344, BIT(31), 0x0); /*block CCK CCA*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x78c8, BIT(24), 0x0);
		break;
	case RF_AB:
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0303);
		halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
		halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
		halrf_wreg(rf, 0x5864, BIT(31) | BIT(30), 0x0);
		halrf_wreg(rf, 0x7864, BIT(31) | BIT(30), 0x0);
		halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x0);
		halrf_wreg(rf, 0x0c1c, BIT(2), 0x0);
		halrf_wreg(rf, 0x0700, BIT(27), 0x0);
		halrf_wreg(rf, 0x0c70, 0x0000001F, 0x03);
		halrf_wreg(rf, 0x0c70, 0x000003E0, 0x03);
		halrf_wreg(rf, 0x12a0, 0x000FF000, 0x00);
		halrf_wreg(rf, 0x32a0, 0x000FF000, 0x00);
		halrf_wreg(rf, 0x0700, BIT(26) | BIT(25) | BIT(24), 0x0);
		halrf_wreg(rf, 0x5864, BIT(29), 0x0);
		halrf_wreg(rf, 0x7864, BIT(29), 0x0);
		halrf_wreg(rf, 0x0c3c, BIT(9), 0x0); /*block OFDM CCA*/
		//halrf_wreg(rf, 0x2344, BIT(31), 0x0); /*block CCK CCA*/
		halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0000);
		halrf_wreg(rf, 0x58c8, BIT(24), 0x0);
		halrf_wreg(rf, 0x78c8, BIT(24), 0x0);
		break;
	default:
		break;
	}
	RF_DBG(rf, DBG_RF_DPK, "[DPK] Restore BB/AFE for PHY%d (kpath=%d)\n", phy, kpath);
#else
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0303);
	halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
	halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
	halrf_wreg(rf, 0x5864 + (path << 13), BIT(31) | BIT(30), 0x0);
	halrf_wreg(rf, 0x2008, 0x01FFFFFF, 0x0);
	halrf_wreg(rf, 0x0c1c, BIT(2), 0x0);
	halrf_wreg(rf, 0x0700, BIT(27), 0x0);
	halrf_wreg(rf, 0x0c70, 0x0000001F, 0x03);
	halrf_wreg(rf, 0x0c70, 0x000003E0, 0x03);
	halrf_wreg(rf, 0x12a0, 0x000FF000, 0x00);
	halrf_wreg(rf, 0x32a0, 0x000FF000, 0x00);
	halrf_wreg(rf, 0x0700, BIT(26) | BIT(25) | BIT(24), 0x0);
	halrf_wreg(rf, 0x5864 + (path << 13), BIT(29), 0x0);
	halrf_wreg(rf, 0x0c3c, BIT(9), 0x0); /*block OFDM CCA*/
	halrf_wreg(rf, 0x2344, BIT(31), 0x0); /*block CCK CCA*/
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0000);
	halrf_wreg(rf, 0x58c8 + (path << 13), BIT(24), 0x0);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d restore BB/AFE (PHY%d)!!\n", path, phy);
#endif
}

void _dpk_bb_afe_restore_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kpath)
{
	if (rf->hal_com->cv == CAV)
		_dpk_bb_afe_restore_a_8852a(rf, phy, path);
	else
		_dpk_bb_afe_restore_b_8852a(rf, phy, path, kpath);
}

void _dpk_tssi_pause_8852a(
	struct rf_info *rf,
	enum rf_path path,
	bool is_pause)
{
	if (rf->hal_com->cv == CAV)
		halrf_wreg(rf, 0x7c18 + (path << 8), BIT(30), is_pause);
	else
		halrf_wreg(rf, 0x5818 + (path << 13), BIT(30), is_pause);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d TSSI %s\n", path,
	       is_pause ? "pause" : "resume");
}

void _dpk_kip_setting_8852a(
	struct rf_info *rf,
	enum rf_path path,
	u8 kidx)
{
	/*cip power on*/
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000080);

	/*clk/en/misc*/
	halrf_wreg(rf, 0x808c, MASKDWORD, 0x00093f3f);
#if 1	
	/*320M*/
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x807f030a);

	halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0xce000a08);
	//halrf_wreg(rf, 0x8120, MASKDWORD, 0xce000a08);
	//halrf_wreg(rf, 0x8220, MASKDWORD, 0xce000a08);
#else
	/*160M*/
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x807f0305);

	if (path == RF_PATH_A)
		halrf_wreg(rf, 0x8120, MASKDWORD, 0xc1000504);
	else
		halrf_wreg(rf, 0x8220, MASKDWORD, 0xc1000504);
#endif
	/*GL (val*0.5 + 1)dB*/
	halrf_wreg(rf, 0x80b8, BIT(14) | BIT(13) | BIT(12), 0x2);

	/*txagc_bnd*/
	halrf_wreg(rf, 0x8000, BIT(2) | BIT(1), path); /*subpage_id*/
	halrf_wreg(rf, 0x81ac + (path << 8) + (kidx << 2), MASKDWORD, 0x003f2e2e);
	halrf_wreg(rf, 0x81bc + (path << 8) + (kidx << 2), MASKDWORD, 0x005b5b5b);

	//halrf_wreg(rf, 0x81ac + (path << 8), MASKDWORD, 0x003f2e2e); /*bnd_ch0*/
	//halrf_wreg(rf, 0x81b0 + (path << 8), MASKDWORD, 0x003f2e2e); /*bnd_ch1*/
	//halrf_wreg(rf, 0x81bc + (path << 8), MASKDWORD, 0x005b5b5b); /*gs_ch0*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), MASKDWORD, 0x005b5b5b); /*gs_ch1*/

	RF_DBG(rf, DBG_RF_DPK, "[DPK] KIP setting for S%d[%d]!!\n", path, kidx);
}

void _dpk_kip_restore_8852a(
	struct rf_info *rf,
	enum rf_path path)
{
	/*cip power on*/
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000000);
	/*CFIR CLK restore*/
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000000);

	halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0x10010000);

	if (rf->hal_com->cv > 0x1) /*hw txagc_offset*/
		halrf_wreg(rf, 0x81c8 + (path << 8), BIT(15), 0x1);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d restore KIP\n", path);
}

void _dpk_lbk_rxiqk_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	u8 i, cur_rxbb;

	cur_rxbb = (u8)halrf_rrf(rf, path, 0x00, MASKRFRXBB);
	
	/*DA off/AD on before RXIQK*/
	//halrf_wreg(rf, 0x12b8 + (path << 13), BIT(30), 0x1);
	//halrf_delay_us(rf, 1);

	halrf_wreg(rf, 0x030c, MASKBYTE3, 0x0f);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x030c, MASKBYTE3, 0x03);

	halrf_wreg(rf, 0x032c, MASKHWORD, 0xa001);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x032c, MASKHWORD, 0xa041);
#if 1
	halrf_wreg(rf, 0x8074, BIT(31), 0x1); /*RxSRAM_ctrl_sel 0:MDPK; 1:IQK*/

	/*RF setting*/
	halrf_wrf(rf, path, 0x00, MASKRFMODE, 0xc);
	halrf_wrf(rf, path, 0x20, BIT(5), 0x1);
	halrf_wrf(rf, path, 0x80, BIT(17) | BIT(16), 0x2);
	halrf_wrf(rf, path, 0x1f, MASKRF, halrf_rrf(rf, path, 0x18, MASKRF));
	halrf_wrf(rf, path, 0x1e, 0x0003F, 0x13); /*4.25MHz offset for IQKPLL*/
	halrf_wrf(rf, path, 0x1e, BIT(19), 0x0); /*POW IQKPLL*/
	halrf_wrf(rf, path, 0x1e, BIT(19), 0x1);

	for (i = 0; i < 7; i++)
		halrf_delay_us(rf, 10); /*IQKPLL's settling time*/

	halrf_wrf(rf, path, 0x8d, 0x01F00, 0x1f); /*[12:8]*/

	if (cur_rxbb <= 0xa)
		halrf_wrf(rf, path, 0x8d, 0x06000, 0x3); /*[14:13]*/
	else if (cur_rxbb <= 0x10 && cur_rxbb >= 0xb)
		halrf_wrf(rf, path, 0x8d, 0x06000, 0x1); /*[14:13]*/
	else 
		halrf_wrf(rf, path, 0x8d, 0x06000, 0x0); /*[14:13]*/

	halrf_wreg(rf, 0x802c, 0x0FFF0000, 0x11); /*[27:16] Rx_tone_idx=0x011 (4.25MHz)*/

	_dpk_one_shot_8852a(rf, phy, path, LBK_RXIQK);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d LBK RXIQC = 0x%x\n", path, halrf_rreg(rf, 0x813c, MASKDWORD));

	/*disable RXIQK PLL*/
	halrf_wrf(rf, path, 0x20, BIT(5), 0x0);
	halrf_wrf(rf, path, 0x80, BIT(17) | BIT(16), 0x0);
	halrf_wrf(rf, path, 0x1e, BIT(19), 0x0); /*POW IQKPLL*/
	halrf_wrf(rf, path, 0x00, MASKRFMODE, 0x5);

	halrf_wreg(rf, 0x8074, BIT(31), 0x0); /*RxSRAM_ctrl_sel 0:MDPK; 1:IQK*/
#endif
	/*DA on/AD on*/
	halrf_wreg(rf, 0x030c, MASKBYTE3, 0x1f);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x030c, MASKBYTE3, 0x13);

	halrf_wreg(rf, 0x032c, MASKHWORD, 0x0001);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x032c, MASKHWORD, 0x0041);

	/*ADDA FIFO rst*/
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x0303);
	halrf_wreg(rf, 0x20fc, MASKHWORD, 0x3333);
}

void _dpk_set_pmac_tx_info(struct rf_info *rf, enum phl_phy_idx phy_idx,
			enum rf_path path, u8 enable)
{
	struct rf_pmac_tx_info tx_info = {0};

	tx_info.ppdu = T_LEGACY;
	tx_info.mcs = 7;
	tx_info.bw = 0;
	tx_info.nss = 1;
	tx_info.gi = 1;
	tx_info.txagc_cw = 0x1d0;
	tx_info.cnt = 1000;
	tx_info.time = 20;
	tx_info.period = 5;
	tx_info.length = 0;

	halrf_set_pmac_tx(rf, phy_idx, path, &tx_info, enable, true);
}

void _dpk_get_thermal_8852a(struct rf_info *rf, u8 kidx, enum rf_path path)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	dpk->bp[path][kidx].ther_dpk = halrf_get_thermal_8852a(rf, path);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] thermal@DPK = 0x%x\n", dpk->bp[path][kidx].ther_dpk);
}

u8 _dpk_set_tx_pwr_8852a(
	struct rf_info *rf,
	u8 gain,
	enum rf_path path)
{
	//struct halrf_dpk_info *dpk = &rf->dpk;

	u8 txagc_ori = 0x38;

	halrf_wrf(rf, path, 0x01, MASKRF, txagc_ori);

	return txagc_ori;
}

void _dpk_rf_setting_8852a(
	struct rf_info *rf,
	u8 gain,
	enum rf_path path,
	u8 kidx)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	/*halrf_rf_direct_cntrl_8852a(rf, path, false); switch control to D/W*/

	if (dpk->bp[path][kidx].band == BAND_ON_24G) {

		//halrf_rf_direct_cntrl_8852a(rf, path, false); /*switch control to direct write*/

		//halrf_wrf(rf, path, 0x00, MASKRFMODE, 0x5);
		//halrf_wrf(rf, path, 0x00, 0xFFFE0, 0x280b); /*[19:5]*/
		halrf_wrf(rf, path, 0x00, MASKRF, 0x50161 | BIT(rf->hal_com->dbcc_en));
#if 0
		/*tx_power*/
		if (path == RF_PATH_A)
			halrf_wrf(rf, path, 0x01, MASKRF, 0x31);
		else
			halrf_wrf(rf, path, 0x01, MASKRF, 0x30);
#endif	
		/*att-c: 0/1/3/7:-31/-37/-43/-49*/
		halrf_wrf(rf, path, 0x83, BIT(2) | BIT(1) | BIT(0), 0x0);
		/*att-r: 0/1/3/7/f:-27/-21/-13/-9/-3.5*/
		halrf_wrf(rf, path, 0x83, 0x000F0, 0x4);
		/*mixer gain*/
		halrf_wrf(rf, path, 0x9f, BIT(4) | BIT(3), 0x0); /*0/1:28/22*/
		/*LNA/ext_BW = 0*/
		//halrf_wrf(rf, path, 0x00, 0x07800, 0x0); /*[14:11]*/
		/*TIA*/
		//halrf_wrf(rf, path, 0x00, BIT(10), 0x0);
		/*rxbb gain*/
		//halrf_wrf(rf, path, 0x00, MASKRFRXBB, 0xb); /*-6~-24*/
	} else {
		//halrf_wrf(rf, path, 0x00, MASKRFMODE, 0x5);
		//halrf_wrf(rf, path, 0x00, 0xFFFE0, 0x282e); /*[19:5]*/
		halrf_wrf(rf, path, 0x00, MASKRF, 0x505c1 | BIT(rf->hal_com->dbcc_en));
		/*tx_power*/
		/*halrf_wrf(rf, path, 0x01, MASKTXPWR, 0x39);*/
		/*att 0~7:-27~-36*/
		halrf_wrf(rf, path, 0x63, BIT(4) | BIT(3) | BIT(2), 0x7);
		/*switch*/
		halrf_wrf(rf, path, 0x64, BIT(16) | BIT(15), 0x3);
		/*mixer gain 0/1/2/3:5/8/20/20*/
		halrf_wrf(rf, path, 0x8a, BIT(9) | BIT(8), 0x3);
		/*LNA/ext_BW = 0*/
		//halrf_wrf(rf, path, 0x00, 0x07800, 0x0); /*[14:11]*/
		/*TIA*/
		//halrf_wrf(rf, path, 0x00, BIT(10), 0x1);
		/*rxbb gain*/
		//halrf_wrf(rf, path, 0x00, MASKRFRXBB, 0xb);
	}
	/*debug rtxbw*/
	halrf_wrf(rf, path, 0xde, BIT(2), 0x1);
	/*txbb filter*/
	halrf_wrf(rf, path, 0x1a, BIT(14) | BIT(13) | BIT(12), dpk->bp[path][kidx].bw + 1);
	/*rxbb filter*/
	halrf_wrf(rf, path, 0x1a, BIT(11) | BIT(10), 0x0);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] RF 0x0/0x1/0x1a = 0x%x/ 0x%x/ 0x%x\n",
	       halrf_rrf(rf, path, 0x00, MASKRF),
	       halrf_rrf(rf, path, 0x01, MASKRF),
	       halrf_rrf(rf, path, 0x1a, MASKRF));
}

void _dpk_get_gapk_offset_8852a(
	struct rf_info *rf,
	enum rf_path path,
	bool is_bkup)
{
	u8 gap_offset = 0;

	if (is_bkup) {
		gap_offset = (u8)halrf_rrf(rf, path, 0x5e, 0x00F00); /*[11:8]*/
		halrf_wreg(rf, 0x58c0 + (path << 13), BIT(4), 0x1);
		halrf_wreg(rf, 0x58c0 + (path << 13), 0x0000000F, gap_offset); /*[3:0]*/

		RF_DBG(rf, DBG_RF_DPK, "[DPK] get gapk offset = 0x%x\n", gap_offset);
	} else {
		halrf_wreg(rf, 0x58c0 + (path << 13), BIT(4), 0x0);

		RF_DBG(rf, DBG_RF_DPK, "[DPK] release force gapk\n");
	}
}

void _dpk_manual_txcfir_8852a(
	struct rf_info *rf,
	enum rf_path path,
	bool is_manual)
{
	u8 tmp_pad, tmp_txbb;

	if (is_manual) {
		halrf_wreg(rf, 0x8140 + (path << 8), BIT(8), 0x1);
		/*set pad to pad_man*/
		tmp_pad = (u8)halrf_rrf(rf, path, 0x56, 0x003e0); /*[9:5]*/
		halrf_wreg(rf, 0x8144 + (path << 8), 0x0001f, tmp_pad); /*[4:0]*/

		/*set txbb to txbb_man*/
		tmp_txbb = (u8)halrf_rrf(rf, path, 0x56, 0x0001f); /*[4:0]*/
		halrf_wreg(rf, 0x8144 + (path << 8), 0x01f00, tmp_txbb); /*[12:8]*/

		/*cfir load shot*/
		halrf_wreg(rf, 0x81dc + (path << 8), BIT(1) | BIT(0), 0x1);
		halrf_wreg(rf, 0x81dc + (path << 8), BIT(1) | BIT(0), 0x0);

		halrf_wreg(rf, 0x81dc + (path << 8), BIT(1), 0x1); /*auto*/

		RF_DBG(rf, DBG_RF_DPK, "[DPK] PAD_man / TXBB_man = 0x%x / 0x%x\n",
		       tmp_pad, tmp_txbb);
	} else {
		halrf_wreg(rf, 0x8140 + (path << 8), BIT(8), 0x0);
		RF_DBG(rf, DBG_RF_DPK, "[DPK] disable manual switch TXCFIR\n");
	}
}

void _dpk_bypass_rxcfir_8852a(
	struct rf_info *rf,
	enum rf_path path,
	bool is_bypass)
{
	if (is_bypass) {
		halrf_wreg(rf, 0x813c + (path << 8), BIT(2), 0x1);
		halrf_wreg(rf, 0x813c + (path << 8), BIT(0), 0x1);
		//halrf_wreg(rf, 0x813c + (path << 8), MASKDWORD, 0x00004002);
		RF_DBG(rf, DBG_RF_DPK, "[DPK] Bypass RXIQC (0x8%d3c = 0x%x)\n",
		       1 + path, halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD));
	} else {
		halrf_wreg(rf, 0x813c + (path << 8), BIT(2), 0x0);
		halrf_wreg(rf, 0x813c + (path << 8), BIT(0), 0x0);
		RF_DBG(rf, DBG_RF_DPK, "[DPK] restore 0x8%d3c = 0x%x\n",
		       1 + path, halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD));
	}
}

void _dpk_tpg_sel_8852a(
	struct rf_info *rf,
	enum rf_path path,
	u8 kidx)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	if (dpk->bp[path][kidx].bw == CHANNEL_WIDTH_80)
		halrf_wreg(rf, 0x806c, BIT(2) | BIT (1), 0x0);
	else if (dpk->bp[path][kidx].bw == CHANNEL_WIDTH_40)
		halrf_wreg(rf, 0x806c, BIT(2) | BIT (1), 0x2);
	else
		halrf_wreg(rf, 0x806c, BIT(2) | BIT (1), 0x1);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] TPG_Select for %s\n",
	       dpk->bp[path][kidx].bw == CHANNEL_WIDTH_80 ? "80M" : 
	       (dpk->bp[path][kidx].bw == CHANNEL_WIDTH_40 ? "40M" : "20M"));
}

void _dpk_table_select_8852a(
	struct rf_info *rf,
	enum rf_path path,
	u8 kidx,
	u8 gain)
{
	u8 val;

	val =  0x80 + kidx * 0x20 + gain * 0x10;
#if 1
	halrf_wreg(rf, 0x81ac + (path << 8), MASKBYTE3, val);
#else
	if (path == RF_PATH_A) {
		//halrf_wreg(rf, 0x81ac, MASKBYTE3, 0x80); /*ch0/gain0*/
		halrf_wreg(rf, 0x81ac, MASKBYTE3, 0x90); /*ch0/gain1*/
		//halrf_wreg(rf, 0x81ac, MASKBYTE3, 0xa0); /*ch1/gain0*/
		//halrf_wreg(rf, 0x81ac, MASKBYTE3, 0xb0); /*ch1/gain1*/
	} else {
		//halrf_wreg(rf, 0x82ac, MASKBYTE3, 0x80); /*ch0/gain0*/
		halrf_wreg(rf, 0x82ac, MASKBYTE3, 0x90); /*ch0/gain1*/
		//halrf_wreg(rf, 0x82ac, MASKBYTE3, 0xa0); /*ch1/gain0*/
		//halrf_wreg(rf, 0x82ac, MASKBYTE3, 0xb0); /*ch1/gain1*/
	}
#endif
	RF_DBG(rf, DBG_RF_DPK, "[DPK] table select for Kidx[%d], Gain[%d] (0x%x)\n",
		kidx, gain, val);
}

bool _dpk_sync_check_8852a(
	struct rf_info *rf,
	enum rf_path path,
	u8 kidx)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u16 dc_i, dc_q;
	u8 corr_val, corr_idx;

	halrf_wreg(rf, 0x80d4, 0x003F0000, 0x0);	/*rpt_sel*/

	corr_idx = (u8)halrf_rreg(rf, 0x80fc, 0x000000ff);
	corr_val = (u8)halrf_rreg(rf, 0x80fc, 0x0000ff00);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d Corr_idx / Corr_val = %d / %d\n",
	       path, corr_idx, corr_val);

	dpk->corr_idx[path][kidx] = corr_idx;
	dpk->corr_val[path][kidx] = corr_val;

	halrf_wreg(rf, 0x80d4, 0x003F0000, 0x9);	/*rpt_sel*/

	dc_i = (u16)halrf_rreg(rf, 0x80fc, 0x0fff0000); /*[27:16]*/
	dc_q = (u16)halrf_rreg(rf, 0x80fc, 0x00000fff); /*[11:0]*/

	if (dc_i >> 11 == 1)
		dc_i = 0x1000 - dc_i;
	if (dc_q >> 11 == 1)
		dc_q = 0x1000 - dc_q;

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d DC I/Q, = %d / %d\n", path, dc_i, dc_q);

	dpk->dc_i[path][kidx] = dc_i;
	dpk->dc_q[path][kidx] = dc_q;

	if ((dc_i > 200) || (dc_q > 200) || (corr_val < 170))
		return true;
	else
		return false;
}

bool _dpk_sync_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx)
{
	_dpk_tpg_sel_8852a(rf, path, kidx);

	//halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001019);
	_dpk_one_shot_8852a(rf, phy, path, SYNC);

	return _dpk_sync_check_8852a(rf, path, kidx); /*1= fail*/
}

u16 _dpk_dgain_read_8852a(
	struct rf_info *rf)
{
	u16 dgain = 0x0;

	halrf_wreg(rf, 0x80d4, 0x003F0000, 0x0);	/*rpt_sel*/
	
	halrf_rreg(rf, 0x80fc, BIT(30));		/*sync error*/

	dgain = (u16)halrf_rreg(rf, 0x80fc, 0x0FFF0000);	/*[27:16]*/

	RF_DBG(rf, DBG_RF_DPK, "[DPK] DGain = 0x%x (%d)\n", dgain, dgain);

	return dgain;
}

s8 _dpk_dgain_mapping_8852a(
	struct rf_info *rf,
	u16 dgain)
{
	s8 offset;

	if (dgain >= 0x783)
		offset = 0x6;
	else if ((0x782 >= dgain) && (dgain >= 0x551))
		offset = 0x3;
	else if ((0x550 >= dgain) && (dgain >= 0x3c4))
		offset = 0x0;
	else if ((0x3c3 >= dgain) && (dgain >= 0x2aa))
		offset = 0xfd;	/*-3*/
	else if ((0x2a9 >= dgain) && (dgain >= 0x1e3))
		offset = 0xfa;	/*-6*/
	else if ((0x1e2 >= dgain) && (dgain >= 0x156))
		offset = 0xf7;	/*-9*/
	else if (0x155 >= dgain)
		offset = 0xf4;	/*-12*/
	else
		offset = 0x0;

	//RF_DBG(rf, DBG_RF_DPK, "[DPK] DGain offset = %d\n", offset);

	return offset;
}

u8 _dpk_pas_check_8852a(
	struct rf_info *rf)
{
	u8 fail = 0;

	halrf_wreg(rf, 0x80d4, MASKBYTE2, 0x06); /*0x80d6, ctrl_out_Kpack*/
	halrf_wreg(rf, 0x80bc, BIT(14), 0x0);	/*query status*/
	halrf_wreg(rf, 0x80c0, MASKBYTE2, 0x08);

	halrf_wreg(rf, 0x80c0, MASKBYTE3, 0x00); /*0x80C3*/
	if (halrf_rreg(rf, 0x80fc, MASKHWORD) == 0x0800) {
		fail = 1;
		RF_DBG(rf, DBG_RF_DPK, "[DPK] PAS check Fail!!\n");
	}
	
	return fail;
}

u8 _dpk_gainloss_read_8852a(
	struct rf_info *rf)
{
	u8 result;

	halrf_wreg(rf, 0x80d4, 0x003F0000, 0x6);	/*rpt_sel*/
	halrf_wreg(rf, 0x80bc, BIT(14), 0x1);		/*query status*/

	result = (u8)halrf_rreg(rf, 0x80fc, 0x000000F0); /*[7:4]*/

	//RF_DBG(rf, DBG_RF_DPK, "[DPK] tmp GL = %d\n", result);

	return result;
}

void _dpk_gainloss_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx)
{
	//_dpk_tpg_sel_8852a(rf, path);

	//_dpk_table_select_8852a(rf, path, kidx, 1);

	//halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001319);
	_dpk_one_shot_8852a(rf, phy, path, GAIN_LOSS);
}

u8 _dpk_set_offset_8852a(
	struct rf_info *rf,
	enum rf_path path,
	s8 gain_offset)
{
	u8 txagc;

	txagc = (u8)halrf_rrf(rf, path, 0x01, MASKRF);

	if (txagc - gain_offset < 0x2e)
		txagc = 0x2e;
	else if (txagc - gain_offset > 0x3f)
		txagc = 0x3f;
	else
		txagc = txagc - gain_offset;

	halrf_wrf(rf, path, 0x01, MASKRF, txagc);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] Adjust TxAGC (offset %d) = 0x%x\n", gain_offset, txagc);
#if 0
	struct halrf_dpk_info *dpk = &rf->dpk;

	s8 offset;

	/*TSSI_OFFSET_VAL*/
	offset = (s8)halrf_rrf(rf, path, 0x01, 0x0F800); /*[15:11] for integer*/

	if (offset >> 4 != 0)
		offset = 0xe0 | offset;

	offset = offset - gain_offset;

	halrf_wrf(rf, path, 0x01, 0x0F800, offset & 0x1f);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] set gain offset = %d\n", offset);

	dpk->bp[0].trk_idx_dpk[path] = (u8)halrf_rrf(rf, path, 0x5D, 0xFC000);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] cur_trk_idx = %d\n",
	       dpk->bp[0].trk_idx_dpk[path]);
#endif
	return txagc;
}

u8 _dpk_pas_read_8852a(
	struct rf_info *rf,
	u8 is_check)
{
	u8 i;
	u32 val1_i = 0, val1_q = 0, val2_i = 0, val2_q = 0;

	halrf_wreg(rf, 0x80d4, MASKBYTE2, 0x06); /*0x80d6, ctrl_out_Kpack*/
	halrf_wreg(rf, 0x80bc, BIT(14), 0x0);	/*query status*/
	halrf_wreg(rf, 0x80c0, MASKBYTE2, 0x08);

	if (is_check) {
		halrf_wreg(rf, 0x80c0, MASKBYTE3, 0x00);
		val1_i = halrf_rreg(rf, 0x80fc, MASKHWORD);
		if (val1_i >= 0x800)
			val1_i = 0x1000 - val1_i;
		val1_q = halrf_rreg(rf, 0x80fc, MASKLWORD);
		if (val1_q >= 0x800)
			val1_q = 0x1000 - val1_q;
		halrf_wreg(rf, 0x80c0, MASKBYTE3, 0x1f);
		val2_i = halrf_rreg(rf, 0x80fc, MASKHWORD);
		if (val2_i >= 0x800)
			val2_i = 0x1000 - val2_i;
		val2_q = halrf_rreg(rf, 0x80fc, MASKLWORD);
		if (val2_q >= 0x800)
			val2_q = 0x1000 - val2_q;

		RF_DBG(rf, DBG_RF_DPK, "[DPK] PAS_delta = 0x%x\n",
			(val1_i * val1_i + val1_q * val1_q) / 
			(val2_i * val2_i + val2_q * val2_q));
	} else {
		for (i = 0; i < 32; i++) {
			halrf_wreg(rf, 0x80c0, MASKBYTE3, i); /*0x80C3*/
			RF_DBG(rf, DBG_RF_DPK, "[DPK] PAS_Read[%02d]= 0x%08x\n", i,
				   halrf_rreg(rf, 0x80fc, MASKDWORD));
		}
	}

	if ((val1_i * val1_i + val1_q * val1_q) >= ((val2_i * val2_i + val2_q * val2_q) * 8 / 5))
		return 1;
	else
		return 0;
}

u8 _dpk_agc_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx,
	u8 init_txagc,
	u8 loss_only)
{
	u8 tmp_txagc, tmp_rxbb = 0, i = 0, tmp_gl_idx = 0;
	u8 goout = 0, agc_cnt = 0, limited_rxbb = 0;
	s8 offset = 0;
	u16 dgain = 0;

	tmp_txagc = init_txagc;
	
	do {
		switch (i) {
		case 0: /*SYNC and Dgain*/
			if (_dpk_sync_8852a(rf, phy, path, kidx) == true) {
				tmp_txagc = 0xff;
				goout = 1;
				break;
			}
	
			dgain = _dpk_dgain_read_8852a(rf);

			if (loss_only == 1 || limited_rxbb == 1)
				i = 2;
			else
				i = 1;
			break;

		case 1: /*Gain adjustment*/
			tmp_rxbb = (u8)halrf_rrf(rf, path, 0x00, MASKRFRXBB);
			offset = _dpk_dgain_mapping_8852a(rf, dgain);

			if (tmp_rxbb + offset > 0x1f) {
				tmp_rxbb = 0x1f;
				limited_rxbb = 1;
			} else if (tmp_rxbb + offset < 0) {
				tmp_rxbb = 0;
				limited_rxbb = 1;
			} else
				tmp_rxbb = tmp_rxbb + offset;

			halrf_wrf(rf, path, 0x00, MASKRFRXBB, tmp_rxbb);
			RF_DBG(rf, DBG_RF_DPK, "[DPK] Adjust RXBB (%d) = 0x%x\n", offset, tmp_rxbb);
#if 1
			if (offset != 0 || agc_cnt == 0) {
				if (rf->hal_com->band[phy].cur_chandef.bw < 2)
					_dpk_bypass_rxcfir_8852a(rf, path, true);
				else
					_dpk_lbk_rxiqk_8852a(rf, phy, path);
			}
#endif
			if ((dgain > 1922) || (dgain < 342))
				i = 0;
			else
				i = 2;

			agc_cnt++;
			break;

		case 2: /*GAIN_LOSS and idx*/
			_dpk_gainloss_8852a(rf, phy, path, kidx);
			tmp_gl_idx = _dpk_gainloss_read_8852a(rf);
			/*_dpk_pas_read_8852a(rf, false);*/

			if ((tmp_gl_idx == 0 && _dpk_pas_read_8852a(rf, true) == 1) || tmp_gl_idx >= 7)
				i = 3; /*GL > criterion*/
			else if (tmp_gl_idx == 0)
				i = 4; /*GL < criterion*/
			else 
				i = 5;
			break;

		case 3: /*GL > criterion*/
			if (tmp_txagc == 0x2e) {
				goout = 1;
				RF_DBG(rf, DBG_RF_DPK, "[DPK] Txagc@lower bound!!\n");
			} else {
				tmp_txagc = _dpk_set_offset_8852a(rf, path, 0x3); /*tx gain -3*/
#if 0
				if (0x1f - tmp_rxbb > 2)
					tmp_rxbb = tmp_rxbb + 2;
				else
					tmp_rxbb = 0x1f;

				halrf_wrf(rf, path, 0x00, MASKRFRXBB, tmp_rxbb);
				RF_DBG(rf, DBG_RF_DPK, "[DPK] Adjust RXBB = 0x%x\n", tmp_rxbb);
#endif
			}
			i = 2;
			agc_cnt++;
			break;

		case 4:	/*GL < criterion*/
			if (tmp_txagc == 0x3f) {
				goout = 1;
				RF_DBG(rf, DBG_RF_DPK, "[DPK] Txagc@upper bound!!\n");
			} else {
				tmp_txagc = _dpk_set_offset_8852a(rf, path, 0xfe); /*tx gain +2*/
#if 0
				if (tmp_rxbb - 2 > 0)
					tmp_rxbb = tmp_rxbb - 2;
				else
					tmp_rxbb = 0x0;

				halrf_wrf(rf, path, 0x00, MASKRFRXBB, tmp_rxbb);
				RF_DBG(rf, DBG_RF_DPK, "[DPK] Adjust RXBB = 0x%x\n", tmp_rxbb);
#endif
			}
			i = 2;
			agc_cnt++;
			break;

		case 5:	/*set tx gain for DPK*/
			tmp_txagc =_dpk_set_offset_8852a(rf, path, tmp_gl_idx);
#if 0
			if (tmp_rxbb + tmp_gl_idx >= 0x1f)
				tmp_rxbb = 0x1f;
			else
				tmp_rxbb = tmp_rxbb + tmp_gl_idx;

			halrf_wrf(rf, path, 0x00, MASKRFRXBB, tmp_rxbb);
#endif
			goout = 1;
			agc_cnt++;
			break;

		default:
			goout = 1;
			break;
		}	
	} while (!goout && (agc_cnt < 6));

	RF_DBG(rf, DBG_RF_DPK, "[DPK] Txagc / RXBB for DPK = 0x%x / 0x%x\n",
		tmp_txagc, tmp_rxbb);

	return tmp_txagc;
}

void _dpk_set_mdpd_para_8852a(
	struct rf_info *rf,
	u8 order)
{
	switch (order) {
	case 0: /*(5,3,1)*/
		halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), order);
		halrf_wreg(rf, 0x80a0, 0x00001F00, 0x3); /*[12:8] phase normalize tap*/
		halrf_wreg(rf, 0x8070, 0xF0000000, 0x1); /*[31:28] tx_delay_man*/
		break;

	case 1: /*(5,3,0)*/
		halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), order);
		halrf_wreg(rf, 0x80a0, 0x00001F00, 0x0); /*[12:8] phase normalize tap*/
		halrf_wreg(rf, 0x8070, 0xF0000000, 0x0); /*[31:28] tx_delay_man*/
		break;

	case 2: /*(5,0,0)*/
		halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), order);
		halrf_wreg(rf, 0x80a0, 0x00001F00, 0x0); /*[12:8] phase normalize tap*/
		halrf_wreg(rf, 0x8070, 0xF0000000, 0x0); /*[31:28] tx_delay_man*/
		break;
	default:
		RF_DBG(rf, DBG_RF_DPK, "[DPK] Wrong MDPD order!!(0x%x)\n", order);
		break;
	}

	RF_DBG(rf, DBG_RF_DPK, "[DPK] Set MDPD order to 0x%x for IDL\n", order);
}

void _dpk_idl_mpa_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx,
	u8 gain)
{
	//struct halrf_dpk_info *dpk = &rf->dpk;

#if 0
	/*MPA (only need if bw = 80M)*/
	if (dpk->bp[path][kidx].bw == CHANNEL_WIDTH_80) {

		halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), 0x0); /* fix (5,3,1) for MPA*/
		
		if (path == RF_PATH_A) {
			halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001218);
			halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001219);
		} else {
			halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001228);
			halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001229);
		}

		RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d MPA !!\n", path);
	}
#endif
	/*IDL*/
	//_dpk_tpg_sel_8852a(rf, path);
	_dpk_set_mdpd_para_8852a(rf, 0x0);
#if 0
	halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), 0x0); /*(5,3,1)*/
	//halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), 0x1); /*(5,3,0)*/
	//halrf_wreg(rf, 0x80a0, BIT(1) | BIT(0), 0x2); /*(5,0,0)*/

	RF_DBG(rf, DBG_RF_DPK, "[DPK] Set Mem order to 0x%x for IDL\n",
	       halrf_rreg(rf, 0x80a0, BIT(1) | BIT(0)));
#endif
	_dpk_table_select_8852a(rf, path, kidx, 1);

	/*halrf_wreg(rf, 0x8000, MASKDWORD, 0x00001119);*/
	_dpk_one_shot_8852a(rf, phy, path, MDPK_IDL);
}

u8 _dpk_order_convert_8852a(
	struct rf_info *rf)
{
	u8 val;

	val = 0x3 >> (u8)halrf_rreg(rf, 0x80a0, 0x00000003);

	/*0x80a0 [1:0] = 0x0 => 0x81bc[26:25] = 0x3   //(5,3,1)*/
	/*0x80a0 [1:0] = 0x1 => 0x81bc[26:25] = 0x1   //(5,3,0)*/
	/*0x80a0 [1:0] = 0x2 => 0x81bc[26:25] = 0x0   //(5,0,0)*/

	/*0x80a0->val : 0->3; 1->1; 2->0*/

	RF_DBG(rf, DBG_RF_DPK, "[DPK] convert MDPD order to 0x%x\n", val);

	return val;
}

u8 _dpk_pwsf_addr_cal_8852a(
	struct rf_info *rf,
	u8 t1,
	u8 t2)
{
	u8 addr;
	s8 offset;

	/*w/o TSSI : t2 = cur_thermal*/
	offset = t2 - t1;
	addr = 0x78 + (offset << 3); /*due to TPG -3dB, start from 0x78*/

	return addr;
}

void _dpk_gs_normalize_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx,
	u8 txagc,
	u8 gs_ori,
	u16 pwsf)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u8 gs;
	u16 sqrt_out;

	if (dpk->bp[path][kidx].bw == 2) /*80M*/
		halrf_wreg(rf, 0x819c + (path << 8), MASKDWORD, 0x000300c0);
	else
		halrf_wreg(rf, 0x819c + (path << 8), MASKDWORD, 0x00030200);

	halrf_wreg(rf, 0x81c8 + (path << 8), 0x0000003F, txagc); /*man_txagc_vall*/
	halrf_wreg(rf, 0x81c8 + (path << 8), BIT(6), 0x1); /*man_txagc_en*/
	halrf_wreg(rf, 0x81c8 + (path << 8), MASKBYTE2, 0x08); /*man_pwsf_en*/
	halrf_wreg(rf, 0x81c8 + (path << 8), MASKBYTE1, pwsf - 24); /*man_pwsf_val*/

	_dpk_one_shot_8852a(rf, phy, path, GAIN_CAL);

	halrf_wreg(rf, 0x81d4, 0x003F0000, 0x13);	/*rpt_sel = pow_diff */
	sqrt_out = (u16)halrf_rreg(rf, 0x81fc, 0x000001FF); /*[8:0]*/

	gs = (u8)(gs_ori * sqrt_out >> 8);

	/*ch0/gain0 [6:0]*/
	halrf_wreg(rf, 0x81bc + (path << 8), 0x0000007F, gs);
	/*ch0/gain1 [14:8]*/
	halrf_wreg(rf, 0x81bc + (path << 8), 0x00007F00, gs);
	/*ch1/gain0 [6:0]*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), 0x0000007F, gs);
	/*ch1/gain1 [14:8]*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), 0x00007F00, gs);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] gs_ori/ sqrt_out/ gs_new = 0x%x/ %d/ 0x%x\n",
		   gs_ori, sqrt_out, gs);

}

void _dpk_fill_result_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 kidx,
	u8 gain,
	u8 txagc)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u16 pwsf = 0x78;
	u8 gs = dpk->dpk_gs[phy];

	halrf_wreg(rf, 0x8104 + (path << 8), BIT(8), kidx);

	/*read agc*/
	//txagc = (u8)(halrf_rrf(rf, path, 0x01, MASKTXPWR) + gain_offset); /*U(6.0)*/

	/*cal pwsf*/
	//pwsf = _dpk_pwsf_addr_cal_8852a(rf, 0, 0);
#if 0
	/*read gs and normalize*/
	gs = (u8)halrf_rreg(rf, 0x81bc + path * PATH_OFST_8852A + ch * CH_OFST_8852A,
						0x7F << (gain * 8));
	gs = _dpk_gs_normalize_8852a(rf, gs, path);
#endif
	RF_DBG(rf, DBG_RF_DPK, "[DPK] Fill txagc/ pwsf/ gs = 0x%x/ 0x%x/ 0x%x\n",
		   txagc, pwsf, gs);

	/*========== txagc_rf ==========*/
	dpk->bp[path][kidx].txagc_dpk = txagc;
	halrf_wreg(rf, 0x81c4 + (path << 8), 0x3F << ((gain << 3) + (kidx << 4)), txagc);
	/*ch0/gain0 [5:0]*/
	//halrf_wreg(rf, 0x81c4 + (path << 8), 0x0000003F, txagc); /*txagc_rf*/
	//halrf_wreg(rf, 0x8190 + (path << 8), 0x000001F8, txagc); /*txagc_orig*/
	/*ch0/gain1 [13:8]*/
	//halrf_wreg(rf, 0x81c4 + (path << 8), 0x00003F00, txagc); /*txagc_rf*/
	//halrf_wreg(rf, 0x8190 + (path << 8), 0x01F80000, txagc); /*txagc_orig*/
	/*ch1/gain0 [21:16]*/
	//halrf_wreg(rf, 0x81c4 + (path << 8), 0x003F0000, txagc);
	/*ch1/gain1 [29:24]*/
	//halrf_wreg(rf, 0x81c4 + (path << 8), 0x3F000000, txagc);

	/*========== txagc_bb ==========*/
	/*ch0/gain0 [9:0]*/
	//halrf_wreg(rf, 0x81a4 + (path << 8), 0x000003FF, txagc_bb);
	/*ch0/gain1 [25:16]*/
	//halrf_wreg(rf, 0x81a4 + (path << 8), 0x03FF0000, bb_gain);
	/*ch1/gain0 [9:0]*/
	//halrf_wreg(rf, 0x81a8 + (path << 8), 0x000003FF, txagc_bb);
	/*ch1/gain1 [25:16]*/
	//halrf_wreg(rf, 0x81a8 + (path << 8), 0x03FF0000, txagc_bb);

	/*========== pwsf ==========*/
	dpk->bp[path][kidx].pwsf = pwsf;
	halrf_wreg(rf, 0x81b4 + (path << 8) + (kidx << 2), 0x1FF << (gain << 4), pwsf);
	/*ch0/gain0 [8:0]*/
	//halrf_wreg(rf, 0x81b4 + (path << 8), 0x000001FF, pwsf);
	/*ch0/gain1 [24:16]*/
	//halrf_wreg(rf, 0x81b4 + (path << 8), 0x01FF0000, pwsf);
	/*ch1/gain0 [8:0]*/
	//halrf_wreg(rf, 0x81b8 + (path << 8), 0x000001FF, pwsf);
	/*ch1/gain1 [24:16]*/
	//halrf_wreg(rf, 0x81b8 + (path << 8), 0x01FF0000, pwsf);

	/*========== road shot MDPD==========*/
	halrf_wreg(rf, 0x81dc + (path << 8), BIT(16), 0x1);
	halrf_wreg(rf, 0x81dc + (path << 8), BIT(16), 0x0);

	//halrf_wreg(rf, 0x81bc + (path << 8), MASKDWORD, 0x075b5b5b);

	/*========== gs & MDPD order ==========*/
	dpk->bp[path][kidx].gs = gs;
	if (dpk->dpk_gs[phy] == 0x7f)
		halrf_wreg(rf, 0x81bc + (path << 8) + (kidx << 2), MASKDWORD, 0x067f7f7f);
	else
		halrf_wreg(rf, 0x81bc + (path << 8) + (kidx << 2), MASKDWORD, 0x065b5b5b);
	/*order [26:25]*/
	/*ch0*/
	//halrf_wreg(rf, 0x81bc + (path << 8), 0x06000000, _dpk_order_convert_8852a(rf));
	/*ch1*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), 0x06000000, _dpk_order_convert_8852a(rf));
	//halrf_wreg(rf, 0x81bc + (path << 8), 0x007F0000, 0x5b); /*gs2 set to -3dB*/
	/*ch0/gain0 [6:0]*/
	//halrf_wreg(rf, 0x81bc + (path << 8), 0x0000007F, gs);
	/*ch0/gain1 [14:8]*/
	//halrf_wreg(rf, 0x81bc + (path << 8), 0x00007F00, gs);
	/*ch1/gain0 [6:0]*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), 0x0000007F, gs);
	/*ch1/gain1 [14:8]*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), 0x00007F00, gs);

	/*========== mdpd_en ==========*/
	/*ch0*/
	//halrf_wreg(rf, 0x81bc + (path << 8), BIT(24), 0x1);
	/*ch1*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), BIT(24), 0x1);

	/*========== release all setting for K==========*/
	//halrf_wreg(rf, 0x81c8, MASKDWORD, 0x0);
	halrf_wreg(rf, 0x81a0 + (path << 8), MASKDWORD, 0x0);			

	halrf_wreg(rf, 0x8070, 0x80000000, 0x0); /*BIT(31)*/

	//_dpk_gs_normalize_8852a(rf, path, txagc, gs, pwsf);
}

void _dpk_coef_read_8852a(
	struct rf_info *rf,
	enum rf_path path,
	u8 kidx,
	u8 gain)
{
	u32 reg, reg_start, reg_stop;

	halrf_wreg(rf, 0x81d8 + (path << 8), MASKDWORD, 0x00010000);

	reg_start = 0x9500 + kidx * 0xa0 + path * 0x200 + gain * 0x50;

	reg_stop = reg_start + 0x50;

	RF_DBG(rf, DBG_RF_DPK, "[DPK] ===== [Coef of S%d[%d], gain%d] =====\n",
		path, kidx, gain);

	for (reg = reg_start; reg < reg_stop ; reg += 4) {
		RF_DBG(rf, DBG_RF_DPK, "[DPK][coef_r] 0x%x = 0x%08x\n", reg,
			   halrf_rreg(rf, reg, MASKDWORD));
	}
	halrf_wreg(rf, 0x81d8 + (path << 8), MASKDWORD, 0x00000000);
}

bool _dpk_reload_check_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	bool is_reload = false;
	u8 idx, cur_band, cur_ch;

	cur_band = rf->hal_com->band[phy].cur_chandef.band;
	cur_ch = rf->hal_com->band[phy].cur_chandef.center_ch;

	for (idx = 0; idx < DPK_BKUP_NUM; idx++) {
		if ((cur_band == dpk->bp[path][idx].band) && (cur_ch == dpk->bp[path][idx].ch)) {
			halrf_wreg(rf, 0x8104 + (path << 8), BIT(8), idx);
			dpk->cur_idx[path] = idx;
			is_reload = true;
			RF_DBG(rf, DBG_RF_DPK, "[DPK] reload S%d[%d] success\n", path, idx);
		}
	}

	return is_reload;
}

bool _dpk_main_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	enum rf_path path,
	u8 gain)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u8 txagc = 0, kidx = dpk->cur_idx[path];
	bool is_fail = false;
	//s8 gain_offset = 0;

	RF_DBG(rf, DBG_RF_DPK, "[DPK] ========= S%d[%d] DPK Start =========\n", path, kidx);

	halrf_rf_direct_cntrl_8852a(rf, path, false); /*switch control to direct write*/
	txagc = _dpk_set_tx_pwr_8852a(rf, gain, path);
	_dpk_rf_setting_8852a(rf, gain, path, kidx);
	_dpk_rx_dck_8852a(rf, phy, path);

	_dpk_kip_setting_8852a(rf, path, kidx);
	_dpk_manual_txcfir_8852a(rf, path, true);
	_dpk_table_select_8852a(rf, path, kidx, 1);
#if 0
	if (rf->hal_com->band[phy].cur_chandef.bw < 2)
		_dpk_bypass_rxcfir_8852a(rf, path, true);
	else
		_dpk_lbk_rxiqk_8852a(rf, phy, path);

	is_fail = _dpk_sync_8852a(rf, phy, path);

	if (is_fail)
		goto _error;

	_dpk_dgain_read_8852a(rf);
	_dpk_gainloss_8852a(rf, phy, path);
	//tmp_gl_idx = _dpk_gainloss_read_8852a(rf);

	//_dpk_get_thermal_8852a(rf, ch, path);

	txagc = _dpk_set_offset_8852a(rf, path, _dpk_gainloss_read_8852a(rf));
#else
	txagc = _dpk_agc_8852a(rf, phy, path, kidx, txagc, false);

	if (txagc == 0xff) {
		is_fail = true;
		goto _error;
	}
#endif
	_dpk_get_thermal_8852a(rf, kidx, path);
	/*_dpk_pas_read_8852a(rf, false);*/

	_dpk_idl_mpa_8852a(rf, phy, path, kidx, gain);
	halrf_wrf(rf, path, 0x00, MASKRFMODE, RF_RX);
#if 0
	_dpk_coef_read_8852a(rf, path, kidx, gain);
#endif
	_dpk_fill_result_8852a(rf, phy, path, kidx, gain, txagc);
_error:
	//_dpk_bypass_rxcfir_8852a(rf, path, false);
	_dpk_manual_txcfir_8852a(rf, path, false);

	if (is_fail == false)
		dpk->bp[path][kidx].path_ok = 1;
	else
		dpk->bp[path][kidx].path_ok = 0;

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d[%d] DPK %s\n", path, kidx, is_fail ? "Check" : "Success");

	//RF_DBG(rf, DBG_RF_DPK, "[DPK] path_ok = 0x%x\n", dpk->bp[path][kidx].path_ok);

	return is_fail;

}

void _dpk_cal_select_8852a(
	struct rf_info *rf,
	bool force,
	enum phl_phy_idx phy,
	u8 kpath)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u32 kip_bkup[DPK_RF_PATH_MAX_8852A][DPK_KIP_REG_NUM_8852A] = {{0}};
	u32 bb_bkup[DPK_BB_REG_NUM_8852A] = {0};
	u32 rf_bkup[DPK_RF_PATH_MAX_8852A][DPK_RF_REG_NUM_8852A] = {{0}};

	u32 kip_reg[] = {0x813c, 0x8124};
	u32 bb_reg[] = {0x2344, 0x58f0, 0x78f0};
	u32 rf_reg[DPK_RF_REG_NUM_8852A] = {0x5, 0x8f, 0xde};

	u8 path;
	bool is_fail = true, reloaded[DPK_RF_PATH_MAX_8852A] = {false};

	if (rf->phl_com->drv_mode != RTW_DRV_MODE_MP && dpk->is_dpk_reload_en) {
		for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
			if (kpath & BIT(path)) {
				reloaded[path] = _dpk_reload_check_8852a(rf, phy, path);
				if ((reloaded[path] == false) && (dpk->bp[path][0].ch != 0))
					dpk->cur_idx[path] = !dpk->cur_idx[path];
				else
					halrf_dpk_onoff_8852a(rf, path, false);
			}
		}
	} else {
		for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++)
			dpk->cur_idx[path] = 0;
	}

	if ((kpath == RF_A && reloaded[RF_PATH_A] == true) ||
	    (kpath == RF_B && reloaded[RF_PATH_B] == true) ||
	    (kpath == RF_AB && reloaded[RF_PATH_A] == true && reloaded[RF_PATH_B] == true))
		return;

	_dpk_bkup_bb_8852a(rf, bb_reg, bb_bkup);

	for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
		if ((kpath & BIT(path)) && (reloaded[path] == false)) {
			if (rf->is_tssi_mode[path])
				_dpk_tssi_pause_8852a(rf, path, true);
			_dpk_bkup_kip_8852a(rf, kip_reg, kip_bkup, path);
			_dpk_bkup_rf_8852a(rf, rf_reg, rf_bkup, path);
			_dpk_information_8852a(rf, phy, path);
		}
	}

	_dpk_bb_afe_setting_8852a(rf, phy, path, kpath);

	for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
		if ((kpath & BIT(path)) && (reloaded[path] == false)) {
			is_fail = _dpk_main_8852a(rf, phy, path, 1);
			halrf_dpk_onoff_8852a(rf, path, is_fail);
		}
	}

	_dpk_bb_afe_restore_8852a(rf, phy, path, kpath);
	_dpk_reload_bb_8852a(rf, bb_reg, bb_bkup);

	for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
		if ((kpath & BIT(path)) && (reloaded[path] == false)) {
			_dpk_kip_restore_8852a(rf, path);
			_dpk_reload_kip_8852a(rf, kip_reg, kip_bkup, path);
			_dpk_reload_rf_8852a(rf, rf_reg, rf_bkup, path);
			if (rf->is_tssi_mode[path])
				_dpk_tssi_pause_8852a(rf, path, false);
		}
	}
}

u8 _dpk_bypass_check_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy)
{
	struct halrf_fem_info *fem = &rf->fem;

	u8 result;

	if (fem->epa_2g && (rf->hal_com->band[phy].cur_chandef.band == BAND_ON_24G)) {
		RF_DBG(rf, DBG_RF_DPK, "[DPK] Skip DPK due to 2G_ext_PA exist!!\n");
		result = 1;
	} else if (fem->epa_5g && (rf->hal_com->band[phy].cur_chandef.band == BAND_ON_5G)) {
		RF_DBG(rf, DBG_RF_DPK, "[DPK] Skip DPK due to 5G_ext_PA exist!!\n");
		result = 1;
	} else if (fem->epa_6g && (rf->hal_com->band[phy].cur_chandef.band == BAND_ON_6G)) {
		RF_DBG(rf, DBG_RF_DPK, "[DPK] Skip DPK due to 6G_ext_PA exist!!\n");
		result = 1;
	} else
		result = 0;

	return result;
}

void _dpk_force_bypass_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy)
{
	u8 path, kpath;

	kpath = halrf_kpath_8852a(rf, phy);

	for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {
		if (kpath & BIT(path))
			halrf_dpk_onoff_8852a(rf, path, true);
	}
}

void halrf_dpk_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	bool force)
{
	RF_DBG(rf, DBG_RF_DPK, "[DPK] ****** DPK Start (Ver: 0x%x, Cv: %d, RF_para: %d) ******\n",
		DPK_VER_8852A, rf->hal_com->cv, RF_RELEASE_VERSION_8852A);

	RF_DBG(rf, DBG_RF_DPK, "[DPK] Driver mode = %d\n", rf->phl_com->drv_mode);
#if 1
	if (_dpk_bypass_check_8852a(rf, phy))
		_dpk_force_bypass_8852a(rf, phy);
	else
		_dpk_cal_select_8852a(rf, force, phy, halrf_kpath_8852a(rf, phy));
#else
	_dpk_information_8852a(rf, 0, RF_PATH_A);
	_dpk_bb_afe_setting_8852a(rf, 0, RF_PATH_A);
	
	//_dpk_main_8852a(rf, 0, 0, 1, RF_PATH_A);
	_dpk_set_tx_pwr_8852a(rf, 1, RF_PATH_A);
	_dpk_rf_setting_8852a(rf, 1, RF_PATH_A);
	halrf_set_rx_dck_8852a(rf, RF_PATH_A, false);

	_dpk_cip_setting_8852a(rf, RF_PATH_A);
	_dpk_manual_txcfir_8852a(rf, RF_PATH_A, true);
	_dpk_bypass_rxcfir_8852a(rf, RF_PATH_A, true);
	
	//_dpk_sync_8852a(rf, RF_PATH_A);
	//_dpk_dgain_read_8852a(rf);
	//_dpk_gainloss_8852a(rf, RF_PATH_A);
	_dpk_idl_mpa_8852a(rf, 0, 1, RF_PATH_A);
	_dpk_fill_result_8852a(rf, 0, 1, RF_PATH_A, 0x36);
	//halrf_dpk_onoff_8852a(rf, RF_PATH_A, 0);
	//_dpk_reload_rf_8852a(rf, rf_reg, rf_bkup, kpath);
	//halrf_rf_direct_cntrl_8852a(rf, RF_PATH_A, true);
	//_dpk_bb_afe_restore_8852a(rf, phy, RF_PATH_A);

#endif
}

void halrf_dpk_onoff_8852a(
	struct rf_info *rf,
	enum rf_path path,
	bool off)
{
	struct halrf_dpk_info *dpk = &rf->dpk;

	u8 val, kidx = dpk->cur_idx[path];

	val = dpk->is_dpk_enable & (!off) & dpk->bp[path][kidx].path_ok;

	halrf_wreg(rf, 0x81bc + (path << 8) + (kidx << 2), MASKBYTE3, 0x6 | val);

	//halrf_wreg(rf, 0x81bc + (path << 8), BIT(24), dpk->is_dpk_enable & (!off)); /*ch0*/
	//halrf_wreg(rf, 0x81c0 + (path << 8), BIT(24), dpk->is_dpk_enable & (!off)); /*ch1*/

	RF_DBG(rf, DBG_RF_DPK, "[DPK] S%d[%d] DPK %s !!!\n", path, kidx,
		   (dpk->is_dpk_enable & (!off)) ? "enable" : "disable");
}

void halrf_dpk_track_8852a(
	struct rf_info *rf)
{
	struct halrf_dpk_info *dpk = &rf->dpk;
	struct halrf_tssi_info *tssi_info = &rf->tssi;

	u8 path, i, kidx;
	u8 trk_idx = 0, txagc_rf = 0;
	s8 txagc_bb = 0, txagc_bb_tp = 0, ini_diff = 0, txagc_ofst = 0;
	u16 pwsf[2];
	u8 cur_ther, ther_avg_cnt = 0;
	u32 ther_avg[2] = {0};
	s8 delta_ther[2] = {0};

	for (path = 0; path < DPK_RF_PATH_MAX_8852A; path++) {

		kidx = dpk->cur_idx[path];

		RF_DBG(rf, DBG_RF_DPK_TRACK,
		       "[DPK_TRK] ================[S%d[%d] (CH %d)]================\n",
		       path, kidx, dpk->bp[path][kidx].ch);

		//cur_ther = halrf_get_thermal_8852a(rf, path);
		cur_ther = (u8)halrf_rreg(rf, 0x1c10 + (path << 13), 0x3F000000); /*[29:24]*/

		RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] thermal now = %d\n", cur_ther);

		dpk->ther_avg[path][dpk->ther_avg_idx] = cur_ther;

		/*Average times */
		ther_avg_cnt = 0;
		for (i = 0; i < THERMAL_DPK_AVG_NUM; i++) {
			if (dpk->ther_avg[path][i]) {
				ther_avg[path] += dpk->ther_avg[path][i];
				ther_avg_cnt++;
#if 0
				RF_DBG(rf, DBG_RF_DPK_TRACK,
				       "[DPK_TRK] thermal avg[%d] = %d\n", i,
					       dpk->thermal_dpk_avg[path][i]);
#endif
			}
		}

		/*Calculate Average ThermalValue after average enough times*/
		if (ther_avg_cnt) {
			cur_ther = (u8)(ther_avg[path] / ther_avg_cnt);
#if 0
			RF_DBG(rf, DBG_RF_DPK_TRACK,
			       "[DPK_TRK] thermal avg total = %d, avg_cnt = %d\n",
			       ther_avg[path], ther_avg_cnt);
#endif
			RF_DBG(rf, DBG_RF_DPK_TRACK,
			       "[DPK_TRK] thermal avg = %d (DPK @ %d)\n",
			       cur_ther, dpk->bp[path][kidx].ther_dpk);
		}

		if (dpk->bp[path][kidx].ch != 0 && cur_ther != 0)
			delta_ther[path] = dpk->bp[path][kidx].ther_dpk - cur_ther;

		if (dpk->bp[path][kidx].band == 0) /*2G*/
			delta_ther[path] = delta_ther[path] * 3 / 2;
		else
			delta_ther[path] = delta_ther[path] * 5 / 2;

		txagc_rf = (u8)halrf_rreg(rf, 0x1c60 + (path << 13), 0x0000003f); /*[5:0]*/

		if (rf->is_tssi_mode[path]) { /*TSSI mode*/
			trk_idx = (u8)halrf_rrf(rf, path, 0x5D, 0xFC000); /*[19:14] for integer*/

			RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] txagc_RF / track_idx = 0x%x / %d\n",
			       txagc_rf, trk_idx);
			if (rf->hal_com->cv == CAV) {
				txagc_bb = (s8)halrf_rreg(rf, 0x4360 + (path << 8), MASKBYTE2); /*[23:16]*/
				txagc_bb_tp = (u8)halrf_rreg(rf, 0x4304 + (path << 8), 0x00000007); /*[2:0]*/
			} else {
				txagc_bb = (s8)halrf_rreg(rf, 0x1c60 + (path << 13), MASKBYTE2); /*[23:16]*/
				txagc_bb_tp = (u8)halrf_rreg(rf, 0x1c04 + (path << 13), 0x00000007); /*[2:0]*/
			}

			RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] txagc_bb_tp / txagc_bb = 0x%x / 0x%x\n",
			       txagc_bb_tp, txagc_bb);

			if (rf->hal_com->cv == CAV)
				txagc_ofst = (s8)halrf_rreg(rf, 0x4360 + (path << 8), MASKBYTE3); /*[31:24]*/
			else
				txagc_ofst = (s8)halrf_rreg(rf, 0x1c60 + (path << 13), MASKBYTE3); /*[31:24]*/

			RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] txagc_offset / delta_ther = %d / %d\n", txagc_ofst, delta_ther[path]);

			if (halrf_rreg(rf, 0x81c8 + (path << 8), BIT(15)) == 0x1)
				txagc_ofst = 0;

			if (txagc_rf != 0 && cur_ther != 0)
				ini_diff = txagc_ofst + (delta_ther[path]);
#if 0
			RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] txagc_RF@DPK / track_idx@DPK = 0x%x / %d\n",
			       dpk->bp[0].txagc_dpk[path], dpk->bp[0].trk_idx_dpk[path]);

			if (txagc_rf != 0 && cur_ther != 0)
				ini_diff = (trk_idx - dpk->bp[0].trk_idx_dpk[path]) * 8 -
						(txagc_rf - dpk->bp[0].txagc_dpk[path]) * 8 +
						(delta_ther[path] * 8 / 5);
#endif
			if (halrf_rreg(rf, 0x58d4 + (path << 13), 0xf0000000) == 0x0) {
				pwsf[0] = dpk->bp[path][kidx].pwsf + txagc_bb_tp - txagc_bb + ini_diff + tssi_info->extra_ofst[path]; /*gain0*/
				pwsf[1] = dpk->bp[path][kidx].pwsf + txagc_bb_tp - txagc_bb + ini_diff + tssi_info->extra_ofst[path]; /*gain1*/
			} else {
				pwsf[0] = dpk->bp[path][kidx].pwsf + ini_diff + tssi_info->extra_ofst[path]; /*gain0*/
				pwsf[1] = dpk->bp[path][kidx].pwsf + ini_diff + tssi_info->extra_ofst[path]; /*gain1*/
			}

		} else { /*without any tx power tracking mechanism*/
			pwsf[0] = (dpk->bp[path][kidx].pwsf + delta_ther[path]) & 0x1ff; /*gain0*/
			pwsf[1] = (dpk->bp[path][kidx].pwsf + delta_ther[path]) & 0x1ff; /*gain1*/
		}

		if (rf->rfk_is_processing != true && halrf_rreg(rf, 0x80f0, BIT(31)) == 0x0 && txagc_rf != 0) {
			RF_DBG(rf, DBG_RF_DPK_TRACK, "[DPK_TRK] New pwsf[0] / pwsf[1] = 0x%x / 0x%x\n",
			       pwsf[0], pwsf[1]);

			halrf_wreg(rf, 0x81b4 + (path << 8) + (kidx << 2), 0x000001FF, pwsf[0]);
			halrf_wreg(rf, 0x81b4 + (path << 8) + (kidx << 2), 0x01FF0000, pwsf[1]);
		}
	}
	dpk->ther_avg_idx++;

	if (dpk->ther_avg_idx == THERMAL_DPK_AVG_NUM)
		dpk->ther_avg_idx = 0;
}

#endif
