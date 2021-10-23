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

void _halrf_tssi_rf_setting_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	if (channel >= 1 && channel <= 14)
		halrf_wrf_fw(rf, path, 0x7f, 0x00002, 0x1);
	else
		halrf_wrf_fw(rf, path, 0x7f, 0x00100, 0x1);
}

void _halrf_tssi_set_sys_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	halrf_wreg_fw(rf, 0x800, 0xf0000000, 0x4);
	halrf_wreg_fw(rf, 0x4ca8, 0x00000001, 0x1);
	halrf_wreg_fw(rf, 0x4ca8, 0x0000000e, 0x2);
	halrf_wreg_fw(rf, 0x4da8, 0x00000001, 0x1);
	halrf_wreg_fw(rf, 0x4da8, 0x0000000e, 0x2);
	halrf_wreg_fw(rf, 0x0b04, 0x0000ffff, 0x1919);

	if (channel >= 1 && channel <= 14) {
		halrf_wreg_fw(rf, 0x0b18, MASKDWORD, 0x20042004);
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "Set 2G 0x0b18=0x%8x\n",
			halrf_rreg(rf, 0x0b18, MASKDWORD ));
	} else {
		halrf_wreg_fw(rf, 0x0b18, MASKDWORD, 0x20022002);
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "Set 5G 0x0b18=0x%8x\n",
			halrf_rreg(rf, 0x0b18, MASKDWORD ));
	}
}

void _halrf_tssi_ini_txpwr_ctrl_bb_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c00, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7c00, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7c00, 0x003f0000, 0x7f);
		halrf_wreg_fw(rf, 0x7c00, 0x0fc00000, 0x80);
		halrf_wreg_fw(rf, 0x7c00, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x7c00, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7c00, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x7c04, 0x000001ff, 0x040);
		halrf_wreg_fw(rf, 0x7c04, 0x07fc0000, 0x12c);
		halrf_wreg_fw(rf, 0x7c04, 0xf8000000, 0x00);
		halrf_wreg_fw(rf, 0x7c08, 0x000001ff, 0x040);
		halrf_wreg_fw(rf, 0x7c08, 0x07fc0000, 0x12c);
		halrf_wreg_fw(rf, 0x7c0c, 0x0000007f, 0x40);
		halrf_wreg_fw(rf, 0x7c0c, 0x00007f00, 0x40);
		halrf_wreg_fw(rf, 0x7c0c, 0x00008000, 0x0);
		halrf_wreg_fw(rf, 0x7c0c, 0x0fff0000, 0x000);
		halrf_wreg_fw(rf, 0x7c10, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7c10, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x7c10, 0x0000fc00, 0x00);
		halrf_wreg_fw(rf, 0x7c10, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x7c10, 0x00fe0000, 0x00);
		halrf_wreg_fw(rf, 0x7c10, 0x01000000, 0x1);
		halrf_wreg_fw(rf, 0x7c10, 0x06000000, 0x0);
		halrf_wreg_fw(rf, 0x7c10, 0x38000000, 0x3);
		halrf_wreg_fw(rf, 0x7c10, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x7c10, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7c14, 0x00000c00, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x7c14, 0x00002000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x00004000, 0x1);
		halrf_wreg_fw(rf, 0x7c14, 0x00038000, 0x5);
		halrf_wreg_fw(rf, 0x7c14, 0x003c0000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x01c00000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0xe0000000, 0x0);
		halrf_wreg_fw(rf, 0x7c18, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x7c18, 0x0001ff00, 0x018);
		halrf_wreg_fw(rf, 0x7c18, 0x03fe0000, 0x016);
		halrf_wreg_fw(rf, 0x7c18, 0xfc000000, 0x00);
		halrf_wreg_fw(rf, 0x7c1c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x7c1c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7c1c, 0x00100000, 0x0);
		halrf_wreg_fw(rf, 0x7c1c, 0x01e00000, 0xe);
		halrf_wreg_fw(rf, 0x7c1c, 0x1e000000, 0xe);
		halrf_wreg_fw(rf, 0x7c1c, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7c20, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7c20, 0x0000f000, 0xf);
		halrf_wreg_fw(rf, 0x7c20, 0x001f0000, 0x00);
		halrf_wreg_fw(rf, 0x7c20, 0xffe00000, 0x000);
		halrf_wreg_fw(rf, 0x7c24, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c24, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c28, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c28, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c28, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c2c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c2c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c30, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c30, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c30, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c34, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c34, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c38, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c38, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c38, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c3c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c3c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c40, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c40, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c40, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c44, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c44, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c48, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c48, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c48, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c4c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c4c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c50, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c50, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c50, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c54, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c54, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c58, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c58, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c58, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c5c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c5c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c60, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c60, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c60, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7c60, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7c64, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7c64, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7c64, 0x03f00000, 0x00);
		halrf_wreg_fw(rf, 0x7c64, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x7c98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7c9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ca0, 0x000000ff, 0xfd);
		halrf_wreg_fw(rf, 0x7ca0, 0x0000ff00, 0xe5);
		halrf_wreg_fw(rf, 0x7ca0, 0x00ff0000, 0xcd);
		halrf_wreg_fw(rf, 0x7ca0, 0xff000000, 0xb5);
		halrf_wreg_fw(rf, 0x7ca4, 0x000000ff, 0x16);
		halrf_wreg_fw(rf, 0x7ca4, 0x07ffff00, 0x000);
		halrf_wreg_fw(rf, 0x7ca4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7cb0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7cb4, 0x0000001f, 0x00);
		halrf_wreg_fw(rf, 0x7cb4, 0x00000020, 0x0);
		halrf_wreg_fw(rf, 0x7cb4, 0x000001c0, 0x0);
		halrf_wreg_fw(rf, 0x7cb4, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x7cb4, 0x0000f000, 0x2);
		halrf_wreg_fw(rf, 0x7cb4, 0x00ff0000, 0x00);
		halrf_wreg_fw(rf, 0x7cb4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x7cb8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x7cb8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7cb8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x7cb8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x7cbc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7cbc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7cbc, 0x00030000, 0x3);
		halrf_wreg_fw(rf, 0x7cbc, 0x000c0000, 0x1);
		halrf_wreg_fw(rf, 0x7cbc, 0x00300000, 0x3);
		halrf_wreg_fw(rf, 0x7cbc, 0x00c00000, 0x3);
		halrf_wreg_fw(rf, 0x7cbc, 0x07000000, 0x7);
		halrf_wreg_fw(rf, 0x7cc0, 0x00fe0000, 0x7f);
		halrf_wreg_fw(rf, 0x7cc0, 0xff000000, 0x00);
		halrf_wreg_fw(rf, 0x7cc4, 0x0003ffff, 0x3ffff);
		halrf_wreg_fw(rf, 0x7cc4, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7cc4, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x7cc8, 0x00ffffff, 0x000000);
		halrf_wreg_fw(rf, 0x7cc8, 0xf0000000, 0x0);
		halrf_wreg_fw(rf, 0x7ccc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7cd0, 0x00001fff, 0x0101);
		halrf_wreg_fw(rf, 0x7cd0, 0x0001e000, 0x4);
		halrf_wreg_fw(rf, 0x7cd0, 0x03fe0000, 0x100);
		halrf_wreg_fw(rf, 0x7cd0, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x7cd4, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x7cd4, 0x0003fe00, 0x0ff);
		halrf_wreg_fw(rf, 0x7cd4, 0x07fc0000, 0x100);
		halrf_wreg_fw(rf, 0x7cd8, 0x000001ff, 0x16c);
		halrf_wreg_fw(rf, 0x7cd8, 0x0003fe00, 0x5c);
		halrf_wreg_fw(rf, 0x7cd8, 0x000c0000, 0x2);
		halrf_wreg_fw(rf, 0x7cd8, 0xfff00000, 0x800);
		halrf_wreg_fw(rf, 0x7cdc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7cdc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7cdc, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x7cdc, 0x3ff00000, 0x000);
		halrf_wreg_fw(rf, 0x7cdc, 0xc0000000, 0x2);
		halrf_wreg_fw(rf, 0x7cf0, 0x000001ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7cf0, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7cf4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7cf4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x7cf4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7cf4, 0x000ffc00, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7d00, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7d00, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7d00, 0x003f0000, 0x7f);
		halrf_wreg_fw(rf, 0x7d00, 0x0fc00000, 0x80);
		halrf_wreg_fw(rf, 0x7d00, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x7d00, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7d00, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x7d04, 0x000001ff, 0x040);
		halrf_wreg_fw(rf, 0x7d04, 0x07fc0000, 0x12c);
		halrf_wreg_fw(rf, 0x7d04, 0xf8000000, 0x00);
		halrf_wreg_fw(rf, 0x7d08, 0x000001ff, 0x040);
		halrf_wreg_fw(rf, 0x7d08, 0x07fc0000, 0x12c);
		halrf_wreg_fw(rf, 0x7d0c, 0x0000007f, 0x40);
		halrf_wreg_fw(rf, 0x7d0c, 0x00007f00, 0x40);
		halrf_wreg_fw(rf, 0x7d0c, 0x00008000, 0x0);
		halrf_wreg_fw(rf, 0x7d0c, 0x0fff0000, 0x000);
		halrf_wreg_fw(rf, 0x7d10, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7d10, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x7d10, 0x0000fc00, 0x00);
		halrf_wreg_fw(rf, 0x7d10, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x7d10, 0x00fe0000, 0x00);
		halrf_wreg_fw(rf, 0x7d10, 0x01000000, 0x1);
		halrf_wreg_fw(rf, 0x7d10, 0x06000000, 0x0);
		halrf_wreg_fw(rf, 0x7d10, 0x38000000, 0x3);
		halrf_wreg_fw(rf, 0x7d10, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x7d10, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7d14, 0x00000c00, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x7d14, 0x00002000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x00004000, 0x1);
		halrf_wreg_fw(rf, 0x7d14, 0x00038000, 0x5);
		halrf_wreg_fw(rf, 0x7d14, 0x003c0000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x01c00000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0xe0000000, 0x0);
		halrf_wreg_fw(rf, 0x7d18, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x7d18, 0x0001ff00, 0x018);
		halrf_wreg_fw(rf, 0x7d18, 0x03fe0000, 0x016);
		halrf_wreg_fw(rf, 0x7d18, 0xfc000000, 0x00);
		halrf_wreg_fw(rf, 0x7d1c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x7d1c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7d1c, 0x00100000, 0x0);
		halrf_wreg_fw(rf, 0x7d1c, 0x01e00000, 0xe);
		halrf_wreg_fw(rf, 0x7d1c, 0x1e000000, 0xe);
		halrf_wreg_fw(rf, 0x7d1c, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7d20, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7d20, 0x0000f000, 0xf);
		halrf_wreg_fw(rf, 0x7d20, 0x001f0000, 0x00);
		halrf_wreg_fw(rf, 0x7d20, 0xffe00000, 0x000);
		halrf_wreg_fw(rf, 0x7d24, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d24, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d28, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d28, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d28, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d2c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d2c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d30, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d30, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d30, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d34, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d34, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d38, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d38, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d38, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d3c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d3c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d40, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d40, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d40, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d44, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d44, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d48, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d48, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d48, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d4c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d4c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d50, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d50, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d50, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d54, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d54, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d58, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d58, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d58, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d5c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d5c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d60, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d60, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d60, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7d60, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7d64, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7d64, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7d64, 0x03f00000, 0x00);
		halrf_wreg_fw(rf, 0x7d64, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x7d98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7d9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7da0, 0x000000ff, 0xfd);
		halrf_wreg_fw(rf, 0x7da0, 0x0000ff00, 0xe5);
		halrf_wreg_fw(rf, 0x7da0, 0x00ff0000, 0xcd);
		halrf_wreg_fw(rf, 0x7da0, 0xff000000, 0xb5);
		halrf_wreg_fw(rf, 0x7da4, 0x000000ff, 0x16);
		halrf_wreg_fw(rf, 0x7da4, 0x07ffff00, 0x000);
		halrf_wreg_fw(rf, 0x7da4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7db0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7db4, 0x0000001f, 0x00);
		halrf_wreg_fw(rf, 0x7db4, 0x00000020, 0x0);
		halrf_wreg_fw(rf, 0x7db4, 0x000001c0, 0x0);
		halrf_wreg_fw(rf, 0x7db4, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x7db4, 0x0000f000, 0x2);
		halrf_wreg_fw(rf, 0x7db4, 0x00ff0000, 0x00);
		halrf_wreg_fw(rf, 0x7db4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x7db8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x7db8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7db8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x7db8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x7dbc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7dbc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7dbc, 0x00030000, 0x3);
		halrf_wreg_fw(rf, 0x7dbc, 0x000c0000, 0x1);
		halrf_wreg_fw(rf, 0x7dbc, 0x00300000, 0x3);
		halrf_wreg_fw(rf, 0x7dbc, 0x00c00000, 0x3);
		halrf_wreg_fw(rf, 0x7dbc, 0x07000000, 0x7);
		halrf_wreg_fw(rf, 0x7dc0, 0x00fe0000, 0x7f);
		halrf_wreg_fw(rf, 0x7dc0, 0xff000000, 0x00);
		halrf_wreg_fw(rf, 0x7dc4, 0x0003ffff, 0x3ffff);
		halrf_wreg_fw(rf, 0x7dc4, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7dc4, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x7dc8, 0x00ffffff, 0x000000);
		halrf_wreg_fw(rf, 0x7dc8, 0xf0000000, 0x0);
		halrf_wreg_fw(rf, 0x7dcc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7dd0, 0x00001fff, 0x0101);
		halrf_wreg_fw(rf, 0x7dd0, 0x0001e000, 0x4);
		halrf_wreg_fw(rf, 0x7dd0, 0x03fe0000, 0x100);
		halrf_wreg_fw(rf, 0x7dd0, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x7dd4, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x7dd4, 0x0003fe00, 0x0ff);
		halrf_wreg_fw(rf, 0x7dd4, 0x07fc0000, 0x100);
		halrf_wreg_fw(rf, 0x7dd8, 0x000001ff, 0x16c);
		halrf_wreg_fw(rf, 0x7dd8, 0x0003fe00, 0x05c);
		halrf_wreg_fw(rf, 0x7dd8, 0x000c0000, 0x2);
		halrf_wreg_fw(rf, 0x7dd8, 0xfff00000, 0x800);
		halrf_wreg_fw(rf, 0x7ddc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7ddc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7ddc, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x7ddc, 0x3ff00000, 0x000);
		halrf_wreg_fw(rf, 0x7ddc, 0xc0000000, 0x2);
		halrf_wreg_fw(rf, 0x7df0, 0x000001ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7df0, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7df4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7df4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x7df4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7df4, 0x000ffc00, 0x000);
	}

}

void _halrf_tssi_set_dck_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u32 r_tssi_adc[TSSI_PATH_MAX_8852A] = {0x7c0c, 0x7d0c};
	u32 r_tssi_dck[TSSI_PATH_MAX_8852A] = {0x7c14, 0x7d14};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, r_tssi_adc[path], 0x0fff0000, 0x000);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00001000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00002000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00004000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00038000, 0x5);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x003c0000, 0x2);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x18000000, 0x0);
}

void _halrf_tssi_set_bbgain_split_bcar_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u32 secret_test_mode[TSSI_PATH_MAX_8852A] = {0x7cd4, 0x7dd4};
	u32 r_tx_gain_scale[TSSI_PATH_MAX_8852A] = {0x7cf0, 0x7df0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, secret_test_mode[path], 0xf0000000, 0x7);
	halrf_wreg_fw(rf, r_tx_gain_scale[path], 0x000c0000, 0x1);
	halrf_wreg_fw(rf, r_tx_gain_scale[path], 0xfff00000, 0x400);
}

void _halrf_tssi_set_tmeter_tbl_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_pwr_track_info *pwr_trk = &rf->pwr_track;
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	struct rtw_hal_com_t *hal = rf->hal_com;

	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	u8 i, thermal = 0xff, j;
	//u8 rate = phydm_get_tx_rate(dm);
	u32 thermal_offset_tmp = 0;
	s8 thermal_offset[64] = {0};
	u8 thermal_up_a[DELTA_SWINGIDX_SIZE] = {0}, thermal_down_a[DELTA_SWINGIDX_SIZE] = {0};
	u8 thermal_up_b[DELTA_SWINGIDX_SIZE] = {0}, thermal_down_b[DELTA_SWINGIDX_SIZE] = {0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (tssi_info->tssi_type == TSSI_CAL) {
		halrf_wreg_fw(rf, 0x7c10, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x7c10, 0x01000000, 0x1);

		halrf_wreg_fw(rf, 0x7d10, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x7d10, 0x01000000, 0x1);

		halrf_wreg_fw(rf, 0x7c10, 0x0000fc00, 32);
		halrf_wreg_fw(rf, 0x7d10, 0x0000fc00, 32);

		for (i = 0; i < 64; i = i + 4) {
			thermal_offset_tmp = (thermal_offset[i] & 0xff) |
					(thermal_offset[i + 1] & 0xff) << 8 |
					(thermal_offset[i + 2] & 0xff) << 16 |
					(thermal_offset[i + 3] & 0xff) << 24;

			halrf_wreg_fw(rf, (0x600 + i), MASKDWORD, thermal_offset_tmp);
			halrf_wreg_fw(rf, (0x700 + i), MASKDWORD, thermal_offset_tmp);
			
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI] write addr:0x%x value=0x%08x\n",
			       (0x600 + i), thermal_offset_tmp);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI] write addr:0x%x value=0x%08x\n",
			       (0x700 + i), thermal_offset_tmp);
		}

		halrf_wreg_fw(rf, 0x7c64, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7c64, BIT(26), 0x0);

		halrf_wreg_fw(rf, 0x7d64, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7d64, BIT(26), 0x0);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s TSSI Calibration Mode return !!!\n", __func__);

		return;
	}

	if (channel >= 1 && channel <= 14) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_2ga_p, sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_2ga_n, sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_2gb_p, sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_2gb_n, sizeof(thermal_down_b));
	} else if (channel >= 36 && channel <= 64) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[0], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[0], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[0], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[0], sizeof(thermal_down_b));
	} else if (channel >= 100 && channel <= 144) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[1], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[1], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[1], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[1], sizeof(thermal_down_b));
	} else if (channel >= 149 && channel <= 177) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[2], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[2], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[2], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[2], sizeof(thermal_down_b));
	}

	/*path s0*/
	if (path == RF_PATH_A) {
		hal_mem_set(hal, thermal_offset, 0, sizeof(thermal_offset));
		halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_A, &thermal, 1);

		/*thermal = 32;*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] channel=%d thermal_pahtA=0x%x tssi_info->tssi_type=%d\n",
		       channel, thermal, tssi_info->tssi_type);

		if (thermal == 0xff) {
			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x600 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x600 + i), thermal_offset_tmp);
			}

			halrf_wreg_fw(rf, 0x7c10, 0x00010000, 0x1);
			halrf_wreg_fw(rf, 0x7c10, 0x01000000, 0x1);

			halrf_wreg_fw(rf, 0x7c10, 0x0000fc00, 32);
		}

		tssi_info->thermal[RF_PATH_A] = thermal;

		if (thermal != 0xff) {
			halrf_wreg_fw(rf, 0x7c10, 0x0000fc00, (thermal & 0x3f));

			i = 0;
			for (j = 0; j < 32; j++) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = -1 * thermal_down_a[i++];
				else
					thermal_offset[j] = -1 * thermal_down_a[DELTA_SWINGIDX_SIZE - 1];
			}

			i = 1;
			for (j = 63; j >= 32; j--) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = thermal_up_a[i++];
				else
					thermal_offset[j] = thermal_up_a[DELTA_SWINGIDX_SIZE - 1];
			}

			for (i = 0; i < 64; i = i + 4) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] thermal_offset[%.2d]=%.2x %.2x %.2x %.2x\n",
				       i, thermal_offset[i + 3] & 0xff, thermal_offset[i + 2] & 0xff,
				       thermal_offset[i + 1] & 0xff, thermal_offset[i] & 0xff);
			}

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x600 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x600 + i), thermal_offset_tmp);
			}

			halrf_wreg_fw(rf, 0x7c10, 0x00010000, 0x0);
			halrf_wreg_fw(rf, 0x7c10, 0x01000000, 0x1);
		}
		halrf_wreg_fw(rf, 0x7c64, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7c64, BIT(26), 0x0);
	} else {
		/*path s1*/
		hal_mem_set(hal, thermal_offset, 0, sizeof(thermal_offset));
		halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_B, &thermal, 1);

		/*thermal = 32;*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] channel=%d thermal_pahtB=0x%x tssi_info->tssi_type=%d\n",
		       channel, thermal, tssi_info->tssi_type);

		if (thermal == 0xff) {
			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x700 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x700 + i), thermal_offset_tmp);
			}

			halrf_wreg_fw(rf, 0x7d10, 0x00010000, 0x1);
			halrf_wreg_fw(rf, 0x7d10, 0x01000000, 0x1);

			halrf_wreg_fw(rf, 0x7d10, 0x0000fc00, 32);
		}

		tssi_info->thermal[RF_PATH_B] = thermal;

		if (thermal != 0xff) {
			halrf_wreg_fw(rf, 0x7d10, 0x0000fc00, (thermal & 0x3f));

			i = 0;
			for (j = 0; j < 32; j++) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = -1 * thermal_down_b[i++];
				else
					thermal_offset[j] = -1 * thermal_down_b[DELTA_SWINGIDX_SIZE - 1];
			}

			i = 1;
			for (j = 63; j >= 32; j--) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = thermal_up_b[i++];
				else
					thermal_offset[j] = thermal_up_b[DELTA_SWINGIDX_SIZE - 1];
			}

			for (i = 0; i < 64; i = i + 4) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] thermal_offset[%.2d]=%.2x %.2x %.2x %.2x\n",
				       i, thermal_offset[i + 3] & 0xff, thermal_offset[i + 2] & 0xff,
				       thermal_offset[i + 1] & 0xff, thermal_offset[i] & 0xff);
			}

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x700 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x700 + i), thermal_offset_tmp);
			}

			halrf_wreg_fw(rf, 0x7d10, 0x00010000, 0x0);
			halrf_wreg_fw(rf, 0x7d10, 0x01000000, 0x1);
		}
		halrf_wreg_fw(rf, 0x7d64, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7d64, BIT(26), 0x0);
	}

}

void _halrf_tssi_set_tmeter_tbl_zere_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 i;
	u32 thermal_offset_tmp = 0;
	u32 ftable_reg[TSSI_PATH_MAX_8852A] = {0x600, 0x700};
	u32 ftable_base_reg[TSSI_PATH_MAX_8852A] = {0x7c10, 0x7d10};
	u32 ftable_trigger_reg[TSSI_PATH_MAX_8852A] = {0x7c64, 0x7d64};
	s8 thermal_offset[64] = {0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, ftable_base_reg[path], 0x00010000, 0x0);
	halrf_wreg_fw(rf, ftable_base_reg[path], 0x01000000, 0x1);

	halrf_wreg_fw(rf, ftable_base_reg[path], 0x0000fc00, 32);

	for (i = 0; i < 64; i = i + 4) {
		thermal_offset_tmp = (thermal_offset[i] & 0xff) |
				(thermal_offset[i + 1] & 0xff) << 8 |
				(thermal_offset[i + 2] & 0xff) << 16 |
				(thermal_offset[i + 3] & 0xff) << 24;

		halrf_wreg_fw(rf, (ftable_reg[path] + i), MASKDWORD, thermal_offset_tmp);
		
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] write addr:0x%x value=0x%08x\n",
		       (ftable_reg[path] + i), thermal_offset_tmp);
	}

	halrf_wreg_fw(rf, ftable_trigger_reg[path], BIT(26), 0x1);
	halrf_wreg_fw(rf, ftable_trigger_reg[path], BIT(26), 0x0);

}

void _halrf_tssi_set_dac_gain_tbl_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7cb0, 0x00000fff, 0x000);
		halrf_wreg_fw(rf, 0x7cb0, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x7a00, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a04, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a08, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a0c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a10, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a14, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a18, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a1c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a20, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a24, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a28, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a2c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a30, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a34, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a38, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a3c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a40, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a44, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a48, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a4c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a50, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a54, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a58, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a5c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a60, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a64, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a68, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a6c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a70, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a74, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a78, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a7c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a80, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a84, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a88, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a8c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a90, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a94, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aac, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7abc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ac0, MASKDWORD, 0x00000000);
	} else {
		halrf_wreg_fw(rf, 0x7db0, 0x00000fff, 0x000);
		halrf_wreg_fw(rf, 0x7db0, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x7b00, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b04, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b08, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b0c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b10, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b14, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b18, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b1c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b20, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b24, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b28, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b2c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b30, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b34, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b38, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b3c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b40, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b44, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b48, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b4c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b50, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b54, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b58, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b5c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b60, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b64, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b68, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b6c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b70, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b74, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b78, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b7c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b80, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b84, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b88, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b8c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b90, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b94, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7b9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ba0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ba4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ba8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bac, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bb0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bb4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bb8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bbc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7bc0, MASKDWORD, 0x00000000);
	}

}

void _halrf_tssi_tssi_slope_cal_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c1c, 0x00100000, 0x1);
		halrf_wreg_fw(rf, 0x7c0c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7c0c, 0x80000000, 0x1);
		halrf_wreg_fw(rf, 0x7c28, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c98, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7c30, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c98, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7c38, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c98, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7c40, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c98, 0xff000000, 0x40);
		halrf_wreg_fw(rf, 0x7c48, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c9c, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7c50, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c9c, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7c58, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c9c, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7c60, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7c9c, 0xff000000, 0x40);
	} else {
		halrf_wreg_fw(rf, 0x7d1c, 0x00100000, 0x1);
		halrf_wreg_fw(rf, 0x7d0c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7d0c, 0x80000000, 0x1);
		halrf_wreg_fw(rf, 0x7d28, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d98, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7d30, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d98, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7d38, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d98, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7d40, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d98, 0xff000000, 0x40);
		halrf_wreg_fw(rf, 0x7d48, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d9c, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7d50, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d9c, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7d58, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d9c, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7d60, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7d9c, 0xff000000, 0x40);
	}

}

void _halrf_tssi_set_rf_gap_tbl_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c14, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7ca4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x7ca4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7ca8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7cac, 0x07fc0000, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7d14, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7da4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x7da4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7da8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x7dac, 0x07fc0000, 0x000);
	}

}

void _halrf_tssi_set_tssi_slope_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7c18, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x7c1c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7c20, 0x0000f000, 0xf);
		halrf_wreg_fw(rf, 0x7c1c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x7c1c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7cb8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x7cb8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x7cb4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x7cb8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x7cb8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7c10, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7c0c, 0x10000000, 0x1);
		halrf_wreg_fw(rf, 0x7c0c, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x7c34, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c34, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c38, 0x00000fff, 0x121);
		//halrf_wreg_fw(rf, 0x7c38, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c54, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c54, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c58, 0x00000fff, 0x121);
		//halrf_wreg_fw(rf, 0x7c58, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c24, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c24, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c28, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c28, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c2c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c2c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c30, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c30, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c3c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c3c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c40, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c40, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c44, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c44, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c48, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c48, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c4c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c4c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c50, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c50, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7c5c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7c5c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7c60, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7c60, 0x003ff000, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7d18, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x7d1c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7d20, 0x0000f000, 0xf);
		halrf_wreg_fw(rf, 0x7d1c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x7d1c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7db8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x7db8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x7db4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x7db8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x7db8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7d10, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7d0c, 0x10000000, 0x1);
		halrf_wreg_fw(rf, 0x7d0c, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x7d34, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d34, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d38, 0x00000fff, 0x121);
		//halrf_wreg_fw(rf, 0x7d38, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d54, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d54, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d58, 0x00000fff, 0x121);
		//halrf_wreg_fw(rf, 0x7d58, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d24, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d24, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d28, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d28, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d2c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d2c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d30, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d30, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d3c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d3c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d40, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d40, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d44, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d44, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d48, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d48, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d4c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d4c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d50, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d50, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7d5c, 0x0003ffff, 0x115f2);
		halrf_wreg_fw(rf, 0x7d5c, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x7d60, 0x00000fff, 0x121);
		halrf_wreg_fw(rf, 0x7d60, 0x003ff000, 0x000);
	}

}

void _halrf_tssi_set_tssi_track_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7c18, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7c14, 0x00000800, 0x0);
		halrf_wreg_fw(rf, 0x7c1c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7c64, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7c64, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7c20, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7c14, 0x01000000, 0x0);
	} else {
		halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7d18, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7d14, 0x00000800, 0x0);
		halrf_wreg_fw(rf, 0x7d1c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7d64, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7d64, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7d20, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7d14, 0x01000000, 0x0);
	}
}

void _halrf_tssi_run_tssi_slope_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x1);
	} else {
		halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x1);
	}
}

void _halrf_tssi_hw_tx_8852a(struct rf_info *rf,
			enum phl_phy_idx phy, u8 path, u16 cnt, s16 dbm, u32 rate, u8 bw,
			bool enable)
{
	struct rf_pmac_tx_info tx_info = {0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	tx_info.ppdu = rate;
	tx_info.mcs = 7;
	tx_info.bw = bw;
	tx_info.nss = 1;
	tx_info.gi = 1;
	tx_info.txagc_cw = 0;
	tx_info.dbm = dbm;
	tx_info.cnt = cnt;
	tx_info.time = 20;
	tx_info.period = 5000;
	tx_info.length = 0;

	halrf_set_pmac_tx(rf, phy, path, &tx_info, enable, false);
}

void _halrf_tssi_tssi_pak_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	/*2G 16, 3, -3, -8dBm*/
	/*5G 16 ,3 ,-4 ,-12dBm*/
	//u16 power[4] = {0x1b8, 0x168, 0x138, 0x108};
