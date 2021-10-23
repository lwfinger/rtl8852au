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

#ifdef BB_DYN_1R_CCA
void halbb_dyn_1r_cca_cfg_8852a_2(struct bb_info *bb, enum rf_path rx_path)
{
	BB_DBG(bb, DBG_IC_API, "[%s]rx_path=%d\n", __func__, rx_path);

	if (bb->bb_8852a_2_i.dyn_1r_cca_cfg == rx_path)
		return;

	bb->bb_8852a_2_i.dyn_1r_cca_cfg = rx_path;

	halbb_1r_cca_cr_cfg_8852a_2(bb, rx_path);
}

void halbb_dyn_1r_cca_8852a_2(struct bb_info *bb)
{
	struct bb_link_info *bb_link = &bb->bb_link_i;
	struct rtw_phl_stainfo_t *phl_sta;
	struct rtw_rssi_info *rssi_t = NULL;
	enum rf_path cfg = RF_PATH_AB;
	bool sta_found = false;
	u16 rssi_diff, rssi_min; /*RSSI  u(16,5)*/
	u8 i, j;

	if (!bb->bb_8852a_2_i.dyn_1r_cca_en)
		return;

	BB_DBG(bb, DBG_IC_API, "[%s]\n", __func__);

	if (!bb_link->is_linked || !bb_link->is_one_entry_only) {
		BB_DBG(bb, DBG_IC_API, "is_linked=%d, one_entry_only=%d\n",
		       bb_link->is_linked, bb_link->is_one_entry_only);

		halbb_dyn_1r_cca_cfg_8852a_2(bb, RF_PATH_AB);
		return;
	}

	for (i = 0; i < PHL_MAX_STA_NUM; i++) {
		if (!bb->sta_exist[i])
			continue;

		phl_sta = bb->phl_sta_info[i];

		if (!is_sta_active(phl_sta))
			continue;

		rssi_t = &phl_sta->hal_sta->rssi_stat;

		for (j = 0; j < HALBB_MAX_PATH; j++) {
			halbb_print_sign_frac_digit(bb, rssi_t->rssi_ma_path[j], 16, 5, bb->dbg_buf, HALBB_SNPRINT_SIZE_S);
			BB_DBG(bb, DBG_IC_API, "[id:%d]rssi[%d]= %s\n",
			       i, j, bb->dbg_buf);
		}
		sta_found = true;
		break;
	}

	if (!sta_found)
		return;

	if (rssi_t->rssi_ma_path[0] >= rssi_t->rssi_ma_path[1]) {
		cfg = RF_PATH_A;
		rssi_diff = rssi_t->rssi_ma_path[0] - rssi_t->rssi_ma_path[1];
		rssi_min = rssi_t->rssi_ma_path[1];
	} else {
		cfg = RF_PATH_B;
		rssi_diff = rssi_t->rssi_ma_path[1] - rssi_t->rssi_ma_path[0];
		rssi_min = rssi_t->rssi_ma_path[0];
	}

	BB_DBG(bb, DBG_IC_API, "[Dyn 1R CCA]en=%d, diff_th=%d, min_th=%d\n",
	       bb->bb_8852a_2_i.dyn_1r_cca_en,
	       bb->bb_8852a_2_i.dyn_1r_cca_rssi_diff_th >> 5,
	       bb->bb_8852a_2_i.dyn_1r_cca_rssi_min_th >> 5);

	if (rssi_diff < bb->bb_8852a_2_i.dyn_1r_cca_rssi_diff_th &&
	    rssi_min > bb->bb_8852a_2_i.dyn_1r_cca_rssi_min_th) {
		BB_DBG(bb, DBG_IC_API, "[1R_CCA] path=%s\n", (cfg == RF_PATH_A) ? "A" : "B");
	} else {
		BB_DBG(bb, DBG_IC_API, "[2R_CCA]\n");
		cfg = RF_PATH_AB;
	}

	halbb_dyn_1r_cca_cfg_8852a_2(bb, cfg);
}

void halbb_dyn_1r_cca_init_8852a_2(struct bb_info *bb)
{
	bb->bb_8852a_2_i.dyn_1r_cca_en = false;
	bb->bb_8852a_2_i.dyn_1r_cca_cfg = RF_PATH_ABCD;
	bb->bb_8852a_2_i.dyn_1r_cca_rssi_diff_th= 10 << 5;
	bb->bb_8852a_2_i.dyn_1r_cca_rssi_min_th= 40 << 5;
}

