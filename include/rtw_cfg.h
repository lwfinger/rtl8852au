/******************************************************************************
 *
 * Copyright(c) 2007 - 2020 Realtek Corporation.
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
#ifndef _RTW_CFG_H_
#define _RTW_CFG_H_

enum rtw_pwr_by_rate_setting {
	RTW_PW_BY_RATE_ON = 0,
	RTW_PW_BY_RATE_ALL_SAME = 1
};

enum rtw_pwr_limit_type {
	RTW_PWLMT_BY_EFUSE = 0,
	RTW_PWLMT_DISABLE = 1,
	RTW_PWBYRATE_AND_PWLMT = 2
};

u8 rtw_load_dvobj_registry(struct dvobj_priv *dvobj);
uint rtw_load_registry(_adapter *adapter);

void rtw_core_update_default_setting (struct dvobj_priv *dvobj);
#endif /*_RTW_CFG_H_*/
