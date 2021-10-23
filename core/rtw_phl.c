/******************************************************************************
 *
 * Copyright(c) 2019 - 2021 Realtek Corporation.
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
#define _RTW_PHL_C_
#include <drv_types.h>


/***************** export API to osdep/core*****************/

static const char *const _band_cap_str[] = {
	/* BIT0 */"2G",
	/* BIT1 */"5G",
	/* BIT2 */"6G",	
};

static const char *const _bw_cap_str[] = {
	/* BIT0 */"20M",
	/* BIT1 */"40M",
	/* BIT2 */"80M",
	/* BIT3 */"160M",
	/* BIT4 */"80_80M",
	/* BIT5 */"5M",
	/* BIT6 */"10M",
};

static const char *const _proto_cap_str[] = {
	/* BIT0 */"b",
	/* BIT1 */"g",
	/* BIT2 */"n",
	/* BIT3 */"ac",
};

static const char *const _wl_func_str[] = {
	/* BIT0 */"P2P",
	/* BIT1 */"MIRACAST",
	/* BIT2 */"TDLS",
	/* BIT3 */"FTM",
};

static const char *const hw_cap_str = "[HW-CAP]";
void rtw_hw_dump_hal_spec(void *sel, struct dvobj_priv *dvobj)
{
	struct hal_spec_t *hal_spec = GET_HAL_SPEC(dvobj);
	int i;

	RTW_PRINT_SEL(sel, "%s ic_name:%s\n", hw_cap_str, hal_spec->ic_name);
	RTW_PRINT_SEL(sel, "%s macid_num:%u\n", hw_cap_str, hal_spec->macid_num);
	RTW_PRINT_SEL(sel, "%s sec_cap:0x%02x\n", hw_cap_str, hal_spec->sec_cap);
	RTW_PRINT_SEL(sel, "%s sec_cam_ent_num:%u\n", hw_cap_str, hal_spec->sec_cam_ent_num);

	RTW_PRINT_SEL(sel, "%s rfpath_num_2g:%u\n", hw_cap_str, hal_spec->rfpath_num_2g);
	RTW_PRINT_SEL(sel, "%s rfpath_num_5g:%u\n", hw_cap_str, hal_spec->rfpath_num_5g);
	RTW_PRINT_SEL(sel, "%s rf_reg_path_num:%u\n", hw_cap_str, hal_spec->rf_reg_path_num);
	RTW_PRINT_SEL(sel, "%s max_tx_cnt:%u\n", hw_cap_str, hal_spec->max_tx_cnt);

	RTW_PRINT_SEL(sel, "%s tx_nss_num:%u\n", hw_cap_str, hal_spec->tx_nss_num);
	RTW_PRINT_SEL(sel, "%s rx_nss_num:%u\n", hw_cap_str, hal_spec->rx_nss_num);

	RTW_PRINT_SEL(sel, "%s band_cap:", hw_cap_str);
	for (i = 0; i < BAND_CAP_BIT_NUM; i++) {
		if (((hal_spec->band_cap) >> i) & BIT0 && _band_cap_str[i])
			_RTW_PRINT_SEL(sel, "%s ", _band_cap_str[i]);
	}
	_RTW_PRINT_SEL(sel, "\n");

	RTW_PRINT_SEL(sel, "%s bw_cap:", hw_cap_str);
	for (i = 0; i < BW_CAP_BIT_NUM; i++) {
		if (((hal_spec->bw_cap) >> i) & BIT0 && _bw_cap_str[i])
			_RTW_PRINT_SEL(sel, "%s ", _bw_cap_str[i]);
	}
	_RTW_PRINT_SEL(sel, "\n");

	RTW_PRINT_SEL(sel, "%s proto_cap:", hw_cap_str);
	for (i = 0; i < PROTO_CAP_BIT_NUM; i++) {
		if (((hal_spec->proto_cap) >> i) & BIT0 && _proto_cap_str[i])
			_RTW_PRINT_SEL(sel, "%s ", _proto_cap_str[i]);
	}
	_RTW_PRINT_SEL(sel, "\n");

#if 0 /*GEORGIA_TODO_FIXIT*/
	RTW_PRINT_SEL(sel, "%s txgi_max:%u\n", hw_cap_str, hal_spec->txgi_max);
	RTW_PRINT_SEL(sel, "%s txgi_pdbm:%u\n", hw_cap_str, hal_spec->txgi_pdbm);
#endif
	RTW_PRINT_SEL(sel, "%s wl_func:", hw_cap_str);
	for (i = 0; i < WL_FUNC_BIT_NUM; i++) {
		if (((hal_spec->wl_func) >> i) & BIT0 && _wl_func_str[i])
			_RTW_PRINT_SEL(sel, "%s ", _wl_func_str[i]);
	}
	_RTW_PRINT_SEL(sel, "\n");
	
#if 0 /*GEORGIA_TODO_FIXIT*/

	RTW_PRINT_SEL(sel, "%s pg_txpwr_saddr:0x%X\n", hw_cap_str, hal_spec->pg_txpwr_saddr);
	RTW_PRINT_SEL(sel, "%s pg_txgi_diff_factor:%u\n", hw_cap_str, hal_spec->pg_txgi_diff_factor);
#endif
}

void rtw_dump_phl_sta_info(void *sel, struct sta_info *sta)
{	
	struct rtw_phl_stainfo_t *phl_sta = sta->phl_sta;

	RTW_PRINT_SEL(sel, "[PHL STA]- role-idx: %d\n", phl_sta->wrole->id);

	RTW_PRINT_SEL(sel, "[PHL STA]- mac_addr:"MAC_FMT"\n", MAC_ARG(phl_sta->mac_addr));
	RTW_PRINT_SEL(sel, "[PHL STA]- aid: %d\n", phl_sta->aid);
	RTW_PRINT_SEL(sel, "[PHL STA]- macid: %d\n", phl_sta->macid);
	
	RTW_PRINT_SEL(sel, "[PHL STA]- wifi_band: %d\n", phl_sta->chandef.band);
	RTW_PRINT_SEL(sel, "[PHL STA]- bw: %d\n", phl_sta->chandef.bw);
	RTW_PRINT_SEL(sel, "[PHL STA]- chan: %d\n", phl_sta->chandef.chan);
	RTW_PRINT_SEL(sel, "[PHL STA]- offset: %d\n", phl_sta->chandef.offset);
}

inline bool rtw_hw_chk_band_cap(struct dvobj_priv *dvobj, u8 cap)
{
	return GET_HAL_SPEC(dvobj)->band_cap & cap;
}

inline bool rtw_hw_chk_bw_cap(struct dvobj_priv *dvobj, u8 cap)
{
	return GET_HAL_SPEC(dvobj)->bw_cap & cap;
}

inline bool rtw_hw_chk_proto_cap(struct dvobj_priv *dvobj, u8 cap)
{
	return GET_HAL_SPEC(dvobj)->proto_cap & cap;
}

inline bool rtw_hw_chk_wl_func(struct dvobj_priv *dvobj, u8 func)
{
	return GET_HAL_SPEC(dvobj)->wl_func & func;
}

inline bool rtw_hw_is_band_support(struct dvobj_priv *dvobj, u8 band)
{
	return GET_HAL_SPEC(dvobj)->band_cap & band_to_band_cap(band);
}

inline bool rtw_hw_is_bw_support(struct dvobj_priv *dvobj, u8 bw)
{
	return GET_HAL_SPEC(dvobj)->bw_cap & ch_width_to_bw_cap(bw);
}

inline bool rtw_hw_is_wireless_mode_support(struct dvobj_priv *dvobj, u8 mode)
{
	u8 proto_cap = GET_HAL_SPEC(dvobj)->proto_cap;

	if (mode == WLAN_MD_11B)
		if ((proto_cap & PROTO_CAP_11B) && rtw_hw_chk_band_cap(dvobj, BAND_CAP_2G))
			return 1;

	if (mode == WLAN_MD_11G)
		if ((proto_cap & PROTO_CAP_11G) && rtw_hw_chk_band_cap(dvobj, BAND_CAP_2G))
			return 1;

	if (mode == WLAN_MD_11A)
		if ((proto_cap & PROTO_CAP_11G) && rtw_hw_chk_band_cap(dvobj, BAND_CAP_5G))
			return 1;

	#ifdef CONFIG_80211N_HT
	if (mode == WLAN_MD_11N)
		if (proto_cap & PROTO_CAP_11N)
			return 1;
	#endif

	#ifdef CONFIG_80211AC_VHT
	if (mode == WLAN_MD_11AC)
		if ((proto_cap & PROTO_CAP_11AC) && rtw_hw_chk_band_cap(dvobj, BAND_CAP_5G))
			return 1;
	#endif

	#ifdef CONFIG_80211AX_HE
	if (mode == WLAN_MD_11AX)
		if (proto_cap & PROTO_CAP_11AX)
			return 1;
	#endif
	return 0;
}


inline u8 rtw_hw_get_wireless_mode(struct dvobj_priv *dvobj)
{
	u8 proto_cap = GET_HAL_SPEC(dvobj)->proto_cap;
	u8 band_cap = GET_HAL_SPEC(dvobj)->band_cap;
	u8 wireless_mode = 0;

	if(proto_cap & PROTO_CAP_11B)
		wireless_mode |= WLAN_MD_11B;

	if(proto_cap & PROTO_CAP_11G)
		wireless_mode |= WLAN_MD_11G;

	if(band_cap & BAND_CAP_5G)
		wireless_mode |= WLAN_MD_11A;

	#ifdef CONFIG_80211N_HT
	if(proto_cap & PROTO_CAP_11N)
		wireless_mode |= WLAN_MD_11N;
	#endif

	#ifdef CONFIG_80211AC_VHT
	if(proto_cap & PROTO_CAP_11AC) 
		wireless_mode |= WLAN_MD_11AC;
	#endif

	#ifdef CONFIG_80211AX_HE
	if(proto_cap & PROTO_CAP_11AX) {
			wireless_mode |= WLAN_MD_11AX;
	}
	#endif
	
	return wireless_mode;
}

