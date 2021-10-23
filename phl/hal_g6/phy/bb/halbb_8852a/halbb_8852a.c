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

#ifdef HALBB_CONFIG_RUN_IN_DRV
bool halbb_chk_pkg_valid_8852a(struct bb_info *bb, u8 bb_ver, u8 rf_ver)
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

bool halbb_fw_polling_tx_done_8852a(struct bb_info *bb)
{
	struct bb_c2h_fw_tx_rpt *fw_tx_i = &bb->bb_fwtx_c2h_i;

	/* Polling Tx status */
	while(!(fw_tx_i->tx_done)) {
	}
	/* set to default value */
	fw_tx_i->tx_done = false;

	/* return value for driver */
	return true;
}

void halbb_plcp_fw_tx_8852a(struct bb_info *bb, struct halbb_pmac_info *tx_info,
		     	    enum phl_phy_idx phy_idx)
{
	struct bb_h2c_fw_tx_setting *fw_tx_i = &bb->bb_fwtx_h2c_i;
	u8 cmdlen;
	u32 period = 0;
	u32 duty_cycle = 100;
	bool ret_val = false;
	u32 *bb_h2c = (u32 *)fw_tx_i;
	cmdlen = sizeof(struct bb_h2c_fw_tx_setting);

	/* Prevent error flow */
	if (tx_info->mode == FW_TRIG_TX) {
		fw_tx_i->pkt_cnt[0] = (u8) (tx_info->tx_cnt & 0x00ff);
		fw_tx_i->pkt_cnt[1] = (u8) ((tx_info->tx_cnt & 0xff00) >> 8);
		fw_tx_i->tx_en = tx_info->en_pmac_tx;
		
		if (tx_info->period >= 1000) {
			period = tx_info->period + 24;
			period = period>>10;
			fw_tx_i->tx_type = 1;
			duty_cycle = 0;
		} else {
			period= 0;
			fw_tx_i->tx_type = 0;
			//duty_cycle = 100;
			if (tx_info->duty_cycle != 0)
				duty_cycle = tx_info->duty_cycle;
		}
		fw_tx_i->tx_period[0] = (u8) (period & 0x000000ff);
		fw_tx_i->tx_period[1] = (u8) ((period & 0x0000ff00) >> 8);
		fw_tx_i->tx_period[2] = (u8) ((period & 0x00ff0000) >> 16);
		fw_tx_i->tx_period[3] = (u8) ((period & 0xff000000) >> 24);
		fw_tx_i->duty_cycle[0] = (u8) (duty_cycle & 0x000000ff);
		fw_tx_i->duty_cycle[1] = (u8) ((duty_cycle & 0x0000ff00) >> 8);
		fw_tx_i->duty_cycle[2] = (u8) ((duty_cycle & 0x00ff0000) >> 16);
		fw_tx_i->duty_cycle[3] = (u8) ((duty_cycle & 0xff000000) >> 24);
		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] Tx pkt cnt=%d\n", tx_info->tx_cnt);
		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] Tx_en=%x\n", fw_tx_i->tx_en);
		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] Tx type=%d\n", fw_tx_i->tx_type);

		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] Tx period=%d\n", period);
		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] Tx duty=%d\n", duty_cycle);

		BB_DBG(bb, DBG_FW_INFO, "[FW][H2C] h2c conent=%x, %x, %x, %x\n", 
		       bb_h2c[0],bb_h2c[1],bb_h2c[2],bb_h2c[3]);


		ret_val = halbb_fill_h2c_cmd(bb, cmdlen, DM_H2C_FW_TRIG_TX, 
					     HALBB_H2C_DM, bb_h2c);
		if (ret_val == false)
				BB_WARNING(" H2C cmd: FW Tx error!!\n");
	}
	
}
#endif

__iram_func__
void halbb_stop_pmac_tx_8852a(struct bb_info *bb,
			      struct halbb_pmac_info *tx_info,
			      enum phl_phy_idx phy_idx)
{
	//u32 tmp = 0;
	
