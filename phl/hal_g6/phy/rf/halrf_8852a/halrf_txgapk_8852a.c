/******************************************************************************
 *
 * Copyright(c) 2007 - 2017  Realtek Corporation.
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

void _txgapk_backup_bb_registers_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)
{
	u32 i;

	for (i = 0; i < reg_num; i++) {
		reg_backup[i] = halrf_rreg(rf, reg[i], MASKDWORD);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK] Backup BB 0x%x = 0x%x\n",
		       reg[i], reg_backup[i]);
	}
}

void _txgapk_reload_bb_registers_8852a(
	struct rf_info *rf,
	enum phl_phy_idx phy,
	u32 *reg,
	u32 *reg_backup,
	u32 reg_num)

{
	u32 i;

	for (i = 0; i < reg_num; i++) {
		halrf_wreg(rf, reg[i], MASKDWORD, reg_backup[i]);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK] Reload BB 0x%x = 0x%x\n",
		       reg[i], reg_backup[i]);
	}
}

void _halrf_txgapk_iqk_dpk_init_reg_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0x00, 0x00001, 0x1);

	halrf_wreg(rf, 0x0c60, 0x00000003, 0x3);
	halrf_wreg(rf, 0x0c6c, 0x00000001, 0x1);
	halrf_wreg(rf, 0x58ac, 0x08000000, 0x1);
	halrf_wreg(rf, 0x78ac, 0x08000000, 0x1);
	halrf_wreg(rf, 0x8000, MASKDWORD, 0x00000008);
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8004, MASKDWORD, 0xf0862966);
	halrf_wreg(rf, 0x800c, MASKDWORD, 0x78000000);
	halrf_wreg(rf, 0x8010, MASKDWORD, 0x88015100);
	halrf_wreg(rf, 0x8014, MASKDWORD, 0x80010100);
	halrf_wreg(rf, 0x8018, MASKDWORD, 0x10010100);
	halrf_wreg(rf, 0x801c, MASKDWORD, 0xa210fc00);
	halrf_wreg(rf, 0x8020, MASKDWORD, 0x000403e0);
	halrf_wreg(rf, 0x8024, MASKDWORD, 0x00072160);
	halrf_wreg(rf, 0x8028, MASKDWORD, 0x00180e00);
	halrf_wreg(rf, 0x8030, MASKDWORD, 0x400000c0);
	halrf_wreg(rf, 0x8034, MASKDWORD, 0x56000800);
	halrf_wreg(rf, 0x8038, MASKDWORD, 0x00000009);
	halrf_wreg(rf, 0x803c, MASKDWORD, 0x00000008);
	halrf_wreg(rf, 0x8040, MASKDWORD, 0x00000046);
	halrf_wreg(rf, 0x8044, MASKDWORD, 0x0010001f);
	halrf_wreg(rf, 0x8048, MASKDWORD, 0xf0000003);
	halrf_wreg(rf, 0x804c, MASKDWORD, 0x62ac6162);
	halrf_wreg(rf, 0x8050, MASKDWORD, 0xf2acf162);
	halrf_wreg(rf, 0x8054, MASKDWORD, 0x62ac6162);
	halrf_wreg(rf, 0x8058, MASKDWORD, 0xf2acf162);
	halrf_wreg(rf, 0x805c, MASKDWORD, 0x150c0b02);
	halrf_wreg(rf, 0x8060, MASKDWORD, 0x150c0b02);
	halrf_wreg(rf, 0x8064, MASKDWORD, 0x2aa00047);
	halrf_wreg(rf, 0x8074, MASKDWORD, 0x80000000);
	halrf_wreg(rf, 0x807c, MASKDWORD, 0x000000ee);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000000);
	halrf_wreg(rf, 0x8098, MASKDWORD, 0x0000ff00);
	halrf_wreg(rf, 0x809c, MASKDWORD, 0x0000001f);
	halrf_wreg(rf, 0x80a0, MASKDWORD, 0x00010300);
	halrf_wreg(rf, 0x80b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8114, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8120, MASKDWORD, 0x10010000);
	halrf_wreg(rf, 0x8124, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x812c, MASKDWORD, 0x0000c000);
	halrf_wreg(rf, 0x8138, MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x813c, MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x8140, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8144, MASKDWORD, 0x0b040b03);
	halrf_wreg(rf, 0x8148, MASKDWORD, 0x0b040b04);
	halrf_wreg(rf, 0x814c, MASKDWORD, 0x0b040b03);
	halrf_wreg(rf, 0x8150, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8158, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x815c, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8160, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8164, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8168, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x816c, MASKDWORD, 0x1fffffff);
	halrf_wreg(rf, 0x81ac, MASKDWORD, 0x003f1a00);
	halrf_wreg(rf, 0x81b0, MASKDWORD, 0x003f1a00);
	halrf_wreg(rf, 0x81bc, MASKDWORD, 0x005b5b5b);
	halrf_wreg(rf, 0x81c0, MASKDWORD, 0x005b5b5b);
	halrf_wreg(rf, 0x81b4, MASKDWORD, 0x00600060);
	halrf_wreg(rf, 0x81b8, MASKDWORD, 0x00600060);
	halrf_wreg(rf, 0x81cc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x8214, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8220, MASKDWORD, 0x10010000);
	halrf_wreg(rf, 0x8224, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x822c, MASKDWORD, 0x0000d000);
	halrf_wreg(rf, 0x8238, MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x823c, MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x8240, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8244, MASKDWORD, 0x0b040b03);
	halrf_wreg(rf, 0x8248, MASKDWORD, 0x0b040b03);
	halrf_wreg(rf, 0x824c, MASKDWORD, 0x0b030b03);
	halrf_wreg(rf, 0x8250, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8258, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x825c, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8260, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8264, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x8268, MASKDWORD, 0xffffffff);
	halrf_wreg(rf, 0x826c, MASKDWORD, 0x1fffffff);
	halrf_wreg(rf, 0x82ac, MASKDWORD, 0x003f1a00);
	halrf_wreg(rf, 0x82b0, MASKDWORD, 0x003f1a00);
	halrf_wreg(rf, 0x82bc, MASKDWORD, 0x005b5b5b);
	halrf_wreg(rf, 0x82c0, MASKDWORD, 0x005b5b5b);
	halrf_wreg(rf, 0x82b4, MASKDWORD, 0x00600060);
	halrf_wreg(rf, 0x82b8, MASKDWORD, 0x00600060);
	halrf_wreg(rf, 0x82cc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000001);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000001);
	halrf_wreg(rf, 0x8d00, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d04, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d0c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d10, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d28, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d2c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d30, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d34, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d38, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d3c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d40, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d44, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d48, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d4c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d50, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d54, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d5c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d60, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d78, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d7c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d80, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d84, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d88, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d8c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d90, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d94, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d98, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8d9c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8da0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8da4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8da8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8db0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8db4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8db8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dbc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dc0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dc4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dc8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dcc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dd0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dd4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dd8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ddc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8de0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8de4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8de8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8df0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8df4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8df8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8dfc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e00, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e04, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e0c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e10, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e28, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e2c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e30, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e34, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e38, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e3c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e40, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e44, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e48, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e4c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e50, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e54, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e5c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e60, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e78, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e7c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e80, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e84, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e88, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e8c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e90, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e94, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e98, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8e9c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ea0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ea4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ea8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8eac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8eb0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8eb4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8eb8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ebc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ec0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ec4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ec8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ecc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ed0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ed4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ed8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8edc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ee0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ee4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ee8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8eec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ef0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ef4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ef8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8efc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f00, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f04, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f0c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f10, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f28, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f2c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f30, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f34, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f38, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f3c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f40, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f44, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f48, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f4c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f50, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f54, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f5c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f60, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f78, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f7c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f80, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f84, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f88, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f8c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f90, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f94, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f98, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8f9c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fa0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fa4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fa8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fb0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fb4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fb8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fbc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fc0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fc4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fc8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fcc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fd0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fd4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fd8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fdc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fe0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fe4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fe8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8fec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ff0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ff4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ff8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ffc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9000, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9004, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9008, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x900c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9010, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9014, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9018, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x901c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9020, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9024, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9028, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x902c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9030, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9034, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9038, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x903c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9040, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9044, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9048, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x904c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9050, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9054, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9058, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x905c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9060, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9064, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9068, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x906c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9070, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9074, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9078, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x907c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9080, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9084, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9088, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x908c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9090, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9094, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9098, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x909c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90a0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90a4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90a8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90ac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90b4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90b8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x90bc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9100, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9104, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9108, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x910c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9110, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9114, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9118, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x911c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9120, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9124, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9128, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x912c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9130, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9134, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9138, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x913c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9140, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9144, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9148, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x914c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9150, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9154, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9158, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x915c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9160, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9164, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9168, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x916c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9170, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9174, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9178, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x917c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9180, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9184, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9188, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x918c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9190, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9194, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9198, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x919c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91a0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91a4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91a8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91ac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91b4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91b8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91bc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91c0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91c4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91c8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91cc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91d4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91dc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91e0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91e4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91e8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91ec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91f0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91f4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91f8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x91fc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9200, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9204, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9208, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x920c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9210, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9214, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9218, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x921c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9220, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9224, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9228, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x922c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9230, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9234, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9238, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x923c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9240, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9244, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9248, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x924c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9250, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9254, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9258, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x925c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9260, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9264, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9268, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x926c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9270, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9274, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9278, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x927c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9280, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9284, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9288, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x928c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9290, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9294, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9298, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x929c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92a0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92a4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92a8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92ac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92b4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92b8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92bc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92c0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92c4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92c8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92cc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92d4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92dc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92e0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92e4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92e8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92ec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92f0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92f4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92f8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x92fc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9300, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9304, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9308, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x930c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9310, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9314, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9318, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x931c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9320, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9324, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9328, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x932c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9330, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9334, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9338, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x933c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9340, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9344, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9348, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x934c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9350, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9354, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9358, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x935c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9360, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9364, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9368, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x936c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9370, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9374, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9378, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x937c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9380, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9384, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9388, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x938c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9390, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9394, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9398, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x939c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93a0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93a4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93a8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93ac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93b4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93b8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93bc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93c0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93c4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93c8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93cc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93d4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93dc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93e0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93e4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93e8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93ec, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93f0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93f4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93f8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x93fc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9400, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9404, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9408, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x940c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9410, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9414, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9418, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x941c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9420, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9424, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9428, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x942c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9430, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9434, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9438, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x943c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9440, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9444, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9448, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x944c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9450, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9454, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9458, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x945c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9460, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9464, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9468, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x946c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9470, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9474, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9478, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x947c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9480, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9484, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9488, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x948c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9490, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9494, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9498, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x949c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94a0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94a4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94a8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94ac, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94b0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94b4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94b8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x94bc, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x82d8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x9f04, MASKDWORD, 0x2b251f19);
	halrf_wreg(rf, 0x9f08, MASKDWORD, 0x433d3731);
	halrf_wreg(rf, 0x9f0c, MASKDWORD, 0x5b554f49);
	halrf_wreg(rf, 0x9f10, MASKDWORD, 0x736d6761);
	halrf_wreg(rf, 0x9f14, MASKDWORD, 0x7f7f7f79);
	halrf_wreg(rf, 0x9f18, MASKDWORD, 0x120f7f7f);
	halrf_wreg(rf, 0x9f1c, MASKDWORD, 0x1e1b1815);
	halrf_wreg(rf, 0x9f20, MASKDWORD, 0x2a272421);
	halrf_wreg(rf, 0x9f24, MASKDWORD, 0x3633302d);
	halrf_wreg(rf, 0x9f28, MASKDWORD, 0x3f3f3c39);
	halrf_wreg(rf, 0x9f2c, MASKDWORD, 0x3f3f3f3f);

}

void _halrf_txgapk_nctl_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000110);
	halrf_wreg(rf, 0x8000, MASKDWORD, 0x00000008);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000005);
	halrf_wreg(rf, 0x8500, MASKDWORD, 0x00060009);
	halrf_wreg(rf, 0x8504, MASKDWORD, 0x000418b0);
	halrf_wreg(rf, 0x8508, MASKDWORD, 0x00089c00);
	halrf_wreg(rf, 0x850c, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x8510, MASKDWORD, 0x4b044a00);
	halrf_wreg(rf, 0x8514, MASKDWORD, 0x40098603);
	halrf_wreg(rf, 0x8518, MASKDWORD, 0x4b05e01f);
	halrf_wreg(rf, 0x851c, MASKDWORD, 0x400b8703);
	halrf_wreg(rf, 0x8520, MASKDWORD, 0x4b00e01f);
	halrf_wreg(rf, 0x8524, MASKDWORD, 0x43800004);
	halrf_wreg(rf, 0x8528, MASKDWORD, 0x4c000007);
	halrf_wreg(rf, 0x852c, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x8530, MASKDWORD, 0x57007430);
	halrf_wreg(rf, 0x8534, MASKDWORD, 0x73000006);
	halrf_wreg(rf, 0x8538, MASKDWORD, 0x50550004);
	halrf_wreg(rf, 0x853c, MASKDWORD, 0xb4163000);
	halrf_wreg(rf, 0x8540, MASKDWORD, 0xe360a510);
	halrf_wreg(rf, 0x8544, MASKDWORD, 0xf117f017);
	halrf_wreg(rf, 0x8548, MASKDWORD, 0xf317f217);
	halrf_wreg(rf, 0x854c, MASKDWORD, 0xf517f417);
	halrf_wreg(rf, 0x8550, MASKDWORD, 0xf717f617);
	halrf_wreg(rf, 0x8554, MASKDWORD, 0xf917f817);
	halrf_wreg(rf, 0x8558, MASKDWORD, 0xfb17fa17);
	halrf_wreg(rf, 0x855c, MASKDWORD, 0xfd17fc17);
	halrf_wreg(rf, 0x8560, MASKDWORD, 0xf117f017);
	halrf_wreg(rf, 0x8564, MASKDWORD, 0xf317f217);
	halrf_wreg(rf, 0x8568, MASKDWORD, 0xa503f417);
	halrf_wreg(rf, 0x856c, MASKDWORD, 0xf116f016);
	halrf_wreg(rf, 0x8570, MASKDWORD, 0x304e0001);
	halrf_wreg(rf, 0x8574, MASKDWORD, 0x30873053);
	halrf_wreg(rf, 0x8578, MASKDWORD, 0x30ab30a8);
	halrf_wreg(rf, 0x857c, MASKDWORD, 0x30b330ae);
	halrf_wreg(rf, 0x8580, MASKDWORD, 0x30ba30b6);
	halrf_wreg(rf, 0x8584, MASKDWORD, 0x30d430c7);
	halrf_wreg(rf, 0x8588, MASKDWORD, 0x310d3100);
	halrf_wreg(rf, 0x858c, MASKDWORD, 0x31cd3112);
	halrf_wreg(rf, 0x8590, MASKDWORD, 0x31ea31d1);
	halrf_wreg(rf, 0x8594, MASKDWORD, 0x322131eb);
	halrf_wreg(rf, 0x8598, MASKDWORD, 0x31c63191);
	halrf_wreg(rf, 0x859c, MASKDWORD, 0x5b00e261);
	halrf_wreg(rf, 0x85a0, MASKDWORD, 0xe2af5500);
	halrf_wreg(rf, 0x85a4, MASKDWORD, 0xe2610001);
	halrf_wreg(rf, 0x85a8, MASKDWORD, 0x5b10e2c1);
	halrf_wreg(rf, 0x85ac, MASKDWORD, 0x20987410);
	halrf_wreg(rf, 0x85b0, MASKDWORD, 0xe3570200);
	halrf_wreg(rf, 0x85b4, MASKDWORD, 0x00002080);
	halrf_wreg(rf, 0x85b8, MASKDWORD, 0x23f0e357);
	halrf_wreg(rf, 0x85bc, MASKDWORD, 0xe3570001);
	halrf_wreg(rf, 0x85c0, MASKDWORD, 0x000023f0);
	halrf_wreg(rf, 0x85c4, MASKDWORD, 0x5507e357);
	halrf_wreg(rf, 0x85c8, MASKDWORD, 0xe2b3e2b3);
	halrf_wreg(rf, 0x85cc, MASKDWORD, 0x20887410);
	halrf_wreg(rf, 0x85d0, MASKDWORD, 0xe3570200);
	halrf_wreg(rf, 0x85d4, MASKDWORD, 0x000123f0);
	halrf_wreg(rf, 0x85d8, MASKDWORD, 0x23f0e357);
	halrf_wreg(rf, 0x85dc, MASKDWORD, 0xe3570000);
	halrf_wreg(rf, 0x85e0, MASKDWORD, 0xe2b35517);
	halrf_wreg(rf, 0x85e4, MASKDWORD, 0x4e004f02);
	halrf_wreg(rf, 0x85e8, MASKDWORD, 0x52015302);
	halrf_wreg(rf, 0x85ec, MASKDWORD, 0x7508e2b7);
	halrf_wreg(rf, 0x85f0, MASKDWORD, 0x74207900);
	halrf_wreg(rf, 0x85f4, MASKDWORD, 0x57005710);
	halrf_wreg(rf, 0x85f8, MASKDWORD, 0x75fbe357);
	halrf_wreg(rf, 0x85fc, MASKDWORD, 0x23f07410);
	halrf_wreg(rf, 0x8600, MASKDWORD, 0xe3570001);
	halrf_wreg(rf, 0x8604, MASKDWORD, 0x000023f0);
	halrf_wreg(rf, 0x8608, MASKDWORD, 0x7430e357);
	halrf_wreg(rf, 0x860c, MASKDWORD, 0x5b100001);
	halrf_wreg(rf, 0x8610, MASKDWORD, 0x20907410);
	halrf_wreg(rf, 0x8614, MASKDWORD, 0xe3570000);
	halrf_wreg(rf, 0x8618, MASKDWORD, 0x000123f0);
	halrf_wreg(rf, 0x861c, MASKDWORD, 0x23f0e357);
	halrf_wreg(rf, 0x8620, MASKDWORD, 0xe3570000);
	halrf_wreg(rf, 0x8624, MASKDWORD, 0xe2b35507);
	halrf_wreg(rf, 0x8628, MASKDWORD, 0x7410e2b3);
	halrf_wreg(rf, 0x862c, MASKDWORD, 0x02002098);
	halrf_wreg(rf, 0x8630, MASKDWORD, 0x23f0e357);
	halrf_wreg(rf, 0x8634, MASKDWORD, 0xe3570001);
	halrf_wreg(rf, 0x8638, MASKDWORD, 0x000023f0);
	halrf_wreg(rf, 0x863c, MASKDWORD, 0x5517e357);
	halrf_wreg(rf, 0x8640, MASKDWORD, 0x4f02e2b3);
	halrf_wreg(rf, 0x8644, MASKDWORD, 0x53024e00);
	halrf_wreg(rf, 0x8648, MASKDWORD, 0xe2b75201);
	halrf_wreg(rf, 0x864c, MASKDWORD, 0x30787509);
	halrf_wreg(rf, 0x8650, MASKDWORD, 0xe2c1e261);
	halrf_wreg(rf, 0x8654, MASKDWORD, 0xe2590001);
	halrf_wreg(rf, 0x8658, MASKDWORD, 0x0001e2c1);
	halrf_wreg(rf, 0x865c, MASKDWORD, 0x5b30e26d);
	halrf_wreg(rf, 0x8660, MASKDWORD, 0xe2af5500);
	halrf_wreg(rf, 0x8664, MASKDWORD, 0xe26d0001);
	halrf_wreg(rf, 0x8668, MASKDWORD, 0x0001e2f2);
	halrf_wreg(rf, 0x866c, MASKDWORD, 0x4380e265);
	halrf_wreg(rf, 0x8670, MASKDWORD, 0x0001e2f2);
	halrf_wreg(rf, 0x8674, MASKDWORD, 0x30e2e261);
	halrf_wreg(rf, 0x8678, MASKDWORD, 0xe3420023);
	halrf_wreg(rf, 0x867c, MASKDWORD, 0x54ed0002);
	halrf_wreg(rf, 0x8680, MASKDWORD, 0x00230baa);
	halrf_wreg(rf, 0x8684, MASKDWORD, 0x0002e342);
	halrf_wreg(rf, 0x8688, MASKDWORD, 0xe259e312);
	halrf_wreg(rf, 0x868c, MASKDWORD, 0xe2610001);
	halrf_wreg(rf, 0x8690, MASKDWORD, 0x002230dd);
	halrf_wreg(rf, 0x8694, MASKDWORD, 0x0002e342);
	halrf_wreg(rf, 0x8698, MASKDWORD, 0x0baa54ec);
	halrf_wreg(rf, 0x869c, MASKDWORD, 0xe3420022);
	halrf_wreg(rf, 0x86a0, MASKDWORD, 0xe3120002);
	halrf_wreg(rf, 0x86a4, MASKDWORD, 0x0001e259);
	halrf_wreg(rf, 0x86a8, MASKDWORD, 0x0baae261);
	halrf_wreg(rf, 0x86ac, MASKDWORD, 0x6d0f6c67);
	halrf_wreg(rf, 0x86b0, MASKDWORD, 0xe342e2c1);
	halrf_wreg(rf, 0x86b4, MASKDWORD, 0xe2c16c8b);
	halrf_wreg(rf, 0x86b8, MASKDWORD, 0x0bace342);
	halrf_wreg(rf, 0x86bc, MASKDWORD, 0x6d0f6cb3);
	halrf_wreg(rf, 0x86c0, MASKDWORD, 0xe342e2c1);
	halrf_wreg(rf, 0x86c4, MASKDWORD, 0x6cdb0bad);
	halrf_wreg(rf, 0x86c8, MASKDWORD, 0xe2c16d0f);
	halrf_wreg(rf, 0x86cc, MASKDWORD, 0x6cf7e342);
	halrf_wreg(rf, 0x86d0, MASKDWORD, 0xe2c16d0f);
	halrf_wreg(rf, 0x86d4, MASKDWORD, 0x6c09e342);
	halrf_wreg(rf, 0x86d8, MASKDWORD, 0xe2c16d00);
	halrf_wreg(rf, 0x86dc, MASKDWORD, 0x6c25e342);
	halrf_wreg(rf, 0x86e0, MASKDWORD, 0xe342e2c1);
	halrf_wreg(rf, 0x86e4, MASKDWORD, 0x6c4df8ca);
	halrf_wreg(rf, 0x86e8, MASKDWORD, 0xe342e2c1);
	halrf_wreg(rf, 0x86ec, MASKDWORD, 0x6c75f9d3);
	halrf_wreg(rf, 0x86f0, MASKDWORD, 0xe342e2c1);
	halrf_wreg(rf, 0x86f4, MASKDWORD, 0xe2c16c99);
	halrf_wreg(rf, 0x86f8, MASKDWORD, 0xe312e342);
	halrf_wreg(rf, 0x86fc, MASKDWORD, 0x0001e259);
	halrf_wreg(rf, 0x8700, MASKDWORD, 0x3141e26d);
	halrf_wreg(rf, 0x8704, MASKDWORD, 0xe3470022);
	halrf_wreg(rf, 0x8708, MASKDWORD, 0x54ec0002);
	halrf_wreg(rf, 0x870c, MASKDWORD, 0x00220baa);
	halrf_wreg(rf, 0x8710, MASKDWORD, 0x0002e347);
	halrf_wreg(rf, 0x8714, MASKDWORD, 0xe265e312);
	halrf_wreg(rf, 0x8718, MASKDWORD, 0xe26d0001);
	halrf_wreg(rf, 0x871c, MASKDWORD, 0xe3123131);
	halrf_wreg(rf, 0x8720, MASKDWORD, 0x0001e265);
	halrf_wreg(rf, 0x8724, MASKDWORD, 0x0ba6e26d);
	halrf_wreg(rf, 0x8728, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x872c, MASKDWORD, 0x6e670009);
	halrf_wreg(rf, 0x8730, MASKDWORD, 0xe30d6f0f);
	halrf_wreg(rf, 0x8734, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x8738, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x873c, MASKDWORD, 0x6e77000a);
	halrf_wreg(rf, 0x8740, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x8744, MASKDWORD, 0x7410e347);
	halrf_wreg(rf, 0x8748, MASKDWORD, 0x000b21e8);
	halrf_wreg(rf, 0x874c, MASKDWORD, 0xe30d6e8b);
	halrf_wreg(rf, 0x8750, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x8754, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8758, MASKDWORD, 0x6e9f000c);
	halrf_wreg(rf, 0x875c, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x8760, MASKDWORD, 0x0baae347);
	halrf_wreg(rf, 0x8764, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8768, MASKDWORD, 0x6eb3000d);
	halrf_wreg(rf, 0x876c, MASKDWORD, 0xe30d6f0f);
	halrf_wreg(rf, 0x8770, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x8774, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8778, MASKDWORD, 0x6ec7000e);
	halrf_wreg(rf, 0x877c, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x8780, MASKDWORD, 0x0bace347);
	halrf_wreg(rf, 0x8784, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8788, MASKDWORD, 0x6edb000f);
	halrf_wreg(rf, 0x878c, MASKDWORD, 0xe30d6f0f);
	halrf_wreg(rf, 0x8790, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x8794, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8798, MASKDWORD, 0x6eef0010);
	halrf_wreg(rf, 0x879c, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x87a0, MASKDWORD, 0xe347e347);
	halrf_wreg(rf, 0x87a4, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x87a8, MASKDWORD, 0x6e110013);
	halrf_wreg(rf, 0x87ac, MASKDWORD, 0xe30d6f00);
	halrf_wreg(rf, 0x87b0, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x87b4, MASKDWORD, 0x7410e347);
	halrf_wreg(rf, 0x87b8, MASKDWORD, 0x001421e8);
	halrf_wreg(rf, 0x87bc, MASKDWORD, 0xe30d6e25);
	halrf_wreg(rf, 0x87c0, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x87c4, MASKDWORD, 0x7410fba0);
	halrf_wreg(rf, 0x87c8, MASKDWORD, 0x001521e8);
	halrf_wreg(rf, 0x87cc, MASKDWORD, 0xe30d6e39);
	halrf_wreg(rf, 0x87d0, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x87d4, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x87d8, MASKDWORD, 0x6e4d0016);
	halrf_wreg(rf, 0x87dc, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x87e0, MASKDWORD, 0xfc9ee347);
	halrf_wreg(rf, 0x87e4, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x87e8, MASKDWORD, 0x6e610017);
	halrf_wreg(rf, 0x87ec, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x87f0, MASKDWORD, 0x7410e347);
	halrf_wreg(rf, 0x87f4, MASKDWORD, 0x001821e8);
	halrf_wreg(rf, 0x87f8, MASKDWORD, 0xe30d6e75);
	halrf_wreg(rf, 0x87fc, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x8800, MASKDWORD, 0x21e87410);
	halrf_wreg(rf, 0x8804, MASKDWORD, 0x6e890019);
	halrf_wreg(rf, 0x8808, MASKDWORD, 0xe2f2e30d);
	halrf_wreg(rf, 0x880c, MASKDWORD, 0x7410e347);
	halrf_wreg(rf, 0x8810, MASKDWORD, 0x001a21e8);
	halrf_wreg(rf, 0x8814, MASKDWORD, 0xe30d6e99);
	halrf_wreg(rf, 0x8818, MASKDWORD, 0xe347e2f2);
	halrf_wreg(rf, 0x881c, MASKDWORD, 0xe265e312);
	halrf_wreg(rf, 0x8820, MASKDWORD, 0x00040001);
	halrf_wreg(rf, 0x8824, MASKDWORD, 0x0007775c);
	halrf_wreg(rf, 0x8828, MASKDWORD, 0x62006220);
	halrf_wreg(rf, 0x882c, MASKDWORD, 0x55010004);
	halrf_wreg(rf, 0x8830, MASKDWORD, 0xe2af5b00);
	halrf_wreg(rf, 0x8834, MASKDWORD, 0x66055b40);
	halrf_wreg(rf, 0x8838, MASKDWORD, 0x62000007);
	halrf_wreg(rf, 0x883c, MASKDWORD, 0xe3326300);
	halrf_wreg(rf, 0x8840, MASKDWORD, 0xe2af0004);
	halrf_wreg(rf, 0x8844, MASKDWORD, 0x0a010900);
	halrf_wreg(rf, 0x8848, MASKDWORD, 0x0d000b40);
	halrf_wreg(rf, 0x884c, MASKDWORD, 0x00320e01);
	halrf_wreg(rf, 0x8850, MASKDWORD, 0x95060004);
	halrf_wreg(rf, 0x8854, MASKDWORD, 0x00074380);
	halrf_wreg(rf, 0x8858, MASKDWORD, 0x00044d01);
	halrf_wreg(rf, 0x885c, MASKDWORD, 0x00074300);
	halrf_wreg(rf, 0x8860, MASKDWORD, 0x05a30562);
	halrf_wreg(rf, 0x8864, MASKDWORD, 0xe3329617);
	halrf_wreg(rf, 0x8868, MASKDWORD, 0xe2af0004);
	halrf_wreg(rf, 0x886c, MASKDWORD, 0x06a20007);
	halrf_wreg(rf, 0x8870, MASKDWORD, 0xe33207a3);
	halrf_wreg(rf, 0x8874, MASKDWORD, 0xe2af0004);
	halrf_wreg(rf, 0x8878, MASKDWORD, 0x0002e322);
	halrf_wreg(rf, 0x887c, MASKDWORD, 0x4380e32a);
	halrf_wreg(rf, 0x8880, MASKDWORD, 0x4d000007);
	halrf_wreg(rf, 0x8884, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x8888, MASKDWORD, 0x00017900);
	halrf_wreg(rf, 0x888c, MASKDWORD, 0x775e0004);
	halrf_wreg(rf, 0x8890, MASKDWORD, 0x00073193);
	halrf_wreg(rf, 0x8894, MASKDWORD, 0x07a306a2);
	halrf_wreg(rf, 0x8898, MASKDWORD, 0xe27131bd);
	halrf_wreg(rf, 0x889c, MASKDWORD, 0x73000005);
	halrf_wreg(rf, 0x88a0, MASKDWORD, 0xe2710001);
	halrf_wreg(rf, 0x88a4, MASKDWORD, 0x5d000006);
	halrf_wreg(rf, 0x88a8, MASKDWORD, 0x42f70004);
	halrf_wreg(rf, 0x88ac, MASKDWORD, 0x6c000005);
	halrf_wreg(rf, 0x88b0, MASKDWORD, 0x42000004);
	halrf_wreg(rf, 0x88b4, MASKDWORD, 0x0004e289);
	halrf_wreg(rf, 0x88b8, MASKDWORD, 0x00074380);
	halrf_wreg(rf, 0x88bc, MASKDWORD, 0x4a004e00);
	halrf_wreg(rf, 0x88c0, MASKDWORD, 0x00064c00);
	halrf_wreg(rf, 0x88c4, MASKDWORD, 0x60007f00);
	halrf_wreg(rf, 0x88c8, MASKDWORD, 0x00046f00);
	halrf_wreg(rf, 0x88cc, MASKDWORD, 0x00054300);
	halrf_wreg(rf, 0x88d0, MASKDWORD, 0x00017300);
	halrf_wreg(rf, 0x88d4, MASKDWORD, 0xe2710001);
	halrf_wreg(rf, 0x88d8, MASKDWORD, 0x5d010006);
	halrf_wreg(rf, 0x88dc, MASKDWORD, 0x60025601);
	halrf_wreg(rf, 0x88e0, MASKDWORD, 0x00056100);
	halrf_wreg(rf, 0x88e4, MASKDWORD, 0xe2897710);
	halrf_wreg(rf, 0x88e8, MASKDWORD, 0x73000005);
	halrf_wreg(rf, 0x88ec, MASKDWORD, 0x43800004);
	halrf_wreg(rf, 0x88f0, MASKDWORD, 0x5e010007);
	halrf_wreg(rf, 0x88f4, MASKDWORD, 0x4d205e00);
	halrf_wreg(rf, 0x88f8, MASKDWORD, 0x4a084e20);
	halrf_wreg(rf, 0x88fc, MASKDWORD, 0x4c3f4960);
	halrf_wreg(rf, 0x8900, MASKDWORD, 0x7f010006);
	halrf_wreg(rf, 0x8904, MASKDWORD, 0x60106380);
	halrf_wreg(rf, 0x8908, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x890c, MASKDWORD, 0x74020006);
	halrf_wreg(rf, 0x8910, MASKDWORD, 0x40010007);
	halrf_wreg(rf, 0x8914, MASKDWORD, 0xab004000);
	halrf_wreg(rf, 0x8918, MASKDWORD, 0x74040006);
	halrf_wreg(rf, 0x891c, MASKDWORD, 0x40010007);
	halrf_wreg(rf, 0x8920, MASKDWORD, 0xab004000);
	halrf_wreg(rf, 0x8924, MASKDWORD, 0x43800004);
	halrf_wreg(rf, 0x8928, MASKDWORD, 0x4d000007);
	halrf_wreg(rf, 0x892c, MASKDWORD, 0x4a004e00);
	halrf_wreg(rf, 0x8930, MASKDWORD, 0x00064c00);
	halrf_wreg(rf, 0x8934, MASKDWORD, 0x63007f00);
	halrf_wreg(rf, 0x8938, MASKDWORD, 0x6f006000);
	halrf_wreg(rf, 0x893c, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x8940, MASKDWORD, 0x00040001);
	halrf_wreg(rf, 0x8944, MASKDWORD, 0x42bf4380);
	halrf_wreg(rf, 0x8948, MASKDWORD, 0x48400007);
	halrf_wreg(rf, 0x894c, MASKDWORD, 0x42ef0004);
	halrf_wreg(rf, 0x8950, MASKDWORD, 0x4d100007);
	halrf_wreg(rf, 0x8954, MASKDWORD, 0x42000004);
	halrf_wreg(rf, 0x8958, MASKDWORD, 0x5f800006);
	halrf_wreg(rf, 0x895c, MASKDWORD, 0x5a010007);
	halrf_wreg(rf, 0x8960, MASKDWORD, 0x00044a08);
	halrf_wreg(rf, 0x8964, MASKDWORD, 0x00054300);
	halrf_wreg(rf, 0x8968, MASKDWORD, 0x73807381);
	halrf_wreg(rf, 0x896c, MASKDWORD, 0x003f9300);
	halrf_wreg(rf, 0x8970, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8974, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8978, MASKDWORD, 0x00020000);
	halrf_wreg(rf, 0x897c, MASKDWORD, 0x5f800006);
	halrf_wreg(rf, 0x8980, MASKDWORD, 0x99005f00);
	halrf_wreg(rf, 0x8984, MASKDWORD, 0x43800004);
	halrf_wreg(rf, 0x8988, MASKDWORD, 0x00074280);
	halrf_wreg(rf, 0x898c, MASKDWORD, 0x00044800);
	halrf_wreg(rf, 0x8990, MASKDWORD, 0x000742ef);
	halrf_wreg(rf, 0x8994, MASKDWORD, 0x00044d00);
	halrf_wreg(rf, 0x8998, MASKDWORD, 0x00064200);
	halrf_wreg(rf, 0x899c, MASKDWORD, 0x60005f00);
	halrf_wreg(rf, 0x89a0, MASKDWORD, 0x5a000007);
	halrf_wreg(rf, 0x89a4, MASKDWORD, 0x48004a00);
	halrf_wreg(rf, 0x89a8, MASKDWORD, 0x43000004);
	halrf_wreg(rf, 0x89ac, MASKDWORD, 0x73000005);
	halrf_wreg(rf, 0x89b0, MASKDWORD, 0x43800001);
	halrf_wreg(rf, 0x89b4, MASKDWORD, 0x78006505);
	halrf_wreg(rf, 0x89b8, MASKDWORD, 0x7a007900);
	halrf_wreg(rf, 0x89bc, MASKDWORD, 0x43007b00);
	halrf_wreg(rf, 0x89c0, MASKDWORD, 0x43800001);
	halrf_wreg(rf, 0x89c4, MASKDWORD, 0x43006500);
	halrf_wreg(rf, 0x89c8, MASKDWORD, 0x43800001);
	halrf_wreg(rf, 0x89cc, MASKDWORD, 0x7c006405);
	halrf_wreg(rf, 0x89d0, MASKDWORD, 0x7e007d00);
	halrf_wreg(rf, 0x89d4, MASKDWORD, 0x43007f00);
	halrf_wreg(rf, 0x89d8, MASKDWORD, 0x43800001);
	halrf_wreg(rf, 0x89dc, MASKDWORD, 0x43006400);
	halrf_wreg(rf, 0x89e0, MASKDWORD, 0x00060001);
	halrf_wreg(rf, 0x89e4, MASKDWORD, 0x55025601);
	halrf_wreg(rf, 0x89e8, MASKDWORD, 0x00055400);
	halrf_wreg(rf, 0x89ec, MASKDWORD, 0x7e127f00);
	halrf_wreg(rf, 0x89f0, MASKDWORD, 0x76007710);
	halrf_wreg(rf, 0x89f4, MASKDWORD, 0x74007500);
	halrf_wreg(rf, 0x89f8, MASKDWORD, 0x42700004);
	halrf_wreg(rf, 0x89fc, MASKDWORD, 0x73810005);
	halrf_wreg(rf, 0x8a00, MASKDWORD, 0x00047380);
	halrf_wreg(rf, 0x8a04, MASKDWORD, 0x93004200);
	halrf_wreg(rf, 0x8a08, MASKDWORD, 0x77000005);
	halrf_wreg(rf, 0x8a0c, MASKDWORD, 0x56000006);
	halrf_wreg(rf, 0x8a10, MASKDWORD, 0x00060001);
	halrf_wreg(rf, 0x8a14, MASKDWORD, 0x5f005f80);
	halrf_wreg(rf, 0x8a18, MASKDWORD, 0x00059900);
	halrf_wreg(rf, 0x8a1c, MASKDWORD, 0x00067300);
	halrf_wreg(rf, 0x8a20, MASKDWORD, 0x63006380);
	halrf_wreg(rf, 0x8a24, MASKDWORD, 0x00019800);
	halrf_wreg(rf, 0x8a28, MASKDWORD, 0x7b484380);
	halrf_wreg(rf, 0x8a2c, MASKDWORD, 0x79007a90);
	halrf_wreg(rf, 0x8a30, MASKDWORD, 0x43007802);
	halrf_wreg(rf, 0x8a34, MASKDWORD, 0x32ab5503);
	halrf_wreg(rf, 0x8a38, MASKDWORD, 0x7b384380);
	halrf_wreg(rf, 0x8a3c, MASKDWORD, 0x79007a80);
	halrf_wreg(rf, 0x8a40, MASKDWORD, 0x43007802);
	halrf_wreg(rf, 0x8a44, MASKDWORD, 0x32ab5513);
	halrf_wreg(rf, 0x8a48, MASKDWORD, 0x7b404380);
	halrf_wreg(rf, 0x8a4c, MASKDWORD, 0x79007a00);
	halrf_wreg(rf, 0x8a50, MASKDWORD, 0x43007802);
	halrf_wreg(rf, 0x8a54, MASKDWORD, 0x74315523);
	halrf_wreg(rf, 0x8a58, MASKDWORD, 0x8e007430);
	halrf_wreg(rf, 0x8a5c, MASKDWORD, 0x74010001);
	halrf_wreg(rf, 0x8a60, MASKDWORD, 0x8e007400);
	halrf_wreg(rf, 0x8a64, MASKDWORD, 0x74310001);
	halrf_wreg(rf, 0x8a68, MASKDWORD, 0x8e007430);
	halrf_wreg(rf, 0x8a6c, MASKDWORD, 0x57020001);
	halrf_wreg(rf, 0x8a70, MASKDWORD, 0x97005700);
	halrf_wreg(rf, 0x8a74, MASKDWORD, 0x42ef0001);
	halrf_wreg(rf, 0x8a78, MASKDWORD, 0x56005610);
	halrf_wreg(rf, 0x8a7c, MASKDWORD, 0x8c004200);
	halrf_wreg(rf, 0x8a80, MASKDWORD, 0x4f780001);
	halrf_wreg(rf, 0x8a84, MASKDWORD, 0x53884e00);
	halrf_wreg(rf, 0x8a88, MASKDWORD, 0x5b205201);
	halrf_wreg(rf, 0x8a8c, MASKDWORD, 0x5480e2d2);
	halrf_wreg(rf, 0x8a90, MASKDWORD, 0xe2d25400);
	halrf_wreg(rf, 0x8a94, MASKDWORD, 0x54005481);
	halrf_wreg(rf, 0x8a98, MASKDWORD, 0x5482e2d2);
	halrf_wreg(rf, 0x8a9c, MASKDWORD, 0xe2dd5400);
	halrf_wreg(rf, 0x8aa0, MASKDWORD, 0x3012bf1d);
	halrf_wreg(rf, 0x8aa4, MASKDWORD, 0xe29ce294);
	halrf_wreg(rf, 0x8aa8, MASKDWORD, 0xe2b7e2a4);
	halrf_wreg(rf, 0x8aac, MASKDWORD, 0x5523e33b);
	halrf_wreg(rf, 0x8ab0, MASKDWORD, 0x5525e2ab);
	halrf_wreg(rf, 0x8ab4, MASKDWORD, 0xe33be2b7);
	halrf_wreg(rf, 0x8ab8, MASKDWORD, 0x54bf0001);
	halrf_wreg(rf, 0x8abc, MASKDWORD, 0x54a354c0);
	halrf_wreg(rf, 0x8ac0, MASKDWORD, 0x54a454c1);
	halrf_wreg(rf, 0x8ac4, MASKDWORD, 0xbf074c18);
	halrf_wreg(rf, 0x8ac8, MASKDWORD, 0x54a454c2);
	halrf_wreg(rf, 0x8acc, MASKDWORD, 0x54c1bf04);
	halrf_wreg(rf, 0x8ad0, MASKDWORD, 0xbf0154a3);
	halrf_wreg(rf, 0x8ad4, MASKDWORD, 0x54dfe34c);
	halrf_wreg(rf, 0x8ad8, MASKDWORD, 0x54bf0001);
	halrf_wreg(rf, 0x8adc, MASKDWORD, 0x050a54e5);
	halrf_wreg(rf, 0x8ae0, MASKDWORD, 0x000154df);
	halrf_wreg(rf, 0x8ae4, MASKDWORD, 0x43807b80);
	halrf_wreg(rf, 0x8ae8, MASKDWORD, 0x7e007f40);
	halrf_wreg(rf, 0x8aec, MASKDWORD, 0x7c027d00);
	halrf_wreg(rf, 0x8af0, MASKDWORD, 0x5b404300);
	halrf_wreg(rf, 0x8af4, MASKDWORD, 0x5c015501);
	halrf_wreg(rf, 0x8af8, MASKDWORD, 0x5480e2bb);
	halrf_wreg(rf, 0x8afc, MASKDWORD, 0xe2bb5400);
	halrf_wreg(rf, 0x8b00, MASKDWORD, 0x54005481);
	halrf_wreg(rf, 0x8b04, MASKDWORD, 0x5482e2bb);
	halrf_wreg(rf, 0x8b08, MASKDWORD, 0x7b005400);
	halrf_wreg(rf, 0x8b0c, MASKDWORD, 0xbfe6e2dd);
	halrf_wreg(rf, 0x8b10, MASKDWORD, 0x56103012);
	halrf_wreg(rf, 0x8b14, MASKDWORD, 0x8c005600);
	halrf_wreg(rf, 0x8b18, MASKDWORD, 0xe34f0001);
	halrf_wreg(rf, 0x8b1c, MASKDWORD, 0xe34fe34f);
	halrf_wreg(rf, 0x8b20, MASKDWORD, 0x0001e34f);
	halrf_wreg(rf, 0x8b24, MASKDWORD, 0x57005704);
	halrf_wreg(rf, 0x8b28, MASKDWORD, 0x57089700);
	halrf_wreg(rf, 0x8b2c, MASKDWORD, 0x97005700);
	halrf_wreg(rf, 0x8b30, MASKDWORD, 0x57805781);
	halrf_wreg(rf, 0x8b34, MASKDWORD, 0x43809700);
	halrf_wreg(rf, 0x8b38, MASKDWORD, 0x5c010007);
	halrf_wreg(rf, 0x8b3c, MASKDWORD, 0x00045c00);
	halrf_wreg(rf, 0x8b40, MASKDWORD, 0x00014300);
	halrf_wreg(rf, 0x8b44, MASKDWORD, 0x0007427f);
	halrf_wreg(rf, 0x8b48, MASKDWORD, 0x62006280);
	halrf_wreg(rf, 0x8b4c, MASKDWORD, 0x00049200);
	halrf_wreg(rf, 0x8b50, MASKDWORD, 0x00014200);
	halrf_wreg(rf, 0x8b54, MASKDWORD, 0x0007427f);
	halrf_wreg(rf, 0x8b58, MASKDWORD, 0x63146394);
	halrf_wreg(rf, 0x8b5c, MASKDWORD, 0x00049100);
	halrf_wreg(rf, 0x8b60, MASKDWORD, 0x00014200);
	halrf_wreg(rf, 0x8b64, MASKDWORD, 0x79010004);
	halrf_wreg(rf, 0x8b68, MASKDWORD, 0xe3577420);
	halrf_wreg(rf, 0x8b6c, MASKDWORD, 0x57005710);
	halrf_wreg(rf, 0x8b70, MASKDWORD, 0xe357e357);
	halrf_wreg(rf, 0x8b74, MASKDWORD, 0x549f0001);
	halrf_wreg(rf, 0x8b78, MASKDWORD, 0x5c015400);
	halrf_wreg(rf, 0x8b7c, MASKDWORD, 0x540054df);
	halrf_wreg(rf, 0x8b80, MASKDWORD, 0x00015c02);
	halrf_wreg(rf, 0x8b84, MASKDWORD, 0x07145c01);
	halrf_wreg(rf, 0x8b88, MASKDWORD, 0x5c025400);
	halrf_wreg(rf, 0x8b8c, MASKDWORD, 0x5c020001);
	halrf_wreg(rf, 0x8b90, MASKDWORD, 0x54000714);
	halrf_wreg(rf, 0x8b94, MASKDWORD, 0x00015c01);
	halrf_wreg(rf, 0x8b98, MASKDWORD, 0x4c184c98);
	halrf_wreg(rf, 0x8b9c, MASKDWORD, 0x003f0001);
	halrf_wreg(rf, 0x8ba0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ba4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8ba8, MASKDWORD, 0x00020000);
	halrf_wreg(rf, 0x8bac, MASKDWORD, 0x00000001);
	halrf_wreg(rf, 0x8bb0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8bb4, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8bb8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8bbc, MASKDWORD, 0x00010000);
	halrf_wreg(rf, 0x8bc0, MASKDWORD, 0x5c020004);
	halrf_wreg(rf, 0x8bc4, MASKDWORD, 0x66076204);
	halrf_wreg(rf, 0x8bc8, MASKDWORD, 0x743070c0);
	halrf_wreg(rf, 0x8bcc, MASKDWORD, 0x0c010901);
	halrf_wreg(rf, 0x8bd0, MASKDWORD, 0x00010ba6);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000004);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x00000000);

}

void _halrf_txgapk_bb_afe_by_mode_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path, bool is_dbcc)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	if (!is_dbcc) {
		/* nodbcc */
		halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0303);
		halrf_wreg(rf, 0x5864, 0x18000000, 0x3);
		halrf_wreg(rf, 0x7864, 0x18000000, 0x3);
		halrf_wreg(rf, 0x12b8, 0x40000000, 0x1);
		halrf_wreg(rf, 0x32b8, 0x40000000, 0x1);
		halrf_wreg(rf, 0x030c, 0xff000000, 0x13);
		halrf_wreg(rf, 0x032c, 0xffff0000, 0x0041);
		halrf_wreg(rf, 0x12b8, 0x10000000, 0x1);
		halrf_wreg(rf, 0x58c8, 0x01000000, 0x1);
		halrf_wreg(rf, 0x78c8, 0x01000000, 0x1);
		halrf_wreg(rf, 0x5864, 0xc0000000, 0x3);
		halrf_wreg(rf, 0x7864, 0xc0000000, 0x3);
		halrf_wreg(rf, 0x2008, 0x01ffffff, 0x1ffffff);
		halrf_wreg(rf, 0x0c1c, 0x00000004, 0x1);
		halrf_wreg(rf, 0x0700, 0x08000000, 0x1);
		halrf_wreg(rf, 0x0c70, 0x000003ff, 0x3ff);
		halrf_wreg(rf, 0x0c60, 0x00000003, 0x3);
		halrf_wreg(rf, 0x0c6c, 0x00000001, 0x1);
		halrf_wreg(rf, 0x58ac, 0x08000000, 0x1);
		halrf_wreg(rf, 0x78ac, 0x08000000, 0x1);