#if 0
#if 1
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 bw = rf->hal_com->band[phy].cur_chandef.bw;

	s16 power_2g[4] = {64, 12, -12, -32};
	s16 power_5g[4] = {64, 12, -16, -48};
	s16 power[4] = {0};
	u32 tssi_cw_rpt = 0, k;
	u32 tssi_pak_offset_addr[TSSI_PATH_MAX_8852A][4] =
					{{0x7cac, 0x7cac, 0x7ca8, 0x7ca4},
					{0x7dac, 0x7dac, 0x7da8, 0x7da4}};
	u32 tssi_pak_offset_mask[4] = {0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000};
	u32 tssi_trigger[TSSI_PATH_MAX_8852A] = {0x7c20, 0x7d20};
	u32 tssi_cw_rpt_addr[TSSI_PATH_MAX_8852A] = {0x4318, 0x4418};
	s32 tssi_c_2g[4] = {300, 196, 148, 108};
	s32 tssi_c_5g[4] = {300, 196, 140, 76};
	s32 tssi_c[4] = {0};
	s32 pak_offset, pak_offset_tmp; 
	u8 i, j, l;
	u8 avg_times = 1;
	u32 tssi_cw_rpt_total;
	u32 rate = T_HT_MF;
	u8 phy_map;

	phy_map = (BIT(phy) << 4) | BIT(path);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	if (channel >= 1 && channel <= 14) {
		for (i = 0; i < 4; i++) {
			power[i] = power_2g[i];
			tssi_c[i] = tssi_c_2g[i];
			rate = T_HT_MF;
			bw = 0;
		}
	} else {
		for (i = 0; i < 4; i++) {
			power[i] = power_5g[i];
			tssi_c[i] = tssi_c_5g[i];
			rate = T_HT_MF;
			bw = 0;
		}
	}

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_START);
	for (j = 0; j < 4; j++) {
		tssi_cw_rpt_total = 0;
		for (l = 0; l < avg_times; l++) {
			halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x0);
			halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x1);

			_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power[j], rate, bw);
			
			for (k = 0; halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0; k++) {
				halrf_delay_us(rf, 1);

				if (k > 100) {
					halrf_set_pseudo_cw(rf, i, power[j], false);
					RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] TSSI finish bit k > 100us path=%d\n",
						i);
					return;
				}
			}
			tssi_cw_rpt = halrf_rreg(rf, tssi_cw_rpt_addr[path], 0x000000ff);
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] tssi_cw_rpt=0x%x times=%d\n", 
				tssi_cw_rpt, l);
			tssi_cw_rpt_total = tssi_cw_rpt_total + tssi_cw_rpt;
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] RF 0x0=0x%x   path=%d\n",
			halrf_rrf(rf, path, 0x0, 0xfffff), path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] RF 0x1=0x%x   path=%d\n",
			halrf_rrf(rf, path, 0x1, 0xfffff), path);

		halrf_set_pseudo_cw(rf, i, power[j], false);

		tssi_cw_rpt = tssi_cw_rpt_total / avg_times;
		pak_offset = tssi_c[j] - tssi_cw_rpt;
		pak_offset_tmp = pak_offset;

		if (pak_offset >= 0xff) {
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(0x%x) >= 0xff, pak_offset=0xff\n",
				pak_offset);
			pak_offset = 0xff;
		} else if (pak_offset <= -256) {
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(0x%x) <= -256, pak_offset=-256\n",
				pak_offset);
			pak_offset = -256;	
		}

		halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
				tssi_pak_offset_mask[j],
				(pak_offset & 0x1ff));

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Set Power=%d tssi_cw_rpt=0x%x\n",
			power[j], tssi_cw_rpt);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(%d) = tssi_c[%d](%d) - tssi_cw_rpt(%d)\n",
			pak_offset_tmp, j, tssi_c[j], tssi_cw_rpt);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Set 0x%x[%X] = 0x%x\n",
			tssi_pak_offset_addr[j], tssi_pak_offset_mask[j], (pak_offset & 0x1ff));

	}

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_STOP);
#else
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 bw = rf->hal_com->band[phy].cur_chandef.bw;
	s16 power_2g[4] = {64, 12, -12, -32};
	s16 power_5g[4] = {64, 12, -16, -48};
	s16 power[4] = {0};
	u32 tssi_cw_rpt = 0, k;
	u32 tssi_pak_offset_addr[TSSI_PATH_MAX_8852A][4] =
					{{0x7cac, 0x7cac, 0x7ca8, 0x7ca4},
					{0x7dac, 0x7dac, 0x7da8, 0x7da4}};
	u32 tssi_pak_offset_mask[4] = {0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000};
	u32 tssi_trigger[TSSI_PATH_MAX_8852A] = {0x7c20, 0x7d20};
	u32 tssi_cw_rpt_addr[TSSI_PATH_MAX_8852A] = {0x4318, 0x4418};
	s32 tssi_c_2g[4] = {300, 196, 148, 108};
	s32 tssi_c_5g[4] = {300, 196, 140, 76};
	s32 tssi_c[4] = {0};
	s32 pak_offset, pak_offset_tmp; 
	u8 i, j, retry;
	u32 rate = T_HT_MF;
	u8 phy_map;

	phy_map = (BIT(phy) << 4) | BIT(path);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	if (channel >= 1 && channel <= 14) {
		for (i = 0; i < 4; i++) {
			power[i] = power_2g[i];
			tssi_c[i] = tssi_c_2g[i];
			rate = T_HT_MF;
		}
	} else {
		for (i = 0; i < 4; i++) {
			power[i] = power_5g[i];
			tssi_c[i] = tssi_c_5g[i];
			rate = T_VHT;
		}
	}	

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_START);
	for (j = 0; j < 4; j++) {
		for (retry = 0; retry < 6; retry++) {
			halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x0);
			halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x1);

			_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power[j], rate, bw);
			
			for (k = 0; halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0; k++) {
				halrf_delay_us(rf, 1);

				if (k > 100) {
					halrf_set_pseudo_cw(rf, i, power[j], false);
					RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] TSSI finish bit k > 100us path=%d\n",
						path);
					return;
				}
			}
			tssi_cw_rpt = halrf_rreg(rf, tssi_cw_rpt_addr[path], 0x000001ff);
			
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] tssi_cw_rpt=0x%x    retry=%d\n", 
				tssi_cw_rpt, retry);

			if (((tssi_c[j] - tssi_cw_rpt) <= 3 || (tssi_c[j] - tssi_cw_rpt) >= -3) && retry > 0) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] tssi_c[%d](%d)-tssi_cw_rpt(%d)=%d   retry=%d\n", 
					j, tssi_c[j], tssi_cw_rpt, (tssi_c[j] - tssi_cw_rpt), retry);
				break;
			}

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] RF 0x0=0x%x   path=%d\n",
				halrf_rrf(rf, path, 0x0, 0xfffff), path);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] RF 0x1=0x%x   path=%d\n",
				halrf_rrf(rf, path, 0x1, 0xfffff), path);

			halrf_set_pseudo_cw(rf, i, power[j], false);

			pak_offset = tssi_c[j] - tssi_cw_rpt;
			pak_offset_tmp = pak_offset;

			if (pak_offset >= 0xff) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(0x%x) >= 0xff, pak_offset=0xff\n",
					pak_offset);
				pak_offset = 0xff;
			} else if (pak_offset <= -256) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(0x%x) <= -256, pak_offset=-256\n",
					pak_offset);
				pak_offset = -256;	
			}

			halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
					tssi_pak_offset_mask[j],
					(pak_offset & 0x1ff));

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Set Power=%d tssi_cw_rpt=0x%x(%d)\n",
				power[j], tssi_cw_rpt, tssi_cw_rpt);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] pak_offset(%d) = tssi_c[%d](%d) - tssi_cw_rpt(%d)\n",
				pak_offset_tmp, j, tssi_c[j], tssi_cw_rpt);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Set 0x%x[%X] = 0x%x\n",
				tssi_pak_offset_addr[path][j], tssi_pak_offset_mask[j],
				halrf_rreg(rf, tssi_pak_offset_addr[path][j],
					tssi_pak_offset_mask[j]));
		}
	}

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_STOP);

#endif
#endif
	u32 tssi_pak_offset_addr[TSSI_PATH_MAX_8852A][4] =
					{{0x7cac, 0x7cac, 0x7ca8, 0x7ca4},
					{0x7dac, 0x7dac, 0x7da8, 0x7da4}};
	u32 tssi_pak_offset_mask[4] = {0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000};
	u32 tssi_pak_offset_2g[TSSI_PATH_MAX_8852A][4] =
					{{0x6f, 0x50, 0x7, 0xffffffe6},
					{0x74, 0x56, 0x7, 0xffffffe6}};
	u32 tssi_pak_offset_5gl[TSSI_PATH_MAX_8852A][4] =
					{{0x71, 0x5c, 0x1a, 0xfffffff7},
					{0x6d, 0x5a, 0x19, 0xfffffff2}};
	u32 tssi_pak_offset_5gm[TSSI_PATH_MAX_8852A][4] =
					{{0x6d, 0x58, 0x24, 0xfffffffa},
					{0x6e, 0x5a, 0x2d, 0x5}};
	u32 tssi_pak_offset_5gh[TSSI_PATH_MAX_8852A][4] =
					{{0x74, 0x5e, 0x2b, 0x1},
					{0x6e, 0x58, 0x2c, 0x6}};
		
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 j;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	for (j = 0; j < 4; j++) {
		if (channel >= 1 && channel <= 14) {
			halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
				tssi_pak_offset_mask[j], (tssi_pak_offset_2g[path][j] & 0x1ff));
		} else if (channel >= 36 && channel <= 64) {
			halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
				tssi_pak_offset_mask[j], (tssi_pak_offset_5gl[path][j] & 0x1ff));
		} else if (channel >= 100 && channel <= 144) {
			halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
				tssi_pak_offset_mask[j], (tssi_pak_offset_5gm[path][j] & 0x1ff));
		} else if (channel >= 149 && channel <= 177) {
			halrf_wreg_fw(rf, tssi_pak_offset_addr[path][j],
				tssi_pak_offset_mask[j], (tssi_pak_offset_5gh[path][j] & 0x1ff));
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Set 0x%x[%X] = 0x%x\n",
				tssi_pak_offset_addr[path][j], tssi_pak_offset_mask[j],
				halrf_rreg(rf, tssi_pak_offset_addr[path][j],
					tssi_pak_offset_mask[j]));
	}
}

u32 _halrf_tssi_get_cck_efuse_group_8852a(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 offset_index = 0;

	if (channel >= 1 && channel <= 2)
		offset_index = 0;
	else if (channel >= 3 && channel <= 5)
		offset_index = 1;
	else if (channel >= 6 && channel <= 8)
		offset_index = 2;
	else if (channel >= 9 && channel <= 11)
		offset_index = 3;
	else if (channel >= 12 && channel <= 13)
		offset_index = 4;
	else if (channel == 14)
		offset_index = 5;

	return offset_index;
}

u32 _halrf_tssi_get_ofdm_efuse_group_8852a(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 offset_index = 0;

	if (channel >= 1 && channel <= 2)
		offset_index = 0;
	else if (channel >= 3 && channel <= 5)
		offset_index = 1;
	else if (channel >= 6 && channel <= 8)
		offset_index = 2;
	else if (channel >= 9 && channel <= 11)
		offset_index = 3;
	else if (channel >= 12 && channel <= 14)
		offset_index = 4;
	else if (channel >= 36 && channel <= 40)
		offset_index = 5;
	else if (channel >= 42 && channel <= 48)
		offset_index = 6;
	else if (channel >= 50 && channel <= 58)
		offset_index = 7;
	else if (channel >= 60 && channel <= 64)
		offset_index = 8;
	else if (channel >= 100 && channel <= 104)
		offset_index = 9;
	else if (channel >= 106 && channel <= 112)
		offset_index = 10;
	else if (channel >= 114 && channel <= 120)
		offset_index = 11;
	else if (channel >= 122 && channel <= 128)
		offset_index = 12;
	else if (channel >= 130 && channel <= 136)
		offset_index = 13;
	else if (channel >= 138 && channel <= 144)
		offset_index = 14;
	else if (channel >= 149 && channel <= 153)
		offset_index = 15;
	else if (channel >= 155 && channel <= 161)
		offset_index = 16;
	else if (channel >= 163 && channel <= 169)
		offset_index = 17;
	else if (channel >= 171 && channel <= 177)
		offset_index = 18;

	return offset_index;
}

void _halrf_tssi_set_efuse_to_de_8852a(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	
	u32 addr_cck[2] = {0x7c58, 0x7d58};
	u32 addr_cck_bitmask[2] = {0x003ff000, 0x003ff000};
	u32 addr_mcs[2] = {0x7c38, 0x7d38};
	u32 addr_mcs_bitmask[2] = {0x003ff000, 0x003ff000};
	u32 i, offset_index = 0;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	if (tssi_info->tssi_type == TSSI_CAL) {
		halrf_set_tssi_de_offset_zero_8852a(rf, phy);
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s enter cal mode set zero !!!\n", __func__);
		return;
	}

	for (i = 0; i < TSSI_PATH_MAX_8852A; i++) {
		offset_index = _halrf_tssi_get_cck_efuse_group_8852a(rf, phy);
		halrf_wreg_fw(rf, addr_cck[i], addr_cck_bitmask[i],
				tssi_info->tssi_efuse[i][EFUSE_TSSI_CCK][offset_index]);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI CCK DE 0x%x[21:12]=0x%x\n",
			addr_cck[i], halrf_rreg(rf, addr_cck[i], addr_cck_bitmask[i]));

		offset_index = _halrf_tssi_get_ofdm_efuse_group_8852a(rf, phy);
		halrf_wreg_fw(rf, addr_mcs[i], addr_mcs_bitmask[i],
				tssi_info->tssi_efuse[i][EFUSE_TSSI_MCS][offset_index]);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI MCS DE 0x%x[21:12]=0x%x\n",
			addr_mcs[i], halrf_rreg(rf, addr_mcs[i], addr_mcs_bitmask[i]));
	}

}

void _halrf_set_tssi_de_for_tx_verify_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de, u8 path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 tssi_de_tmp;
	s32 s_tssi_de;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d   tssi_de=0x%x   path=%d\n",
		__func__, phy, tssi_de, path);

	if (tssi_de & BIT(7))
		s_tssi_de = tssi_de | 0xffffff00;
	else
		s_tssi_de = tssi_de;

	tssi_de_tmp = s_tssi_de & 0x3ff;

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x7c38, 0x003ff000, tssi_de_tmp);
		halrf_wreg_fw(rf, 0x7c58, 0x003ff000, tssi_de_tmp);
		tssi_info->tssi_de[RF_PATH_A] = s_tssi_de;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7c38[21:12]=0x%x 0x7c58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7c38, 0x003ff000),
			halrf_rreg(rf, 0x7c58, 0x003ff000),
			path);
	} else {
		halrf_wreg_fw(rf, 0x7d38, 0x003ff000, tssi_de_tmp);
		halrf_wreg_fw(rf, 0x7d58, 0x003ff000, tssi_de_tmp);
		tssi_info->tssi_de[RF_PATH_B] = s_tssi_de;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7d38[21:12]=0x%x 0x7d58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7d38, 0x003ff000),
			halrf_rreg(rf, 0x7d58, 0x003ff000),
			path);
	}
	
}

void _halrf_set_tssi_de_offset_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de_offset, u8 path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	s32 tssi_tmp, s_tssi_de_offset;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d   tssi_de_offset=0x%x   path=%d\n",
		__func__, phy, tssi_de_offset, path);

	if (tssi_de_offset & BIT(7))
		s_tssi_de_offset = tssi_de_offset | 0xffffff00;
	else
		s_tssi_de_offset = tssi_de_offset;

	s_tssi_de_offset = s_tssi_de_offset * -1;

	if (path == RF_PATH_A) {
		tssi_tmp = tssi_info->tssi_de[RF_PATH_A] + s_tssi_de_offset;
		tssi_tmp = tssi_tmp & 0x3ff;
		halrf_wreg_fw(rf, 0x7c38, 0x003ff000, tssi_tmp);
		halrf_wreg_fw(rf, 0x7c58, 0x003ff000, tssi_tmp);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] tssi_tmp(%d) = tssi_info->tssi_de[RF_PATH_A](%d) + s_tssi_de_offset(%d) path=%d\n",
			tssi_tmp, tssi_info->tssi_de[RF_PATH_A],
			s_tssi_de_offset, path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7c38[21:12]=0x%x 0x7c58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7c38, 0x003ff000),
			halrf_rreg(rf, 0x7c58, 0x003ff000),
			path);
	} else {
		tssi_tmp = tssi_info->tssi_de[RF_PATH_B] + s_tssi_de_offset;
		tssi_tmp = tssi_tmp & 0x3ff;
		halrf_wreg_fw(rf, 0x7d38, 0x003ff000, tssi_tmp);
		halrf_wreg_fw(rf, 0x7d58, 0x003ff000, tssi_tmp);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] tssi_tmp(%d) = tssi_info->tssi_de[RF_PATH_B](%d) + s_tssi_de_offset(%d) path=%d\n",
			tssi_tmp, tssi_info->tssi_de[RF_PATH_B],
			s_tssi_de_offset, path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7d38[21:12]=0x%x 0x7d58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7d38, 0x003ff000),
			halrf_rreg(rf, 0x7d58, 0x003ff000),
			path);
	}
}