void halbb_dyn_1r_cca_en_8852a_2(struct bb_info *bb, bool en)
{
	bb->bb_8852a_2_i.dyn_1r_cca_en = en;
}
#endif

void halbb_manual_pkt_fmt_sel_8852a_2(struct bb_info *bb, enum bb_pfd_fmt_type type) {

	bb->bb_8852a_2_i.pfd_fmt_curr = type;

	if (type == PFD_ALL_FMT) {
		halbb_set_reg(bb, 0x439c, 0xf, 8);
		return;
	}

	halbb_set_reg(bb, 0x439c, 0xf, type & 0x7);
}

#ifdef BB_TDMA_PFD
void halbb_pkt_fmt_sel_callback_8852a_2(void *context)
{
	struct bb_info *bb = (struct bb_info *)context;

	BB_DBG(bb, DBG_IC_API, "Callback ===>\n");
	halbb_pkt_fmt_sel_main_8852a_2(bb);
}

void halbb_pkt_fmt_sel_timers_8852a_2(struct bb_info *bb, enum bb_timer_state_t state)
{
	if (bb->ic_type != BB_RTL8852A)
		return;

	BB_DBG(bb, DBG_IC_API, "[%s] state = %d\n", __func__, state);

	if (state == BB_INIT_TIMER) {
		halbb_init_timer(bb, &bb->bb_8852a_2_i.pfd_timer, halbb_pkt_fmt_sel_callback_8852a_2, bb, "halbb_pfd_timers");
	} else if (state == BB_CANCEL_TIMER) {
		halbb_cancel_timer(bb, &bb->bb_8852a_2_i.pfd_timer);
	} else if (state == BB_RELEASE_TIMER) {
		halbb_release_timer(bb, &bb->bb_8852a_2_i.pfd_timer);
	}
}

void halbb_pkt_fmt_sel_main_8852a_2(struct bb_info *bb)
{
	struct bb_8852a_2_info	*bb8852a_2 = &bb->bb_8852a_2_i;
	enum bb_pfd_fmt_type pfd_fmt_tmp = bb->bb_8852a_2_i.pfd_fmt_curr;
	u32 pfd_period_ms = 0;

	if (!bb8852a_2->tdma_pfd_en) {
		BB_DBG(bb, DBG_IC_API, "[%s] tdma_pfd_en = %d\n", __func__, bb8852a_2->tdma_pfd_en);
		return;
	}

	if (pfd_fmt_tmp == PFD_ALL_FMT) {
		pfd_period_ms = bb8852a_2->pfd_period_ms_0;
		bb->bb_8852a_2_i.pfd_fmt_curr = PFD_LEGACY; /*CR1*/
	} else { /*PFD_LEGACY*/
		pfd_period_ms = bb8852a_2->pfd_period_ms_1;
		bb->bb_8852a_2_i.pfd_fmt_curr = PFD_ALL_FMT;  /*CR2*/
	}

	halbb_manual_pkt_fmt_sel_8852a_2(bb, bb->bb_8852a_2_i.pfd_fmt_curr);
	halbb_set_timer(bb, &bb8852a_2->pfd_timer, pfd_period_ms);

	BB_DBG(bb, DBG_IC_API, "(%03d)->(%03d) (%d ms)\n",
	       pfd_fmt_tmp, bb->bb_8852a_2_i.pfd_fmt_curr, pfd_period_ms);
}

void halbb_pkt_fmt_sel_deinit_8852a_2(struct bb_info *bb)
{
	BB_DBG(bb, DBG_IC_API, "[%s]\n", __func__);
	halbb_pkt_fmt_sel_timers_8852a_2(bb, BB_CANCEL_TIMER);
	halbb_pkt_fmt_sel_timers_8852a_2(bb, BB_RELEASE_TIMER);
}

void halbb_pkt_fmt_sel_init_8852a_2(struct bb_info *bb)
{
	struct bb_8852a_2_info	*bb8852a_2 = &bb->bb_8852a_2_i;

	BB_DBG(bb, DBG_IC_API, "[%s]\n", __func__);
	bb8852a_2->tdma_pfd_en = false;
	bb8852a_2->pfd_period_ms_0 = 30;
	bb8852a_2->pfd_period_ms_1 = 30;
	halbb_pkt_fmt_sel_timers_8852a_2(bb, BB_INIT_TIMER);
}
#endif

