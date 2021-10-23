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
#include "../halrf_precomp.h"

#ifdef RF_8852A_SUPPORT

s8 _halrf_efuse_exchange_8852a(struct rf_info *rf, u8 value, u8 mask)
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

void _halrf_get_normal_efuse_rx_gain_k_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;
	u8 i, j, tmp;
	u32 check_tmp = 0;

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d\n", __func__, phy);

	halrf_efuse_get_info(rf, EFUSE_INFO_RF_RX_GAIN_K_A_2G_CCK, &tmp, 1);
	rx->rx_gain_offset[RF_PATH_A][0] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_offset[RF_PATH_B][0] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_efuse_get_info(rf, EFUSE_INFO_RF_RX_GAIN_K_A_2G_OFMD, &tmp, 1);
	rx->rx_gain_offset[RF_PATH_A][1] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_offset[RF_PATH_B][1] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_efuse_get_info(rf, EFUSE_INFO_RF_RX_GAIN_K_A_5GL, &tmp, 1);
	rx->rx_gain_offset[RF_PATH_A][2] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_offset[RF_PATH_B][2] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_efuse_get_info(rf, EFUSE_INFO_RF_RX_GAIN_K_A_5GM, &tmp, 1);
	rx->rx_gain_offset[RF_PATH_A][3] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_offset[RF_PATH_B][3] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_efuse_get_info(rf, EFUSE_INFO_RF_RX_GAIN_K_A_5GH, &tmp, 1);
	rx->rx_gain_offset[RF_PATH_A][4] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_offset[RF_PATH_B][4] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	for (i = 0; i < RX_GAIN_K_PATH_MAX_8852A; i++) {
		for (j = 0; j < RX_GAIN_K_OFFSET_MAX_8852A; j++) {
			RF_DBG(rf, DBG_RF_RXGAINK, "rx->rx_gain_offset[%d][%d]=0x%x\n", i, j, rx->rx_gain_offset[i][j]);
			if ((rx->rx_gain_offset[i][j] & 0xf) == 0xf)
				check_tmp++;
		}
	}

	RF_DBG(rf, DBG_RF_RXGAINK, "check_tmp=%d\n", check_tmp);

	if (check_tmp == RX_GAIN_K_PATH_MAX_8852A * RX_GAIN_K_OFFSET_MAX_8852A)
		rx->efuse_chenk = false;
	else
		rx->efuse_chenk = true;
}

void _halrf_get_hide_efuse_rx_gain_k_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;
	u8 i, j, tmp;
	u32 check_tmp = 0;

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d\n", __func__, phy);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_A_2G_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_A][0] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_A][0] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_A_5GL_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_A][1] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_A][1] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_A_5GM_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_A][2] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_A][2] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_A_5GH_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_A][3] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_A][3] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_B_2G_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_B][0] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_B][0] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_B_5GL_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_B][1] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_B][1] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_B_5GM_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_B][2] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_B][2] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	halrf_phy_efuse_get_info(rf, RX_GAIN_K_HIDE_EFUSE_B_5GH_8852A, 1, &tmp);
	rx->rx_gain_cs[RF_PATH_B][3] = _halrf_efuse_exchange_8852a(rf, tmp, LOW_MASK);
	rx->rx_gain_cg[RF_PATH_B][3] = _halrf_efuse_exchange_8852a(rf, tmp, HIGH_MASK);

	for (i = 0; i < RX_GAIN_K_PATH_MAX_8852A; i++) {
		for (j = 0; j < RX_GAIN_K_HIDE_OFFSET_MAX_8852A; j++) {
			RF_DBG(rf, DBG_RF_RXGAINK, "rx->rx_gain_cs[%d][%d]=0x%x   rx->rx_gain_cg[%d][%d]=0x%x\n",
				i, j, rx->rx_gain_cs[i][j], i, j, rx->rx_gain_cg[i][j]);
			if ((rx->rx_gain_cs[i][j] & 0xf) == 0xf)
				check_tmp++;
		}
	}

	RF_DBG(rf, DBG_RF_RXGAINK, "check_tmp=%d\n", check_tmp);

	if (check_tmp == RX_GAIN_K_PATH_MAX_8852A * RX_GAIN_K_HIDE_OFFSET_MAX_8852A)
		rx->hide_efuse_chenk = false;
	else
		rx->hide_efuse_chenk = true;
}