void _halrf_set_tssi_de_offset_zero_8852a(struct rf_info *rf,
							enum phl_phy_idx phy)
{
	halrf_wreg_fw(rf, 0x7c38, 0x003ff000, 0x0);
	halrf_wreg_fw(rf, 0x7c58, 0x003ff000, 0x0);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE Zero 0x7c38[21:12]=0x%x 0x7c58[21:12]=0x%x\n",
		halrf_rreg(rf, 0x7c38, 0x003ff000),
		halrf_rreg(rf, 0x7c58, 0x003ff000));

	halrf_wreg_fw(rf, 0x7d38, 0x003ff000, 0x0);
	halrf_wreg_fw(rf, 0x7d58, 0x003ff000, 0x0);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE Zero 0x7d38[21:12]=0x%x 0x7d58[21:12]=0x%x\n",
		halrf_rreg(rf, 0x7d38, 0x003ff000),
		halrf_rreg(rf, 0x7d58, 0x003ff000));
}

void _halrf_tssi_enable_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 i;
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s\n", __func__);

	for (i = 0; i < TSSI_PATH_MAX_8852A; i++) {
		_halrf_tssi_set_tssi_slope_8852a(rf, phy, i);
		_halrf_tssi_set_tssi_track_8852a(rf, phy, i);

		if (i == RF_PATH_A) { 
			halrf_wrf_fw(rf, RF_PATH_A, 0x55, 0x00080, 0x1);
			halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x0);
			halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x1);
			halrf_wreg_fw(rf, 0x7c18, 0x18000000, 0x3);
			rf->is_tssi_mode[RF_PATH_A] = true;
		} else {
			halrf_wrf_fw(rf, RF_PATH_B, 0x55, 0x00080, 0x1);
			halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x0);
			halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x1);
			halrf_wreg_fw(rf, 0x7d18, 0x18000000, 0x3);
			rf->is_tssi_mode[RF_PATH_B] = true;
		}
	}

}

void _halrf_tssi_disable_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);
	
	halrf_wreg_fw(rf, 0x7c20, 0x80000000, 0x0);
	halrf_wreg_fw(rf, 0x7c18, 0x18000000, 0x0);

	halrf_wreg_fw(rf, 0x7d20, 0x80000000, 0x0);
	halrf_wreg_fw(rf, 0x7d18, 0x18000000, 0x0);

	rf->is_tssi_mode[RF_PATH_A] = false;
	rf->is_tssi_mode[RF_PATH_B] = false;

}

#if 0
void _halrf_do_tssi_thermal_period_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u8 thermal[2] = {0}, i;
	s16 delta_thermal = 0;

	if (tssi_info->tssi_type == TSSI_ON) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy);

		for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
			thermal[i] = halrf_only_get_thermal(rf, i);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] ===> %s   current thermal[%d]=%d\n",
				__func__, i, thermal[i]);

			delta_thermal = thermal[i] - tssi_info->do_tssi_thermal[i];

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] ===> %s   delta_thermal(%d) = thermal[%d](%d) - tssi_info->do_tssi_thermal[%d](%d)\n",
				__func__, delta_thermal, i, thermal[i], i, tssi_info->do_tssi_thermal[i]);

			if (delta_thermal >= 8 || delta_thermal <= -8) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] ===> %s   delta thermal >= 8 do TSSI !!!\n",
					__func__);
				halrf_do_tssi_8852a(rf, phy);
			}
		}
	}
}
#endif

s32 _halrf_get_online_tssi_de_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx,
				u8 path, s32 dbm, s32 puot)
{
	s32 de = ((puot - dbm) * 8) / 100;
	s32 s_tssi_offset;
	u32 tssi_offset;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy_idx);

	if (path == RF_PATH_A) {
		tssi_offset = halrf_rreg(rf, 0x7c38, 0x003ff000);
		/*tssi_offset = halrf_rreg(rf, 0x7c58, 0x003ff000);*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7c38[21:12]=0x%x 0x7c58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7c38, 0x003ff000),
			halrf_rreg(rf, 0x7c58, 0x003ff000),
			path);
	} else {
		tssi_offset = halrf_rreg(rf, 0x7d38, 0x003ff000);
		/*tssi_offset = halrf_rreg(rf, 0x7d58, 0x003ff000);*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7d38[21:12]=0x%x 0x7d58[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7d38, 0x003ff000),
			halrf_rreg(rf, 0x7d58, 0x003ff000),
			path);
	}

	if (tssi_offset & BIT(9))
		s_tssi_offset = tssi_offset | 0xfffffc00;
	else
		s_tssi_offset = tssi_offset;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] de(%d) = de(%d) + s_tssi_offset(%d) path=%d\n",
		(de + s_tssi_offset), de, s_tssi_offset, path);

	de = de + s_tssi_offset;

	de = de & 0xff;

	return de;
}

void _halrf_do_tssi_8852a_acart(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 i;

	halrf_tssi_disable_8852a(rf, phy);

	for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
		_halrf_tssi_rf_setting_8852a(rf, phy, i);
		_halrf_tssi_set_sys_8852a(rf, phy);
		_halrf_tssi_ini_txpwr_ctrl_bb_8852a(rf, phy, i);
		_halrf_tssi_set_dck_8852a(rf, phy, i);
		_halrf_tssi_set_bbgain_split_bcar_8852a(rf, phy, i);
		_halrf_tssi_set_tmeter_tbl_8852a(rf, phy, i);
		_halrf_tssi_set_dac_gain_tbl_8852a(rf, phy, i);
		_halrf_tssi_tssi_slope_cal_8852a(rf, phy, i);
		_halrf_tssi_set_rf_gap_tbl_8852a(rf, phy, i);
		_halrf_tssi_set_tssi_slope_8852a(rf, phy, i);
		_halrf_tssi_run_tssi_slope_8852a(rf, phy, i);

		/*Enable halrf_tmac_tx_pause*/
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
		halrf_tmac_tx_pause(rf, phy, true);
		_halrf_tssi_tssi_pak_8852a(rf, phy, i);
		halrf_tmac_tx_pause(rf, phy, false);
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);
		/*Disable halrf_tmac_tx_pause*/

		/*_halrf_tssi_set_tmeter_tbl_zere_8852a(rf, phy, i);*/
	}

	_halrf_tssi_set_efuse_to_de_8852a(rf, phy);

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
		if (tssi_info->tssi_type == TSSI_ON || tssi_info->tssi_type == TSSI_CAL) {
			halrf_tssi_enable_8852a(rf, phy);
		} else
			halrf_tssi_disable_8852a(rf, phy);

	} else {
		if (rf->support_ability & HAL_RF_TX_PWR_TRACK)
			halrf_tssi_enable_8852a(rf, phy);
	}

}


void halrf_tssi_cck_8852a(struct rf_info *rf, enum phl_phy_idx phy, bool is_cck)
{
#if 0
	u8 i;
	u32 read_tssi_dc_offset_addr[TSSI_PATH_MAX_8852A] = {0x4304, 0x4404};
	u32 tssi_dc_offset;
	u32 auto_dc_k_off[TSSI_PATH_MAX_8852A] = {0x7c14, 0x7d14};
	u32 write_tssi_dc_offset_addr[TSSI_PATH_MAX_8852A] = {0x7c0c, 0x7d0c};
	u8 phy_map;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy);

	if (is_cck == true) {
		/*Enable halrf_tmac_tx_pause*/
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
		halrf_tmac_tx_pause(rf, phy, true);
		
		for (i = RF_PATH_A ; i < TSSI_PATH_MAX_8852A; i++) {
			phy_map = (BIT(phy) << 4) | BIT(i);

			halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_START);

			halrf_wreg_fw(rf, auto_dc_k_off[i], BIT(13), 0x0);
			halrf_wreg_fw(rf, write_tssi_dc_offset_addr[i], 0x0fff0000, 0x0);
			halrf_wreg_fw(rf, auto_dc_k_off[i], BIT(13), 0x1);
			
			_halrf_tssi_hw_tx_8852a(rf, phy, i, 20, 64, T_HT_MF, 0);
			tssi_dc_offset = halrf_rreg(rf, read_tssi_dc_offset_addr[i], 0x00fff000);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Get tssi_dc_offset 0x%x[23:12]=0x%x   path=%d\n", 
				read_tssi_dc_offset_addr[i], tssi_dc_offset, i);

			halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_STOP);

			halrf_wreg_fw(rf, auto_dc_k_off[i], BIT(13), 0x0);
			halrf_wreg_fw(rf, write_tssi_dc_offset_addr[i], 0x0fff0000, tssi_dc_offset);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set 0x%x[27:16]=0x%x   path=%d\n", 
				write_tssi_dc_offset_addr[i], tssi_dc_offset, i);
		}

		halrf_tmac_tx_pause(rf, phy, false);
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);
	} else {
		for (i = RF_PATH_A ; i < TSSI_PATH_MAX_8852A; i++) {
			halrf_wreg_fw(rf, write_tssi_dc_offset_addr[i], 0x0fff0000, 0x0);
			halrf_wreg_fw(rf, auto_dc_k_off[i], BIT(13), 0x1);
		}
	}
#endif
}


/*AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA*/

void _tssi_backup_bb_registers_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)
{
	u32 i;

	for (i = 0; i < reg_num; i++) {
		reg_backup[i] = halrf_rreg(rf, reg[i], MASKDWORD);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Backup BB 0x%x = 0x%x\n",
		       reg[i], reg_backup[i]);
	}
}

void _tssi_reload_bb_registers_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)

{
	u32 i;

	for (i = 0; i < reg_num; i++) {
		halrf_wreg(rf, reg[i], MASKDWORD, reg_backup[i]);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Reload BB 0x%x = 0x%x\n",
		       reg[i], reg_backup[i]);
	}
}

void _halrf_tssi_rf_setting_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	if (channel >= 1 && channel <= 14)
		halrf_wrf_fw(rf, path, 0x7f, 0x00002, 0x1);
	else
		halrf_wrf_fw(rf, path, 0x7f, 0x00100, 0x1);

	if (rfe_type > 50) {
		if (channel >= 1 && channel <= 14)
			halrf_wrf_fw(rf, path, 0x6d, 0x00004, 0x1);
		else
			halrf_wrf_fw(rf, path, 0x6d, 0x00080, 0x1);
	}
}

void _halrf_tssi_set_sys_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	halrf_wreg_fw(rf, 0x12a8, 0x00000001, 0x1);
	halrf_wreg_fw(rf, 0x12a8, 0x0000000e, 0x2);
	halrf_wreg_fw(rf, 0x32a8, 0x00000001, 0x1);
	halrf_wreg_fw(rf, 0x32a8, 0x0000000e, 0x2);
	halrf_wreg_fw(rf, 0x12bc, 0x000000f0, 0x5);
	halrf_wreg_fw(rf, 0x12bc, 0x00000f00, 0x5);
	halrf_wreg_fw(rf, 0x12bc, 0x000f0000, 0x5);
	halrf_wreg_fw(rf, 0x12bc, 0x0000f000, 0x5);
	halrf_wreg_fw(rf, 0x120c, 0x000000ff, 0x33);
	halrf_wreg_fw(rf, 0x12c0, 0x0ff00000, 0x33);
	halrf_wreg_fw(rf, 0x32bc, 0x000000f0, 0x5);
	halrf_wreg_fw(rf, 0x32bc, 0x00000f00, 0x5);
	halrf_wreg_fw(rf, 0x32bc, 0x000f0000, 0x5);
	halrf_wreg_fw(rf, 0x32bc, 0x0000f000, 0x5);
	halrf_wreg_fw(rf, 0x320c, 0x000000ff, 0x33);
	halrf_wreg_fw(rf, 0x32c0, 0x0ff00000, 0x33);
	halrf_wreg_fw(rf, 0x0300, 0xff000000, 0x19);
	halrf_wreg_fw(rf, 0x0304, 0x000000ff, 0x19);
	halrf_wreg_fw(rf, 0x0304, 0x0000ff00, 0x1d);
	halrf_wreg_fw(rf, 0x0314, 0xffff0000, 0x2044);
	halrf_wreg_fw(rf, 0x0318, 0x0000ffff, 0x2042);
	halrf_wreg_fw(rf, 0x0318, 0xffff0000, 0x2002);
	halrf_wreg_fw(rf, 0x0020, 0x00006000, 0x3);
	halrf_wreg_fw(rf, 0x0024, 0x00006000, 0x3);
	halrf_wreg_fw(rf, 0x0704, 0xffff0000, 0x601e);
	halrf_wreg_fw(rf, 0x2704, 0xffff0000, 0x601e);
	halrf_wreg_fw(rf, 0x0700, 0xf0000000, 0x4);
	halrf_wreg_fw(rf, 0x2700, 0xf0000000, 0x4);
	halrf_wreg_fw(rf, 0x0650, 0x3c000000, 0x0);
	halrf_wreg_fw(rf, 0x2650, 0x3c000000, 0x0);	

	if (channel >= 1 && channel <= 14) {
		halrf_wreg_fw(rf, 0x120c, 0x000000ff, 0x33);
		halrf_wreg_fw(rf, 0x12c0, 0x0ff00000, 0x33);

		halrf_wreg_fw(rf, 0x32c0, 0x0ff00000, 0x33);
		halrf_wreg_fw(rf, 0x320c, 0x000000ff, 0x33);
	} else {
		
		halrf_wreg_fw(rf, 0x120c, 0x000000ff, 0x44);
		halrf_wreg_fw(rf, 0x12c0, 0x0ff00000, 0x44);

		halrf_wreg_fw(rf, 0x32c0, 0x0ff00000, 0x44);
		halrf_wreg_fw(rf, 0x320c, 0x000000ff, 0x44);
	}
}

void _halrf_tssi_ini_txpwr_ctrl_bb_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 bb_version;

	bb_version = halrf_rreg(rf, 0xf4, 0x000000ff);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   bb_version=%d   path=%d\n",
		__func__, bb_version, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5800, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x5800, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x5800, 0x003f0000, 0x3f);
		/*halrf_wreg_fw(rf, 0x5800, 0x0fc00000, 0x0);*/
		halrf_wreg_fw(rf, 0x5800, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x5800, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x5800, 0xc0000000, 0x0);
		/*Debug power cw setting with Dino*/
		/*halrf_wreg_fw(rf, 0x5804, 0x000001ff, 0x040);*/
		/*halrf_wreg_fw(rf, 0x5804, 0x07fc0000, 0xac);*/
		halrf_wreg_fw(rf, 0x5804, 0xf8000000, 0x00);
		/*halrf_wreg_fw(rf, 0x5808, 0x000001ff, 0x040);*/
		/*halrf_wreg_fw(rf, 0x5808, 0x07fc0000, 0xac);*/
		halrf_wreg_fw(rf, 0x580c, 0x0000007f, 0x40);
		halrf_wreg_fw(rf, 0x580c, 0x00007f00, 0x40);
		halrf_wreg_fw(rf, 0x580c, 0x00008000, 0x0);
		halrf_wreg_fw(rf, 0x580c, 0x0fff0000, 0x000);
		halrf_wreg_fw(rf, 0x5810, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x5810, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x5810, 0x0000fc00, 0x00);
		halrf_wreg_fw(rf, 0x5810, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x5810, 0x00fe0000, 0x00);
		halrf_wreg_fw(rf, 0x5810, 0x01000000, 0x1);
		halrf_wreg_fw(rf, 0x5810, 0x06000000, 0x0);
		halrf_wreg_fw(rf, 0x5810, 0x38000000, 0x3);
		halrf_wreg_fw(rf, 0x5810, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x5810, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x5814, 0x00000c00, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x5814, 0x00002000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x00004000, 0x1);
		halrf_wreg_fw(rf, 0x5814, 0x00038000, 0x5);

		if (rfe_type <= 50)
			halrf_wreg_fw(rf, 0x5814, 0x003c0000, 0x0);

		halrf_wreg_fw(rf, 0x5814, 0x01c00000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0xe0000000, 0x0);
		halrf_wreg_fw(rf, 0x5818, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x5818, 0x0001ff00, 0x018);
		halrf_wreg_fw(rf, 0x5818, 0x03fe0000, 0x016);
		halrf_wreg_fw(rf, 0x5818, 0xfc000000, 0x00);
		halrf_wreg_fw(rf, 0x581c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x581c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x581c, 0x00100000, 0x0);

		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0x8);
			halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0xe);
			halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0x8);
			halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0xe);
		}

		halrf_wreg_fw(rf, 0x581c, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x5820, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x5820, 0x0000f000, 0xf);
		halrf_wreg_fw(rf, 0x5820, 0x001f0000, 0x00);
		halrf_wreg_fw(rf, 0x5820, 0xffe00000, 0x000);


		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x121);
		}

		halrf_wreg_fw(rf, 0x5828, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5828, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5830, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5830, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5838, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5838, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5840, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5840, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5848, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5848, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5850, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5850, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5858, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5858, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x5860, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x5860, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x5860, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5864, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x5864, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x5864, 0x03f00000, 0x00);
		halrf_wreg_fw(rf, 0x5864, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x5898, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x589c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x58a0, 0x000000ff, 0xfd);
		halrf_wreg_fw(rf, 0x58a0, 0x0000ff00, 0xe5);
		halrf_wreg_fw(rf, 0x58a0, 0x00ff0000, 0xcd);
		halrf_wreg_fw(rf, 0x58a0, 0xff000000, 0xb5);
		halrf_wreg_fw(rf, 0x58a4, 0x000000ff, 0x16);
		halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x000);
		if (bb_version < 0x28)
			halrf_wreg_fw(rf, 0x58b0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x58b4, 0x0000001f, 0x00);
		halrf_wreg_fw(rf, 0x58b4, 0x00000020, 0x0);
		halrf_wreg_fw(rf, 0x58b4, 0x000001c0, 0x0);
		halrf_wreg_fw(rf, 0x58b4, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x58b4, 0x0000f000, 0x2);
		halrf_wreg_fw(rf, 0x58b4, 0x00ff0000, 0x00);
		halrf_wreg_fw(rf, 0x58b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x58b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x58b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x58b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x58b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x58bc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x58bc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x58bc, 0x00030000, 0x3);

		/*GNT_WL==0 & 0x58bc[18]==1, Bypass TSSI*/
		/*GNT_BT==1 & 0x58bc[20]==1, Bypass TSSI*/
		/*GNT_BT_TX==1 & 0x58bc[22]==1, Bypass TSSI*/

		halrf_wreg_fw(rf, 0x58bc, 0x000c0000, 0x1);
		if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
			halrf_wreg_fw(rf, 0x58bc, 0x00300000, 0x3);
			halrf_wreg_fw(rf, 0x58bc, 0x00c00000, 0x3);
		} else {
			halrf_wreg_fw(rf, 0x58bc, 0x00300000, 0x2);
			halrf_wreg_fw(rf, 0x58bc, 0x00c00000, 0x2);
		}

		halrf_wreg_fw(rf, 0x58bc, 0x07000000, 0x7);
		halrf_wreg_fw(rf, 0x58c0, 0x00fe0000, 0x3f);
		halrf_wreg_fw(rf, 0x58c0, 0xff000000, 0x00);
		halrf_wreg_fw(rf, 0x58c4, 0x0003ffff, 0x3ffff);
		halrf_wreg_fw(rf, 0x58c4, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x58c4, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x58c8, 0x00ffffff, 0x000000);
		halrf_wreg_fw(rf, 0x58c8, 0xf0000000, 0x0);
		halrf_wreg_fw(rf, 0x58cc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x58d0, 0x00001fff, 0x0101);
		halrf_wreg_fw(rf, 0x58d0, 0x0001e000, 0x4);
		halrf_wreg_fw(rf, 0x58d0, 0x03fe0000, 0x100);
		halrf_wreg_fw(rf, 0x58d0, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x58d4, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x58d4, 0x0003fe00, 0x0ff);
		halrf_wreg_fw(rf, 0x58d4, 0x07fc0000, 0x100);

		if (channel <= 14) {
			halrf_wreg_fw(rf, 0x58d8, 0x000001ff, 0x13c);
			halrf_wreg_fw(rf, 0x58d8, 0x0003fe00, 0x05c);
		} else {
			if (rfe_type > 50) {
				halrf_wreg_fw(rf, 0x58d8, 0x000001ff, 0x18c);
				halrf_wreg_fw(rf, 0x58d8, 0x0003fe00, 0x000);
			} else if (rfe_type == 50) {
				halrf_wreg_fw(rf, 0x58d8, 0x000001ff, 0x15c);
				halrf_wreg_fw(rf, 0x58d8, 0x0003fe00, 0x05c);
			} else {
				halrf_wreg_fw(rf, 0x58d8, 0x000001ff, 0x16c);
				halrf_wreg_fw(rf, 0x58d8, 0x0003fe00, 0x05c);
			}
		}

		halrf_wreg_fw(rf, 0x58d8, 0x000c0000, 0x2);
		halrf_wreg_fw(rf, 0x58d8, 0xfff00000, 0x800);
		halrf_wreg_fw(rf, 0x58dc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x58dc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x58dc, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x58dc, 0x3ff00000, 0x000);
		halrf_wreg_fw(rf, 0x58dc, 0x80000000, 0x1);
		halrf_wreg_fw(rf, 0x58f0, 0x000001ff, 0x1ff);
		halrf_wreg_fw(rf, 0x58f0, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7800, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x7800, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x7800, 0x003f0000, 0x3f);
		/*halrf_wreg_fw(rf, 0x7800, 0x0fc00000, 0x0);*/
		halrf_wreg_fw(rf, 0x7800, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x7800, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7800, 0xc0000000, 0x0);
		/*Debug power cw setting with Dino*/
		/*halrf_wreg_fw(rf, 0x7804, 0x000001ff, 0x040);*/
		/*halrf_wreg_fw(rf, 0x7804, 0x07fc0000, 0xac);*/
		halrf_wreg_fw(rf, 0x7804, 0xf8000000, 0x00);
		/*halrf_wreg_fw(rf, 0x7808, 0x000001ff, 0x040);*/
		/*halrf_wreg_fw(rf, 0x7808, 0x07fc0000, 0xac);*/
		halrf_wreg_fw(rf, 0x780c, 0x0000007f, 0x40);
		halrf_wreg_fw(rf, 0x780c, 0x00007f00, 0x40);
		halrf_wreg_fw(rf, 0x780c, 0x00008000, 0x0);
		halrf_wreg_fw(rf, 0x780c, 0x0fff0000, 0x000);
		halrf_wreg_fw(rf, 0x7810, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x7810, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x7810, 0x0000fc00, 0x00);
		halrf_wreg_fw(rf, 0x7810, 0x00010000, 0x1);
		halrf_wreg_fw(rf, 0x7810, 0x00fe0000, 0x00);
		halrf_wreg_fw(rf, 0x7810, 0x01000000, 0x1);
		halrf_wreg_fw(rf, 0x7810, 0x06000000, 0x0);
		halrf_wreg_fw(rf, 0x7810, 0x38000000, 0x3);
		halrf_wreg_fw(rf, 0x7810, 0x40000000, 0x1);
		halrf_wreg_fw(rf, 0x7810, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x7814, 0x00000c00, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x7814, 0x00002000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x00004000, 0x1);
		halrf_wreg_fw(rf, 0x7814, 0x00038000, 0x5);

		if (rfe_type <= 50)
			halrf_wreg_fw(rf, 0x7814, 0x003c0000, 0x0);

		halrf_wreg_fw(rf, 0x7814, 0x01c00000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0xe0000000, 0x0);
		halrf_wreg_fw(rf, 0x7818, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x7818, 0x0001ff00, 0x018);
		halrf_wreg_fw(rf, 0x7818, 0x03fe0000, 0x016);
		halrf_wreg_fw(rf, 0x7818, 0xfc000000, 0x00);
		halrf_wreg_fw(rf, 0x781c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x781c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x781c, 0x00100000, 0x0);

		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0x8);
			halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0xe);
			halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0x8);
			halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0xe);
		}

		halrf_wreg_fw(rf, 0x781c, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7820, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7820, 0x0000f000, 0x0);
		halrf_wreg_fw(rf, 0x7820, 0x001f0000, 0x00);
		halrf_wreg_fw(rf, 0x7820, 0xffe00000, 0x000);

		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x121);
		}

		halrf_wreg_fw(rf, 0x7828, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7828, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7830, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7830, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7838, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7838, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7840, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7840, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7848, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7848, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7850, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7850, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7858, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7858, 0x7fc00000, 0x000);

		halrf_wreg_fw(rf, 0x7860, 0x003ff000, 0x000);
		halrf_wreg_fw(rf, 0x7860, 0x7fc00000, 0x000);
		halrf_wreg_fw(rf, 0x7860, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7864, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7864, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7864, 0x03f00000, 0x00);
		halrf_wreg_fw(rf, 0x7864, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x7898, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x789c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x78a0, 0x000000ff, 0xfd);
		halrf_wreg_fw(rf, 0x78a0, 0x0000ff00, 0xe5);
		halrf_wreg_fw(rf, 0x78a0, 0x00ff0000, 0xcd);
		halrf_wreg_fw(rf, 0x78a0, 0xff000000, 0xb5);
		halrf_wreg_fw(rf, 0x78a4, 0x000000ff, 0x16);
		halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x000);
		if (bb_version < 0x28)
			halrf_wreg_fw(rf, 0x78b0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x78b4, 0x0000001f, 0x00);
		halrf_wreg_fw(rf, 0x78b4, 0x00000020, 0x0);
		halrf_wreg_fw(rf, 0x78b4, 0x000001c0, 0x0);
		halrf_wreg_fw(rf, 0x78b4, 0x00000200, 0x0);
		halrf_wreg_fw(rf, 0x78b4, 0x0000f000, 0x2);
		halrf_wreg_fw(rf, 0x78b4, 0x00ff0000, 0x00);
		halrf_wreg_fw(rf, 0x78b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x78b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x78b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x78b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x78b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x78bc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x78bc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x78bc, 0x00030000, 0x3);

		/*GNT_WL==0 & 0x78bc[18]==1, Bypass TSSI*/
		/*GNT_BT==1 & 0x78bc[20]==1, Bypass TSSI*/
		/*GNT_BT_TX==1 & 0x78bc[22]==1, Bypass TSSI*/

		halrf_wreg_fw(rf, 0x78bc, 0x000c0000, 0x1);
		if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
			halrf_wreg_fw(rf, 0x78bc, 0x00300000, 0x3);
			halrf_wreg_fw(rf, 0x78bc, 0x00c00000, 0x3);
		} else {
			halrf_wreg_fw(rf, 0x78bc, 0x00300000, 0x2);
			halrf_wreg_fw(rf, 0x78bc, 0x00c00000, 0x2);
		}

		halrf_wreg_fw(rf, 0x78bc, 0x07000000, 0x7);
		halrf_wreg_fw(rf, 0x78c0, 0x00fe0000, 0x3f);
		halrf_wreg_fw(rf, 0x78c0, 0xff000000, 0x00);
		halrf_wreg_fw(rf, 0x78c4, 0x0003ffff, 0x3ffff);
		halrf_wreg_fw(rf, 0x78c4, 0x3ffc0000, 0x000);
		halrf_wreg_fw(rf, 0x78c4, 0xc0000000, 0x0);
		halrf_wreg_fw(rf, 0x78c8, 0x00ffffff, 0x000000);
		halrf_wreg_fw(rf, 0x78c8, 0xf0000000, 0x0);
		halrf_wreg_fw(rf, 0x78cc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x78d0, 0x00001fff, 0x0101);
		halrf_wreg_fw(rf, 0x78d0, 0x0001e000, 0x4);
		halrf_wreg_fw(rf, 0x78d0, 0x03fe0000, 0x100);
		halrf_wreg_fw(rf, 0x78d0, 0x04000000, 0x0);
		halrf_wreg_fw(rf, 0x78d4, 0x000000ff, 0x00);
		halrf_wreg_fw(rf, 0x78d4, 0x0003fe00, 0x0ff);
		halrf_wreg_fw(rf, 0x78d4, 0x07fc0000, 0x100);

		if (channel <= 14) {
			halrf_wreg_fw(rf, 0x78d8, 0x000001ff, 0x13c);
			halrf_wreg_fw(rf, 0x78d8, 0x0003fe00, 0x05c);
		} else {
			if (rfe_type > 50) {
				halrf_wreg_fw(rf, 0x78d8, 0x000001ff, 0x18c);
				halrf_wreg_fw(rf, 0x78d8, 0x0003fe00, 0x000);
			} else if (rfe_type == 50) {
				halrf_wreg_fw(rf, 0x78d8, 0x000001ff, 0x15c);
				halrf_wreg_fw(rf, 0x78d8, 0x0003fe00, 0x05c);
			} else {
				halrf_wreg_fw(rf, 0x78d8, 0x000001ff, 0x16c);
				halrf_wreg_fw(rf, 0x78d8, 0x0003fe00, 0x05c);
			}
		}

		halrf_wreg_fw(rf, 0x78d8, 0x000c0000, 0x2);
		halrf_wreg_fw(rf, 0x78d8, 0xfff00000, 0x800);
		halrf_wreg_fw(rf, 0x78dc, 0x000000ff, 0x7f);
		halrf_wreg_fw(rf, 0x78dc, 0x0000ff00, 0x80);
		halrf_wreg_fw(rf, 0x78dc, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x78dc, 0x3ff00000, 0x000);
		halrf_wreg_fw(rf, 0x78dc, 0x80000000, 0x1);
		halrf_wreg_fw(rf, 0x78f0, 0x000001ff, 0x1ff);
		halrf_wreg_fw(rf, 0x78f0, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
	}

}