#ifdef  CF_PHL_BB_CTRL_RX_CCA
		halrf_bb_ctrl_rx_cca(rf, false, phy);
#else			
		halrf_wreg(rf, 0x0c3c, 0x00000200, 0x1);	
		halrf_wreg(rf, 0x2344, 0x80000000, 0x1);

		halrf_wreg(rf, 0x0704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x0704, BIT(1), 0x1);
		halrf_delay_us(rf, 1);
#endif
		
		halrf_wreg(rf, 0x4490, 0x80000000, 0x1);
		halrf_wreg(rf, 0x12a0, 0x00007000, 0x7);
		halrf_wreg(rf, 0x12a0, 0x00008000, 0x1);
		halrf_wreg(rf, 0x12a0, 0x00070000, 0x3);
		halrf_wreg(rf, 0x12a0, 0x00080000, 0x1);
		halrf_wreg(rf, 0x32a0, 0x00070000, 0x3);
		halrf_wreg(rf, 0x32a0, 0x00080000, 0x1);
		halrf_wreg(rf, 0x0700, 0x01000000, 0x1);
		halrf_wreg(rf, 0x0700, 0x06000000, 0x2);
		halrf_wreg(rf, 0x20fc, 0xffff0000, 0x3333);
	} else {
		if (phy == HW_PHY_0) {
			/* dbcc phy0 path 0 */
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0101);
			halrf_wreg(rf, 0x5864, 0x18000000, 0x3);
			halrf_wreg(rf, 0x7864, 0x18000000, 0x3);
			halrf_wreg(rf, 0x12b8, 0x40000000, 0x1);
			halrf_wreg(rf, 0x030c, 0xff000000, 0x13);
			halrf_wreg(rf, 0x032c, 0xffff0000, 0x0041);
			halrf_wreg(rf, 0x12b8, 0x10000000, 0x1);
			halrf_wreg(rf, 0x58c8, 0x01000000, 0x1);
			halrf_wreg(rf, 0x5864, 0xc0000000, 0x3);
			halrf_wreg(rf, 0x2008, 0x01ffffff, 0x1ffffff);
			halrf_wreg(rf, 0x0c1c, 0x00000004, 0x1);
			halrf_wreg(rf, 0x0700, 0x08000000, 0x1);
			halrf_wreg(rf, 0x0c70, 0x000003ff, 0x3ff);
			halrf_wreg(rf, 0x0c60, 0x00000003, 0x3);
			halrf_wreg(rf, 0x0c6c, 0x00000001, 0x1);
			halrf_wreg(rf, 0x58ac, 0x08000000, 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
			halrf_bb_ctrl_rx_cca(rf, false, phy);
#else			
			halrf_wreg(rf, 0x0c3c, 0x00000200, 0x1);
			/* halrf_wreg(rf, 0x2344, 0x80000000, 0x1); */		
			if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x0) /*CCK @PHY0*/
				halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/
			
			halrf_wreg(rf, 0x0704, BIT(1), 0x0); /*bb rst*/
			halrf_wreg(rf, 0x0704, BIT(1), 0x1);
			halrf_delay_us(rf, 1);
#endif				
			halrf_wreg(rf, 0x4490, 0x80000000, 0x1);
			halrf_wreg(rf, 0x12a0, 0x00007000, 0x7);
			halrf_wreg(rf, 0x12a0, 0x00008000, 0x1);
			halrf_wreg(rf, 0x12a0, 0x00070000, 0x3);
			halrf_wreg(rf, 0x12a0, 0x00080000, 0x1);
			halrf_wreg(rf, 0x0700, 0x01000000, 0x1);
			halrf_wreg(rf, 0x0700, 0x06000000, 0x2);					
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x1111);
		} else if (phy == HW_PHY_1) {
			/* dbcc phy1 path 1 */
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0202);
			halrf_wreg(rf, 0x7864, 0x18000000, 0x3);
			halrf_wreg(rf, 0x32b8, 0x40000000, 0x1);
			halrf_wreg(rf, 0x030c, 0xff000000, 0x13);
			halrf_wreg(rf, 0x032c, 0xffff0000, 0x0041);
			halrf_wreg(rf, 0x32b8, 0x10000000, 0x1);
			halrf_wreg(rf, 0x78c8, 0x01000000, 0x1);
			halrf_wreg(rf, 0x7864, 0xc0000000, 0x3);
			halrf_wreg(rf, 0x2008, 0x01ffffff, 0x1ffffff);
			halrf_wreg(rf, 0x2c1c, 0x00000004, 0x1);
			halrf_wreg(rf, 0x2700, 0x08000000, 0x1);
			halrf_wreg(rf, 0x0c70, 0x000003ff, 0x3ff);
			halrf_wreg(rf, 0x0c60, 0x00000003, 0x3);
			halrf_wreg(rf, 0x0c6c, 0x00000001, 0x1);
			halrf_wreg(rf, 0x78ac, 0x08000000, 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
			halrf_bb_ctrl_rx_cca(rf, false, phy);
#else			
			halrf_wreg(rf, 0x2c3c, 0x00000200, 0x1);

			if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x1) /*CCK @PHY1*/
				halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/

			halrf_wreg(rf, 0x2704, BIT(1), 0x0); /*bb rst*/
			halrf_wreg(rf, 0x2704, BIT(1), 0x1);
			halrf_delay_us(rf, 1);