inline u8 rtw_hw_get_band_type(struct dvobj_priv *dvobj)
{
	u8 band_cap = GET_HAL_SPEC(dvobj)->band_cap;
	u8 band_type = 0;

	if(band_cap & BAND_CAP_2G)
		band_type |= BAND_CAP_2G;

#if CONFIG_IEEE80211_BAND_5GHZ
	if(band_cap & BAND_CAP_5G)
		band_type |= BAND_CAP_5G;
#endif

#if CONFIG_IEEE80211_BAND_6GHZ
	if(band_cap & BAND_CAP_6G)
		band_type |= BAND_CAP_6G;
#endif

	return band_type;
}

inline bool rtw_hw_is_mimo_support(struct dvobj_priv *dvobj)
{
	if ((GET_HAL_TX_NSS(dvobj) == 1) &&
		(GET_HAL_RX_NSS(dvobj) == 1))
		return 0;
	return 1;
}

/*
* rtw_hw_largest_bw - starting from in_bw, get largest bw supported by HAL
* @adapter:
* @in_bw: starting bw, value of enum channel_width
*
* Returns: value of enum channel_width
*/
u8 rtw_hw_largest_bw(struct dvobj_priv *dvobj, u8 in_bw)
{
	for (; in_bw > CHANNEL_WIDTH_20; in_bw--) {
		if (rtw_hw_is_bw_support(dvobj, in_bw))
			break;
	}

	if (!rtw_hw_is_bw_support(dvobj, in_bw))
		rtw_warn_on(1);

	return in_bw;
}

u8 rtw_hw_get_mac_addr(struct dvobj_priv *dvobj, u8 *hw_mac_addr)
{
	if (rtw_phl_get_mac_addr_efuse(dvobj->phl, hw_mac_addr) != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s failed\n", __func__);
		return _FAIL;
	}
	return _SUCCESS;
}


/***************** register hw *****************/
#if 0 /*GEORGIA_TODO_ADDIT*/

#define hal_trx_error_msg(ops_fun)		\
	RTW_PRINT("### %s - Error : Please hook hal_trx_ops.%s ###\n", __FUNCTION__, ops_fun)
static u8 rtw_hw_trx_ops_check(struct hal_com_t *hal_com)
{
	u8 rst = _SUCCESS;

	if (!hal_com->trx_ops.intf_hal_configure) {
		hal_trx_error_msg("intf_hal_configure");
		rst = _FAIL;
	}

	if (!hal_com->trx_ops.get_txdesc_len) {
		hal_trx_error_msg("get_txdesc_len");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.fill_txdesc_h2c) {
		hal_trx_error_msg("fill_txdesc_h2c");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.fill_txdesc_fwdl) {
		hal_trx_error_msg("fill_txdesc_fwdl");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.fill_txdesc_pkt) {
		hal_trx_error_msg("fill_txdesc_pkt");
		rst = _FAIL;
	}

#if defined(CONFIG_USB_HCI)
	if (!hal_com->trx_ops.get_bulkout_id) {
		hal_trx_error_msg("get_bulkout_id");
		rst = _FAIL;
	}
#endif

#if 0 /*GEORGIA_TODO_ADDIT*/
	if (!hal_com->trx_ops.init_xmit) {
		hal_trx_error_msg("init_xmit");
		rst = _FAIL;
	}

	if (!hal_com->trx_ops.init_recv) {
		hal_trx_error_msg("init_recv");
		rst = _FAIL;
	}

	#if defined(CONFIG_PCI_HCI)
	if (!hal_com->trx_ops.check_enough_txdesc) {
		hal_trx_error_msg("check_enough_txdesc");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.trxbd_init) {
		hal_trx_error_msg("trxbd_init");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.trxbd_deinit) {
		hal_trx_error_msg("trxbd_deinit");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.trxbd_reset) {
		hal_trx_error_msg("trxbd_reset");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.interrupt_handler) {
		hal_trx_error_msg("interrupt_handler");
		rst = _FAIL;
	}
	#endif

	#if defined(CONFIG_USB_HCI)
	#ifdef CONFIG_SUPPORT_USB_INT
	if (!hal_com->trx_ops.interrupt_handler) {
		hal_trx_error_msg("interrupt_handler");
		rst = _FAIL;
	}
	#endif
	#endif

	if (!hal_com->trx_ops.enable_interrupt) {
		hal_trx_error_msg("enable_interrupt");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.disable_interrupt) {
		hal_trx_error_msg("disable_interrupt");
		rst = _FAIL;
	}

	#if defined(CONFIG_SDIO_HCI)
	if (!hal_com->trx_ops.interrupt_handler) {
		hal_trx_error_msg("interrupt_handler");
		rst = _FAIL;
	}
	if (!hal_com->trx_ops.get_tx_addr) {
		hal_trx_error_msg("get_tx_addr");
		rst = _FAIL;
	}
	#endif
#endif
	return rst;
}
#endif

u8 rtw_core_deregister_phl_msg(struct dvobj_priv *dvobj)
{
	enum rtw_phl_status psts = RTW_PHL_STATUS_FAILURE;

	psts = rtw_phl_msg_hub_deregister_recver(dvobj->phl, MSG_RECV_CORE);
	if(psts	== RTW_PHL_STATUS_FAILURE) {
		RTW_ERR("%s failed\n", __func__);
		return _FAIL;
	}
	return _SUCCESS;
}

void rtw_hw_deinit(struct dvobj_priv *dvobj)
{
	if (dvobj->phl) {
		rtw_phl_trx_free(dvobj->phl);
		rtw_core_deregister_phl_msg(dvobj);
		rtw_phl_watchdog_deinit(dvobj->phl);
		rtw_phl_deinit(dvobj->phl);
	}

	#ifdef DBG_PHL_MEM_ALLOC
	RTW_INFO("[PHL-MEM] %s PHL memory :%d\n", __func__,
					ATOMIC_READ(&(dvobj->phl_mem)));
	#endif
}

#if 0
void dump_ic_spec(struct dvobj_priv *dvobj)
{
	struct hal_com_t *hal_com = dvobj->hal_com;
	struct hal_spec_t *hal_spec = &hal_com->hal_spec;

	RTW_INFO("dvobj:%p,hal:%p(size:%d), hal_com:%p, hal_spec:%p\n",
		dvobj, dvobj->hal_info, dvobj->hal_info_sz, hal_com, hal_spec);
	RTW_INFO("dvobj:%p, hal_com:%p, hal_spec:%p\n", dvobj, GET_PHL_COM(dvobj), GET_HAL_SPEC(dvobj));

	RTW_INFO("[IC-SPEC]- band_cap: %x\n", GET_HAL_SPEC(dvobj)->band_cap);
}
#endif

#if 0 /*GEORGIA_TODO_FIXIT*/
void rtw_hw_intf_cfg(struct dvobj_priv *dvobj, struct hal_com_t *hal_com)
{
	struct hci_info_st hci_info;

	#ifdef CONFIG_PCI_HCI
	if (dvobj->interface_type == RTW_HCI_PCIE) {
		PPCI_DATA pci = dvobj_to_pci(dvobj);
		//hci_info.
	}
	#endif

	#ifdef CONFIG_USB_HCI
	if (dvobj->interface_type == RTW_HCI_USB) {
		PUSB_DATA usb = dvobj_to_usb(dvobj);
		#if 0
		u8 usb_speed; /* 1.1, 2.0 or 3.0 */
		u16 usb_bulkout_size;
		u8 nr_endpoint; /*MAX_ENDPOINT_NUM*/

		/* Bulk In , Out Pipe information */
		int RtInPipe[MAX_BULKIN_NUM];
		u8 RtNumInPipes;
		int RtOutPipe[MAX_BULKOUT_NUM];
		u8 RtNumOutPipes;
		#endif
		//hci_info
	}
	#endif

	#ifdef CONFIG_SDIO_HCI
	if (dvobj->interface_type == RTW_HCI_SDIO) {
		PSDIO_DATA sdio = dvobj_to_sdio(dvobj);

		hci_info.clock = sdio->clock;
		hci_info.timing = sdio->timing;
		hci_info.sd3_bus_mode = sdio->sd3_bus_mode;
		hci_info.block_sz = sdio->block_transfer_len;
		hci_info.align_sz = sdio->block_transfer_len;
	}
	#endif

	rtw_hal_intf_config(hal_com, &hci_info);
}
#endif