void _halrf_tssi_ini_txpwr_ctrl_bb_he_tb_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x58a0, MASKDWORD, 0x000000fc);
		halrf_wreg_fw(rf, 0x58e4, 0x0000007f, 0x20);
	} else {
		halrf_wreg_fw(rf, 0x78a0, MASKDWORD, 0x000000fc);
		halrf_wreg_fw(rf, 0x78e4, 0x0000007f, 0x20);
	}
}

void _halrf_tssi_set_dck_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u32 r_tssi_adc[TSSI_PATH_MAX_8852A] = {0x580c, 0x780c};
	u32 r_tssi_dck[TSSI_PATH_MAX_8852A] = {0x5814, 0x7814};
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, r_tssi_adc[path], 0x0fff0000, 0x000);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00001000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00002000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00004000, 0x1);
	halrf_wreg_fw(rf, r_tssi_dck[path], 0x00038000, 0x5);

	if (rfe_type <= 50)
		halrf_wreg_fw(rf, r_tssi_dck[path], 0x003c0000, 0x3);

	halrf_wreg_fw(rf, r_tssi_dck[path], 0x18000000, 0x0);
	
}

void _halrf_tssi_set_bbgain_split_bcar_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
#if 0
	u32 secret_test_mode[TSSI_PATH_MAX_8852A] = {0x5818, 0x7818};
	u32 r_tx_gain_scale[TSSI_PATH_MAX_8852A] = {0x58d4, 0x78d4};
	u32 tx_gain_post[TSSI_PATH_MAX_8852A] = {0x58f0, 0x78f0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, secret_test_mode[path], 0x08000000, 0x1);
	halrf_wreg_fw(rf, r_tx_gain_scale[path], 0xf0000000, 0x7);
	halrf_wreg_fw(rf, tx_gain_post[path], 0x000c0000, 0x1);
	halrf_wreg_fw(rf, tx_gain_post[path], 0xfff00000, 0x400);
#endif
}

void _halrf_tssi_set_tmeter_tbl_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_pwr_track_info *pwr_trk = &rf->pwr_track;
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	struct rtw_hal_com_t *hal = rf->hal_com;

	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	u8 i, thermal = 0xff, j;
	//u8 rate = phydm_get_tx_rate(dm);
	u32 thermal_offset_tmp = 0;
	s8 thermal_offset[64] = {0};
	u8 thermal_up_a[DELTA_SWINGIDX_SIZE] = {0}, thermal_down_a[DELTA_SWINGIDX_SIZE] = {0};
	u8 thermal_up_b[DELTA_SWINGIDX_SIZE] = {0}, thermal_down_b[DELTA_SWINGIDX_SIZE] = {0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (tssi_info->tssi_type == TSSI_CAL) {
		halrf_wreg_fw(rf, 0x5810, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x5810, 0x01000000, 0x1);

		halrf_wreg_fw(rf, 0x7810, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x7810, 0x01000000, 0x1);

		halrf_wreg_fw(rf, 0x5810, 0x0000fc00, 32);
		halrf_wreg_fw(rf, 0x5864, 0x03f00000, 32);
		halrf_wreg_fw(rf, 0x7810, 0x0000fc00, 32);
		halrf_wreg_fw(rf, 0x7864, 0x03f00000, 32);

		for (i = 0; i < 64; i = i + 4) {
			thermal_offset_tmp = (thermal_offset[i] & 0xff) |
					(thermal_offset[i + 1] & 0xff) << 8 |
					(thermal_offset[i + 2] & 0xff) << 16 |
					(thermal_offset[i + 3] & 0xff) << 24;

			halrf_wreg_fw(rf, (0x5c00 + i), MASKDWORD, thermal_offset_tmp);
			halrf_wreg_fw(rf, (0x7c00 + i), MASKDWORD, thermal_offset_tmp);
			
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI] write addr:0x%x value=0x%08x\n",
			       (0x5c00 + i), thermal_offset_tmp);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI] write addr:0x%x value=0x%08x\n",
			       (0x7c00 + i), thermal_offset_tmp);
		}

		halrf_wreg_fw(rf, 0x5864, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x5864, BIT(26), 0x0);

		halrf_wreg_fw(rf, 0x7864, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7864, BIT(26), 0x0);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s TSSI Calibration Mode return !!!\n", __func__);

		return;
	}

	if (channel >= 1 && channel <= 14) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_2ga_p, sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_2ga_n, sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_2gb_p, sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_2gb_n, sizeof(thermal_down_b));
	} else if (channel >= 36 && channel <= 64) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[0], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[0], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[0], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[0], sizeof(thermal_down_b));
	} else if (channel >= 100 && channel <= 144) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[1], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[1], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[1], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[1], sizeof(thermal_down_b));
	} else if (channel >= 149 && channel <= 177) {
		hal_mem_cpy(hal, thermal_up_a, pwr_trk->delta_swing_table_idx_5ga_p[2], sizeof(thermal_up_a));
		hal_mem_cpy(hal, thermal_down_a, pwr_trk->delta_swing_table_idx_5ga_n[2], sizeof(thermal_down_a));
		hal_mem_cpy(hal, thermal_up_b, pwr_trk->delta_swing_table_idx_5gb_p[2], sizeof(thermal_up_b));
		hal_mem_cpy(hal, thermal_down_b, pwr_trk->delta_swing_table_idx_5gb_n[2], sizeof(thermal_down_b));
	}

	/*path s0*/
	if (path == RF_PATH_A) {
		hal_mem_set(hal, thermal_offset, 0, sizeof(thermal_offset));
		halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_A, &thermal, 1);

		/*thermal = 32;*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] channel=%d thermal_pahtA=0x%x tssi_info->tssi_type=%d\n",
		       channel, thermal, tssi_info->tssi_type);

		halrf_wreg_fw(rf, 0x5810, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x5810, 0x01000000, 0x1);

		if (thermal == 0xff) {
			halrf_wreg_fw(rf, 0x5810, 0x0000fc00, 32);
			halrf_wreg_fw(rf, 0x5864, 0x03f00000, 32);

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x5c00 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x5c00 + i), thermal_offset_tmp);
			}
		}

		tssi_info->thermal[RF_PATH_A] = thermal;

		if (thermal != 0xff) {
			halrf_wreg_fw(rf, 0x5810, 0x0000fc00, (thermal & 0x3f));
			halrf_wreg_fw(rf, 0x5864, 0x03f00000, (thermal & 0x3f));

			i = 0;
			for (j = 0; j < 32; j++) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = -1 * thermal_down_a[i++];
				else
					thermal_offset[j] = -1 * thermal_down_a[DELTA_SWINGIDX_SIZE - 1];
			}

			i = 1;
			for (j = 63; j >= 32; j--) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = thermal_up_a[i++];
				else
					thermal_offset[j] = thermal_up_a[DELTA_SWINGIDX_SIZE - 1];
			}

			for (i = 0; i < 64; i = i + 4) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] thermal_offset[%.2d]=%.2x %.2x %.2x %.2x\n",
				       i, thermal_offset[i + 3] & 0xff, thermal_offset[i + 2] & 0xff,
				       thermal_offset[i + 1] & 0xff, thermal_offset[i] & 0xff);
			}

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x5c00 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x5c00 + i), thermal_offset_tmp);
			}
		}
		halrf_wreg_fw(rf, 0x5864, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x5864, BIT(26), 0x0);
	} else {
		/*path s1*/
		hal_mem_set(hal, thermal_offset, 0, sizeof(thermal_offset));
		halrf_efuse_get_info(rf, EFUSE_INFO_RF_THERMAL_B, &thermal, 1);

		/*thermal = 32;*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] channel=%d thermal_pahtB=0x%x tssi_info->tssi_type=%d\n",
		       channel, thermal, tssi_info->tssi_type);

		halrf_wreg_fw(rf, 0x7810, 0x00010000, 0x0);
		halrf_wreg_fw(rf, 0x7810, 0x01000000, 0x1);

		if (thermal == 0xff) {
			halrf_wreg_fw(rf, 0x7810, 0x0000fc00, 32);
			halrf_wreg_fw(rf, 0x7864, 0x03f00000, 32);

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x7c00 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x7c00 + i), thermal_offset_tmp);
			}
		}

		tssi_info->thermal[RF_PATH_B] = thermal;

		if (thermal != 0xff) {
			halrf_wreg_fw(rf, 0x7810, 0x0000fc00, (thermal & 0x3f));
			halrf_wreg_fw(rf, 0x7864, 0x03f00000, (thermal & 0x3f));

			i = 0;
			for (j = 0; j < 32; j++) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = -1 * thermal_down_b[i++];
				else
					thermal_offset[j] = -1 * thermal_down_b[DELTA_SWINGIDX_SIZE - 1];
			}

			i = 1;
			for (j = 63; j >= 32; j--) {
				if (i < DELTA_SWINGIDX_SIZE)
					thermal_offset[j] = thermal_up_b[i++];
				else
					thermal_offset[j] = thermal_up_b[DELTA_SWINGIDX_SIZE - 1];
			}

			for (i = 0; i < 64; i = i + 4) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] thermal_offset[%.2d]=%.2x %.2x %.2x %.2x\n",
				       i, thermal_offset[i + 3] & 0xff, thermal_offset[i + 2] & 0xff,
				       thermal_offset[i + 1] & 0xff, thermal_offset[i] & 0xff);
			}

			for (i = 0; i < 64; i = i + 4) {
				thermal_offset_tmp = (thermal_offset[i] & 0xff) |
						(thermal_offset[i + 1] & 0xff) << 8 |
						(thermal_offset[i + 2] & 0xff) << 16 |
						(thermal_offset[i + 3] & 0xff) << 24;

				halrf_wreg_fw(rf, (0x7c00 + i), MASKDWORD, thermal_offset_tmp);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				       "[TSSI] write addr:0x%x value=0x%08x\n",
				       (0x7c00 + i), thermal_offset_tmp);
			}
		}
		halrf_wreg_fw(rf, 0x7864, BIT(26), 0x1);
		halrf_wreg_fw(rf, 0x7864, BIT(26), 0x0);
	}

}

void _halrf_tssi_set_tmeter_tbl_zere_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 i;
	u32 thermal_offset_tmp = 0;
	u32 ftable_reg[TSSI_PATH_MAX_8852A] = {0x5c00, 0x7c00};
	u32 ftable_base_reg[TSSI_PATH_MAX_8852A] = {0x5810, 0x7810};
	u32 ftable_trigger_reg[TSSI_PATH_MAX_8852A] = {0x5864, 0x7864};
	s8 thermal_offset[64] = {0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	halrf_wreg_fw(rf, ftable_base_reg[path], 0x00010000, 0x0);
	halrf_wreg_fw(rf, ftable_base_reg[path], 0x01000000, 0x1);

	halrf_wreg_fw(rf, ftable_base_reg[path], 0x0000fc00, 32);
	halrf_wreg_fw(rf, ftable_trigger_reg[path], 0x03f00000, 32);

	for (i = 0; i < 64; i = i + 4) {
		thermal_offset_tmp = (thermal_offset[i] & 0xff) |
				(thermal_offset[i + 1] & 0xff) << 8 |
				(thermal_offset[i + 2] & 0xff) << 16 |
				(thermal_offset[i + 3] & 0xff) << 24;

		halrf_wreg_fw(rf, (ftable_reg[path] + i), MASKDWORD, thermal_offset_tmp);
		
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI] write addr:0x%x value=0x%08x\n",
		       (ftable_reg[path] + i), thermal_offset_tmp);
	}

	halrf_wreg_fw(rf, ftable_trigger_reg[path], BIT(26), 0x1);
	halrf_wreg_fw(rf, ftable_trigger_reg[path], BIT(26), 0x0);

}

void _halrf_tssi_set_dac_gain_tbl_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u32 bb_version;

	bb_version = halrf_rreg(rf, 0xf4, 0x000000ff);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   bb_version=%d   path=%d\n",
			__func__, bb_version, path);

	if (bb_version >= 0x28)
		return;

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x58b0, 0x00000fff, 0x000);
		halrf_wreg_fw(rf, 0x58b0, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x5a00, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a04, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a08, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a0c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a10, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a14, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a18, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a1c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a20, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a24, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a28, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a2c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a30, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a34, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a38, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a3c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a40, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a44, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a48, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a4c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a50, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a54, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a58, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a5c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a60, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a64, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a68, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a6c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a70, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a74, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a78, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a7c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a80, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a84, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a88, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a8c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a90, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a94, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5a9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5aa0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5aa4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5aa8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5aac, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5ab0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5ab4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5ab8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5abc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x5ac0, MASKDWORD, 0x00000000);
	} else {
		halrf_wreg_fw(rf, 0x78b0, 0x00000fff, 0x000);
		halrf_wreg_fw(rf, 0x78b0, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x7a00, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a04, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a08, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a0c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a10, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a14, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a18, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a1c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a20, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a24, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a28, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a2c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a30, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a34, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a38, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a3c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a40, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a44, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a48, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a4c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a50, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a54, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a58, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a5c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a60, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a64, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a68, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a6c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a70, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a74, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a78, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a7c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a80, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a84, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a88, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a8c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a90, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a94, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a98, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7a9c, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aa8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7aac, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab0, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab4, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ab8, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7abc, MASKDWORD, 0x00000000);
		halrf_wreg_fw(rf, 0x7ac0, MASKDWORD, 0x00000000);
	}

}

void _halrf_tssi_tssi_slope_cal_org_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x581c, 0x00100000, 0x0);
		halrf_wreg_fw(rf, 0x58cc, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x58cc, 0x00000007, 0x0);
		halrf_wreg_fw(rf, 0x58cc, 0x00000038, 0x1);
		halrf_wreg_fw(rf, 0x58cc, 0x000001c0, 0x2);
		halrf_wreg_fw(rf, 0x58cc, 0x00000e00, 0x3);
		halrf_wreg_fw(rf, 0x5828, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x5898, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x5830, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x5898, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x5838, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x5898, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x5840, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x5898, 0xff000000, 0x40);
		halrf_wreg_fw(rf, 0x5848, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x589c, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x5850, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x589c, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x5858, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x589c, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x5860, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x589c, 0xff000000, 0x40);
	} else {
		halrf_wreg_fw(rf, 0x781c, 0x00100000, 0x0);
		halrf_wreg_fw(rf, 0x78cc, 0x00001000, 0x1);
		halrf_wreg_fw(rf, 0x78cc, 0x00000007, 0x0);
		halrf_wreg_fw(rf, 0x78cc, 0x00000038, 0x1);
		halrf_wreg_fw(rf, 0x78cc, 0x000001c0, 0x2);
		halrf_wreg_fw(rf, 0x78cc, 0x00000e00, 0x3);
		halrf_wreg_fw(rf, 0x7828, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7898, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7830, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7898, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7838, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7898, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7840, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x7898, 0xff000000, 0x40);
		halrf_wreg_fw(rf, 0x7848, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x789c, 0x000000ff, 0x40);
		halrf_wreg_fw(rf, 0x7850, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x789c, 0x0000ff00, 0x40);
		halrf_wreg_fw(rf, 0x7878, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x789c, 0x00ff0000, 0x40);
		halrf_wreg_fw(rf, 0x7860, 0x7fc00000, 0x040);
		halrf_wreg_fw(rf, 0x789c, 0xff000000, 0x40);
	}
}


void _halrf_tssi_tssi_slope_cal_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	s16 power_2g_high[4] = {80, 24, 0, -24};
	s16 power_2g_low[4] = {56, 8, -16, -36};
	s16 power_5g_high[4] = {80, 24, 0, -36};
	s16 power_5g_low[4] = {56, 8, -16, -44};
	s16 power_high[4] = {0}, power_low[4] = {0};
	u32 tssi_cw_rpt_high = 0, tssi_cw_rpt_low = 0,
		tssi_cw_rpt_offset[4] = {0};

	u32 tssi_trigger[TSSI_PATH_MAX_8852A] = {0x5820, 0x7820};
	u32 tssi_cw_rpt_addr[TSSI_PATH_MAX_8852A] = {0x1c18, 0x3c18};

	u8 i, j, k;
	u32 rate = T_HT_MF;
	u8 bw = 0;
	u8 phy_map;

	phy_map = (BIT(phy) << 4) | BIT(path);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	if (channel >= 1 && channel <= 14) {
		for (i = 0; i < 4; i++) {
			power_high[i] = power_2g_high[i];
			power_low[i] = power_2g_low[i];
		}
	} else {
		for (i = 0; i < 4; i++) {
			power_high[i] = power_5g_high[i];
			power_low[i] = power_5g_low[i];
		}
	}

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_START);
	for (j = 0; j < 4; j++) {
		/*high power*/
		halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x0);
		halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x1);

		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power_high[j], rate, bw, true);
		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power_high[j], rate, bw, false);
		
		for (k = 0; halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0; k++) {
			halrf_delay_us(rf, 1);

			if (k > 100) {
				//halrf_set_pseudo_cw(rf, i, power_high[j], false);
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] TSSI finish bit k > 100us path=%d\n",
					i);
				return;
			}
		}
		tssi_cw_rpt_high = halrf_rreg(rf, tssi_cw_rpt_addr[path], 0x000001ff);

		/*low power*/
		halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x0);
		halrf_wreg_fw(rf, tssi_trigger[path], 0x80000000, 0x1);

		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power_low[j], rate, bw, true);
		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power_low[j], rate, bw, false);
		
		for (k = 0; halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0; k++) {
			halrf_delay_us(rf, 1);

			if (k > 100) {
				//halrf_set_pseudo_cw(rf, i, power_low[j], false);
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] TSSI finish bit k > 100us path=%d\n",
					i);
				return;
			}
		}
		tssi_cw_rpt_low = halrf_rreg(rf, tssi_cw_rpt_addr[path], 0x000001ff);

		tssi_cw_rpt_offset[j] = tssi_cw_rpt_high - tssi_cw_rpt_low;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] power_high[%d]=%d  power_low[%d]=%d\n", 
			j, power_high[j], j, power_low[j]);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] tssi_cw_rpt_offset[%d](0x%x) = tssi_cw_rpt_high(0x%x) - tssi_cw_rpt_low(0x%x)\n", 
			j, tssi_cw_rpt_offset[j], tssi_cw_rpt_high, tssi_cw_rpt_low);

		//halrf_set_pseudo_cw(rf, i, power_low[j], false);

	}

	halrf_wreg_fw(rf, 0x581c, 0x00100000, 0x1);
	halrf_wreg_fw(rf, 0x58cc, 0x00001000, 0x1);
	halrf_wreg_fw(rf, 0x58cc, 0x00000007, 0x7);
	halrf_wreg_fw(rf, 0x58cc, 0x00000038, 0x6);
	halrf_wreg_fw(rf, 0x58cc, 0x000001c0, 0x3);
	halrf_wreg_fw(rf, 0x58cc, 0x00000e00, 0x1);

	halrf_wreg_fw(rf, 0x5828, 0x7fc00000, tssi_cw_rpt_offset[0]);
	halrf_wreg_fw(rf, 0x5898, 0x000000ff, power_high[0] - power_low[0]);

	halrf_wreg_fw(rf, 0x5830, 0x7fc00000, tssi_cw_rpt_offset[0]);
	halrf_wreg_fw(rf, 0x5898, 0x0000ff00, power_high[0] - power_low[0]);

	halrf_wreg_fw(rf, 0x5838, 0x7fc00000, tssi_cw_rpt_offset[1]);
	halrf_wreg_fw(rf, 0x5898, 0x00ff0000, power_high[1] - power_low[1]);

	halrf_wreg_fw(rf, 0x5840, 0x7fc00000, tssi_cw_rpt_offset[1]);
	halrf_wreg_fw(rf, 0x5898, 0xff000000, power_high[1] - power_low[1]);

	halrf_wreg_fw(rf, 0x5848, 0x7fc00000, tssi_cw_rpt_offset[2]);
	halrf_wreg_fw(rf, 0x589c, 0x000000ff, power_high[2] - power_low[2]);

	halrf_wreg_fw(rf, 0x5850, 0x7fc00000, tssi_cw_rpt_offset[2]);
	halrf_wreg_fw(rf, 0x589c, 0x0000ff00, power_high[2] - power_low[2]);

	halrf_wreg_fw(rf, 0x5858, 0x7fc00000, tssi_cw_rpt_offset[3]);
	halrf_wreg_fw(rf, 0x589c, 0x00ff0000, power_high[3] - power_low[3]);

	halrf_wreg_fw(rf, 0x5860, 0x7fc00000, tssi_cw_rpt_offset[3]);
	halrf_wreg_fw(rf, 0x589c, 0xff000000, power_high[3] - power_low[3]);

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_ONESHOT_STOP);

}