	if (tx_info->mode == CONT_TX) {
		if (phy_idx == HW_PHY_MAX)
			return;

		if (tx_info->is_cck) {
			halbb_set_reg(bb, 0x7838, BIT(17), 0);
			halbb_set_reg(bb, 0x7800, BIT(26), 1);
		} else {
			halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 0, phy_idx);
		}
	} 
	
	/* TX Disable */
	if (tx_info->mode == FW_TRIG_TX) {
		#ifdef HALBB_CONFIG_RUN_IN_DRV
		halbb_plcp_fw_tx_8852a(bb, tx_info, phy_idx);
		#endif
	} else {
		if (phy_idx == HW_PHY_MAX) {
			halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, HW_PHY_0);
			halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, HW_PHY_1);
		} else {
			halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, phy_idx);
		}
	}
}

__iram_func__
void halbb_start_pmac_tx_8852a(struct bb_info *bb,
			       struct halbb_pmac_info *tx_info,
			       enum halbb_pmac_mode mode, u16 pkt_cnt,u16 period,
			       u16 tx_time_outer, enum phl_phy_idx phy_idx)
{
	#ifdef HALBB_CONFIG_RUN_IN_DRV
	struct bb_c2h_fw_tx_rpt *fw_tx_i = &bb->bb_fwtx_c2h_i;
	#endif

	u16 i;

	if (mode == CONT_TX) {
		if (phy_idx == HW_PHY_MAX)
			return;

		if (tx_info->is_cck)
			halbb_set_reg(bb, 0x7838, BIT(17), 1);
		else
			halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 1, phy_idx);

		/*Tx Enable */
		halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 1, phy_idx);
	} else if (mode == PKTS_TX) {
		if (phy_idx == HW_PHY_MAX) {
			for (i = 0; i < pkt_cnt; i++) {
				if (tx_info->is_cck) {
					halbb_set_reg(bb, 0x7838, BIT(17), 0);
				} else {
					halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 0, HW_PHY_0);
					halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 0, HW_PHY_1);
				}
				/*Tx Enable */
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 1, HW_PHY_0);
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 1, HW_PHY_1);
				halbb_delay_us(bb, tx_time_outer);
				/*Tx Disable */
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, HW_PHY_0);
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, HW_PHY_1);
				halbb_delay_us(bb, period);
			}
		} else {
			for (i = 0; i < pkt_cnt; i++) {
				if (tx_info->is_cck)
					halbb_set_reg(bb, 0x7838, BIT(17), 0);
				else
					halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 0, phy_idx);

				/*Tx Enable */
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 1, phy_idx);
				halbb_delay_us(bb, tx_time_outer);
				/*Tx Disable */
				halbb_set_reg_cmn(bb, 0x14a8, BIT(3), 0, phy_idx);
				halbb_delay_us(bb, period);
			}
		}	
	} else if (mode == FW_TRIG_TX) {
		#ifdef HALBB_CONFIG_RUN_IN_DRV
		/* init tx status */
		fw_tx_i->tx_done = false;
		
		if (tx_info->is_cck)
			halbb_set_reg(bb, 0x7838, BIT(17), 0);
		else
			halbb_set_reg_cmn(bb, 0x14a8, BIT(2), 0, phy_idx);

		BB_DBG(bb, DBG_FW_INFO, "[FW] FW trigger Tx\n");
		halbb_plcp_fw_tx_8852a(bb, tx_info, phy_idx);
		#endif
	}
}

__iram_func__
void halbb_set_pmac_tx_8852a(struct bb_info *bb, struct halbb_pmac_info *tx_info,
			     enum phl_phy_idx phy_idx)
{
	/*==== Rx Path (For 52A CAV same TRx)====*/
	u8 rx_path_tmp = bb->rx_path;