bool halbb_chk_pkg_valid_8852a_2(struct bb_info *bb, u8 bb_ver, u8 rf_ver)
{
	bool valid = true;

#if 0
	if (bb_ver >= X && rf_ver >= Y)
		valid = true;
	else if (bb_ver < X && rf_ver < Y)
		valid = true;
	else
		valid = false;
#endif

	if (!valid) {
		/*halbb_set_reg(bb, 0x1c3c, (BIT(0) | BIT(1)), 0x0);*/
		BB_WARNING("[%s] Pkg_ver{bb, rf}={%d, %d} disable all BB block\n",
			 __func__, bb_ver, rf_ver);
	}

	return valid;
}

bool halbb_chk_tx_idle_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	u8 tx_state = 0;
	bool idle = 0;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	halbb_set_reg(bb, 0x20f8, BIT(31), 1);

	if (phy_idx == HW_PHY_0)
		halbb_set_reg(bb, 0x20f0, MASKDWORD, 0x30002);
	else
		halbb_set_reg(bb, 0x20f0, MASKDWORD, 0x30005);

	tx_state = (u8)halbb_get_reg(bb, 0x1730, 0x3f);

	if (tx_state == 0)
		idle = true;
	else
		idle = false;

	return idle;
}

void halbb_stop_pmac_tx_8852a_2(struct bb_info *bb,
			      struct halbb_pmac_info *tx_info,
			      enum phl_phy_idx phy_idx)
{
	if (tx_info->is_cck) { // CCK
		if (tx_info->mode == CONT_TX) {
			halbb_set_reg(bb, 0x2300, BIT(26), 1);
			halbb_set_reg(bb, 0x2338, BIT(17), 0);
			halbb_set_reg(bb, 0x2300, BIT(28), 0);
			halbb_set_reg(bb, 0x2300, BIT(26), 0);
		} else if (tx_info->mode == PKTS_TX) {
			halbb_set_reg_cmn(bb, 0x9c4, BIT(4), 0, phy_idx);
		} else if (tx_info->mode == CCK_CARRIER_SIPPRESSION_TX) {
			halbb_set_reg_cmn(bb, 0x9c4, BIT(4), 0, phy_idx);
			/*Carrier Suppress Tx*/
			halbb_set_reg(bb, 0x2338, BIT(18), 0);
			/*Enable scrambler at payload part*/
			halbb_set_reg(bb, 0x2304, BIT(26), 0);
		}
	} else { // OFDM
		if (tx_info->mode == CONT_TX)
			halbb_set_reg_cmn(bb, 0x9c4, BIT(0), 0, phy_idx);
		else if (tx_info->mode == PKTS_TX)
			halbb_set_reg_cmn(bb, 0x9c4, BIT(4), 0, phy_idx);
	}
}

void halbb_start_pmac_tx_8852a_2(struct bb_info *bb,
			       struct halbb_pmac_info *tx_info,
			       enum halbb_pmac_mode mode, u32 pkt_cnt,u16 period,
			       enum phl_phy_idx phy_idx)
{
	if (mode == CONT_TX) {
		if (tx_info->is_cck) {
			halbb_set_reg(bb, 0x2338, BIT(17), 1);
			halbb_set_reg(bb, 0x2300, BIT(28), 0);
		} else {
			halbb_set_reg_cmn(bb, 0x9c4, BIT(0), 1, phy_idx);
		}
	} else if (mode == PKTS_TX) {
		/*Tx_N_PACKET_EN */
		halbb_set_reg_cmn(bb, 0x9c4, BIT(4), 1, phy_idx);
		/*Tx_N_PERIOD */
		halbb_set_reg_cmn(bb, 0x9c4, 0xffffff00, period, phy_idx);
		/*Tx_N_PACKET */
		halbb_set_reg_cmn(bb, 0x9c8, 0xffffffff, pkt_cnt, phy_idx);
	} else if (mode == CCK_CARRIER_SIPPRESSION_TX) {
		if (tx_info->is_cck) {
			/*Carrier Suppress Tx*/
			halbb_set_reg(bb, 0x2338, BIT(18), 1);
			/*Disable scrambler at payload part*/
			halbb_set_reg(bb, 0x2304, BIT(26), 1);
		} else {
			return;
		}
		/*Tx_N_PACKET_EN */
		halbb_set_reg_cmn(bb, 0x9c4, BIT(4), 1, phy_idx);
		/*Tx_N_PERIOD */
		halbb_set_reg_cmn(bb, 0x9c4, 0xffffff00, period, phy_idx);
		/*Tx_N_PACKET */
		halbb_set_reg_cmn(bb, 0x9c8, 0xffffffff, pkt_cnt, phy_idx);
	}
	/*Tx_EN */
	halbb_set_reg_cmn(bb, 0x9c0, BIT(0), 1, phy_idx);
	halbb_set_reg_cmn(bb, 0x9c0, BIT(0), 0, phy_idx);
}

