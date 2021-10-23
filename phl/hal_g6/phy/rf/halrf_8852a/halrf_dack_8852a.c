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

const u32 array_dack_init_8852a[] = {
	0x00, 0x0030EB41,
	0x04, 0x06000000,
	0x08, 0x00000000,
	0x0c, 0x00000000,
	0x10, 0xa2000001,
	0x14, 0x00000000,
	0x18, 0x00000000,
	0x1c, 0x00000000,
	0x20, 0x00000000,
	0x24, 0x00000000,
	0x30, 0x00000000,
	0x50, 0x0030EB41,
	0x54, 0x06000000,
	0x58, 0x00000000,
	0x5c, 0x00000000,
	0x60, 0xa2000001,
	0x64, 0x00000000,
	0x68, 0x00000000,
	0x6c, 0x00000000,
	0x70, 0x00000000,
	0x74, 0x00000000
};

void halrf_addck_backup_8852a(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;

	halrf_wreg(rf, 0x4cd8, 0x300, 0x0);
	dack->addck_d[0][0] = (u16)halrf_rreg(rf, 0x4020,0xffc00) ;
	dack->addck_d[0][1] = (u16)halrf_rreg(rf, 0x4020,0x003ff) ;

	halrf_wreg(rf, 0x4dd8, 0x300, 0x0);
	dack->addck_d[1][0] = (u16)halrf_rreg(rf, 0x4024,0xffc00) ;
	dack->addck_d[1][1] = (u16)halrf_rreg(rf, 0x4024,0x003ff) ;
}

void halrf_addck_reload_8852a(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	/*S0*/
	halrf_wreg(rf, 0x4cd4, 0x3ff0000, dack->addck_d[0][0]);
	halrf_wreg(rf, 0x4cd8, 0xf, (dack->addck_d[0][1] >> 6));
	halrf_wreg(rf, 0x4cd4, 0xfc000000, (dack->addck_d[0][1] & 0x3f));
	/*manual*/
	halrf_wreg(rf, 0x4cd8, 0x30, 0x3);
	/*S1*/
	halrf_wreg(rf, 0x4dd4, 0x3ff0000, dack->addck_d[1][0]);
	halrf_wreg(rf, 0x4dd8, 0xf, (dack->addck_d[1][1] >> 6));
	halrf_wreg(rf, 0x4dd4, 0xfc000000, (dack->addck_d[1][1] & 0x3f));
	/*manual*/
	halrf_wreg(rf, 0x4dd8, 0x30, 0x3);
}

void halrf_dack_backup_s0_8852a(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 i;

	halrf_wreg(rf, 0x4c00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x1);
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x1);
	/*MSBK*/
	for (i = 0; i < 0x10; i++) {
		/*S0*/
		halrf_wreg(rf, 0x4c00, 0xf0000000, i);
		dack->msbk_d[0][0][i] = (u8)halrf_rreg(rf, 0x4c44, 0xff00);
		halrf_wreg(rf, 0x4c50, 0xf0000000, i);
		dack->msbk_d[0][1][i] = (u8)halrf_rreg(rf, 0x4c94, 0xff00);
	}
	/*biasK*/
	dack->biask_d[0][0] = (u16)halrf_rreg(rf, 0x4c30, 0x3ff000);
	dack->biask_d[0][1] = (u16)halrf_rreg(rf, 0x4c80, 0x3ff000);
	/*DADCK*/
	dack->dadck_d[0][0] = (u8)halrf_rreg(rf, 0x4c48, 0xff00) - 8;
	dack->dadck_d[0][1] = (u8)halrf_rreg(rf, 0x4c98, 0xff00) - 8;
}

void halrf_dack_backup_s1_8852a(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 i;
	halrf_wreg(rf, 0x4d00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4d50, BIT(3), 0x1);
	halrf_wreg(rf, 0x4db8, BIT(30), 0x1);
	/*MSBK*/
	for (i = 0; i < 0x10; i++) {
		/*S1*/
		halrf_wreg(rf, 0x4d00, 0xf0000000, i);
		dack->msbk_d[1][0][i] = (u8)halrf_rreg(rf, 0x4d44, 0xff00);
		halrf_wreg(rf, 0x4d50, 0xf0000000, i);
		dack->msbk_d[1][1][i] = (u8)halrf_rreg(rf, 0x4d94, 0xff00);
	}
	/*biasK*/
	dack->biask_d[1][0] = (u16)halrf_rreg(rf, 0x4d30, 0x3ff000);
	dack->biask_d[1][1] = (u16)halrf_rreg(rf, 0x4d80, 0x3ff000);
	/*DADCK*/
	dack->dadck_d[1][0] = (u8)halrf_rreg(rf, 0x4d48, 0xff00) - 8;
	dack->dadck_d[1][1] = (u8)halrf_rreg(rf, 0x4d98, 0xff00) - 8;
}

void halrf_dack_reload_by_path_8852a(struct rf_info *rf, u8 path, u8 index)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 temp = 0 , temp_offset, temp_reg;
	u8 i;
	u32 idx_offset, path_offset;

	if (index ==0)
		idx_offset = 0;
	else
		idx_offset = 0x50;

	if (path == RF_PATH_A)
		path_offset = 0;
	else
		path_offset = 0x100;

	temp_offset = idx_offset + path_offset;
	/*msbk_d: 15/14/13/12*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+12] << (i * 8);
	}
	temp_reg = 0x4c14 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 11/10/9/8*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+8] << (i * 8);
	}
	temp_reg = 0x4c18 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 7/6/5/4*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+4] << (i * 8);
	}
	temp_reg = 0x4c1c + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 3/2/1/0*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i] << (i * 8);
	}
	temp_reg = 0x4c20 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf,temp_reg, MASKDWORD));
	/*dadak_d/biask_d*/
	temp = 0x0;
	temp =   (dack->biask_d[path][index] << 22) |
		 (dack->dadck_d[path][index] << 14);
	temp_reg = 0x4c24 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
}

void halrf_dack_reload_8852a(struct rf_info *rf)
{
	u8 i;
	u32 temp1, temp2;
	u32 c = 0;

	temp1 = halrf_rreg(rf, 0x4cb8, MASKDWORD);
	temp2 = halrf_rreg(rf, 0x4db8, MASKDWORD);
#if 1
	// step1 
	halrf_wreg(rf, 0x4CB8, BIT(30), 0x1);
	halrf_wreg(rf, 0x4DB8, BIT(30), 0x1);
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4cb8=0x%x\n", halrf_rreg(rf, 0x4cb8, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4db8=0x%x\n", halrf_rreg(rf, 0x4db8, MASKDWORD));
	while (halrf_rreg(rf, 0x4c40, BIT(22)) == 0) {
		halrf_delay_us(rf, 1);	
		c++;

		if (c > 5000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]time out\n");
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4c40---1=0x%x\n", halrf_rreg(rf, 0x4c40, MASKDWORD));

  	halrf_wreg(rf, 0x0B2C, BIT(31), 0x1);
	halrf_wreg(rf, 0x4CE0, BIT(19), 0x0);
	halrf_wreg(rf, 0x4DE0, BIT(19), 0x0);
	// step2
	for (i = 0; i < 2; i++)
		halrf_dack_reload_by_path_8852a(rf, RF_PATH_A, i);
	halrf_wreg(rf, 0x4c00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c10, 0x80000000, 0x1);
	halrf_wreg(rf, 0x4c60, 0x80000000, 0x1);
	halrf_wreg(rf, 0x4CE0, BIT(19), 0x1);
	halrf_wreg(rf, 0x0B2C, BIT(31), 0x0);
	halrf_wreg(rf, 0x4c00, BIT(3), 0x0);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x0);
	halrf_wreg(rf, 0x0B2C, BIT(31), 0x1);
	// step3
	halrf_wreg(rf, 0x4ce0, BIT(19), 0x0);
	// step4
	for (i = 0; i < 2; i++)
		halrf_dack_reload_by_path_8852a(rf, RF_PATH_B, i);
	halrf_wreg(rf, 0x4D00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4D50, BIT(3), 0x1);
	halrf_wreg(rf, 0x4DE0, BIT(19), 0x1);
	halrf_wreg(rf, 0x0B2C, BIT(31), 0x0);
	halrf_wreg(rf, 0x4D00, BIT(3), 0x0);
	halrf_wreg(rf, 0x4D50, BIT(3), 0x0);
	// step5
	halrf_wreg(rf, 0x0b2c, BIT(31), 0x1);
	halrf_wreg(rf, 0x4ce0, BIT(19), 0x1);
	halrf_wreg(rf, 0x0b2c, BIT(31), 0x0);
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4c44=0x%x\n", halrf_rreg(rf, 0x4c44, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4d44=0x%x\n", halrf_rreg(rf, 0x4d44, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4cec=0x%x\n", halrf_rreg(rf, 0x4cec, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4ce0=0x%x\n", halrf_rreg(rf, 0x4ce0, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x0b2c=0x%x\n", halrf_rreg(rf, 0x0b2c, MASKDWORD));
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4c40=0x%x\n", halrf_rreg(rf, 0x4c40, MASKDWORD));
	halrf_wreg(rf, 0x4cb8, MASKDWORD, temp1);
	halrf_wreg(rf, 0x4db8, MASKDWORD, temp2);
#endif

#if 0
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x1);
	halrf_wreg(rf, 0x4db8, BIT(30), 0x1);
	halrf_wreg(rf, 0x0b2c, BIT(31), 0x0);
	while (halrf_rreg(rf, 0x4c40, BIT(22)) == 0) {
		halrf_delay_us(rf, 1);	
		c++;

		if (c > 5000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]time out\n");
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x4c40---1=0x%x\n", halrf_rreg(rf, 0x4c40, MASKDWORD));


	//step1
//????	halrf_wreg(rf, 0x4ce0, BIT(19), 0x1);
	halrf_wreg(rf, 0x4c00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x1);
//	halrf_wreg(rf, 0x4cb8, BIT(30), 0x1);
//	halrf_wreg(rf, 0x4db8, BIT(30), 0x1);
//
	halrf_wreg(rf, 0x4ce0, BIT(19), 0x1);
	halrf_wreg(rf, 0x4de0, BIT(19), 0x0);
	for (i = 0; i < 2; i++)
		halrf_dack_reload_by_path_8852a(rf, RF_PATH_A, i);
	halrf_wreg(rf, 0x4c10, 0x80000000, 0x1);
	halrf_wreg(rf, 0x4c60, 0x80000000, 0x1);
//	halrf_dack_backup_s0_8852a(rf);
	halrf_wreg(rf, 0x4c00, BIT(3), 0x0);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x0);
	//step2
	halrf_wreg(rf, 0x4ce0, BIT(19), 0x0);
	halrf_wreg(rf, 0x4c00, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c50, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c10, 0x80000000, 0x0);
	halrf_wreg(rf, 0x4c60, 0x80000000, 0x0);
	halrf_wreg(rf, 0x4de0, BIT(19), 0x1);
	//step3
	for (i = 0; i < 2; i++)
		halrf_dack_reload_by_path_8852a(rf, RF_PATH_B, i);
	halrf_wreg(rf, 0x4d10, 0x80000000, 0x1);
	halrf_wreg(rf, 0x4d60, 0x80000000, 0x1);
//	halrf_dack_backup_s1_8852a(rf);
	halrf_wreg(rf, 0x4d00, BIT(3), 0x0);
	halrf_wreg(rf, 0x4d50, BIT(3), 0x0);
	//step4
	halrf_wreg(rf, 0x4ce0, BIT(19), 0x1);
	//
//	halrf_wreg(rf, 0x4cb8, BIT(30), 0x0);
//	halrf_wreg(rf, 0x4db8, BIT(30), 0x0);
	halrf_wreg(rf, 0x4cb8, MASKDWORD, temp1);
	halrf_wreg(rf, 0x4db8, MASKDWORD, temp2);
#endif
}