#endif			
			halrf_wreg(rf, 0x6490, 0x80000000, 0x1);
			halrf_wreg(rf, 0x32a0, 0x00007000, 0x7);
			halrf_wreg(rf, 0x32a0, 0x00008000, 0x1);
			halrf_wreg(rf, 0x32a0, 0x00070000, 0x3);
			halrf_wreg(rf, 0x32a0, 0x00080000, 0x1);
			halrf_wreg(rf, 0x2700, 0x01000000, 0x1);
			halrf_wreg(rf, 0x2700, 0x06000000, 0x2);
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x2222);
		}
	}
		
}




void _halrf_txgapk_iqk_preset_by_mode_8852a(struct rf_info *rf,
					enum phl_phy_idx phy, enum rf_path path, bool is_dbcc)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	if (!is_dbcc) {
		/* nodbcc */
		halrf_wrf(rf, RF_PATH_A, 0x5, 0x00001, 0x0);
		halrf_wrf(rf, RF_PATH_B, 0x5, 0x00001, 0x0);
		halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000080);
		halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
		halrf_wreg(rf, 0x8088, MASKDWORD, 0x81ff010a);
		halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00200000);
		halrf_wreg(rf, 0x8074, MASKDWORD, 0x80000000);
		halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000000);
		halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000000);
	} else {
		/* dbcc */
		if (phy == HW_PHY_0)
			halrf_wrf(rf, RF_PATH_A, 0x5, 0x00001, 0x0);
		else if (phy == HW_PHY_1)
			halrf_wrf(rf, RF_PATH_B, 0x5, 0x00001, 0x0);
			
		halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000080);
		halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
		halrf_wreg(rf, 0x8088, MASKDWORD, 0x83ff010a);
		halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00200000);
		halrf_wreg(rf, 0x8074, MASKDWORD, 0x80000000);

		if (phy == HW_PHY_0)
			halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000000);
		else if (phy == HW_PHY_1)
			halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000000);
	}
	
}