void halbb_set_pmac_tx_8852a_2(struct bb_info *bb, struct halbb_pmac_info *tx_info,
			     enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (!tx_info->en_pmac_tx) {
		halbb_stop_pmac_tx_8852a_2(bb, tx_info, phy_idx);
		/* PD hit enable */
		halbb_set_reg_cmn(bb, 0xc3c, BIT(9), 0, phy_idx);
		if (bb->hal_com->band[phy_idx].cur_chandef.band == BAND_ON_24G)
			halbb_set_reg(bb, 0x2344, BIT(31), 0);
		return;
	}
	/*Turn on PMAC */
	/* Tx */
	halbb_set_reg_cmn(bb, 0x0980, BIT(0), 1, phy_idx);
	/* Rx */
	halbb_set_reg_cmn(bb, 0x0980, BIT(16), 1, phy_idx);
	halbb_set_reg_cmn(bb, 0x0988, 0x3f, 0x3f, phy_idx);

	/* PD hit enable */
	halbb_set_reg_cmn(bb, 0x704, BIT(1), 0, phy_idx);
	halbb_set_reg_cmn(bb, 0xc3c, BIT(9), 1, phy_idx);
	halbb_set_reg(bb, 0x2344, BIT(31), 1);
	halbb_set_reg_cmn(bb, 0x704, BIT(1), 1, phy_idx);

	halbb_start_pmac_tx_8852a_2(bb, tx_info, tx_info->mode, tx_info->tx_cnt,
		       tx_info->period, phy_idx);
}

void halbb_set_tmac_tx_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	/* To do: 0x0d80[16] [25] / 0x0d88[5:0] Should be set to default value in parameter package*/
	/* Turn on TMAC */
	halbb_set_reg_cmn(bb, 0x0980, BIT(0), 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0980, BIT(16), 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0988, 0xfff, 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0994, 0xf0, 0, phy_idx);
	// PDP bypass from TMAC
	halbb_set_reg_cmn(bb, 0x09a4, BIT(10), 0, phy_idx);
	// TMAC Tx path
	halbb_set_reg_cmn(bb, 0x09a4, 0x1c, 0, phy_idx);
	// TMAC Tx power
	halbb_set_reg_cmn(bb, 0x09a4, BIT(16), 0, phy_idx);
}

void halbb_dpd_bypass_8852a_2(struct bb_info *bb, bool pdp_bypass,
			      enum phl_phy_idx phy_idx)
{
	halbb_set_reg_cmn(bb, 0x09a4, BIT(10), 1, phy_idx);
	halbb_set_reg_cmn(bb, 0x45b8, BIT(16), pdp_bypass, phy_idx);
}

void halbb_6m_hw_setting_8852a_2(struct bb_info *bb)
{
	/*for 6M issue*/
	halbb_set_reg(bb, 0xc, BIT(0), 1);
	halbb_set_reg(bb, 0xdc0, MASKDWORD, 0x864FA000);
	halbb_set_reg(bb, 0xdc4, MASKDWORD, 0x43F);
	halbb_set_reg(bb, 0xdc8, MASKDWORD, 0x7FFF);
	halbb_set_reg(bb, 0xd84, BIT(15), 1);
	halbb_set_reg(bb, 0x58dc, BIT(30), 1);
	halbb_set_reg(bb, 0x78dc, BIT(30), 1);
	halbb_set_reg(bb, 0x738, 0xf0000, 0xf);
}