static void _hw_ic_info_cfg(struct dvobj_priv *dvobj, struct rtw_ic_info *ic_info)
{
	_rtw_memset(ic_info, 0,sizeof(struct rtw_ic_info));

	ic_info->ic_id = dvobj->ic_id;
	ic_info->hci_type = dvobj->interface_type;

	#ifdef CONFIG_PCI_HCI
	if (dvobj->interface_type == RTW_HCI_PCIE) {
		PPCI_DATA pci = dvobj_to_pci(dvobj);

	}
	#endif

	#ifdef CONFIG_USB_HCI
	if (dvobj->interface_type == RTW_HCI_USB) {
		PUSB_DATA usb = dvobj_to_usb(dvobj);

		ic_info->usb_info.usb_speed = usb->usb_speed;
		ic_info->usb_info.usb_bulkout_size = usb->usb_bulkout_size;
		ic_info->usb_info.inep_num = usb->RtNumInPipes;
		ic_info->usb_info.outep_num = usb->RtNumOutPipes;
	}
	#endif

	#ifdef CONFIG_SDIO_HCI
	if (dvobj->interface_type == RTW_HCI_SDIO) {
		PSDIO_DATA sdio = dvobj_to_sdio(dvobj);

		ic_info->sdio_info.clock = sdio->clock;
		ic_info->sdio_info.timing = sdio->timing;
		ic_info->sdio_info.sd3_bus_mode = sdio->sd3_bus_mode;
		ic_info->sdio_info.io_align_sz = 4;
		ic_info->sdio_info.block_sz = sdio->block_transfer_len;
		ic_info->sdio_info.tx_align_sz = sdio->block_transfer_len;
		ic_info->sdio_info.tx_512_by_byte_mode =
				(sdio->max_byte_size >= 512) ? true : false;
	}
	#endif
}
static void core_hdl_phl_evt(struct dvobj_priv *dvobj, u16 evt_id)
{
	_adapter *iface;
	u8 i = 0;

	if(evt_id == MSG_EVT_BCN_RESEND) {
		for (i = 0; i < dvobj->iface_nums; i++) {
			iface = dvobj->padapters[i];
			if(!rtw_is_adapter_up(iface))
				continue;

			if(MLME_IS_MESH(iface)
				|| MLME_IS_AP(iface)
				|| MLME_IS_ADHOC_MASTER(iface)) {
				if (send_beacon(iface) == _FAIL)
					RTW_ERR(ADPT_FMT" issue_beacon, fail!\n",
								ADPT_ARG(iface));
			}
		}
	}
	else if (evt_id == MSG_EVT_SER_L2) {
		RTW_INFO("RECV PHL MSG_EVT_SER_L2\n");
	}
#ifdef CONFIG_XMIT_ACK
	else if (evt_id == MSG_EVT_CCX_REPORT_TX_OK) {
			iface = dvobj_get_primary_adapter(dvobj);
			if (rtw_is_adapter_up(iface))
				rtw_ack_tx_done(&iface->xmitpriv, RTW_SCTX_DONE_SUCCESS);
	}
	else if (evt_id == MSG_EVT_CCX_REPORT_TX_FAIL) {
			iface = dvobj_get_primary_adapter(dvobj);
			if (rtw_is_adapter_up(iface))
				rtw_ack_tx_done(&iface->xmitpriv, RTW_SCTX_DONE_CCX_PKT_FAIL);
	}
#endif
	else {
		RTW_INFO("%s evt_id :%d\n", __func__, evt_id);
	}
}

void core_handler_phl_msg(void *drv_priv, struct phl_msg *msg)
{
	struct dvobj_priv *dvobj = (struct dvobj_priv *)drv_priv;
	u8 mdl_id = MSG_MDL_ID_FIELD(msg->msg_id);
	u16 evt_id = MSG_EVT_ID_FIELD(msg->msg_id);

	switch(mdl_id) {
	case PHL_MDL_RX:
	case PHL_MDL_SER:
	case PHL_MDL_WOW:
		core_hdl_phl_evt(dvobj, evt_id);
		break;
	default:
		RTW_ERR("%s mdl_id :%d not support\n", __func__, mdl_id);
		break;
	}
}

u8 rtw_core_register_phl_msg(struct dvobj_priv *dvobj)
{
	struct phl_msg_receiver ctx = {0};
	u8 imr[] = {PHL_MDL_RX, PHL_MDL_SER, PHL_MDL_WOW};
	enum rtw_phl_status psts = RTW_PHL_STATUS_FAILURE;

	ctx.incoming_evt_notify = core_handler_phl_msg;
	ctx.priv = (void*)dvobj;

	psts = rtw_phl_msg_hub_register_recver(dvobj->phl, &ctx, MSG_RECV_CORE);
	if(psts	== RTW_PHL_STATUS_FAILURE) {
		RTW_ERR("phl_msg_hub_register failed\n");
		return _FAIL;
	}

	psts = rtw_phl_msg_hub_update_recver_mask(dvobj->phl,
					MSG_RECV_CORE, imr, sizeof(imr), false);
	if(psts	== RTW_PHL_STATUS_FAILURE) {
		RTW_ERR("phl_msg_hub_update_recver_mask failed\n");
		return _FAIL;
	}
	return _SUCCESS;
}

/*RTW_WKARD_CORE_RSSI_V1*/
s8 rtw_phl_rssi_to_dbm(u8 rssi)
{
	return rssi - PHL_MAX_RSSI;
}


#ifdef CONFIG_MCC_MODE
u8 rtw_hw_mcc_chk_inprogress(struct _ADAPTER *a)
{
	struct dvobj_priv *d;
	void *phl;
	u8 ret = _FALSE;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		goto exit;

	ret = rtw_phl_mr_query_mcc_inprogress(phl, a->phl_role, RTW_PHL_MCC_CHK_INPROGRESS);

exit:
	return ret;
}

#ifdef CONFIG_P2P_PS
static int _mcc_update_noa(void *priv, struct rtw_phl_mcc_noa *param)
{
	struct dvobj_priv *dvobj = (struct dvobj_priv *) priv;
	struct rtw_wifi_role_t *wrole = NULL;
	struct _ADAPTER *a = NULL;
	struct wifidirect_info *wdinfo;
	u8 id = 0;
	u8 ret = _SUCCESS;
#ifdef CONFIG_PHL_P2PPS
	struct rtw_phl_noa_desc desc= {0};
#endif
	wrole = param->wrole;
	if (wrole == NULL) {
		RTW_ERR("%s wrole is NULL\n", __func__);
		ret = _FAIL;
		goto exit;
	}

	id = wrole->id;
	if (id >= CONFIG_IFACE_NUMBER) {
		RTW_ERR("%s error id (%d)\n", __func__, id);
		ret = _FAIL;
		goto exit;
	}

	a = dvobj->padapters[id];
	if (a == NULL) {
		RTW_ERR("%s adapter(%d) is NULL\n", __func__, id);
		ret = _FAIL;
		goto exit;
	}

	/* by pass non-GO case */
	if (!MLME_IS_GO(a))
		goto exit;

	wdinfo = &a->wdinfo;
	RTW_INFO(FUNC_ADPT_FMT":(%d)\n", FUNC_ADPT_ARG(a), id);
	RTW_INFO("start_t_h=0x%02x,start_t_l=0x%02x\n", param->start_t_h, param->start_t_l);
	RTW_INFO("dur=0x%d,cnt=0x%d,interval=0x%d\n", param->dur, param->cnt, param->interval);

#ifdef CONFIG_PHL_P2PPS
	/* enable TSF32 toggle */
	desc.tag = P2PPS_TRIG_MCC;
	desc.enable = true;
	desc.duration = param->dur * NET80211_TU_TO_US;
	desc.interval = param->interval * NET80211_TU_TO_US;
	desc.start_t_h = param->start_t_h;
	desc.start_t_l = param->start_t_l;
	desc.count = param->cnt;
	desc.w_role = param->wrole;
	if (rtw_phl_p2pps_noa_update(dvobj->phl, &desc) != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s rtw_phl_p2pps_noa_update fail\n", __func__);
		ret = _FAIL;
		goto exit;
	}
#endif

	/* update NoA IE */
	wdinfo->noa_index = wdinfo->noa_index + 1;
	wdinfo->noa_num = 1;
	wdinfo->noa_count[0] = param->cnt;
	wdinfo->noa_duration[0] =param->dur * NET80211_TU_TO_US;
	wdinfo->noa_interval[0] = param->interval * NET80211_TU_TO_US;
	wdinfo->noa_start_time[0] = param->start_t_l;

	update_beacon(a, _VENDOR_SPECIFIC_IE_, P2P_OUI, _TRUE, RTW_CMDF_DIRECTLY);
exit:
	return ret;
}
#endif
struct rtw_phl_mcc_ops rtw_mcc_ops = {
	.priv = NULL,
	.mcc_update_noa = _mcc_update_noa,
};
#endif

struct rtw_phl_mr_ops rtw_mr_ops = {
#ifdef CONFIG_MCC_MODE
	.mcc_ops = &rtw_mcc_ops,
#endif
};

void rtw_core_register_mr_config(struct dvobj_priv *dvobj)
{
#ifdef CONFIG_MCC_MODE
	rtw_mr_ops.mcc_ops->priv = (void *)dvobj;
#endif
	rtw_phl_mr_ops_init(dvobj->phl, &rtw_mr_ops);
}