	/* init fw tx rpt */
	#ifdef HALBB_CONFIG_RUN_IN_DRV
	struct bb_c2h_fw_tx_rpt *fw_tx_i = &bb->bb_fwtx_c2h_i;
	fw_tx_i->tx_done = 0;
	#endif

	if (!tx_info->en_pmac_tx) {
		halbb_stop_pmac_tx_8852a(bb, tx_info, phy_idx);
		/* PD hit enable */
		if (phy_idx == HW_PHY_MAX) {
			halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 0, HW_PHY_0);
			halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 0, HW_PHY_1);
		} else {
			halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 0, phy_idx);
		}
		halbb_set_reg(bb, 0x7844, BIT(31), 0);
		return;
	}
	/*Turn on PMAC */
	if (phy_idx == HW_PHY_MAX) {
		/* Tx */
		halbb_set_reg_cmn(bb, 0x0d80, BIT(0), 1, HW_PHY_0);
		halbb_set_reg_cmn(bb, 0x0d80, BIT(0), 1, HW_PHY_1);
		/* Rx */
		halbb_set_reg_cmn(bb, 0x0d80, BIT(16), 1, HW_PHY_0);
		halbb_set_reg_cmn(bb, 0x0d80, BIT(16), 1, HW_PHY_1);
		halbb_set_reg_cmn(bb, 0x0d88, 0x3f, 0x3f, HW_PHY_0);
		halbb_set_reg_cmn(bb, 0x0d88, 0x3f, 0x3f, HW_PHY_1);
	} else {
		/* Tx */
		halbb_set_reg_cmn(bb, 0x0d80, BIT(0), 1, phy_idx);
		/* Rx */
		halbb_set_reg_cmn(bb, 0x0d80, BIT(16), 1, phy_idx);
		halbb_set_reg_cmn(bb, 0x0d88, 0x3f, 0x3f, phy_idx);
	}
	

	/*==== CCK same TRx Path (For 52A CAV same TRx)====*/
	if (bb->hal_com->cv == CAV)
		if (tx_info->is_cck)
			halbb_ctrl_cck_rx_path_8852a(bb, bb->bb_cmn_backup_i.cur_tx_path);

	/* PD hit disable */
	if (phy_idx == HW_PHY_MAX) {
		halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 1, HW_PHY_0);
		halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 1, HW_PHY_1);
	} else {
		halbb_set_reg_cmn(bb, 0xa3c, BIT(9), 1, phy_idx);
	}
	halbb_set_reg(bb, 0x7844, BIT(31), 1);
	halbb_start_pmac_tx_8852a(bb, tx_info, tx_info->mode, tx_info->tx_cnt,
		       tx_info->period, tx_info->tx_time, phy_idx);
	/*==== Restore Rx Path (For 52A CAV same TRx)====*/
	if (bb->hal_com->cv == CAV)
		halbb_ctrl_cck_rx_path_8852a(bb, rx_path_tmp);
}

#ifdef HALBB_CONFIG_RUN_IN_DRV
void halbb_set_tmac_tx_8852a(struct bb_info *bb, enum phl_phy_idx phy_idx)
{
	/* To do: 0x0d80[16] [25] / 0x0d88[5:0] Should be set to default value in parameter package*/
	/* Turn on TMAC */
	halbb_set_reg_cmn(bb, 0x0d80, BIT(0), 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0d80, BIT(16), 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0d88, 0xfff, 0, phy_idx);
	halbb_set_reg_cmn(bb, 0x0d94, 0xf0, 0, phy_idx);
}