void halbb_ic_hw_setting_init_8852a_2(struct bb_info *bb)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bb->phl_com->drv_mode != RTW_DRV_MODE_NORMAL) {
		// r_en_sound_wo_ndp
		halbb_set_reg(bb, 0xd7c, BIT(1), 1);
		halbb_set_reg(bb, 0x2d7c, BIT(1), 1);
		// cck rxin settings
		halbb_set_reg(bb, 0x2388, BIT(26) | BIT(25), 0);
		halbb_set_reg(bb, 0x233C, BIT(27), 0);
		halbb_set_reg(bb, 0x233C, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2340, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2354, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2358, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2374, 0x3fc0000, 0xdd);
		halbb_set_reg(bb, 0x2378, 0xff, 0xbf);
		halbb_set_reg(bb, 0x2360, 0xff00, 0xb0);
	} else { /*if (bb->phl_com->drv_mode == RTW_DRV_MODE_NORMAL)*/
		// r_en_sound_wo_ndp
		halbb_set_reg(bb, 0xd7c, BIT(1), 0);
		halbb_set_reg(bb, 0x2d7c, BIT(1), 0);
		// cck rxin settings
		halbb_set_reg(bb, 0x2388, BIT(26) | BIT(25), 0);
		halbb_set_reg(bb, 0x233C, BIT(27), 0);
		halbb_set_reg(bb, 0x233C, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2340, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2354, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2358, 0x3ff, 0x234);
		halbb_set_reg(bb, 0x2374, 0x3fc0000, 0xdd);
		halbb_set_reg(bb, 0x2378, 0xff, 0xbf);
		halbb_set_reg(bb, 0x2360, 0xff00, 0xb0);
		#ifdef BB_TDMA_PFD
		halbb_pkt_fmt_sel_init_8852a_2(bb);
		#endif

		halbb_6m_hw_setting_8852a_2(bb);
		halbb_pop_en_8852a_2(bb, true, bb->bb_phy_idx);
	}

	if (bb->phl_com->dev_cap.rfe_type >= 50) {
		halbb_set_reg_phy0_1(bb, 0x42AC, 0x70000000, 0x4);
		halbb_set_reg_phy0_1(bb, 0x42B0, 0x7E000000, 0x22);
		halbb_set_reg_phy0_1(bb, 0x42B4, BIT(31) | BIT(30), 0x1);
		halbb_set_reg_phy0_1(bb, 0x42B8, BIT(31) | BIT(30), 0x1);
	}
#ifdef BB_DYN_1R_CCA
	halbb_dyn_1r_cca_init_8852a_2(bb);
#endif
}

void halbb_ic_hw_setting_h2c(struct bb_info *bb, u8 cmw_connect)
{
	struct bb_h2c_fw_cmw *fw_cmw_i = &bb->bb_fw_cmw_i;
	u8 cmdlen;
	bool ret_val = false;
	u32 *bb_h2c = (u32 *)fw_cmw_i;
	struct rtw_phl_com_t *phl = bb->phl_com;
	struct dev_cap_t *dev = &phl->dev_cap;

	cmdlen = sizeof(struct bb_h2c_fw_cmw);
	
	if (dev->rfe_type >= 50)
		return;

	fw_cmw_i->cmw_connect = cmw_connect;
	
	BB_DBG(bb, DBG_PHY_CONFIG, "[FW][H2C] h2c conent[0]=%x\n", bb_h2c[0]);

	ret_val = halbb_fill_h2c_cmd(bb, cmdlen, DM_H2C_FW_CMW, 
					     HALBB_H2C_DM, bb_h2c);
	if (ret_val == false)
		BB_WARNING(" H2C cmd: FW Tx error!!\n");

}

