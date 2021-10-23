/******************************************************************************
 *
 * Copyright(c) 2007 - 2019 Realtek Corporation.
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
#define _RTW_WOW_C_

#include <drv_types.h>

#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
	#include <linux/inetdevice.h>
	#define ETH_TYPE_OFFSET	12
	#define PROTOCOL_OFFSET	23
	#define IP_OFFSET	30
	#define IPv6_OFFSET	38
	#define IPv6_PROTOCOL_OFFSET	20
#endif

#ifdef CONFIG_WOWLAN
void rtw_wowlan_set_pattern_cast_type(_adapter *adapter, struct rtw_wowcam_upd_info *wowcam_info)
{
	if (is_broadcast_mac_addr(wowcam_info->ptrn))
		wowcam_info->bc = 1;
	else if (is_multicast_mac_addr(wowcam_info->ptrn))
		wowcam_info->mc = 1;
	else if (!memcmp(wowcam_info->ptrn, adapter_mac_addr(adapter), ETH_ALEN))
		wowcam_info->uc = 1;
}

inline bool _rtw_wow_chk_cap(_adapter *adapter, u8 cap)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct wow_ctl_t *wow_ctl = &dvobj->wow_ctl;

	if (wow_ctl->wow_cap & cap)
		return _TRUE;
	return _FALSE;
}
bool rtw_wowlan_parser_pattern_cmd(u8 *input, char *pattern,
				   int *pattern_len, char *bit_mask)
{
	char *cp = NULL;
	size_t len = 0;
	int pos = 0, mask_pos = 0, res = 0;
	u8 member[2] = {0};

	/* To get the pattern string after "=", when we use :
	 * iwpriv wlanX pattern=XX:XX:..:XX
	 */
	cp = strchr(input, '=');
	if (cp) {
		*cp = 0;
		cp++;
		input = cp;
	}

	/* To take off the newline character '\n'(0x0a) at the end of pattern string,
	 * when we use echo xxxx > /proc/xxxx
	 */
	cp = strchr(input, '\n');
	if (cp)
		*cp = 0;

	while (input) {
		cp = strsep((char **)(&input), ":");

		if (bit_mask && (strcmp(cp, "-") == 0 ||
				 strcmp(cp, "xx") == 0 ||
				 strcmp(cp, "--") == 0)) {
			/* skip this byte and leave mask bit unset */
		} else {
			u8 hex;

			if (strlen(cp) != 2) {
				RTW_ERR("%s:[ERROR] hex len != 2, input=[%s]\n",
					__func__, cp);
				goto error;
			}

			if (hexstr2bin(cp, &hex, 1) < 0) {
				RTW_ERR("%s:[ERROR] pattern is invalid, input=[%s]\n",
					__func__, cp);
				goto error;
			}

			pattern[pos] = hex;
			mask_pos = pos / 8;
			if (bit_mask)
				bit_mask[mask_pos] |= 1 << (pos % 8);
		}

		pos++;
	}

	(*pattern_len) = pos;

	return _TRUE;
error:
	return _FALSE;
}

void rtw_wow_pattern_sw_reset(_adapter *adapter)
{
	int i;
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(adapter);

	if (pwrctrlpriv->default_patterns_en == _TRUE)
		pwrctrlpriv->wowlan_pattern_idx = DEFAULT_PATTERN_NUM;
	else
		pwrctrlpriv->wowlan_pattern_idx = 0;

	for (i = 0 ; i < MAX_WKFM_CAM_NUM; i++) {
		_rtw_memset(pwrctrlpriv->patterns[i].content, '\0', sizeof(pwrctrlpriv->patterns[i].content));
		_rtw_memset(pwrctrlpriv->patterns[i].mask, '\0', sizeof(pwrctrlpriv->patterns[i].mask));
		pwrctrlpriv->patterns[i].len = 0;
	}
}