void halrf_dack_bbrf_config_8852a(struct rf_info *rf)
{
	u32 temp;

	temp = halrf_rreg(rf, 0x4c00, MASKDWORD);
	halrf_wrf(rf, RF_PATH_A, 0x0, 0xfffff, 0x40001);
	halrf_wrf(rf, RF_PATH_B, 0x0, 0xfffff, 0x40001);
	halrf_wreg(rf, 0x4c00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4d00, MASKDWORD, 0x0030EB49);
}

#define t_avg 100

void halrf_check_addc_8852a(struct rf_info *rf, enum rf_path path)
{
	u32 temp, dc_re, dc_im;
	u32 i, m, p, t;
	u32 re[t_avg], im[t_avg];

	halrf_wreg(rf, 0x09f4, BIT(24), 0x0);
	halrf_wreg(rf, 0x09f8, 0x80000000, 0x1);
	halrf_wreg(rf, 0x09f4, 0xff, 0x1);
	halrf_wreg(rf, 0x09f0, 0xf00, 0x2);
	halrf_wreg(rf, 0x09f0, 0xf, 0x0);
	if (path  == RF_PATH_A)
		halrf_wreg(rf, 0x09f0, 0xc0, 0x2);
	else
		halrf_wreg(rf, 0x09f0, 0xc0, 0x3);
#if 0
	temp = halrf_rreg(rf, 0x40b0, 0xffffffff);
	dc_re = (temp & 0xfff000) >> 12;
	dc_im = temp & 0xfff;
#endif
	for (i = 0; i < t_avg; i++) {
		temp = halrf_rreg(rf, 0x40b0, 0xffffffff);
		re[i] = (temp & 0xfff000) >> 12;
		im[i] = temp & 0xfff;
//		RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,re[i]= 0x%x,im[i] =0x%x\n",
//		path, re[i], im[i]);
	}

	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (re[i] > 0x800)
			m = (0x1000 - re[i]) + m;
		else
			p = re[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_re = t;

	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (im[i] > 0x800)
			m = (0x1000 - im[i]) + m;
		else
			p = im[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_im = t;

	RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,dc_re = 0x%x,dc_im =0x%x\n",
		path, dc_re, dc_im);

#if 0
	for (i = 0; i < t_avg; i++) {
		if (path == RF_PATH_A)
			temp = halrf_rreg(rf, 0x438c, 0xffffffff);
		else
			temp = halrf_rreg(rf, 0x448c, 0xffffffff);
		re[i] = (temp & 0xfff000) >> 12;
		im[i] = temp & 0xfff;
//		RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,re[i]= 0x%x,im[i] =0x%x\n",
//		path, re[i], im[i]);
	}

	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (re[i] > 0x800)
			m = (0x1000 - re[i]) + m;
		else
			p = re[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_re = t;

	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (im[i] > 0x800)
			m = (0x1000 - im[i]) + m;
		else
			p = im[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_im = t;

	RF_DBG(rf, DBG_RF_DACK, "[DACK]BB report S%d,dc_re = 0x%x,dc_im =0x%x\n",
		path, dc_re, dc_im);
#endif
}

void halrf_dack_clk_en_8852a(struct rf_info *rf, bool en)
{
	/*DACK clock enalbe*/
	if (en) {
		halrf_wreg(rf, 0x4c00, BIT(3), 0x1);
		halrf_wreg(rf, 0x4d00, BIT(3), 0x1);
	} else {
		halrf_wreg(rf, 0x4c00, BIT(3), 0x0);
		halrf_wreg(rf, 0x4d00, BIT(3), 0x0);
	}
}

void halrf_addck_8852a(struct rf_info *rf)
{
	u32 c;
//	u32 temp;

//	temp = halrf_rreg(rf, 0x4c0c, MASKDWORD);
	/*S0*/
#if 1
	/*1.ADC & clk enable */
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x1);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);	//20200408

	/*4.Reset calibration*/
	halrf_wreg(rf, 0x4ccc, BIT(0), 0x0);		//20200408
	halrf_wreg(rf, 0x4ccc, BIT(0), 0x1);		//20200408

	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0xf);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x0);
	/*2.ADC input not from RXBB & ADC input short*/
	halrf_wreg(rf, 0x0b2c, BIT(16), 0x0);	
	halrf_wreg(rf, 0x4cdc, BIT(1), 0x1);
	/*3.release ADC reset*/
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0x3);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]before S0 ADDCK\n");
	halrf_check_addc_8852a(rf, RF_PATH_A);
	/*4.Reset calibration*/
//	halrf_wreg(rf, 0x4ccc, BIT(0), 0x0);	//20200408
//	halrf_wreg(rf, 0x4ccc, BIT(0), 0x1);	//20200408
	/*average 128 times*/
	halrf_wreg(rf, 0x4cd8, BIT(7) | BIT(6), 0x3);		//20200408
	/*5.trigger dc offset calibration*/
	halrf_wreg(rf, 0x4cd8, BIT(11), 0x1);
	halrf_wreg(rf, 0x4cd8, BIT(11), 0x0);
	halrf_delay_us(rf, 1);
	/*check if cal done*/
	halrf_wreg(rf, 0x4cd8, 0x300, 0x1);
	c = 0;
	while (halrf_rreg(rf, 0x4020, BIT(0)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 ADDCK timeout\n");
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]ADDCK c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S0 ADDCK\n");
	halrf_check_addc_8852a(rf, RF_PATH_A);
	/*restore*/
	halrf_wreg(rf, 0x4cdc, BIT(1), 0x0);
	halrf_wreg(rf, 0x0b2c, BIT(16), 0x1);
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0xc);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x0);
#endif

#if 1
	/*S1*/
	/*1.ADC & clk enable */
	halrf_wreg(rf, 0x4db8, BIT(30), 0x1);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);	//20200408
	/*4.Reset calibration*/
	halrf_wreg(rf, 0x4dcc, BIT(0), 0x0);		//20200408
	halrf_wreg(rf, 0x4dcc, BIT(0), 0x1);		//20200408


	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0xf);
	halrf_wreg(rf, 0x4d0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x0);
	/*2.ADC input not from RXBB & ADC input short*/
	halrf_wreg(rf, 0x0b2c, BIT(16), 0x0);	
	halrf_wreg(rf, 0x4ddc, BIT(1), 0x1);
	/*3.release ADC reset*/
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0x3);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]before S1 ADDCK\n");
	halrf_check_addc_8852a(rf, RF_PATH_B);
	/*4.Reset calibration*/