void halbb_ic_hw_setting_8852a_2(struct bb_info *bb)
{
	struct bb_link_info *link = &bb->bb_link_i;
	struct rtw_phl_stainfo_t *sta = NULL;
	struct rtw_rssi_info *sta_rssi = NULL;
	u16 rssi_a = 0;
	u16 rssi_b = 0;
	u16 rssi_path_diff = 0;
	u8 cmw_connect = bb->phl_com->id.id == 0x02010109;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if ((bb->hal_com->band[0].cur_chandef.band == BAND_ON_5G) &&
	    (link->is_one_entry_only)) {
		sta = bb->phl_sta_info[link->one_entry_macid];
		sta_rssi = &sta->hal_sta->rssi_stat;

		rssi_a = sta_rssi->rssi_ma_path[0];
		rssi_b = sta_rssi->rssi_ma_path[1];

		rssi_path_diff = DIFF_2(rssi_a, rssi_b) >> 5;

		if ((bb->bb_physts_i.bb_physts_rslt_hdr_i.rssi_avg >> 1) > 41) {
			halbb_set_reg(bb, 0x4148, BIT(30), 0x1);
			halbb_set_reg(bb, 0x4200, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x4210, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x4214, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x4220, 0xf0000000, 0x3);
			halbb_set_reg(bb, 0x4224, 0x3ffc00, 0x333);
			if (rssi_path_diff < 12) {
				halbb_set_reg(bb, 0x490c, BIT(31), 0);
				BB_DBG(bb, DBG_PHY_CONFIG,
				       "rssi(a,b)=(%d,%d),rssi_diff=(%d),ant_wgt_OFF\n",
				       rssi_a, rssi_b, rssi_path_diff);
			} else {
				halbb_set_reg(bb, 0x490c, BIT(31), 1);
				BB_DBG(bb, DBG_PHY_CONFIG,
				       "rssi(a,b)=(%d,%d),rssi_diff=(%d),ant_wgt_ON\n",
				       rssi_a, rssi_b, rssi_path_diff);
			}
		} else if ((bb->bb_physts_i.bb_physts_rslt_hdr_i.rssi_avg >> 1) < 39) {
			halbb_set_reg(bb, 0x4148, BIT(30), 0);
			halbb_set_reg(bb, 0x4200, 0xf0000000, 0);
			halbb_set_reg(bb, 0x4210, 0xf0000000, 0);
			halbb_set_reg(bb, 0x4214, 0xf0000000, 0);
			halbb_set_reg(bb, 0x4220, 0xf0000000, 0);
			halbb_set_reg(bb, 0x4224, 0x3ffc00, 0);
			halbb_set_reg(bb, 0x490c, BIT(31), 1);
			BB_DBG(bb, DBG_PHY_CONFIG, "rssi_avg=(%d),ant_wgt_ON\n",
			       bb->bb_physts_i.bb_physts_rslt_hdr_i.rssi_avg >> 1);
		}
	} else {
		halbb_set_reg(bb, 0x4148, BIT(30), 0);
		halbb_set_reg(bb, 0x4200, 0xf0000000, 0);
		halbb_set_reg(bb, 0x4210, 0xf0000000, 0);
		halbb_set_reg(bb, 0x4214, 0xf0000000, 0);
		halbb_set_reg(bb, 0x4220, 0xf0000000, 0);
		halbb_set_reg(bb, 0x4224, 0x3ffc00, 0);
		halbb_set_reg(bb, 0x490c, BIT(31), 1);
	}
	//BB_DBG(bb, DBG_PHY_CONFIG, "cmw connect(no_chk) = %d\n", cmw_connect);
	//halbb_ic_hw_setting_h2c(bb, 1);

	// CMW270 related patch
	if (bb->phl_com->id.id == 0x02010109) {
		halbb_set_reg(bb, 0x498c, BIT(0), 1);
		halbb_set_reg(bb, 0x497c, MASKDWORD, 0x10000000);
		halbb_set_reg(bb, 0x4980, MASKDWORD, 0);
		halbb_set_reg(bb, 0x4984, MASKDWORD, 0x10000000);
		halbb_set_reg(bb, 0x4988, MASKDWORD, 0);

		BB_DBG(bb, DBG_PHY_CONFIG, "cmw connect = %d\n", cmw_connect);
		halbb_ic_hw_setting_h2c(bb, cmw_connect);

	} else {
		halbb_set_reg(bb, 0x498c, BIT(0), 0);
		BB_DBG(bb, DBG_PHY_CONFIG, "cmw connect = %d\n", cmw_connect);
		halbb_ic_hw_setting_h2c(bb, cmw_connect);
	}
}

