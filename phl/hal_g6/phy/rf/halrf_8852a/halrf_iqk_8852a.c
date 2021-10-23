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

#ifndef __iram_func__
#define __iram_func__
#endif


const u32 array_set_nondbcc_path01[] = {
	0x20fc, 0xffff0000, 0x0303,
	0x5864, 0x18000000, 0x3,
	0x7864, 0x18000000, 0x3,
	0x12b8, 0x40000000, 0x1,
	0x32b8, 0x40000000, 0x1,
	0x030c, 0xff000000, 0x13,
	0x032c, 0xffff0000, 0x0001,
	0x12b8, 0x10000000, 0x1,
	0x58c8, 0x01000000, 0x1,
	0x78c8, 0x01000000, 0x1,
	0x5864, 0xc0000000, 0x3,
	0x7864, 0xc0000000, 0x3,
	0x2008, 0x01ffffff, 0x1ffffff,
	0x0c1c, 0x00000004, 0x1,
	0x0700, 0x08000000, 0x1,
	0x0c70, 0x000003ff, 0x3ff,
	0x0c60, 0x00000003, 0x3,
	0x0c6c, 0x00000001, 0x1,
	0x58ac, 0x08000000, 0x1,
	0x78ac, 0x08000000, 0x1,
	0x0c3c, 0x00000200, 0x1,
	0x2344, 0x80000000, 0x1,
	0x4490, 0x80000000, 0x1,
	0x12a0, 0x00007000, 0x7,
	0x12a0, 0x00008000, 0x1,
	0x12a0, 0x00070000, 0x3,
	0x12a0, 0x00080000, 0x1,
	0x32a0, 0x00070000, 0x3,
	0x32a0, 0x00080000, 0x1,
	0x0700, 0x01000000, 0x1,
	0x0700, 0x06000000, 0x2,
//	0x5864, 0x20000000, 0x1,
//	0x7864, 0x20000000, 0x1,
	0x20fc, 0xffff0000, 0x3333,
};

const u32 array_set_dbcc_path0[] = {
	0x20fc, 0xffff0000, 0x0101,
	0x5864, 0x18000000, 0x3,
	0x7864, 0x18000000, 0x3,
	0x12b8, 0x40000000, 0x1,
	0x030c, 0xff000000, 0x13,
	0x032c, 0xffff0000, 0x0001,
	0x12b8, 0x10000000, 0x1,
	0x58c8, 0x01000000, 0x1,
	0x5864, 0xc0000000, 0x3,
	0x2008, 0x01ffffff, 0x1ffffff,
	0x0c1c, 0x00000004, 0x1,
	0x0700, 0x08000000, 0x1,
	0x0c70, 0x000003ff, 0x3ff,
	0x0c60, 0x00000003, 0x3,
	0x0c6c, 0x00000001, 0x1,
	0x58ac, 0x08000000, 0x1,
	0x0c3c, 0x00000200, 0x1,
	0x2320, 0x00000001, 0x1,
	0x4490, 0x80000000, 0x1,
	0x12a0, 0x00007000, 0x7,
	0x12a0, 0x00008000, 0x1,
	0x12a0, 0x00070000, 0x3,
	0x12a0, 0x00080000, 0x1,
	0x0700, 0x01000000, 0x1,
	0x0700, 0x06000000, 0x2,
	//0x5864, 0x20000000, 0x1,
	0x20fc, 0xffff0000, 0x1111,
};

const u32 array_set_dbcc_path1[] = {
	0x20fc, 0xffff0000, 0x0202,
	0x7864, 0x18000000, 0x3,
	0x32b8, 0x40000000, 0x1,
	0x030c, 0xff000000, 0x13,
	0x032c, 0xffff0000, 0x0001,
	0x32b8, 0x10000000, 0x1,
	0x78c8, 0x01000000, 0x1,
	0x7864, 0xc0000000, 0x3,
	0x2008, 0x01ffffff, 0x1ffffff,
	0x2c1c, 0x00000004, 0x1,
	0x2700, 0x08000000, 0x1,
	0x0c70, 0x000003ff, 0x3ff,
	0x0c60, 0x00000003, 0x3,
	0x0c6c, 0x00000001, 0x1,
	0x78ac, 0x08000000, 0x1,
	0x2c3c, 0x00000200, 0x1,
	0x6490, 0x80000000, 0x1,
	0x32a0, 0x00007000, 0x7,
	0x32a0, 0x00008000, 0x1,
	0x32a0, 0x00070000, 0x3,
	0x32a0, 0x00080000, 0x1,
	0x2700, 0x01000000, 0x1,
	0x2700, 0x06000000, 0x2,
	//0x7864, 0x20000000, 0x1,
	0x20fc, 0xffff0000, 0x2222,
};

const u32 array_restore_nondbcc_path01[] = {
	0x20fc, 0xffff0000, 0x0303,
	0x12b8, 0x40000000, 0x0,
	0x32b8, 0x40000000, 0x0,
	0x5864, 0xc0000000, 0x0,
	0x7864, 0xc0000000, 0x0,
	0x2008, 0x01ffffff, 0x0000000,
	0x0c1c, 0x00000004, 0x0,
	0x0700, 0x08000000, 0x0,
	0x0c70, 0x0000001f, 0x03,
	0x0c70, 0x000003e0, 0x03,
	0x12a0, 0x000ff000, 0x00,
	0x32a0, 0x000ff000, 0x00,
	0x0700, 0x07000000, 0x0,
	0x5864, 0x20000000, 0x0,
	0x7864, 0x20000000, 0x0,
	0x0c3c, 0x00000200, 0x0,
	0x2344, 0x80000000, 0x1,
	0x20fc, 0xffff0000, 0x0000,
	0x58c8, 0x01000000, 0x0,
	0x78c8, 0x01000000, 0x0,
};

const u32 array_restore_dbcc_path0[] = {	
	0x20fc, 0xffff0000, 0x0101,
	0x12b8, 0x40000000, 0x0,
	0x5864, 0xc0000000, 0x0,
	0x2008, 0x01ffffff, 0x0000000,
	0x0c1c, 0x00000004, 0x0,
	0x0700, 0x08000000, 0x0,
	0x0c70, 0x0000001f, 0x03,
	0x0c70, 0x000003e0, 0x03,
	0x12a0, 0x000ff000, 0x00,
	0x0700, 0x07000000, 0x0,
	0x5864, 0x20000000, 0x0,
	0x0c3c, 0x00000200, 0x0,
	0x2320, 0x00000001, 0x0,
	0x20fc, 0xffff0000, 0x0000,
	0x58c8, 0x01000000, 0x0,
};

const u32 array_restore_dbcc_path1[] = {
	0x20fc, 0xffff0000, 0x0202,
	0x32b8, 0x40000000, 0x0,
	0x7864, 0xc0000000, 0x0,
	0x2008, 0x01ffffff, 0x0000000,
	0x2c1c, 0x00000004, 0x0,
	0x2700, 0x08000000, 0x0,
	0x0cf0, 0x000001ff, 0x000,
	0x32a0, 0x000ff000, 0x00,
	0x2700, 0x07000000, 0x0,
	0x7864, 0x20000000, 0x0,
	0x2c3c, 0x00000200, 0x0,
	0x20fc, 0xffff0000, 0x0000,
	0x78c8, 0x01000000, 0x0,
	0x0c70, 0x0000001f, 0x03,
};

__iram_func__
void iqk_backup_rf0_8852a(
	struct rf_info *rf, u8 path,
	u32 backup_rf0[rf_reg_num_8852a],
	u32 backup_rf_reg0[rf_reg_num_8852a])
{
	u8 i;

	if(path != RF_PATH_A)
		return;
	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	for (i = 0; i < rf_reg_num_8852a; i++) {
		backup_rf0[i] = halrf_rrf(rf, RF_PATH_A, backup_rf_reg0[i], MASKRF);		
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]bk rf0, %x = %x\n", backup_rf_reg0[i], backup_rf0[i]);
	}	
	return;
}

__iram_func__
void iqk_backup_rf1_8852a(
	struct rf_info *rf, u8 path,
	u32 backup_rf1[rf_reg_num_8852a],
	u32 backup_rf_reg1[rf_reg_num_8852a])
{
	u8 i;

	if(path != RF_PATH_B)
		return;

	//DBG_LOG_SERIOUS(DBGMSG_RF, DBG_WARNING, "[IQK] 06 \n");
	for (i = 0; i < rf_reg_num_8852a; i++) {
		backup_rf1[i] = halrf_rrf(rf, RF_PATH_B, backup_rf_reg1[i], MASKRF);		
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]bk rf1, %x = %x\n", backup_rf_reg1[i], backup_rf1[i]);
	}	
	return;
}

__iram_func__
void iqk_restore_rf0_8852a(
	struct rf_info *rf, u8 path,
	u32 backup_rf0[rf_reg_num_8852a],
	u32 backup_rf_reg0[rf_reg_num_8852a])
{
	u32 i = 0;
	
	if(path != RF_PATH_A)
		return;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);	
	//DBG_LOG_SERIOUS(DBGMSG_RF, DBG_WARNING, "[IQK] 13 \n");
	for (i = 0; i < rf_reg_num_8852a; i++) {
		halrf_wrf(rf, RF_PATH_A, backup_rf_reg0[i], MASKRF, backup_rf0[i]);
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]restore rf0, 0x%x = 0x%x\n", backup_rf_reg0[i], halrf_rrf(rf, 0x0, backup_rf_reg0[i], MASKRF));
	}
	return;
}

__iram_func__
void iqk_restore_rf1_8852a(
	struct rf_info *rf, u8 path,
	u32 backup_rf1[rf_reg_num_8852a],
	u32 backup_rf_reg1[rf_reg_num_8852a])
{
	u32 i;
	
	if(path != RF_PATH_B)
		return;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	for (i = 0; i < rf_reg_num_8852a; i++) {
		halrf_wrf(rf, RF_PATH_B, backup_rf_reg1[i], MASKRF, backup_rf1[i]);		
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]restore rf S%d = %x, value = %x\n", path, backup_rf_reg[path][i], halrf_rrf(rf, path, backup_rf_reg[path][i], MASKRF));
	}
	return;
}