void _halrf_get_lna_error_frontend_loss_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;

	u32 lna_err_a_5g[RX_GAIN_K_PATH_MAX_8852A][7] =
		{{0x462c, 0x462c, 0x4630, 0x4634, 0x4634, 0x4638, 0x4638},
		{0x4700, 0x4700, 0x4704, 0x4708, 0x4708, 0x470c, 0x470c}};
	u32 lna_err_a_5g_mask[7] = {0x00000fc0, 0x3f000000, 0x0003f000,
		0x0000003f, 0x00fc0000, 0x00000fc0, 0x3f000000};

	u32 lna_err_a_2g[RX_GAIN_K_PATH_MAX_8852A][7] =
		{{0x462c, 0x4630, 0x4630, 0x4634, 0x4634, 0x4638, 0x463c},
		{0x4700, 0x4704, 0x4704, 0x4708, 0x4708, 0x470c, 0x4710}};
	u32 lna_err_a_2g_mask[7] = {0x0003f000, 0x0000003f, 0x00fc0000,
			0x00000fc0, 0x3f000000, 0x0003f000, 0x0000003f};

	u32 rx_frontend_loss[RX_GAIN_K_PATH_MAX_8852A] = {0x464c, 0x4720};
	u32 rx_rpl_bias_comp[RX_GAIN_K_PATH_MAX_8852A] = {0x494c, 0x494c};
	u32 rx_rssi_bias_comp[RX_GAIN_K_PATH_MAX_8852A] = {0x4964, 0x4964};

	u8 path, lna_idx;
	u32 tmp;

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d\n", __func__, phy);

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++) {
		for (lna_idx = 0; lna_idx < 7; lna_idx++) {

			tmp = halrf_rreg(rf, lna_err_a_5g[path][lna_idx], lna_err_a_5g_mask[lna_idx]);

			if (tmp & 0x20)
				rx->rx_lna_err_5g[path][lna_idx] = (s8)(tmp | 0xffffffc0);
			else
				rx->rx_lna_err_5g[path][lna_idx] = (s8)tmp;

			tmp = halrf_rreg(rf, lna_err_a_2g[path][lna_idx], lna_err_a_2g_mask[lna_idx]);

			if (tmp & 0x20)
				rx->rx_lna_err_2g[path][lna_idx] = (s8)(tmp | 0xffffffc0);
			else
				rx->rx_lna_err_2g[path][lna_idx] = (s8)tmp;
		}
	}

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++)
		for (lna_idx = 0; lna_idx < 7; lna_idx++)
			RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] rx_lna_err_5g 0x%x[%08X]=0x%08x\n",
				lna_err_a_5g[path][lna_idx],
				lna_err_a_5g_mask[lna_idx],
				rx->rx_lna_err_5g[path][lna_idx]);

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++)
		for (lna_idx = 0; lna_idx < 7; lna_idx++)
			RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] rx_lna_err_2g 0x%x[%08X]=0x%08x\n",
				lna_err_a_2g[path][lna_idx],
				lna_err_a_2g_mask[lna_idx],
				rx->rx_lna_err_2g[path][lna_idx]);

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++) {
		tmp = halrf_rreg(rf, rx_frontend_loss[path], 0x3e000000);
		if (tmp & 0x10)
				rx->rx_frontend_loss[path] = (s8)(tmp | 0xffffffe0);
			else
				rx->rx_frontend_loss[path] = (s8)tmp;
		tmp = halrf_rreg(rf, rx_rpl_bias_comp[path], 0xf8000000);
		if (tmp & 0x10)
				rx->rx_rpl_bias_comp[path] = (s8)(tmp | 0xffffffe0);
			else
				rx->rx_rpl_bias_comp[path] = (s8)tmp;

		tmp = halrf_rreg(rf, rx_rssi_bias_comp[path], 0x0fe00000);
		if (tmp & 0x40)
				rx->rx_rssi_bias_comp[path] = (s8)(tmp | 0xffffff80);
			else
				rx->rx_rssi_bias_comp[path] = (s8)tmp;
	}

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++)
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] rx_frontend_loss 0x%x[29:25]=0x%08x\n",
			rx_frontend_loss[path],
			rx->rx_frontend_loss[path]);

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++)
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] rx_rpl_bias_comp 0x%x[31:27]=0x%08x\n",
			rx_rpl_bias_comp[path],
			rx->rx_rpl_bias_comp[path]);

	for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++)
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] rx_rssi_bias_comp 0x%x[27:21]=0x%08x\n",
			rx_rssi_bias_comp[path],
			rx->rx_rssi_bias_comp[path]);

}