//	halrf_wreg(rf, 0x4dcc, BIT(0), 0x0);	//20200408
//	halrf_wreg(rf, 0x4dcc, BIT(0), 0x1);	//20200408
	/*5.trigger dc offset calibration*/
	halrf_wreg(rf, 0x4dd8, BIT(11), 0x1);
	halrf_wreg(rf, 0x4dd8, BIT(11), 0x0);
	halrf_delay_us(rf, 1);
	/*check if cal done*/
	halrf_wreg(rf, 0x4dd8, 0x300, 0x1);
	c = 0;
	while (halrf_rreg(rf, 0x4024,BIT(0)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 ADDCK timeout\n");
			break;
		};
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]ADDCK c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S1 ADDCK\n");
	halrf_check_addc_8852a(rf, RF_PATH_B);
	/*restore*/
	halrf_wreg(rf, 0x4ddc, BIT(1), 0x0);
	halrf_wreg(rf, 0x0b2c, BIT(16), 0x1);
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0xc);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x1);
	halrf_wreg(rf, 0x4db8, BIT(30), 0x0);
#endif
}

void halrf_check_dadc_8852a(struct rf_info *rf, enum rf_path path)
{
//	u32 temp;

//	temp = halrf_rreg(rf, 0x4c0c, MASKDWORD);
	halrf_wreg(rf, 0x0b2c, BIT(30), 0x0);
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0xf);
	halrf_wreg(rf, 0x0b0c, 0x0f000000, 0x3);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, BIT(16), 0x0);
	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x4cdc, BIT(0), 0x1);
		halrf_wreg(rf, 0x4ce8, BIT(2), 0x1);
		halrf_wrf(rf, RF_PATH_A, 0x8f, BIT(13), 0x1);
	} else {
		halrf_wreg(rf, 0x4ddc, BIT(0), 0x1);
		halrf_wreg(rf, 0x4de8, BIT(2), 0x1);
		halrf_wrf(rf, RF_PATH_B, 0x8f, BIT(13), 0x1);
	}
	halrf_check_addc_8852a(rf, path);
	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x4cdc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4ce8, BIT(2), 0x0);
		halrf_wrf(rf, RF_PATH_A, 0x8f, BIT(13), 0x0);
	} else {
		halrf_wreg(rf, 0x4ddc, BIT(0), 0x0);
		halrf_wreg(rf, 0x4de8, BIT(2), 0x0);
		halrf_wrf(rf, RF_PATH_B, 0x8f, BIT(13), 0x0);
	}
}

void halrf_dack_init_8852a(struct rf_info *rf)
{
	halrf_wreg(rf, 0x4C00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4C04, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x4C08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x4C10, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x4C14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C30, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C50, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4C54, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x4C58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C5c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x4C60, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x4C64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4d04, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x4d08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x4d10, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x4d14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d30, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d50, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4d54, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x4d58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d5c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x4d60, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x4d64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4d74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x4C00, MASKDWORD, 0x0030EB48);
	halrf_wreg(rf, 0x4C00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x4C50, MASKDWORD, 0x0030EB48);
	halrf_wreg(rf, 0x4C50, MASKDWORD, 0x0030EB49);
}

void halrf_dack_recover_8852a(struct rf_info *rf,
			      u8 offset,
			      enum rf_path path,
			      u32 val,
			      bool reload)
{
#if 1
	struct halrf_dack_info *dack = &rf->dack;
	u32	i = 0;
	u32	array_len = sizeof(array_dack_init_8852a) / sizeof(u32);
	u32	*array = (u32 *)array_dack_init_8852a;
	u32	v1 = 0, v2 = 0;
	u32	tt[1];
	u32	temp = 0, temp1 = 0, temp2 = 0;

	tt[0]= 0x1;
	if (path == RF_PATH_A) {

	}
//	RF_DBG(rf, DBG_RF_DACK, "[DACK]offset = 0x%x, val = 0x%x\n", offset, val);
	while ((i + 1) < array_len) {
		v1 = array[i];
		v2 = array[i + 1];

		if (offset == v1) {
			halrf_wreg(rf, 0x4c00 | offset, MASKDWORD, v2);
			
			if ((!dack->dack_done) && ((offset == 0x10) || (offset == 0x60)))
				halrf_wreg(rf, 0x4c00 | offset, MASKDWORD, 0x22000001);

			if (((offset == 0x0) && (val != 0x1) && (val != 0x11))  || (offset == 0x50)) {
//			if ((offset == 0x0)  || (offset == 0x50)) {
//			if (((offset == 0x0) && ((!(val & BIT(0))) || (val & BIT(1))))  ||
//				((offset == 0x50) && (val & BIT(1)))) {
//				RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK reset\n");
				halrf_wreg(rf, 0x4c00, MASKDWORD, 0x0030EB40);
				halrf_wreg(rf, 0x4c00, MASKDWORD, 0x0030EB49);
				halrf_wreg(rf, 0x4c50, MASKDWORD, 0x0030EB49);



			}
			break;
		}
		i += 2;
	}

	if (!dack->dack_done)
		return;

//	if(reload) {
		if (((offset == 0x0) && (val != 0x1) && (val != 0x11))  || (offset == 0x50)) {
//		if ((offset == 0x0)  || (offset == 0x50)) {
//		if (((offset == 0x0) && ((!(val & BIT(0))) || (val & BIT(1)) || (val & BIT(3))))  ||
//			((offset == 0x50) && ((val & BIT(1)) || (val & BIT(3))))) {
//				RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK reload\n");
				halrf_dack_reload_8852a(rf);

				temp = halrf_rreg(rf, 0x7c18, MASKDWORD);
				temp1 = halrf_rreg(rf, 0x7d18, MASKDWORD);
				temp2 = halrf_rreg(rf, 0x0908, MASKDWORD);
				/*TSSI off*/
				halrf_wreg(rf,  0x7cdc, BIT(30), 0x1);
				halrf_wreg(rf,  0x7c18, BIT(30), 0x1);
				halrf_wreg(rf,  0x7ddc, BIT(30), 0x1);
				halrf_wreg(rf,  0x7d18, BIT(30), 0x1);
				halrf_wreg(rf,  0x0908, BIT(20), 0x1);
				halrf_wreg(rf,  0x0908, 0x3, 0x0);
				halrf_wreg(rf,  0x0908, 0x3, 0x3);
				halrf_wreg(rf,  0x0908, MASKDWORD, temp2);
				/*TSSI on*/
				halrf_wreg(rf,  0x7c18, MASKDWORD, temp);
				halrf_wreg(rf,  0x7cdc, BIT(30), 0x0);
				halrf_wreg(rf,  0x7d18, MASKDWORD, temp1);
				halrf_wreg(rf,  0x7ddc, BIT(30), 0x0);
//				halrf_wreg(rf,  0x0804, 0x1ff, 0x0);
//				halrf_wreg(rf,  0x0804, 0x1ff, 0x1ff);
		}
//	}
#endif
}

void halrf_dack_8852a(struct rf_info *rf)
{
	u32 c = 0;
//	u32 temp;

#if 1
//	temp = halrf_rreg(rf, 0x4c0c, MASKDWORD);
//	halrf_wreg(rf, 0x4ce0, BIT(19), 0x1);
//	halrf_wreg(rf, 0x4de0, BIT(19), 0x0);


	/*S0*/
	halrf_wreg(rf, 0x4ca0, BIT(15), 0x1);
	halrf_wreg(rf, 0x4ca0, 0x7000, 0x3);

	halrf_wreg(rf, 0x4da0, BIT(15), 0x1);
	halrf_wreg(rf, 0x4da0, 0x7000, 0x3);
	
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x1);

	halrf_wreg(rf, 0x4db8, BIT(30), 0x1);
	
	halrf_wreg(rf, 0x0b0c, BIT(28), 0x1);
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x0b2c, 0x80000000, 0x0);

	halrf_wreg(rf, 0x4ce0, BIT(16), 0x1);
	halrf_wreg(rf, 0x4ce4, BIT(27) | BIT(26), 0x3);

	halrf_wreg(rf, 0x4de0, BIT(16), 0x1);
	halrf_wreg(rf, 0x4de4, BIT(27) | BIT(26), 0x3);

	halrf_wreg(rf, 0x4c00, 0x03ff0000, 0x30);
	halrf_wreg(rf, 0x4c50, 0x03ff0000, 0x30);
	halrf_wreg(rf, 0x4c00, BIT(27) | BIT(26), 0x0);
	halrf_wreg(rf, 0x4c50, BIT(27) | BIT(26), 0x0);
//	halrf_wreg(rf, 0x4c0c, BIT(2), 0x0);
//	halrf_wreg(rf, 0x4c5c, BIT(2), 0x0);
	halrf_wreg(rf, 0x4c0c, BIT(3), 0x0);
	halrf_wreg(rf, 0x4c5c, BIT(3), 0x0);
	/*bypass DADCK*/