#if 0
static void _iqk_config_8852a_reg(struct rf_info *rf, u32 *array_map)
{
	u32	i = 0;
	u32	array_len = 0x0;
	u32 *array = NULL;
	u32	addr = 0, mask = 0,  val = 0;	
	
	RF_DBG(rf, DBG_RF_INIT, "===> %s\n", __func__);
	array_len = sizeof(array_map) / sizeof(u32);
	array = (u32 *) &array_map;
	while ((i + 1) < array_len) {
		addr = array[i];
		mask = array[i + 1];
		val = array[i + 2];
		halrf_wreg(rf, addr, mask, val);		
		RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x[%x] = 0x%x\n", addr, mask, val);
		i += 3;
	}

}
#endif
static void _iqk_check_xym_8852ab(struct rf_info *rf, u8 path, u8 gp, u8 ktype)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 re[24] ={0};
	u32 im[24] ={0};	
	u32 max_re = 0, max_im = 0;
	u8 i, idx_s = 0, num = 0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x8000, 0x00000006, path);
	halrf_wreg(rf, 0x801c, 0x00000003, 0x1);

	if(iqk_info->iqk_bw[path] == CHANNEL_WIDTH_20) {
		idx_s = 12;
		num = 6;
	} else if(iqk_info->iqk_bw[path] == CHANNEL_WIDTH_40) {
		idx_s = 10;
		num = 10;
	} else {
		idx_s = 6;
		num = 18;
	}
	max_re = 0;
	max_im = 0;	
	for (i = idx_s; i < (idx_s+ num); i++) {
		halrf_wreg(rf, 0x8014, MASKDWORD, 0x000000c0 + i);
		halrf_wreg(rf, 0x8014, MASKDWORD, 0x00000000);
		re[i] = halrf_rreg(rf, 0x8138 + (path << 8), 0xfff00000);
		im[i] = halrf_rreg(rf, 0x8138 + (path << 8), 0x000fff00);

		if((re[i] == 0x0)  || (re[i] == 0xfff)) 
			RF_DBG(rf, DBG_RF_IQK, "[IQK]xym_check Fail !!!\n");
		
		if (re[i] < 0x400) 
			re[i] = (0x400 - re[i]);
		else
			re[i] = re[i] - 0x400;				
		
		if (im[i] > 0x800) 
			im[i] = (0x1000 - im[i]);

		if(re[i] > max_re)
			max_re = re[i];
		if(max_re > 0xff)
			max_re = 0xff;

		if(im[i] > max_im)
			max_im = im[i];
		if(max_im > 0xff)
			max_im = 0xff;

		//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, re[%d]= %x, max_re = %x\n", path, i, re[i], max_re);
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, im[%d] = %x, max_im = %x\n", path, i, im[i], max_im);
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, gp = %d, addr = 0x%x, mask = 0x%x, value =0x%x\n", path, gp, 0x9f30 + (path * 0x40), 0xff << (gp * 8), max_re);

#if 1
		if(ktype == ID_TXK) {
			halrf_wreg(rf, 0x9f30 + (path * 0x40), 0xff << (gp * 8), max_re);
			halrf_wreg(rf, 0x9f40 + (path * 0x40), 0xff << (gp * 8), max_im);
		} else {
			halrf_wreg(rf, 0x9f50 + (path * 0x40), 0xff << (gp * 8), max_re);
			halrf_wreg(rf, 0x9f60 + (path * 0x40), 0xff << (gp * 8), max_im);
		}
#endif
		halrf_delay_us(rf, 1);
	}
	halrf_wreg(rf, 0x801c, 0x00000003, 0x0);
	halrf_wreg(rf, 0x8138+ (path << 8), MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x8014, MASKDWORD, 0x80010100);	
	return;
}

__iram_func__
static void _iqk_read_fft_dbcc0_8852ab(struct rf_info *rf, u8 path)
{
	u8 i = 0x0;
	u32 fft[6] = {0x0};

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00160000);
	fft[0] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00170000);
	fft[1] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00180000);
	fft[2] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00190000);
	fft[3] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x001a0000);
	fft[4] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x001b0000);
	fft[5] = halrf_rreg(rf, 0x80fc, MASKDWORD);
	for(i =0; i< 6; i++)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x,fft[%x]= %x\n", path, i, fft[i]);

	return;
}

__iram_func__
static void _iqk_read_xym_dbcc0_8852ab(struct rf_info *rf, u8 path)
{

	u8 i = 0x0;
	u32 tmp = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x8000, 0x00000006, path);
	halrf_wreg(rf, 0x801c, 0x00000003, 0x1);

	for (i = 0x0; i < 0x18; i++) {
		halrf_wreg(rf, 0x8014, MASKDWORD, 0x000000c0 + i);
		halrf_wreg(rf, 0x8014, MASKDWORD, 0x00000000);
		tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x38 = %x\n", path, 1 << path, tmp);
		halrf_delay_us(rf, 1);
	}
	halrf_wreg(rf, 0x801c, 0x00000003, 0x0);
	halrf_wreg(rf, 0x8138+ (path << 8), MASKDWORD, 0x40000000);
	halrf_wreg(rf, 0x8014, MASKDWORD, 0x80010100);
	halrf_delay_us(rf, 1);

	return;
}

__iram_func__
static void _iqk_read_txcfir_dbcc0_8852ab(struct rf_info *rf, u8 path, u8 group)
{

	u8 idx = 0x0;
	u32 tmp = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x81d8+ (path << 8), MASKDWORD, 0x00000001);
	if (path == 0x0) {
		switch (group) {
		case 0:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x8f20 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x8f20 + (idx << 2), tmp);
			}
			break;
		case 1:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x8f54 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x8f54 + (idx << 2), tmp);
			}
			break;
		case 2:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x8f88 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x8f88 + (idx << 2), tmp);
			}
			break;
		case 3:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x8fbc + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x8fbc + (idx << 2), tmp);
			}
			break;
		default:
			break;
		}
		RF_DBG(rf, DBG_RF_IQK, "[IQK]\n");
		tmp = halrf_rreg(rf, 0x8f50, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8f50 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8f84, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8f84 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8fb8, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8fb8 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8fec, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8fec = %x\n", tmp);
	} else {
		switch (group) {
		case 0:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x9320 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x9320 + (idx << 2), tmp);
			}
			break;
		case 1:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x9354 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x9354 + (idx << 2), tmp);
			}
			break;
		case 2:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x9388 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x9388 + (idx << 2), tmp);
			}
			break;
		case 3:
			for (idx = 0; idx < 0x0d; idx++) {
				tmp = halrf_rreg(rf, 0x93bc + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK] %x = %x\n", 0x93bc + (idx << 2), tmp);
			}
			break;
		default:
			break;
		}
		RF_DBG(rf, DBG_RF_IQK, "[IQK]\n");
		tmp = halrf_rreg(rf, 0x9350, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x9350 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x9384, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x9384 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x93b8, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x93b8 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x93ec, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x93ec = %x\n", tmp);
	}
	halrf_wreg(rf, 0x81d8+ (path << 8), MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81d4 + (path << 8), 0x003f0000, 0xc);
	halrf_delay_us(rf, 1);
	tmp = halrf_rreg(rf, 0x81fc + (path << 8), MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%xfc = %x\n", path, 1 << path, tmp);

	return;
}

__iram_func__
static void _iqk_read_rxcfir_dbcc0_8852ab(struct rf_info *rf, u8 path, u8 group)
{

	u8 idx = 0x0;
	u32 tmp = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x81d8 + (path << 8), MASKDWORD, 0x00000001);

	if (path == 0x0) {
		switch (group) {
		case 0:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x8d00 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x8d00 + (idx << 2), tmp);
			}
			break;
		case 1:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x8d44 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x8d44 + (idx << 2), tmp);
			}
			break;
		case 2:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x8d88 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x8d88 + (idx << 2), tmp);
			}
			break;
		case 3:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x8dcc + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x8dcc + (idx << 2), tmp);
			}
			break;
		default:
			break;
		}
		RF_DBG(rf, DBG_RF_IQK, "[IQK]\n");
		tmp = halrf_rreg(rf, 0x8d40, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8d40 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8d84, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8d84 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8dc8, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8dc8 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x8e0c, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x8e0c = %x\n", tmp);
	} else {
		switch (group) {
		case 0:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x9100 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x9100 + (idx << 2), tmp);
			}
			break;
		case 1:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x9144 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x9144 + (idx << 2), tmp);
			}
			break;
		case 2:
			for(idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x9188 + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x9188 + (idx << 2), tmp);
			}
			break;
		case 3:
			for (idx = 0; idx < 0x10; idx++) {
				tmp = halrf_rreg(rf, 0x91cc + (idx << 2), MASKDWORD);
				RF_DBG(rf, DBG_RF_IQK, "[IQK]%x = %x\n", 0x91cc + (idx << 2), tmp);
			}
			break;
		default:
			break;
		}
		RF_DBG(rf, DBG_RF_IQK, "[IQK]\n");
		tmp = halrf_rreg(rf, 0x9140, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x9140 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x9184, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x9184 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x91c8, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x91c8 = %x\n", tmp);
		tmp = halrf_rreg(rf, 0x920c, MASKDWORD);
		RF_DBG(rf, DBG_RF_IQK, "[IQK] 0x920c = %x\n", tmp);
	}
	halrf_wreg(rf, 0x81d8 + (path << 8), MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81d4 + (path << 8), 0x003f0000, 0xd);
	tmp = halrf_rreg(rf, 0x81fc + (path << 8), MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%xfc = %x\n", path, 1 << path, tmp);

	return;
}

__iram_func__
static void _iqk_sram_8852ab(struct rf_info *rf, u8 path)
{

	u32 tmp = 0x0;
	u32 i = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00020000);
	halrf_wreg(rf, 0x80e8, MASKDWORD, 0x00000080);
	halrf_wreg(rf, 0x80d8, MASKDWORD, 0x00010000);
	halrf_wreg(rf, 0x802c, 0x00000fff, 0x009);

	for (i = 0; i <= 0x9f; i++) {
		halrf_wreg(rf, 0x80d8, MASKDWORD, 0x00010000 + i);
		tmp = halrf_rreg(rf, 0x80fc, 0x0fff0000);
		RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x\n", tmp);
	}

	for (i = 0; i <= 0x9f; i++) {
		halrf_wreg(rf, 0x80d8, MASKDWORD, 0x00010000 + i);
		tmp = halrf_rreg(rf, 0x80fc, 0x00000fff);
		RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x\n", tmp);
	}
	halrf_wreg(rf, 0x80e8, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x80d8, MASKDWORD, 0x00000000);

	return;
}

__iram_func__
static void _iqk_rxk_setting_8852ab(struct rf_info *rf, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 tmp = 0x0;
	//ADDA RXIQK
    halrf_wreg(rf, 0x12b8 + (path << 13), 0x40000000, 0x1);
    halrf_wreg(rf, 0x030c, 0xff000000, 0x3);   //ADD for DA off
    halrf_wreg(rf, 0x032c, 0xffff0000, 0xa041);   //ADD for DA clk off
    halrf_delay_us(rf, 1);
    halrf_wreg(rf, 0x030c, 0x0c000000, 0x3);
    halrf_wreg(rf, 0x032c, 0x00400000, 0x0);
    halrf_delay_us(rf, 1);
    halrf_wreg(rf, 0x032c, 0x00400000, 0x1);
    halrf_wreg(rf, 0x030c, 0x0c000000, 0x0);
	halrf_delay_us(rf, 1);
    halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0303);   //rst DAC/ADC  fifo 
    halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0000);

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wrf(rf, path, 0x0, 0xf0000, 0x7);
		halrf_wrf(rf, path, 0x20, 0x00100, 0x1);
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x0, 0xf0000, 0x7);
		halrf_wrf(rf, path, 0x2, 0x70000, 0x5);
		halrf_wrf(rf, path, 0x20, 0x00080, 0x1);
		break;
	default:
		break;
	}
	tmp = halrf_rrf(rf, path, 0x18, MASKRF);
	halrf_wrf(rf, path, 0x1f, MASKRF, tmp);
	halrf_wrf(rf, path, 0x1e, 0x0003f, 0x13);
	halrf_delay_us(rf, 2);
	halrf_wrf(rf, path, 0x1e, 0x80000, 0x0);
	halrf_delay_us(rf, 2);
	halrf_wrf(rf, path, 0x1e, 0x80000, 0x1);
	//halrf_delay_us(rf, 64);
	for (tmp = 0; tmp < 128; tmp++)
		halrf_delay_us(rf, 1);

	return;
}