u8 rtw_set_default_pattern(_adapter *adapter)
{
	struct pwrctrl_priv *pwrpriv = adapter_to_pwrctl(adapter);
	struct mlme_ext_priv *pmlmeext = &adapter->mlmeextpriv;
	struct mlme_ext_info *pmlmeinfo = &pmlmeext->mlmext_info;
	u8 index = 0;
	u8 multicast_addr[3] = {0x01, 0x00, 0x5e};
	u8 multicast_ip[4] = {0xe0, 0x28, 0x28, 0x2a};

	u8 unicast_mask[5] = {0x3f, 0x70, 0x80, 0xc0, 0x03};
	u8 icmpv6_mask[7] = {0x00, 0x70, 0x10, 0x00, 0xc0, 0xc0, 0x3f};
	u8 multicast_mask[5] = {0x07, 0x70, 0x80, 0xc0, 0x03};

	u8 ip_protocol[3] = {0x08, 0x00, 0x45};
	u8 ipv6_protocol[3] = {0x86, 0xdd, 0x60};

	u8 *target = NULL;

	if (pwrpriv->default_patterns_en == _FALSE)
		return 0;

	for (index = 0 ; index < DEFAULT_PATTERN_NUM ; index++) {
		_rtw_memset(pwrpriv->patterns[index].content, 0,
			    sizeof(pwrpriv->patterns[index].content));
		_rtw_memset(pwrpriv->patterns[index].mask, 0,
			    sizeof(pwrpriv->patterns[index].mask));
		pwrpriv->patterns[index].len = 0;
	}

	/*TCP/ICMP unicast*/
	for (index = 0 ; index < DEFAULT_PATTERN_NUM ; index++) {
		switch (index) {
		case 0:
			target = pwrpriv->patterns[index].content;
			_rtw_memcpy(target, adapter_mac_addr(adapter),
				    ETH_ALEN);

			target += ETH_TYPE_OFFSET;
			_rtw_memcpy(target, &ip_protocol,
				    sizeof(ip_protocol));

			/* TCP */
			target += (PROTOCOL_OFFSET - ETH_TYPE_OFFSET);
			_rtw_memset(target, 0x06, 1);

			target += (IP_OFFSET - PROTOCOL_OFFSET);

			_rtw_memcpy(target, pmlmeinfo->ip_addr,
				    RTW_IP_ADDR_LEN);

			_rtw_memcpy(pwrpriv->patterns[index].mask,
				    &unicast_mask, sizeof(unicast_mask));

			pwrpriv->patterns[index].len =
				IP_OFFSET + RTW_IP_ADDR_LEN;
			break;
		case 1:
			target = pwrpriv->patterns[index].content;
			_rtw_memcpy(target, adapter_mac_addr(adapter),
				    ETH_ALEN);

			target += ETH_TYPE_OFFSET;
			_rtw_memcpy(target, &ip_protocol, sizeof(ip_protocol));

			/* ICMP */
			target += (PROTOCOL_OFFSET - ETH_TYPE_OFFSET);
			_rtw_memset(target, 0x01, 1);

			target += (IP_OFFSET - PROTOCOL_OFFSET);
			_rtw_memcpy(target, pmlmeinfo->ip_addr,
				    RTW_IP_ADDR_LEN);

			_rtw_memcpy(pwrpriv->patterns[index].mask,
				    &unicast_mask, sizeof(unicast_mask));
			pwrpriv->patterns[index].len =

				IP_OFFSET + RTW_IP_ADDR_LEN;
			break;
#ifdef CONFIG_IPV6
		case 2:
			if (pwrpriv->wowlan_ns_offload_en == _TRUE) {
				target = pwrpriv->patterns[index].content;
				target += ETH_TYPE_OFFSET;

				_rtw_memcpy(target, &ipv6_protocol,
					    sizeof(ipv6_protocol));

				/* ICMPv6 */
				target += (IPv6_PROTOCOL_OFFSET -
					   ETH_TYPE_OFFSET);
				_rtw_memset(target, 0x3a, 1);

				target += (IPv6_OFFSET - IPv6_PROTOCOL_OFFSET);
				_rtw_memcpy(target, pmlmeinfo->ip6_addr,
					    RTW_IPv6_ADDR_LEN);

				_rtw_memcpy(pwrpriv->patterns[index].mask,
					    &icmpv6_mask, sizeof(icmpv6_mask));
				pwrpriv->patterns[index].len =
					IPv6_OFFSET + RTW_IPv6_ADDR_LEN;
			}
			break;
#endif /*CONFIG_IPV6*/
		case 3:
			target = pwrpriv->patterns[index].content;
			_rtw_memcpy(target, &multicast_addr,
				    sizeof(multicast_addr));

			target += ETH_TYPE_OFFSET;
			_rtw_memcpy(target, &ip_protocol, sizeof(ip_protocol));

			/* UDP */
			target += (PROTOCOL_OFFSET - ETH_TYPE_OFFSET);
			_rtw_memset(target, 0x11, 1);

			target += (IP_OFFSET - PROTOCOL_OFFSET);
			_rtw_memcpy(target, &multicast_ip,
				    sizeof(multicast_ip));

			_rtw_memcpy(pwrpriv->patterns[index].mask,
				    &multicast_mask, sizeof(multicast_mask));

			pwrpriv->patterns[index].len =
				IP_OFFSET + sizeof(multicast_ip);
			break;
		default:
			break;
		}
	}
	return index;
}