void _halrf_set_rx_gain_k_normal_efuse(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;

	u8 ch = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 band, i;
#if 0
	s32 reg_tmp;

	u32 front_end_loss[2] = {0x464c, 0x4720};
	u32 rpl_bias_comp[2] = {0x494c, 0x694c};
	u32 rssi_bias_comp[2] = {0x4964, 0x6964};
#endif

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d   channel=%d\n",
		__func__, phy, ch);

	if (ch >= 0 && ch <= 14)
		band = 1;
	else if (ch >= 36 && ch <= 64)
		band = 2;
	else if (ch >= 100 && ch <= 144)
		band = 3;
	else if (ch >= 149 && ch <= 177)
		band = 4;
	else
		band = 1;

	for (i = 0; i < 2; i++) {
		
		halrf_set_rx_gain_offset_for_rx_verify_8852a(rf, phy,
					rx->rx_gain_offset[i][band], i);
#if 0
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[29:25]=0x%x\n",
			front_end_loss[i], rx->rx_frontend_loss[i]);
		reg_tmp = rx->rx_frontend_loss[i] - rx->rx_gain_offset[i][band] * 4;
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx->rx_gain_offset[%d][%d](0x%x)(%d) * 4\n",
			reg_tmp, reg_tmp, rx->rx_frontend_loss[i], rx->rx_frontend_loss[i],
			i, band, rx->rx_gain_offset[i][band], rx->rx_gain_offset[i][band]);
		reg_tmp = (reg_tmp > 15) ? 15 : reg_tmp;
		reg_tmp = (reg_tmp < -16) ? -16 : reg_tmp;
		halrf_wreg(rf, front_end_loss[i], 0x3e000000, (reg_tmp & 0x1f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[29:25]=0x%x\n",
			front_end_loss[i], halrf_rreg(rf, front_end_loss[i], 0x3e000000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[31:27]=0x%x\n",
			rpl_bias_comp[i], rx->rx_rpl_bias_comp[i]);
		reg_tmp = rx->rx_rpl_bias_comp[i] - rx->rx_gain_offset[i][band] * 4;
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx->rx_gain_offset[%d][%d](0x%x)(%d) * 4\n",
			reg_tmp, reg_tmp, rx->rx_rpl_bias_comp[i], rx->rx_rpl_bias_comp[i],
			i, band, rx->rx_gain_offset[i][band], rx->rx_gain_offset[i][band]);
		reg_tmp = (reg_tmp > 15) ? 15 : reg_tmp;
		reg_tmp = (reg_tmp < -16) ? -16 : reg_tmp;
		halrf_wreg(rf, rpl_bias_comp[i], 0xf8000000, (reg_tmp & 0x1f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[31:27]=0x%x\n",
			 rpl_bias_comp[i], halrf_rreg(rf,  rpl_bias_comp[i], 0xf8000000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[27:21]=0x%x\n",
			rssi_bias_comp[i], rx->rx_rssi_bias_comp[i]);
		reg_tmp = rx->rx_rssi_bias_comp[i] - rx->rx_gain_offset[i][band] * 16;
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx->rx_gain_offset[%d][%d](0x%x)(%d) * 16\n",
			reg_tmp, reg_tmp, rx->rx_rssi_bias_comp[i], rx->rx_rssi_bias_comp[i],
			i, band, rx->rx_gain_offset[i][band], rx->rx_gain_offset[i][band]);
		reg_tmp = (reg_tmp > 63) ? 63 : reg_tmp;
		reg_tmp = (reg_tmp < -64) ? -64 : reg_tmp;
		halrf_wreg(rf, rssi_bias_comp[i], 0x0fe00000, (reg_tmp & 0x7f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[27:21]=0x%x\n",
			rssi_bias_comp[i], halrf_rreg(rf, rssi_bias_comp[i], 0x0fe00000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");
#endif
	}
}

void _halrf_set_rx_gain_k_hide_efuse(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;

	u8 ch = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 path, lna_idx, band;
	s32 reg_tmp;
	u32 lna_err_a_5g[RX_GAIN_K_PATH_MAX_8852A][7] =
		{{0x462c, 0x462c, 0x4630, 0x4634, 0x4634, 0x4638, 0x4638},
		{0x4700, 0x4700, 0x4704, 0x4708, 0x4708, 0x470c, 0x470c}};

	u32 lna_err_a_5g_mask[7] = {0x00000fc0, 0x3f000000, 0x0003f000,
		0x0000003f, 0x00fc0000, 0x00000fc0, 0x3f000000};

	u32 lna_err_a_2g[RX_GAIN_K_PATH_MAX_8852A][7] =
		{{0x462c, 0x4630, 0x4630, 0x4634, 0x4634, 0x4638, 0x463c},
		{0x4700, 0x4704, 0x4704, 0x4708, 0x4708, 0x470c, 0x4710}};

	u32 lna_err_a_2g_mask[7] = {0x0003f000, 0x0000003f, 0x00fc0000,
			0x00000fc0, 0x3f000000, 0x0003f000, 0x0000003f};

	/*2G, 5GL, 5GM, 5GH*/
	/*path, band, LNA*/
	s8 lna_err[2][4][7] =
		{{{-7, -8, -11, -18, -14, -17, -12},
		{-1, -8, -11, -4, -8, -6, -10},
		{-1, -6, -9, -2, -6, -4, -8},
		{-1, -4, -7, -0, -4, -2, -6}},

		{{-7, -8, -11, -18, -14, -17, -12},
		{-1, -8, -11, -4, -8, -6, -10},
		{-1, -6, -9, -2, -6, -4, -8},
		{-1, -4, -7, -0, -4, -2, -6}}};

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d   channel=%d\n",
		__func__, phy, ch);

	if (ch >= 0 && ch <= 14)
		band = 0;
	else if (ch >= 36 && ch <= 64)
		band = 1;
	else if (ch >= 100 && ch <= 144)
		band = 2;
	else if (ch >= 149 && ch <= 177)
		band = 3;
	else
		band = 0;

	if (ch >= 1 && ch <= 14) {
		for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++) {
			for (lna_idx = 0; lna_idx < 7; lna_idx++) {
				RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");
				RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] lna_err[%d][%d][%d]=0x%x\n",
					path, band, lna_idx, lna_err[path][band][lna_idx]);
				
				reg_tmp = lna_err[path][band][lna_idx] +
					rx->rx_gain_cs[path][band] * 4;

				RF_DBG(rf, DBG_RF_RXGAINK,
					"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) + rx->rx_gain_cg[%d][%d](0x%x) * 4\n",
					reg_tmp, lna_err[path][band][lna_idx], path, band, rx->rx_gain_cs[path][band]);
				reg_tmp = (reg_tmp > 63) ? 63 : reg_tmp;
				reg_tmp = (reg_tmp < -64) ? -64 : reg_tmp;
				
				halrf_wreg(rf, lna_err_a_2g[path][lna_idx], lna_err_a_2g_mask[lna_idx], (reg_tmp & 0x3f));
				RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] final result 0x%x[%X]=0x%x\n",
					lna_err_a_2g[path][lna_idx], lna_err_a_2g_mask[lna_idx],
					halrf_rreg(rf, lna_err_a_2g[path][lna_idx], lna_err_a_2g_mask[lna_idx]));
			}
		}
	} else {
		for (path = RF_PATH_A; path < RX_GAIN_K_PATH_MAX_8852A; path++) {
			for (lna_idx = 0; lna_idx < 7; lna_idx++) {

				RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");
				RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] lna_err[%d][%d][%d]=0x%x\n",
					path, band, lna_idx, lna_err[path][band][lna_idx]);

				reg_tmp = lna_err[path][band][lna_idx] +
					rx->rx_gain_cs[path][band] * 4;

				RF_DBG(rf, DBG_RF_RXGAINK,
					"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) + rx->rx_gain_cg[%d][%d](0x%x) * 4\n",
					reg_tmp, lna_err[path][band][lna_idx], path, band, rx->rx_gain_cs[path][band]);
				reg_tmp = (reg_tmp > 63) ? 63 : reg_tmp;
				reg_tmp = (reg_tmp < -64) ? -64 : reg_tmp;
				
				halrf_wreg(rf, lna_err_a_5g[path][lna_idx], lna_err_a_5g_mask[lna_idx], (reg_tmp & 0x3f));
				RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] final result 0x%x[%X]=0x%x\n",
					lna_err_a_5g[path][lna_idx], lna_err_a_5g_mask[lna_idx],
					halrf_rreg(rf, lna_err_a_5g[path][lna_idx], lna_err_a_5g_mask[lna_idx]));
			}
		}
	}
}