__iram_func__
static bool _iqk_check_cal_8852ab(struct rf_info *rf, u8 path, u8 ktype)
{

	//struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool notready = true, fail = true;
	u32 delay_count = 0x0;
	u32 tmp = 0x0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	delay_count = 0x0;
	while (notready) {
		if (halrf_rreg(rf, 0xbff8, MASKBYTE0) == 0x55) {
			notready = false;
		} else {
			halrf_delay_us(rf, 1);
			delay_count++;
		}
		if (delay_count > 8200) {
			fail = true;
			RF_DBG(rf, DBG_RF_IQK, "[IQK]IQK timeout!!!\n");
			break;
		}
	}
	if (!notready)
		fail = (bool)halrf_rreg(rf, 0x8008, BIT(26));
	halrf_wreg(rf, 0x8010, MASKBYTE0, 0x0);

	//DBG_LOG_SERIOUS(DBGMSG_RF, DBG_WARNING, "[IQK]%x\n", delay_count);

	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, cnt= %d\n", path, delay_count);
	tmp = halrf_rreg(rf, 0x8008, MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, type= %x, 0x8008 = 0x%x \n", path, ktype, tmp);

	return false;
}

__iram_func__
static bool _iqk_one_shot_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				 u8 path, u8 ktype)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool fail = false;
	u32 iqk_cmd = 0x0;
	u8 phy_map;
	u32 addr_rfc_ctl = 0x0;

	phy_map = (BIT(phy_idx) << 4) | BIT(path);

	if (path == RF_PATH_A)
		addr_rfc_ctl = 0x5864;
	else
		addr_rfc_ctl = 0x7864;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_IQK, RFK_ONESHOT_START);
	switch (ktype) {
	case ID_TXAGC:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d TXAGC ============\n", path);
		iqk_cmd = 0x008 | (1 << (4 + path)) | (path << 1);
		break;
	case ID_FLoK_coarse:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d ID_FLoK_coarse ============\n", path);
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x1);
		halrf_wreg(rf, 0x802c, 0x00000fff, 0x011);
		iqk_cmd = 0x108 | (1 << (4 + path));
		break;
	case ID_FLoK_fine:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d ID_FLoK_fine ============\n", path);
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x1);
		halrf_wreg(rf, 0x802c, 0x00000fff, 0x011);
		iqk_cmd = 0x208 | (1 << (4 + path));
		break;
	case ID_TXK:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d ID_TXK ============\n", path);
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x0);
		halrf_wreg(rf, 0x802c, 0x00000fff, 0x025);
		iqk_cmd = 0x008 | (1 << (path + 4)) | (((0x8 + iqk_info->iqk_bw[path] ) & 0xf) << 8);
		break;
	case ID_RXAGC:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d ID_RXAGC ============\n", path);
		iqk_cmd = 0x508 | (1 << (4 + path)) | (path << 1);
		break;
	case ID_RXK:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]============ S%d ID_RXK ============\n", path);
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x1);
		halrf_wreg(rf, 0x802c, 0x0fff0000, 0x011);
		iqk_cmd = 0x008 | (1 << (path + 4)) | (((0xb + iqk_info->iqk_bw[path]) & 0xf) << 8);
		break;
	case ID_NBTXK:
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x0);
		halrf_wreg(rf, 0x802c, 0x00000fff, 0x025);
		iqk_cmd = 0x308 | (1 << (4 + path));
		break;
	case ID_NBRXK:
		halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x1);
		halrf_wreg(rf, 0x802c, 0x0fff0000, 0x011);
		iqk_cmd = 0x608 | (1 << (4 + path));
		break;
	default:
		return false;
		break;
	}

	halrf_wreg(rf, 0x8000, MASKDWORD, iqk_cmd + 1);
	//halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x80b0, 0x10000000, 0x1);
	halrf_delay_us(rf, 1);
	fail = _iqk_check_cal_8852ab(rf, path, ktype);
	if (iqk_info->iqk_xym_en == true)
		_iqk_read_xym_dbcc0_8852ab(rf, path);
	if (iqk_info->iqk_fft_en == true)
		_iqk_read_fft_dbcc0_8852ab(rf, path);
	if (iqk_info->iqk_sram_en == true)
		_iqk_sram_8852ab(rf, path);
	if (iqk_info->iqk_cfir_en == true) {
		if (ktype == ID_TXK) {
			_iqk_read_txcfir_dbcc0_8852ab(rf, path, 0x0);
			_iqk_read_txcfir_dbcc0_8852ab(rf, path, 0x1);
			_iqk_read_txcfir_dbcc0_8852ab(rf, path, 0x2);
			_iqk_read_txcfir_dbcc0_8852ab(rf, path, 0x3);
		} else {
			_iqk_read_rxcfir_dbcc0_8852ab(rf, path, 0x0);
			_iqk_read_rxcfir_dbcc0_8852ab(rf, path, 0x1);
			_iqk_read_rxcfir_dbcc0_8852ab(rf, path, 0x2);
			_iqk_read_rxcfir_dbcc0_8852ab(rf, path, 0x3);
		}
	}

	//8. IQK cotrol RFC
	halrf_wreg(rf, addr_rfc_ctl, 0x20000000, 0x0);

	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_IQK, RFK_ONESHOT_STOP);

	return fail;
}
__iram_func__
static bool _iqk_check_txk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,  u8 path)
{
	u32 x = 0;
	u32 y = 0;
	u32 reg80d4 = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	reg80d4 = halrf_rreg(rf, 0x80d4, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00300000);
	x = halrf_rreg(rf, 0x80fc, 0x0fff0000);	
	y = halrf_rreg(rf, 0x80fc, 0x00000fff);
	x = HALRF_ABS(x, 1024);
	RF_DBG(rf, DBG_RF_IQK, "[IQK] x= %d, y= %d, 0x80fc =%x\n", x, y,  halrf_rreg(rf, 0x80fc, MASKDWORD));
	halrf_wreg(rf, 0x80d4, MASKDWORD, reg80d4);
#if 0
	if ( (x > 250) ||(y > 250))
		return true;
	else
#endif
		return false;
}

__iram_func__
static bool _iqk_check_rxk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,  u8 path)
{
	u32 x = 0;
	u32 y = 0;
	u32 reg80d4 = 0x0;

	RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	reg80d4 = halrf_rreg(rf, 0x80d4, MASKDWORD);
	halrf_wreg(rf, 0x80d4, MASKDWORD, 0x00300000);
	x = halrf_rreg(rf, 0x80fc, 0x0fff0000);	
	y = halrf_rreg(rf, 0x80fc, 0x00000fff);
	x = HALRF_ABS(x, 1024);
	RF_DBG(rf, DBG_RF_IQK, "[IQK] x= %d, y= %d, 0x80fc =%x\n", x, y,  halrf_rreg(rf, 0x80fc, MASKDWORD));
	halrf_wreg(rf, 0x80d4, MASKDWORD, reg80d4);
#if 0
	if ( (x > 250) ||(y > 250))
		return true;
	else
#endif
		return false;
}

__iram_func__
static bool _rxk_group_sel_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				  u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 gp = 0x0;
	u32 rxgn_a[4] = {0x18C, 0x1A0, 0x28C, 0x2A0};
	u32 attc2_a[4] = {0x0, 0x0, 0x07, 0x30};
	u32 attc1_a[4] = {0x7, 0x5, 0x1, 0x1};
	u32 rxgn_g[4] = {0x1CC, 0x1E0, 0x2CC, 0x2E0};
	u32 attc2_g[4] = {0x0, 0x15, 0x3, 0x1a};
	u32 attc1_g[4] = {0x1, 0x0, 0x1, 0x0};
	bool fail = false;
	bool kfail = false;
	u32 rf0 = 0x0;

	//A-mode
	//for idx = 0 1 2 3
	//alias idx idxrxgain = 0x18C 0x1A0 0x28C 0x2A0
	//alias idx idxattC2 = 0x0 0x0 0x07 0x30
	//alias idx idxattC1 = 0x7 0x5 0x1 0x1
	//G-mode

	//for idx = 0 1 2 3
	//alias idx idxrxgain = 0x1CC 0x1E0 0x2CC 0x2E0
	//alias idx idxattC2 = 0x0 0x15 0x3 0x1a
	//alias idx idxattC1 = 0x1 0x0 0x1 0x0

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);

	for (gp = 0; gp < 0x4; gp++) {
		switch (iqk_info->iqk_band[path]) {
		case BAND_ON_24G:
			halrf_wrf(rf, path, 0x00, 0x03ff0, rxgn_g[gp]);
			halrf_wrf(rf, path, 0x83, 0x1fc00, attc2_g[gp]);
			halrf_wrf(rf, path, 0x83, 0x00300, attc1_g[gp]);
			break;
		case BAND_ON_5G:
			halrf_wrf(rf, path, 0x00, 0x03ff0, rxgn_a[gp]);
			halrf_wrf(rf, path, 0x8c, 0x003f8, attc2_a[gp]);
			halrf_wrf(rf, path, 0x8c, 0x01c00, attc1_a[gp]);
			break;
		default:
			break;
		}
		halrf_wreg(rf, 0x8034, 0x00000030, 0x3);
		rf0 = halrf_rrf(rf, path, 0x0, MASKRF);
		halrf_wreg(rf, 0x8024, 0x000fffff, rf0  |iqk_info->syn1to2);
		halrf_wreg(rf, 0x8018, MASKDWORD, 0x40010100);
		halrf_wreg(rf, 0x8124 + (path << 8), 0x0000000f, 0x0);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x1);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x0);
		//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000003, gp);
		halrf_wreg(rf, 0xc60, 0x00000002, 0x1);
		halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
		fail = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_RXK);
		halrf_wreg(rf, 0x9fe0, BIT(16 + gp + path * 4), fail);
		_iqk_check_xym_8852ab(rf, path, gp, ID_RXK);
	}	
	kfail = _iqk_check_rxk_8852ab(rf, phy_idx, path);

	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wrf(rf, path, 0x20, 0x00100, 0x0);
		halrf_wrf(rf, path, 0x1e, 0x80000, 0x0);
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x20, 0x00080, 0x0);
		halrf_wrf(rf, path, 0x1e, 0x80000, 0x0);
		halrf_wrf(rf, path, 0x02, 0x70000, 0x0);
		break;
	default:
		break;
	}

	if (kfail) {
		iqk_info->nb_rxcfir[path] = 0x40000002;
		halrf_wreg(rf, 0x8124 + (path << 8), 0x0000000f, 0x0);
		iqk_info->is_wb_rxiqk[path] = false;
	} else {
		iqk_info->nb_rxcfir[path] = 0x40000000;
		halrf_wreg(rf, 0x8124 + (path << 8), 0x0000000f, 0x5);
		iqk_info->is_wb_rxiqk[path] = true;
	}