//	halrf_wreg(rf, 0x4c10, BIT(2), 0x1);		//20200408
//	halrf_wreg(rf, 0x4c60, BIT(2), 0x1);		//20200408
	/*avg 8192 times*/
	halrf_wreg(rf, 0x4c00, 0x1c00, 0x6);
	halrf_wreg(rf, 0x4c50, 0x1c00, 0x6);
	/*auto mode*/
	halrf_wreg(rf, 0x4c0c, BIT(0), 0x1);
	halrf_wreg(rf, 0x4c5c, BIT(0), 0x1);
	halrf_delay_ms(rf, 4);
	c = 0x0;
	while((halrf_rreg(rf, 0x4c28, BIT(15)) == 0) && (halrf_rreg(rf, 0x4c78, BIT(15)) == 0)) {
		c++;
		halrf_delay_us(rf, 10);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 DACK timeout -1\n");
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);	
	halrf_wreg(rf, 0x4ce4, BIT(27) | BIT(26), 0x0);
	halrf_wreg(rf, 0x4de4, BIT(27) | BIT(26), 0x0);

//	halrf_wreg(rf, 0x4c0c, BIT(2), 0x1);
//	halrf_wreg(rf, 0x4c5c, BIT(2), 0x1);
	halrf_wreg(rf, 0x4c0c, BIT(3), 0x1);
	halrf_wreg(rf, 0x4c5c, BIT(3), 0x1);
	halrf_delay_us(rf, 10);
	c = 0x0;
	while(halrf_rreg(rf, 0x4c48, BIT(17)) == 0 && halrf_rreg(rf, 0x4c98, BIT(17)) == 0) {
		c++;
		halrf_delay_us(rf, 10);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 DACK timeout -2\n");
			break;
		}
	}
	/*auto mode off*/
	halrf_wreg(rf, 0x4c0c, MASKDWORD, 0x0);
	halrf_wreg(rf, 0x4c5c, MASKDWORD, 0x0);
	halrf_wreg(rf, 0x4c04, MASKDWORD, 0x06000000);
	halrf_wreg(rf, 0x4c54, MASKDWORD, 0x06000000);

	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);
	halrf_wreg(rf, 0x4ce0, BIT(16), 0x0);
	halrf_wreg(rf, 0x4ca0, BIT(15), 0x0);

	halrf_wreg(rf, 0x4de0, BIT(16), 0x0);
	halrf_wreg(rf, 0x4da0, BIT(15), 0x0);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S0 DADCK\n");
	halrf_check_dadc_8852a(rf, RF_PATH_A);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S1 DADCK\n");
	halrf_check_dadc_8852a(rf, RF_PATH_B);

	/*backup here*/
	halrf_dack_backup_s0_8852a(rf);
	halrf_dack_backup_s1_8852a(rf);
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x0);
	halrf_wreg(rf, 0x4db8, BIT(30), 0x0);
#endif
}

void halrf_dack_dump_8852a(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 i;
	u8 t;

	RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 ADC_DCK ic = 0x%x, qc = 0x%x\n",
	                    dack->addck_d[0][0], dack->addck_d[0][1] );
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 ADC_DCK ic = 0x%x, qc = 0x%x\n",
	                    dack->addck_d[1][0], dack->addck_d[1][1] );
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 DAC_DCK ic = 0x%x, qc = 0x%x\n",
	       dack->dadck_d[0][0], dack->dadck_d[0][1] );
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 DAC_DCK ic = 0x%x, qc = 0x%x\n",
	       dack->dadck_d[1][0], dack->dadck_d[1][1] );

	RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 biask ic = 0x%x, qc = 0x%x\n",
	       dack->biask_d[0][0], dack->biask_d[0][1] );
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 biask ic = 0x%x, qc = 0x%x\n",
	       dack->biask_d[1][0], dack->biask_d[1][1] );

	RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 MSBK ic:\n");
	for (i = 0; i < 0x10; i++) {
		t = dack->msbk_d[0][0][i];
		RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x\n", t);
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 MSBK qc:\n");
	for (i = 0; i < 0x10; i++) {
		t = dack->msbk_d[0][1][i];
		RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x\n", t);
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 MSBK ic:\n");
	for (i = 0; i < 0x10; i++) {
		t = dack->msbk_d[1][0][i];
		RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x\n", t);
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 MSBK qc:\n");
	for (i = 0; i < 0x10; i++) {
		t = dack->msbk_d[1][1][i];
		RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x\n", t);
	}
}

void halrf_dac_cal_8852a(struct rf_info *rf, bool force)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 rf0_0, rf1_0;
	u8 phy_map;

	phy_map = (BIT(HW_PHY_0) << 4) | RF_AB;
#if 0
	if (dack->dack_en) {
		if (!force) {
			halrf_dack_reload_8852a(rf);
			RF_DBG(rf, DBG_RF_DACK, "[DACK]reload dack value\n");
			return;
		}
	} else {
		dack->dack_en = true;
	}
#endif
	dack->dack_done = false;
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK a\n");
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK start!!!\n");
	rf0_0 = halrf_rrf(rf,RF_PATH_A, 0x0, MASKRF);
	rf1_0 = halrf_rrf(rf,RF_PATH_B, 0x0, MASKRF);

	halrf_wrf(rf, RF_PATH_A, 0x5, BIT(0), 0x0);
	halrf_wrf(rf, RF_PATH_B, 0x5, BIT(0), 0x0);
	halrf_wrf(rf, RF_PATH_A, 0x0, 0xfffff, 0x30001);
	halrf_wrf(rf, RF_PATH_B, 0x0, 0xfffff, 0x30001);
	halrf_dack_init_8852a(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_START);
	halrf_addck_8852a(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_STOP);
	halrf_addck_backup_8852a(rf);
	halrf_addck_reload_8852a(rf);
	halrf_wrf(rf, RF_PATH_A, 0x0, 0xfffff, 0x40001);
	halrf_wrf(rf, RF_PATH_B, 0x0, 0xfffff, 0x40001);
	halrf_dack_init_8852a(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_START);
	halrf_dack_8852a(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_STOP);
	halrf_dack_dump_8852a(rf);
	dack->dack_done = true;

	halrf_wrf(rf, RF_PATH_A, 0x0, MASKRF, rf0_0);
	halrf_wrf(rf, RF_PATH_B, 0x0, MASKRF, rf1_0 );
	halrf_wrf(rf, RF_PATH_A, 0x5, BIT(0), 0x1);
	halrf_wrf(rf, RF_PATH_B, 0x5, BIT(0), 0x1);
	halrf_wreg(rf, 0x4cb8, BIT(30), 0x0);
	halrf_wreg(rf, 0x4db8, BIT(30), 0x0);

	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK finish!!!\n");
}

void halrf_afe_init_8852a_b(struct rf_info *rf)
{
//	halrf_wreg(rf, 0x12cc, MASKDWORD, 0x00000CC1);
//	halrf_wreg(rf, 0x12D0, MASKDWORD, 0x00000000);
//	halrf_wreg(rf, 0x12d4, MASKDWORD, 0x00000000);
//	halrf_wreg(rf, 0x12d8, MASKDWORD, 0x00000040);
//	halrf_wreg(rf, 0x12dc, MASKDWORD, 0x4486888C);
//	halrf_wreg(rf, 0x12e0, MASKDWORD, 0xC43A10E0);
//	halrf_wreg(rf, 0x12e4, MASKDWORD, 0x30D52268);
//	halrf_wreg(rf, 0x12e8, MASKDWORD, 0x02024128);
//	halrf_wreg(rf, 0x12ec, MASKDWORD, 0x888ca72a);
//	halrf_wreg(rf, 0x12ec, MASKDWORD, 0x888ca72b);
	halrf_wreg(rf, 0x12ec, BIT(15), 0x0);
	halrf_wreg(rf, 0x12ec, BIT(15), 0x1);
	halrf_wreg(rf, 0x5e00, BIT(0), 0x0);
	halrf_wreg(rf, 0x5e00, BIT(0), 0x1);
//	halrf_wreg(rf, 0x32cc, MASKDWORD, 0x00000CC1);
//	halrf_wreg(rf, 0x32D0, MASKDWORD, 0x00000000);
//	halrf_wreg(rf, 0x32d4, MASKDWORD, 0x00000000);
//	halrf_wreg(rf, 0x32d8, MASKDWORD, 0x00000040);
//	halrf_wreg(rf, 0x32dc, MASKDWORD, 0x4486888C);
//	halrf_wreg(rf, 0x32e0, MASKDWORD, 0xC43A10E0);
//	halrf_wreg(rf, 0x32e4, MASKDWORD, 0x30D52268);
//	halrf_wreg(rf, 0x32e8, MASKDWORD, 0x02024128);
//	halrf_wreg(rf, 0x32ec, MASKDWORD, 0x888ca72a);
//	halrf_wreg(rf, 0x32ec, MASKDWORD, 0x888ca72b);
//	halrf_wreg(rf, 0x32ec, MASKDWORD, 0x888ca72a);
//	halrf_wreg(rf, 0x32ec, MASKDWORD, 0x888ca72b);
	halrf_wreg(rf, 0x32ec, BIT(15), 0x0);
	halrf_wreg(rf, 0x32ec, BIT(15), 0x1);
	halrf_wreg(rf, 0x7e00, BIT(0), 0x0);
	halrf_wreg(rf, 0x7e00, BIT(0), 0x1);
}
void halrf_dack_init_8852a_b(struct rf_info *rf)
{
	halrf_wreg(rf, 0x5e00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x5e04, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x5e08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x5e10, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x5e14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e28, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e2c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e30, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x5e34, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e38, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e3c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e40, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x5e44, MASKDWORD, 0x005E0000);
	halrf_wreg(rf, 0x5e48, MASKDWORD, 0x00008000);
	halrf_wreg(rf, 0x5e4c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e50, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x5e54, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x5e58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e5c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x5e60, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x5e64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e78, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e7c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e80, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x5e84, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e88, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e8c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e90, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e94, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x5e98, MASKDWORD, 0x00008000);
	halrf_wreg(rf, 0x5e9c, MASKDWORD, 0x00000000);
	
	halrf_wreg(rf, 0x7e00, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x7e04, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x7e08, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e0c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x7e10, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x7e14, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e18, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e1c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e20, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e24, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e28, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e2c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e30, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x7e34, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e38, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e3c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e40, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x7e44, MASKDWORD, 0x007E0000);
	halrf_wreg(rf, 0x7e48, MASKDWORD, 0x00008000);
	halrf_wreg(rf, 0x7e4c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e50, MASKDWORD, 0x0030EB49);
	halrf_wreg(rf, 0x7e54, MASKDWORD, 0x04000000);
	halrf_wreg(rf, 0x7e58, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e5c, MASKDWORD, 0x0000001C);
	halrf_wreg(rf, 0x7e60, MASKDWORD, 0x22000001);
	halrf_wreg(rf, 0x7e64, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e68, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e6c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e70, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e74, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e78, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e7c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e80, MASKDWORD, 0x00200000);
	halrf_wreg(rf, 0x7e84, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e88, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e8c, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e90, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e94, MASKDWORD, 0x00000000);
	halrf_wreg(rf, 0x7e98, MASKDWORD, 0x00008000);
	halrf_wreg(rf, 0x7e9c, MASKDWORD, 0x00000000);
}

void halrf_addck_backup_8852a_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;

	halrf_wreg(rf, 0x12d8, 0x300, 0x0);
	dack->addck_d[0][0] = (u16)halrf_rreg(rf, 0x1e00,0xffc00) ;
	dack->addck_d[0][1] = (u16)halrf_rreg(rf, 0x1e00,0x003ff) ;

	halrf_wreg(rf, 0x32d8, 0x300, 0x0);
	dack->addck_d[1][0] = (u16)halrf_rreg(rf, 0x3e00,0xffc00) ;
	dack->addck_d[1][1] = (u16)halrf_rreg(rf, 0x3e00,0x003ff) ;
}