void rtw_dump_priv_pattern(_adapter *adapter, u8 idx)
{
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(adapter);
	char str_1[128];
	char *p_str;
	u8 val8 = 0;
	int i = 0, j = 0, len = 0, max_len = 0;

	RTW_INFO("=========[%d]========\n", idx);

	RTW_INFO(">>>priv_pattern_content:\n");
	p_str = str_1;
	max_len = sizeof(str_1);
	for (i = 0 ; i < MAX_WKFM_PATTERN_SIZE / 8 ; i++) {
		_rtw_memset(p_str, 0, max_len);
		len = 0;
		for (j = 0 ; j < 8 ; j++) {
			val8 = pwrctl->patterns[idx].content[i * 8 + j];
			len += snprintf(p_str + len, max_len - len,
					"%02x ", val8);
		}
		RTW_INFO("%s\n", p_str);
	}

	RTW_INFO(">>>priv_pattern_mask:\n");
	for (i = 0 ; i < MAX_WKFM_SIZE / 8 ; i++) {
		_rtw_memset(p_str, 0, max_len);
		len = 0;
		for (j = 0 ; j < 8 ; j++) {
			val8 = pwrctl->patterns[idx].mask[i * 8 + j];
			len += snprintf(p_str + len, max_len - len,
					"%02x ", val8);
		}
		RTW_INFO("%s\n", p_str);
	}

	RTW_INFO(">>>priv_pattern_len:\n");
	RTW_INFO("%s: len: %d\n", __func__, pwrctl->patterns[idx].len);
}

void rtw_wow_pattern_sw_dump(_adapter *adapter)
{
	int i;

	RTW_INFO("********[RTK priv-patterns]*********\n");
	for (i = 0 ; i < MAX_WKFM_CAM_NUM; i++)
		rtw_dump_priv_pattern(adapter, i);
}

void rtw_get_sec_iv(_adapter *padapter, u8 *pcur_dot11txpn, u8 *StaAddr)
{
	struct sta_info		*psta;
	struct security_priv *psecpriv = &padapter->securitypriv;

	_rtw_memset(pcur_dot11txpn, 0, 8);
	if (NULL == StaAddr)
		return;
	psta = rtw_get_stainfo(&padapter->stapriv, StaAddr);
	RTW_INFO("%s(): StaAddr: %02x %02x %02x %02x %02x %02x\n",
		 __func__, StaAddr[0], StaAddr[1], StaAddr[2],
		 StaAddr[3], StaAddr[4], StaAddr[5]);

	if (psta) {
		if (psecpriv->dot11PrivacyAlgrthm == _AES_)
			AES_IV(pcur_dot11txpn, psta->dot11txpn, 0);
		else if (psecpriv->dot11PrivacyAlgrthm == _TKIP_)
			TKIP_IV(pcur_dot11txpn, psta->dot11txpn, 0);

		RTW_INFO("%s(): CurrentIV: %02x %02x %02x %02x %02x %02x %02x %02x\n"
			 , __func__, pcur_dot11txpn[0], pcur_dot11txpn[1],
			pcur_dot11txpn[2], pcur_dot11txpn[3], pcur_dot11txpn[4],
			pcur_dot11txpn[5], pcur_dot11txpn[6], pcur_dot11txpn[7]);
	}
}

