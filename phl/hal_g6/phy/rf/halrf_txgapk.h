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
#ifndef _HALRF_TXGAPK_H_
#define _HALRF_TXGAPK_H_
#include "halrf_headers.h"

#define NUM		4

struct halrf_gapk_info {

	u32 gapk_rf3f_bp[5][12][NUM]; /* band(2Gcck/2GOFDM/5GL/5GM/5GH)/idx/path */
	bool gapk_bp_done;
	s8 offset[12][NUM];
	s8 fianl_offset[12][NUM];
	u8 read_txgain;
	bool is_gapk_enable;
	u8 band[NUM];
	u8 ch[NUM];
	u8 bw[NUM];	
	s32 track_d[2][17];
	s32 track_ta[2][17];
	s32 power_d[2][17];
	s32 power_ta[2][17];
};

#endif