void _halrf_tssi_set_rf_gap_tbl_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x58f8, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x58f8, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x78f8, 0x000003ff, 0x000);
		halrf_wreg_fw(rf, 0x78f8, 0x000ffc00, 0x000);
		halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
		halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x000);
		halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x000);
	}

}

void _halrf_tssi_set_tssi_slope_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5818, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x581c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x5820, 0x0000f000, 0x1);
		halrf_wreg_fw(rf, 0x581c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x581c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x58b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x58b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x58b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x58b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x58b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x5810, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x580c, 0x10000000, 0x1);
		halrf_wreg_fw(rf, 0x580c, 0x40000000, 0x1);

		halrf_wreg_fw(rf, 0x5838, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5858, 0x003ff000, 0x000);

		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x121);
		}
	
		halrf_wreg_fw(rf, 0x5828, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5830, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5840, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5848, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5850, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x5860, 0x003ff000, 0x000);
	} else {
		halrf_wreg_fw(rf, 0x7820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7818, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x781c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7820, 0x0000f000, 0x1);
		halrf_wreg_fw(rf, 0x781c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x781c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x78b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x78b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x78b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x78b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x78b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7810, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x780c, 0x10000000, 0x1);
		halrf_wreg_fw(rf, 0x780c, 0x40000000, 0x1);

		halrf_wreg_fw(rf, 0x7838, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7858, 0x003ff000, 0x000);

		if (rfe_type <= 50) {
			halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x121);
			halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x115f2);
			halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
			halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x121);
		}

		halrf_wreg_fw(rf, 0x7828, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7830, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7840, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7848, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7850, 0x003ff000, 0x000);

		halrf_wreg_fw(rf, 0x7860, 0x003ff000, 0x000);
	}

}

void _halrf_tssi_efem_set_tssi_slope_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   rfe_type=%d\n",
		__func__, rfe_type);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5818, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x581c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x5820, 0x0000f000, 0x1);
		halrf_wreg_fw(rf, 0x581c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x581c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x58b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x58b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x58b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x58b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x58b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x5810, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x580c, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x580c, 0x40000000, 0x1);

		if (rfe_type == 51 || rfe_type == 53) {
			if (channel >= 1 && channel <= 14) {
				/*SKY85333*/
				halrf_wrf_fw(rf, RF_PATH_A, 0xdf, 0x00400, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_A, 0x6e, 0xfffff, 0x45a7c);
				halrf_wreg_fw(rf, 0x5820, 0x40000000, 0x1);
				//halrf_wreg_fw(rf, 0x5814, 0x003c0000, 0x3);
				//halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0x3);
				//halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0x3);
				//halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x00654);
				//halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x00654);
				//halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x00654);
				//halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x006e2);
				//halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x0c6);
				//halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x006e2);
				//halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x0c6);
				//halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x006e2);
				//halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x0c6);
				//halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x00654);
				//halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x0bd);
				//halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x00654);
				//halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x0bd);
			} else {
				/*SKY85747*/
				halrf_wrf_fw(rf, RF_PATH_A, 0xdf, 0x00200, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_A, 0x6f, 0xfffff, 0x56a7c);
				halrf_wreg_fw(rf, 0x5820, 0x40000000, 0x1);
				//halrf_wreg_fw(rf, 0x5814, 0x003c0000, 0x0);
				//halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x115);
				//halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x115);
				//halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x115);
				//halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x122);
				//halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x122);
				//halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x001c0);
				//halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x122);
				//halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x121);
				//halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x121);
			}
		}

		if (rfe_type == 52 || rfe_type == 54) {
			if (channel >= 1 && channel <= 14) {
				/*RTC66204*/
				halrf_wrf_fw(rf, RF_PATH_A, 0xdf, 0x00400, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_A, 0x6e, 0xfffff, 0x55a7c);
				halrf_wreg_fw(rf, 0x5820, 0x40000000, 0x0);
				//halrf_wreg_fw(rf, 0x5814, 0x003c0000, 0x2);
				//halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x1e8);				
			} else {
				/*RTC66504*/
				halrf_wrf_fw(rf, RF_PATH_A, 0xdf, 0x00200, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_A, 0x6f, 0xfffff, 0x45a7c);
				halrf_wreg_fw(rf, 0x5820, 0x40000000, 0x0);
				//halrf_wreg_fw(rf, 0x5814, 0x003c0000, 0x2);
				//halrf_wreg_fw(rf, 0x581c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x581c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x5824, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x5824, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5828, 0x00000fff, 0x225);
				//halrf_wreg_fw(rf, 0x582c, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x582c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5830, 0x00000fff, 0x225);
				//halrf_wreg_fw(rf, 0x5834, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x5834, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5838, 0x00000fff, 0x225);
				//halrf_wreg_fw(rf, 0x583c, 0x0003ffff, 0x20845);
				//halrf_wreg_fw(rf, 0x583c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5840, 0x00000fff, 0x231);
				//halrf_wreg_fw(rf, 0x5844, 0x0003ffff, 0x20845);
				//halrf_wreg_fw(rf, 0x5844, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5848, 0x00000fff, 0x231);
				//halrf_wreg_fw(rf, 0x584c, 0x0003ffff, 0x20845);
				//halrf_wreg_fw(rf, 0x584c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x5850, 0x00000fff, 0x231);
				//halrf_wreg_fw(rf, 0x5854, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x5854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5858, 0x00000fff, 0x121);
				//halrf_wreg_fw(rf, 0x585c, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x585c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x5860, 0x00000fff, 0x121);
			}
		}
	} else {
		halrf_wreg_fw(rf, 0x7820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7818, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x00000800, 0x1);
		halrf_wreg_fw(rf, 0x781c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7820, 0x0000f000, 0x1);
		halrf_wreg_fw(rf, 0x781c, 0x000003ff, 0x280);
		halrf_wreg_fw(rf, 0x781c, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x78b8, 0x007f0000, 0x00);
		halrf_wreg_fw(rf, 0x78b8, 0x7f000000, 0x00);
		halrf_wreg_fw(rf, 0x78b4, 0x7f000000, 0x0a);
		halrf_wreg_fw(rf, 0x78b8, 0x0000007f, 0x28);
		halrf_wreg_fw(rf, 0x78b8, 0x00007f00, 0x76);
		halrf_wreg_fw(rf, 0x7810, 0x20000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x780c, 0x10000000, 0x0);
		halrf_wreg_fw(rf, 0x780c, 0x40000000, 0x1);

		if (rfe_type == 51 || rfe_type == 53) {
			if (channel >= 1 && channel <= 14) {
				/*SKY85333*/
				halrf_wrf_fw(rf, RF_PATH_B, 0xdf, 0x00400, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_B, 0x6e, 0xfffff, 0x45a7c);
				halrf_wreg_fw(rf, 0x7820, 0x40000000, 0x1);
				//halrf_wreg_fw(rf, 0x7814, 0x003c0000, 0x3);
				//halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0x3);
				//halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0x3);
				//halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x0078f);
				//halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x0078f);
				//halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x0078f);
				//halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x0c0);
				//halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x00815);
				//halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x0c7);
				//halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x00815);
				//halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x0c7);
				//halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x00815);
				//halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x0c7);
				//halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x0078f);
				//halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x0bd);
				//halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x0078f);
				//halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x0bd);
			} else {
				/*SKY85747*/
				halrf_wrf_fw(rf, RF_PATH_B, 0xdf, 0x00200, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_B, 0x6f, 0xfffff, 0x56a7c);
				halrf_wreg_fw(rf, 0x7820, 0x40000000, 0x1);
				//halrf_wreg_fw(rf, 0x7814, 0x003c0000, 0x0);
				//halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x110);
				//halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x110);
				//halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x110);
				//halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x11e);
				//halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x11e);
				//halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x0019f);
				//halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x11e);
				//halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x121);
				//halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x121);						
			}
		}

		if (rfe_type == 52 || rfe_type == 54) {
			if (channel >= 1 && channel <= 14) {
				/*RTC66024*/
				halrf_wrf_fw(rf, RF_PATH_B, 0xdf, 0x00400, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_B, 0x6e, 0xfffff, 0x55a7c);
				halrf_wreg_fw(rf, 0x7820, 0x40000000, 0x0);
				//halrf_wreg_fw(rf, 0x7814, 0x003c0000, 0x2);
				//halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x1e8);
				//halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x19b08);
				//halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x1e8);					
			} else {
				/*RTC66504*/
				halrf_wrf_fw(rf, RF_PATH_B, 0xdf, 0x00200, 0x1);
				//halrf_wrf_fw(rf, RF_PATH_B, 0x6f, 0xfffff, 0x45a7c);
				halrf_wreg_fw(rf, 0x7820, 0x40000000, 0x0);
				//halrf_wreg_fw(rf, 0x7814, 0x003c0000, 0x2);
				//halrf_wreg_fw(rf, 0x781c, 0x01e00000, 0x4);
				//halrf_wreg_fw(rf, 0x781c, 0x1e000000, 0x4);
				//halrf_wreg_fw(rf, 0x7824, 0x0003ffff, 0x1eeba);
				//halrf_wreg_fw(rf, 0x7824, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7828, 0x00000fff, 0x21c);
				//halrf_wreg_fw(rf, 0x782c, 0x0003ffff, 0x1eeba);
				//halrf_wreg_fw(rf, 0x782c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7830, 0x00000fff, 0x21c);
				//halrf_wreg_fw(rf, 0x7834, 0x0003ffff, 0x1eeba);
				//halrf_wreg_fw(rf, 0x7834, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7838, 0x00000fff, 0x21c);
				//halrf_wreg_fw(rf, 0x783c, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x783c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7840, 0x00000fff, 0x227);
				//halrf_wreg_fw(rf, 0x7844, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x7844, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7848, 0x00000fff, 0x227);
				//halrf_wreg_fw(rf, 0x784c, 0x0003ffff, 0x1fa43);
				//halrf_wreg_fw(rf, 0x784c, 0x3ffc0000, 0xffb);
				//halrf_wreg_fw(rf, 0x7850, 0x00000fff, 0x227);
				//halrf_wreg_fw(rf, 0x7854, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x7854, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7858, 0x00000fff, 0x121);
				//halrf_wreg_fw(rf, 0x785c, 0x0003ffff, 0x115f2);
				//halrf_wreg_fw(rf, 0x785c, 0x3ffc0000, 0x000);
				//halrf_wreg_fw(rf, 0x7860, 0x00000fff, 0x121);
			}
		}
	}
}


void _halrf_tssi_set_tssi_track_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5818, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x5814, 0x00000800, 0x0);
		halrf_wreg_fw(rf, 0x581c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x5864, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x5864, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x5820, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x5814, 0x01000000, 0x0);
	} else {
		halrf_wreg_fw(rf, 0x7820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7818, 0x18000000, 0x0);
		halrf_wreg_fw(rf, 0x7814, 0x00000800, 0x0);
		halrf_wreg_fw(rf, 0x781c, 0x20000000, 0x1);
		halrf_wreg_fw(rf, 0x7864, 0x000003ff, 0x1ff);
		halrf_wreg_fw(rf, 0x7864, 0x000ffc00, 0x200);
		halrf_wreg_fw(rf, 0x7820, 0x00000fff, 0x080);
		halrf_wreg_fw(rf, 0x7814, 0x01000000, 0x0);
	}
}

void _halrf_tssi_set_txagc_offset_mv_avg_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 txagc_offset_mv_avg_rpt_sel[TSSI_PATH_MAX_8852A] = {0x58e4, 0x78e4};
	
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
		if (tssi_info->tssi_type == TSSI_ON) {
			if (rf->phl_com->dev_cap.rfe_type >= 50)
				halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00003800, 0x2);
			else
				halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00003800, 0x4);
		} else if (tssi_info->tssi_type == TSSI_CAL)
			halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00003800, 0x0);
	}

	halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00004000, 0x0);
	halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00004000, 0x1);
	halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00004000, 0x0);
	halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x00008000, 0x0);
	halrf_wreg_fw(rf, txagc_offset_mv_avg_rpt_sel[path], 0x000f0000, 0x0);
}

void _halrf_tssi_run_tssi_slope_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n", __func__, path);

	if (path == RF_PATH_A) {
		halrf_wreg_fw(rf, 0x5820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x5820, 0x80000000, 0x1);
	} else {
		halrf_wreg_fw(rf, 0x7820, 0x80000000, 0x0);
		halrf_wreg_fw(rf, 0x7820, 0x80000000, 0x1);
	}
}

void _halrf_tssi_tssi_pak_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{		
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	if (path == RF_PATH_A) {
		if (channel >= 1 && channel <= 14) {
			halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x1d0);
			halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x1e8);
			halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x00b);
			halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x088);
		} else if (channel >= 36 && channel <= 64) {
			halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x1d7);
			halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x1fb);
			halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x005);
			halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x087);
		} else if (channel >= 100 && channel <= 144) {
			halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x1d8);
			halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x1fc);
			halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x006);
			halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x084);
		} else if (channel >= 149 && channel <= 177) {
			halrf_wreg_fw(rf, 0x5814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x58f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x58a4, 0x03fe0000, 0x1e5);
			halrf_wreg_fw(rf, 0x58a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58a8, 0x0003fe00, 0x00a);
			halrf_wreg_fw(rf, 0x58a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x58ac, 0x0003fe00, 0x011);
			halrf_wreg_fw(rf, 0x58ac, 0x07fc0000, 0x083);
		}
	} else {
		if (channel >= 1 && channel <= 14) {
			halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x1cc);
			halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x1e2);
			halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x005);
			halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x089);
		} else if (channel >= 36 && channel <= 64) {	
			halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x1d5);
			halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x1fc);
			halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x005);
			halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x083);
		} else if (channel >= 100 && channel <= 144) {
			halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x1dc);
			halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x002);
			halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x00b);
			halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x082);
		} else if (channel >= 149 && channel <= 177) {
			halrf_wreg_fw(rf, 0x7814, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f4, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000003ff, 0x000);
			halrf_wreg_fw(rf, 0x78f8, 0x000ffc00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x0001ff00, 0x000);
			halrf_wreg_fw(rf, 0x78a4, 0x03fe0000, 0x1f0);
			halrf_wreg_fw(rf, 0x78a8, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78a8, 0x0003fe00, 0x016);
			halrf_wreg_fw(rf, 0x78a8, 0x07fc0000, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x000001ff, 0x000);
			halrf_wreg_fw(rf, 0x78ac, 0x0003fe00, 0x01f);
			halrf_wreg_fw(rf, 0x78ac, 0x07fc0000, 0x080);
		}
	}

}

void _halrf_tssi_tssi_alimentk_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
//	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	s16 power_2g[4] = {40, 16, 4, 4};
	s16 power_5g[4] = {40, 16, 4, 4};
	s16 power[4] = {0};
	s32 tssi_alim_offset[4] = {0};
	s32 aliment_diff, tssi_cw_default;
	u32 tssi_cw_rpt[4] = {0}, tx_couter, tx_couter_tmp, tmp, retry;

	u32 tssi_trigger[TSSI_PATH_MAX_8852A] = {0x5820, 0x7820};
	u32 tssi_cw_rpt_addr[TSSI_PATH_MAX_8852A] = {0x1c18, 0x3c18};

	//u32 tssi_cw_default[TSSI_PATH_MAX_8852A][4] =
	//			{{0xb, 0x5, 0x6, 0x11},
	//			{0x5, 0x5, 0xb, 0x1f}};

	u32 tssi_cw_default_addr[TSSI_PATH_MAX_8852A][4] =
					{{0x58ac, 0x58ac, 0x58a8, 0x58a4},
					{0x78ac, 0x78ac, 0x78a8, 0x78a4}};
	
	u32 tssi_cw_default_mask[4][4] =
			{{0x07fc0000, 0x000001ff, 0x0003fe00, 0x03fe0000},
			{0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000},
			{0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000},
			{0x07fc0000, 0x0003fe00, 0x0003fe00, 0x03fe0000}};

	u32 bb_reg[5] = {0x5820, 0x7820, 0x4978, 0x58e4, 0x78e4};
	u32 bb_reg_backup[5] = {0};
	u32 backup_num = 5;

	u8 tssi_cw_group = 0;

	u8 i, j, k;
	u32 rate = T_HT_MF;
	u8 bw = 0;
	u8 phy_map;

	phy_map = (BIT(phy) << 4) | BIT(path);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   channel=%d   path=%d\n",
		__func__, channel, path);

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP)
		halrf_hal_bb_backup_info(rf, phy);

	if (channel >= 1 && channel <= 14)
		tssi_cw_group = 0;
	else if (channel >= 36 && channel <= 64)
		tssi_cw_group = 1;
	else if (channel >= 100 && channel <= 144)
		tssi_cw_group = 2;
	else if (channel >= 149 && channel <= 177)
		tssi_cw_group = 3;

	if (channel >= 1 && channel <= 14) {
		for (i = 0; i < 4; i++) {
			power[i] = power_2g[i];
		}
	} else {
		for (i = 0; i < 4; i++) {
			power[i] = power_5g[i];
		}
	}

	_tssi_backup_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	halrf_wreg(rf, 0x5820, 0x0000f000, 0x4);
	halrf_wreg(rf, 0x7820, 0x0000f000, 0x4);

	halrf_wreg(rf, 0x58e4, 0x00003800, 0x2);
	halrf_wreg(rf, 0x78e4, 0x00003800, 0x2);

	//halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_START);
	for (j = 0; j < 2; j++) {
		halrf_wreg(rf, tssi_trigger[path], 0x80000000, 0x0);
		halrf_wreg(rf, tssi_trigger[path], 0x80000000, 0x1);

		tx_couter = halrf_rreg(rf, 0x1a40, 0xffff);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] 0x%x = 0x%08x   path=%d\n",
			tssi_trigger[path],
			halrf_rreg(rf, tssi_trigger[path], 0xffffffff), path);

		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power[j], rate, bw, true);

		//halrf_delay_ms(rf, 1500);

		tx_couter_tmp = halrf_rreg(rf, 0x1a40, 0xffff) - tx_couter;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] First HWTXcounter=%d path=%d\n",
			tx_couter_tmp, path);

		//for (k = 0; (halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0 || tx_couter_tmp < 20); k++) {
		for (k = 0; halrf_rreg(rf, tssi_cw_rpt_addr[path], BIT(16)) == 0; k++) {
			if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
				halrf_delay_ms(rf, 1);
				retry = 30;
			} else {
				halrf_delay_us(rf, 30);
				retry = 100;
			}

			tx_couter_tmp = halrf_rreg(rf, 0x1a40, 0xffff) - tx_couter;

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] Flow k = %d HWTXcounter=%d path=%d\n",
					k, tx_couter_tmp, path);

			if (k > retry) {
				RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI PA K] TSSI finish bit k > %d mp:100ms normal:30us path=%d\n",
					k, path);
				_tssi_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);
				_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power[j], rate, bw, false);
				return;
			}
		}

		tssi_cw_rpt[j] = halrf_rreg(rf, tssi_cw_rpt_addr[path], 0x000001ff);

		_halrf_tssi_hw_tx_8852a(rf, phy, path, 20, power[j], rate, bw, false);		

		tx_couter = halrf_rreg(rf, 0x1a40, 0xffff) - tx_couter;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] Fianl HWTXcounter = %d path=%d\n",
			tx_couter, path);

	}