void rtw_wow_lps_level_decide(_adapter *adapter, u8 wow_en)
{
	struct dvobj_priv *dvobj = adapter_to_dvobj(adapter);
	struct pwrctrl_priv *pwrpriv = dvobj_to_pwrctl(dvobj);

	if (wow_en) {
		pwrpriv->lps_level_bk = pwrpriv->lps_level;
		pwrpriv->lps_level = pwrpriv->wowlan_lps_level;
		#ifdef CONFIG_LPS_1T1R
		pwrpriv->lps_1t1r_bk = pwrpriv->lps_1t1r;
		pwrpriv->lps_1t1r = pwrpriv->wowlan_lps_1t1r;
		#endif
	} else {
		pwrpriv->lps_level = pwrpriv->lps_level_bk;
		#ifdef CONFIG_LPS_1T1R
		pwrpriv->lps_1t1r = pwrpriv->lps_1t1r_bk;
		#endif
	}
}

int rtw_pm_set_wow_lps(_adapter *padapter, u8 mode)
{
	int	ret = 0;
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(padapter);

	if (mode < PM_PS_MODE_NUM) {
		if (pwrctrlpriv->wowlan_power_mgmt != mode) 
			pwrctrlpriv->wowlan_power_mgmt = mode;
	} else
		ret = -EINVAL;

	return ret;
}
int rtw_pm_set_wow_lps_level(_adapter *padapter, u8 level)
{
	int	ret = 0;
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(padapter);

	if (level < LPS_LEVEL_MAX)
		pwrctrlpriv->wowlan_lps_level = level;
	else
		ret = -EINVAL;

	return ret;
}
#ifdef CONFIG_LPS_1T1R
int rtw_pm_set_wow_lps_1t1r(_adapter *padapter, u8 en)
{
	int	ret = 0;
	struct pwrctrl_priv *pwrctrlpriv = adapter_to_pwrctl(padapter);

	en = en ? 1 : 0;
	pwrctrlpriv->wowlan_lps_1t1r = en;

	return ret;
}
#endif /* CONFIG_LPS_1T1R */
#endif /* CONFIG_WOWLAN */

#ifdef CONFIG_PNO_SUPPORT
#define CSCAN_TLV_TYPE_SSID_IE	'S'
#define CIPHER_IE "key_mgmt="
#define CIPHER_NONE "NONE"
#define CIPHER_WPA_PSK "WPA-PSK"
#define CIPHER_WPA_EAP "WPA-EAP IEEE8021X"
/*
 *  SSIDs list parsing from cscan tlv list
 */