u8 rtw_hw_init(struct dvobj_priv *dvobj)
{
	u8 rst = _FAIL;
	enum rtw_phl_status phl_status;
	struct rtw_ic_info ic_info;
	struct rtw_phl_evt_ops *evt_ops;

#ifdef DBG_PHL_MEM_ALLOC
	ATOMIC_SET(&dvobj->phl_mem, 0);
#endif

	_hw_ic_info_cfg(dvobj, &ic_info);
	phl_status = rtw_phl_init(dvobj, &(dvobj->phl), &ic_info);

	if ((phl_status != RTW_PHL_STATUS_SUCCESS) || (dvobj->phl == NULL)) {
		RTW_ERR("%s - rtw_phl_init failed status(%d), dvobj->phl(%p)\n",
			__func__, phl_status, dvobj->phl);
		goto _free_hal;
	}

	dvobj->phl_com = rtw_phl_get_com(dvobj->phl);

	/*init sw cap from registary*/
	rtw_core_update_default_setting(dvobj);

	/* sw & hw cap*/
	rtw_phl_cap_pre_config(dvobj->phl);

	#ifdef CONFIG_RX_PSTS_PER_PKT
	rtw_phl_init_ppdu_sts_para(dvobj->phl_com,
		_TRUE, _FALSE,
		RTW_PHL_PSTS_FLTR_MGNT | RTW_PHL_PSTS_FLTR_DATA /*| RTW_PHL_PSTS_FLTR_CTRL*/
		);
	#endif
	/*init datapath section*/
	rtw_phl_trx_alloc(dvobj->phl);
	evt_ops = &(dvobj->phl_com->evt_ops);
	evt_ops->rx_process = rtw_core_rx_process;
	evt_ops->tx_recycle = rtw_core_tx_recycle;
#ifdef CONFIG_RTW_IPS
	evt_ops->set_rf_state = rtw_core_set_ips_state;
#endif
	rtw_core_register_phl_msg(dvobj);

	/* load wifi feature or capability from efuse*/
	rtw_phl_preload(dvobj->phl);

	rtw_phl_final_cap_decision(dvobj->phl);

	/* after final cap decision */
	rtw_core_register_mr_config(dvobj);

	rtw_hw_dump_hal_spec(RTW_DBGDUMP, dvobj);

	#ifdef CONFIG_CMD_GENERAL
	rtw_phl_watchdog_init(dvobj->phl,
				0,
				rtw_core_watchdog_sw_hdlr,
				rtw_core_watchdog_hw_hdlr);
	#else
	rtw_phl_job_reg_wdog(dvobj->phl,
			rtw_dynamic_check_handlder,
                        dvobj, NULL, 0, "rtw_dm", PWR_BASIC_IO);
	#endif
	rst = _SUCCESS;
	return rst;

_free_hal :
	rtw_hw_deinit(dvobj);
	return rst;
}

u8 rtw_hw_start(struct dvobj_priv *dvobj)
{
	if (dev_is_hw_start(dvobj))
		return _FAIL;

	if (rtw_phl_start(GET_PHL_INFO(dvobj)) != RTW_PHL_STATUS_SUCCESS)
		return _FAIL;

	#ifdef CONFIG_PCI_HCI
	//intr init flag
	dvobj_to_pci(dvobj)->irq_enabled = 1;
	#endif
	#ifdef CONFIG_CMD_GENERAL
	rtw_phl_watchdog_start(dvobj->phl);
	#endif

	dev_set_hw_start(dvobj);

	return _SUCCESS;
}
void rtw_hw_stop(struct dvobj_priv *dvobj)
{
	if (!dev_is_hw_start(dvobj))
		return;

	#ifdef CONFIG_CMD_GENERAL
	rtw_phl_watchdog_stop(dvobj->phl);
	#endif
	rtw_phl_stop(GET_PHL_INFO(dvobj));

	#ifdef CONFIG_PCI_HCI
	//intr init flag
	dvobj_to_pci(dvobj)->irq_enabled = 0;
	#endif

	dev_clr_hw_start(dvobj);
}

bool rtw_hw_get_init_completed(struct dvobj_priv *dvobj)
{
	return rtw_phl_is_init_completed(GET_PHL_INFO(dvobj));
}

bool rtw_hw_is_init_completed(struct dvobj_priv *dvobj)
{
	return (rtw_phl_is_init_completed(GET_PHL_INFO(dvobj))) ? _TRUE : _FALSE;
}

#define NSS_VALID(nss) (nss > 0)
void rtw_hw_cap_init(struct dvobj_priv *dvobj)
{
	struct hal_spec_t *hal_spec = GET_HAL_SPEC(dvobj);
	struct registry_priv  *regpriv =
		&(dvobj_get_primary_adapter(dvobj)->registrypriv);

#ifdef DIRTY_FOR_WORK
	dvobj->phl_com->tx_nss = hal_spec->tx_nss_num; /*GET_HAL_TX_NSS*/
	if (NSS_VALID(regpriv->tx_nss))
		dvobj->phl_com->tx_nss =
			rtw_min(dvobj->phl_com->tx_nss, regpriv->tx_nss);

	dvobj->phl_com->rx_nss = hal_spec->rx_nss_num; /*GET_HAL_RX_NSS*/
	if (NSS_VALID(regpriv->rx_nss))
		dvobj->phl_com->rx_nss =
			rtw_min(dvobj->phl_com->rx_nss, regpriv->rx_nss);

	dvobj->phl_com->rf_path_num = hal_spec->rf_reg_path_num; /*GET_HAL_RFPATH_NUM*/
	dvobj->phl_com->rf_type = RF_2T2R; /*GET_HAL_RFPATH*/

	/* GEORGIA_TODO move related control module to phl layer*/
	/* macid_ctl moved to phl */
	/* dvobj->macid_ctl.num = rtw_min(hal_spec->macid_num, MACID_NUM_SW_LIMIT); */
	// Freddie ToDo: check macid_number from PHL?
	dvobj->wow_ctl.wow_cap = hal_spec->wow_cap;

	dvobj->cam_ctl.sec_cap = hal_spec->sec_cap;
	dvobj->cam_ctl.num = rtw_min(hal_spec->sec_cam_ent_num, SEC_CAM_ENT_NUM_SW_LIMIT);
#endif
}


/*
 * _ch_offset_drv2phl() - Convert driver channel offset to PHL type
 * @ch_offset:	channel offset, ref: HAL_PRIME_CHNL_OFFSET_*
 *
 * Return PHL channel offset type "enum chan_offset"
 */
static enum chan_offset _ch_offset_drv2phl(u8 ch_offset)
{
	if (ch_offset == CHAN_OFFSET_UPPER)
		return CHAN_OFFSET_UPPER;
	if (ch_offset == CHAN_OFFSET_LOWER)
		return CHAN_OFFSET_LOWER;

	return CHAN_OFFSET_NO_EXT;
}

/*
 * rtw_hw_set_ch_bw() - Set channel, bandwidth and channel offset
 * @a:		pointer of struct _ADAPTER
 * @ch:		channel
 * @bw:		bandwidth
 * @offset:	channel offset, ref: HAL_PRIME_CHNL_OFFSET_*
 *
 * Set channel, bandwidth and channel offset.
 *
 * Return 0 for success, otherwise fail
 */
int rtw_hw_set_ch_bw(struct _ADAPTER *a, u8 ch, enum channel_width bw,
		      u8 offset, u8 do_rfk)
{
	enum rtw_phl_status status = RTW_PHL_STATUS_SUCCESS;
	struct dvobj_priv *dvobj = adapter_to_dvobj(a);
	int err = 0;
	struct rtw_chan_def chdef = {0};
	enum phl_cmd_type cmd_type = PHL_CMD_DIRECTLY;
	u32 cmd_timeout = 0;

	chdef.chan = ch;
	chdef.bw = bw;
	chdef.offset = offset;
	chdef.band = (ch > 14) ? BAND_ON_5G : BAND_ON_24G;

	_rtw_mutex_lock_interruptible(&dvobj->setch_mutex);
#ifdef DBG_CONFIG_CMD_DISP
	if (a->cmd_type == 0xFF) {
		cmd_type = PHL_CMD_DIRECTLY;
		cmd_timeout = 0;
	}
	else {
		cmd_type = a->cmd_type;
		cmd_timeout = a->cmd_timeout;
	}
#endif
	status = rtw_phl_cmd_set_ch_bw(a->phl_role,
					&chdef, do_rfk,
					cmd_type, cmd_timeout);

	if (status == RTW_PHL_STATUS_SUCCESS) {
		if (a->bNotifyChannelChange)
			RTW_INFO("[%s] ch = %d, offset = %d, bwmode = %d, success\n",
				__FUNCTION__, ch, offset, bw);

	} else {
		err = -1;
		RTW_ERR("%s: set ch(%u) bw(%u) offset(%u) FAIL!\n",
			__func__, ch, bw, offset);
	}

	_rtw_mutex_unlock(&dvobj->setch_mutex);

	return err;
}

void rtw_hw_update_chan_def(_adapter *adapter)
{
	struct mlme_ext_priv *mlmeext = &(adapter->mlmeextpriv);
	struct rtw_phl_stainfo_t *phl_sta_self = NULL;

	/*update chan_def*/
	adapter->phl_role->chandef.band =
		(mlmeext->chandef.chan > 14) ? BAND_ON_5G : BAND_ON_24G;
	adapter->phl_role->chandef.chan = mlmeext->chandef.chan;
	adapter->phl_role->chandef.bw = mlmeext->chandef.bw;
	adapter->phl_role->chandef.offset = mlmeext->chandef.offset;
	adapter->phl_role->chandef.center_ch = rtw_phl_get_center_ch(mlmeext->chandef.chan,
										mlmeext->chandef.bw, mlmeext->chandef.offset);
	/* ToDo: 80+80 BW & 160 BW */

	phl_sta_self = rtw_phl_get_stainfo_self(adapter_to_dvobj(adapter)->phl, adapter->phl_role);
	_rtw_memcpy(&phl_sta_self->chandef, &adapter->phl_role->chandef, sizeof(struct rtw_chan_def));
}

static void _dump_phl_role_info(struct rtw_wifi_role_t *wrole)
{
	RTW_INFO("[WROLE]- role-idx: %d\n", wrole->id);

	RTW_INFO("[WROLE]- type: %d\n", wrole->type);
	RTW_INFO("[WROLE]- mstate: %d\n", wrole->mstate);
	RTW_INFO("[WROLE]- mac_addr:"MAC_FMT"\n", MAC_ARG(wrole->mac_addr));
	RTW_INFO("[WROLE]- hw_band: %d\n", wrole->hw_band);
	RTW_INFO("[WROLE]- hw_port: %d\n", wrole->hw_port);
	RTW_INFO("[WROLE]- hw_wmm: %d\n", wrole->hw_wmm);

	RTW_INFO("[WROLE]- band: %d\n", wrole->chandef.band);
	RTW_INFO("[WROLE]- chan: %d\n", wrole->chandef.chan);
	RTW_INFO("[WROLE]- bw: %d\n", wrole->chandef.bw);
	RTW_INFO("[WROLE]- offset: %d\n", wrole->chandef.offset);	
	// Freddie ToDo: MBSSID
}
u8 rtw_hw_iface_init(_adapter *adapter)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	u8 phl_role_idx = INVALID_WIFI_ROLE_IDX;
	u8 rst = _FAIL;
	int chctx_num = 0;