void _halrf_txgapk_clk_setting_dac960mhz_by_mode_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_dbcc)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	if (!is_dbcc) {
		/* nodbcc */
		halrf_wreg(rf, 0x8120, MASKDWORD, 0xce000a08);
		halrf_wreg(rf, 0x8220, MASKDWORD, 0xce000a08);
	} else {
		/* dbcc */
		if (phy == HW_PHY_0)
			halrf_wreg(rf, 0x8120, MASKDWORD, 0xce000a08);
		else if (phy == HW_PHY_1)
			halrf_wreg(rf, 0x8220, MASKDWORD, 0xce000a08);

	}
	
}

void _halrf_txgapk_track_table_nctl_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	u32 i, rf_tmp;
	u32 d[17] = {0}, ta[17] = {0};
	u32 bb_reg[6] = {0x8124, 0x8138, 0x813c,
			0x8224, 0x8238, 0x823c};
	u32 bb_reg_backup[6] = {0};
	u32 backup_num = 6;
	u32 itqt[2] = {0x81cc, 0x82cc};
	u32 gapk_on_table0_setting[2] = {0x8158, 0x8258};
	u32 path_setting[2] = {0x5864, 0x7864};
	u32 cal_path[2] = {0x00002019, 0x00002029};
	u32 disable_cfir_en[2] = {0x8124, 0x8224};
	u32 listen_iqc_cfir[2] = {0x8138, 0x8238};
	u32 listen_cfir_iqc[2] = {0x813c, 0x823c};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0x05, 0x00001, 0x0);
	halrf_wrf(rf, path, 0x01, 0xfffff, 0x35);
	/* halrf_wrf(rf, path, 0x00, 0xf0000, 0x5); */
	halrf_wrf(rf, path, 0x00, MASKRF, 0x50001 | BIT(rf->hal_com->dbcc_en)); /* if exit LPS */
	halrf_wrf(rf, path, 0x00, 0x00400, 0x0);
	halrf_wrf(rf, path, 0x00, 0x07800, 0x0);
	halrf_wrf(rf, path, 0x00, 0x003e0, 0x1f);
	halrf_wrf(rf, path, 0x83, 0x00007, 0x0);
	halrf_wrf(rf, path, 0x83, 0x000f0, 0x4);
	halrf_wrf(rf, path, 0x9f, 0x00018, 0x0);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x3f);
	halrf_wrf(rf, path, 0x94, 0x000fc, 0x3f);
	halrf_wrf(rf, path, 0x93, 0x00008, 0x0);
	halrf_wrf(rf, path, 0x92, 0x00001, 0x0);
	halrf_wrf(rf, path, 0x92, 0x00001, 0x1);

	//for (i = 0; i < 1000; i++)
	//	halrf_delay_us(rf, 1);
	halrf_wrf(rf, path, 0x92, 0x00001, 0x0);

	//for (i = 0; i < 1000; i++)
	//	halrf_delay_us(rf, 1);

	halrf_wrf(rf, path, 0x5c, 0x80000, 0x1);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x1);
	halrf_wrf(rf, path, 0x5e, 0x3f000, 0x22);
	halrf_wrf(rf, path, 0xde, 0x00004, 0x0);

	_txgapk_backup_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	halrf_wreg(rf, disable_cfir_en[path], MASKDWORD, 0x00000000);
	halrf_wreg(rf, listen_iqc_cfir[path], MASKDWORD, 0x40000002);
	halrf_wreg(rf, listen_cfir_iqc[path], MASKDWORD, 0x40000002);

	rf_tmp = halrf_rrf(rf, path, 0x0, 0xfffff);
	halrf_wreg(rf, 0x8024, 0x000fffff, rf_tmp);

	halrf_wreg(rf, 0x801c, 0x000e0000, 0x3);
	halrf_wreg(rf, 0x80e0, 0x00000001, 0x1);
	halrf_wreg(rf, 0x80e0, 0x00000002, 0x0);
	halrf_wreg(rf, 0x80e0, 0x000001f0, 0x1f);
	halrf_wreg(rf, 0x80e0, 0x0000f000, 0x0);
	halrf_wreg(rf, 0x8038, 0x00000038, 0x2);
	if (rfe_type <= 50) {
		/* NIC */
		/* AP iFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000555);
	} else {
		/* AP eFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000aaa);
	}
	/* halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000555); */

	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x8158, 0xffe00000, 0x7ff);
		halrf_wreg(rf, 0x815c, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x815c, 0x7ffffc00, 0x005555);
	}

	rf_tmp = halrf_rrf(rf, path, 0x56, 0xfffff);
	halrf_wreg(rf, 0x8040, 0x000fffff, rf_tmp);

	rf_tmp = halrf_rrf(rf, path, 0x5c, 0xc0000);
	halrf_wreg(rf, 0x80e4, 0x30000000, rf_tmp);

	rf_tmp = halrf_rrf(rf, path, 0x5c, 0x007ff);
	halrf_wreg(rf, 0x80e4, 0x07ff0000, rf_tmp);

	//halrf_rrf(rf, path, 0x5c, 0xfffff);

	halrf_wreg(rf, 0xa60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	halrf_wreg(rf, path_setting[path], 0x20000000, 0x1);

	//halrf_rreg(rf, 0x80e0, MASKDWORD);

	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x802c, 0x0fff0000, 0x009);
	halrf_wreg(rf, 0x8000, MASKDWORD, cal_path[path]);

	//for (i = 0; i < 100; i++)
	//	halrf_delay_us(rf, 1);

	halrf_wreg(rf, 0x80b0, 0x10000000, 0x1);

	for (i = 0; i < 1000; i++)
		halrf_delay_us(rf, 1);

	halrf_wreg(rf, path_setting[path], 0x20000000, 0x0);

	//halrf_rrf(rf, path, 0x5c, 0xfffff);

	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00130000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x3);

	d[0] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[1] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[2] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[3] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x4);

	d[4] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[5] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[6] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[7] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x5);

	d[8] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[9] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[10] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[11] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x6);

	d[12] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[13] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[14] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[15] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x7);

	d[16] = halrf_rreg(rf, 0x80fc, 0x0000007f);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x9);

	ta[0] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[1] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[2] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[3] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xa);
	
	ta[4] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[5] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[6] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[7] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xb);

	ta[8] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[9] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[10]= halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[11] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xc);

	ta[12] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[13] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[14] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[15] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xd);
	
	ta[16] = halrf_rreg(rf, 0x80fc, 0x000000ff);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	//halrf_rreg(rf, 0x80fc, 0x0000007f);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x0);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	//halrf_rreg(rf, 0x80fc, 0x007f0000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x1);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xf);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	
	halrf_rrf(rf, path, 0x5c, 0xfffff);

	_txgapk_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	halrf_wreg(rf, 0x801c, 0x000e0000, 0x0);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x12);
	halrf_wreg(rf, 0x80d0, 0x00100000, 0x1);

	for (i = 0; i < 17; i++) {
		if (d[i] & BIT(6))
			txgapk_info->track_d[path][i] = (s32)(d[i] | 0xffffff80);
		else
			txgapk_info->track_d[path][i] = (s32)(d[i]);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	track	d[%d][%d]=0x%x\n",
			path, i, txgapk_info->track_d[path][i]);
	}

	for (i = 0; i < 17; i++) {
		if (ta[i] & BIT(7))
			txgapk_info->track_ta[path][i] = (s32)(ta[i] | 0xffffff00);
		else
			txgapk_info->track_ta[path][i] = (s32)(ta[i]);
		
		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	track	ta[%d][%d]=0x%x\n",
			path, i, txgapk_info->track_ta[path][i]);
	}

	//halrf_rrf(rf, path, 0x5c, 0xfffff);
	//halrf_rrf(rf, path, 0x5c, 0x3f800);
}