void _halrf_set_thermal_trim_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 thermal_a, thermal_b;

	RF_DBG(rf, DBG_RF_THER_TRIM, "======> %s   phy=%d\n", __func__, phy);

	if (!(rf->support_ability & HAL_RF_THER_TRIM)) {
		RF_DBG(rf, DBG_RF_THER_TRIM, "<== %s phy=%d support_ability=%d Ther Trim Off!!!\n",
			__func__, phy, rf->support_ability);
		return;
	}

	halrf_phy_efuse_get_info(rf, THERMAL_TRIM_HIDE_EFUSE_A_8852A, 1, &thermal_a);

	halrf_phy_efuse_get_info(rf, THERMAL_TRIM_HIDE_EFUSE_B_8852A, 1, &thermal_b);

	RF_DBG(rf, DBG_RF_THER_TRIM, "efuse Ther_A=0x%x Ther_B=0x%x\n",
		thermal_a, thermal_b);

	if (thermal_a == 0xff && thermal_b == 0xff) {
		RF_DBG(rf, DBG_RF_THER_TRIM, "Ther_A, Ther_B=0xff no PG Return!!!\n");
		return; 
	}

	thermal_a = thermal_a & 0x1f;
	thermal_a = ((thermal_a & 0x1) << 3) | (thermal_a >> 1);

	thermal_b = thermal_b & 0x1f;
	thermal_b = ((thermal_b & 0x1) << 3) | (thermal_b >> 1);

	RF_DBG(rf, DBG_RF_THER_TRIM, "After Exchange Ther_A=0x%x Ther_B=0x%x\n",
		thermal_a, thermal_b);

	halrf_wrf(rf, RF_PATH_A, 0x43, 0x000f0000, thermal_a);
	halrf_wrf(rf, RF_PATH_B, 0x43, 0x000f0000, thermal_b);
}