/*
	tmp = halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x3c = 0x%x\n", path, 1 << path, tmp);
*/
	return kfail;
}

__iram_func__
static bool _iqk_nbrxk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
			      u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 group = 0x0;
	u32 tmp = 0x0;
	u32 idxrxgain_a = 0x1a0;
	u32 idxattc2_a = 0x00;
	u32 idxattc1_a = 0x5;
	u32 idxrxgain_g = 0x1CC;
	u32 idxattc2_g = 0x0;
	u32 idxattc1_g = 0x1;
	bool fail = false;
	u32 rf0 = 0x0;

	//A-mode
	//for idx = 0 1 2 3
	//alias idx idxrxgain = 0x18C 0x1A0 0x28C 0x2A0
	//alias idx idxattC2 = 0x0 0x0 0x07 0x30
	//alias idx idxattC1 = 0x7 0x5 0x1 0x1

	//G-mode
	//for idx = 0 1 2 3
	//alias idx idxrxgain = 0x1CC 0x1E0 0x2CC 0x2E0
	//alias idx idxattC2 = 0x0 0x15 0x3 0x1a
	//alias idx idxattC1 = 0x1 0x0 0x1 0x0

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wrf(rf, path, 0x0, 0x03ff0, idxrxgain_g);
		halrf_wrf(rf, path, 0x83, 0x1fc00, idxattc2_g);
		halrf_wrf(rf, path, 0x83, 0x00300, idxattc1_g);
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x0, 0x03ff0, idxrxgain_a);
		halrf_wrf(rf, path, 0x8c, 0x003f8, idxattc2_a);
		halrf_wrf(rf, path, 0x8c, 0x01c00, idxattc1_a);
		break;
	default:
		break;
	}
	halrf_wreg(rf, 0x8034, 0x00000030, 0x3);
	rf0 = halrf_rrf(rf, path, 0x0, MASKRF);
	halrf_wreg(rf, 0x8024, 0x000fffff, rf0  |iqk_info->syn1to2);
	halrf_wreg(rf, 0x8018, MASKDWORD, 0x40010100);
	halrf_wreg(rf, 0x8124 + (path << 8), 0x0000000f, 0x0);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x1);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x0);
	//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000003, group);
	halrf_wreg(rf, 0xc60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	fail = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_NBRXK);

	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wrf(rf, path, 0x20, 0x00100, 0x0);
		halrf_wrf(rf, path, 0x1e, 0x80000, 0x0);
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x20, 0x00080, 0x0);
		halrf_wrf(rf, path, 0x1e, 0x80000, 0x0);
		halrf_wrf(rf, path, 0x02, 0x70000, 0x0);
		break;
	default:
		break;
	}

	if (!fail) {
		tmp = halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD);
		iqk_info->nb_rxcfir[path] = tmp | 0x2;
	} else {
		iqk_info->nb_rxcfir[path] = 0x40000002;
	}
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, rest = %x, 0x8%x3c = 0x%x\n", path, fail, 1 << path, halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD));
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8024 = 0x%x\n", path, halrf_rreg(rf, 0x8024, MASKDWORD));
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x54 = 0x%x\n", path, 1 << path, halrf_rreg(rf, 0x8154 + (path << 8), MASKDWORD));
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x04 = 0x%x\n", path, 1 << path, halrf_rreg(rf, 0x8104 + (path << 8), MASKDWORD));
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, RF0x00 = 0x%x\n", path, halrf_rrf(rf, path, 0x0, MASKDWORD));
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, RF0x21 = 0x%x\n", path, halrf_rrf(rf, path, 0x21, 0x000001ff));
#if 0
	xval =  halrf_rreg(rf, 0x813c + (path << 8), 0xfff00000);
	yval =  halrf_rreg(rf, 0x813c + (path << 8), 0x000fff00);

	if(xval > 0x430) {		
		RF_DBG(rf, DBG_RF_IQK, " case a\n");
		fail_x =  true;
	} else if (xval > 0x3c0){	
		RF_DBG(rf, DBG_RF_IQK, " case b\n");
		fail_x = false;
	}else {	
		RF_DBG(rf, DBG_RF_IQK, " case c\n");
		fail_x = true;
	}
	if(( (yval & 0x800) >> 11) == 1) { // -	
		if(yval < 0xfc0) {			
			RF_DBG(rf, DBG_RF_IQK, " case 1\n");
			fail_y = true;
		} else {
			RF_DBG(rf, DBG_RF_IQK, " case 2\n");
			fail_y = false;
		}
	} else { //positive	
		if(yval > 0x30) {			
			RF_DBG(rf, DBG_RF_IQK, " case 3\n");
			fail_y = true;
		} else {
			RF_DBG(rf, DBG_RF_IQK, " case 4\n");
			fail_y = false;
		}
	}
		fail = fail_x | fail_y;
#endif	
	return false;
}

__iram_func__
static void _iqk_rxclk_setting_8852ab(struct rf_info *rf, u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	//07_CLK_Setting_RxIQK_BW40M_Non_DBCC_PHY0_path01_reg
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	if (iqk_info->iqk_bw[path] == CHANNEL_WIDTH_80) { //BW80
		RF_DBG(rf, DBG_RF_IQK, "[IQK]===> BW80\n");
		halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0x4d000a08);
		halrf_wreg(rf, 0x12a0 + (path << 13), 0x00070000, 0x2);
		halrf_wreg(rf, 0x12a0 + (path << 13), 0x00080000, 0x1);
		halrf_wreg(rf, 0x0700, 0x01000000, 0x1);
		halrf_wreg(rf, 0x0700, 0x06000000, 0x1);
	} else {//BW2040
		RF_DBG(rf, DBG_RF_IQK, "[IQK]===> BW20/40\n");
		halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0x44000a08);
		halrf_wreg(rf, 0x12a0 + (path << 13), 0x00070000, 0x1);
		halrf_wreg(rf, 0x12a0 + (path << 13), 0x00080000, 0x1);
		halrf_wreg(rf, 0x0700, 0x01000000, 0x1);
		halrf_wreg(rf, 0x0700, 0x06000000, 0x0);
	}
	return;
}

__iram_func__
static bool _txk_group_sel_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				  u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool fail = false;
	bool kfail = false;
	u8 gp = 0x0;
	u32 a_txgain[4] = {0xE466, 0x646D, 0xE4E2, 0x64ED};
	u32 g_txgain[4] = {0x60e8, 0x60f0, 0x61e8, 0x61ED};
	u32 a_itqt[4] = {0x12, 0x12, 0x12, 0x1b};
	u32 g_itqt[4] = {0x09, 0x12, 0x12, 0x12};
	u32 g_attsmxr[4] = {0x0, 0x1, 0x1, 0x1};
	//u32 tmp = 0x0;
	//Amode CBV
	//for idx = 0 1 2 3
	//alias idx txgain = 0xE466 0x646D 0xE4E2 0x64ED
	//alias idx itqt = 0x12 0x12 0x12 0x1b
	// Gmode CBV
	//for idx = 0 1 2 3
	//alias idx txgain = 0x60e8 0x60f0 0x61e8 0x61ED
	//alias idx itqt = 0x09 0x12 0x12 0x12
	//alias idx attsmxr = 0x0 0x1 0x1 0x1

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
		
	for (gp = 0x0; gp < 0x4; gp++) {
		switch (iqk_info->iqk_band[path]) {
		case BAND_ON_24G:
			halrf_wreg(rf, 0x8148 + (path << 8), 0x0000001f,
				   0x08);
			halrf_wrf(rf, path, 0x56, 0x0ffff, g_txgain[gp]);
			halrf_wrf(rf, path, 0x51, 0x00800, g_attsmxr[gp]);
			halrf_wrf(rf, path, 0x52, 0x00800, g_attsmxr[gp]);
			halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD,
				   g_itqt[gp]);
			break;
		case BAND_ON_5G:
			halrf_wreg(rf, 0x8148 + (path << 8), 0x0000001f,
				   0x04);
			halrf_wrf(rf, path, 0x56, 0x0ffff, a_txgain[gp]);
			halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD,
				   a_itqt[gp]);
			break;
		default:
			break;
		}
		halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x0);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x1);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x1);
		//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000003, gp);
		halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
		fail = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_TXK);
		halrf_wreg(rf, 0x9fe0, BIT(8 + gp + path * 4), fail);
		_iqk_check_xym_8852ab(rf, path, gp, ID_TXK);
	}	
	kfail = _iqk_check_txk_8852ab(rf, phy_idx, path);
	if (kfail) {
		iqk_info->nb_txcfir[path] = 0x40000002;
		halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x0);
		iqk_info->is_wb_txiqk[path] = false;
	} else {
		iqk_info->nb_txcfir[path] = 0x40000000;
		halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x5);
		iqk_info->is_wb_txiqk[path] = true;
	}
/*
	tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x38 = 0x%x\n", path, 1 << path, tmp);
*/
	return kfail;
}

__iram_func__
static bool _iqk_nbtxk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
			      u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 group = 0x2;
	u32 a_mode_txgain = 0x64e2;
	u32 g_mode_txgain = 0x61e8;
	u32 attsmxr = 0x1;
	u32 itqt = 0x12;
	bool fail = false;
	u32 tmp = 0x0;

	//Amode CBV
	//for idx = 0 1 2 3
	//alias idx txgain = 0xE466 0x646D 0xE4E2 0x64ED
	//alias idx itqt = 0x12 0x12 0x12 0x1b
	// Gmode CBV
	//for idx = 0 1 2 3
	//alias idx txgain = 0x60e8 0x60f0 0x61e8 0x61ED
	//alias idx itqt = 0x09 0x12 0x12 0x12
	//alias idx attsmxr = 0x0 0x1 0x1 0x1

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wreg(rf, 0x8148 + (path << 8), 0x0000001f, 0x08);
		halrf_wrf(rf, path, 0x56, 0x0ffff, g_mode_txgain);
		halrf_wrf(rf, path, 0x51, 0x00800, attsmxr);
		halrf_wrf(rf, path, 0x52, 0x00800, attsmxr);
		break;
	case BAND_ON_5G:
		halrf_wreg(rf, 0x8148 + (path << 8), 0x0000001f, 0x04);
		halrf_wrf(rf, path, 0x56, 0x0ffff, a_mode_txgain);
		break;
	default:
		break;
	}
	halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x0);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x1);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x1);
	//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000003, group);
	halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD, itqt);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	fail = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_NBTXK);
	if (!fail) {
		tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
		iqk_info->nb_txcfir[path] = tmp | 0x2;
	} else {
		iqk_info->nb_txcfir[path] = 0x40000002;
	}