bool halbb_set_pd_lower_bound_8852a_2(struct bb_info *bb, u8 bound,
				      enum channel_width bw,
				      enum phl_phy_idx phy_idx)
{
	/* 
	Range of bound value: 
	BW20: 95~33
	BW40: 92~30
	BW80: 89~27
	*/
	u8 bw_attenuation = 0;
	u8 subband_filter_atteniation = 7;
	u8 bound_idx = 0;
	bool rpt = true;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bound == 0) {
		halbb_set_reg_cmn(bb, 0x481C, BIT(29), 0, phy_idx);
		BB_DBG(bb, DBG_PHY_CONFIG,
		       "[PD Bound] Set Boundary to default!\n");
		return true;
	}

	bb->bb_cmn_backup_i.cur_pd_lower_bound = bound;

	if (bw == CHANNEL_WIDTH_20) {
		bw_attenuation = 0;
	} else if (bw == CHANNEL_WIDTH_40) {
		bw_attenuation = 3;
	} else if (bw == CHANNEL_WIDTH_80) {
		bw_attenuation = 6;
	} else {
		BB_DBG(bb, DBG_PHY_CONFIG,
		       "[PD Bound] Only support BW20/40/80 !\n");
		return false;
	}

	bound += (bw_attenuation + subband_filter_atteniation);
	// If Boundary dbm is odd, set it to even number
	bound = bound % 2 ? bound + 1 : bound;

	if (bound < 40) {
		BB_DBG(bb, DBG_PHY_CONFIG,
		       "[PD Bound] Threshold too high, set to highest level!\n");
		bound = 40;
		rpt = false;
	}

	if (bound > 102) {
		BB_DBG(bb, DBG_PHY_CONFIG,
		       "[PD Bound] Threshold too low, disable PD lower bound function!\n");
		halbb_set_reg_cmn(bb, 0x481C, BIT(29), 0, phy_idx);
		return true;
	}

	bound_idx =  (102 - bound) >> 1;

	halbb_set_reg_cmn(bb, 0x481C, 0x7c0, bound_idx, phy_idx);
	halbb_set_reg_cmn(bb, 0x481C, BIT(29), 1, phy_idx);

	BB_DBG(bb, DBG_PHY_CONFIG, "[PD Bound] Set Boundary Success!\n");

	return rpt;
}

u8 halbb_querry_pd_lower_bound_8852a_2(struct bb_info *bb, bool get_en_info, enum phl_phy_idx phy_idx)
{
	u8 tmp;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (get_en_info)
		tmp = (u8)halbb_get_reg_cmn(bb, 0x481C, BIT(29), phy_idx);
	else
		tmp = bb->bb_cmn_backup_i.cur_pd_lower_bound;

	return tmp;
}

void halbb_pop_en_8852a_2(struct bb_info *bb, bool en, enum phl_phy_idx phy_idx)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (en) {
		halbb_set_reg_cmn(bb, 0x4798, BIT(8), 1, phy_idx);

		halbb_set_reg_cmn(bb, 0xc54, 0x6, 0, phy_idx);
		halbb_set_reg_cmn(bb, 0xc68, 0xff, 0x50, phy_idx);
		halbb_set_reg_cmn(bb, 0x4794, 0xF8000, 0xc, phy_idx);
		halbb_set_reg_cmn(bb, 0x4794, 0x1F00000, 8, phy_idx);
		halbb_set_reg_cmn(bb, 0x4798, BIT(9), 0, phy_idx);
	} else {
		halbb_set_reg_cmn(bb, 0x4798, BIT(8), 0, phy_idx);
	}
}

bool halbb_querry_pop_en_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	bool en;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	en = (bool)halbb_get_reg_cmn(bb, 0x4798, BIT(8), phy_idx);

	return en;
}

u16 halbb_get_per_8852a_2(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	u16 per;
	u32 crc_ok, crc_err, brk_cnt;
	u32 numer, denomer;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	crc_ok = halbb_mp_get_rx_crc_ok(bb, phy_idx);
	crc_err = halbb_mp_get_rx_crc_err(bb, phy_idx);
	brk_cnt = halbb_get_reg_cmn(bb, 0x1a08, MASKHWORD, phy_idx);

	if ((crc_ok > 0xffff) || (crc_err > 0xffff)) {
		BB_DBG(bb, DBG_DBG_API, "[PER] Value > Brk cnt upper bound!\n");
		return 0xffff;
	}

	denomer = crc_ok + crc_err + brk_cnt;
	numer = crc_err + brk_cnt + (denomer >> 1);
	per = (u16)HALBB_DIV(numer * 100, denomer);

	return per;
}

u8 halbb_get_losel_8852a_2(struct bb_info *bb)
{
	u8 tmp = 0;

	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	tmp = (u8)halbb_get_reg(bb, 0x35c, BIT(11) | BIT(10));
	return tmp;
}

#endif