void _halrf_txgapk_track_table_nctl_5g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;

	u32 i, rf_tmp;
	u32 d[17] = {0}, ta[17] = {0};
	u32 bb_reg[6] = {0x8124, 0x8138, 0x813c,
			0x8224, 0x8238, 0x823c};
	u32 bb_reg_backup[6] = {0};
	u32 backup_num = 6;
	u32 itqt[2] = {0x81cc, 0x82cc};
	u32 gapk_on_table0_setting[2] = {0x8158, 0x8258};
	u32 path_setting[2] = {0x5864, 0x7864};
	u32 cal_path[2] = {0x00002019, 0x00002029};
	u32 disable_cfir_en[2] = {0x8124, 0x8224};
	u32 listen_iqc_cfir[2] = {0x8138, 0x8238};
	u32 listen_cfir_iqc[2] = {0x813c, 0x823c};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* halrf_wrf(rf, path, 0x00, 0xf0000, 0x5); */
	halrf_wrf(rf, path, 0x00, MASKRF, 0x50001 | BIT(rf->hal_com->dbcc_en)); /* if exit LPS */
	
	halrf_wrf(rf, path, 0x01, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x63, 0x0001c, 0x0);
	halrf_wrf(rf, path, 0x64, 0x18000, 0x3);
	halrf_wrf(rf, path, 0x8a, 0x00300, 0x3);
	halrf_wrf(rf, path, 0x00, 0x00400, 0x1);
	halrf_wrf(rf, path, 0x00, 0x07800, 0x0);
	halrf_wrf(rf, path, 0x00, 0x003e0, 0x1f);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x3f);
	halrf_wrf(rf, path, 0x94, 0x000fc, 0x3f);
	halrf_wrf(rf, path, 0x93, 0x00008, 0x0);
	halrf_wrf(rf, path, 0x92, 0x00001, 0x0);
	halrf_wrf(rf, path, 0x92, 0x00001, 0x1);

	halrf_wrf(rf, path, 0x92, 0x00001, 0x0);

	halrf_wrf(rf, path, 0x5c, 0x80000, 0x1);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x1);
	halrf_wrf(rf, path, 0x5e, 0x3f000, 0x1f);
	halrf_wrf(rf, path, 0xde, 0x00004, 0x0);

	_txgapk_backup_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	halrf_wreg(rf, disable_cfir_en[path], MASKDWORD, 0x00000000);
	halrf_wreg(rf, listen_iqc_cfir[path], MASKDWORD, 0x40000002);
	halrf_wreg(rf, listen_cfir_iqc[path], MASKDWORD, 0x40000002);

	rf_tmp = halrf_rrf(rf, path, 0x0, 0xfffff);
	halrf_wreg(rf, 0x8024, 0x000fffff, rf_tmp);

	halrf_wreg(rf, 0x801c, 0x000e0000, 0x3);
	halrf_wreg(rf, 0x80e0, 0x00000001, 0x1);
	halrf_wreg(rf, 0x80e0, 0x00000002, 0x0);
	halrf_wreg(rf, 0x80e0, 0x000001f0, 0x1f);
	halrf_wreg(rf, 0x80e0, 0x0000f000, 0x0);
	halrf_wreg(rf, 0x8038, 0x00000038, 0x3);

	/* NIC */
	/* AP iFEM */
	/* AP eFEM */
	halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x00054a);

	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x8158, 0xffe00000, 0x7ff);
		halrf_wreg(rf, 0x815c, 0x000003ff, 0x000);
		halrf_wreg(rf, 0x815c, 0x7ffffc00, 0x005555);
	}

	rf_tmp = halrf_rrf(rf, path, 0x56, 0xfffff);
	halrf_wreg(rf, 0x8040, 0x000fffff, rf_tmp);

	rf_tmp = halrf_rrf(rf, path, 0x5c, 0xc0000);
	halrf_wreg(rf, 0x80e4, 0x30000000, rf_tmp);

	rf_tmp = halrf_rrf(rf, path, 0x5c, 0x007ff);
	halrf_wreg(rf, 0x80e4, 0x07ff0000, rf_tmp);

	//halrf_rrf(rf, path, 0x5c, 0xfffff);

	halrf_wreg(rf, 0xa60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	halrf_wreg(rf, path_setting[path], 0x20000000, 0x1);

	//halrf_rreg(rf, 0x80e0, MASKDWORD);

	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x802c, 0x0fff0000, 0x009);
	halrf_wreg(rf, 0x8000, MASKDWORD, cal_path[path]);

	//for (i = 0; i < 100; i++)
	//	halrf_delay_us(rf, 1);

	halrf_wreg(rf, 0x80b0, 0x10000000, 0x1);

	for (i = 0; i < 1000; i++)
		halrf_delay_us(rf, 1);

	halrf_wreg(rf, path_setting[path], 0x20000000, 0x0);

	//halrf_rrf(rf, path, 0x5c, 0xfffff);

	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00130000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x3);

	d[0] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[1] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[2] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[3] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x4);

	d[4] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[5] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[6] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[7] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x5);

	d[8] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[9] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[10] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[11] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x6);

	d[12] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[13] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[14] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[15] = halrf_rreg(rf, 0x80fc, 0x0fe00000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x7);

	d[16] = halrf_rreg(rf, 0x80fc, 0x0000007f);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x9);

	ta[0] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[1] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[2] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[3] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xa);
	
	ta[4] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[5] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[6] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[7] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xb);

	ta[8] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[9] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[10]= halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[11] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xc);

	ta[12] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[13] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[14] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[15] = halrf_rreg(rf, 0x80fc, 0xff000000);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xd);
	
	ta[16] = halrf_rreg(rf, 0x80fc, 0x000000ff);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	//halrf_rreg(rf, 0x80fc, 0x0000007f);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x0);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	//halrf_rreg(rf, 0x80fc, 0x007f0000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x1);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xf);
	//halrf_rreg(rf, 0x80fc, MASKDWORD);
	
	halrf_rrf(rf, path, 0x5c, 0xfffff);

	_txgapk_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	halrf_wreg(rf, 0x801c, 0x000e0000, 0x0);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x12);
	halrf_wreg(rf, 0x80d0, 0x00100000, 0x1);

	for (i = 0; i < 17; i++) {
		if (d[i] & BIT(6))
			txgapk_info->track_d[path][i] = (s32)(d[i] | 0xffffff80);
		else
			txgapk_info->track_d[path][i] = (s32)(d[i]);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	track	d[%d][%d]=0x%x\n",
			path, i, txgapk_info->track_d[path][i]);
	}

	for (i = 0; i < 17; i++) {
		if (ta[i] & BIT(7))
			txgapk_info->track_ta[path][i] = (s32)(ta[i] | 0xffffff00);
		else
			txgapk_info->track_ta[path][i] = (s32)(ta[i]);
		
		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	track	ta[%d][%d]=0x%x\n",
			path, i, txgapk_info->track_ta[path][i]);
	}

	//halrf_rrf(rf, path, 0x5c, 0xfffff);
	//halrf_rrf(rf, path, 0x5c, 0x3f800);
}