#if 0
	/*tssi_cw_rpt_offset[0] = 308 - tssi_cw_rpt[0];*/
	tssi_cw_rpt_offset[1] = (s32)tssi_cw_rpt[0] - ((power[0] - power[1]) * 2) -
				(s32)tssi_cw_rpt[1] + (s32)tssi_cw_default[path][tssi_cw_group] + 
				tssi_info->txagc_offset_k_diff[path];
	//tssi_cw_rpt_offset[2] = (s32)tssi_cw_rpt_offset[1] - ((power[1] - power[2]) * 2) -
	//			(s32)tssi_cw_rpt[2] + (s32)tssi_cw_default[path][tssi_cw_group];
	//tssi_cw_rpt_offset[3] = (s32)tssi_cw_rpt_offset[2] - ((power[2] - power[3]) * 2) -
	//			(s32)tssi_cw_rpt[3] + (s32)tssi_cw_default[path][tssi_cw_group];

	tssi_cw_rpt_offset[2] = (s32)tssi_cw_rpt[0] - ((power[0] - power[2]) * 2) -
				(s32)tssi_cw_rpt[2] + (s32)tssi_cw_default[path][tssi_cw_group] + 
				tssi_info->txagc_offset_k_diff[path];
	tssi_cw_rpt_offset[3] = (s32)tssi_cw_rpt[0] - ((power[0] - power[3]) * 2) -
				(s32)tssi_cw_rpt[3] + (s32)tssi_cw_default[path][tssi_cw_group] + 
				tssi_info->txagc_offset_k_diff[path];
#endif
	/*tssi_cw_rpt_offset[0] = 308 - tssi_cw_rpt[0];*/

	/*tssi_alim_offset[1]*/
	//tssi_alim_offset[1] = 308 - ((68 - power[0]) * 2) - tssi_cw_rpt[0];

	for (j = 0; j < 2; j++) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] power[%d]=%d  tssi_cw_rpt[%d]=%d\n",
			j, power[j], j, tssi_cw_rpt[j]);
	}

	tmp = halrf_rreg(rf, tssi_cw_default_addr[path][1],
					tssi_cw_default_mask[tssi_cw_group][1]);

	if (tmp & BIT(8))
		tssi_cw_default = tmp | 0xfffffe00;
	else
		tssi_cw_default = tmp;

	tssi_alim_offset[1] = tssi_cw_rpt[0] - ((power[0] - power[1]) * 2) - tssi_cw_rpt[1] + tssi_cw_default;

	aliment_diff = tssi_alim_offset[1] - tssi_cw_default;		
		
	/*tssi_alim_offset[2]*/
	tmp = halrf_rreg(rf, tssi_cw_default_addr[path][2],
					tssi_cw_default_mask[tssi_cw_group][2]);
	
	if (tmp & BIT(8))
		tssi_cw_default = tmp | 0xfffffe00;
	else
		tssi_cw_default = tmp;

	tssi_alim_offset[2] = tssi_cw_default + aliment_diff;

	/*tssi_alim_offset[3]*/
	tmp = halrf_rreg(rf, tssi_cw_default_addr[path][3],
					tssi_cw_default_mask[tssi_cw_group][3]);
	
	if (tmp & BIT(8))
		tssi_cw_default = tmp | 0xfffffe00;
	else
		tssi_cw_default = tmp;

	tssi_alim_offset[3] = tssi_cw_default + aliment_diff;

	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x5814, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x58f4, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x58f4, 0x000ffc00, 0x000);
		halrf_wreg(rf, 0x58f8, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x58f8, 0x000ffc00, 0x000);
		halrf_wreg(rf, 0x58a4, 0x0001ff00, 0x000);
		halrf_wreg(rf, 0x58a4, 0x03fe0000, (tssi_alim_offset[3] & 0x1ff));
		halrf_wreg(rf, 0x58a8, 0x000001ff, 0x000);
		halrf_wreg(rf, 0x58a8, 0x0003fe00, (tssi_alim_offset[2] & 0x1ff));
		halrf_wreg(rf, 0x58a8, 0x07fc0000, 0x000);

		if (channel >= 1 && channel <= 14) {
			halrf_wreg(rf, 0x58ac, 0x000001ff, (tssi_alim_offset[1] & 0x1ff));
			halrf_wreg(rf, 0x58ac, 0x0003fe00, 0x000);
		} else {
			halrf_wreg(rf, 0x58ac, 0x000001ff, 0x000);
			halrf_wreg(rf, 0x58ac, 0x0003fe00, (tssi_alim_offset[1] & 0x1ff));
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] tssi_alim_offset = 0x%x   5G:0x%x   2G:0x%x   0x%x   0x%x\n",
			halrf_rreg(rf, 0x58ac, 0x07fc0000),
			halrf_rreg(rf, 0x58ac, 0x0003fe00),
			halrf_rreg(rf, 0x58ac, 0x000001ff),
			halrf_rreg(rf, 0x58a8, 0x0003fe00),
			halrf_rreg(rf, 0x58a4, 0x03fe0000)
			);
	} else {
		halrf_wreg(rf, 0x7814, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x78f4, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x78f4, 0x000ffc00, 0x000);
		halrf_wreg(rf, 0x78f8, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x78f8, 0x000ffc00, 0x000);
		halrf_wreg(rf, 0x78a4, 0x0001ff00, 0x000);
		halrf_wreg(rf, 0x78a4, 0x03fe0000, (tssi_alim_offset[3] & 0x3ff));
		halrf_wreg(rf, 0x78a8, 0x000001ff, 0x000);
		halrf_wreg(rf, 0x78a8, 0x0003fe00, (tssi_alim_offset[2] & 0x3ff));
		halrf_wreg(rf, 0x78a8, 0x07fc0000, 0x000);

		if (channel >= 1 && channel <= 14) {
			halrf_wreg(rf, 0x78ac, 0x000001ff, (tssi_alim_offset[1] & 0x3ff));
			halrf_wreg(rf, 0x78ac, 0x0003fe00, 0x000);
		} else {
			halrf_wreg(rf, 0x78ac, 0x000001ff, 0x000);
			halrf_wreg(rf, 0x78ac, 0x0003fe00, (tssi_alim_offset[1] & 0x3ff));
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI PA K] tssi_alim_offset = 0x%x   5G:0x%x   2G:0x%x   0x%x   0x%x\n",
			halrf_rreg(rf, 0x78ac, 0x07fc0000),
			halrf_rreg(rf, 0x78ac, 0x0003fe00),
			halrf_rreg(rf, 0x78ac, 0x000001ff),
			halrf_rreg(rf, 0x78a8, 0x0003fe00),
			halrf_rreg(rf, 0x78a4, 0x03fe0000)
			);
	}

	_tssi_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
						backup_num);

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP)
		halrf_hal_bb_restore_info(rf, phy);
	else
		halrf_tx_mode_switch(rf, phy, 0);

	//halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_TSSI, RFK_STOP);

}

u32 _halrf_tssi_get_cck_efuse_group_8852ab(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 offset_index = 0;

	if (channel >= 1 && channel <= 2)
		offset_index = 0;
	else if (channel >= 3 && channel <= 5)
		offset_index = 1;
	else if (channel >= 6 && channel <= 8)
		offset_index = 2;
	else if (channel >= 9 && channel <= 11)
		offset_index = 3;
	else if (channel >= 12 && channel <= 13)
		offset_index = 4;
	else if (channel == 14)
		offset_index = 5;

	return offset_index;
}

u32 _halrf_tssi_get_ofdm_efuse_group_8852ab(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 offset_index = 0;

	if (channel >= 1 && channel <= 2)
		offset_index = 0;
	else if (channel >= 3 && channel <= 5)
		offset_index = 1;
	else if (channel >= 6 && channel <= 8)
		offset_index = 2;
	else if (channel >= 9 && channel <= 11)
		offset_index = 3;
	else if (channel >= 12 && channel <= 14)
		offset_index = 4;
	else if (channel >= 36 && channel <= 40)
		offset_index = 5;
	else if (channel >= 44 && channel <= 48)
		offset_index = 6;
	else if (channel >= 52 && channel <= 56)
		offset_index = 7;
	else if (channel >= 60 && channel <= 64)
		offset_index = 8;
	else if (channel >= 100 && channel <= 104)
		offset_index = 9;
	else if (channel >= 108 && channel <= 112)
		offset_index = 10;
	else if (channel >= 116 && channel <= 120)
		offset_index = 11;
	else if (channel >= 124 && channel <= 128)
		offset_index = 12;
	else if (channel >= 132 && channel <= 136)
		offset_index = 13;
	else if (channel >= 140 && channel <= 144)
		offset_index = 14;
	else if (channel >= 149 && channel <= 153)
		offset_index = 15;
	else if (channel >= 157 && channel <= 161)
		offset_index = 16;
	else if (channel >= 165 && channel <= 169)
		offset_index = 17;
	else if (channel >= 173 && channel <= 177)
		offset_index = 18;
	else if (channel > 40 && channel < 44)
		offset_index = 0x0506;
	else if (channel > 48  && channel < 52)
		offset_index = 0x0607;
	else if (channel > 56 && channel < 60)
		offset_index = 0x0708;
	else if (channel > 104 && channel < 108)
		offset_index = 0x090a;
	else if (channel > 112 && channel < 116)
		offset_index = 0x0a0b;
	else if (channel > 120 && channel < 124)
		offset_index = 0x0b0c;
	else if (channel > 128 && channel < 132)
		offset_index = 0x0c0d;
	else if (channel > 136 && channel < 140)
		offset_index = 0x0d0e;
	else if (channel > 153 && channel < 157)
		offset_index = 0x0f10;
	else if (channel > 161 && channel < 165)
		offset_index = 0x1011;
	else if (channel > 169 && channel < 173)
		offset_index = 0x1112;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI efuse] ======> %s offset_index=0x%x(%d) channel=%d\n",
			__func__, offset_index, offset_index, channel);

	return offset_index;
}

s8 _halrf_tssi_get_ofdm_efuse_tssi_de_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 group_idx;
	s8 first_de = 0, second_de = 0, final_de;

	group_idx = _halrf_tssi_get_ofdm_efuse_group_8852ab(rf, phy);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   group_idx=0x%x(%d)\n",
		__func__, group_idx, group_idx);

	if (group_idx >= 0x0506) {
		first_de = tssi_info->tssi_efuse[path][EFUSE_TSSI_MCS][group_idx >> 8];

		second_de = tssi_info->tssi_efuse[path][EFUSE_TSSI_MCS][group_idx & 0xff];

		final_de = (first_de + second_de) / 2;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI efuse] first_de=%d second_de=%d final_de=%d\n",
			first_de, second_de, final_de);
	} else {
		final_de = tssi_info->tssi_efuse[path][EFUSE_TSSI_MCS][group_idx];

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI efuse] final_de=%d\n",
			final_de);
	}

	return final_de;
}


u32 _halrf_tssi_get_tssi_trim_efuse_group_8852ab(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u32 group_index = 0;

	if (channel >= 1 && channel <= 8)
		group_index = 0;
	else if (channel >= 9 && channel <= 14)
		group_index = 1;
	else if (channel >= 36 && channel <= 48)
		group_index = 2;
	else if (channel >= 52 && channel <= 64)
		group_index = 3;
	else if (channel >= 100 && channel <= 112)
		group_index = 4;
	else if (channel >= 116 && channel <= 128)
		group_index = 5;
	else if (channel >= 132 && channel <= 144)
		group_index = 6;
	else if (channel >= 149 && channel <= 177)
		group_index = 7;
#if 0
	else if (channel > 48 && channel < 52)
		group_index = 0x0203;
	else if (channel > 120 && channel < 124)
		group_index = 0x0405;
	else if (channel > 161 && channel < 165)
		group_index = 0x0607;
#endif

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI Trim] ======> %s offset_index=0x%x(%d) channel=%d\n",
			__func__, group_index, group_index, channel);

	return group_index;
}

s8 _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 group_idx;
	s8 first_de = 0, second_de = 0, final_de;

	group_idx = _halrf_tssi_get_tssi_trim_efuse_group_8852ab(rf, phy);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI Trim] ======> %s   group_idx=0x%x(%d)\n",
		__func__, group_idx, group_idx);

	if (group_idx >= 0x0203) {
		first_de = tssi_info->tssi_trim[path][group_idx >> 8];

		second_de = tssi_info->tssi_trim[path][group_idx & 0xff];

		final_de = (first_de + second_de) / 2;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI Trim] Trim_first_de=%d Trim_second_de=%d Trim_final_de=%d\n",
			first_de, second_de, final_de);
	} else {
		final_de = tssi_info->tssi_trim[path][group_idx];

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI Trim] Trim_final_de=%d\n",
			final_de);
	}

	return final_de;
}

void halrf_tssi_set_efuse_to_de_8852ab(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;

	u32 addr_cck_long[2] = {0x5858, 0x7858};
	u32 addr_cck_short[2] = {0x5860, 0x7860};
	u32 addr_cck_bitmask[2] = {0x003ff000, 0x003ff000};
	u32 addr_mcs_20m[2] = {0x5838, 0x7838};
	u32 addr_mcs_40m[2] = {0x5840, 0x7840};
	u32 addr_mcs_80m[2] = {0x5848, 0x7848};
	u32 addr_mcs_80m_80m[2] = {0x5850, 0x7850};
	u32 addr_mcs_5m[2] = {0x5828, 0x7828};
	u32 addr_mcs_10m[2] = {0x5830, 0x7830};
	u32 addr_mcs_bitmask[2] = {0x003ff000, 0x003ff000};
	u32 i, group_idx;
	s32 tmp;
	s8 ofmd_de, tssi_trim;

	phy = HW_PHY_0;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy);

	if (tssi_info->tssi_type == TSSI_CAL) {
		/*halrf_set_tssi_de_offset_zero_8852a(rf, phy);*/
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s TSSI_CAL mode set TSSI Trim !!!\n", __func__);

		for (i = 0; i < TSSI_PATH_MAX_8852A; i++) {
			tssi_trim = _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(rf, phy, i);

			tmp = (s32)tssi_trim;

			halrf_wreg_fw(rf, addr_cck_long[i], addr_cck_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_cck_short[i], addr_cck_bitmask[i],
					(u32)(tmp & 0x3ff));

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] TSSI_CAL Set TSSI Trim CCK DE 0x%x[21:12]=0x%x\n",
				addr_cck_long[i], halrf_rreg(rf, addr_cck_long[i], addr_cck_bitmask[i]));

			halrf_wreg_fw(rf, addr_mcs_20m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_mcs_40m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_mcs_80m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_mcs_80m_80m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_mcs_5m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));
			halrf_wreg_fw(rf, addr_mcs_10m[i], addr_mcs_bitmask[i],
					(u32)(tmp & 0x3ff));

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] TSSI_CAL Set TSSI Trim MCS DE 0x%x[21:12]=0x%x\n",
				addr_mcs_20m[i], halrf_rreg(rf, addr_mcs_20m[i], addr_mcs_bitmask[i]));

			/*Save TSSI data for WINCLI*/
			tssi_info->curr_tssi_cck_de[i] =
				(s8)(halrf_rreg(rf, addr_cck_long[i], addr_cck_bitmask[i]) & 0xff);
			tssi_info->curr_tssi_ofdm_de[i] =
				(s8)(halrf_rreg(rf, addr_mcs_20m[i], addr_mcs_bitmask[i]) & 0xff);
			tssi_info->curr_tssi_efuse_cck_de[i] = 0;
			tssi_info->curr_tssi_efuse_ofdm_de[i] = 0;
			tssi_info->curr_tssi_trim_de[i] = tssi_trim;
		}
		return;
	}

	for (i = 0; i < TSSI_PATH_MAX_8852A; i++) {
		/*CCK*/
		group_idx = _halrf_tssi_get_cck_efuse_group_8852ab(rf, phy);
		tssi_trim = _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(rf, phy, i);

		tmp = tssi_info->tssi_efuse[i][EFUSE_TSSI_CCK][group_idx] +
			tssi_trim;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI] tmp(0x%x) = tssi_efuse_cck[%d](0x%x) + tssi_trim(0x%x) path=%d\n",
			tmp, group_idx, tssi_info->tssi_efuse[i][EFUSE_TSSI_CCK][group_idx],
			tssi_trim, i);

		halrf_wreg_fw(rf, addr_cck_long[i], addr_cck_bitmask[i],
				(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_cck_short[i], addr_cck_bitmask[i],
				(u32)(tmp & 0x3ff));

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI CCK DE 0x%x[21:12]=0x%x\n",
			addr_cck_long[i], halrf_rreg(rf, addr_cck_long[i], addr_cck_bitmask[i]));

		/*OFDM*/
		ofmd_de = _halrf_tssi_get_ofdm_efuse_tssi_de_8852ab(rf, phy, i);
		tssi_trim = _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(rf, phy, i);
		tmp = ofmd_de + tssi_trim;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"[TSSI] tmp(0x%x) = tssi_efuse_ofdm(0x%x) + tssi_trim(0x%x) path=%d\n",
			tmp, ofmd_de, tssi_trim, i);

		halrf_wreg_fw(rf, addr_mcs_20m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_mcs_40m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_mcs_80m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_mcs_80m_80m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_mcs_5m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));
		halrf_wreg_fw(rf, addr_mcs_10m[i], addr_mcs_bitmask[i],
			(u32)(tmp & 0x3ff));

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI MCS DE 0x%x[21:12]=0x%x\n",
			addr_mcs_20m[i], halrf_rreg(rf, addr_mcs_20m[i], addr_mcs_bitmask[i]));
		
		/*Save TSSI data for WINCLI*/
		tssi_info->curr_tssi_cck_de[i] =
			(s8)(halrf_rreg(rf, addr_cck_long[i], addr_cck_bitmask[i]) & 0xff);
		tssi_info->curr_tssi_ofdm_de[i] =
			(s8)(halrf_rreg(rf, addr_mcs_20m[i], addr_mcs_bitmask[i]) & 0xff);
		tssi_info->curr_tssi_efuse_cck_de[i] = tssi_info->tssi_efuse[i][EFUSE_TSSI_CCK][group_idx];
		tssi_info->curr_tssi_efuse_ofdm_de[i] = ofmd_de;
		tssi_info->curr_tssi_trim_de[i] = tssi_trim;
	}

}

void _halrf_set_tssi_de_for_tx_verify_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de, u8 path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 tssi_de_tmp;
	s32 s_tssi_de, tmp;
	s8 tssi_trim;
	u32 addr_cck_long[2] = {0x5858, 0x7858};
	u32 addr_cck_short[2] = {0x5860, 0x7860};
	u32 addr_cck_bitmask[2] = {0x003ff000, 0x003ff000};
	u32 addr_mcs_20m[2] = {0x5838, 0x7838};
	u32 addr_mcs_40m[2] = {0x5840, 0x7840};
	u32 addr_mcs_80m[2] = {0x5848, 0x7848};
	u32 addr_mcs_80m_80m[2] = {0x5850, 0x7850};
	u32 addr_mcs_5m[2] = {0x5828, 0x7828};
	u32 addr_mcs_10m[2] = {0x5830, 0x7830};
	u32 addr_mcs_bitmask[2] = {0x003ff000, 0x003ff000};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d   tssi_de=0x%x   path=%d\n",
		__func__, phy, tssi_de, path);

	if (tssi_de & BIT(7))
		s_tssi_de = tssi_de | 0xffffff00;
	else
		s_tssi_de = tssi_de;

	tssi_trim = _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(rf, phy, path);
	tmp = s_tssi_de + tssi_trim;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] tmp(%d) = s_tssi_de(%d) + tssi_trim(%d)\n",
		tmp, s_tssi_de, tssi_trim);

	tssi_de_tmp = tmp & 0x3ff;

	halrf_wreg(rf, addr_cck_long[path], addr_cck_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_cck_short[path], addr_cck_bitmask[path], tssi_de_tmp);

	halrf_wreg(rf, addr_mcs_20m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_mcs_40m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_mcs_80m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_mcs_80m_80m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_mcs_5m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	halrf_wreg(rf, addr_mcs_10m[path], addr_mcs_bitmask[path], tssi_de_tmp);
	tssi_info->tssi_de[path] = tmp;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x%x[21:12]=0x%x 0x%x[21:12]=0x%x path=%d\n",
		addr_mcs_20m[path],
		halrf_rreg(rf, addr_mcs_20m[path], 0x003ff000),
		addr_cck_long[path],
		halrf_rreg(rf, addr_cck_long[path], 0x003ff000),
		path);

	/*Save TSSI data for WINCLI*/
	tssi_info->curr_tssi_cck_de[path] =
		(s8)(halrf_rreg(rf, addr_cck_long[path], addr_cck_bitmask[path]) & 0xff);
	tssi_info->curr_tssi_ofdm_de[path] =
		(s8)(halrf_rreg(rf, addr_mcs_20m[path], addr_mcs_bitmask[path]) & 0xff);
	tssi_info->curr_tssi_efuse_cck_de[path] = (s8)s_tssi_de;
	tssi_info->curr_tssi_efuse_ofdm_de[path] = (s8)s_tssi_de;
	tssi_info->curr_tssi_trim_de[path] = tssi_trim;
}