#if defined(CONFIG_RTW_IPS) || defined(CONFIG_RTW_LPS)
	bool ps_allow = _FALSE;

	rtw_phl_ps_set_rt_cap(GET_PHL_INFO(dvobj), HW_BAND_0, ps_allow);
#endif
	// Freddie ToDo: For AP mode, net type should be set to net device already.

	/* will allocate phl self sta info */
	phl_role_idx = rtw_phl_wifi_role_alloc(GET_PHL_INFO(dvobj),
			adapter_mac_addr(adapter), PHL_RTYPE_STATION,
			adapter->iface_id, &(adapter->phl_role));

	if ((phl_role_idx == INVALID_WIFI_ROLE_IDX) ||
		(adapter->phl_role == NULL)) {
		RTW_ERR("rtw_phl_wifi_role_alloc failed\n");
		rtw_warn_on(1);
		goto _error;
	}

	/*init default value*/
	#ifdef DBG_CONFIG_CMD_DISP
	adapter->cmd_type = 0xFF;
	adapter->cmd_timeout = 0;
	#endif
	rtw_hw_update_chan_def(adapter);
	chctx_num = rtw_phl_mr_get_chanctx_num(GET_PHL_INFO(dvobj), adapter->phl_role);

	if (chctx_num == 0) {
		if (rtw_phl_cmd_set_ch_bw(adapter->phl_role,
					&(adapter->phl_role->chandef),
					_FALSE,
					PHL_CMD_WAIT, 0) != RTW_PHL_STATUS_SUCCESS) {
			RTW_ERR("%s init ch failed\n", __func__);
		}
	}

	_dump_phl_role_info(adapter->phl_role);

	/* init self staion info after wifi role alloc */
	rst = rtw_init_self_stainfo(adapter);

	#if defined (CONFIG_PCI_HCI) && defined (CONFIG_PCIE_TRX_MIT)
	rtw_pcie_trx_mit_cmd(adapter, 0, 0,
			     PCIE_RX_INT_MIT_TIMER, 0, 1);
	#endif
#if defined(CONFIG_RTW_IPS) || defined(CONFIG_RTW_LPS)
	ps_allow = _TRUE;
	rtw_phl_ps_set_rt_cap(GET_PHL_INFO(dvobj), HW_BAND_0, ps_allow);
#endif

	return rst;

_error:
	return rst;
}

u8 rtw_hw_iface_type_change(_adapter *adapter, u8 iface_type)
{
	void *phl = GET_PHL_INFO(adapter_to_dvobj(adapter));
#ifdef CONFIG_WIFI_MONITOR
	struct rtw_phl_com_t *phl_com = GET_PHL_COM(adapter_to_dvobj(adapter));
#endif
	struct rtw_wifi_role_t *wrole = adapter->phl_role;
	enum role_type rtype = PHL_RTYPE_NONE;
	enum rtw_phl_status status;
	struct sta_info *sta = NULL;

	if (wrole == NULL) {
		RTW_ERR("%s - wrole = NULL\n", __func__);
		rtw_warn_on(1);
		return _FAIL;
	}

	switch (iface_type) {
	case _HW_STATE_ADHOC_:
		rtype = PHL_RTYPE_ADHOC;
		break;
	case _HW_STATE_STATION_:
		rtype = PHL_RTYPE_STATION;
		break;
	case _HW_STATE_AP_:
		rtype = PHL_RTYPE_AP;
		break;
	case _HW_STATE_MONITOR_:
		rtype = PHL_RTYPE_MONITOR;
		break;
	case _HW_STATE_NOLINK_:
	default:
		/* TBD */
		break;
	}

	status = rtw_phl_cmd_wrole_change(phl, wrole,
				WR_CHG_TYPE, (u8*)&rtype, sizeof(enum role_type),
				PHL_CMD_DIRECTLY, 0);

	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s - change to phl role type = %d fail with error = %d\n",
			__func__, rtype, status);
		rtw_warn_on(1);
		return _FAIL;
	}

#ifdef CONFIG_WIFI_MONITOR
	if (rtype == PHL_RTYPE_MONITOR) {
		phl_com->append_fcs = false; /* This need to check again by yiwei*/
		rtw_phl_enter_mon_mode(phl, wrole);
	} else {
		phl_com->append_fcs = true; /* This need to check again by yiwei*/
		rtw_phl_leave_mon_mode(phl, wrole);
	}
#endif

	/* AP allocates self-station and changes broadcast-station before hostapd adds key */
	if (rtype == PHL_RTYPE_AP) {
		sta = rtw_get_stainfo(&adapter->stapriv, adapter_mac_addr(adapter));
		if (sta == NULL) {
			sta = rtw_alloc_stainfo(&adapter->stapriv, adapter_mac_addr(adapter));
			if (sta == NULL) {
				RTW_ERR("%s - allocate AP self-station failed\n", __func__);
				rtw_warn_on(1);
				return _FAIL;
			}
		}
	}

	RTW_INFO("%s - change to type = %d success !\n", __func__, iface_type);

	return _SUCCESS;
}

void rtw_hw_iface_deinit(_adapter *adapter)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
#if defined(CONFIG_RTW_IPS) || defined(CONFIG_RTW_LPS)
	bool ps_allow = _FALSE;

	rtw_phl_ps_set_rt_cap(GET_PHL_INFO(dvobj), HW_BAND_0, ps_allow);
#endif
	if (adapter->phl_role) {
		rtw_free_self_stainfo(adapter);
		rtw_phl_wifi_role_free(GET_PHL_INFO(dvobj), adapter->phl_role->id);
		adapter->phl_role = NULL;
	}
#if defined(CONFIG_RTW_IPS) || defined(CONFIG_RTW_LPS)
	ps_allow = _TRUE;
	rtw_phl_ps_set_rt_cap(GET_PHL_INFO(dvobj), HW_BAND_0, ps_allow);
#endif
}

/*
 * _sec_algo_drv2phl() - Convert security algorithm to PHL's definition
 * @drv_algo:		security algorithm
 * @phl_algo:		security algorithm for PHL, ref to enum rtw_enc_algo
 * @phl_key_len:	key length
 *
 * Convert driver's security algorithm defintion to PHL's type.
 *
 */
static void _sec_algo_drv2phl(enum security_type drv_algo,
			      u8 *algo, u8 *key_len)
{
	u8 phl_algo = RTW_ENC_NONE;
	u8 phl_key_len = 0;

	switch(drv_algo) {
	case _NO_PRIVACY_:
		phl_algo = RTW_ENC_NONE;
		phl_key_len = 0;
		break;
	case _WEP40_:
		phl_algo = RTW_ENC_WEP40;
		phl_key_len = 5;
		break;
	case _TKIP_:
	case _TKIP_WTMIC_:
		phl_algo = RTW_ENC_TKIP;
		phl_key_len = 16;
		break;
	case _AES_:
		phl_algo = RTW_ENC_CCMP;
		phl_key_len = 16;
		break;
	case _WEP104_:
		phl_algo = RTW_ENC_WEP104;
		phl_key_len = 13;
		break;
	case _SMS4_:
		phl_algo = RTW_ENC_WAPI;
		phl_key_len = 32;
		break;
	case _GCMP_:
		phl_algo = RTW_ENC_GCMP;
		phl_key_len = 16;
		break;
	case _CCMP_256_:
		phl_algo = RTW_ENC_CCMP256;
		phl_key_len = 32;
		break;
	case _GCMP_256_:
		phl_algo = RTW_ENC_GCMP256;
		phl_key_len = 32;
		break;
#ifdef CONFIG_IEEE80211W
	case _BIP_CMAC_128_:
		phl_algo = RTW_ENC_BIP_CCMP128;
		phl_key_len = 16;
		break;
#endif /* CONFIG_IEEE80211W */
	default:
		RTW_ERR("%s: No rule to covert drv algo(0x%x) to phl!!\n",
			__func__, drv_algo);
		phl_algo = RTW_ENC_MAX;
		phl_key_len = 0;
		break;
	}

	if(algo)
		*algo = phl_algo;
	if(key_len)
		*key_len = phl_key_len;
}

u8 rtw_sec_algo_drv2phl(enum security_type drv_algo)
{
	u8 algo = 0;

	_sec_algo_drv2phl(drv_algo, &algo, NULL);
	return algo;
}

static int rtw_hw_chk_sec_mode(struct _ADAPTER *a, struct sta_info *sta,
			enum phl_cmd_type cmd_type,  u32 cmd_timeout)
{
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	u8 sec_mode = 0;
	struct security_priv *psecuritypriv = &a->securitypriv;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	if (!phl)
		return _FAIL;

	sec_mode = rtw_phl_trans_sec_mode(
		rtw_sec_algo_drv2phl(psecuritypriv->dot11PrivacyAlgrthm),
		rtw_sec_algo_drv2phl(psecuritypriv->dot118021XGrpPrivacy));

	RTW_INFO("After phl trans_sec_mode = %d\n", sec_mode);

	if (sec_mode != sta->phl_sta->sec_mode) {
		RTW_INFO("%s: original sec_mode =%d update sec mode to %d.\n",
			__func__, sta->phl_sta->sec_mode, sec_mode);
		status = rtw_phl_cmd_change_stainfo(phl, sta->phl_sta, STA_CHG_SEC_MODE,
				&sec_mode, sizeof(u8), cmd_type, cmd_timeout);
	/* To Do: check the return status */
	} else {
		RTW_INFO("%s: sec mode remains the same. skip update.\n", __func__);
	}
	return _SUCCESS;
}