void _halrf_txgapk_track_table_nctl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;
	
	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_track_table_nctl_2g_8852a(rf, phy, path);
	else
		_halrf_txgapk_track_table_nctl_5g_8852a(rf, phy, path);
}

void _halrf_txgapk_write_track_table_default_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x20);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x2F);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x6F);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_default_5gl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x820);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x821);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x823);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x827);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x867);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_default_5gm_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);
	
	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa20);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa67);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_default_5gh_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc20);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc67);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_default_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_write_track_table_default_2g_8852a(rf, phy, path);
	else if (channel >= 36 && channel <= 64)
		_halrf_txgapk_write_track_table_default_5gl_8852a(rf, phy, path);
	else if (channel >= 100 && channel <= 144)
		_halrf_txgapk_write_track_table_default_5gm_8852a(rf, phy, path);
	else if (channel >= 149 && channel <= 177)
		_halrf_txgapk_write_track_table_default_5gh_8852a(rf, phy, path);
}

void _halrf_txgapk_write_track_table_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 pa_change[6] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);
	
	if (rfe_type <= 50) { 
		/* NIC */	
		/* AP iFEM */
		pa_change[0] = 0;
		pa_change[1] = 2;
		pa_change[2] = 4;
		pa_change[3] = 6;
		pa_change[4] = 8;
		pa_change[5] = 10;		
	}
	else { 
		/* AP eFEM */
		pa_change[0] = 1;
		pa_change[1] = 3;
		pa_change[2] = 5;
		pa_change[3] = 7;
		pa_change[4] = 9;
		pa_change[5] = 11;	
	}

	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x20); 
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[0]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[1]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[2]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[3]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x2F);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[4]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x6F);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[5]] / 2) & 0x3f);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_5gl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 pa_change[5] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	pa_change[0] = 1;
	pa_change[1] = 3;
	pa_change[2] = 6;
	pa_change[3] = 8;
	pa_change[4] = 10;
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x820);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[0]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x821);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[1]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x823);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[2]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x827);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[3]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x867);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[4]] / 2) & 0x3f);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */

}

void _halrf_txgapk_write_track_table_5gm_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 pa_change[5] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	pa_change[0] = 1;
	pa_change[1] = 3;
	pa_change[2] = 6;
	pa_change[3] = 8;
	pa_change[4] = 10;
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa20);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[0]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[1]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[2]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[3]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xa67);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[4]] / 2) & 0x3f);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */

}

void _halrf_txgapk_write_track_table_5gh_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 pa_change[5] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */	
	/* AP iFEM */
	/* AP eFEM */
	pa_change[0] = 1;
	pa_change[1] = 3;
	pa_change[2] = 6;
	pa_change[3] = 8;
	pa_change[4] = 10;
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc20);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[0]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[1]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[2]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc27);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[3]] / 2) & 0x3f);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0xc67);
	halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->track_ta[path][pa_change[4]] / 2) & 0x3f);
	
	halrf_wrf(rf, path, 0xee, 0x08000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_track_table_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_write_track_table_2g_8852a(rf, phy, path);
	else if (channel >= 36 && channel <= 64)
		_halrf_txgapk_write_track_table_5gl_8852a(rf, phy, path);
	else if (channel >= 100 && channel <= 144)
		_halrf_txgapk_write_track_table_5gm_8852a(rf, phy, path);
	else if (channel >= 149 && channel <= 177)
		_halrf_txgapk_write_track_table_5gh_8852a(rf, phy, path);
}

void _halrf_txgapk_power_table_nctl_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	u32 i, rf_tmp;
	u32 d[17] = {0}, ta[17] = {0};
	u32 gapk_on_table0_setting[2] = {0x8170, 0x8270};
	u32 path_setting[2] = {0x5864, 0x7864};
	u32 itqt[2] = {0x81cc, 0x82cc};
	u32 cal_path[2] = {0x00002119, 0x00002129};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0x05, 0x00001, 0x0);
	halrf_wrf(rf, path, 0x01, 0xfffff, 0x35);
	/* halrf_wrf(rf, path, 0x00, 0xf0000, 0x5); */
	halrf_wrf(rf, path, 0x00, MASKRF, 0x50001 | BIT(rf->hal_com->dbcc_en)); /* if exit LPS */
	
	halrf_wrf(rf, path, 0x00, 0x00400, 0x0);
	halrf_wrf(rf, path, 0x00, 0x07800, 0x0);
	halrf_wrf(rf, path, 0x00, 0x003e0, 0x1f);
	halrf_wrf(rf, path, 0x83, 0x00007, 0x0);
	halrf_wrf(rf, path, 0x83, 0x000f0, 0x4);
	halrf_wrf(rf, path, 0x9f, 0x00018, 0x0);
	halrf_wrf(rf, path, 0x5c, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x1);
	halrf_wrf(rf, path, 0xde, 0x02000, 0x0);
	halrf_wreg(rf, 0x801c, 0x000e0000, 0x3);
	halrf_wreg(rf, 0x80e0, 0x00000002, 0x1);
	halrf_wreg(rf, 0x80e0, 0x000001f0, 0x1f);
	halrf_wreg(rf, 0x8038, 0x00000038, 0x2);
	if (rfe_type <= 50) {
		/* NIC */
		/* AP iFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000540);
		
	} else {
		/* AP eFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x004000);
	}

	/* halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000540); */

	rf_tmp = halrf_rrf(rf, path, 0x56, 0xfffff);
	halrf_wreg(rf, 0x8040, 0x000fffff, rf_tmp);
	rf_tmp = halrf_rrf(rf, path, 0x5e, 0xc0000);
	halrf_wreg(rf, 0x80e4, 0x30000000, rf_tmp);
	rf_tmp = halrf_rrf(rf, path, 0x5e, 0x00fff);
	halrf_wreg(rf, 0x80e4, 0x0fff0000, rf_tmp);

	halrf_wreg(rf, 0xa60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	halrf_wreg(rf, path_setting[path], 0x20000000, 0x1);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x802c, 0x0fff0000, 0x009);
	halrf_wreg(rf, 0x8000, MASKDWORD, cal_path[path]);
	halrf_wreg(rf, 0x80b0, 0x10000000, 0x1);

	for (i = 0; i < 1000; i++)
		halrf_delay_us(rf, 1);

	halrf_wreg(rf, path_setting[path], 0x20000000, 0x0);
	/*= halrf_rrf(rf, path, 0x5e, RFREGOFFSETMASK);*/
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00130000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x3);
	d[0] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[1] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[2] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[3] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x4);
	d[4] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[5] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[6] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[7] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x5);
	d[8] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[9] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[10] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[11] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x6);
	d[12] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[13] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[14] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[15] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x7);
	d[16] = halrf_rreg(rf, 0x80fc, 0x0000007f);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x9);
	ta[0] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[1] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[2] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[3] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xa);
	ta[4] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[5] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[6] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[7] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xb);
	ta[8] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[9] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[10] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[11] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xc);
	ta[12] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[13] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[14] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[15] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xd);
	ta[16] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	