void halrf_addck_reload_8852a_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	/*S0*/
	halrf_wreg(rf, 0x12d4, 0x3ff0000, dack->addck_d[0][0]);
	halrf_wreg(rf, 0x12d8, 0xf, (dack->addck_d[0][1] >> 6));
	halrf_wreg(rf, 0x12d4, 0xfc000000, (dack->addck_d[0][1] & 0x3f));
	/*manual*/
	halrf_wreg(rf, 0x12d8, 0x30, 0x3);
	/*S1*/
	halrf_wreg(rf, 0x32d4, 0x3ff0000, dack->addck_d[1][0]);
	halrf_wreg(rf, 0x32d8, 0xf, (dack->addck_d[1][1] >> 6));
	halrf_wreg(rf, 0x32d4, 0xfc000000, (dack->addck_d[1][1] & 0x3f));
	/*manual*/
	halrf_wreg(rf, 0x32d8, 0x30, 0x3);
}

void halrf_dack_backup_s0_8852a_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 i;

	halrf_wreg(rf, 0x5e00, BIT(3), 0x1);
	halrf_wreg(rf, 0x5e50, BIT(3), 0x1);
	halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
	/*MSBK*/
	for (i = 0; i < 0x10; i++) {
		/*S0*/
		halrf_wreg(rf, 0x5e00, 0xf0000000, i);
		dack->msbk_d[0][0][i] = (u8)halrf_rreg(rf, 0x5e44, 0xff00);
		halrf_wreg(rf, 0x5e50, 0xf0000000, i);
		dack->msbk_d[0][1][i] = (u8)halrf_rreg(rf, 0x5e94, 0xff00);
	}
	/*biasK*/
	dack->biask_d[0][0] = (u16)halrf_rreg(rf, 0x5e30, 0x3ff000);
	dack->biask_d[0][1] = (u16)halrf_rreg(rf, 0x5e80, 0x3ff000);
	/*DADCK*/
	dack->dadck_d[0][0] = (u8)halrf_rreg(rf, 0x5e48, 0xff00) - 8;
	dack->dadck_d[0][1] = (u8)halrf_rreg(rf, 0x5e98, 0xff00) - 8;
}

void halrf_dack_backup_s1_8852a_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 i;
	halrf_wreg(rf, 0x7e00, BIT(3), 0x1);
	halrf_wreg(rf, 0x7e50, BIT(3), 0x1);
	halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
	/*MSBK*/
	for (i = 0; i < 0x10; i++) {
		/*S1*/
		halrf_wreg(rf, 0x7e00, 0xf0000000, i);
		dack->msbk_d[1][0][i] = (u8)halrf_rreg(rf, 0x7e44, 0xff00);
		halrf_wreg(rf, 0x7e50, 0xf0000000, i);
		dack->msbk_d[1][1][i] = (u8)halrf_rreg(rf, 0x7e94, 0xff00);
	}
	/*biasK*/
	dack->biask_d[1][0] = (u16)halrf_rreg(rf, 0x7e30, 0x3ff000);
	dack->biask_d[1][1] = (u16)halrf_rreg(rf, 0x7e80, 0x3ff000);
	/*DADCK*/
	dack->dadck_d[1][0] = (u8)halrf_rreg(rf, 0x7e48, 0xff00) - 8;
	dack->dadck_d[1][1] = (u8)halrf_rreg(rf, 0x7e98, 0xff00) - 8;
}

void halrf_dack_reload_by_path_8852a_b(struct rf_info *rf, enum rf_path path, u8 index)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 temp = 0 , temp_offset, temp_reg;
	u8 i;
	u32 idx_offset, path_offset;

	if (index ==0)
		idx_offset = 0;
	else
		idx_offset = 0x50;

	if (path == RF_PATH_A)
		path_offset = 0;
	else
		path_offset = 0x2000;

	temp_offset = idx_offset + path_offset;
	/*msbk_d: 15/14/13/12*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+12] << (i * 8);
	}
	temp_reg = 0x5e14 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 11/10/9/8*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+8] << (i * 8);
	}
	temp_reg = 0x5e18 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 7/6/5/4*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i+4] << (i * 8);
	}
	temp_reg = 0x5e1c + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf, temp_reg, MASKDWORD));
	/*msbk_d: 3/2/1/0*/
	temp = 0x0;
	for (i = 0; i < 4; i++) {
		temp |= dack->msbk_d[path][index][i] << (i * 8);
	}
	temp_reg = 0x5e20 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x=0x%x\n", temp_reg, halrf_rreg(rf,temp_reg, MASKDWORD));
	/*dadak_d/biask_d*/
	temp = 0x0;
	temp =   (dack->biask_d[path][index] << 22) |
		 (dack->dadck_d[path][index] << 14);
	temp_reg = 0x5e24 + temp_offset;
	halrf_w32(rf, temp_reg, temp);
}

void halrf_dack_reload_8852a_b(struct rf_info *rf, enum rf_path path)
{
	u8 i;
	u32 pid;

	if (path == RF_PATH_A)
		pid = 0x5e00;
	else
		pid = 0x7e00;

	for (i = 0; i < 2; i++)
		halrf_dack_reload_by_path_8852a_b(rf, path, i);
	halrf_wreg(rf, pid +0x00, BIT(3), 0x1);
	halrf_wreg(rf, pid + 0x50, BIT(3), 0x1);
	halrf_wreg(rf, pid + 0x10, 0x80000000, 0x1);
	halrf_wreg(rf, pid + 0x60, 0x80000000, 0x1);
	halrf_wreg(rf, pid + 0x00, BIT(3), 0x0);
	halrf_wreg(rf, pid + 0x50, BIT(3), 0x0);
}