int rtw_parse_ssid_list_tlv(char **list_str, pno_ssid_t *ssid,
			    int max, int *bytes_left)
{
	char *str;

	int idx = 0;

	if ((list_str == NULL) || (*list_str == NULL) || (*bytes_left < 0)) {
		RTW_INFO("%s error paramters\n", __func__);
		return -1;
	}

	str = *list_str;
	while (*bytes_left > 0) {

		if (str[0] != CSCAN_TLV_TYPE_SSID_IE) {
			*list_str = str;
			RTW_INFO("nssid=%d left_parse=%d %d\n", idx, *bytes_left, str[0]);
			return idx;
		}

		/* Get proper CSCAN_TLV_TYPE_SSID_IE */
		*bytes_left -= 1;
		str += 1;

		if (str[0] == 0) {
			/* Broadcast SSID */
			ssid[idx].SSID_len = 0;
			_rtw_memset((char *)ssid[idx].SSID, 0x0, WLAN_SSID_MAXLEN);
			*bytes_left -= 1;
			str += 1;

			RTW_INFO("BROADCAST SCAN  left=%d\n", *bytes_left);
		} else if (str[0] <= WLAN_SSID_MAXLEN) {
			/* Get proper SSID size */
			ssid[idx].SSID_len = str[0];
			*bytes_left -= 1;
			str += 1;

			/* Get SSID */
			if (ssid[idx].SSID_len > *bytes_left) {
				RTW_INFO("%s out of memory range len=%d but left=%d\n",
					__func__, ssid[idx].SSID_len, *bytes_left);
				return -1;
			}

			_rtw_memcpy((char *)ssid[idx].SSID, str, ssid[idx].SSID_len);

			*bytes_left -= ssid[idx].SSID_len;
			str += ssid[idx].SSID_len;

			RTW_INFO("%s :size=%d left=%d\n",
				(char *)ssid[idx].SSID, ssid[idx].SSID_len, *bytes_left);
		} else {
			RTW_INFO("### SSID size more that %d\n", str[0]);
			return -1;
		}

		if (idx++ >  max) {
			RTW_INFO("%s number of SSIDs more that %d\n", __func__, idx);
			return -1;
		}
	}

	*list_str = str;
	return idx;
}

int rtw_parse_cipher_list(struct pno_nlo_info *nlo_info, char *list_str)
{

	char *pch, *pnext, *pend;
	u8 key_len = 0, index = 0;

	pch = list_str;

	if (nlo_info == NULL || list_str == NULL) {
		RTW_INFO("%s error paramters\n", __func__);
		return -1;
	}

	while (strlen(pch) != 0) {
		pnext = strstr(pch, "key_mgmt=");
		if (pnext != NULL) {
			pch = pnext + strlen(CIPHER_IE);
			pend = strstr(pch, "}");
			if (strncmp(pch, CIPHER_NONE,
				    strlen(CIPHER_NONE)) == 0)
				nlo_info->ssid_cipher_info[index] = 0x00;
			else if (strncmp(pch, CIPHER_WPA_PSK,
					 strlen(CIPHER_WPA_PSK)) == 0)
				nlo_info->ssid_cipher_info[index] = 0x66;
			else if (strncmp(pch, CIPHER_WPA_EAP,
					 strlen(CIPHER_WPA_EAP)) == 0)
				nlo_info->ssid_cipher_info[index] = 0x01;
			index++;
			pch = pend + 1;
		} else
			break;
	}
	return 0;
}

int rtw_dev_nlo_info_set(struct pno_nlo_info *nlo_info, pno_ssid_t *ssid,
		 int num, int pno_time, int pno_repeat, int pno_freq_expo_max)
{

	int i = 0;
	struct file *fp;
	mm_segment_t fs;
	loff_t pos = 0;
	u8 *source = NULL;
	long len = 0;

	RTW_INFO("+%s+\n", __func__);

	nlo_info->fast_scan_period = pno_time;
	nlo_info->ssid_num = num & BIT_LEN_MASK_32(8);
	nlo_info->hidden_ssid_num = num & BIT_LEN_MASK_32(8);
	nlo_info->slow_scan_period = (pno_time * 2);
	nlo_info->fast_scan_iterations = 5;

	if (nlo_info->hidden_ssid_num > 8)
		nlo_info->hidden_ssid_num = 8;

	/* TODO: channel list and probe index is all empty. */
	for (i = 0 ; i < num ; i++) {
		nlo_info->ssid_length[i]
			= ssid[i].SSID_len;
	}

	/* cipher array */
	fp = filp_open("/data/misc/wifi/wpa_supplicant.conf", O_RDONLY,  0644);
	if (IS_ERR(fp)) {
		RTW_INFO("Error, wpa_supplicant.conf doesn't exist.\n");
		RTW_INFO("Error, cipher array using default value.\n");
		return 0;
	}

	len = i_size_read(fp->f_path.dentry->d_inode);
	if (len < 0 || len > 2048) {
		RTW_INFO("Error, file size is bigger than 2048.\n");
		RTW_INFO("Error, cipher array using default value.\n");
		return 0;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);

	source = rtw_zmalloc(2048);

	if (source != NULL) {
		len = vfs_read(fp, source, len, &pos);
		rtw_parse_cipher_list(nlo_info, source);
		rtw_mfree(source, 2048);
	}

	set_fs(fs);
	filp_close(fp, NULL);

	RTW_INFO("-%s-\n", __func__);
	return 0;
}