/*
 * rtw_hw_add_key() - Add security key
 * @a:		pointer of struct _ADAPTER
 * @sta:	pointer of struct sta_info
 * @keyid:	key index
 * @keyalgo:	key algorithm
 * @keytype:	0: unicast / 1: multicast / 2: bip (ref: enum SEC_CAM_KEY_TYPE)
 * @key:	key content
 * @spp:	spp mode
 *
 * Add security key.
 *
 * Return 0 for success, otherwise fail.
 */
int rtw_hw_add_key(struct _ADAPTER *a, struct sta_info *sta,
		u8 keyid, enum security_type keyalgo, u8 keytype, u8 *key,
		u8 spp, enum phl_cmd_type cmd_type,  u32 cmd_timeout)
{
	struct dvobj_priv *d;
	void *phl;
	struct phl_sec_param_h crypt = {0};
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	if (rtw_hw_chk_sec_mode(a, sta, cmd_type, cmd_timeout) == _FAIL)
		return -1;

	crypt.keyid = keyid;
	crypt.key_type= keytype;
	crypt.spp = spp;
	_sec_algo_drv2phl(keyalgo, &crypt.enc_type, &crypt.key_len);

	/* delete key before adding key */
	rtw_phl_cmd_del_key(phl, sta->phl_sta, &crypt, cmd_type, cmd_timeout);
	status = rtw_phl_cmd_add_key(phl, sta->phl_sta, &crypt, key, cmd_type, cmd_timeout);
	if (status != RTW_PHL_STATUS_SUCCESS)
		return -1;

	return 0;
}

/*
 * rtw_hw_del_key() - Delete security key
 * @a:		pointer of struct _ADAPTER
 * @sta:	pointer of struct sta_info
 * @keyid:	key index
 * @keytype:	0: unicast / 1: multicast / 2: bip (ref: enum SEC_CAM_KEY_TYPE)
 *
 * Delete security key by macid, keyid and keytype.
 *
 * Return 0 for success, otherwise fail.
 */
int rtw_hw_del_key(struct _ADAPTER *a, struct sta_info *sta,
		u8 keyid, u8 keytype, enum phl_cmd_type cmd_type, u32 cmd_timeout)
{
	struct dvobj_priv *d;
	void *phl;
	struct phl_sec_param_h crypt = {0};
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	crypt.keyid = keyid;
	crypt.key_type= keytype;

	status = rtw_phl_cmd_del_key(phl, sta->phl_sta, &crypt, cmd_type, cmd_timeout);
	if (status != RTW_PHL_STATUS_SUCCESS)
		return -1;

	return 0;
}

/*
 * rtw_hw_del_all_key() - Delete all security key for this STA
 * @a:		pointer of struct _ADAPTER
 * @sta:	pointer of struct sta_info
 *
 * Delete all security keys belong to this STA.
 *
 * Return 0 for success, otherwise fail.
 */
int rtw_hw_del_all_key(struct _ADAPTER *a, struct sta_info *sta,
			enum phl_cmd_type cmd_type, u32 cmd_timeout)
{
	struct dvobj_priv *d;
	void *phl;
	u8 keyid;
	u8 keytype;
	struct phl_sec_param_h crypt = {0};
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	/* Delete Group and Pairwise key */
	for (keytype = 0; keytype < 2; keytype++) {
		for (keyid = 0; keyid < 4; keyid++) {
			crypt.keyid = keyid;
			crypt.key_type = keytype;
			rtw_phl_cmd_del_key(phl, sta->phl_sta, &crypt, cmd_type, cmd_timeout);
		}
	}

	/* Delete BIP key */
	crypt.key_type = 2;
	for (keyid = 4; keyid <= BIP_MAX_KEYID; keyid++) {
		crypt.keyid = keyid;
		rtw_phl_cmd_del_key(phl, sta->phl_sta, &crypt, cmd_type, cmd_timeout);
	}

	return 0;
}

int rtw_hw_start_bss_network(struct _ADAPTER *a)
{
	/* some hw related ap settings */
	if (rtw_phl_ap_started(adapter_to_dvobj(a)->phl, a->phl_role) !=
		RTW_PHL_STATUS_SUCCESS)
		return _FAIL;

	return _SUCCESS;
}

/* connect */
int rtw_hw_prepare_connect(struct _ADAPTER *a, struct sta_info *sta, u8 *target_addr)
{
	/*adapter->phl_role.mac_addr*/
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	status = rtw_phl_connect_prepare(phl, a->phl_role, target_addr);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s: Fail to setup hardware for connecting!(%d)\n",
			__func__, status);
		return -1;
	}
	/* Todo: Enable TSF update */
	/* Todo: Set support short preamble or not by beacon capability */
	/* Todo: Set slot time */

	return 0;
}

/* Handle connect fail case */
int rtw_hw_connect_abort(struct _ADAPTER *a, struct sta_info *sta)
{
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	rtw_hw_del_all_key(a, sta, PHL_CMD_DIRECTLY, 0);

	status = rtw_phl_cmd_update_media_status(phl, sta->phl_sta, NULL, false,
							PHL_CMD_DIRECTLY, 0);
	if (status != RTW_PHL_STATUS_SUCCESS)
		return -1;

#ifndef CONFIG_STA_CMD_DISPR
	/*
	 * In CONFIG_STA_CMD_DISPR case, connect abort hw setting has been moved
	 * to MSG_EVT_DISCONNECT@PHL_FG_MDL_CONNECT .
	 */

	/* disconnect hw setting */
	rtw_phl_disconnect(phl, a->phl_role);

	/* delete sta channel ctx */
	rtw_phl_chanctx_del(adapter_to_dvobj(a)->phl, a->phl_role, NULL);
	/* restore orig union ch */
	rtw_join_done_chk_ch(a, -1);

	/* free connecting AP sta info */
	rtw_free_stainfo(a, sta);
	rtw_init_self_stainfo(a);
#endif /* !CONFIG_STA_CMD_DISPR */

	return 0;
}

#ifdef CONFIG_80211N_HT
#ifdef CONFIG_80211AC_VHT
static void update_phl_sta_cap_vht(struct _ADAPTER *a, struct sta_info *sta,
			           struct protocol_cap_t *cap)
{
	struct vht_priv *vht;


	vht = &sta->vhtpriv;

	if (cap->ampdu_len_exp < vht->ampdu_len)
		cap->ampdu_len_exp = vht->ampdu_len;
	if (cap->max_amsdu_len < vht->max_mpdu_len)
		cap->max_amsdu_len = vht->max_mpdu_len;

	cap->sgi_80 = (vht->sgi_80m == _TRUE) ? 1 : 0;

	cap->vht_ldpc = vht->ldpc_cap ? 1 : 0;

	_rtw_memcpy(cap->vht_rx_mcs, vht->vht_mcs_map, 2);
	/* Todo: cap->vht_tx_mcs[2]; */
	if (vht->op_present)
		_rtw_memcpy(cap->vht_basic_mcs, &vht->vht_op[3], 2);
}
#endif /* CONFIG_80211AC_VHT */
static void update_phl_sta_cap_ht(struct _ADAPTER *a, struct sta_info *sta,
			          struct protocol_cap_t *cap)
{
	struct mlme_ext_info *info;
	struct ht_priv *ht;


	info = &a->mlmeextpriv.mlmext_info;
	ht = &sta->htpriv;

	cap->num_ampdu = 0xFF;	/* Set to MAX */

	cap->ampdu_density = ht->rx_ampdu_min_spacing;
	cap->ampdu_len_exp = GET_HT_CAP_ELE_MAX_AMPDU_LEN_EXP(&ht->ht_cap);
	cap->amsdu_in_ampdu = 1;
	cap->max_amsdu_len = GET_HT_CAP_ELE_MAX_AMSDU_LENGTH(&ht->ht_cap);

	/*GET_HT_CAP_ELE_SM_PS(&info->HT_caps.u.HT_cap_element.HT_caps_info);*/
	cap->sm_ps = info->SM_PS;

	cap->sgi_20 = (ht->sgi_20m == _TRUE) ? 1 : 0;
	cap->sgi_40 = (ht->sgi_40m == _TRUE) ? 1 : 0;

	cap->ht_ldpc = ht->ldpc_cap ? 1 : 0;

	_rtw_memcpy(cap->ht_rx_mcs, ht->ht_cap.supp_mcs_set, 4);
	/* Todo: cap->ht_tx_mcs[4]; */
	if (info->HT_info_enable)
		_rtw_memcpy(cap->ht_basic_mcs, info->HT_info.MCS_rate, 4);
}
#endif /* CONFIG_80211N_HT */

void rtw_update_phl_sta_cap(struct _ADAPTER *a, struct sta_info *sta,
			    struct protocol_cap_t *cap)
{
	struct mlme_ext_info *info;


	info = &a->mlmeextpriv.mlmext_info;

	/* MAC related */
	/* update beacon interval */
	cap->bcn_interval = info->bcn_interval;
#if 0
	cap->num_ampdu;		/* HT, VHT, HE */
	cap->ampdu_density:3;	/* HT, VHT, HE */
	cap->ampdu_len_exp;	/* HT, VHT, HE */
	cap->amsdu_in_ampdu:1;	/* HT, VHT, HE */
	cap->max_amsdu_len:2;	/* HT, VHT, HE */
	cap->htc_rx:1;
	cap->sm_ps:2;		/* HT */
	cap->trig_padding:2;
	cap->twt:6;
	cap->all_ack:1;
	cap->a_ctrl:3;
	cap->ops:1;
	cap->ht_vht_trig_rx:1;
#endif
	cap->short_slot = (info->slotTime == SHORT_SLOT_TIME) ? 1 : 0;
	cap->preamble = (info->preamble_mode == PREAMBLE_SHORT) ? 1 : 0;
#if 0
	cap->sgi_20:1;		/* HT */
	cap->sgi_40:1;		/* HT */
	cap->sgi_80:1;		/* VHT */
	cap->sgi_160:1		/* VHT, HE */