void halrf_check_addc_8852a_b(struct rf_info *rf, enum rf_path path)
{
	u32 temp, dc_re, dc_im;
	u32 i, m, p, t;
	u32 re[t_avg], im[t_avg];
#if 1
	halrf_wreg(rf, 0x20f4, BIT(24), 0x0);
	halrf_wreg(rf, 0x20f8, 0x80000000, 0x1);
	halrf_wreg(rf, 0x20f0, 0xff0000, 0x1);
	halrf_wreg(rf, 0x20f0, 0xf00, 0x2);
	halrf_wreg(rf, 0x20f0, 0xf, 0x0);
	if (path  == RF_PATH_A)
		halrf_wreg(rf, 0x20f0, 0xc0, 0x2);
	else
		halrf_wreg(rf, 0x20f0, 0xc0, 0x3);
	for (i = 0; i < t_avg; i++) {
		temp = halrf_rreg(rf, 0x1730, 0xffffffff);
		re[i] = (temp & 0xfff000) >> 12;
		im[i] = temp & 0xfff;
//		RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,re[i]= 0x%x,im[i] =0x%x\n",
//		path, re[i], im[i]);
	}
#else
	for (i = 0; i < t_avg; i++) {
		if (path  == RF_PATH_A)
			temp = halrf_rreg(rf, 0x1c8c, MASKDWORD);
		else
			temp = halrf_rreg(rf, 0x3c8c, MASKDWORD);

		re[i] = (temp & 0xfff000) >> 12;
		im[i] = temp & 0xfff;
//		RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,re[i]= 0x%x,im[i] =0x%x\n",
//		path, re[i], im[i]);
	}

#endif
	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (re[i] > 0x800)
			m = (0x1000 - re[i]) + m;
		else
			p = re[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_re = t;

	m = 0;
	p = 0;
	for (i = 0; i < t_avg; i++) {
		if (im[i] > 0x800)
			m = (0x1000 - im[i]) + m;
		else
			p = im[i] + p;
	}

	if (p > m) {
		t = p - m;
		t = t / t_avg;
	} else {
		t = m - p;
		t = t / t_avg;
		if (t != 0x0)
			t = 0x1000 - t;
	}
	dc_im = t;

	RF_DBG(rf, DBG_RF_DACK, "[DACK]S%d,dc_re = 0x%x,dc_im =0x%x\n",
		path, dc_re, dc_im);
}

void halrf_addck_8852a_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 c;
	/*S0*/
#if 1
	/*manual off*/
	halrf_wreg(rf, 0x12d8, 0x30, 0x0);
	halrf_wreg(rf, 0x32d8, 0x30, 0x0);
	/*1.ADC & clk enable */
	halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x032c, BIT(30), 0x0);
	/*4.Reset calibration*/
	halrf_wreg(rf, 0x032c, BIT(22), 0x0);	
	halrf_wreg(rf, 0x032c, BIT(22), 0x1);

	halrf_wreg(rf, 0x030c, 0x0f000000, 0xf);
	/*2.ADC input not from RXBB & ADC input short*/
	halrf_wreg(rf, 0x032c, BIT(16), 0x0);	
	halrf_wreg(rf, 0x12dc, BIT(1), 0x1);
	/*3.release ADC reset*/
	halrf_wreg(rf, 0x030c, 0x0f000000, 0x3);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]before S0 ADDCK\n");
	halrf_check_addc_8852a_b(rf, RF_PATH_A);
	/*average 128 times*/
	halrf_wreg(rf, 0x12d8, BIT(7) | BIT(6), 0x3);
	/*5.trigger dc offset calibration*/
	halrf_wreg(rf, 0x12d8, BIT(11), 0x1);
	halrf_wreg(rf, 0x12d8, BIT(11), 0x0);
	halrf_delay_us(rf, 1);
	/*check if cal done*/
	halrf_wreg(rf, 0x12d8, 0x300, 0x1);
	c = 0;
	while (halrf_rreg(rf, 0x1e00, BIT(0)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 ADDCK timeout\n");
			dack->dack_fail = true;
			dack->addck_timeout[0] = true;
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]ADDCK c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S0 ADDCK\n");
	halrf_check_addc_8852a_b(rf, RF_PATH_A);
	/*restore*/
	halrf_wreg(rf, 0x12dc, BIT(1), 0x0);
	halrf_wreg(rf, 0x032c, BIT(16), 0x1);
	halrf_wreg(rf, 0x030c, 0x0f000000, 0xc);
	halrf_wreg(rf, 0x032c, BIT(30), 0x1);
	halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
#endif

	/*S1*/
#if 1
	/*1.ADC & clk enable */
	halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x032c, BIT(30), 0x0);
	/*4.Reset calibration*/
	halrf_wreg(rf, 0x032c, BIT(22), 0x0);	
	halrf_wreg(rf, 0x032c, BIT(22), 0x1);

	halrf_wreg(rf, 0x030c, 0x0f000000, 0xf);
	/*2.ADC input not from RXBB & ADC input short*/
	halrf_wreg(rf, 0x032c, BIT(16), 0x0);	
	halrf_wreg(rf, 0x32dc, BIT(1), 0x1);
	/*3.release ADC reset*/
	halrf_wreg(rf, 0x030c, 0x0f000000, 0x3);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]before S1 ADDCK\n");
	halrf_check_addc_8852a_b(rf, RF_PATH_B);
	/*average 128 times*/
	halrf_wreg(rf, 0x32d8, BIT(7) | BIT(6), 0x3);	
	/*5.trigger dc offset calibration*/
	halrf_wreg(rf, 0x32d8, BIT(11), 0x1);
	halrf_wreg(rf, 0x32d8, BIT(11), 0x0);
	halrf_delay_us(rf, 1);
	/*check if cal done*/
	halrf_wreg(rf, 0x32d8, 0x300, 0x1);
	c = 0;
	while (halrf_rreg(rf, 0x3e00, BIT(0)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 ADDCK timeout\n");
			dack->dack_fail = true;
			dack->addck_timeout[1] = true;
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]ADDCK c = %d\n", c);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S1 ADDCK\n");
	halrf_check_addc_8852a_b(rf, RF_PATH_B);
	/*restore*/
	halrf_wreg(rf, 0x32dc, BIT(1), 0x0);
	halrf_wreg(rf, 0x032c, BIT(16), 0x1);
	halrf_wreg(rf, 0x030c, 0x0f000000, 0xc);
	halrf_wreg(rf, 0x032c, BIT(30), 0x1);
	halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
#endif
}

void halrf_check_dadc_8852a_b(struct rf_info *rf, enum rf_path path)
{
	halrf_wreg(rf, 0x032c, BIT(30), 0x0);
	halrf_wreg(rf, 0x030c, 0x0f000000, 0xf);
	halrf_wreg(rf, 0x030c, 0x0f000000, 0x3);
	halrf_wreg(rf, 0x032c, BIT(16), 0x0);
	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x12dc, BIT(0), 0x1);
		halrf_wreg(rf, 0x12e8, BIT(2), 0x1);
		halrf_wrf(rf, RF_PATH_A, 0x8f, BIT(13), 0x1);
	} else {
		halrf_wreg(rf, 0x32dc, BIT(0), 0x1);
		halrf_wreg(rf, 0x32e8, BIT(2), 0x1);
		halrf_wrf(rf, RF_PATH_B, 0x8f, BIT(13), 0x1);
	}
	halrf_check_addc_8852a_b(rf, path);
	if (path == RF_PATH_A) {
		halrf_wreg(rf, 0x12dc, BIT(0), 0x0);
		halrf_wreg(rf, 0x12e8, BIT(2), 0x0);
		halrf_wrf(rf, RF_PATH_A, 0x8f, BIT(13), 0x0);
	} else {
		halrf_wreg(rf, 0x32dc, BIT(0), 0x0);
		halrf_wreg(rf, 0x32e8, BIT(2), 0x0);
		halrf_wrf(rf, RF_PATH_B, 0x8f, BIT(13), 0x0);
	}
	halrf_wreg(rf, 0x032c, BIT(16), 0x1);
}

void halrf_dump(struct rf_info *rf, u8 path)
{
	u32 i;
	u32 ofst;

	if (path == 0)
		ofst = 0x0;
	else
		ofst = 0x2000;
	RF_DBG(rf, DBG_RF_DACK, "[DACK]***********************************\n");
	for (i = 0; i < 40; i ++)
		RF_DBG(rf, DBG_RF_DACK, "[DACK]0x%x%x=0x%x\n",(path ==0 ? 0x5e : 0x7e) , i*4, halrf_rreg(rf, 0x5e00 + (i * 4) + ofst, MASKDWORD));
	RF_DBG(rf, DBG_RF_DACK, "[DACK]***********************************\n");
}