/*
	tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, 0x8%x38 = 0x%x\n", path, 1 << path,
	       tmp);
*/
	return fail;
}

__iram_func__
static void _lok_res_table_8852ab(struct rf_info *rf, u8 path, u8 ibias)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, ibias = %x\n", path, ibias);
	halrf_wrf(rf, path, 0xef, MASKRF, 0x2);
	if (iqk_info->iqk_band[path] == BAND_ON_24G)
		halrf_wrf(rf, path, 0x33, MASKRF, 0x0);
	else
		halrf_wrf(rf, path, 0x33, MASKRF, 0x1);
	halrf_wrf(rf, path, 0x3f, MASKRF, ibias);
	halrf_wrf(rf, path, 0xef, MASKRF, 0x0);
	return;
}

__iram_func__
static bool _lok_finetune_check_8852ab(struct rf_info *rf, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool is_fail = false;
	u32 temp = 0x0;
	u32 core_i = 0x0;
	u32 core_q = 0x0;
	u32 fine_i = 0x0;
	u32 fine_q = 0x0;
	u8 ch = 0x0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	temp = halrf_rrf(rf, path, 0x58, MASKRF);
	core_i = (temp & 0xf8000) >> 15;
	core_q = (temp & 0x07c00) >> 10;
	fine_i = (temp & 0x003c0) >> 6;
	fine_q = (temp & 0x0003c) >> 2;
	ch = ((iqk_info->iqk_times /2) % 2) & 0x1;
	
	if (core_i  < 0x2 || core_i  > 0x1d || core_q < 0x2 || core_q > 0x1d) {
		is_fail = true;
	} else {	
		is_fail = false;
		iqk_info->lok_idac[ch][path] = temp;
	}
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, lok_idac[%x][%x] = 0x%x\n", path, ch, path, iqk_info->lok_idac[ch][path]);

	return is_fail;
}

__iram_func__
static bool _iqk_lok_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
			    u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 rf0 = 0x0;
	u8 itqt = 0x12;
	bool fail = false;
	bool tmp = false;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		//halrf_wrf(rf, path, 0x56, 0x0ffff, 0x61e2);//PA=000 Amode
		halrf_wrf(rf, path, 0x56, 0x0ffff, 0xe5e0);//PA=000 Amode
		itqt = 0x09;
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x56, 0x0ffff, 0xe4e0); //PA=001 Amode
		itqt = 0x12;
		break;
	default:
		break;
	}
	halrf_wreg(rf, 0x8034, 0x00000030, 0x3);
	rf0 = halrf_rrf(rf, path, 0x0, MASKRF);
	halrf_wreg(rf, 0x8020, 0x000fffff, rf0 |iqk_info->syn1to2 );
	halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x0);
	//halrf_wreg(rf, 0x8154 + (path << 8), MASKDWORD, 0x00000108);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x1);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x1);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000003, 0x0);
	//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
	halrf_wreg(rf, 0xc60, 0x00000002, 0x1);
	halrf_wreg(rf, 0x8010, 0x000000ff, 0x00);
	// itqt =0x12
	halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD, itqt);
	tmp = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_FLoK_coarse);
	iqk_info->lok_cor_fail[0][path] = tmp;
	halrf_delay_us(rf, 10);
	halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD, itqt);
	tmp = _iqk_one_shot_8852ab(rf, phy_idx, path, ID_FLoK_fine);
	iqk_info->lok_fin_fail[0][path] = tmp;
	fail = _lok_finetune_check_8852ab(rf, path);
	return fail;
}

__iram_func__
static void _iqk_txk_setting_8852ab(struct rf_info *rf, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	//u32 tmp = 0x0;

	//TX init gain setting
	/*0/1:G/A*/

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	//ADDA TXIQK
	halrf_wreg(rf, 0x12b8 + (path << 13), 0x40000000, 0x1);
	//halrf_wreg(rf, 0x32b8, 0x40000000, 0x1);
	halrf_wreg(rf, 0x030c, 0xff000000, 0x1f);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x030c, 0xff000000, 0x13);
	halrf_wreg(rf, 0x032c, 0xffff0000, 0x0001);
	halrf_delay_us(rf, 1);
	halrf_wreg(rf, 0x032c, 0xffff0000, 0x0041);
	halrf_delay_us(rf, 1);
    halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0303);   //rst DAC/ADC  fifo 
    halrf_wreg(rf, 0x20fc, 0xffff0000, 0x0000);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		//05a_G_S0_SetRF_IQKpath_DAK_Reload_20200506
		halrf_wrf(rf, path, 0x90, 0x00003, 0x00);
		halrf_wrf(rf, path, 0xde, 0xfe000, 0x3f);
		halrf_wrf(rf, path, 0x51, 0x80000, 0x0);
		halrf_wrf(rf, path, 0x51, 0x00800, 0x1);
		halrf_wrf(rf, path, 0x52, 0x00800, 0x1);
		halrf_wrf(rf, path, 0x55, 0x00001, 0x0);
		halrf_wrf(rf, path, 0xef, 0x00004, 0x1);
		halrf_wrf(rf, path, 0xdf, 0x00004, 0x0);
		halrf_wrf(rf, path, 0x33, 0x003ff, 0x000);
		halrf_wrf(rf, path, 0x09, MASKRF, 0x80200);
		halrf_wrf(rf, path, 0x08, MASKRF, 0x80200);
		halrf_wrf(rf, path, 0x0, MASKRF, 0x403e0 |iqk_info->syn1to2);
		halrf_delay_us(rf, 1);
		break;
	case BAND_ON_5G:
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
		//05a_A_S0_SetRF_IQKpath_DAK_Reload_20200506
		halrf_wrf(rf, path, 0x85, 0x00003, 0x00);
		halrf_wrf(rf, path, 0xde, 0xfe000, 0x3f);
		halrf_wrf(rf, path, 0x60, 0x00007, 0x7);
		halrf_wrf(rf, path, 0x55, 0x00001, 0x0);
		halrf_wrf(rf, path, 0xef, 0x00004, 0x1);
		halrf_wrf(rf, path, 0xdf, 0x00004, 0x0);
		halrf_wrf(rf, path, 0x33, 0x003ff, 0x100);
		halrf_wrf(rf, path, 0x09, MASKRF, 0x80200);
		halrf_wrf(rf, path, 0x08, MASKRF, 0x80200);
		halrf_wrf(rf, path, 0x3f, MASKRF, 0x1);
		halrf_wrf(rf, path, 0x3f, MASKRF, 0x0);
		halrf_wrf(rf, path, 0x0, MASKRF, 0x403e0 |iqk_info->syn1to2);
		halrf_delay_us(rf, 1);
		break;
	default:
		break;
	}
/*
	tmp = halrf_rrf(rf, path, 0x00, MASKRF);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, rf%x = 0x%x\n", path, path, tmp);
*/
	return;
}

__iram_func__
static void _iqk_txclk_setting_8852ab(struct rf_info *rf, u8 path)
{

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	//04_CLK_Setting_TxIQK_DAC960MHz_PHY0_path01_reg
	halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0xce000a08);
	return;
}

__iram_func__
static void _iqk_info_iqk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				 u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 tmp = 0x0;
	bool flag = 0x0;
	u8 i = 0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	iqk_info->thermal[path] = halrf_get_thermal_8852a(rf, path);	
	iqk_info->thermal_rek_en = false;
/*
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d_thermal = %d\n", path,
	       iqk_info->thermal[path] );
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d_LOK_COR_fail= %d\n", path,
	       iqk_info->lok_cor_fail[0][path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d_LOK_FIN_fail= %d\n", path,
	       iqk_info->lok_fin_fail[0][path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d_TXIQK_fail = %d\n", path,
	       iqk_info->iqk_tx_fail[0][path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d_RXIQK_fail= %d,\n", path,
	       iqk_info->iqk_rx_fail[0][path]);
*/
	flag = iqk_info->lok_cor_fail[0][path];
	halrf_wreg(rf, 0x9fe0, BIT(0) << (path * 4), flag);
	flag = iqk_info->lok_fin_fail[0][path];
	halrf_wreg(rf, 0x9fe0, BIT(1) << (path * 4), flag);
	flag = iqk_info->iqk_tx_fail[0][path];
	halrf_wreg(rf, 0x9fe0, BIT(2) << (path * 4), flag);
	flag = iqk_info->iqk_rx_fail[0][path];
	halrf_wreg(rf, 0x9fe0, BIT(3) << (path * 4), flag);

	tmp = halrf_rreg(rf, 0x8124 + (path << 8), MASKDWORD);
	iqk_info->bp_iqkenable[path] = tmp;
	tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
	iqk_info->bp_txkresult[path] = tmp;
	tmp = halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD);
	iqk_info->bp_rxkresult[path] = tmp;

	halrf_wreg(rf, 0x9fe8, 0x0000ff00, (u8)iqk_info->iqk_times);

	tmp = halrf_rreg(rf, 0x9fe0, 0x0000000f << (path * 4));
	if (tmp != 0x0)
		iqk_info->iqk_fail_cnt++;
	halrf_wreg(rf, 0x9fe8, 0x00ff0000 << (path * 4), iqk_info->iqk_fail_cnt);
	for(i = 0; i < 8; i ++)
		RF_DBG(rf, DBG_RF_IQK, "[IQK](2) xym_check 0x9f%x= 0x%x,\n", 0x30 + (0x10 *i), halrf_rreg(rf, 0x9f30 + (0x10 *i), MASKDWORD));

	return;

}

__iram_func__
void iqk_set_info_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				 u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 tmp = 0x0;
	bool flag = 0x0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	iqk_info->thermal[path] = halrf_get_thermal_8852a(rf, path);	
	iqk_info->thermal_rek_en = false;

	flag = (bool) halrf_rreg(rf, 0x9fe0, BIT(0) << (path * 4));
	iqk_info->lok_cor_fail[0][path] = flag;

	flag = (bool) halrf_rreg(rf, 0x9fe0, BIT(1) << (path * 4));
	iqk_info->lok_fin_fail[0][path] = flag;

	flag = (bool) halrf_rreg(rf, 0x9fe0, BIT(2) << (path * 4));
	iqk_info->iqk_tx_fail[0][path] = flag;

	flag = (bool) halrf_rreg(rf, 0x9fe0, BIT(3) << (path * 4));
	iqk_info->iqk_rx_fail[0][path] = flag;
	
	tmp = halrf_rreg(rf, 0x8124 + (path << 8), MASKDWORD);
	iqk_info->bp_iqkenable[path] = tmp;

	tmp = halrf_rreg(rf, 0x8138 + (path << 8), MASKDWORD);
	iqk_info->bp_txkresult[path] = tmp;

	tmp = halrf_rreg(rf, 0x813c + (path << 8), MASKDWORD);
	iqk_info->bp_rxkresult[path] = tmp;

	iqk_info->iqk_times = (u8) halrf_rreg(rf, 0x9fe8, 0x0000ff00);
	iqk_info->iqk_fail_cnt = halrf_rreg(rf, 0x9fe8, 0x00ff0000 << (path * 4));
	return;
}