#if 0
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	 = halrf_rreg(rf, 0x80fc, 0x0000007f);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x0);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	 = halrf_rreg(rf, 0x80fc, 0x007f0000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x1);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xf);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
#endif
	
	/*halrf_wreg(rf, 0x8124, MASKDWORD, varfromtmp);*/
	/*halrf_wreg(rf, 0x8138, MASKDWORD, varfromtmp);*/
	/*halrf_wreg(rf, 0x813c, MASKDWORD, varfromtmp);*/
	
	halrf_wreg(rf, 0x801c, 0x000e0000, 0x0);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x80d0, 0x00100000, 0x1);

	for (i = 0; i < 17; i++) {
		if (d[i] & BIT(6))
			txgapk_info->power_d[path][i] = (s32)(d[i] | 0xffffff80);
		else
			txgapk_info->power_d[path][i] = (s32)(d[i]);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	power	d[%d][%d]=0x%x\n",
			path, i, txgapk_info->power_d[path][i]);
	}

	for (i = 0; i < 17; i++) {
		if (ta[i] & BIT(7))
			txgapk_info->power_ta[path][i] = (s32)(ta[i] | 0xffffff00);
		else
			txgapk_info->power_ta[path][i] = (s32)(ta[i]);
		
		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	power	ta[%d][%d]=0x%x\n",
			path, i, txgapk_info->power_ta[path][i]);
	}
	/* = halrf_rrf(rf, path, 0x5e, RFREGOFFSETMASK);*/
	/* = halrf_rrf(rf, path, 0x5e, 0x3f000);*/
}

void _halrf_txgapk_power_table_nctl_5g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;

	u32 i, rf_tmp;
	u32 d[17] = {0}, ta[17] = {0};
	u32 gapk_on_table0_setting[2] = {0x8170, 0x8270};
	u32 path_setting[2] = {0x5864, 0x7864};
	u32 itqt[2] = {0x81cc, 0x82cc};
	u32 cal_path[2] = {0x00002119, 0x00002129};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* halrf_wrf(rf, path, 0x00, 0xf0000, 0x5); */
	halrf_wrf(rf, path, 0x00, MASKRF, 0x50001 | BIT(rf->hal_com->dbcc_en)); /* if exit LPS */
	
	halrf_wrf(rf, path, 0x01, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x63, 0x0001c, 0x0);
	halrf_wrf(rf, path, 0x64, 0x18000, 0x3);
	halrf_wrf(rf, path, 0x8a, 0x00300, 0x3);
	halrf_wrf(rf, path, 0x00, 0x00400, 0x1);
	halrf_wrf(rf, path, 0x00, 0x07800, 0x0);
	halrf_wrf(rf, path, 0x00, 0x003e0, 0x1f);
	halrf_wrf(rf, path, 0x5c, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x1);
	halrf_wrf(rf, path, 0xde, 0x02000, 0x0);
	halrf_wreg(rf, 0x801c, 0x000e0000, 0x3);
	halrf_wreg(rf, 0x80e0, 0x00000002, 0x1);
	halrf_wreg(rf, 0x80e0, 0x000001f0, 0x1f);
	halrf_wreg(rf, 0x8038, 0x00000038, 0x3);

	if (rfe_type < 50) {
		/* NIC */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000520);		
	} else if (rfe_type == 50) {
		/* AP iFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x000640);
	} else {
		/* AP eFEM */
		halrf_wreg(rf, gapk_on_table0_setting[path], 0x001fffff, 0x004000);
	}


	rf_tmp = halrf_rrf(rf, path, 0x56, 0xfffff);
	halrf_wreg(rf, 0x8040, 0x000fffff, rf_tmp);
	rf_tmp = halrf_rrf(rf, path, 0x5e, 0xc0000);
	halrf_wreg(rf, 0x80e4, 0x30000000, rf_tmp);
	rf_tmp = halrf_rrf(rf, path, 0x5e, 0x00fff);
	halrf_wreg(rf, 0x80e4, 0x0fff0000, rf_tmp);

	halrf_wreg(rf, 0xa60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	halrf_wreg(rf, path_setting[path], 0x20000000, 0x1);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x802c, 0x0fff0000, 0x009);
	halrf_wreg(rf, 0x8000, MASKDWORD, cal_path[path]);
	halrf_wreg(rf, 0x80b0, 0x10000000, 0x1);

	for (i = 0; i < 1000; i++)
		halrf_delay_us(rf, 1);

	halrf_wreg(rf, path_setting[path], 0x20000000, 0x0);
	/*= halrf_rrf(rf, path, 0x5e, RFREGOFFSETMASK);*/
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00130000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x3);
	d[0] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[1] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[2] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[3] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x4);
	d[4] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[5] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[6] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[7] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x5);
	d[8] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[9] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[10] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[11] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x6);
	d[12] = halrf_rreg(rf, 0x80fc, 0x0000007f);
	d[13] = halrf_rreg(rf, 0x80fc, 0x00003f80);
	d[14] = halrf_rreg(rf, 0x80fc, 0x001fc000);
	d[15] = halrf_rreg(rf, 0x80fc, 0x0fe00000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x7);
	d[16] = halrf_rreg(rf, 0x80fc, 0x0000007f);

	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x9);
	ta[0] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[1] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[2] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[3] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xa);
	ta[4] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[5] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[6] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[7] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xb);
	ta[8] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[9] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[10] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[11] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xc);
	ta[12] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	ta[13] = halrf_rreg(rf, 0x80fc, 0x0000ff00);
	ta[14] = halrf_rreg(rf, 0x80fc, 0x00ff0000);
	ta[15] = halrf_rreg(rf, 0x80fc, 0xff000000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xd);
	ta[16] = halrf_rreg(rf, 0x80fc, 0x000000ff);
	
#if 0
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	 = halrf_rreg(rf, 0x80fc, 0x0000007f);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x0);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x2);
	 = halrf_rreg(rf, 0x80fc, 0x007f0000);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0x1);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80e4, 0x00000f00, 0xf);
	 = halrf_rreg(rf, 0x80fc, MASKDWORD);
#endif
	
	/*halrf_wreg(rf, 0x8124, MASKDWORD, varfromtmp);*/
	/*halrf_wreg(rf, 0x8138, MASKDWORD, varfromtmp);*/
	/*halrf_wreg(rf, 0x813c, MASKDWORD, varfromtmp);*/
	
	halrf_wreg(rf, 0x801c, 0x000e0000, 0x0);
	halrf_wreg(rf, itqt[path], MASKDWORD, 0x1b);
	halrf_wreg(rf, 0x80d0, 0x00100000, 0x1);

	for (i = 0; i < 17; i++) {
		if (d[i] & BIT(6))
			txgapk_info->power_d[path][i] = (s32)(d[i] | 0xffffff80);
		else
			txgapk_info->power_d[path][i] = (s32)(d[i]);

		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	power	d[%d][%d]=0x%x\n",
			path, i, txgapk_info->power_d[path][i]);
	}

	for (i = 0; i < 17; i++) {
		if (ta[i] & BIT(7))
			txgapk_info->power_ta[path][i] = (s32)(ta[i] | 0xffffff00);
		else
			txgapk_info->power_ta[path][i] = (s32)(ta[i]);
		
		RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]	power	ta[%d][%d]=0x%x\n",
			path, i, txgapk_info->power_ta[path][i]);
	}
	/* = halrf_rrf(rf, path, 0x5e, RFREGOFFSETMASK);*/
	/* = halrf_rrf(rf, path, 0x5e, 0x3f000);*/
}


void _halrf_txgapk_power_table_nctl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_power_table_nctl_2g_8852a(rf, phy, path);
	else
		_halrf_txgapk_power_table_nctl_5g_8852a(rf, phy, path);

}

void _halrf_txgapk_write_power_table_default_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */
	/* AP iFEM */
	/* AP eFEM */
	
	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x1);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x3);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x5);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0); 
	
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_default_5gl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x23);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x26);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_default_5gm_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x29);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x2b);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x2e);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_default_5gh_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	/* NIC */
	/* AP iFEM */
	/* AP eFEM */
	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */
	
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x31);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x33);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0x33, 0x01fff, 0x36);
	halrf_wrf(rf, path, 0x3f, 0x0003f, 0x0);
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_default_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_write_power_table_default_2g_8852a(rf, phy, path);
	else if (channel >= 36 && channel <= 64)
		_halrf_txgapk_write_power_table_default_5gl_8852a(rf, phy, path);
	else if (channel >= 100 && channel <= 144)
		_halrf_txgapk_write_power_table_default_5gm_8852a(rf, phy, path);
	else if (channel >= 149 && channel <= 177)
		_halrf_txgapk_write_power_table_default_5gh_8852a(rf, phy, path);
}

void _halrf_txgapk_write_power_table_2g_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 pa_change[3] = {0};
	

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */

	if (rfe_type <= 50) {
		/* NIC */
		/* AP iFEM */
		
		pa_change[0] = 6;
		pa_change[1] = 8;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x1);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x3);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x5);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	}
	else {
		/* AP eFEM */
		pa_change[0] = 14;
		
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x1);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
	}

	
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_5gl_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 pa_change[3] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */

	if (rfe_type < 50) {
		/* NIC */
		pa_change[0] = 5;
		pa_change[1] = 8;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x23);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x26);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else if (rfe_type == 50) {
		/* AP iFEM */
		pa_change[0] = 6;
		pa_change[1] = 9;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x23);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x26);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else {
		/* AP eFEM */
		pa_change[0] = 14;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x21);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
	}

	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_5gm_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 pa_change[3] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */

	if (rfe_type < 50) {
		/* NIC */
		pa_change[0] = 5;
		pa_change[1] = 8;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x29);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x2b);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x2e);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else if (rfe_type == 50) {
		/* AP iFEM */
		pa_change[0] = 6;
		pa_change[1] = 9;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x29);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x2b);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x2e);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else {
		/* AP eFEM */
		pa_change[0] = 14;
		
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x29);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
	}
	
	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_5gh_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	struct halrf_gapk_info *txgapk_info = &rf->gapk;
	u8 rfe_type = rf->phl_com->dev_cap.rfe_type;
	u8 pa_change[3] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	halrf_wrf(rf, path, 0xee, 0x40000, 0x1); /* enter debug mode before write */

	if (rfe_type < 50) {
		/* NIC */
		pa_change[0] = 5;
		pa_change[1] = 8;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x31);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x33);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x36);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else if (rfe_type == 50) {
		/* AP iFEM */
		pa_change[0] = 6;
		pa_change[1] = 9;
		pa_change[2] = 10;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x31);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x33);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[1]] / 2) & 0x3f);
		halrf_wrf(rf, path, 0x33, 0x01fff, 0x36);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[2]] / 2) & 0x3f);
	} else {
		/* AP eFEM */
		pa_change[0] = 14;

		halrf_wrf(rf, path, 0x33, 0x01fff, 0x31);
		halrf_wrf(rf, path, 0x3f, 0x0003f, (txgapk_info->power_ta[path][pa_change[0]] / 2) & 0x3f);	
	}

	halrf_wrf(rf, path, 0xee, 0x40000, 0x0); /* exit debug mode after write */
}

void _halrf_txgapk_write_power_table_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path)
{
	u8 channel = rf->hal_com->band[phy].cur_chandef.center_ch;

	if (channel >= 1 && channel <= 14)
		_halrf_txgapk_write_power_table_2g_8852a(rf, phy, path);
	else if (channel >= 36 && channel <= 64)
		_halrf_txgapk_write_power_table_5gl_8852a(rf, phy, path);
	else if (channel >= 100 && channel <= 144)
		_halrf_txgapk_write_power_table_5gm_8852a(rf, phy, path);
	else if (channel >= 149 && channel <= 177)
		_halrf_txgapk_write_power_table_5gh_8852a(rf, phy, path);
}