void _halrf_set_pa_bias_trim_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 pa_bias_a, pa_bias_b;
	u8 pa_bias_a_2g, pa_bias_b_2g, pa_bias_a_5g, pa_bias_b_5g;

	RF_DBG(rf, DBG_RF_PABIAS_TRIM, "======> %s   phy=%d\n", __func__, phy);

	if (!(rf->support_ability & HAL_RF_PABIAS_TRIM)) {
		RF_DBG(rf, DBG_RF_PABIAS_TRIM, "<== %s phy=%d support_ability=%d PA Bias K Off!!!\n",
			__func__, phy, rf->support_ability);
		return;
	}

	halrf_phy_efuse_get_info(rf, PABIAS_TRIM_HIDE_EFUSE_A_8852A, 1, &pa_bias_a);

	halrf_phy_efuse_get_info(rf, PABIAS_TRIM_HIDE_EFUSE_B_8852A, 1, &pa_bias_b);

	RF_DBG(rf, DBG_RF_PABIAS_TRIM, "efuse PA_Bias_A=0x%x PA_Bias_B=0x%x\n",
		pa_bias_a, pa_bias_b);

	if (pa_bias_a == 0xff && pa_bias_b == 0xff) {
		RF_DBG(rf, DBG_RF_PABIAS_TRIM, "PA_Bias_A, PA_Bias_B=0xff no PG Return!!!\n");
		return; 
	}

	pa_bias_a_2g = pa_bias_a & 0xf;
	pa_bias_a_5g = (pa_bias_a & 0xf0) >> 4;

	pa_bias_b_2g = pa_bias_b & 0xf;
	pa_bias_b_5g = (pa_bias_b & 0xf0) >> 4;

	RF_DBG(rf, DBG_RF_PABIAS_TRIM, "After Calculate PA_Bias_A_2G=0x%x PA_Bias_A_5G=0x%x\n",
		pa_bias_a_2g, pa_bias_a_5g);

	RF_DBG(rf, DBG_RF_PABIAS_TRIM, "After Calculate PA_Bias_B_2G=0x%x PA_Bias_B_5G=0x%x\n",
		pa_bias_b_2g, pa_bias_b_5g);

	halrf_wrf(rf, RF_PATH_A, 0x60, 0x0000f000, pa_bias_a_2g);
	halrf_wrf(rf, RF_PATH_A, 0x60, 0x000f0000, pa_bias_a_5g);

	halrf_wrf(rf, RF_PATH_B, 0x60, 0x0000f000, pa_bias_b_2g);
	halrf_wrf(rf, RF_PATH_B, 0x60, 0x000f0000, pa_bias_b_5g);
}