void _halrf_set_tssi_de_offset_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de_offset, u8 path)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	s32 tssi_tmp, s_tssi_de_offset;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d   tssi_de_offset=0x%x   path=%d\n",
		__func__, phy, tssi_de_offset, path);

	if (tssi_de_offset & BIT(7))
		s_tssi_de_offset = tssi_de_offset | 0xffffff00;
	else
		s_tssi_de_offset = tssi_de_offset;

	s_tssi_de_offset = s_tssi_de_offset * -1;

	if (path == RF_PATH_A) {
		tssi_tmp = tssi_info->tssi_de[RF_PATH_A] + s_tssi_de_offset;
		tssi_tmp = tssi_tmp & 0x3ff;
		halrf_wreg(rf, 0x5838, 0x003ff000, tssi_tmp);	/*20M*/
		halrf_wreg(rf, 0x5858, 0x003ff000, tssi_tmp);	/*CCK long*/
		halrf_wreg(rf, 0x5828, 0x003ff000, tssi_tmp);	/*5M*/
		halrf_wreg(rf, 0x5830, 0x003ff000, tssi_tmp);	/*10M*/
		halrf_wreg(rf, 0x5840, 0x003ff000, tssi_tmp);	/*40M*/
		halrf_wreg(rf, 0x5848, 0x003ff000, tssi_tmp);	/*80M*/
		halrf_wreg(rf, 0x5850, 0x003ff000, tssi_tmp);	/*80M+80M*/
		halrf_wreg(rf, 0x5860, 0x003ff000, tssi_tmp);	/*CCK short*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] tssi_tmp(%d) = tssi_info->tssi_de[RF_PATH_A](%d) + s_tssi_de_offset(%d) path=%d\n",
			tssi_tmp, tssi_info->tssi_de[RF_PATH_A],
			s_tssi_de_offset, path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x5838[21:12]=0x%x 0x5858[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x5838, 0x003ff000),
			halrf_rreg(rf, 0x5858, 0x003ff000),
			path);
	} else {
		tssi_tmp = tssi_info->tssi_de[RF_PATH_B] + s_tssi_de_offset;
		tssi_tmp = tssi_tmp & 0x3ff;
		halrf_wreg(rf, 0x7838, 0x003ff000, tssi_tmp);	/*20M*/
		halrf_wreg(rf, 0x7858, 0x003ff000, tssi_tmp);	/*CCK long*/
		halrf_wreg(rf, 0x7828, 0x003ff000, tssi_tmp);	/*5M*/
		halrf_wreg(rf, 0x7830, 0x003ff000, tssi_tmp);	/*10M*/
		halrf_wreg(rf, 0x7840, 0x003ff000, tssi_tmp);	/*40M*/
		halrf_wreg(rf, 0x7848, 0x003ff000, tssi_tmp);	/*80M*/
		halrf_wreg(rf, 0x7850, 0x003ff000, tssi_tmp);	/*80M+80M*/
		halrf_wreg(rf, 0x7860, 0x003ff000, tssi_tmp);	/*CCK short*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] tssi_tmp(%d) = tssi_info->tssi_de[RF_PATH_B](%d) + s_tssi_de_offset(%d) path=%d\n",
			tssi_tmp, tssi_info->tssi_de[RF_PATH_B],
			s_tssi_de_offset, path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7838[21:12]=0x%x 0x7858[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7838, 0x003ff000),
			halrf_rreg(rf, 0x7858, 0x003ff000),
			path);
	}
}

void _halrf_set_tssi_de_offset_zero_8852ab(struct rf_info *rf,
							enum phl_phy_idx phy)
{
	halrf_wreg(rf, 0x5838, 0x003ff000, 0x0);	/*20M*/
	halrf_wreg(rf, 0x5858, 0x003ff000, 0x0);	/*CCK long*/
	halrf_wreg(rf, 0x5828, 0x003ff000, 0x0);	/*5M*/
	halrf_wreg(rf, 0x5830, 0x003ff000, 0x0);	/*10M*/
	halrf_wreg(rf, 0x5840, 0x003ff000, 0x0);	/*40M*/
	halrf_wreg(rf, 0x5848, 0x003ff000, 0x0);	/*80M*/
	halrf_wreg(rf, 0x5850, 0x003ff000, 0x0);	/*80M+80M*/
	halrf_wreg(rf, 0x5860, 0x003ff000, 0x0);	/*CCK short*/

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE Zero 0x5838[21:12]=0x%x 0x5858[21:12]=0x%x\n",
		halrf_rreg(rf, 0x5838, 0x003ff000),
		halrf_rreg(rf, 0x5858, 0x003ff000));

	halrf_wreg(rf, 0x7838, 0x003ff000, 0x0);	/*20M*/
	halrf_wreg(rf, 0x7858, 0x003ff000, 0x0);	/*CCK long*/
	halrf_wreg(rf, 0x7828, 0x003ff000, 0x0);	/*5M*/
	halrf_wreg(rf, 0x7830, 0x003ff000, 0x0);	/*10M*/
	halrf_wreg(rf, 0x7840, 0x003ff000, 0x0);	/*40M*/
	halrf_wreg(rf, 0x7848, 0x003ff000, 0x0);	/*80M*/
	halrf_wreg(rf, 0x7850, 0x003ff000, 0x0);	/*80M+80M*/
	halrf_wreg(rf, 0x7860, 0x003ff000, 0x0);	/*CCK short*/

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE Zero 0x7838[21:12]=0x%x 0x7858[21:12]=0x%x\n",
		halrf_rreg(rf, 0x7838, 0x003ff000),
		halrf_rreg(rf, 0x7858, 0x003ff000));
}

void _halrf_tssi_enable_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u8 i;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s\n", __func__);

	for (i = 0; i < TSSI_PATH_MAX_8852A; i++) {
		/*_halrf_tssi_set_tssi_slope_8852ab(rf, phy, i);*/
		_halrf_tssi_set_tssi_track_8852ab(rf, phy, i);
		_halrf_tssi_set_txagc_offset_mv_avg_8852ab(rf, phy, i);

		if (i == RF_PATH_A) { 
			halrf_wrf(rf, RF_PATH_A, 0x55, 0x00080, 0x1);
			if (rf->phl_com->drv_mode != RTW_DRV_MODE_MP) {
				/*Start Power minus 8dBm*/
				halrf_wreg(rf, 0x5818, 0x000000ff, 0xc0);
				halrf_wreg(rf, 0x5818, 0x10000000, 0x0);
				halrf_wreg(rf, 0x5818, 0x10000000, 0x1);
			}
			halrf_wreg(rf, 0x5820, 0x80000000, 0x0);
			halrf_wreg(rf, 0x5820, 0x80000000, 0x1);
			halrf_wreg(rf, 0x5818, 0x18000000, 0x3);
			rf->is_tssi_mode[RF_PATH_A] = true;
		} else {
			halrf_wrf(rf, RF_PATH_B, 0x55, 0x00080, 0x1);
			if (rf->phl_com->drv_mode != RTW_DRV_MODE_MP) {
				/*Start Power minus 8dBm*/
				halrf_wreg(rf, 0x7818, 0x000000ff, 0xc0);
				halrf_wreg(rf, 0x7818, 0x10000000, 0x0);
				halrf_wreg(rf, 0x7818, 0x10000000, 0x1);
			}
			halrf_wreg(rf, 0x7820, 0x80000000, 0x0);
			halrf_wreg(rf, 0x7820, 0x80000000, 0x1);
			halrf_wreg(rf, 0x7818, 0x18000000, 0x3);
			rf->is_tssi_mode[RF_PATH_B] = true;
		}

		tssi_info->base_thermal[i] = halrf_get_thermal_8852a(rf, i);
	}
}

void _halrf_tssi_disable_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);
	
	/*Path A*/
	halrf_wreg(rf, 0x5820, 0x80000000, 0x0);

	/*Request by Jerry for DPK*/
	halrf_wreg(rf, 0x5818, 0x18000000, 0x1);

	/*Path B*/
	halrf_wreg(rf, 0x7820, 0x80000000, 0x0);

	/*Request by Jerry for DPK*/
	halrf_wreg(rf, 0x7818, 0x18000000, 0x1);

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
		/*Set TXAGC init == 0*/
		halrf_wreg(rf, 0x5818, 0x000000ff, 0x0);
		halrf_wreg(rf, 0x7818, 0x000000ff, 0x0);
	}

#if 0
	/*Path A*/
	halrf_wreg(rf, 0x5818, 0x000000ff, 0x0);      /*TXAGC_OFFSET init = 0*/
	halrf_wreg(rf, 0x58dc, 0x80000000, 0x0);     /*rst release*/
	halrf_wreg(rf, 0x58dc, 0x80000000, 0x1);     /*rst*/

	/*Path B*/
	halrf_wreg(rf, 0x7818, 0x000000ff, 0x0);      /*TXAGC_OFFSET init = 0*/
	halrf_wreg(rf, 0x78dc, 0x80000000, 0x0);     /*rst release*/
	halrf_wreg(rf, 0x78dc, 0x80000000, 0x1);     /*rst*/
#endif
	rf->is_tssi_mode[RF_PATH_A] = false;
	rf->is_tssi_mode[RF_PATH_B] = false;
}

s32 _halrf_get_online_tssi_de_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				u8 path, s32 dbm, s32 puot)
{
	s32 de = ((puot - dbm) * 8) / 100;
	s32 s_tssi_offset;
	u32 tssi_offset;
	s8 tssi_trim;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy_idx);

	if (path == RF_PATH_A) {
		tssi_offset = halrf_rreg(rf, 0x5838, 0x003ff000);
		/*tssi_offset = halrf_rreg(rf, 0x5858, 0x003ff000);*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x5838[21:12]=0x%x 0x5858[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x5838, 0x003ff000),
			halrf_rreg(rf, 0x5858, 0x003ff000),
			path);
	} else {
		tssi_offset = halrf_rreg(rf, 0x7838, 0x003ff000);
		/*tssi_offset = halrf_rreg(rf, 0x7858, 0x003ff000);*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] Set TSSI DE 0x7838[21:12]=0x%x 0x7858[21:12]=0x%x path=%d\n",
			halrf_rreg(rf, 0x7838, 0x003ff000),
			halrf_rreg(rf, 0x7858, 0x003ff000),
			path);
	}

	if (tssi_offset & BIT(9))
		s_tssi_offset = tssi_offset | 0xfffffc00;
	else
		s_tssi_offset = tssi_offset;

	tssi_trim = _halrf_tssi_get_ofdm_tssi_trim_de_8852ab(rf, phy_idx, path);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI] de(%d) = de(%d) + s_tssi_offset(%d) - tssi_trim(%d) path=%d\n",
		(de + s_tssi_offset - tssi_trim), de, s_tssi_offset, tssi_trim, path);

	de = de + s_tssi_offset - tssi_trim;

	de = de & 0xff;

	return de;
}

void _halrf_tssi_ther_tracking_8852a(struct rf_info *rf)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;

	u8 i, path;
	u8 cur_ther;
	u32 ther_avg[TSSI_PATH_MAX_8852A] = {0};
	s32 delta_ther = 0, gain_offset_int, gain_offset_float;
	s8 gain_offset;
	u32 dpd_offset_en[TSSI_PATH_MAX_8852A] = {0x5800, 0x7800};
	u32 dpd_offset_addr[TSSI_PATH_MAX_8852A] = {0x5804, 0x7804};
	u32 tx_gain_scale[TSSI_PATH_MAX_8852A] = {0x58f0, 0x78f0};
	u32 tx_gain_scale_table[16] = {
		0x400,	//0.000dB
		0x40e,	//0.125dB
		0x41d,	//0.250dB
		0x427,	//0.325dB
		0x43c,	//0.500dB
		0x44c,	//0.625dB
		0x45c,	//0.750dB
		0x46c,	//0.875dB
		0x400,	//0.000dB
		0x39d,	//-0.875dB
		0x3ab,	//-0.750dB
		0x3b8,	//-0.625dB
		0x3c6,	//-0.500dB
		0x3d4,	//-0.375dB
		0x3e2,	//-0.250dB
		0x3f1	//-0.125dB
		};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s\n", __func__);

	for (path = RF_PATH_A; path < TSSI_PATH_MAX_8852A; path++) {
		if (tssi_info->tssi_tracking_check[path] == false) {
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				"tssi_info->tssi_tracking_check == false path=%d return!!!\n",
				path);
			continue;
		}

		//cur_ther = halrf_get_thermal_8852a(rf, path);
		cur_ther = (u8)halrf_rreg(rf, 0x1c10 + (path << 13), 0x3F000000); /*[29:24]*/

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI_TRK] thermal now=%d path=%d\n",
			cur_ther, path);

		if (cur_ther != 0) {
			tssi_info->ther_avg[path][tssi_info->ther_avg_fifo_idx[path]] = cur_ther;
			tssi_info->ther_avg_idx[path]++;
			tssi_info->ther_avg_fifo_idx[path]++;
		}

		if (tssi_info->ther_avg_fifo_idx[path] >= AVG_THERMAL_NUM_TSSI) {
			tssi_info->ther_avg_fifo_idx[path] = 0;
		}

		if (tssi_info->ther_avg_idx[path] >= AVG_THERMAL_NUM_TSSI) {
			tssi_info->ther_avg_idx[path] = AVG_THERMAL_NUM_TSSI;
		}

		/*Average times */
		//ther_avg_cnt = 0;
		for (i = 0; i < tssi_info->ther_avg_idx[path]; i++) {
			ther_avg[path] += tssi_info->ther_avg[path][i];
			//ther_avg_cnt++;
#if 0
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI_TRK] ther_avg[%d] = %d\n", i,
				       tssi_info->ther_avg[path][i]);
#endif
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI_TRK] thermal avg total = %d, tssi_info->ther_avg_idx = %d path=%d\n",
		       ther_avg[path], tssi_info->ther_avg_idx[path], path);

		/*Calculate Average ThermalValue after average enough times*/
		if (tssi_info->ther_avg_idx[path] != 0)
			ther_avg[path] = (u8)(ther_avg[path] / tssi_info->ther_avg_idx[path]);

		if (tssi_info->base_thermal[path] <= 15) {
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				"tssi_info->base_thermal=%d path=%d Return!!!\n",
				tssi_info->base_thermal[path], path);
		}

		if (ther_avg[path] != 0)
			delta_ther = ther_avg[path] - tssi_info->base_thermal[path];

#if 0
		if (delta_ther < -2)
			gain_offset = (((delta_ther + 2) * 18) / 10);
		else if (delta_ther < 0)
			gain_offset = delta_ther * 3 / 10;
		else if (delta_ther < 2)
			gain_offset = delta_ther * 3 / 10;
		else
			gain_offset = (((delta_ther - 2) * 18) / 10);
#else
		gain_offset = (s8)delta_ther * 15 / 10;
#endif
		tssi_info->ther_avg_final[path] = ther_avg[path];
		tssi_info->extra_ofst[path] = gain_offset;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI_TRK] ther_avg=%d tssi_info->base_thermal=%d gain_offset=0x%x path=%d\n",
		       ther_avg[path], tssi_info->base_thermal[path], gain_offset, path);

		gain_offset_int = gain_offset / 8;
		gain_offset_float = gain_offset % 8;

		if (gain_offset_int > 15)
			gain_offset_int = 15;
		else if (gain_offset_int < -16)
			gain_offset_int = -16;

		if (!rf->rfk_is_processing) {
			halrf_wreg(rf, dpd_offset_en[path], 0x10000000, 0x1);
			halrf_wreg(rf, tx_gain_scale[path], 0x00080000, 0x1);

			halrf_wreg(rf, dpd_offset_addr[path], 0xf8000000, gain_offset_int);
			
			halrf_wreg(rf, tx_gain_scale[path], 0xfff00000,
				tx_gain_scale_table[(gain_offset_float & 0xf)] & 0xfff);
		} else {
			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			       "[TSSI_TRK] rf->rfk_is_processing=%d path=%d Return!!!\n",
			       rf->rfk_is_processing, path);
		}

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI_TRK] Set dpd_offset(0x%x=0x%x) path=%d\n",
		       dpd_offset_addr[path],
		       halrf_rreg(rf, dpd_offset_addr[path], 0xf8000000), path);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		       "[TSSI_TRK] Set tx_gain_scale(0x%x=0x%x) path=%d\n",
		       tx_gain_scale[path],
		       halrf_rreg(rf, tx_gain_scale[path], 0xfff00000), path);

	}
}

void _halrf_tssi_high_power_8852a(struct rf_info *rf, enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 dpd_offset_en[TSSI_PATH_MAX_8852A] = {0x5800, 0x7800};
	u32 dpd_offset_addr[TSSI_PATH_MAX_8852A] = {0x5804, 0x7804};
	u32 tx_gain_scale[TSSI_PATH_MAX_8852A] = {0x58f0, 0x78f0};
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 bw = rf->hal_com->band[phy].cur_chandef.bw;
	s8 power;
	s32 xdbm;
	u8 i;

	if (bw == CHANNEL_WIDTH_40)
		channel = channel - 2;
	else if (bw == CHANNEL_WIDTH_80)
		channel = channel - 6;
	else
		channel = channel;

	power = halrf_get_power_limit(rf, phy, RF_PATH_A,
		RTW_DATA_RATE_MCS0, CHANNEL_WIDTH_20,
		PW_LMT_NONBF, PW_LMT_PH_1T, channel);

	xdbm = power * 100 / 4;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d  xdbm=%d\n",
		__func__, phy, xdbm);

	if (xdbm > 1800) {
		halrf_set_tssi_avg_mp_8852a(rf, phy, xdbm);
	} else {
		for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
			tssi_info->tssi_tracking_check[i] = false;

			halrf_wreg(rf, dpd_offset_en[i], 0x10000000, 0x0);
			halrf_wreg(rf, tx_gain_scale[i], 0x00080000, 0x0);
			halrf_wreg(rf, dpd_offset_addr[i], 0xf8000000, 0x0);
			halrf_wreg(rf, tx_gain_scale[i], 0xfff00000, 0x400);

			tssi_info->extra_ofst[i] = 0;
		}
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "tssi_info->tssi_type=%d ch=%d Return !!!\n",
			tssi_info->tssi_type, channel);
	}
}

void _halrf_tssi_set_avg_org_8852ab(struct rf_info *rf,
				enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d  path=%d\n",
		__func__, phy, path);

	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x5820, 0x0000f000, 0x4);
		halrf_wreg(rf, 0x58e4, 0x00003800, 0x2);
	} else {
		halrf_wreg(rf, 0x7820, 0x0000f000, 0x4);
		halrf_wreg(rf, 0x78e4, 0x00003800, 0x2);
	}
}

void _halrf_tssi_set_avg_8852ab(struct rf_info *rf,
				enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n",
		__func__, phy);

	halrf_tssi_disable_8852a(rf, phy);

	halrf_wreg(rf, 0x5820, 0x0000f000, 0x4);
	halrf_wreg(rf, 0x58e4, 0x00003800, 0x2);

	halrf_wreg(rf, 0x7820, 0x0000f000, 0x4);
	halrf_wreg(rf, 0x78e4, 0x00003800, 0x2);

	/*enable*/
	halrf_wreg(rf, 0x5820, 0x80000000, 0x0);
	halrf_wreg(rf, 0x5820, 0x80000000, 0x1);
	halrf_wreg(rf, 0x5818, 0x18000000, 0x3);

	/*enable*/
	halrf_wreg(rf, 0x7820, 0x80000000, 0x0);
	halrf_wreg(rf, 0x7820, 0x80000000, 0x1);
	halrf_wreg(rf, 0x7818, 0x18000000, 0x3);

	rf->is_tssi_mode[RF_PATH_A] = true;
	rf->is_tssi_mode[RF_PATH_B] = true;
}

void _halrf_tssi_set_avg_scan_8852ab(struct rf_info *rf, enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n",
		__func__, phy);

	halrf_tssi_disable_8852a(rf, phy);

	halrf_wreg(rf, 0x5820, 0x0000f000, 0x0);
	halrf_wreg(rf, 0x58e4, 0x00003800, 0x0);

	halrf_wreg(rf, 0x7820, 0x0000f000, 0x0);
	halrf_wreg(rf, 0x78e4, 0x00003800, 0x0);

	/*enable*/
	halrf_wreg(rf, 0x5820, 0x80000000, 0x0);
	halrf_wreg(rf, 0x5820, 0x80000000, 0x1);
	halrf_wreg(rf, 0x5818, 0x18000000, 0x3);

	/*enable*/
	halrf_wreg(rf, 0x7820, 0x80000000, 0x0);
	halrf_wreg(rf, 0x7820, 0x80000000, 0x1);
	halrf_wreg(rf, 0x7818, 0x18000000, 0x3);

	rf->is_tssi_mode[RF_PATH_A] = true;
	rf->is_tssi_mode[RF_PATH_B] = true;
}

void _halrf_tssi_pre_tx_8852ab(struct rf_info *rf, enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 bw = rf->hal_com->band[phy].cur_chandef.bw;
	s8 power;
	s16 xdbm;
	u32 i, tx_couter = 0;

	if (bw == CHANNEL_WIDTH_40)
		channel = channel - 2;
	else if (bw == CHANNEL_WIDTH_80)
		channel = channel - 6;
	else
		channel = channel;

	power = halrf_get_power_limit(rf, phy, RF_PATH_A,
		RTW_DATA_RATE_MCS0, CHANNEL_WIDTH_20,
		PW_LMT_NONBF, PW_LMT_PH_1T, channel);

	xdbm = power * 100 / 4;

	if (xdbm > 1800)
		xdbm = 17 * 4;
	else
		xdbm = power;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d original power limit=%d xdbm=%d\n",
		__func__, phy, power, xdbm);

	halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
	halrf_tmac_tx_pause(rf, phy, true);

	//halrf_delay_ms(rf, 15);

	/*Path A*/
	tx_couter= halrf_rreg(rf, 0x1a40, 0xffff);
	
	_halrf_tssi_hw_tx_8852a(rf, phy, RF_PATH_AB, 20, xdbm, T_HT_MF, 0, true);
	halrf_delay_ms(rf, 15);
	_halrf_tssi_hw_tx_8852a(rf, phy, RF_PATH_AB, 20, xdbm, T_HT_MF, 0, false);

	tx_couter = halrf_rreg(rf, 0x1a40, 0xffff) - tx_couter;

#if 0
	/*Path B*/
	tx_couter[RF_PATH_B] = halrf_rreg(rf, 0x1a40, 0xffff);
	
	_halrf_tssi_hw_tx_8852a(rf, phy, RF_PATH_B, 20, xdbm, T_HT_MF, 0, true);
	halrf_delay_ms(rf, 15);
	_halrf_tssi_hw_tx_8852a(rf, phy, RF_PATH_B, 20, xdbm, T_HT_MF, 0, false);
#endif

	if (halrf_rreg(rf, 0x1c60, 0xffff0000) != 0xc000 &&
		halrf_rreg(rf, 0x1c60, 0xffff0000) != 0x0) {
		for (i = 0; i < 6; i++) {
			tssi_info->default_txagc_offset[RF_PATH_A] =
				halrf_rreg(rf, 0x1c60, 0xff000000);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				"Current 0x1c60=0x%x i=%d\n",
				tssi_info->default_txagc_offset[RF_PATH_A], i);

			if (tssi_info->default_txagc_offset[RF_PATH_A] != 0x0)
				break;
		}
	}

	if (halrf_rreg(rf, 0x3c60, 0xffff0000) != 0xc000 &&
		halrf_rreg(rf, 0x3c60, 0xffff0000) != 0x0) {
		for (i = 0; i < 6; i++) {
			tssi_info->default_txagc_offset[RF_PATH_B] =
				halrf_rreg(rf, 0x3c60, 0xff000000);

			RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
				"Current 0x3c60=0x%x i=%d\n",
				tssi_info->default_txagc_offset[RF_PATH_B], i);

			if (tssi_info->default_txagc_offset[RF_PATH_B] != 0x0)
				break;
		}
	}

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		"======>%s tx couter=%d\n",
		__func__, tx_couter);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		"Backup 0x1c60=0x%x   0x3c60=0x%x\n",
		tssi_info->default_txagc_offset[RF_PATH_A],
		tssi_info->default_txagc_offset[RF_PATH_B]);

	halrf_wreg(rf, 0x5818, 0x000000ff,
		tssi_info->default_txagc_offset[RF_PATH_A]);
	halrf_wreg(rf, 0x7818, 0x000000ff,
		tssi_info->default_txagc_offset[RF_PATH_B]);

	halrf_wreg(rf, 0x5818 , 0x10000000, 0x0);
	halrf_wreg(rf, 0x5818 , 0x10000000, 0x1);

	halrf_wreg(rf, 0x7818 , 0x10000000, 0x0);
	halrf_wreg(rf, 0x7818 , 0x10000000, 0x1);

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		"======>%s Set 0x5818[7:0]=0x%x 0x7818[7:0]=0x%x\n",
		__func__,
		halrf_rreg(rf, 0x5818, 0x000000ff),
		halrf_rreg(rf, 0x7818, 0x000000ff));

	halrf_tx_mode_switch(rf, phy, 0);

	halrf_tmac_tx_pause(rf, phy, false);
	halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);

}