void _halrf_txgapk_iqk_bk_reg_by_mode_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_dbcc)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);
	
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8074, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000000);
	halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x80e0, 0x00000001, 0x0);

	if (!is_dbcc) {
		/* no dbcc */
		halrf_wreg(rf, 0x8120, MASKDWORD, 0x10010000);
		halrf_wreg(rf, 0x8140, 0x00000100, 0x0);
		halrf_wreg(rf, 0x8150, MASKDWORD, 0xe4e4e4e4);
		halrf_wreg(rf, 0x8154, 0x00000100, 0x0);
		halrf_wreg(rf, 0x81cc, 0x0000003f, 0x0);
		halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
		halrf_wreg(rf, 0x8220, MASKDWORD, 0x10010000);
		halrf_wreg(rf, 0x8240, 0x00000100, 0x0);
		halrf_wreg(rf, 0x8250, MASKDWORD, 0xe4e4e4e4);
		halrf_wreg(rf, 0x8254, 0x00000100, 0x0);
		halrf_wreg(rf, 0x82cc, 0x0000003f, 0x0);
		halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	} else {
		/* dbcc */
		if (phy == HW_PHY_0) {
			halrf_wreg(rf, 0x8120, MASKDWORD, 0x10010000);
			halrf_wreg(rf, 0x8140, 0x00000100, 0x0);
			halrf_wreg(rf, 0x8150, MASKDWORD, 0xe4e4e4e4);
			halrf_wreg(rf, 0x8154, 0x00000100, 0x0);
			halrf_wreg(rf, 0x81cc, 0x0000003f, 0x0);
			halrf_wreg(rf, 0x81dc, MASKDWORD, 0x00000002);
		} else if (phy == HW_PHY_1) {
			halrf_wreg(rf, 0x8220, MASKDWORD, 0x10010000);
			halrf_wreg(rf, 0x8240, 0x00000100, 0x0);
			halrf_wreg(rf, 0x8250, MASKDWORD, 0xe4e4e4e4);
			halrf_wreg(rf, 0x8254, 0x00000100, 0x0);
			halrf_wreg(rf, 0x82cc, 0x0000003f, 0x0);
			halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
		}
	}
		
	halrf_wrf(rf, path, 0xef, 0x00004, 0x0);
	halrf_wrf(rf, path, 0xde, 0xfe000, 0x0);
	halrf_wrf(rf, path, 0xef, 0x00004, 0x0);
	halrf_wrf(rf, path, 0x0, 0xf0000, 0x3);
	halrf_wrf(rf, path, 0x5c, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5, 0x00001, 0x1);
}



void _halrf_txgapk_afe_bk_reg_by_mode_8852a
	(struct rf_info *rf, enum phl_phy_idx phy, enum rf_path path, bool is_dbcc)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s\n", __func__);

	if (!is_dbcc) {
		/* no dbcc */
		halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0303);
		halrf_wreg(rf, 0x12b8, 0x40000000, 0x0);
		halrf_wreg(rf, 0x32b8, 0x40000000, 0x0);
		halrf_wreg(rf, 0x5864, 0xc0000000, 0x0);
		halrf_wreg(rf, 0x7864, 0xc0000000, 0x0);
		halrf_wreg(rf, 0x2008, 0x01ffffff, 0x0000000);
		halrf_wreg(rf, 0x0c1c, 0x00000004, 0x0);
		halrf_wreg(rf, 0x0700, 0x08000000, 0x0);
		halrf_wreg(rf, 0x0c70, 0x0000001f, 0x03);
		halrf_wreg(rf, 0x0c70, 0x000003e0, 0x03);
		halrf_wreg(rf, 0x12a0, 0x000ff000, 0x00);
		halrf_wreg(rf, 0x32a0, 0x000ff000, 0x00);
		halrf_wreg(rf, 0x0700, 0x07000000, 0x0);
		halrf_wreg(rf, 0x0c3c, 0x00000200, 0x0);
		halrf_wreg(rf, 0x2344, 0x80000000, 0x0);
		halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0000);
		halrf_wreg(rf, 0x58c8, 0x01000000, 0x0);
		halrf_wreg(rf, 0x78c8, 0x01000000, 0x0);
	} else {
		/* dbcc */
		if (phy == HW_PHY_0) {
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0101);
			halrf_wreg(rf, 0x12b8, 0x40000000, 0x0);
			halrf_wreg(rf, 0x5864, 0xc0000000, 0x0);
			halrf_wreg(rf, 0x2008, 0x01ffffff, 0x0000000);
			halrf_wreg(rf, 0x0c1c, 0x00000004, 0x0);
			halrf_wreg(rf, 0x0700, 0x08000000, 0x0);
			halrf_wreg(rf, 0x0c70, 0x0000001f, 0x03);
			halrf_wreg(rf, 0x0c70, 0x000003e0, 0x03);
			halrf_wreg(rf, 0x12a0, 0x000ff000, 0x00);
			halrf_wreg(rf, 0x0700, 0x07000000, 0x0);
			halrf_wreg(rf, 0x0c3c, 0x00000200, 0x0);
			halrf_wreg(rf, 0x2344, 0x80000000, 0x0);
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0000);
			halrf_wreg(rf, 0x58c8, 0x01000000, 0x0);
		} else if (phy == HW_PHY_1) {
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0202);
			halrf_wreg(rf, 0x32b8, 0x40000000, 0x0);
			halrf_wreg(rf, 0x7864, 0xc0000000, 0x0);
			halrf_wreg(rf, 0x2008, 0x01ffffff, 0x0000000);
			halrf_wreg(rf, 0x2c1c, 0x00000004, 0x0);
			halrf_wreg(rf, 0x2700, 0x08000000, 0x0);
			halrf_wreg(rf, 0x0cf0, 0x000001ff, 0x000);
			halrf_wreg(rf, 0x32a0, 0x000ff000, 0x00);
			halrf_wreg(rf, 0x2700, 0x07000000, 0x0);
			halrf_wreg(rf, 0x2c3c, 0x00000200, 0x0);
			halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0000);
			halrf_wreg(rf, 0x78c8, 0x01000000, 0x0);
			halrf_wreg(rf, 0x0c70, 0x0000001f, 0x03);
		}
	}
	

	//halrf_wrf(rf, path, 0x00, 0x00001, 0x0);
}




void _halrf_do_non_dbcc_txgapk_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u8 i;
	u32 bb_reg[1] = {0x2344};
	u32 bb_reg_backup[1] = {0};
	u32 backup_num = 1;
	u32 rf_3wire[2] = {0};

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s   phy=%d\n", __func__, phy);
	
	_txgapk_backup_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	/*rf_0[RF_PATH_A] = halrf_rrf(rf, RF_PATH_A, 0x0, 0xfffff);*/
	/*rf_0[RF_PATH_B] = halrf_rrf(rf, RF_PATH_B, 0x0, 0xfffff);*/

	rf_3wire[RF_PATH_A] = halrf_rrf(rf, RF_PATH_A, 0x5, 0xfffff);
	rf_3wire[RF_PATH_B] = halrf_rrf(rf, RF_PATH_B, 0x5, 0xfffff);

	/*halrf_wrf(rf, RF_PATH_A, 0x0, 0x00001, 0x1);*/
	/*halrf_wrf(rf, RF_PATH_B, 0x0, 0x00001, 0x1);*/

	for (i = 0; i < 2; i++) {
		/*_halrf_txgapk_iqk_dpk_init_reg_8852a(rf, phy, RF_PATH_A);*/
		/*_halrf_txgapk_nctl_8852a(rf, phy, RF_PATH_A);*/

		_halrf_txgapk_write_track_table_default_8852a(rf, phy, i);
		_halrf_txgapk_write_power_table_default_8852a(rf, phy, i);

		_halrf_txgapk_bb_afe_by_mode_8852a(rf, phy, i, false);
		_halrf_txgapk_iqk_preset_by_mode_8852a(rf, phy, i, false);
		_halrf_txgapk_clk_setting_dac960mhz_by_mode_8852a(rf, phy, i, false);

		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
		halrf_tmac_tx_pause(rf, phy, true);

		_halrf_txgapk_track_table_nctl_8852a(rf, phy, i);
		_halrf_txgapk_write_track_table_8852a(rf, phy, i);
		_halrf_txgapk_power_table_nctl_8852a(rf, phy, i);
		_halrf_txgapk_write_power_table_8852a(rf, phy, i);

		halrf_tmac_tx_pause(rf, phy, false);
		halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);

		_halrf_txgapk_iqk_bk_reg_by_mode_8852a(rf, phy, i, false);
		_halrf_txgapk_afe_bk_reg_by_mode_8852a(rf, phy, i, false);
	}

	/*halrf_wrf(rf, RF_PATH_A, 0x0, 0x00001, rf_0[RF_PATH_A]);*/
	/*halrf_wrf(rf, RF_PATH_B, 0x0, 0x00001, rf_0[RF_PATH_B]);*/

	halrf_wrf(rf, RF_PATH_A, 0x5, 0xfffff, rf_3wire[RF_PATH_A]);
	halrf_wrf(rf, RF_PATH_B, 0x5, 0xfffff, rf_3wire[RF_PATH_B]);

	_txgapk_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

}
void _halrf_do_dbcc_txgapk_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	u32 bb_reg[1] = {0x2344};
	u32 bb_reg_backup[1] = {0};
	u32 backup_num = 1;
	u32 rf_3wire[2] = {0};
	enum rf_path path = 0;

	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s   phy=%d\n", __func__, phy);
	
	_txgapk_backup_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

	rf_3wire[RF_PATH_A] = halrf_rrf(rf, RF_PATH_A, 0x5, 0xfffff);
	rf_3wire[RF_PATH_B] = halrf_rrf(rf, RF_PATH_B, 0x5, 0xfffff);

	/* halrf_wrf(rf, RF_PATH_A, 0x00, 0x00001, 0x1); */
	/* halrf_wrf(rf, RF_PATH_B, 0x00, 0x00001, 0x1); */


	/*_halrf_txgapk_iqk_dpk_init_reg_8852a(rf, phy, RF_PATH_A);*/
	/*_halrf_txgapk_nctl_8852a(rf, phy, RF_PATH_A);*/

	if (phy == HW_PHY_0)
		path = RF_PATH_A;
	else if (phy == HW_PHY_1)
		path = RF_PATH_B;

	_halrf_txgapk_write_track_table_default_8852a(rf, phy, path);
	_halrf_txgapk_write_power_table_default_8852a(rf, phy, path);
		
	_halrf_txgapk_bb_afe_by_mode_8852a(rf, phy, path, true);
	_halrf_txgapk_iqk_preset_by_mode_8852a(rf, phy, path, true);
	_halrf_txgapk_clk_setting_dac960mhz_by_mode_8852a(rf, phy, path, true);

	halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_START);
	halrf_tmac_tx_pause(rf, phy, true);
	
	_halrf_txgapk_track_table_nctl_8852a(rf, phy, path);
	_halrf_txgapk_write_track_table_8852a(rf, phy, path);

		
	_halrf_txgapk_power_table_nctl_8852a(rf, phy, path);
	_halrf_txgapk_write_power_table_8852a(rf, phy, path);
  
	halrf_tmac_tx_pause(rf, phy, true);
	halrf_btc_rfk_ntfy(rf, (BIT(phy) << 4), RF_BTC_TSSI, RFK_STOP);

	_halrf_txgapk_iqk_bk_reg_by_mode_8852a(rf, phy, path, true);
	_halrf_txgapk_afe_bk_reg_by_mode_8852a(rf, phy, path, true);
	
	/* halrf_wrf(rf, RF_PATH_A, 0x00, 0x00001, 0x0); */
	/* halrf_wrf(rf, RF_PATH_B, 0x00, 0x00001, 0x0); */

	halrf_wrf(rf, RF_PATH_A, 0x5, 0xfffff, rf_3wire[RF_PATH_A]);
	halrf_wrf(rf, RF_PATH_B, 0x5, 0xfffff, rf_3wire[RF_PATH_B]);

	_txgapk_reload_bb_registers_8852a(rf, phy, bb_reg, bb_reg_backup,
					backup_num);

}
void halrf_do_txgapk_8852a(struct rf_info *rf,
					enum phl_phy_idx phy)
{
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s   phy=%d, dbcc_en = %d\n", __func__, phy, rf->hal_com->dbcc_en);
	/* RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> HALRF_MP_01\n"); */
	
	if (rf->hal_com->dbcc_en)
		_halrf_do_dbcc_txgapk_8852a(rf, phy);
	else
		_halrf_do_non_dbcc_txgapk_8852a(rf, phy);
}

void halrf_txgapk_enable_8852a
	(struct rf_info *rf, enum phl_phy_idx phy)
{
	u8 i;
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s   phy=%d\n", __func__, phy);

	for (i = RF_PATH_A; i < 2; i++) {
		_halrf_txgapk_write_track_table_8852a(rf, phy, i);
		_halrf_txgapk_write_power_table_8852a(rf, phy, i);
	}
}

void halrf_txgapk_write_table_default_8852a
	(struct rf_info *rf, enum phl_phy_idx phy)
{
	u8 i;
	RF_DBG(rf, DBG_RF_TXGAPK, "[TXGAPK]======> %s   phy=%d\n", __func__, phy);

	for (i = RF_PATH_A; i < 2; i++) {
		_halrf_txgapk_write_track_table_default_8852a(rf, phy, i);
		_halrf_txgapk_write_power_table_default_8852a(rf, phy, i);
	}
}

#endif