	/* BB related */
	cap->ht_ldpc:1;		/* HT */
	cap->vht_ldpc:1;	/* VHT */
	cap->he_ldpc:1;		/* HE */
	cap->sgi:1;
	cap->su_bfmr:1;
	cap->su_bfme:1;
	cap->mu_bfmr:1;
	cap->mu_bfme:1;
	cap->bfme_sts:3;
	cap->num_snd_dim:3;
#endif
	_rtw_memset(cap->supported_rates, 0, 12);
	_rtw_memcpy(cap->supported_rates, sta->bssrateset,
		    sta->bssratelen < 12 ? sta->bssratelen : 12);
#if 0
	cap->ht_rx_mcs[4];	/* HT */
	cap->ht_tx_mcs[4];	/* HT */
	cap->ht_basic_mcs[4];	/* Basic rate of HT */
	cap->vht_rx_mcs[2];	/* VHT */
	cap->vht_tx_mcs[2];	/* VHT */
	cap->vht_basic_mcs[2];	/* Basic rate of VHT */
#endif
#if 0
	/* HE done */
	cap->he_rx_mcs[2];
	cap->he_tx_mcs[2];
	cap->he_basic_mcs[2];	/* Basic rate of HE */
	cap->stbc_tx:1;
	cap->stbc_rx:3;
	cap->ltf_gi;
	cap->doppler_tx:1;
	cap->doppler_rx:1;
	cap->dcm_max_const_tx:2;
	cap->dcm_max_nss_tx:1;
	cap->dcm_max_const_rx:2;
	cap->dcm_max_nss_rx:1;
	cap->partial_bw_su_in_mu:1;
	cap->bfme_sts_greater_80mhz:3;
	cap->num_snd_dim_greater_80mhz:3;
	cap->stbc_tx_greater_80mhz:1;
	cap->stbc_rx_greater_80mhz:1;
	cap->ng_16_su_fb:1;
	cap->ng_16_mu_fb:1;
	cap->cb_sz_su_fb:1;
	cap->cb_sz_mu_fb:1;
	cap->trig_su_bfm_fb:1;
	cap->trig_mu_bfm_fb:1;
	cap->trig_cqi_fb:1;
	cap->partial_bw_su_er:1;
	cap->pkt_padding:2;
	cap->ppe_th[24];
	cap->pwr_bst_factor:1;
	cap->max_nc:3;
	cap->dcm_max_ru:2;
	cap->long_sigb_symbol:1;
	cap->non_trig_cqi_fb:1;
	cap->tx_1024q_ru:1;
	cap->rx_1024q_ru:1;
	cap->fbw_su_using_mu_cmprs_sigb:1;
	cap->fbw_su_using_mu_non_cmprs_sigb:1;
	cap->er_su:1;
	cap->tb_pe:3;
	cap->txop_du_rts_th;
#endif

#ifdef CONFIG_80211N_HT
	if (sta->htpriv.ht_option) {
		update_phl_sta_cap_ht(a, sta, cap);
#ifdef CONFIG_80211AC_VHT
		if (sta->vhtpriv.vht_option)
			update_phl_sta_cap_vht(a, sta, cap);;
#endif /* CONFIG_80211AC_VHT */
	}
#endif /* CONFIG_80211N_HT */
}

/**
 * rtw_hw_set_edca() - setup WMM EDCA parameter
 * @a:		struct _ADAPTER *
 * @ac:		Access Category, 0:BE, 1:BK, 2:VI, 3:VO
 * @param:	AIFS:BIT[7:0], CWMIN:BIT[11:8], CWMAX:BIT[15:12],
 *		TXOP:BIT[31:16]
 *
 * Setup WMM EDCA parameter set.
 *
 * Return 0 for SUCCESS, otherwise fail.
 */
int rtw_hw_set_edca(struct _ADAPTER *a, u8 ac, u32 param)
{
	struct dvobj_priv *d;
	void *phl;
	struct rtw_edca_param edca = {0};
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	edca.ac = ac;
	edca.param = param;

	status = rtw_phl_cmd_wrole_change(phl, a->phl_role,
				WR_CHG_EDCA_PARAM, (u8*)&edca, sizeof(struct rtw_edca_param),
				PHL_CMD_DIRECTLY, 0);

	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s: fail to set edca parameter, ac(%u), "
			"param(0x%08x)\n",
			__func__, ac, param);
		return -1;
	}

	return 0;
}

int rtw_hw_connected(struct _ADAPTER *a, struct sta_info *sta)
{

	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	struct security_priv *psecuritypriv = &a->securitypriv;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	rtw_update_phl_sta_cap(a, sta, &sta->phl_sta->asoc_cap);

	status = rtw_phl_cmd_update_media_status(phl, sta->phl_sta,
					sta->phl_sta->mac_addr, true,
					PHL_CMD_DIRECTLY, 0);
	if (status != RTW_PHL_STATUS_SUCCESS)
		return -1;
	rtw_dump_phl_sta_info(RTW_DBGDUMP, sta);

	/* Todo: update IOT-releated issue */
#if 0
	update_IOT_info(a);
#endif
	/* Todo: RTS full bandwidth setting */
#if 0
#ifdef CONFIG_RTS_FULL_BW
	rtw_set_rts_bw(a);
#endif /* CONFIG_RTS_FULL_BW */
#endif
	/* Todo: Basic rate setting */
#if 0
	rtw_hal_set_hwreg(a, HW_VAR_BASIC_RATE, cur_network->SupportedRates);
#endif
	/* Todo: udpate capability: short preamble, slot time */
	update_capinfo(a, a->mlmeextpriv.mlmext_info.capability);

	WMMOnAssocRsp(a);

	/* Todo: HT: AMPDU factor, min space, max time and related parameters */
#if 0
#ifdef CONFIG_80211N_HT
	HTOnAssocRsp(a);
#endif /* CONFIG_80211N_HT */
#endif
	/* Todo: VHT */
#if 0
#ifdef CONFIG_80211AC_VHT
	VHTOnAssocRsp(a);
#endif
#endif
	/* Todo: Set Data rate and RA */
#if 0
	set_sta_rate(a, psta);
#endif
	/* Todo: Firmware media status report */
#if 0
	rtw_sta_media_status_rpt(a, psta, 1);
#endif
	/* Todo: IC specific hardware setting */
#if 0
	join_type = 2;
	rtw_hal_set_hwreg(a, HW_VAR_MLME_JOIN, (u8 *)(&join_type));
#endif
	if ((a->mlmeextpriv.mlmext_info.state & 0x03) == WIFI_FW_STATION_STATE) {
		/* Todo: Correct TSF */
#if 0
		correct_TSF(a, MLME_STA_CONNECTED);
#endif
	}

	/* Todo: btcoex connect event notify */
#if 0
	rtw_btcoex_connect_notify(a, join_type);
#endif
	/* Todo: Beamforming setting */
#if 0
	beamforming_wk_cmd(a, BEAMFORMING_CTRL_ENTER, (u8 *)psta, sizeof(struct sta_info), 0);
#endif

	rtw_join_done_chk_ch(a, 1);
	rtw_phl_connected(phl, a->phl_role, sta->phl_sta);
	ATOMIC_SET(&a->need_tsf_sync_done, _TRUE);
	return 0;
}

int rtw_hw_disconnect(struct _ADAPTER *a, struct sta_info *sta)
{
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	int tid;
	u8 is_ap_self = _FALSE;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	if (MLME_IS_AP(a) &&
		_rtw_memcmp(a->phl_role->mac_addr, sta->phl_sta->mac_addr, ETH_ALEN))
		is_ap_self = _TRUE;

	/* Check and reset setting related to rx ampdu resources of PHL. */
	for (tid = 0; tid < TID_NUM; tid++) {
		if(sta->recvreorder_ctrl[tid].enable == _TRUE) {
			sta->recvreorder_ctrl[tid].enable =_FALSE;
			rtw_phl_stop_rx_ba_session(phl, sta->phl_sta, tid);
			RTW_INFO(FUNC_ADPT_FMT"stop process tid %d \n",
				FUNC_ADPT_ARG(a), tid);
		}
	}

	/*reset sec setting and clean all connection setting*/
	rtw_hw_del_all_key(a, sta, PHL_CMD_DIRECTLY, 0);

	if (is_ap_self == _FALSE) {
		status = rtw_phl_cmd_update_media_status(phl, sta->phl_sta, NULL, false,
						PHL_CMD_DIRECTLY, 0);
		if (status != RTW_PHL_STATUS_SUCCESS)
			return -1;

		rtw_dump_phl_sta_info(RTW_DBGDUMP, sta);
	}

	if (MLME_IS_STA(a)) {
		/*
		 * the following flow only for STA
		 * bypass client disconnect from softAP
		 */
#ifndef CONFIG_STA_CMD_DISPR
		rtw_phl_disconnect(phl, a->phl_role);
#endif /* !CONFIG_STA_CMD_DISPR */
		rtw_disconnect_ch_switch(a);
	}

	return 0;
}

int rtw_hw_connected_apmode(struct _ADAPTER *a, struct sta_info *sta)
{
	struct dvobj_priv *d;
	void *phl;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	if (!phl)
		return -1;

	rtw_ap_set_sta_wmode(a, sta);
	update_sta_ra_info(a, sta);
	rtw_update_phl_sta_cap(a, sta, &sta->phl_sta->asoc_cap);

	if (RTW_PHL_STATUS_SUCCESS != rtw_phl_cmd_update_media_status(
		phl, sta->phl_sta, sta->phl_sta->mac_addr, true,
		PHL_CMD_DIRECTLY, 0))
		return -1;

	rtw_dump_phl_sta_info(RTW_DBGDUMP, sta);

	return 0;
}

