/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
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
#ifndef _RTW_MBO_H_
#define _RTW_MBO_H_

#define MBO_CH_LIST_MAX_NUM 247
#define MBO_OCE_ELEMENT_MAX_LEN 255

#define MBO_CH_PREFER_NON_OP 0
#define MBO_CH_PREFER_NOT 1
#define MBO_CH_PREFER_OK 255

#define rtw_mbo_wifi_logo_test(a)	((a->registrypriv.wifi_spec) == 1)

#define rtw_mbo_set_ext_cap_internw(_pEleStart, _val) \
	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart))+3, 7, 1, _val)

#define rtw_mbo_wnm_notification_req(c, a) \
	(((c) == RTW_WLAN_CATEGORY_WNM) &&	\
	(((a) == RTW_WLAN_ACTION_WNM_NOTIF_REQ)))

/* IEEE Std 802.11-2016 Table 9-46 - Status codes */
#define RTW_ASSOC_DENIED_NO_MORE_STAS	17
#define RTW_ASSOC_REFUSED_TEMPORARILY	30

/* MBO-OCE Information Element */
#define RTW_MBO_EID		WLAN_EID_VENDOR_SPECIFIC
#define RTW_MBO_OUI		0x506F9A
#define RTW_MBO_OUI_TYPE	0x16

/* Non-preferred Channel Report */
#define RTW_MBO_ATTR_NPREF_CH_RPT_ID	0x2
/* Cellular Data Capabilities */
#define RTW_MBO_ATTR_CELL_DATA_CAP_ID	0x3
/* Association Disallowed */
#define RTW_MBO_ATTR_ASSOC_DISABLED_ID	0x4
/* Transition Reason Code */
#define RTW_MBO_ATTR_TRANS_RES_ID		0x6
/* Transition Rejection Reason Code */
#define RTW_MBO_ATTR_TRANS_REJ_ID		0x7
/* Association Retry Delay */
#define RTW_MBO_ATTR_TASSOC_RETRY_ID	0x8

#define RTW_MBO_MAX_CH_LIST_NUM MAX_CHANNEL_NUM

#define RTW_MBO_MAX_CH_RPT_NUM 32

enum rtw_mbo_attri_type {
	MBO_AP_CAPABILITY = 1,
	NON_PREFER_CHANNEL_RPT = 2,
	CELLULAR_DATA_CAPABILITY = 3,
	ASSOCIATION_DISALLOW = 4,
	CELLULAR_DATA_CONNECT_PREFER = 5,
	TRANS_REASON_CODE = 6,
	TRANS_REJECT_REASON_CODE = 7,
	ASSOCIATION_RETRY_DELAY = 8
};

struct rtw_mbo_ch_list {
	u8 op_class;
	u8 channel;
	u8 preference;
};

struct mbo_priv {
	u8 enable;
	u8 assoc_disallow;
	u8 cellular_aware;
	struct rtw_mbo_ch_list ch_list[MBO_CH_LIST_MAX_NUM];
	u8 ch_list_num;
	u8 mbo_oce_element[MBO_OCE_ELEMENT_MAX_LEN];
	u8 mbo_oce_element_len;
};


struct npref_ch {
	u8 op_class;
	u8 chs[RTW_MBO_MAX_CH_LIST_NUM];
	size_t nm_of_ch;
	u8 preference;
	u8 reason;
};

struct npref_ch_rtp {
	struct npref_ch ch_rpt[RTW_MBO_MAX_CH_RPT_NUM];
	size_t nm_of_rpt;
};


void rtw_mbo_ie_handler(_adapter *padapter, struct mbo_priv *mbopriv,
						const u8 *pbuf, uint limit_len);
struct sta_info;
void rtw_ap_parse_sta_mbo_element(_adapter *padapter,
						struct sta_info *psta, u8 *ies_buf, u16 ies_len);
void rtw_mbo_fill_non_prefer_channel_list(_adapter *padapter, struct mbo_priv *mbopriv,
											const u8 *pbuf, u8 len);



void rtw_mbo_build_cell_data_cap_attr(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

void rtw_mbo_update_ie_data(
	_adapter *padapter, u8 *pie, u32 ie_len);

void rtw_mbo_build_supp_op_class_elem(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

void rtw_mbo_build_npref_ch_rpt_attr(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

void rtw_mbo_build_trans_reject_reason_attr(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib, u8 *pres);

u8 rtw_mbo_disallowed_network(struct wlan_network *pnetwork);

void rtw_mbo_build_exented_cap(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

ssize_t rtw_mbo_proc_non_pref_chans_set(
	struct file *pfile, const char __user *buffer,
	size_t count, loff_t *pos, void *pdata);

int rtw_mbo_proc_non_pref_chans_get(
	struct seq_file *m, void *v);

ssize_t rtw_mbo_proc_cell_data_set(
	struct file *pfile, const char __user *buffer,
	size_t count, loff_t *pos, void *pdata);

void rtw_mbo_wnm_notification_parsing(
	_adapter *padapter, const u8 *pdata, size_t data_len);

void rtw_mbo_build_wnm_notification(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

void rtw_mbo_build_probe_req_ies(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

void rtw_mbo_build_assoc_req_ies(
	_adapter *padapter, u8 **pframe, struct pkt_attrib *pattrib);

#endif /* _RTW_MBO_H_ */