int rtw_dev_ssid_list_set(struct pno_ssid_list *pno_ssid_list,
			  pno_ssid_t *ssid, u8 num)
{

	int i = 0;
	if (num > MAX_PNO_LIST_COUNT)
		num = MAX_PNO_LIST_COUNT;

	for (i = 0 ; i < num ; i++) {
		_rtw_memcpy(&pno_ssid_list->node[i].SSID,
			    ssid[i].SSID, ssid[i].SSID_len);
		pno_ssid_list->node[i].SSID_len = ssid[i].SSID_len;
	}
	return 0;
}

int rtw_dev_scan_info_set(_adapter *padapter, pno_ssid_t *ssid,
	  unsigned char ch, unsigned char ch_offset, unsigned short bw_mode)
{

	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(padapter);
	struct pno_scan_info *scan_info = pwrctl->pscan_info;
	u8 band = ch <= 14 ? BAND_ON_24G : BAND_ON_5G;
	int i;

	scan_info->channel_num = MAX_SCAN_LIST_COUNT;
	scan_info->orig_ch = ch;
	scan_info->orig_bw = bw_mode;
	scan_info->orig_40_offset = ch_offset;

	for (i = 0 ; i < scan_info->channel_num ; i++) {
		if (i < 11)
			scan_info->ssid_channel_info[i].active = 1;
		else
			scan_info->ssid_channel_info[i].active = 0;

		scan_info->ssid_channel_info[i].timeout = 100;

		scan_info->ssid_channel_info[i].tx_power =
			phy_get_tx_power_index_ex(padapter, 0, CCK, MGN_1M, bw_mode, band, i + 1);

		scan_info->ssid_channel_info[i].channel = i + 1;
	}

	RTW_INFO("%s, channel_num: %d, orig_ch: %d, orig_bw: %d orig_40_offset: %d\n",
		 __func__, scan_info->channel_num, scan_info->orig_ch,
		 scan_info->orig_bw, scan_info->orig_40_offset);
	return 0;
}

