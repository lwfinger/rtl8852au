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
#define _PHL_API_DRV_C_
#include "phl_headers.h"
#include "phl_api_drv.h"

void *rtw_phl_get_txbd_buf(struct rtw_phl_com_t *phl_com)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;

	return phl_info->hci_trx_ops->get_txbd_buf(phl_info);
}

void *rtw_phl_get_rxbd_buf(struct rtw_phl_com_t *phl_com)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;

	return phl_info->hci_trx_ops->get_rxbd_buf(phl_info);
}

struct rtw_h2c_pkt *rtw_phl_query_h2c_pkt(struct rtw_phl_com_t *phl_com,
											enum rtw_h2c_pkt_type type)
{
	struct rtw_h2c_pkt *h2c_pkt = NULL;
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;

	h2c_pkt = (struct rtw_h2c_pkt *)phl_query_idle_h2c_pkt(phl_info, type);

	if (!h2c_pkt)
		phl_dump_h2c_pool_stats(phl_info->h2c_pool);

	return h2c_pkt;
}

enum rtw_phl_status rtw_phl_pltfm_tx(struct rtw_phl_com_t *phl_com,
										struct rtw_h2c_pkt *pkt)
{
	enum rtw_phl_status pstatus = RTW_PHL_STATUS_FAILURE;
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;

	#ifdef CONFIG_PCI_HCI
	struct phl_hci_trx_ops *hci_trx_ops = phl_info->hci_trx_ops;
	hci_trx_ops->recycle_busy_h2c(phl_info);
	#endif

	pstatus = phl_info->hci_trx_ops->pltfm_tx(phl_info, pkt);

	return pstatus;
}

void rtw_phl_proc_cmd(void *phl, char proc_cmd,
		      struct rtw_proc_cmd *incmd,
		      char *output, u32 out_len)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;
	struct hal_info_t *hal_info = (struct hal_info_t *)phl_info->hal;
	enum rtw_hal_status status = RTW_HAL_STATUS_SUCCESS;

	if (RTW_PROC_CMD_PHL == proc_cmd)
		status = rtw_phl_dbg_proc_cmd(phl_info, incmd, output, out_len);
	else if (RTW_PROC_CMD_CORE == proc_cmd)
		status = (enum rtw_hal_status)rtw_phl_dbg_core_cmd(phl_info, incmd, output, out_len);
	else
		status = rtw_hal_proc_cmd(hal_info, proc_cmd, incmd, output, out_len);
}

void rtw_phl_get_fw_ver(void *phl, char *ver_str, u16 len)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;

	rtw_hal_get_fw_ver(phl_info->hal, ver_str, len);
}

enum rtw_phl_status rtw_phl_msg_hub_hal_send(struct rtw_phl_com_t *phl_com,
							struct phl_msg_attribute* attr, struct phl_msg* msg)
{
	return phl_msg_hub_send((struct phl_info_t*)phl_com->phl_priv, attr, msg);
}
void rtw_phl_test_txtb_cfg(struct rtw_phl_com_t* phl_com,
	void *buf, u32 buf_len, u8 *cfg_bssid, u16 cfg_aid, u8 cfg_bsscolor)
{
	/* Get parameters from MP UL */
	u8 ui_bssid[6]={0x00, 0x09, 0x08, 0x07, 0x06, 0x05};
	u16 ui_aid = 2;
	u8 ui_bsscolor = 1;
	u8 cfg;
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;
	struct rtw_wifi_role_t *role = &phl_com->wifi_roles[0];
	struct rtw_phl_stainfo_t *sta = rtw_phl_get_stainfo_self(
		phl_com->phl_priv, role);

	cfg = (u8)*((u8 *)buf);
	switch (cfg) {
		case 0:
			/* disable */
			role->mstate = MLME_NO_LINK;
			_os_mem_cpy(phlcom_to_drvpriv(phl_com), sta->mac_addr,
				role->mac_addr, MAC_ALEN);
			rtw_hal_update_sta_entry(phl_info->hal, sta, false);
			rtw_hal_role_cfg(phl_info->hal, role);
		break;

		case 1:
			role->mstate = MLME_LINKED;
			_os_mem_cpy(phlcom_to_drvpriv(phl_com), sta->mac_addr,
				cfg_bssid, MAC_ALEN);
			sta->wmode = WLAN_MD_MAX;
			sta->aid = cfg_aid;
			sta->asoc_cap.bsscolor = cfg_bsscolor;
			sta->asoc_cap.pkt_padding = 2;
			rtw_hal_update_sta_entry(phl_info->hal, sta, false);
			rtw_hal_role_cfg(phl_info->hal, role);
		break;

		case 2:
			role->mstate = MLME_LINKED;
			_os_mem_cpy(phlcom_to_drvpriv(phl_com), sta->mac_addr,
				ui_bssid, MAC_ALEN);
			sta->wmode = WLAN_MD_MAX;
			sta->aid = ui_aid;
			sta->asoc_cap.bsscolor = ui_bsscolor;
			sta->asoc_cap.pkt_padding = 2;
			rtw_hal_update_sta_entry(phl_info->hal, sta, false);
			rtw_hal_role_cfg(phl_info->hal, role);
		break;		default:
		break;
	}
}

void rtw_phl_pkt_ofld_reset_all_entry(struct rtw_phl_com_t* phl_com)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;

	phl_pkt_ofld_reset_all_entry(phl_info);
}

void rtw_phl_dbg_dump_rx(void *phl, struct rtw_wifi_role_t *wrole)
{
	enum rtw_phl_status phl_status = RTW_PHL_STATUS_FAILURE;
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;
	struct phl_msg msg = {0};
	struct phl_msg_attribute attr = {0};

	do {
		SET_MSG_MDL_ID_FIELD(msg.msg_id, PHL_MDL_GENERAL);
		SET_MSG_EVT_ID_FIELD(msg.msg_id, MSG_EVT_DBG_RX_DUMP);

		msg.band_idx = wrole->hw_band;

		phl_status = phl_disp_eng_send_msg(phl_info, &msg, &attr, NULL);

		if (phl_status != RTW_PHL_STATUS_SUCCESS) {
			PHL_TRACE(COMP_PHL_DBG, _PHL_ERR_, "%s: Dispr send msg fail!\n",
				  __func__);
			break;
		}

	} while (0);

}
