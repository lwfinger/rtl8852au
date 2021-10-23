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
#include "halrf_precomp.h"

u32 halrf_get_sys_time(struct rf_info *rf)
{
	return 0;
}

u32 halrf_cal_bit_shift(u32 bit_mask)
{
	u32 i;

	for (i = 0; i <= 31; i++) {
		if ((bit_mask >> i) & BIT0)
			break;
	}
	return i;
}

void halrf_wreg(struct rf_info *rf, u32 addr, u32 mask, u32 val)
{
	u32 ori_val, bit_shift;

//	u32 page_temp;
//	u32 offset_temp;	
//	u32 temp = 0;

//	page_temp = addr & 0xff00;
//	offset_temp = addr & 0xff;

//	if ((page_temp != 0x4c00) && (page_temp != 0x4d00)) {
//		if (offset_temp <= 0x9c)
//			temp = halrf_r32(rf, offset_temp | 0x4c00);
//	}

	if (mask != MASKDWORD) {
		ori_val = halrf_r32(rf, addr);
		bit_shift = halrf_cal_bit_shift(mask);
		val = ((ori_val) & (~mask)) |( ((val << bit_shift)) & mask);
	}
	halrf_w32(rf, addr, val);

//	if ((page_temp != 0x4c00) && (page_temp != 0x4d00)) {
//		if (offset_temp <= 0x9c)
//			halrf_w32(rf, offset_temp | 0x4c00, temp);
//	}
}

u32 halrf_rreg(struct rf_info *rf, u32 addr, u32 mask)
{
	u32 reg_val = 0, ori_val, bit_shift;

	ori_val = halrf_r32(rf, addr);
	bit_shift = halrf_cal_bit_shift(mask);
	reg_val = (ori_val & mask) >> bit_shift;

	return reg_val;
}

void halrf_delay_10us(struct rf_info *rf, u32 count)
{
	u32 i;

	for (i = 0; i < count; i++)
		halrf_delay_us(rf, 10);
}

void halrf_fill_h2c_cmd(struct rf_info *rf, u16 cmdlen, u8 cmdid,
			u8 classid, u32 cmdtype, u32 *pval)
{
	u32 rt_val = 0;
	struct rtw_g6_h2c_hdr hdr = {0};
	struct rtw_hal_com_t *hal_com = NULL;

	hdr.h2c_class = classid;
	hdr.h2c_func = cmdid;
	hdr.type = cmdtype;
	hdr.content_len = cmdlen;
	hal_com = rf->hal_com;
	RF_DBG(rf, DBG_RF_FW, "======>%s   H2C: %x %x %x\n",
		__func__, classid, cmdid, cmdlen);
	rt_val =  rtw_hal_mac_send_h2c(hal_com, &hdr, pval);
	if (rt_val != 0)
		RF_WARNING("Error H2C CLASS=%d, ID=%d\n", classid, cmdid);
}