int rtw_dev_pno_set(struct net_device *net, pno_ssid_t *ssid, int num,
		    int pno_time, int pno_repeat, int pno_freq_expo_max)
{

	_adapter *padapter = (_adapter *)rtw_netdev_priv(net);
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(padapter);
	struct mlme_ext_priv	*pmlmeext = &padapter->mlmeextpriv;

	int ret = -1;

	if (num == 0) {
		RTW_INFO("%s, nssid is zero, no need to setup pno ssid list\n", __func__);
		return 0;
	}

	if (pwrctl == NULL) {
		RTW_INFO("%s, ERROR: pwrctl is NULL\n", __func__);
		return -1;
	} else {
		pwrctl->pnlo_info =
			(pno_nlo_info_t *)rtw_zmalloc(sizeof(pno_nlo_info_t));
		pwrctl->pno_ssid_list =
			(pno_ssid_list_t *)rtw_zmalloc(sizeof(pno_ssid_list_t));
		pwrctl->pscan_info =
			(pno_scan_info_t *)rtw_zmalloc(sizeof(pno_scan_info_t));
	}

	if (pwrctl->pnlo_info == NULL ||
	    pwrctl->pscan_info == NULL ||
	    pwrctl->pno_ssid_list == NULL) {
		RTW_INFO("%s, ERROR: alloc nlo_info, ssid_list, scan_info fail\n", __func__);
		goto failing;
	}

	pwrctl->wowlan_in_resume = _FALSE;

	pwrctl->pno_inited = _TRUE;
	/* NLO Info */
	ret = rtw_dev_nlo_info_set(pwrctl->pnlo_info, ssid, num,
				   pno_time, pno_repeat, pno_freq_expo_max);

	/* SSID Info */
	ret = rtw_dev_ssid_list_set(pwrctl->pno_ssid_list, ssid, num);

	/* SCAN Info */
	ret = rtw_dev_scan_info_set(padapter, ssid, pmlmeext->chandef.chan,
			    pmlmeext->chandef.offset, pmlmeext->chandef.bw);

	RTW_INFO("+%s num: %d, pno_time: %d, pno_repeat:%d, pno_freq_expo_max:%d+\n",
		 __func__, num, pno_time, pno_repeat, pno_freq_expo_max);

	return 0;

failing:
	if (pwrctl->pnlo_info) {
		rtw_mfree((u8 *)pwrctl->pnlo_info, sizeof(pno_nlo_info_t));
		pwrctl->pnlo_info = NULL;
	}
	if (pwrctl->pno_ssid_list) {
		rtw_mfree((u8 *)pwrctl->pno_ssid_list, sizeof(pno_ssid_list_t));
		pwrctl->pno_ssid_list = NULL;
	}
	if (pwrctl->pscan_info) {
		rtw_mfree((u8 *)pwrctl->pscan_info, sizeof(pno_scan_info_t));
		pwrctl->pscan_info = NULL;
	}

	return -1;
}

#ifdef CONFIG_PNO_SET_DEBUG
void rtw_dev_pno_debug(struct net_device *net)
{
	_adapter *padapter = (_adapter *)rtw_netdev_priv(net);
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(padapter);
	int i = 0, j = 0;

	RTW_INFO("*******NLO_INFO********\n");
	RTW_INFO("ssid_num: %d\n", pwrctl->pnlo_info->ssid_num);
	RTW_INFO("fast_scan_iterations: %d\n",
		 pwrctl->pnlo_info->fast_scan_iterations);
	RTW_INFO("fast_scan_period: %d\n", pwrctl->pnlo_info->fast_scan_period);
	RTW_INFO("slow_scan_period: %d\n", pwrctl->pnlo_info->slow_scan_period);



	for (i = 0 ; i < MAX_PNO_LIST_COUNT ; i++) {
		RTW_INFO("%d SSID (%s) length (%d) cipher(%x) channel(%d)\n",
			i, pwrctl->pno_ssid_list->node[i].SSID, pwrctl->pnlo_info->ssid_length[i],
			pwrctl->pnlo_info->ssid_cipher_info[i], pwrctl->pnlo_info->ssid_channel_info[i]);
	}

	RTW_INFO("******SCAN_INFO******\n");
	RTW_INFO("ch_num: %d\n", pwrctl->pscan_info->channel_num);
	RTW_INFO("orig_ch: %d\n", pwrctl->pscan_info->orig_ch);
	RTW_INFO("orig bw: %d\n", pwrctl->pscan_info->orig_bw);
	RTW_INFO("orig 40 offset: %d\n", pwrctl->pscan_info->orig_40_offset);
	for (i = 0 ; i < MAX_SCAN_LIST_COUNT ; i++) {
		RTW_INFO("[%02d] avtive:%d, timeout:%d, tx_power:%d, ch:%02d\n",
			 i, pwrctl->pscan_info->ssid_channel_info[i].active,
			 pwrctl->pscan_info->ssid_channel_info[i].timeout,
			 pwrctl->pscan_info->ssid_channel_info[i].tx_power,
			 pwrctl->pscan_info->ssid_channel_info[i].channel);
	}
	RTW_INFO("*****************\n");
}
#endif /* CONFIG_PNO_SET_DEBUG */
#endif /* CONFIG_PNO_SUPPORT */