void _halrf_get_tssi_trim_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi = &rf->tssi;
	u8 i, j , check_tmp = 0;

	RF_DBG(rf, DBG_RF_TSSI_TRIM, "======> %s   phy=%d\n", __func__, phy);

	if (!(rf->support_ability & HAL_RF_TSSI_TRIM)) {
		RF_DBG(rf, DBG_RF_TSSI_TRIM, "<== %s phy=%d support_ability=%d TSSI Trim Off!!!\n",
			__func__, phy, rf->support_ability);
		return;
	}

	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_2GL_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][0]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_2GH_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][1]);

	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GL1_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][2]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GL2_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][3]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GM1_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][4]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GM2_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][5]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GH1_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][6]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GH2_A_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_A][7]);

	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_2GL_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][0]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_2GH_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][1]);

	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GL1_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][2]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GL2_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][3]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GM1_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][4]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GM2_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][5]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GH1_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][6]);
	halrf_phy_efuse_get_info(rf, TSSI_TRIM_HIDE_EFUSE_5GH2_B_8852A, 1,
		(u8 *)&tssi->tssi_trim[RF_PATH_B][7]);

	for (i = 0; i < 2; i++) {
		for (j = 0; j < TSSI_HIDE_EFUSE_NUM; j++) {
			RF_DBG(rf, DBG_RF_TSSI_TRIM, "tssi->tssi_trim[%d][%d]=0x%x\n", i, j, tssi->tssi_trim[i][j]);
			if ((tssi->tssi_trim[i][j] & 0xff) == 0xff)
				check_tmp++;
		}
	}

	RF_DBG(rf, DBG_RF_TSSI_TRIM, "check_tmp=%d\n", check_tmp);

	if (check_tmp == 2 * TSSI_HIDE_EFUSE_NUM) {
		for (i = 0; i < 2; i++) {
			for (j = 0; j < TSSI_HIDE_EFUSE_NUM; j++)
				tssi->tssi_trim[i][j] = 0;
		}

		RF_DBG(rf, DBG_RF_TSSI_TRIM, "TSSI Trim no PG tssi->tssi_trim=0x0\n");
	}

}


void halrf_get_efuse_rx_gain_k_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	_halrf_get_normal_efuse_rx_gain_k_8852a(rf, phy);
	_halrf_get_hide_efuse_rx_gain_k_8852a(rf, phy);
	_halrf_get_lna_error_frontend_loss_8852a(rf, phy);
}

void halrf_get_efuse_trim_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	_halrf_set_thermal_trim_8852a(rf, phy);
	_halrf_set_pa_bias_trim_8852a(rf, phy);
	_halrf_get_tssi_trim_8852a(rf, phy);
}