__iram_func__
static void _iqk_by_path_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx,
				u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool lok_is_fail = false;
	u8 ibias = 0x1;
	u8 i = 0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	_iqk_txclk_setting_8852ab(rf, path);

	//LOK
	for (i = 0; i < 3; i++) {
		_lok_res_table_8852ab(rf, path, ibias++);
		_iqk_txk_setting_8852ab(rf, path);
		lok_is_fail = _iqk_lok_8852ab(rf, phy_idx, path);
		if (!lok_is_fail)
			break;
	}
	//TXK
	if (iqk_info->is_nbiqk)
		iqk_info->iqk_tx_fail[0][path] = _iqk_nbtxk_8852ab(rf, phy_idx, path);
	else
		iqk_info->iqk_tx_fail[0][path] = _txk_group_sel_8852ab(rf, phy_idx, path);

	//RX
	_iqk_rxclk_setting_8852ab(rf, path);
	_iqk_rxk_setting_8852ab(rf, path);
	if (iqk_info->is_nbiqk) 
		iqk_info->iqk_rx_fail[0][path] = _iqk_nbrxk_8852ab(rf, phy_idx, path);
	else
		iqk_info->iqk_rx_fail[0][path] = _rxk_group_sel_8852ab(rf, phy_idx, path);

	_iqk_info_iqk_8852ab(rf, phy_idx, path);
	return;
}

__iram_func__
void _iqk_mcc_load_lok_8852ab(struct rf_info *rf, enum phl_phy_idx phy, u8 path, u32 reg58)
{
	halrf_wrf(rf, path, 0xdf, BIT(2), 0x1);
	halrf_wrf(rf, path, 0x58, MASKRF, reg58);
	halrf_wrf(rf, path, 0xdf, BIT(2), 0x0);
	return;	
}

__iram_func__
bool iqk_mcc_page_sel_8852ab(struct rf_info *rf, enum phl_phy_idx phy, u8 path)
{
#if 0
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	bool flag = false;

	if (rf->hal_com->band[phy].cur_chandef.center_ch ==  iqk_info->iqk_mcc_ch[0][path]) {
		halrf_wreg(rf, 0x8104 + (path << 8), 0x00000001, 0x0);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
		_iqk_mcc_load_lok_8852ab(rf, phy, path, iqk_info->lok_idac[0][path]);
		flag = true;
	} else if (rf->hal_com->band[phy].cur_chandef.center_ch == iqk_info->iqk_mcc_ch[1][path]) {
		halrf_wreg(rf, 0x8104 + (path << 8), 0x00000001, 0x1);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x1);
		_iqk_mcc_load_lok_8852ab(rf, phy, path, iqk_info->lok_idac[1][path]);
		flag = true;
	} else
		flag = false;
#endif	
	return false;
}

__iram_func__
void iqk_get_ch_info_8852ab(struct rf_info *rf, enum phl_phy_idx phy, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 reg_rf18 = 0x0;
	u32 reg_35c = 0x0;
	u8 ver;
	u8 idx = 0;
	u8 get_empty_table = false;

	for  (idx = 0;  idx < 2; idx++) {
		if (iqk_info->iqk_mcc_ch[idx][path] == 0) {
			get_empty_table = true;
			break;
		}
	}
	if (false == get_empty_table) {
		idx = iqk_info->iqk_table_idx[path] + 1;
		if (idx > 1) {
			idx = 0;
		}		
		RF_DBG(rf, DBG_RF_IQK, "[IQK]we will replace iqk table index(%d), !!!!! \n", idx);
	}	

	//ch = ((iqk_info->iqk_times /2) % 2) & 0x1;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	reg_rf18 = halrf_rrf(rf, path, 0x18, MASKRF);
	reg_35c = halrf_rreg(rf, 0x35c, 0x00000c00);

#if 1
	iqk_info->iqk_band[path] = rf->hal_com->band[phy].cur_chandef.band;
	iqk_info->iqk_bw[path] = rf->hal_com->band[phy].cur_chandef.bw;
	iqk_info->iqk_ch[path] = rf->hal_com->band[phy].cur_chandef.center_ch;
	iqk_info->iqk_mcc_ch[idx][path] = rf->hal_com->band[phy].cur_chandef.center_ch;
	iqk_info->iqk_table_idx[path] =  idx;
#else
	iqk_info->iqk_band[path] = (u8)((reg_rf18 & BIT(16)) >> 16); /*0/1:G/A*/
	iqk_info->iqk_ch[path] = (u8)reg_rf18 & 0xff;
	/*3/2/1:20/40/80*/
	iqk_info->iqk_bw[path] = (u8)((reg_rf18 & 0xc00) >> 10); 
#endif
	RF_DBG(rf, DBG_RF_IQK, "[IQK]iqk_info->iqk_band[%x] = 0x%x\n", path, iqk_info->iqk_band[path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]iqk_info->iqk_bw[%x] = 0x%x\n", path, iqk_info->iqk_bw[path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]iqk_info->iqk_ch[%x] = 0x%x\n", path, iqk_info->iqk_ch[path]);	
	RF_DBG(rf, DBG_RF_IQK, "[IQK]iqk_info->iqk_mcc_ch[%x][%x]= 0x%x\n", idx, path, iqk_info->iqk_mcc_ch[idx][path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d (PHY%d): / DBCC %s/ %s/ CH%d/ %s\n",
		   path, phy,  rf->hal_com->dbcc_en ? "on" : "off",
		   iqk_info->iqk_band[path]  == 0 ? "2G" : (iqk_info->iqk_band[path]  == 1 ? "5G" : "6G"),
		   iqk_info->iqk_ch[path] ,
		   iqk_info->iqk_bw[path] == 0 ? "20M" : (iqk_info->iqk_bw[path] == 1 ? "40M" : "80M"));
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%d rf->hal_com->cv = 0x%x\n", path, rf->hal_com->cv);
#if 1
	if (reg_35c == 0x01)
		iqk_info->syn1to2 = 0x1;
	else
		iqk_info->syn1to2 = 0x0;
#else
	iqk_info->syn1to2 = 0x1;
#endif

	RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, iqk_info->syn1to2= 0x%x\n", path, iqk_info->syn1to2);

	halrf_wreg(rf, 0x9fe0, 0xff000000, iqk_version);
	//2G5G6G = 0/1/2
	halrf_wreg(rf, 0x9fe4, 0x000f << (path * 16), (u8)iqk_info->iqk_band[path]);
	//20/40/80 = 0/1/2
	halrf_wreg(rf, 0x9fe4, 0x00f0 << (path * 16), (u8)iqk_info->iqk_bw[path]);
	halrf_wreg(rf, 0x9fe4, 0xff00 << (path * 16), (u8)iqk_info->iqk_ch[path]); 

	ver = (u8) halrf_get_8852a_nctl_reg_ver();
	halrf_wreg(rf, 0x9fe8, 0x000000ff, ver);


#if 0
	if (iqk_info->iqk_band[path] == BAND_ON_5G)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band = BAND_ON_5G\n");
	else
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band = BAND_ON_24G\n");
		
	if (iqk_info->iqk_bw[path] == CHANNEL_WIDTH_20)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band_width = 20MHz\n");
	else if(iqk_info->iqk_bw[path] == CHANNEL_WIDTH_40)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band_width = 40MHz\n");
	else if(iqk_info->iqk_bw[path] == CHANNEL_WIDTH_80)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band_width = 80MHz\n");
	else if(iqk_info->iqk_bw[path] == CHANNEL_WIDTH_160)
		RF_DBG(rf, DBG_RF_IQK, "[IQK]band_width = 160MHz\n");
	else
		RF_DBG(rf, DBG_RF_IQK, "[IQK]!!!! Channle is not support !!!!\n");
#endif
	return;
}

__iram_func__
void halrf_iqk_reload_8852ab(struct rf_info *rf, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 tmp;
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	iqk_info->reload_cnt++;
	tmp = iqk_info->bp_iqkenable[path];
	halrf_wreg(rf, 0x8124 + (path << 8), MASKDWORD, tmp);
	tmp = iqk_info->bp_txkresult[path];
	halrf_wreg(rf, 0x8138 + (path << 8), MASKDWORD, tmp);
	tmp = iqk_info->bp_rxkresult[path];
	halrf_wreg(rf, 0x813c + (path << 8), MASKDWORD, tmp);
	return;
}

__iram_func__
void iqk_start_iqk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx, u8 path)
{

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	_iqk_by_path_8852ab(rf, phy_idx, path);
	return;
}

__iram_func__
void iqk_restore_8852ab(struct rf_info *rf, u8 path)
{

	struct halrf_iqk_info *iqk_info = &rf->iqk;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	//98_IQK_Reg_Non_DBCC_PHY0_path01_Restore_reg
	halrf_wreg(rf, 0x8138 + (path << 8), MASKDWORD, iqk_info->nb_txcfir[path]);
	//halrf_wreg(rf, 0x8238, MASKDWORD, iqk_info->nb_txcfir[1]);
	halrf_wreg(rf, 0x813c + (path << 8), MASKDWORD, iqk_info->nb_rxcfir[path]);
	//halrf_wreg(rf, 0x823c, MASKDWORD, iqk_info->nb_rxcfir[1]);
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8074, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x80000000);
	halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x80e0, 0x00000001, 0x0);
	halrf_wreg(rf, 0x8120 + (path << 8), MASKDWORD, 0x10010000);
	halrf_wreg(rf, 0x8140 + (path << 8), 0x00000100, 0x0);
	halrf_wreg(rf, 0x8150 + (path << 8), MASKDWORD, 0xe4e4e4e4);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000100, 0x0);
	halrf_wreg(rf, 0x81cc + (path << 8), 0x0000003f, 0x0);
	halrf_wreg(rf, 0x81dc + (path << 8), MASKDWORD, 0x00000002);
	//halrf_wreg(rf, 0x8220, MASKDWORD, 0x10010000);
	//halrf_wreg(rf, 0x8240, 0x00000100, 0x0);
	//halrf_wreg(rf, 0x8250, MASKDWORD, 0xe4e4e4e4);
	//halrf_wreg(rf, 0x8254, 0x00000100, 0x0);
	//halrf_wreg(rf, 0x82cc, 0x0000003f, 0x0);
	//halrf_wreg(rf, 0x82dc, MASKDWORD, 0x00000002);
	halrf_wrf(rf, path, 0xef, 0x00004, 0x0);
	halrf_wrf(rf, path, 0xde, 0xfe000, 0x0);
	halrf_wrf(rf, path, 0xef, 0x00004, 0x0);
	halrf_wrf(rf, path, 0x0, 0xf0000, 0x3);
	halrf_wrf(rf, path, 0x5c, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5e, 0x80000, 0x0);
	halrf_wrf(rf, path, 0x5, 0x00001, 0x1);
	return;
}