void halbb_rx_setting_8852a(struct bb_info *bb, u8 patch_idx)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (patch_idx == 0) {
		// default
		halbb_set_reg(bb, 0x1724, MASKDWORD, 0x576DF814);
		halbb_set_reg(bb, 0x1340, MASKDWORD, 0xA80668A0);
		halbb_set_reg(bb, 0x1878, MASKDWORD, 0x400CD62D);
		halbb_set_reg(bb, 0x16bc, MASKDWORD, 0x922A8153);
		halbb_set_reg(bb, 0x1718, MASKDWORD, 0x2318C610);
		halbb_set_reg(bb, 0x16b8, MASKDWORD, 0x9248C631);
		halbb_set_reg(bb, 0x171c, MASKDWORD, 0x45336753);
		halbb_set_reg(bb, 0x134c, MASKDWORD, 0x32488A62);
		halbb_set_reg(bb, 0x1720, MASKDWORD, 0x236A7A88);
		halbb_set_reg(bb, 0xa68, MASKDWORD, 0x100000FF);
		halbb_set_reg(bb, 0x16c0, MASKDWORD, 0x00000005);
		halbb_set_reg(bb, 0x16b0, MASKDWORD, 0x98682C18);
		halbb_set_reg(bb, 0xa40, MASKDWORD, 0x00000000);
	} else if (patch_idx == 1) {
		// patch 1
		halbb_set_reg(bb, 0x1724, MASKDWORD, 0x576df994);
		halbb_set_reg(bb, 0x16bc, MASKDWORD, 0x922a8153);
		halbb_set_reg(bb, 0x1718, MASKDWORD, 0x6318c610);
		halbb_set_reg(bb, 0x16b8, MASKDWORD, 0x9248c631);
		halbb_set_reg(bb, 0x171c, MASKDWORD, 0x45336753);
		halbb_set_reg(bb, 0x1348, MASKDWORD, 0x9f28518c);
		halbb_set_reg(bb, 0x1720, MASKDWORD, 0x236a8a88);
	} else {
		// patch 2
		halbb_set_reg(bb, 0x1724, MASKDWORD, 0x776dfb14);
		halbb_set_reg(bb, 0x1340, MASKDWORD, 0xe80668a0);
		halbb_set_reg(bb, 0x1878, MASKDWORD, 0x800c562d);
		halbb_set_reg(bb, 0xa44, MASKDWORD, 0x14500807);
		halbb_set_reg(bb, 0xa68, MASKDWORD, 0x900000ff);
		halbb_set_reg(bb, 0x16c0, MASKDWORD, 0x00000004);
		halbb_set_reg(bb, 0x16b8, MASKDWORD, 0x8248c631);
		halbb_set_reg(bb, 0x16b0, MASKDWORD, 0x98682a98);
		halbb_set_reg(bb, 0x171c, MASKDWORD, 0x45337753);
		halbb_set_reg(bb, 0x134c, MASKDWORD, 0x32488a60);
		halbb_set_reg(bb, 0x1720, MASKDWORD, 0x236a9a88);
		halbb_set_reg(bb, 0x16bc, MASKDWORD, 0x122a8253);
		halbb_set_reg(bb, 0xa40, MASKDWORD, 0x00000060);
		halbb_set_reg(bb, 0x1718, MASKDWORD, 0x6318c610);
	}
	BB_DBG(bb, DBG_PHY_CONFIG, "[Rx setting] Patch : %d\n", patch_idx);
}

void halbb_ic_hw_setting_init_8852a(struct bb_info *bb)
{
	BB_DBG(bb, DBG_PHY_CONFIG, "<====== %s ======>\n", __func__);

	if (bb->phl_com->drv_mode != RTW_DRV_MODE_NORMAL) {
		// Rx Setting Patch-0 (MP default)
		halbb_rx_setting_8852a(bb, 0);
		// r_en_sound_wo_ndp
		halbb_set_reg(bb, 0x457c, BIT(1), 1);
	} else { /*if (bb->phl_com->drv_mode == RTW_DRV_MODE_NORMAL)*/
		// Rx Setting Patch-1 (Normal default)
		halbb_rx_setting_8852a(bb, 1);
		// r_en_sound_wo_ndp
		halbb_set_reg(bb, 0x457c, BIT(1), 0);
	}
}
#endif
#endif