void halrf_do_rx_gain_k_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s   phy=%d\n", __func__, phy);

	if (rx->efuse_chenk == false)
		RF_DBG(rf, DBG_RF_RXGAINK, "Read RX Gain Efuse All are 0xff don't offset !!!\n");
	else {
		RF_DBG(rf, DBG_RF_RXGAINK, "Read RX Gain Efuse OK\n");
		_halrf_set_rx_gain_k_normal_efuse(rf, phy);
	}

	if (rx->hide_efuse_chenk == false)
		RF_DBG(rf, DBG_RF_RXGAINK, "Read RX Gain Hide Efuse All are 0xff don't offset !!!\n");
	else {
		RF_DBG(rf, DBG_RF_RXGAINK, "Read RX Gain Hide Efuse OK\n");
		_halrf_set_rx_gain_k_hide_efuse(rf, phy);

	}
}

void halrf_set_rx_gain_offset_for_rx_verify_8852a(struct rf_info *rf,
					enum phl_phy_idx phy,
					s8 rx_gain_offset, u8 path)
{
	struct halrf_rx_gain_k_info *rx = &rf->rx_gain_k;
	s32 reg_tmp;

	u32 front_end_loss[2] = {0x464c, 0x4720};
	/*u32 rpl_bias_comp[2] = {0x494c, 0x694c};*/
	/*u32 rssi_bias_comp[2] = {0x4964, 0x6964};*/
	u32 rpl_bias_comp[2] = {0x494c, 0x494c};
	u32 rssi_bias_comp[2] = {0x4964, 0x4964};

	RF_DBG(rf, DBG_RF_RXGAINK, "======> %s  phy=%d  rx_gain_offset=%d  path=%d\n",
		__func__, phy, rx_gain_offset, path);

	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[29:25]=0x%x\n",
		front_end_loss[path], rx->rx_frontend_loss[path]);
	reg_tmp = rx->rx_frontend_loss[path] - rx_gain_offset * 4;
	RF_DBG(rf, DBG_RF_RXGAINK,
		"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx_gain_offset(0x%x)(%d) * 4\n",
		reg_tmp, reg_tmp, rx->rx_frontend_loss[path], rx->rx_frontend_loss[path],
		rx_gain_offset, rx_gain_offset);
	reg_tmp = (reg_tmp > 15) ? 15 : reg_tmp;
	reg_tmp = (reg_tmp < 0) ? 0 : reg_tmp;
	halrf_wreg(rf, front_end_loss[path], 0x3e000000, (reg_tmp & 0x1f));
	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[29:25]=0x%x\n",
		front_end_loss[path], halrf_rreg(rf, front_end_loss[path], 0x3e000000));
	RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[31:27]=0x%x\n",
		rpl_bias_comp[path], rx->rx_rpl_bias_comp[path]);
	reg_tmp = rx->rx_rpl_bias_comp[path] - rx_gain_offset * 4;
	RF_DBG(rf, DBG_RF_RXGAINK,
		"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx_gain_offset(0x%x)(%d) * 4\n",
		reg_tmp, reg_tmp, rx->rx_rpl_bias_comp[path], rx->rx_rpl_bias_comp[path],
		rx_gain_offset, rx_gain_offset);
	reg_tmp = (reg_tmp > 15) ? 15 : reg_tmp;
	reg_tmp = (reg_tmp < -16) ? -16 : reg_tmp;
	halrf_wreg(rf, rpl_bias_comp[path], 0xf8000000, (reg_tmp & 0x1f));
	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[31:27]=0x%x\n",
		 rpl_bias_comp[path], halrf_rreg(rf,  rpl_bias_comp[path], 0xf8000000));
	RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[27:21]=0x%x\n",
		rssi_bias_comp[path], rx->rx_rssi_bias_comp[path]);
	reg_tmp = rx->rx_rssi_bias_comp[path] - rx_gain_offset * 16;
	RF_DBG(rf, DBG_RF_RXGAINK,
		"[RXGAINK] reg_tmp(0x%x)(%d) = org_reg_tmp(0x%x)(%d) - rx_gain_offset(0x%x)(%d) * 16\n",
		reg_tmp, reg_tmp, rx->rx_rssi_bias_comp[path], rx->rx_rssi_bias_comp[path],
		rx_gain_offset, rx_gain_offset);
	reg_tmp = (reg_tmp > 63) ? 63 : reg_tmp;
	reg_tmp = (reg_tmp < -64) ? -64 : reg_tmp;
	halrf_wreg(rf, rssi_bias_comp[path], 0x0fe00000, (reg_tmp & 0x7f));
	RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x%x[27:21]=0x%x\n",
		rssi_bias_comp[path], halrf_rreg(rf, rssi_bias_comp[path], 0x0fe00000));
	RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