__iram_func__
void iqk_afebb_restore_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx, u8 path)
{
	u32	i = 0;
	u32	array_len = 0x0;
	u32 *array = NULL;
	u32	addr = 0, mask = 0,  val = 0;	

	RF_DBG(rf, DBG_RF_IQK, "===> %s\n", __func__);
	
	switch(halrf_kpath_8852a(rf, phy_idx)) {
		case RF_A:			
			array_len = sizeof(array_restore_dbcc_path0) / sizeof(u32);
			array = (u32 *) &array_restore_dbcc_path0;
			break;
		case RF_B:			
			array_len = sizeof(array_restore_dbcc_path1) / sizeof(u32);
			array = (u32 *) &array_restore_dbcc_path1;
			break;
		default:
			array_len = sizeof(array_restore_nondbcc_path01) / sizeof(u32);
			array = (u32 *) &array_restore_nondbcc_path01;
			break;
	}

	while ((i + 1) < array_len) {
		addr = array[i];
		mask = array[i + 1];
		val = array[i + 2];
		halrf_wreg(rf, addr, mask, val);	
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x[%x] = 0x%x\n", addr, mask, val);
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x[%x] = 0x%x\n", addr, mask, halrf_rreg(rf, addr, mask));
		i += 3;
	}
	return;
}

__iram_func__
void iqk_preset_8852ab(struct rf_info *rf, u8 path)
{
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	//03_IQK_Preset_Non_DBCC_PHY0_path01
	struct halrf_iqk_info *iqk_info = &rf->iqk;	
	//u8 ch = 0x0;
	u8 idx = iqk_info->iqk_table_idx[path];

	//ch = ((iqk_info->iqk_times /2) % 2) & 0x1;

	if(rf->hal_com->dbcc_en) {
		halrf_wreg(rf, 0x8104 + (path << 8), 0x00000001, path & 0x1);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, path & 0x1);
	} else {
		halrf_wreg(rf, 0x8104 + (path << 8), 0x00000001, idx);
		halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, idx);
	}

	RF_DBG(rf, DBG_RF_IQK, "[IQK] times = 0x%x, times_2 =%x, ch =%x\n", iqk_info->iqk_times, (iqk_info->iqk_times  /2) , idx);	
	RF_DBG(rf, DBG_RF_IQK, "[IQK] iqk_mcc_ch[%x][%x] = 0x%x\n",  idx, path, iqk_info->iqk_mcc_ch[idx][path]);
	RF_DBG(rf, DBG_RF_IQK, "[IQK] iqk_info->iqk_mcc_ch[%x][%x]= 0x%x\n", idx, path, iqk_info->iqk_mcc_ch[idx][path]);		
	halrf_wrf(rf, path, 0x5, 0x00001, 0x0);
	halrf_wreg(rf, 0x8008, MASKDWORD, 0x00000080);
	halrf_wreg(rf, 0x8080, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x8088, MASKDWORD, 0x81ff010a);
	halrf_wreg(rf, 0x80d0, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x8074, MASKDWORD, 0x80000000);
	halrf_wreg(rf, 0x81dc + (path << 8) , MASKDWORD, 0x00000000);
	return;
}

__iram_func__
void iqk_macbb_setting_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx, u8 path)
{
	u32	i = 0;
	u32	array_len = 0x0;
	u32 *array = NULL;
	u32	addr = 0, mask = 0,  val = 0;	

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===> %s\n", __func__);
	
	switch(halrf_kpath_8852a(rf, phy_idx)) {
		case RF_A:			
			array_len = sizeof(array_set_dbcc_path0) / sizeof(u32);
			array = (u32 *) &array_set_dbcc_path0;
			if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x0) /*CCK @PHY0*/
				halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/
			break;
		case RF_B:			
			array_len = sizeof(array_set_dbcc_path1) / sizeof(u32);
			array = (u32 *) &array_set_dbcc_path1;
			if (halrf_rreg(rf, 0x4970, BIT(1)) == 0x1) /*CCK @PHY1*/
				halrf_wreg(rf, 0x2344, BIT(31), 0x1); /*block CCK CCA*/
			break;
		default:
			array_len = sizeof(array_set_nondbcc_path01) / sizeof(u32);
			array = (u32 *) &array_set_nondbcc_path01;
			break;
	}

	while ((i + 1) < array_len) {
		addr = array[i];
		mask = array[i + 1];
		val = array[i + 2];
		halrf_wreg(rf, addr, mask, val);
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x[%x] = 0x%x\n", addr, mask, val);		
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]0x%x[%x] = 0x%x\n", addr, mask, halrf_rreg(rf, addr, mask));
		i += 3;
	}
	if (halrf_kpath_8852a(rf, phy_idx) == RF_B) {
		halrf_wreg(rf, 0x2704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x2704, BIT(1), 0x1);
	} else {
		halrf_wreg(rf, 0x0704, BIT(1), 0x0); /*bb rst*/
		halrf_wreg(rf, 0x0704, BIT(1), 0x1);
	}		
	halrf_delay_us(rf, 1);
	return;
}

__iram_func__
void halrf_iqk_toneleakage_8852ab(struct rf_info *rf, u8 path)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u32 tmp = 0x0;

	if(!rf->hal_com->dbcc_en)
		iqk_macbb_setting_8852ab(rf, HW_PHY_0, path);
	else if (path == 0x0)
		iqk_macbb_setting_8852ab(rf, HW_PHY_0, path);
	else
		iqk_macbb_setting_8852ab(rf, HW_PHY_1, path);
		
	iqk_preset_8852ab(rf, path);
	_iqk_txclk_setting_8852ab(rf, path);
	// main iqk single tone process
	halrf_wrf(rf, path, 0x0, 0xffff0, 0x203e);
	halrf_wrf(rf, path, 0xde, 0xfe000, 0x3f);
	switch (iqk_info->iqk_band[path]) {
	case BAND_ON_24G:
		halrf_wrf(rf, path, 0x56, 0x0ffff, 0x64e8);
		break;
	case BAND_ON_5G:
		halrf_wrf(rf, path, 0x56, 0x0ffff, 0xe4e8);
		break;
	default:
		break;
	}
	halrf_wreg(rf, 0x8000, 0x00000006, 0x1 & path);
	halrf_wreg(rf, 0x8034, 0x00000030, 0x3);
	halrf_wreg(rf, 0x8018, 0x70000000, 0x2);
	halrf_wreg(rf, 0x8014, 0x00000100, 0x1);
	halrf_wreg(rf, 0x8124 + (path << 8), 0x00000f00, 0x0);
	halrf_wreg(rf, 0x8154 + (path << 8), 0x00000008, 0x1);
	//halrf_wreg(rf, 0x8154 + (path << 8), 0x00000004, 0x0);
	halrf_wreg(rf, 0x8140 + (path << 8), 0x00000100, 0x1);
	tmp = halrf_rrf(rf, path, 0x56, 0x003e0);
	halrf_wreg(rf, 0x8144 + (path << 8), 0x0000001f, tmp);
	tmp = halrf_rrf(rf, path, 0x56, 0x0001f);
	halrf_wreg(rf, 0x8144 + (path << 8), 0x00001f00, tmp);
	halrf_wreg(rf, 0x81dc + (path << 8), MASKDWORD, 0x00000001);
	halrf_wreg(rf, 0x81dc + (path << 8), MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x81cc + (path << 8), MASKDWORD, 0x9);
	halrf_wreg(rf, 0x802c, 0x00000fff, 0x009);
	halrf_wreg(rf, 0x8034, 0x00000001, 0x1);
	halrf_wreg(rf, 0x8034, 0x00000001, 0x0);
	halrf_delay_us(rf, 5);
	halrf_wreg(rf, 0x80d0, 0x00300000, 0x3);
	return;
}

__iram_func__
void halrf_iqk_dbcc_8852ab(struct rf_info *rf, u8 path)
{
	bool bkdbcc = false;	
	u8 phy_idx = 0x0;
	
	bkdbcc = rf->hal_com->dbcc_en;
	rf->hal_com->dbcc_en = true;

	if (path == 0x0)
		phy_idx = HW_PHY_0;
	else
		phy_idx = HW_PHY_1;

	//iqk_mcc_page_sel_8852ab(rf,phy_idx, path);		
	iqk_get_ch_info_8852ab(rf,phy_idx, path);
	iqk_macbb_setting_8852ab(rf, phy_idx, path);
	iqk_preset_8852ab(rf, path);
	iqk_start_iqk_8852ab(rf, phy_idx, path);
	iqk_restore_8852ab(rf, path);
	iqk_afebb_restore_8852ab(rf, phy_idx, path);	
	rf->hal_com->dbcc_en = bkdbcc;
	return;
}

__iram_func__
void halrf_iqk_onoff_8852ab(struct rf_info *rf, bool is_enable)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (is_enable) {
		halrf_wreg(rf, 0x8124, MASKDWORD, 0x00000505); /*ch0*/
		halrf_wreg(rf, 0x8224, MASKDWORD, 0x00000505); /*ch1*/
		iqk_info->is_iqk_enable = true;
	} else {
		halrf_wreg(rf, 0x8124, MASKDWORD, 0x00000000); /*ch0*/
		halrf_wreg(rf, 0x8224, MASKDWORD, 0x00000000); /*ch1*/
		iqk_info->is_iqk_enable = false;
	}
	RF_DBG(rf, DBG_RF_IQK, "[IQK] IQK enable :  %s !!!\n",
	       iqk_info->is_iqk_enable ? "enable" : "disable");
	return;
}

__iram_func__
void halrf_iqk_tx_bypass_8852ab(struct rf_info *rf, u8 path)
{
	if (path == RF_PATH_A) { //path A
		/*ch0*/
		halrf_wreg(rf, 0x8124, 0x00000f00, 0x0);
		halrf_wreg(rf, 0x8138, MASKDWORD, 0x40000002);
	} else {
		/*ch1*/
		halrf_wreg(rf, 0x8224, 0x00000f00, 0x0);
		halrf_wreg(rf, 0x8238, MASKDWORD, 0x40000002);
	}
	return;
}

__iram_func__
void halrf_iqk_rx_bypass_8852ab(struct rf_info *rf, u8 path)
{
	if (path == RF_PATH_A) { //path A
		/*ch0*/
		halrf_wreg(rf, 0x8124, 0x0000000f, 0x0);
		halrf_wreg(rf, 0x813c, MASKDWORD, 0x40000002);
	} else {
		/*ch1*/
		halrf_wreg(rf, 0x8224, 0x0000000f, 0x0);
		halrf_wreg(rf, 0x823c, MASKDWORD, 0x40000002);
	}
	return;
}

__iram_func__
void halrf_iqk_lok_bypass_8852ab(struct rf_info *rf, u8 path)
{
	halrf_wrf(rf, path, 0xdf, 0x00004, 0x1);
	halrf_wrf(rf, path, 0x58, MASKRF, 0x84220);
	return;
}

__iram_func__
void halrf_nbiqk_enable_8852ab(struct rf_info *rf, bool nbiqk_en)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (nbiqk_en) {
		iqk_info->is_nbiqk = true;
	} else {
		iqk_info->is_nbiqk = false;
	}

	return;
}