void halrf_dack_8852a_s0_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 c = 0;

	/*S0*/
	halrf_wreg(rf, 0x5e00, BIT(3), 0x1);
	halrf_wreg(rf, 0x5e10, BIT(31), 0x0);
	halrf_wreg(rf, 0x5e50, BIT(3), 0x1);
	halrf_wreg(rf, 0x5e60, BIT(31), 0x0);
	/*step 1*/
	halrf_wreg(rf, 0x12a0, BIT(15), 0x1);
	halrf_wreg(rf, 0x12a0, 0x7000, 0x3);
	/*step 2*/
	halrf_wreg(rf, 0x12b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x030c, BIT(28), 0x1);
	halrf_wreg(rf, 0x032c, 0x80000000, 0x0);
	/*step 3*/
	halrf_wreg(rf, 0x12e0, BIT(16), 0x1);
	/*step 4*/
	halrf_wreg(rf, 0x12e4, BIT(27) | BIT(26), 0x3);
	/*step 5/6*/
	halrf_wreg(rf, 0x5e00, 0x03ff0000, 0x30);
	halrf_wreg(rf, 0x5e50, 0x03ff0000, 0x30);
	/*step 7*/
	halrf_wreg(rf, 0x5e00, BIT(27) | BIT(26), 0x0);
	halrf_wreg(rf, 0x5e50, BIT(27) | BIT(26), 0x0);
	/*step 8*/
	halrf_wreg(rf, 0x5e0c, BIT(3), 0x0);
	halrf_wreg(rf, 0x5e5c, BIT(3), 0x0);
	/*avg 8192 times*/
//	halrf_wreg(rf, 0x4c00, 0x1c00, 0x6);
//	halrf_wreg(rf, 0x4c50, 0x1c00, 0x6);
	/*step 9*/ /*auto mode*/
	halrf_wreg(rf, 0x5e0c, BIT(0), 0x1);
	halrf_wreg(rf, 0x5e5c, BIT(0), 0x1);
	halrf_delay_10us(rf, 10);
	/*step 10*/
	c = 0x0;
	while((halrf_rreg(rf, 0x5e28, BIT(15)) == 0) || (halrf_rreg(rf, 0x5e78, BIT(15)) == 0)) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 MSBK timeout\n");
			dack->dack_fail = true;
			dack->msbk_timeout[0] = true;
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);
	/*step 11*/
	halrf_wreg(rf, 0x12e4, BIT(27) | BIT(26), 0x0);
	/*step 12*/
	halrf_wreg(rf, 0x5e0c, BIT(3), 0x1);
	halrf_wreg(rf, 0x5e5c, BIT(3), 0x1);
	halrf_delay_10us(rf, 10);
	/*step 13*/
	c = 0x0;
	while(halrf_rreg(rf, 0x5e48, BIT(17)) == 0 || halrf_rreg(rf, 0x5e98, BIT(17)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S0 DADACK timeout\n");
			dack->dack_fail = true;
			dack->dadck_timeout[0] = true;
			break;
		}
	}
	/*step 14*/ /*auto mode off*/
	halrf_wreg(rf, 0x5e0c, BIT(0), 0x0);
	halrf_wreg(rf, 0x5e5c, BIT(0), 0x0);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);
	/*step 15*/
	halrf_wreg(rf, 0x12e0, BIT(16), 0x0);
	/*step 16*/
	halrf_wreg(rf, 0x12a0, BIT(15), 0x0);
	halrf_wreg(rf, 0x12a0, 0x7000, 0x7);

	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S0 DADCK\n");
	halrf_check_dadc_8852a_b(rf, RF_PATH_A);

	/*backup here*/
	halrf_dack_backup_s0_8852a_b(rf);
	halrf_dack_reload_8852a_b(rf, RF_PATH_A);
	/*step 17*/
	halrf_wreg(rf, 0x12b8, BIT(30), 0x0);
}

void halrf_dack_8852a_s1_b(struct rf_info *rf)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 c = 0;
//	u32 temp;

	/*S1*/
	halrf_wreg(rf, 0x7e00, BIT(3), 0x1);
	halrf_wreg(rf, 0x7e10, BIT(31), 0x0);
	halrf_wreg(rf, 0x7e50, BIT(3), 0x1);
	halrf_wreg(rf, 0x7e60, BIT(31), 0x0);
	/*step 1*/
	halrf_wreg(rf, 0x32a0, BIT(15), 0x1);
	halrf_wreg(rf, 0x32a0, 0x7000, 0x3);
	/*step 2*/
	halrf_wreg(rf, 0x32b8, BIT(30), 0x1);
	halrf_wreg(rf, 0x030c, BIT(28), 0x1);
	halrf_wreg(rf, 0x032c, 0x80000000, 0x0);
	/*step 3*/
	halrf_wreg(rf, 0x32e0, BIT(16), 0x1);
	/*step 4*/
	halrf_wreg(rf, 0x32e4, BIT(27) | BIT(26), 0x3);
	/*step 5/6*/
	halrf_wreg(rf, 0x7e00, 0x03ff0000, 0x30);
	halrf_wreg(rf, 0x7e50, 0x03ff0000, 0x30);
	/*step 7*/
	halrf_wreg(rf, 0x7e00, BIT(27) | BIT(26), 0x0);
	halrf_wreg(rf, 0x7e50, BIT(27) | BIT(26), 0x0);
	/*step 8*/
	halrf_wreg(rf, 0x7e0c, BIT(3), 0x0);
	halrf_wreg(rf, 0x7e5c, BIT(3), 0x0);
	/*avg 8192 times*/
//	halrf_wreg(rf, 0x4c00, 0x1c00, 0x6);
//	halrf_wreg(rf, 0x4c50, 0x1c00, 0x6);
	/*step 9*/ /*auto mode*/
	halrf_wreg(rf, 0x7e0c, BIT(0), 0x1);
	halrf_wreg(rf, 0x7e5c, BIT(0), 0x1);
	halrf_delay_10us(rf, 10);
	/*step 10*/
	c = 0x0;
	while((halrf_rreg(rf, 0x7e28, BIT(15)) == 0) || (halrf_rreg(rf, 0x7e78, BIT(15)) == 0)) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 MSBK timeout\n");
			dack->dack_fail = true;
			dack->msbk_timeout[1] = true;
			break;
		}
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);
	/*step 11*/
	halrf_wreg(rf, 0x32e4, BIT(27) | BIT(26), 0x0);
	/*step 12*/
	halrf_wreg(rf, 0x7e0c, BIT(3), 0x1);
	halrf_wreg(rf, 0x7e5c, BIT(3), 0x1);
	halrf_delay_10us(rf, 10);
	/*step 13*/
	c = 0x0;
	while(halrf_rreg(rf, 0x7e48, BIT(17)) == 0 || halrf_rreg(rf, 0x7e98, BIT(17)) == 0) {
		c++;
		halrf_delay_us(rf, 1);
		if (c > 10000) {
			RF_DBG(rf, DBG_RF_DACK, "[DACK]S1 DADCK timeout\n");
			dack->dack_fail = true;
			dack->dadck_timeout[1] = true;
			break;
		}
	}
	/*step 14*/ /*auto mode off*/
	halrf_wreg(rf, 0x7e0c, BIT(0), 0x0);
	halrf_wreg(rf, 0x7e5c, BIT(0), 0x0);
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK c = %d\n", c);
	/*step 15*/
	halrf_wreg(rf, 0x32e0, BIT(16), 0x0);
	/*step 16*/
	halrf_wreg(rf, 0x32a0, BIT(15), 0x0);
	halrf_wreg(rf, 0x32a0, 0x7000, 0x7);

	RF_DBG(rf, DBG_RF_DACK, "[DACK]after S1 DADCK\n");
	halrf_check_dadc_8852a_b(rf, RF_PATH_B);
	/*backup here*/
	halrf_dack_backup_s1_8852a_b(rf);
	halrf_dack_reload_8852a_b(rf, RF_PATH_B);
	/*step 17*/
	halrf_wreg(rf, 0x32b8, BIT(30), 0x0);
}

void halrf_dack_8852a_b(struct rf_info *rf)
{
	halrf_dack_8852a_s0_b(rf);
	halrf_dack_8852a_s1_b(rf);
}


void halrf_dac_cal_t_8852a_b(struct rf_info *rf, bool force)
{
	struct halrf_dack_info *dack = &rf->dack;
	u32 rf0_0, rf1_0;
	u8 phy_map;

	phy_map = (BIT(HW_PHY_0) << 4) | RF_AB;
	dack->dack_fail = false;
#if 0
	if (dack->dack_en) {
		if (!force) {
			halrf_dack_reload_8852a(rf);
			RF_DBG(rf, DBG_RF_DACK, "[DACK]reload dack value\n");
			return;
		}
	} else {
		dack->dack_en = true;
	}
#endif
	dack->dack_done = false;
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK b\n");
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK start!!!\n");
	rf0_0 = halrf_rrf(rf,RF_PATH_A, 0x0, MASKRF);
	rf1_0 = halrf_rrf(rf,RF_PATH_B, 0x0, MASKRF);
//	halrf_dack_init_8852a_b(rf);
	halrf_afe_init_8852a_b(rf);
#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, false, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	halrf_wrf(rf, RF_PATH_A, 0x5, BIT(0), 0x0);
	halrf_wrf(rf, RF_PATH_B, 0x5, BIT(0), 0x0);
	halrf_wrf(rf, RF_PATH_A, 0x0, MASKRF, 0x30001);
	halrf_wrf(rf, RF_PATH_B, 0x0, MASKRF, 0x30001);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_START);
	halrf_addck_8852a_b(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_STOP);
	halrf_addck_backup_8852a_b(rf);
	halrf_addck_reload_8852a_b(rf);
	halrf_wrf(rf, RF_PATH_A, 0x0, MASKRF, 0x40001);
	halrf_wrf(rf, RF_PATH_B, 0x0, MASKRF, 0x40001);
	halrf_wrf(rf, RF_PATH_A, 0x1, MASKRF, 0x0);
	halrf_wrf(rf, RF_PATH_B, 0x1, MASKRF, 0x0);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_START);
	halrf_dack_8852a_b(rf);
	halrf_btc_rfk_ntfy(rf, phy_map, RF_BTC_DACK, RFK_ONESHOT_STOP);
	halrf_dack_dump_8852a(rf);
	dack->dack_done = true;
	halrf_wrf(rf, RF_PATH_A, 0x0, MASKRF, rf0_0);
	halrf_wrf(rf, RF_PATH_B, 0x0, MASKRF, rf1_0);
	halrf_wrf(rf, RF_PATH_A, 0x5, BIT(0), 0x1);
	halrf_wrf(rf, RF_PATH_B, 0x5, BIT(0), 0x1);
