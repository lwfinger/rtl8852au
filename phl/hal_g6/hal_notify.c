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
#define _HAL_NOTIFY_C_
#include "hal_headers.h"

void rtw_hal_notification(void *hal, enum phl_msg_evt_id event, u8 hw_idx)
{
	struct hal_info_t *hal_info = (struct hal_info_t *)hal;
	u8 idx = 0;

	PHL_TRACE(COMP_PHL_DBG, _PHL_INFO_, "%s: event(%d), hw_idx(%d)\n",
	          __func__, event, hw_idx);

	if (hw_idx == HW_BAND_MAX) {
		for (idx = 0; idx < hw_idx; idx++) {
			rtw_hal_bb_notification(hal_info, event, idx);
			rtw_hal_mac_notification(hal_info, event, idx);
			rtw_hal_rf_notification(hal_info, event, idx);
		}
	} else {
		rtw_hal_bb_notification(hal_info, event, hw_idx);
		rtw_hal_mac_notification(hal_info, event, hw_idx);
		rtw_hal_rf_notification(hal_info, event, idx);
	}
}

enum rtw_phl_status
rtw_hal_cmd_notify(struct rtw_phl_com_t *phl_com,
                   enum phl_msg_evt_id event,
                   u8 hw_idx)
{
#ifdef CONFIG_CMD_DISP
	return rtw_phl_cmd_notify(phl_com, event, hw_idx);
#else
	PHL_TRACE(COMP_PHL_DBG, _PHL_INFO_, "%s: not support cmd notify\n",
	          __func__);

	return RTW_PHL_STATUS_FAILURE;
#endif /* CONFIG_CMD_DISP */
}