__iram_func__
void halrf_iqk_xym_enable_8852ab(struct rf_info *rf, bool iqk_xym_en)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (iqk_xym_en) {
		iqk_info->iqk_xym_en = true;
	} else {
		iqk_info->iqk_xym_en = false;
	}

	return;
}

__iram_func__
void halrf_iqk_fft_enable_8852ab(struct rf_info *rf, bool iqk_fft_en)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (iqk_fft_en) {
		iqk_info->iqk_fft_en = true;
	} else {
		iqk_info->iqk_fft_en = false;
	}

	return;
}

__iram_func__
void halrf_iqk_sram_enable_8852ab(struct rf_info *rf, bool iqk_sram_en)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (iqk_sram_en) {
		iqk_info->iqk_sram_en = true;
	} else {
		iqk_info->iqk_sram_en = false;
	}

	return;
}

__iram_func__
void halrf_iqk_cfir_enable_8852ab(struct rf_info *rf, bool iqk_cfir_en)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (iqk_cfir_en) {
		iqk_info->iqk_cfir_en = true;
	} else {
		iqk_info->iqk_cfir_en = false;
	}
	return;
}

__iram_func__
void halrf_iqk_track_8852ab(
	struct rf_info *rf)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 path =0x0, i = 0x0;
	u8 cur_ther = 0x0, ther_avg_cnt = 0;
	u32 ther_avg[2] = {0};

	/*only check path 0 */
	for (path = 0; path < 1; path++) {
		cur_ther = halrf_get_thermal_8852a(rf, path);
		iqk_info->ther_avg[path][iqk_info->ther_avg_idx] = cur_ther;		
		/*Average times */
		ther_avg_cnt = 0;
		for (i = 0; i < 0x4; i++) {
			if (iqk_info->ther_avg[path][i]) {
				ther_avg[path] += iqk_info->ther_avg[path][i];
				ther_avg_cnt++;
#if 0
				RF_DBG(rf, DBG_RF_IQK,
						 "[IQK] thermal avg[%d] = %d\n", i,
						   iqk_info->ther_avg[path][i]);
#endif
			}
		}
		/*Calculate Average ThermalValue after average enough times*/
		if (ther_avg_cnt) {
			cur_ther = (u8)(ther_avg[path] / ther_avg_cnt);
		}
		if (HALRF_ABS(cur_ther, iqk_info->thermal[path] ) > IQK_THR_ReK) {
			iqk_info->thermal_rek_en = true;
		}
		else {
			iqk_info->thermal_rek_en = false;
		}
		//RF_DBG(rf, DBG_RF_IQK, "[IQK]S%x, iqk_ther =%d, ther_now = %d\n", path, iqk->thermal[path], cur_ther);
	}
	iqk_info->ther_avg_idx++;
	if (iqk_info->ther_avg_idx == 0x4)
		iqk_info->ther_avg_idx = 0;
	return;
}

__iram_func__
bool halrf_iqk_get_ther_rek_8852ab(struct rf_info *rf ) {
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	return iqk_info->thermal_rek_en;
}

__iram_func__
u8 halrf_iqk_get_mcc_ch0_8852ab(struct rf_info *rf ) {
	
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	return iqk_info->iqk_mcc_ch[0][0];
}

__iram_func__
u8 halrf_iqk_get_mcc_ch1_8852ab(struct rf_info *rf ) {
	
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	return iqk_info->iqk_mcc_ch[1][0];
}

__iram_func__
void halrf_check_fwiqk_done_8852ab(struct rf_info *rf)
{
	u32 counter = 0x0;
	bool flag = false;
#if 1
	while (1) {
		if (halrf_rreg(rf, 0xbff8, MASKBYTE0) == 0xaa  || counter > 3000) {
			flag = true;
			break;
		}
		counter++;
		halrf_delay_us(rf, 10);
	};
#else
	for(counter = 0; counter < 6000; counter++)
		halrf_delay_us(rf, 10);
#endif
	if(flag) {
		RF_DBG(rf, DBG_RF_IQK, "[IQK] Load FW Done, counter = %d!!\n", counter);
	} else {
		RF_DBG(rf, DBG_RF_IQK, "[IQK] Load FW Fail, counter = %d!!\n", counter);	
		halrf_iqk_tx_bypass_8852ab(rf, RF_PATH_A);
		halrf_iqk_tx_bypass_8852ab(rf, RF_PATH_B);
		halrf_iqk_rx_bypass_8852ab(rf, RF_PATH_A);
		halrf_iqk_rx_bypass_8852ab(rf, RF_PATH_B);
		halrf_iqk_lok_bypass_8852ab(rf, RF_PATH_A);		
		halrf_iqk_lok_bypass_8852ab(rf, RF_PATH_B);
	}
	halrf_wreg(rf, 0x8010, 0x000000ff,0x0);
	return;
}

__iram_func__
void halrf_enable_fw_iqk_8852ab(struct rf_info *rf, bool is_fw_iqk)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;

	if (is_fw_iqk) {
		iqk_info->is_fw_iqk = true;
	} else {
		iqk_info->is_fw_iqk = false;
	}
	return;
}

__iram_func__ 
u8 halrf_get_iqk_times_8852ab(struct rf_info *rf) {
	u8 times  = 0x0;

	times = (u8) halrf_rreg(rf, 0x9fe8, 0x0000ff00);
	return times;
}

__iram_func__ 
u32 halrf_get_iqk_ver_8852a(void)
{
	return iqk_version;
}

__iram_func__
void iqk_init_8852ab(struct rf_info *rf)
{
	struct halrf_iqk_info *iqk_info = &rf->iqk;
	u8 idx, path,i;
	
	halrf_wreg(rf, 0x9fe0, MASKDWORD, 0x0);	
	for(i = 0; i < 8; i ++)
		halrf_wreg(rf, 0x9f30 + (i *0x10), MASKDWORD, 0x0);
	if (!iqk_info->is_iqk_init) {		
		RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
		iqk_info->is_iqk_init = true;
		iqk_info->is_nbiqk = false;
		iqk_info->iqk_fft_en = false;
		iqk_info->iqk_sram_en = false;
		iqk_info->iqk_cfir_en = false;
		iqk_info->iqk_xym_en = false;
		iqk_info->thermal_rek_en = false;
		iqk_info->ther_avg_idx = 0x0;
		iqk_info->iqk_times = 0x0;
		iqk_info->is_fw_iqk = true;
		/*channel/path/TRX(TX:0, RX:1) */
		iqk_info->iqk_times = 0x0;
		for (idx = 0; idx < 2; idx++) { //channel		
			iqk_info->iqk_channel[idx] = 0x0;
			for (path = 0; path < ss_8852a; path++) {//path
				iqk_info->lok_cor_fail[idx][path] = false;
				iqk_info->lok_fin_fail[idx][path] = false;
				iqk_info->iqk_tx_fail[idx][path] = false;
				iqk_info->iqk_rx_fail[idx][path] = false;
				iqk_info->iqk_mcc_ch[idx][path] = 0x0;
				iqk_info->iqk_table_idx[path] = 0x0;
			}
		}
	}
	return;
}

#if 0
__iram_func__
void halrf_doiqk_8852ab(struct rf_info *rf, bool force, enum phl_phy_idx phy_idx, u8 path)
{


	u32 backup_mac[2] = {0x0};
	u32 backup_bb[2] = {0x0};
	u32 backup_rf0[6] = {0x0};
	u32 backup_rf1[6] = {0x0};
	u32 backup_mac_reg[1] = {0x0};
	u32 backup_bb_reg[1] = {0x2344};
	u32 backup_rf_reg0[6] = {0xef, 0xde, 0x0, 0x1e, 0x2, 0x5};
	u32 backup_rf_reg1[6] =	{0xef, 0xde, 0x0, 0x1e, 0x2, 0x5};

	struct halrf_iqk_info *iqk_info = &rf->iqk;

	//halrf_btc_rfk_ntfy(rf, ((BIT(phy_idx) << 4) | RF_AB), RF_BTC_IQK, RFK_ONESHOT_START);
	RF_DBG(rf, DBG_RF_IQK, "[IQK]==========IQK strat!!!!!==========\n");
	iqk_info->version = iqk_version;
	iqk_info->iqk_step = 0;
	iqk_info->rxiqk_step = 0;
	iqk_info->reload_cnt = 0;

	//RF_DBG(rf, DBG_RF_IQK, "[IQK]Test Ver 0x%x\n", iqk_version);
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]Test Ver 0x%x\n", 0x2);
	//u32 addr = 0x0;
	//for (addr = 0x8000; addr < 0x8300; addr += 0x4)
	//	RF_DBG(rf, DBG_RF_IQK, "[IQK]  %8x,  %8x \n", addr, halrf_rreg(rf, addr + 0x4, MASKDWORD));

	if (iqk_mcc_page_sel_8852ab(rf,phy_idx, path))
		return;
	iqk_get_ch_info_8852ab(rf,phy_idx, path);
	iqk_backup_mac_bb_8852a(rf, path, backup_mac, backup_bb, backup_mac_reg, backup_bb_reg);
	iqk_backup_rf0_8852a(rf, path, backup_rf0, backup_rf_reg0);
	iqk_backup_rf1_8852a(rf, path, backup_rf1, backup_rf_reg1);
	iqk_macbb_setting_8852ab(rf, phy_idx, path);
	iqk_preset_8852ab(rf, path);
	iqk_start_iqk_8852ab(rf, phy_idx, path);
	iqk_restore_8852ab(rf, path);
	iqk_afebb_restore_8852ab(rf, phy_idx, path);	
	iqk_restore_mac_bb_8852a(rf, path, backup_mac, backup_bb, backup_mac_reg, backup_bb_reg);
	iqk_restore_rf0_8852a(rf, path, backup_rf0, backup_rf_reg0);
	iqk_restore_rf1_8852a(rf, path, backup_rf1, backup_rf_reg1);
	iqk_info->iqk_times++;
	//halrf_btc_rfk_ntfy(rf, ((BIT(phy_idx) << 4) | RF_AB), RF_BTC_IQK, RFK_ONESHOT_STOP);	
	RF_DBG(rf, DBG_RF_IQK, "[IQK]==========IQK End!!!!!==========\n");
	return;
}

__iram_func__
void halrf_iqk_8852ab(struct rf_info *rf, enum phl_phy_idx phy_idx, bool force)
{
	//RF_DBG(rf, DBG_RF_IQK, "[IQK]===>%s\n", __func__);
	iqk_init_8852ab(rf);
	switch(halrf_kpath_8852a(rf, phy_idx)) {
	case RF_A:
		halrf_doiqk_8852ab(rf, force, phy_idx, RF_PATH_A);
		break;
	case RF_B:		
		halrf_doiqk_8852ab(rf, force, phy_idx, RF_PATH_B);
		break;
	case RF_AB:		
		halrf_doiqk_8852ab(rf, force, phy_idx, RF_PATH_A);
		halrf_doiqk_8852ab(rf, force, phy_idx, RF_PATH_B);
		break;
	default:
		break;
	}
	return;
}
#endif

#endif