#if 0
	if (path == RF_PATH_A) {
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x1850[31:27]=0x%x\n",
			rx->rx_rpl_bias_comp[RF_PATH_A]);
		reg_tmp = rx->rx_rpl_bias_comp[RF_PATH_A] - (rx_gain_offset * 4);
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) - rx_gain_offset(0x%x) * 4\n",
			reg_tmp, rx->rx_rpl_bias_comp[RF_PATH_A], rx_gain_offset);
		reg_tmp = (reg_tmp > 127) ? 127 : reg_tmp;
		reg_tmp = (reg_tmp < -128) ? -128 : reg_tmp;
		halrf_wreg(rf, 0x1850, 0xf8000000, (reg_tmp & 0x1f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x1850[31:27]=0x%x\n",
			halrf_rreg(rf, 0x1850, 0xf8000000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==========================\n");


		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x1868[27:21]=0x%x\n",
		rx->rx_rssi_bias_comp[RF_PATH_A]);
		reg_tmp = rx->rx_rssi_bias_comp[RF_PATH_A] - rx_gain_offset * 16;
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) - rx_gain_offset(0x%x) * 16\n",
			reg_tmp, rx->rx_rssi_bias_comp[RF_PATH_A], rx_gain_offset);
		reg_tmp = (reg_tmp > 127) ? 127 : reg_tmp;
		reg_tmp = (reg_tmp < -128) ? -128 : reg_tmp;
		halrf_wreg(rf, 0x1868, 0x0fe00000, (reg_tmp & 0x7f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x1868[27:21]=0x%x\n",
			halrf_rreg(rf, 0x1868, 0x0fe00000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");
	} else {
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x5850[31:27]=0x%x\n",
			rx->rx_rpl_bias_comp[RF_PATH_B]);
		reg_tmp = rx->rx_rpl_bias_comp[RF_PATH_B] - (rx_gain_offset * 4);
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) - rx_gain_offset(0x%x) * 4\n",
			reg_tmp, rx->rx_rpl_bias_comp[RF_PATH_B], rx_gain_offset);
		reg_tmp = (reg_tmp > 127) ? 127 : reg_tmp;
		reg_tmp = (reg_tmp < -128) ? -128 : reg_tmp;
		halrf_wreg(rf, 0x5850, 0xf8000000, (reg_tmp & 0x1f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x5850[31:27]=0x%x\n",
			halrf_rreg(rf, 0x5850, 0xf8000000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");

		
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x5868[27:21]=0x%x\n",
			rx->rx_rssi_bias_comp[RF_PATH_B]);
		reg_tmp = rx->rx_rssi_bias_comp[RF_PATH_B] - rx_gain_offset * 16;
		RF_DBG(rf, DBG_RF_RXGAINK,
			"[RXGAINK] reg_tmp(0x%x) = org_reg_tmp(0x%x) - rx_gain_offset(0x%x) * 16\n",
			reg_tmp, rx->rx_rssi_bias_comp[RF_PATH_B], rx_gain_offset);
		reg_tmp = (reg_tmp > 127) ? 127 : reg_tmp;
		reg_tmp = (reg_tmp < -128) ? -128 : reg_tmp;
		halrf_wreg(rf, 0x5868, 0x0fe00000, (reg_tmp & 0x7f));
		RF_DBG(rf, DBG_RF_RXGAINK, "[RXGAINK] 0x5868[27:21]=0x%x\n",
			halrf_rreg(rf, 0x5868, 0x0fe00000));
		RF_DBG(rf, DBG_RF_RXGAINK, "==================================\n");
	}
#endif
}

#endif	/*RF_8852A_SUPPORT*/