void _halrf_do_tssi_8852a_bcart(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 i;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	halrf_tssi_disable_8852a(rf, phy);

	for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
		_halrf_tssi_rf_setting_8852ab(rf, phy, i);
		_halrf_tssi_set_sys_8852ab(rf, phy);
		_halrf_tssi_ini_txpwr_ctrl_bb_8852ab(rf, phy, i);
		_halrf_tssi_ini_txpwr_ctrl_bb_he_tb_8852ab(rf, phy, i);
		_halrf_tssi_set_dck_8852ab(rf, phy, i);
		_halrf_tssi_set_bbgain_split_bcar_8852ab(rf, phy, i);
		_halrf_tssi_set_tmeter_tbl_8852ab(rf, phy, i);
		_halrf_tssi_set_dac_gain_tbl_8852ab(rf, phy, i);
		_halrf_tssi_tssi_slope_cal_org_8852ab(rf, phy, i);

		if (rfe_type <= 50)
			_halrf_tssi_set_tssi_slope_8852ab(rf, phy, i);
		else
			_halrf_tssi_efem_set_tssi_slope_8852ab(rf, phy, i);

		if (rfe_type < 50)
			_halrf_tssi_tssi_pak_8852ab(rf, phy, i);

#if 0
		//_halrf_tssi_run_tssi_slope_8852ab(rf, phy, i);

		//halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
		//halrf_tmac_tx_pause(rf, phy, true);

		//if (rfe_type <= 50)
			//_halrf_tssi_tssi_slope_cal_8852ab(rf, phy, i);

		//halrf_tmac_tx_pause(rf, phy, false);
		//halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);
#endif

		halrf_write_fw_final(rf);

#if 1
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
		halrf_tmac_tx_pause(rf, phy, true);

		if (rfe_type <= 50)
			_halrf_tssi_tssi_alimentk_8852ab(rf, phy, i);

		halrf_tmac_tx_pause(rf, phy, false);
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);
#endif
		_halrf_tssi_set_avg_org_8852ab(rf, phy, i);
	}

	if (rf->phl_com->drv_mode == RTW_DRV_MODE_MP) {
		if (tssi_info->tssi_type == TSSI_ON || tssi_info->tssi_type == TSSI_CAL) {
			halrf_tssi_enable_8852a(rf, phy);
			halrf_tssi_set_efuse_to_de_8852ab(rf, phy);
		} else
			halrf_tssi_disable_8852a(rf, phy);

	} else {
		if (rf->support_ability & HAL_RF_TX_PWR_TRACK) {
			halrf_tssi_enable_8852a(rf, phy);
			halrf_tssi_set_efuse_to_de_8852ab(rf, phy);
			_halrf_tssi_high_power_8852a(rf, phy);
			_halrf_tssi_pre_tx_8852ab(rf, phy);
		}
	}

	halrf_write_fw_final(rf);

	tssi_info->do_tssi = true;
}

void halrf_do_tssi_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   phy=%d\n", __func__, phy);

	if (rf->hal_com->cv == CAV)
		_halrf_do_tssi_8852a_acart(rf, phy);
	else {
		_halrf_do_tssi_8852a_bcart(rf, phy);
	}
}

void halrf_do_tssi_scan_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 i;
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 tmp = channel;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d  channel=%d\n",
					__func__, phy, channel);
	tmp++;

	if (tssi_info->do_tssi != true) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"======>%s   phy=%d   tssi_info->do_tssi != true  return!!!\n",
					__func__, phy);
		return;
	}

	halrf_tssi_disable_8852a(rf, phy);

	for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
		_halrf_tssi_rf_setting_8852ab(rf, phy, i);
		_halrf_tssi_set_sys_8852ab(rf, phy);
		//_halrf_tssi_ini_txpwr_ctrl_bb_8852ab(rf, phy, i);
		//_halrf_tssi_ini_txpwr_ctrl_bb_he_tb_8852ab(rf, phy, i);
		//_halrf_tssi_set_dck_8852ab(rf, phy, i);
		//_halrf_tssi_set_bbgain_split_bcar_8852ab(rf, phy, i);
		_halrf_tssi_set_tmeter_tbl_8852ab(rf, phy, i);
		//_halrf_tssi_set_dac_gain_tbl_8852ab(rf, phy, i);
		//_halrf_tssi_tssi_slope_cal_org_8852ab(rf, phy, i);
		//_halrf_tssi_set_rf_gap_tbl_8852ab(rf, phy, i);
		//_halrf_tssi_set_tssi_slope_8852ab(rf, phy, i);
		_halrf_tssi_tssi_pak_8852ab(rf, phy, i);
	}

	halrf_tssi_enable_8852a(rf, phy);
	halrf_tssi_set_efuse_to_de_8852ab(rf, phy);

	halrf_write_fw_final(rf);
}

void halrf_tssi_get_efuse_8852a(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	s8 de_offset = 0;
	u32 i, j;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	/*path s0*/
	for (i = EFUSE_INFO_RF_2G_CCK_A_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_2G_CCK_A_TSSI_DE_6; i++, j++) {
		halrf_efuse_get_info(rf, i, &de_offset, 1);
		tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_CCK][j] = de_offset;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] Efuse_addr=0x%x   value=0x%x\n",
			i, tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_CCK][j]);
	}

	for (i = EFUSE_INFO_RF_2G_BW40M_A_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_5G_BW40M_A_TSSI_DE_14; i++, j++) {
		halrf_efuse_get_info(rf, i, &de_offset, 1);
		tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_MCS][j] = de_offset;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] Efuse_addr=0x%x   value=0x%x\n",
			i, tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_MCS][j]);
	}

	/*path s1*/
	for (i = EFUSE_INFO_RF_2G_CCK_B_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_2G_CCK_B_TSSI_DE_6; i++, j++) {
		halrf_efuse_get_info(rf, i, &de_offset, 1);
		tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_CCK][j] = de_offset;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] Efuse_addr=0x%x   value=0x%x\n",
			i, tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_CCK][j]);
	}

	for (i = EFUSE_INFO_RF_2G_BW40M_B_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_5G_BW40M_B_TSSI_DE_14; i++, j++) {
		halrf_efuse_get_info(rf, i, &de_offset, 1);
		tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_MCS][j] = de_offset;

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] Efuse_addr=0x%x   value=0x%x\n",
			i, tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_MCS][j]);
	}
}

bool halrf_tssi_check_efuse_data_8852a(struct rf_info *rf,
						enum phl_phy_idx phy)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u32 i, j, countr = 0;

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s\n", __func__);

	halrf_tssi_get_efuse_8852a(rf, phy);

	/*path s0*/
	for (i = EFUSE_INFO_RF_2G_CCK_A_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_2G_CCK_A_TSSI_DE_6; i++, j++) {
		if ((tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_CCK][j] & 0xff) == 0xff)
			countr++;
	}

	for (i = EFUSE_INFO_RF_2G_BW40M_A_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_5G_BW40M_A_TSSI_DE_14; i++, j++) {
		if ((tssi_info->tssi_efuse[RF_PATH_A][EFUSE_TSSI_MCS][j] & 0xff) == 0xff)
			countr++;
	}

	/*path s1*/
	for (i = EFUSE_INFO_RF_2G_CCK_B_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_2G_CCK_B_TSSI_DE_6; i++, j++) {
		if ((tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_CCK][j] & 0xff) == 0xff)
			countr++;
	}

	for (i = EFUSE_INFO_RF_2G_BW40M_B_TSSI_DE_1, j = 0; i <= EFUSE_INFO_RF_5G_BW40M_B_TSSI_DE_14; i++, j++) {
		if ((tssi_info->tssi_efuse[RF_PATH_B][EFUSE_TSSI_MCS][j] & 0xff) == 0xff)
			countr++;
	}

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] countr == %d\n", countr);

	if (countr == 50) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "[TSSI][Efuse] Efuse all are 0xff, return false!!!\n");
		return false;
	}

	return true;	
}

void halrf_set_tssi_de_for_tx_verify_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de, u8 path)
{
	if (rf->hal_com->cv == CAV)
		_halrf_set_tssi_de_for_tx_verify_8852a(rf, phy, tssi_de, path);
	else
		_halrf_set_tssi_de_for_tx_verify_8852ab(rf, phy, tssi_de, path);
}

void halrf_set_tssi_de_offset_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, u32 tssi_de_offset, u8 path)
{
	if (rf->hal_com->cv == CAV)
		_halrf_set_tssi_de_offset_8852a(rf, phy, tssi_de_offset, path);
	else
		_halrf_set_tssi_de_offset_8852ab(rf, phy, tssi_de_offset, path);
}

void halrf_set_tssi_de_offset_zero_8852a(struct rf_info *rf,
							enum phl_phy_idx phy)
{
	if (rf->hal_com->cv == CAV)
		_halrf_set_tssi_de_offset_zero_8852a(rf, phy);
	else
		_halrf_set_tssi_de_offset_zero_8852ab(rf, phy);
}

void halrf_tssi_enable_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	if (rf->hal_com->cv == CAV)
		_halrf_tssi_enable_8852a(rf, phy);
	else
		_halrf_tssi_enable_8852ab(rf, phy);
}

void halrf_tssi_disable_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	if (rf->hal_com->cv == CAV)
		_halrf_tssi_disable_8852a(rf, phy);
	else
		_halrf_tssi_disable_8852ab(rf, phy);
}

void halrf_tssi_tracking_8852a(struct rf_info *rf)
{
	_halrf_tssi_ther_tracking_8852a(rf);
}

s32 halrf_get_online_tssi_de_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx,
				u8 path, s32 dbm, s32 puot)
{
	if (rf->hal_com->cv == CAV)
		return _halrf_get_online_tssi_de_8852a(rf, phy_idx, path, dbm, puot);
	else
		return _halrf_get_online_tssi_de_8852ab(rf, phy_idx, path, dbm, puot);
}

void halrf_set_tssi_avg_reset_8852a(struct rf_info *rf, enum phl_phy_idx phy_idx)
{
	halrf_wreg(rf, 0x58e4, 0x00004000, 0x0);
	halrf_wreg(rf, 0x58e4, 0x00004000, 0x1);
	halrf_wreg(rf, 0x58e4, 0x00004000, 0x0);

	halrf_wreg(rf, 0x78e4, 0x00004000, 0x0);
	halrf_wreg(rf, 0x78e4, 0x00004000, 0x1);
	halrf_wreg(rf, 0x78e4, 0x00004000, 0x0);
}

void halrf_set_tssi_avg_mp_8852a(struct rf_info *rf,
					enum phl_phy_idx phy_idx, s32 xdbm)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;
	u8 ch = rf->hal_com->band[phy_idx].cur_chandef.center_ch;
	u8 i;
	u32 dpd_offset_en[TSSI_PATH_MAX_8852A] = {0x5800, 0x7800};
	u32 dpd_offset_addr[TSSI_PATH_MAX_8852A] = {0x5804, 0x7804};
	u32 tx_gain_scale[TSSI_PATH_MAX_8852A] = {0x58f0, 0x78f0};

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   phy=%d   xdBm=%d\n",
					__func__, phy_idx, xdbm);

	if (!(rf->support_ability & HAL_RF_TSSI_TRK)) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "rf->support_ability & HAL_RF_TSSI_TRK = %d\n",
			rf->support_ability & HAL_RF_TSSI_TRK);
		return;
	}

	tssi_info->tssi_xdbm = xdbm;

	if ((tssi_info->tssi_type == TSSI_OFF && rf->phl_com->drv_mode == RTW_DRV_MODE_MP) || ch > 14) {
		for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
			tssi_info->tssi_tracking_check[i] = false;

			halrf_wreg(rf, dpd_offset_en[i], 0x10000000, 0x0);
			halrf_wreg(rf, tx_gain_scale[i], 0x00080000, 0x0);
			halrf_wreg(rf, dpd_offset_addr[i], 0xf8000000, 0x0);
			halrf_wreg(rf, tx_gain_scale[i], 0xfff00000, 0x400);

			tssi_info->extra_ofst[i] = 0;
		}
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "tssi_info->tssi_type=%d ch=%d Return !!!\n",
			tssi_info->tssi_type, ch);
		return;
	}

	for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
		if (xdbm > 1800)
			tssi_info->tssi_tracking_check[i] = true;
		else {
			for (i = RF_PATH_A; i < TSSI_PATH_MAX_8852A; i++) {
				tssi_info->tssi_tracking_check[i] = false;
				tssi_info->extra_ofst[i] = 0;

				halrf_wreg(rf, dpd_offset_en[i], 0x10000000, 0x0);
				halrf_wreg(rf, tx_gain_scale[i], 0x00080000, 0x0);
				halrf_wreg(rf, dpd_offset_addr[i], 0xf8000000, 0x0);
				halrf_wreg(rf, tx_gain_scale[i], 0xfff00000, 0x400);
			}
		}
	}
}

void halrf_tssi_default_txagc_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, bool enable)
{
//	struct rtw_tpu_info *tpu = &rf->hal_com->rtw_tpu_i;
	struct halrf_tssi_info *tssi_info = &rf->tssi;
//	u8 ch = rf->hal_com->band[phy].cur_chandef.center_ch;
	u8 i;

	if (enable == true) {
		/*SCAN_START*/
		//if (tssi_info->default_txagc_offset_check == 2) {
		//	tssi_info->default_txagc_offset_check = 3;

		if (halrf_rreg(rf, 0x1c60, 0xffff0000) != 0xc000 &&
			halrf_rreg(rf, 0x1c60, 0xffff0000) != 0x0) {
			for (i = 0; i < 6; i++) {
				tssi_info->default_txagc_offset[RF_PATH_A] =
					halrf_rreg(rf, 0x1c60, 0xff000000);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
					"======>%s SCAN_START 0x1c60=0x%x i=%d\n",
					__func__, tssi_info->default_txagc_offset[RF_PATH_A], i);

				if (tssi_info->default_txagc_offset[RF_PATH_A] != 0x0)
					break;
			}
		}

		if (halrf_rreg(rf, 0x3c60, 0xffff0000) != 0xc000 &&
			halrf_rreg(rf, 0x3c60, 0xffff0000) != 0x0) {
			for (i = 0; i < 6; i++) {
				tssi_info->default_txagc_offset[RF_PATH_B] =
					halrf_rreg(rf, 0x3c60, 0xff000000);

				RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
					"======>%s SCAN_START 0x3c60=0x%x i=%d\n",
					__func__, tssi_info->default_txagc_offset[RF_PATH_B], i);

				if (tssi_info->default_txagc_offset[RF_PATH_B] != 0x0)
					break;
			}
		}

		//_halrf_tssi_disable_8852ab(rf, phy);

		//halrf_wreg(rf, 0x5804, 0x000FF000, tpu->base_cw_0db - 4);
		//halrf_wreg(rf, 0x7804, 0x000FF000, tpu->base_cw_0db - 4);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   SCAN_START\n", __func__);
	} else {
		/*SCAN_END*/
		//halrf_wreg(rf, 0x5804, 0x000FF000, tpu->base_cw_0db);
		//halrf_wreg(rf, 0x7804, 0x000FF000, tpu->base_cw_0db);

		//halrf_tssi_enable_8852a(rf, phy);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"======>%s 1 SCAN_END Set 0x5818[7:0]=0x%x 0x7818[7:0]=0x%x\n",
			__func__,
			halrf_rreg(rf, 0x5818, 0x000000ff),
			halrf_rreg(rf, 0x7818, 0x000000ff));

		halrf_wreg(rf, 0x5818, 0x000000ff,
			tssi_info->default_txagc_offset[RF_PATH_A]);
		halrf_wreg(rf, 0x7818, 0x000000ff,
			tssi_info->default_txagc_offset[RF_PATH_B]);

		halrf_wreg(rf, 0x5818 , 0x10000000, 0x0);
		halrf_wreg(rf, 0x5818 , 0x10000000, 0x1);

		halrf_wreg(rf, 0x7818 , 0x10000000, 0x0);
		halrf_wreg(rf, 0x7818 , 0x10000000, 0x1);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
			"======>%s 2 SCAN_END Set 0x5818[7:0]=0x%x 0x7818[7:0]=0x%x\n",
			__func__,
			halrf_rreg(rf, 0x5818, 0x000000ff),
			halrf_rreg(rf, 0x7818, 0x000000ff));

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   SCAN_END\n", __func__);
	}
}

void halrf_tssi_set_avg_8852ab(struct rf_info *rf,
					enum phl_phy_idx phy, bool enable)
{
	if (enable == true) {
		/*SCAN_START*/
		_halrf_tssi_set_avg_scan_8852ab(rf, phy);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   SCAN_START\n", __func__);
	} else {
		/*SCAN_END*/
		_halrf_tssi_set_avg_8852ab(rf, phy);

		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======> %s   SCAN_END\n", __func__);
	}

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		"Set TSSI avg 0x5820[15:12]=0x%x 0x58e4[13:11]=0x%x\n",
		halrf_rreg(rf, 0x5820, 0x0000f000),
		halrf_rreg(rf, 0x58e4, 0x00003800));

	RF_DBG(rf, DBG_RF_TX_PWR_TRACK,
		"Set TSSI avg 0x7820[15:12]=0x%x 0x78e4[13:11]=0x%x\n",
		halrf_rreg(rf, 0x7820, 0x0000f000),
		halrf_rreg(rf, 0x78e4, 0x00003800));

}

void halrf_tssi_scan_ch_8852ab(struct rf_info *rf, enum rf_path path)
{
	u32 tssi_trk_man[2] = {0x5818, 0x7818};
	u32 tssi_en[2] = {0x5820, 0x7820};

	if (!(rf->phl_com->drv_mode == RTW_DRV_MODE_MP)) {
		RF_DBG(rf, DBG_RF_TX_PWR_TRACK, "======>%s   path=%d\n",
			__func__, path);

		halrf_wreg(rf, tssi_en[path], BIT(31), 0x0);
		halrf_wreg(rf, tssi_en[path], BIT(31), 0x1);
		halrf_wreg(rf, tssi_trk_man[path], 0x000000ff, 0xc0);
		halrf_wreg(rf, tssi_trk_man[path], BIT(28), 0x0);
		halrf_wreg(rf, tssi_trk_man[path], BIT(28), 0x1);
	}
}

void halrf_get_tssi_info_8852a(struct rf_info *rf,
		char input[][16], u32 *_used, char *output, u32 *_out_len)
{
	u32 tmp, tmp1, pg_ther, cur_ther;
	s32 delta_tssi;
	s32 diff_ther;
	
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %x\n",
		 "RF Para Ver", halrf_rrf(rf, RF_PATH_A, 0x67, 0xfff));

	tmp = halrf_rreg(rf, 0x1c78, 0x1ff);
	tmp1 = halrf_rreg(rf, 0x3c78, 0x1ff);
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %d.%d / %d.%d\n",
		 "T-MAC xdbm A / B",
		 tmp / 4, tmp * 100 / 4 % 100,
		 tmp1 / 4, tmp1 * 100 / 4 % 100);

	halrf_wreg(rf, 0x58b4, 0x0000001f, 0x7);
	tmp = halrf_rreg(rf, 0x1c00, 0x0ffc0000);
	if (tmp & BIT(9))
		delta_tssi = tmp | 0xfffffc00;
	else
		delta_tssi = tmp;
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %d / %d / %d\n",
		 "TSSI C / Final / Delta A", halrf_rreg(rf, 0x1c00, 0x000001ff),
		 halrf_rreg(rf, 0x1c00, 0x0003fe00), delta_tssi);

	halrf_wreg(rf, 0x78b4, 0x0000001f, 0x7);
	tmp = halrf_rreg(rf, 0x3c00, 0x0ffc0000);
	if (tmp & BIT(9))
		delta_tssi = tmp | 0xfffffc00;
	else
		delta_tssi = tmp;
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %d / %d / %d\n",
		 "TSSI C / Final / Delta B", halrf_rreg(rf, 0x1c00, 0x000001ff),
		 halrf_rreg(rf, 0x1c00, 0x0003fe00), delta_tssi);

	pg_ther = halrf_rreg(rf, 0x5810, 0x0000fc00);
	cur_ther = halrf_rreg(rf, 0x1c10, 0xff000000);
	diff_ther = pg_ther - cur_ther;
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %d / %d / %d / %d\n",
		 "TherA PG / Cur / Diff / F", pg_ther, cur_ther, diff_ther, (s8)halrf_rreg(rf, 0x1c08, 0xff000000)
		 );

	pg_ther = halrf_rreg(rf, 0x7810, 0x0000fc00);
	cur_ther = halrf_rreg(rf, 0x3c10, 0xff000000);
	diff_ther = pg_ther - cur_ther;
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %d / %d / %d / %d\n",
		 "TherB PG / Cur / Diff / F", pg_ther, cur_ther, diff_ther, (s8)halrf_rreg(rf, 0x3c08, 0xff000000)
		 );
		 
	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = 0x%x / 0x%x\n",
		 "TXAGC Offset A / B", halrf_rreg(rf, 0x1c60, 0xff000000),
		 halrf_rreg(rf, 0x3c60, 0xff000000));
}

void halrf_get_tssi_trk_info_8852a(struct rf_info *rf,
		char input[][16], u32 *_used, char *output, u32 *_out_len)
{
	struct halrf_tssi_info *tssi_info = &rf->tssi;

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		 "\n===============[ TSSI Tracking info 8852A ]===============\n");

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = %s / %s\n",
		 "TSSI Tracking A / B",
		 tssi_info->tssi_tracking_check[RF_PATH_A] ? "Enable" : "Disable",
		 tssi_info->tssi_tracking_check[RF_PATH_B] ? "Enable" : "Disable");

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = 0x%x / 0x%x / %d\n",
		"TherA Base / AVG / Oft",
		tssi_info->base_thermal[RF_PATH_A],
		tssi_info->ther_avg_final[RF_PATH_A],
		tssi_info->extra_ofst[RF_PATH_A]);

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = 0x%x / 0x%x / %d\n",
		"TherB Base / AVG / Oft",
		tssi_info->base_thermal[RF_PATH_B],
		tssi_info->ther_avg_final[RF_PATH_B],
		tssi_info->extra_ofst[RF_PATH_B]);

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = 0x%x / 0x%x\n",
		"DPD Offset A / B",
		halrf_rreg(rf, 0x5804, 0xf8000000),
		halrf_rreg(rf, 0x7804, 0xf8000000));

	RF_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used, " %-25s = 0x%x / 0x%x\n",
		"TX Gain Scale A / B",
		halrf_rreg(rf, 0x58f0, 0xfff00000),
		halrf_rreg(rf, 0x78f0, 0xfff00000));
}

void halrf_tssi_hw_tx_8852a(struct rf_info *rf,
			enum phl_phy_idx phy, u8 path, u16 cnt, s16 dbm, u32 rate, u8 bw,
			bool enable)
{
	_halrf_tssi_hw_tx_8852a(rf, phy, path, cnt, dbm, T_HT_MF, 0, enable);
}

#endif	/*RF_8852A_SUPPORT*/