u8 rtw_hal_get_def_var(struct _ADAPTER *a,
		       enum _HAL_DEF_VARIABLE def_var, void *val)
{
	switch (def_var) {
	case HAL_DEF_IS_SUPPORT_ANT_DIV:
		*(u8*)val = _FALSE;
		break;
	case HAL_DEF_DBG_DUMP_RXPKT:
		*(u8*)val = 0;
		break;
	case HAL_DEF_BEAMFORMER_CAP:
		*(u8*)val = a->phl_role->proto_role_cap.num_snd_dim;
		break;
	case HAL_DEF_BEAMFORMEE_CAP:
		*(u8*)val = a->phl_role->proto_role_cap.bfme_sts;
		break;
	case HW_VAR_MAX_RX_AMPDU_FACTOR:
		/* HT only */
		*(enum _HT_CAP_AMPDU_FACTOR*)val = MAX_AMPDU_FACTOR_64K;
		break;
	case HW_DEF_RA_INFO_DUMP:
		/* do nothing */
		break;
	case HAL_DEF_DBG_DUMP_TXPKT:
		*(u8*)val = 0;
		break;
	case HAL_DEF_TX_PAGE_SIZE:
		/* would be removed later */
		break;
	case HW_VAR_BEST_AMPDU_DENSITY:
		*(u8*)val = 0;
		break;
	default:
		break;
	}

	return 0;
}
#ifdef CONFIG_RTW_ACS
u16 rtw_acs_get_channel_by_idx(struct _ADAPTER *a, u8 idx)
{
	struct dvobj_priv *d = adapter_to_dvobj(a);
	void *phl = GET_PHL_INFO(d);

	if (phl)
		return rtw_phl_acs_get_channel_by_idx(phl, idx);
	else
		return 0;
}

u8 rtw_acs_get_clm_ratio_by_idx(struct _ADAPTER *a, u8 idx)
{
	struct dvobj_priv *d = adapter_to_dvobj(a);
	void *phl = GET_PHL_INFO(d);

	if (phl)
		return rtw_phl_acs_get_clm_ratio_by_idx(phl, idx);
	else
		return 0;
}

s8 rtw_noise_query_by_idx(struct _ADAPTER *a, u8 idx)
{
	struct dvobj_priv *d = adapter_to_dvobj(a);
	void *phl = GET_PHL_INFO(d);

	if (phl)
		return rtw_phl_noise_query_by_idx(phl, idx);
	else
		return 0;
}
#endif /* CONFIG_RTW_ACS */

void rtw_dump_env_rpt(struct _ADAPTER *a, void *sel)
{
	struct dvobj_priv *d = adapter_to_dvobj(a);
	struct rtw_phl_com_t *phl_com = GET_PHL_COM(d);
	void *phl = GET_PHL_INFO(d);
	struct rtw_env_report rpt;

	rtw_phl_get_env_rpt(phl, &rpt, a->phl_role);

	RTW_PRINT_SEL(sel, "clm_ratio:%d (%%)\n", rpt.nhm_cca_ratio);
	RTW_PRINT_SEL(sel, "nhm_ratio:%d (%%)\n", rpt.nhm_ratio);
}

#ifdef CONFIG_WOWLAN
static u8 _cfg_keep_alive_info(struct _ADAPTER *a, u8 enable)
{
	struct rtw_keep_alive_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	u8 check_period = 5;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	_rtw_memset(&info, 0, sizeof(struct rtw_keep_alive_info));

	info.keep_alive_en = enable;
	info.keep_alive_period = check_period;

	RTW_INFO("%s: keep_alive_en=%d, keep_alive_period=%d\n",
				__func__, info.keep_alive_en, info.keep_alive_period);

	status = rtw_phl_cfg_keep_alive_info(phl, &info);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_INFO("%s fail(%d)\n", __func__, status);
		return _FAIL;
	}

	return _SUCCESS;
}

static u8 _cfg_disc_det_info(struct _ADAPTER *a, u8 enable)
{
	struct rtw_disc_det_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	struct registry_priv *registry;
	u8 check_period = 100, trypkt_num = 5;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	registry = &a->registrypriv;

	_rtw_memset(&info, 0, sizeof(struct rtw_disc_det_info));

	info.disc_det_en = enable;

	/* wake up event includes deauth wake up */
	if (registry->wakeup_event & BIT(2))
		info.disc_wake_en = _TRUE;

	info.try_pkt_count = trypkt_num;
	info.check_period = check_period;

	info.cnt_bcn_lost_en = 0;
	info.cnt_bcn_lost_limit = 0;

	status = rtw_phl_cfg_disc_det_info(phl, &info);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_INFO("%s fail(%d)\n", __func__, status);
		return _FAIL;
	}

	return _SUCCESS;
}

static u8 _cfg_nlo_info(struct _ADAPTER *a)
{
	struct rtw_nlo_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	_rtw_memset(&info, 0, sizeof(struct rtw_nlo_info));
	status = rtw_phl_cfg_nlo_info(phl, &info);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_INFO("%s fail(%d)\n", __func__, status);
		return _FAIL;
	}

	return _SUCCESS;
}

static u8 _cfg_arp_ofld_info(struct _ADAPTER *a)
{
	struct rtw_arp_ofld_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;
	struct security_priv *secpriv;
	u8 arp_en;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);
	secpriv = &a->securitypriv;

	_rtw_memset(&info, 0, sizeof(struct rtw_arp_ofld_info));

#ifdef CONFIG_ARP_KEEP_ALIVE
	if ((secpriv->dot11PrivacyAlgrthm == _WEP40_) ||
		(secpriv->dot11PrivacyAlgrthm == _WEP104_))
		arp_en = 0;
	else
		arp_en = 1;
#else
	arp_en = 0;
#endif /* CONFIG_ARP_KEEP_ALIVE */

	info.arp_en = arp_en;

	if (info.arp_en) {
	} else {
	}

	rtw_phl_cfg_arp_ofld_info(phl, &info);

	return _SUCCESS;
}

static u8 _cfg_ndp_ofld_info(struct _ADAPTER *a)
{
	struct rtw_ndp_ofld_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	_rtw_memset(&info, 0, sizeof(struct rtw_ndp_ofld_info));

	rtw_phl_cfg_ndp_ofld_info(phl, &info);

	return _SUCCESS;
}

static u8 _cfg_gtk_ofld_info(struct _ADAPTER *a)
{
	struct rtw_gtk_ofld_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	_rtw_memset(&info, 0, sizeof(struct rtw_gtk_ofld_info));
	rtw_phl_cfg_gtk_ofld_info(phl, &info);

	return _SUCCESS;
}

static u8 _cfg_realwow_info(struct _ADAPTER *a)
{
	struct rtw_realwow_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	/* default disable */
	_rtw_memset(&info, 0, sizeof(struct rtw_realwow_info));
	status = rtw_phl_cfg_realwow_info(phl, &info);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_INFO("%s fail(%d)\n", __func__, status);
		return _FAIL;
	}

	return _SUCCESS;
}

static u8 _cfg_wow_wake(struct _ADAPTER *a, u8 wow_en)
{
	struct rtw_wow_wake_info info;
	struct dvobj_priv *d;
	void *phl;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	_rtw_memset(&info, 0, sizeof(struct rtw_wow_wake_info));

	info.wow_en = 1;
	info.pattern_match_en = 1;
	info.magic_pkt_en = 1;

	status = rtw_phl_cfg_wow_wake(phl, &info);
	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_INFO("%s fail(%d)\n", __func__, status);
		return _FAIL;
	}

	return _SUCCESS;
}

static u8 _wow_cfg(struct _ADAPTER *a, u8 wow_en)
{
	struct dvobj_priv *d;
	void *phl;
	struct rtw_phl_stainfo_t *phl_sta;
	enum rtw_phl_status status;

	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	if (!_cfg_keep_alive_info(a, wow_en))
		return _FAIL;

	if(!_cfg_disc_det_info(a, wow_en))
		return _FAIL;

	if (!_cfg_nlo_info(a))
		return _FAIL;

	if (!_cfg_arp_ofld_info(a))
		return _FAIL;

	if (!_cfg_ndp_ofld_info(a))
		return _FAIL;

	if (!_cfg_gtk_ofld_info(a))
		return _FAIL;

	if (!_cfg_realwow_info(a))
		return _FAIL;

	if (!_cfg_wow_wake(a, wow_en))
		return _FAIL;

	return _SUCCESS;
}

u8 rtw_hw_wow(struct _ADAPTER *a, u8 wow_en)
{
	struct dvobj_priv *d;
	void *phl;
	struct rtw_phl_stainfo_t *phl_sta;
	enum rtw_phl_status status;


	d = adapter_to_dvobj(a);
	phl = GET_PHL_INFO(d);

	rtw_wow_lps_level_decide(a, _TRUE);


	if (!_wow_cfg(a, wow_en))
		return _FAIL;

	phl_sta = rtw_phl_get_stainfo_by_addr(phl, a->phl_role, get_bssid(&a->mlmepriv));

	if (wow_en)
		status = rtw_phl_suspend(phl, phl_sta, wow_en);
	else
		status = rtw_phl_resume(phl, phl_sta, &wow_en);

	if (status != RTW_PHL_STATUS_SUCCESS) {
		RTW_ERR("%s wow %s fail(status: %d)\n", __func__, wow_en ? "enable" : "disable", status);
		return _FAIL;
	}

	return _SUCCESS;
}
#endif