#ifdef CF_PHL_BB_CTRL_RX_CCA
	halrf_bb_ctrl_rx_cca(rf, true, HW_PHY_0);
#else
	RF_WARNING("[RF]%s !! no CF_PHL_BB_CTRL_RX_CCA\n", __FUNCTION__);
#endif
	dack->dack_cnt++;
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK finish!!!\n");
}



void halrf_dac_cal_8852a_b(struct rf_info *rf, bool force)
{
	struct halrf_dack_info *dack = &rf->dack;
	u8 c = 0;

	while (c < 3) {
		c++;
		halrf_dac_cal_t_8852a_b(rf, force);
		if (!dack->dack_fail)
			break;
	}
	RF_DBG(rf, DBG_RF_DACK, "[DACK]DACK count = %d\n", c);
}


bool halrf_dack_reg_check_fail_8852a(struct rf_info *rf)
{
#if 1
	struct halrf_dack_info *dack = &rf->dack;
	u16 addck_d[2][2]; 
	u8 msbk_d[2][2][16];
	u8 dadck_d[2][2];
	u16 biask_d[2][2];	
	u32 i, id, j, p;
	bool fail = false;

	addck_d[0][0] = (u16)halrf_rreg(rf, 0x12d4, 0x3ff0000);
	addck_d[0][1] = (u16)((halrf_rreg(rf, 0x12d8, 0xf) << 6) | halrf_rreg(rf, 0x12d4, 0xfc000000));
	addck_d[1][0] =(u16) halrf_rreg(rf, 0x32d4, 0x3ff0000);
	addck_d[1][1] = (u16)((halrf_rreg(rf, 0x32d8, 0xf) << 6) | halrf_rreg(rf, 0x32d4, 0xfc000000));


	biask_d[0][0] = (u16)halrf_rreg(rf, 0x5e24, 0xffc00000);
	biask_d[0][1] = (u16)halrf_rreg(rf, 0x5e74, 0xffc00000);
	biask_d[1][0] = (u16)halrf_rreg(rf, 0x7e24, 0xffc00000);
	biask_d[1][1] = (u16)halrf_rreg(rf, 0x7e74, 0xffc00000);

	dadck_d[0][0] = (u8)halrf_rreg(rf, 0x5e24, 0x3fc000);
	dadck_d[0][1] = (u8)halrf_rreg(rf, 0x5e74, 0x3fc000);
	dadck_d[1][0] = (u8)halrf_rreg(rf, 0x7e24, 0x3fc000);
	dadck_d[1][1] = (u8)halrf_rreg(rf, 0x7e74, 0x3fc000);

	for (i =0; i < 2;i++) {
		if (i ==0)
			id = 0;
		else
			id = 0x50;
		
		msbk_d[0][i][0] = (u8)halrf_rreg(rf, 0x5e20 + id, 0xff);
		msbk_d[0][i][1] = (u8)halrf_rreg(rf, 0x5e20 + id, 0xff00);
		msbk_d[0][i][2] = (u8)halrf_rreg(rf, 0x5e20 + id, 0xff0000);
		msbk_d[0][i][3] = (u8)halrf_rreg(rf, 0x5e20 + id, 0xff000000);
		msbk_d[0][i][4] = (u8)halrf_rreg(rf, 0x5e1c + id, 0xff);
		msbk_d[0][i][5] = (u8)halrf_rreg(rf, 0x5e1c + id, 0xff00);
		msbk_d[0][i][6] = (u8)halrf_rreg(rf, 0x5e1c + id, 0xff0000);
		msbk_d[0][i][7] = (u8)halrf_rreg(rf, 0x5e1c + id, 0xff000000);
		msbk_d[0][i][8] = (u8)halrf_rreg(rf, 0x5e18 + id, 0xff);
		msbk_d[0][i][9] = (u8)halrf_rreg(rf, 0x5e18 + id, 0xff00);
		msbk_d[0][i][10] = (u8)halrf_rreg(rf, 0x5e18 + id, 0xff0000);
		msbk_d[0][i][11] = (u8)halrf_rreg(rf, 0x5e18 + id, 0xff000000);
		msbk_d[0][i][12] = (u8)halrf_rreg(rf, 0x5e14 + id, 0xff);
		msbk_d[0][i][13] = (u8)halrf_rreg(rf, 0x5e14 + id, 0xff00);
		msbk_d[0][i][14] = (u8)halrf_rreg(rf, 0x5e14 + id, 0xff0000);
		msbk_d[0][i][15] = (u8)halrf_rreg(rf, 0x5e14 + id, 0xff000000);
	}
	for (i =0; i < 2; i++) {
		if (i ==0)
			id = 0;
		else
			id = 0x50;
		
		msbk_d[1][i][0] = (u8)halrf_rreg(rf, 0x7e20 + id, 0xff);
		msbk_d[1][i][1] = (u8)halrf_rreg(rf, 0x7e20 + id, 0xff00);
		msbk_d[1][i][2] = (u8)halrf_rreg(rf, 0x7e20 + id, 0xff0000);
		msbk_d[1][i][3] = (u8)halrf_rreg(rf, 0x7e20 + id, 0xff000000);
		msbk_d[1][i][4] = (u8)halrf_rreg(rf, 0x7e1c + id, 0xff);
		msbk_d[1][i][5] = (u8)halrf_rreg(rf, 0x7e1c + id, 0xff00);
		msbk_d[1][i][6] = (u8)halrf_rreg(rf, 0x7e1c + id, 0xff0000);
		msbk_d[1][i][7] = (u8)halrf_rreg(rf, 0x7e1c + id, 0xff000000);
		msbk_d[1][i][8] = (u8)halrf_rreg(rf, 0x7e18 + id, 0xff);
		msbk_d[1][i][9] = (u8)halrf_rreg(rf, 0x7e18 + id, 0xff00);
		msbk_d[1][i][10] = (u8)halrf_rreg(rf, 0x7e18 + id, 0xff0000);
		msbk_d[1][i][11] = (u8)halrf_rreg(rf, 0x7e18 + id, 0xff000000);
		msbk_d[1][i][12] = (u8)halrf_rreg(rf, 0x7e14 + id, 0xff);
		msbk_d[1][i][13] = (u8)halrf_rreg(rf, 0x7e14 + id, 0xff00);
		msbk_d[1][i][14] = (u8)halrf_rreg(rf, 0x7e14 + id, 0xff0000);
		msbk_d[1][i][15] = (u8)halrf_rreg(rf, 0x7e14 + id, 0xff000000);
	}

	for (p = 0; p < 2; p++) {
		for (i = 0;i < 2; i++) {
			for (j = 0; j < 16; j ++) {
				if (msbk_d[p][i][j] != dack->msbk_d[p][i][j]) {
					RF_DBG(rf, DBG_RF_RFK, "[RFK]msbk_d[%d][%d][%d] mismatch\n", p, i, j);
					fail = true;
				}
			}
		}
	}

	for (p =0; p < 2; p++) {
		for (i = 0; i < 2; i++) {
			if (biask_d[p][i] != dack->biask_d[p][i]) {
				RF_DBG(rf, DBG_RF_RFK, "[RFK]biask_d[%d][%d] mismatch\n", p, i);
				fail = true;
			}
		}
	}

	for (p =0; p < 2; p++) {
		for (i = 0; i < 2; i++) {
			if (dadck_d[p][i] != dack->dadck_d[p][i]) {
				RF_DBG(rf, DBG_RF_RFK, "[RFK]dadck_d[%d][%d] mismatch\n", p, i);
				fail = true;
			}
		}
	}



	for (p =0; p < 2; p++) {
		for (i = 0; i < 2; i++) {
			if (addck_d[p][i] != dack->addck_d[p][i]) {
				RF_DBG(rf, DBG_RF_RFK, "[RFK]addck_d[%d][%d] mismatch\n", p, i);
				fail = true;
			}
		}
	}

	return fail;
#endif
}
#endif

