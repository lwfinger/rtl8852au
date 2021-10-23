/******************************************************************************
 *
 * Copyright(c) 2021 Realtek Corporation.
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
#define _PHL_CMD_PS_C_
#include "phl_headers.h"

/* structure of a power request */
struct pwr_req {
	_os_list list;
	u16 evt_id;
};

enum {
	PS_STATE_NONE = 0,
	PS_STATE_LEAVED,
	/**
	 * lps: protocol only
	 * ips: won't in this state
	 */
	PS_STATE_PROTO,
	/**
	 * lps: protocol + power
	 * ips: power
	 */
	PS_STATE_ENTERED
};

static const char *_ps_state_to_str(u8 ps_state)
{
	switch (ps_state) {
	case PS_STATE_ENTERED:
		return "PS_ENTERED";
	case PS_STATE_PROTO:
		return "PS_PROTOCOL";
	case PS_STATE_LEAVED:
		return "PS_LEAVED";
	default:
		return "NONE";
	}
}

struct _ps_mr_info {
	bool ap_active;
	bool gc_active;
};

#define MAX_PWE_REQ_NUM 16
struct cmd_ps {
	struct phl_info_t *phl_info;
	void *dispr;

	struct phl_queue req_busy_q;
	struct phl_queue req_idle_q;
	struct pwr_req req_pool[MAX_PWE_REQ_NUM];

	bool rej_pwr_req; /* reject all pwr request */
	bool btc_req_pwr; /* btc request pwr or not */

	/* current state */
	u8 cur_pwr_lvl;
	u8 ps_state;
	u8 ps_mode;

	/* lps */
	u16 macid;
	u32 null_token;
	bool pre_phase_leave_ps;

	/**
	 * runtime capability
	 * set by "rtw_phl_ps_set_rt_cap"
	 * true: allow entering ps
	 * false: disallow entering ps
	 */
	bool runtime_ps_en;

	struct _ps_mr_info mr_info;

	/* rssi */
	u8 rssi_bcn_min;
};

/**
 * determine leave lps or not
 * return true if rssi variation reach threshold
 * @ps: see cmd_ps
 */
static bool _chk_rssi_diff_reach_thld(struct cmd_ps *ps)
{
	struct phl_info_t *phl_info = ps->phl_info;
	struct rtw_ps_cap_t *ps_cap = _get_ps_cap(ps->phl_info);
	u8 cur_rssi_bcn_min = 0;
	u8 *rssi_bcn_min = &ps->rssi_bcn_min;
	bool leave_ps = false;

	cur_rssi_bcn_min = phl_get_min_rssi_bcn(phl_info);

	do {
		if (*rssi_bcn_min == 0 || *rssi_bcn_min == 0xFF) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): (criteria = %d, cur = %d) criteria invalid, set criteria to cur\n",
					__func__, *rssi_bcn_min, cur_rssi_bcn_min);
			*rssi_bcn_min = cur_rssi_bcn_min; /* update with current_rssi */
			break;
		}

		if (DIFF(*rssi_bcn_min, cur_rssi_bcn_min) < ps_cap->lps_rssi_diff_threshold) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): (criteria = %d, cur = %d) RSSI diff < %d, do nothing\n",
					__func__, *rssi_bcn_min, cur_rssi_bcn_min, ps_cap->lps_rssi_diff_threshold);
			break;
		}

		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): leave ps and update criteria from %d to %d\n", __func__, *rssi_bcn_min, cur_rssi_bcn_min);

		leave_ps = true;
		*rssi_bcn_min = cur_rssi_bcn_min;
	} while (0);

	return leave_ps;
}

/**
 * determine enter/leave lps or not
 * return true if lps state is changed
 * @ps: see cmd_ps
 * @mac_id: macid of corresponding sta
 * @cur_state: currently lps state
 * @new_state: target lps state
 */
static bool
_lps_state_judge_changed(struct cmd_ps *ps, u16 macid, u8 cur_state, u8 *new_state)
{
	struct phl_info_t *phl_info = ps->phl_info;
	struct rtw_ps_cap_t *ps_cap = _get_ps_cap(ps->phl_info);
	struct rtw_stats *phl_stats = &phl_info->phl_com->phl_stats;
	struct rtw_phl_stainfo_t *sta = NULL;
	u8 rssi = 0;

	*new_state = cur_state;

	sta = rtw_phl_get_stainfo_by_macid(phl_info, macid);
	if (sta == NULL) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): cannot get sta of macid %d.\n", __func__, macid);
		return false;
	}

	rssi = rtw_hal_get_sta_rssi(sta);

	if (cur_state == PS_STATE_LEAVED || cur_state == PS_STATE_PROTO) {
		if (rssi > ps_cap->lps_rssi_enter_threshold &&
			phl_stats->tx_traffic.lvl == RTW_TFC_IDLE &&
			phl_stats->rx_traffic.lvl == RTW_TFC_IDLE) {
			*new_state = PS_STATE_ENTERED;
		}
	} else if (cur_state == PS_STATE_ENTERED) {
		if (_chk_rssi_diff_reach_thld(ps) ||
			rssi < ps_cap->lps_rssi_leave_threshold ||
			phl_stats->tx_traffic.lvl != RTW_TFC_IDLE ||
			phl_stats->rx_traffic.lvl != RTW_TFC_IDLE) {
			*new_state = PS_STATE_LEAVED;
		}
	}

	if (*new_state != cur_state) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): state %s -> %s, Tx(%d), Rx(%d), RSSI(%d)\n",
			__func__, _ps_state_to_str(cur_state), _ps_state_to_str(*new_state),
			phl_stats->tx_traffic.lvl,
			phl_stats->rx_traffic.lvl, rssi);
		return true;
	}

	return false;
}

/**
 * Leave power saving
 * return RTW_PHL_STATUS_SUCCESS if leave ps ok
 * @ps: see cmd_ps
 * @leave_proto: whether to leave protocol
 */
enum rtw_phl_status _leave_ps(struct cmd_ps *ps, bool leave_proto)
{
	struct phl_info_t *phl_info = ps->phl_info;
	enum rtw_phl_status status = RTW_PHL_STATUS_FAILURE;
	struct ps_cfg cfg = {0};

	if (ps->ps_state == PS_STATE_LEAVED) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): not in power saving.\n", __func__);
		return RTW_PHL_STATUS_SUCCESS;
	}

	cfg.cur_pwr_lvl = ps->cur_pwr_lvl;

	if (ps->ps_mode == PS_MODE_LPS) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): target macid to leave lps %d (leave_proto %d).\n",
				__func__, ps->macid, leave_proto);
		if (!leave_proto) {
			if (ps->ps_state == PS_STATE_PROTO) {
				PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): in lps protocal only.\n", __func__);
				return RTW_PHL_STATUS_SUCCESS;
			}
			cfg.pwr_cfg = true;
			cfg.proto_cfg = false;
		} else {
			if (ps->ps_state == PS_STATE_PROTO) {
				cfg.pwr_cfg = false;
				cfg.proto_cfg = true;
			} else {
				cfg.pwr_cfg = true;
				cfg.proto_cfg = true;
			}
		}
		cfg.macid = ps->macid;
		cfg.token = &ps->null_token;
		cfg.pwr_lvl = PS_PWR_LVL_PWRON;
		cfg.ps_mode = ps->ps_mode;
	} else if (ps->ps_mode == PS_MODE_IPS) {
		cfg.pwr_lvl = PS_PWR_LVL_PWRON;
		cfg.ps_mode = ps->ps_mode;
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): unknown ps mode!\n", __func__);
	}

	status = phl_ps_leave_ps(phl_info, &cfg);

	if (status == RTW_PHL_STATUS_SUCCESS) {
		ps->cur_pwr_lvl = cfg.pwr_lvl;
		if (ps->ps_mode == PS_MODE_LPS) {
			ps->ps_mode = (leave_proto != true) ? PS_MODE_LPS : PS_MODE_NONE;
			ps->ps_state = (leave_proto != true) ? PS_STATE_PROTO : PS_STATE_LEAVED;
			ps->macid = (leave_proto != true) ? ps->macid : 0xFFFF;
		} else {
			ps->ps_mode = PS_MODE_NONE;
			ps->ps_state = PS_STATE_LEAVED;
			ps->macid = 0xFFFF;
		}
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): leave ps success.\n", __func__);
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): ps mode(%s), pwr lvl(%s), ps state(%s)\n",
				__func__, phl_ps_ps_mode_to_str(ps->ps_mode),
				phl_ps_pwr_lvl_to_str(ps->cur_pwr_lvl), _ps_state_to_str(ps->ps_state));
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): leave ps fail!\n", __func__);
	}

	return status;
}

/**
 * Enter power saving
 * return RTW_PHL_STATUS_SUCCESS if enter ps ok
 * @ps: see cmd_ps
 * @ps_mode: target ps mode to enter
 * @macid : target macid to enter lps
 */
enum rtw_phl_status _enter_ps(struct cmd_ps *ps, u8 ps_mode, u16 macid)
{
	enum rtw_phl_status status = RTW_PHL_STATUS_FAILURE;
	struct ps_cfg cfg = {0};
	struct rtw_ps_cap_t *ps_cap = _get_ps_cap(ps->phl_info);

	if (ps->ps_state == PS_STATE_ENTERED) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): already in power saving.\n", __func__);
		return RTW_PHL_STATUS_SUCCESS;
	}

	cfg.cur_pwr_lvl = ps->cur_pwr_lvl;

	if (ps_mode == PS_MODE_LPS) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): target macid to enter lps %d.\n", __func__, macid);
		cfg.proto_cfg = (ps->ps_state == PS_STATE_PROTO) ? false : true;
		cfg.pwr_cfg = true;
		cfg.macid = macid;
		cfg.token = &ps->null_token;
		cfg.pwr_lvl = phl_ps_judge_pwr_lvl(ps_cap->lps_cap, ps_mode, true);
		cfg.ps_mode = ps_mode;
		cfg.awake_interval = ps_cap->lps_awake_interval;
		cfg.listen_bcn_mode = ps_cap->lps_listen_bcn_mode;
		cfg.smart_ps_mode = ps_cap->lps_smart_ps_mode;
	} else if (ps_mode == PS_MODE_IPS) {
		cfg.pwr_lvl = PS_PWR_LVL_PWROFF;
		cfg.ps_mode = ps_mode;
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): unknown ps mode!\n", __func__);
	}

	status = phl_ps_enter_ps(ps->phl_info, &cfg);

	if (status == RTW_PHL_STATUS_SUCCESS) {
		ps->cur_pwr_lvl = cfg.pwr_lvl;
		ps->ps_mode = cfg.ps_mode;
		ps->macid = cfg.macid;
		ps->ps_state = PS_STATE_ENTERED;
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): enter ps success.\n", __func__);
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): ps mode(%s), pwr lvl(%s), ps state(%s)\n",
					__func__, phl_ps_ps_mode_to_str(ps->ps_mode),
					phl_ps_pwr_lvl_to_str(ps->cur_pwr_lvl), _ps_state_to_str(ps->ps_state));
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): enter ps fail!\n", __func__);
	}

	return status;
}

static bool _pop_idle_req(struct cmd_ps *ps, struct pwr_req **req)
{
	void *d = phl_to_drvpriv(ps->phl_info);
	_os_list *new_req = NULL;
	bool ret = false;

	(*req) = NULL;
	if (pq_pop(d, &(ps->req_idle_q), &new_req, _first, _bh)) {
		(*req) = (struct pwr_req *)new_req;
		ret = true;
	} else {
		ret = false;
	}

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): remaining idle req cnt %d.\n", __func__, ps->req_idle_q.cnt);

	return ret;
}

static void _push_idle_req(struct cmd_ps *ps, struct pwr_req *req)
{
	void *d = phl_to_drvpriv(ps->phl_info);

	pq_push(d, &(ps->req_idle_q), &(req->list), _tail, _bh);

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): remaining idle req cnt %d.\n", __func__, ps->req_idle_q.cnt);
}

static bool _pop_busy_req(struct cmd_ps *ps, struct pwr_req **req)
{
	void *d = phl_to_drvpriv(ps->phl_info);
	_os_list *new_req = NULL;
	bool ret = false;

	(*req) = NULL;
	if (pq_pop(d, &(ps->req_busy_q), &new_req, _tail, _bh)) {
		(*req) = (struct pwr_req *)new_req;
		ret = true;
	} else {
		ret = false;
	}

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): current busy req cnt %d.\n", __func__, ps->req_busy_q.cnt);

	return ret;
}

static void _push_busy_req(struct cmd_ps *ps, struct pwr_req *req)
{
	void *d = phl_to_drvpriv(ps->phl_info);

	pq_push(d, &(ps->req_busy_q), &(req->list), _tail, _bh);

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): current busy req cnt %d.\n", __func__, ps->req_busy_q.cnt);
}

static void _cancel_pwr_req(struct cmd_ps *ps, u16 evt_id)
{
	struct pwr_req *req = NULL;

	if (!_pop_busy_req(ps, &req)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): busy queue is empty.\n", __func__);
		return;
	}

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): evt_id %d\n", __func__, req->evt_id);

	if (req->evt_id != evt_id && MSG_EVT_PHY_IDLE != evt_id)
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): evt_id mismatch.\n", __func__);

	req->evt_id = MSG_EVT_NONE;

	_push_idle_req(ps, req);
}

static enum rtw_phl_status _add_pwr_req(struct cmd_ps *ps, u16 evt_id)
{
	enum rtw_phl_status status = RTW_PHL_STATUS_SUCCESS;
	struct pwr_req *req = NULL;

	if (!_pop_idle_req(ps, &req)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): idle queue is empty.\n", __func__);
		return RTW_PHL_STATUS_RESOURCE;
	}

	req->evt_id = evt_id;

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): evt_id %d\n", __func__, evt_id);

	_push_busy_req(ps, req);

	return status;
}

static void _init_pwr_req_q(struct cmd_ps *ps)
{
	void *d = phl_to_drvpriv(ps->phl_info);
	u8 i = 0;

	pq_init(d, &ps->req_busy_q);
	pq_init(d, &ps->req_idle_q);

	_os_mem_set(d, ps->req_pool, 0,
		    sizeof(struct pwr_req) * MAX_PWE_REQ_NUM);

	for (i = 0; i < MAX_PWE_REQ_NUM; i++) {
		pq_push(d, &(ps->req_idle_q),
			&(ps->req_pool[i].list), _tail, _bh);
	}
}

static void _reset_pwr_req_q(struct cmd_ps *ps)
{
	void *d = phl_to_drvpriv(ps->phl_info);
	u8 i = 0;

	pq_reset(d, &ps->req_busy_q, _bh);
	pq_reset(d, &ps->req_idle_q, _bh);

	_os_mem_set(d, ps->req_pool, 0,
		    sizeof(struct pwr_req) * MAX_PWE_REQ_NUM);

	for (i = 0; i < MAX_PWE_REQ_NUM; i++) {
		pq_push(d, &(ps->req_idle_q),
			&(ps->req_pool[i].list), _tail, _bh);
	}
}

static void _deinit_pwr_req_q(struct cmd_ps *ps)
{
	void *d = phl_to_drvpriv(ps->phl_info);

	pq_deinit(d, &ps->req_busy_q);
	pq_deinit(d, &ps->req_idle_q);
}

static void _init_ps_dflt_sw_cap(struct cmd_ps *ps)
{
	struct rtw_ps_cap_t *sw_cap = _get_ps_sw_cap(ps->phl_info);

	sw_cap->init_rf_state = RTW_RF_ON;

	sw_cap->ips_en = PS_OP_MODE_DISABLED;
	sw_cap->ips_cap = 0;
	sw_cap->ips_wow_en = PS_OP_MODE_DISABLED;
	sw_cap->ips_wow_cap = 0;

	sw_cap->lps_en = PS_OP_MODE_DISABLED;
	sw_cap->lps_cap = 0;
	sw_cap->lps_awake_interval = 0;
	sw_cap->lps_listen_bcn_mode = RTW_LPS_RLBM_MIN;
	sw_cap->lps_smart_ps_mode = RTW_LPS_TRX_PWR0;
	sw_cap->lps_rssi_enter_threshold = 40;
	sw_cap->lps_rssi_leave_threshold = 35;
	sw_cap->lps_rssi_diff_threshold = 5;

	sw_cap->lps_wow_en = PS_OP_MODE_DISABLED;
	sw_cap->lps_wow_cap = 0;
	sw_cap->lps_wow_awake_interval = 0;
	sw_cap->lps_wow_listen_bcn_mode = RTW_LPS_RLBM_MAX;
	sw_cap->lps_wow_smart_ps_mode = RTW_LPS_TRX_PWR0;
}

static enum phl_mdl_ret_code _ps_mdl_init(void *phl, void *dispr, void **priv)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;
	struct cmd_ps *ps = NULL;

	PHL_INFO("[PS_CMD], %s(): \n", __func__);

	if (priv == NULL)
		return MDL_RET_FAIL;

	(*priv) = NULL;
	ps = (struct cmd_ps *)_os_mem_alloc(phl_to_drvpriv(phl_info),
					       sizeof(struct cmd_ps));
	if (ps == NULL) {
		PHL_ERR("[PS_CMD], %s(): allocate cmd ps fail.\n", __func__);
		return MDL_RET_FAIL;
	}

	_os_mem_set(phl_to_drvpriv(phl_info), ps, 0, sizeof(struct cmd_ps));

	ps->phl_info = phl_info;
	ps->dispr = dispr;
	(*priv) = (void *)ps;
	ps->cur_pwr_lvl = PS_PWR_LVL_PWRON;
	ps->ps_state = PS_STATE_LEAVED;
	ps->ps_mode = PS_MODE_NONE;
	ps->rej_pwr_req = false;
	ps->runtime_ps_en = false;

	_init_pwr_req_q(ps);

	_init_ps_dflt_sw_cap(ps);

	return MDL_RET_SUCCESS;
}

static void _ps_mdl_deinit(void *dispr, void *priv)
{
	struct cmd_ps *ps = (struct cmd_ps *)priv;

	PHL_INFO("[PS_CMD], %s(): \n", __func__);

	_deinit_pwr_req_q(ps);

	if (ps != NULL)
		_os_mem_free(phl_to_drvpriv(ps->phl_info), ps, sizeof(struct cmd_ps));
}

static void _dump_ps_cap(struct cmd_ps *ps)
{
	struct rtw_ps_cap_t *ps_cap = NULL;

	ps_cap = _get_ps_cap(ps->phl_info);

	PHL_INFO("[PS_CMD], %s(): \n", __func__);
	PHL_INFO("[PS_CMD], init_rf_state: %d\n", ps_cap->init_rf_state);
	PHL_INFO("[PS_CMD], ips_en: %d\n", ps_cap->ips_en);
	PHL_INFO("[PS_CMD], ips_cap: %d\n", ps_cap->ips_cap);
	PHL_INFO("[PS_CMD], ips_wow_en: %d\n", ps_cap->ips_wow_en);
	PHL_INFO("[PS_CMD], ips_wow_cap: %d\n", ps_cap->ips_wow_cap);
	PHL_INFO("[PS_CMD], lps_en: %d\n", ps_cap->lps_en);
	PHL_INFO("[PS_CMD], lps_cap: %d\n", ps_cap->lps_cap);
	PHL_INFO("[PS_CMD], lps_awake_interval: %d\n", ps_cap->lps_awake_interval);
	PHL_INFO("[PS_CMD], lps_listen_bcn_mode: %d\n", ps_cap->lps_listen_bcn_mode);
	PHL_INFO("[PS_CMD], lps_smart_ps_mode: %d\n", ps_cap->lps_smart_ps_mode);
	PHL_INFO("[PS_CMD], lps_rssi_enter_threshold: %d\n", ps_cap->lps_rssi_enter_threshold);
	PHL_INFO("[PS_CMD], lps_rssi_leave_threshold: %d\n", ps_cap->lps_rssi_leave_threshold);
	PHL_INFO("[PS_CMD], lps_rssi_diff_threshold: %d\n", ps_cap->lps_rssi_diff_threshold);
	PHL_INFO("[PS_CMD], lps_wow_en: %d\n", ps_cap->lps_wow_en);
	PHL_INFO("[PS_CMD], lps_wow_cap: %d\n", ps_cap->lps_wow_cap);
	PHL_INFO("[PS_CMD], lps_wow_awake_interval: %d\n", ps_cap->lps_wow_awake_interval);
	PHL_INFO("[PS_CMD], lps_wow_listen_bcn_mode: %d\n", ps_cap->lps_wow_listen_bcn_mode);
	PHL_INFO("[PS_CMD], lps_wow_smart_ps_mode: %d\n", ps_cap->lps_wow_smart_ps_mode);
}

static enum phl_mdl_ret_code _ps_mdl_start(void *dispr, void *priv)
{
	enum phl_mdl_ret_code ret = MDL_RET_SUCCESS;
	struct cmd_ps *ps = (struct cmd_ps *)priv;
	struct rtw_ps_cap_t *ps_cap = _get_ps_cap(ps->phl_info);
	u8 idx = 0;

	_reset_pwr_req_q(ps);

	phl_dispr_get_idx(dispr, &idx);

	if (idx == 0) {
		_dump_ps_cap(ps);
		PHL_INFO("[PS_CMD], %s(): init rf state %d, reject pwr req %d\n",
				__func__, ps_cap->init_rf_state, ps->rej_pwr_req);
		if (ps_cap->init_rf_state == RTW_RF_OFF)
			ps->rej_pwr_req = true;
		if (ps->rej_pwr_req == true)
			_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
	}

	return ret;
}

static enum phl_mdl_ret_code _ps_mdl_stop(void *dispr, void *priv)
{
	enum phl_mdl_ret_code ret = MDL_RET_SUCCESS;
	struct cmd_ps *ps = (struct cmd_ps *)priv;

	if (ps->ps_state == PS_STATE_ENTERED) {
		PHL_WARN("[PS_CMD], %s(): module stop in power saving!\n", __func__);
		_leave_ps(ps, true);
	}

	PHL_INFO("[PS_CMD], %s(): \n", __func__);

	return ret;
}

static bool _chk_role_all_inactive(struct cmd_ps *ps)
{
	struct phl_info_t *phl_info = ps->phl_info;
	u8 role_idx = 0;
	bool ret = true;

	for (role_idx = 0; role_idx < MAX_WIFI_ROLE_NUMBER; role_idx++) {
		if (phl_info->phl_com->wifi_roles[role_idx].mstate != MLME_NO_LINK) {
			ret = false;
			break;
		}
	}

	return ret;
}

static struct rtw_wifi_role_t *_get_role_of_ps_permitted(struct cmd_ps *ps)
{
	struct phl_info_t *phl_info = ps->phl_info;
	struct rtw_phl_com_t *phl_com = phl_info->phl_com;
	u8 role_idx = 0;

	for (role_idx = 0; role_idx < MAX_WIFI_ROLE_NUMBER; role_idx++) {
		if (phl_com->wifi_roles[role_idx].active == false)
			continue;
		if (phl_com->wifi_roles[role_idx].type == PHL_RTYPE_STATION &&
			phl_com->wifi_roles[role_idx].mstate == MLME_LINKED) {
				return &(phl_com->wifi_roles[role_idx]);
		}
	}

	return NULL;
}

/**
 * go through all wifi roles and check whether input
 * ps mode desired is allowed with existing wroles
 * @ps: see cmd_ps
 * @target_mode: the desired ps mode (lps or ips)
 * @macid: target macid to enter lps
 */
static bool _chk_wrole_with_ps_mode(struct cmd_ps *ps,
		u8 target_mode, u16 *macid)
{
	struct rtw_wifi_role_t *role = NULL;
	bool ret = false;
	struct rtw_phl_stainfo_t *sta = NULL;

	if (ps->mr_info.ap_active || ps->mr_info.gc_active) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): ap %d or gc %d, is active.\n", __func__,
				ps->mr_info.ap_active, ps->mr_info.gc_active);
		return false;
	}

	if (target_mode == PS_MODE_IPS) {
		if (_chk_role_all_inactive(ps))
			ret = true;
	} else if (target_mode == PS_MODE_LPS) {
		role = _get_role_of_ps_permitted(ps);
		if (role == NULL) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): there is no role to enter ps.\n", __func__);
			return false;
		}
		sta = rtw_phl_get_stainfo_self(ps->phl_info, role);
		if (sta == NULL) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): cannot get sta info.\n", __func__);
			return false;
		}
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): role id to enter ps (%d).\n", __func__, role->id);
		*macid = sta->macid;
		ret = true;
	}

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): decide enter ps(%s), target mode(%s).\n",
				__func__, (ret) ? "Yes" : "No", phl_ps_ps_mode_to_str(target_mode));

	return ret;
}

static void _stop_datapath(struct cmd_ps *ps)
{
	struct phl_info_t *phl_info = ps->phl_info;
	struct phl_msg msg = {0};
	struct phl_msg_attribute attr = {0};

	SET_MSG_MDL_ID_FIELD(msg.msg_id, PHL_MDL_POWER_MGNT);
	SET_MSG_EVT_ID_FIELD(msg.msg_id, MSG_EVT_SW_TX_PAUSE);
	msg.band_idx = HW_BAND_0;

	if (phl_disp_eng_send_msg(phl_info, &msg, &attr, NULL) !=
	                          RTW_PHL_STATUS_SUCCESS) {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): fail to send tx pause msg.\n", __func__);
	}
}

static bool _is_datapath_active(struct cmd_ps *ps)
{
	struct phl_info_t *phl_info = ps->phl_info;

	return (_os_atomic_read(phl_to_drvpriv(phl_info), &phl_info->phl_sw_tx_sts)
			!= PHL_TX_STATUS_SW_PAUSE) ? true : false;
}

/**
 * check current capability of power saving
 * return able to enter ps or not
 * @ps: see cmd_ps
 * @mode: the target ps mode to be check
 */
static bool _chk_ps_cap(struct cmd_ps *ps, u8 mode)
{
	struct rtw_ps_cap_t *ps_cap = _get_ps_cap(ps->phl_info);

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): mode(%s), lps_en(%d), ips_en(%d), runtime_ps_en(%d).\n",
			__func__, phl_ps_ps_mode_to_str(mode), ps_cap->lps_en, ps_cap->ips_en, ps->runtime_ps_en);

	switch (mode) {
	case PS_MODE_LPS:
		if (ps_cap->lps_en == PS_OP_MODE_DISABLED) {
			return false;
		} else if (ps_cap->lps_en == PS_OP_MODE_FORCE_ENABLED) {
			/* force enable */
			return true;
		} else if (ps_cap->lps_en == PS_OP_MODE_AUTO) {
			if (ps->runtime_ps_en == true)
				return true;
		}
		break;
	case PS_MODE_IPS:
		if (ps_cap->ips_en == PS_OP_MODE_DISABLED) {
			return false;
		} else if (ps_cap->ips_en == PS_OP_MODE_FORCE_ENABLED) {
			/* force enable */
			return true;
		} else if (ps_cap->ips_en == PS_OP_MODE_AUTO) {
			if (ps->runtime_ps_en == true)
				return true;
		}
		break;
	default:
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): unknown ps mode.\n", __func__);
		return false;
	}

	return false;
}

/**
 * check the condition of ips
 * return whether to enter ips or not
 * @ps: see cmd_ps
 */
static bool _chk_ips_enter(struct cmd_ps *ps)
{
	if (TEST_STATUS_FLAG(ps->phl_info->phl_com->dev_state, RTW_DEV_RESUMING)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): resume in progress.\n", __func__);
		return false;
	}

	if (!_chk_ps_cap(ps, PS_MODE_IPS)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): ips is not allowed.\n", __func__);
		return false;
	}

	if (ps->req_busy_q.cnt) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): req q is not empty.\n", __func__);
		return false;
	}

	if (ps->btc_req_pwr) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): btc req pwr.\n", __func__);
		return false;
	}

	if (!_chk_wrole_with_ps_mode(ps, PS_MODE_IPS, NULL)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): no need to enter ips.\n", __func__);
		return false;
	}

	return true;
}

/**
 * check the condition of lps
 * return whether to enter lps or not
 * @ps: see cmd_ps
 */
static bool _chk_lps_enter(struct cmd_ps *ps, u16 *macid)
{
	/* check enter lps or not */
	u8 target_ps_state = PS_STATE_NONE;

	if (TEST_STATUS_FLAG(ps->phl_info->phl_com->dev_state, RTW_DEV_RESUMING)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): resume in progress.\n", __func__);
		return false;
	}

	/* check capability */
	if (!_chk_ps_cap(ps, PS_MODE_LPS)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): lps is not allowed.\n", __func__);
		return false;
	}

	/* ref cnt */
	if (ps->req_busy_q.cnt) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): req q is not empty.\n", __func__);
		return false;
	}

	/* btc */
	if (ps->btc_req_pwr) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): btc req pwr.\n", __func__);
		return false;
	}

	if (ps->pre_phase_leave_ps == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): just leave ps in watchdog prephase.\n", __func__);
		return false;
	}

	/* check wifi role */
	if (!_chk_wrole_with_ps_mode(ps, PS_MODE_LPS, macid)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): no need to enter lps.\n", __func__);
		return false;
	}

	/* lps */
	if (_lps_state_judge_changed(ps, *macid, ps->ps_state, &target_ps_state)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): lps state changed.\n", __func__);
		if (target_ps_state == PS_STATE_ENTERED) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): try enter lps.\n", __func__);
			/* check data path stop or not */
			if (!_is_datapath_active(ps)) {
				return true;
			} else {
				_stop_datapath(ps);
			}
		}
	}

	return false;
}

static void _ps_watchdog_info_dump(struct cmd_ps *ps)
{
	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "========== PS CMD Info ========== \n");
	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "ps mode(%s), pwr lvl(%s), ps state(%s)\n",
			phl_ps_ps_mode_to_str(ps->ps_mode),
			phl_ps_pwr_lvl_to_str(ps->cur_pwr_lvl),
			_ps_state_to_str(ps->ps_state));
	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "req idle cnt(%d), req busy cnt(%d)\n",
			ps->req_idle_q.cnt, ps->req_busy_q.cnt);
	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "========== PS CMD Info ========== \n");
}

/**
 * pre-phase handler of watchdog
 * will check whether to leave lps or not
 * @ps: see cmd_ps
 */
static enum phl_mdl_ret_code
_ps_watchdog_pre_hdlr(struct cmd_ps *ps)
{
	/* check leave lps or not */
	u8 target_ps_state = PS_STATE_NONE;

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): pwr lvl(%s).\n", __func__, phl_ps_pwr_lvl_to_str(ps->cur_pwr_lvl));

	ps->pre_phase_leave_ps = false;

	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	if (ps->ps_mode == PS_MODE_IPS) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): power off.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	if (ps->ps_state == PS_STATE_LEAVED) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): not in power saving.\n", __func__);
		return MDL_RET_SUCCESS;
	} else {
		if (_lps_state_judge_changed(ps, ps->macid, ps->ps_state, &target_ps_state)) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): lps state changed.\n", __func__);
			if (target_ps_state == PS_STATE_LEAVED) {
				PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): try leave lps.\n", __func__);
				if (_leave_ps(ps, true) == RTW_PHL_STATUS_SUCCESS) {
					ps->pre_phase_leave_ps = true;
					return MDL_RET_SUCCESS;
				} else {
					PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
					return MDL_RET_CANNOT_IO;
				}
			}
		}
		if (ps->ps_state == PS_STATE_PROTO)
			return MDL_RET_SUCCESS;
		else
			return MDL_RET_CANNOT_IO;
	}
}

/**
 * post-phase handler of watchdog
 * will check whether to enter lps or not
 * @ps: see cmd_ps
 */
static enum phl_mdl_ret_code
_ps_watchdog_post_hdlr(struct cmd_ps *ps)
{
	u16 macid = 0xFFFF;

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): pwr lvl(%s).\n", __func__, phl_ps_pwr_lvl_to_str(ps->cur_pwr_lvl));

	if (_chk_lps_enter(ps, &macid))
		_enter_ps(ps, PS_MODE_LPS, macid);
	else if (_chk_ips_enter(ps))
		_enter_ps(ps, PS_MODE_IPS, 0xFFFF);

	_ps_watchdog_info_dump(ps);

	return MDL_RET_SUCCESS;
}

static enum phl_mdl_ret_code _phy_on_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	if (IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], MSG_EVT_PHY_ON\n");
		if (_leave_ps(ps, true) == RTW_PHL_STATUS_SUCCESS) {
			_add_pwr_req(ps, MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_SUCCESS;
		} else {
			PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
			/* L2 Recovery */
		}
	} else {
		ret = MDL_RET_SUCCESS;
	}

	return ret;
}

static enum phl_mdl_ret_code _tx_pwr_req_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	if (IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], MSG_EVT_TRX_PWR_REQ\n");
		if (_leave_ps(ps, false) == RTW_PHL_STATUS_SUCCESS) {
			ret = MDL_RET_SUCCESS;
		} else {
			PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
			/* L2 Recovery */
		}
	} else {
		ret = MDL_RET_SUCCESS;
	}

	return ret;
}

static enum phl_mdl_ret_code _phy_idle_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	if (!IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], MSG_EVT_PHY_IDLE\n");
		_cancel_pwr_req(ps, MSG_EVT_ID_FIELD(msg->msg_id));
		/* check enter ips or not */
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): try enter ips.\n", __func__);
		if (_chk_ips_enter(ps)) {
			_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
		}
		ret = MDL_RET_SUCCESS;
	} else {
		ret = MDL_RET_SUCCESS;
	}

	return ret;
}

/**
 * pre-phase handler of msg
 * leave ps and return corresponding status
 * @ps: see cmd_ps
 * @evt_id: evt id of msg
 */
static enum phl_mdl_ret_code
_ext_msg_pre_hdlr(struct cmd_ps *ps, u16 evt_id)
{
	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	/* power request */
	if (_leave_ps(ps, true) == RTW_PHL_STATUS_SUCCESS) {
		_add_pwr_req(ps, evt_id);
		return MDL_RET_SUCCESS;
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
		/* L2 Recovery */
	}

	return MDL_RET_CANNOT_IO;
}

/**
 * post-phase handler of msg
 * cancel power req and chk enter ips or not
 * @ps: see cmd_ps
 * @evt_id: evt id of msg
 */
static enum phl_mdl_ret_code
_ext_msg_post_hdlr(struct cmd_ps *ps, u16 evt_id)
{
	/* cancel power request */
	_cancel_pwr_req(ps, evt_id);

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): try enter ips.\n", __func__);
	if (_chk_ips_enter(ps)) {
		_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
	}

	return MDL_RET_SUCCESS;
}

static void _ps_mr_info_upt(struct cmd_ps *ps, struct rtw_wifi_role_t *role)
{
	struct phl_info_t *phl_info = ps->phl_info;
	struct rtw_wifi_role_t *wr = NULL;
	u8 role_idx = 0;

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): id %d, type %d, mstate %d\n", __func__,
				role->id, role->type, role->mstate);
	#ifdef RTW_WKARD_PHL_NTFY_MEDIA_STS
	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): is_gc %d\n", __func__, role->is_gc);
	#endif

	_os_mem_set(phl_to_drvpriv(phl_info), &ps->mr_info, 0, sizeof(struct _ps_mr_info));

	for (role_idx = 0; role_idx < MAX_WIFI_ROLE_NUMBER; role_idx++) {
		wr = &(phl_info->phl_com->wifi_roles[role_idx]);
		if (wr->active == false)
			continue;
		#ifdef RTW_WKARD_PHL_NTFY_MEDIA_STS
		if (wr->type == PHL_RTYPE_STATION) {
			if(wr->is_gc == true && wr->mstate != MLME_NO_LINK)
				ps->mr_info.gc_active = true;
		}
		#endif
		if (wr->type == PHL_RTYPE_AP || wr->type == PHL_RTYPE_VAP ||
			wr->type == PHL_RTYPE_MESH ||wr->type == PHL_RTYPE_P2P_GO)
			ps->mr_info.ap_active = (wr->mstate == MLME_NO_LINK) ? false : true;
	}

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): gc_active %d, ap_active %d\n", __func__,
				ps->mr_info.gc_active, ps->mr_info.ap_active);
}

static bool _is_ignored_mrc_evt(u16 evt_id)
{
	return false;
}

static enum phl_mdl_ret_code
_mrc_mdl_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;
	struct rtw_role_cmd *rcmd = NULL;
	struct rtw_wifi_role_t *role = NULL;

	if (_is_ignored_mrc_evt(MSG_EVT_ID_FIELD(msg->msg_id)))
		return MDL_RET_SUCCESS;

	switch (MSG_EVT_ID_FIELD(msg->msg_id)) {
	case MSG_EVT_ROLE_NTFY:
		if (!IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
			if (msg->inbuf && (msg->inlen == sizeof(struct rtw_role_cmd))) {
				rcmd  = (struct rtw_role_cmd *)msg->inbuf;
				role = rcmd->wrole;
				_ps_mr_info_upt(ps, role);
			}
		}
		ret = MDL_RET_SUCCESS;
		break;
	default:
		if (ps->cur_pwr_lvl != PS_PWR_LVL_PWRON) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) get cannot I/O!\n", __func__,
		         MSG_MDL_ID_FIELD(msg->msg_id), MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_CANNOT_IO;
		} else {
			ret = MDL_RET_SUCCESS;
		}
		break;
	}

	return ret;
}

static bool _is_ignored_ser_evt(u16 evt_id)
{
	if (MSG_EVT_SER_M9_L2_RESET != evt_id)
		return true;

	return false;
}

static enum phl_mdl_ret_code
_ser_mdl_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (_is_ignored_ser_evt(MSG_EVT_ID_FIELD(msg->msg_id)))
		return MDL_RET_SUCCESS;

	switch (MSG_EVT_ID_FIELD(msg->msg_id)) {
	case MSG_EVT_SER_M9_L2_RESET:
		ret = MDL_RET_SUCCESS;
		break;
	default:
		if (ps->cur_pwr_lvl != PS_PWR_LVL_PWRON) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) get cannot I/O!\n",
			         __func__,
			         MSG_MDL_ID_FIELD(msg->msg_id),
			         MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_CANNOT_IO;
		} else {
			ret = MDL_RET_SUCCESS;
		}
		break;
	}

	return ret;
}

static bool _is_ignored_general_evt(u16 evt_id)
{
	return false;
}

#ifdef RTW_WKARD_LINUX_CMD_WKARD
static enum phl_mdl_ret_code
_linux_cmd_wkard_hdlr(struct cmd_ps *ps)
{
	if (ps->rej_pwr_req == true) {
		PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
		return MDL_RET_CANNOT_IO;
	}

	/* power request */
	if (_leave_ps(ps, true) == RTW_PHL_STATUS_SUCCESS) {
		return MDL_RET_SUCCESS;
	} else {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
		/* L2 Recovery */
	}

	return MDL_RET_CANNOT_IO;
}
#endif /* RTW_WKARD_LINUX_CMD_WKARD */

static enum phl_mdl_ret_code
_general_mdl_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (_is_ignored_general_evt(MSG_EVT_ID_FIELD(msg->msg_id)))
		return MDL_RET_SUCCESS;

	switch (MSG_EVT_ID_FIELD(msg->msg_id)) {
	case MSG_EVT_SWCH_START:
	case MSG_EVT_PCIE_TRX_MIT:
	case MSG_EVT_FORCE_USB_SW:
	case MSG_EVT_GET_USB_SPEED:
	case MSG_EVT_GET_USB_SW_ABILITY:
	case MSG_EVT_CFG_AMPDU:
	case MSG_EVT_DFS_PAUSE_TX:
	case MSG_EVT_ROLE_RECOVER:
	case MSG_EVT_ROLE_SUSPEND:
	case MSG_EVT_HAL_SET_L2_LEAVE:
	case MSG_EVT_NOTIFY_HAL:
	case MSG_EVT_ISSUE_BCN:
	case MSG_EVT_STOP_BCN:
	case MSG_EVT_SEC_KEY:
	case MSG_EVT_ROLE_START:
	case MSG_EVT_ROLE_CHANGE:
	case MSG_EVT_ROLE_STOP:
	case MSG_EVT_STA_INFO_CTRL:
	case MSG_EVT_STA_MEDIA_STATUS_UPT:
	case MSG_EVT_CFG_CHINFO:
	case MSG_EVT_STA_CHG_STAINFO:
#ifdef RTW_WKARD_LINUX_CMD_WKARD
	case MSG_EVT_LINUX_CMD_WRK_TRI_PS:
#endif /* RTW_WKARD_LINUX_CMD_WKARD */
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) in %s phase.\n", __func__,
			MSG_MDL_ID_FIELD(msg->msg_id), MSG_EVT_ID_FIELD(msg->msg_id),
			(IS_MSG_IN_PRE_PHASE(msg->msg_id) ? "pre-protocol" : "post-protocol"));
		if (IS_MSG_IN_PRE_PHASE(msg->msg_id))
			ret = _ext_msg_pre_hdlr(ps, MSG_EVT_ID_FIELD(msg->msg_id));
		else
			ret = _ext_msg_post_hdlr(ps, MSG_EVT_ID_FIELD(msg->msg_id));
		break;
#ifdef RTW_WKARD_LINUX_CMD_WKARD
	case MSG_EVT_LINUX_CMD_WRK:
		if (IS_MSG_IN_PRE_PHASE(msg->msg_id))
			ret = _linux_cmd_wkard_hdlr(ps);
		else
			ret = MDL_RET_SUCCESS;
		break;
#endif /* RTW_WKARD_LINUX_CMD_WKARD */
	case MSG_EVT_WATCHDOG:
		if (IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
			ret = _ps_watchdog_pre_hdlr(ps);
			if (ret == MDL_RET_SUCCESS) {
				ps->rssi_bcn_min = phl_get_min_rssi_bcn(ps->phl_info);
				PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): update rssi_bcn_min to %d\n", __func__, ps->rssi_bcn_min);
			}
		} else {
			ret = _ps_watchdog_post_hdlr(ps);
		}
		break;
	case MSG_EVT_RF_ON:
		if (!IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], MSG_EVT_RF_ON\n");
			ps->rej_pwr_req = false;
		}
		ret = MDL_RET_SUCCESS;
		break;
	case MSG_EVT_RF_OFF:
		if (!IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], MSG_EVT_RF_OFF\n");
			ps->rej_pwr_req = true;
			_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
		}
		ret = MDL_RET_SUCCESS;
		break;
	case MSG_EVT_PHY_ON:
		ret = _phy_on_msg_hdlr(ps, msg);
		break;
	case MSG_EVT_PHY_IDLE:
		ret = _phy_idle_msg_hdlr(ps, msg);
		break;
	default:
		if (ps->cur_pwr_lvl != PS_PWR_LVL_PWRON) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) get cannot I/O!\n",
			         __func__,
			         MSG_MDL_ID_FIELD(msg->msg_id),
			         MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_CANNOT_IO;
		} else {
			ret = MDL_RET_SUCCESS;
		}
		break;
	}

	return ret;
}

static bool _is_ignored_datapath_evt(u16 evt_id)
{
	return false;
}

static enum phl_mdl_ret_code
_datapath_mdl_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (_is_ignored_datapath_evt(MSG_EVT_ID_FIELD(msg->msg_id)))
		return MDL_RET_SUCCESS;

	switch (MSG_EVT_ID_FIELD(msg->msg_id)) {
	case MSG_EVT_TRX_PWR_REQ:
		ret = _tx_pwr_req_msg_hdlr(ps, msg);
		break;
	default:
		if (ps->cur_pwr_lvl != PS_PWR_LVL_PWRON) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) get cannot I/O!\n",
			         __func__,
			         MSG_MDL_ID_FIELD(msg->msg_id),
			         MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_CANNOT_IO;
		} else {
			ret = MDL_RET_SUCCESS;
		}
		break;
	}

	return ret;
}

/**
 * bypass msg of specific module
 * @msg: see phl_msg
 */
static bool _is_ignored_mdl(struct phl_msg *msg)
{
	if (MSG_MDL_ID_FIELD(msg->msg_id) == PHL_MDL_BTC)
		return true;
	if (MSG_MDL_ID_FIELD(msg->msg_id) == PHL_MDL_LED)
		return true;

	return false;
}

static enum phl_mdl_ret_code
_ps_mdl_hdl_external_evt(void *dispr, struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	if (_is_ignored_mdl(msg)) {
		/* PHL_INFO("[PS_CMD], %s(): ignore MDL_ID(%d)-EVT_ID(%d).\n", __func__,
		         MSG_MDL_ID_FIELD(msg->msg_id), MSG_EVT_ID_FIELD(msg->msg_id)); */
		return MDL_RET_SUCCESS;
	}

	switch (MSG_MDL_ID_FIELD(msg->msg_id)) {
	case PHL_MDL_GENERAL:
		ret = _general_mdl_msg_hdlr(ps, msg);
		break;
	case PHL_MDL_MRC:
		ret = _mrc_mdl_msg_hdlr(ps, msg);
		break;
	case PHL_MDL_TX:
	case PHL_MDL_RX:
		ret = _datapath_mdl_msg_hdlr(ps, msg);
		break;
	case PHL_MDL_SER:
		ret = _ser_mdl_msg_hdlr(ps, msg);
		break;
	/* handle ohters mdl here */
	default:
		if (ps->cur_pwr_lvl != PS_PWR_LVL_PWRON) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) get cannot I/O!\n", __func__,
		         MSG_MDL_ID_FIELD(msg->msg_id), MSG_EVT_ID_FIELD(msg->msg_id));
			ret = MDL_RET_CANNOT_IO;
		} else {
			ret = MDL_RET_SUCCESS;
		}
		break;
	}

	return ret;
}

static enum phl_mdl_ret_code _ps_cap_chg_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	if (IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): update runtime cap %d.\n", __func__, (bool)*msg->inbuf);
		ps->runtime_ps_en = (bool)*msg->inbuf;
		if (ps->rej_pwr_req == true) {
			PHL_TRACE(COMP_PHL_PS, _PHL_WARNING_, "[PS_CMD], %s(): reject pwr req.\n", __func__);
			return MDL_RET_CANNOT_IO;
		}
		if (ps->runtime_ps_en == false) {
			if (_leave_ps(ps, true) == RTW_PHL_STATUS_SUCCESS) {
				return MDL_RET_SUCCESS;
			} else {
				PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): going to L2 reset!\n", __func__);
				return MDL_RET_CANNOT_IO;
			}
		}
	} else {
		if (ps->runtime_ps_en == true) {
			PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): try enter ips.\n", __func__);
			if (_chk_ips_enter(ps)) {
				_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
			}
		}
	}

	return MDL_RET_SUCCESS;
}

static enum phl_mdl_ret_code _sw_tx_pause_msg_hdlr(struct cmd_ps *ps, struct phl_msg *msg)
{
	u16 macid = 0xFFFF;

	if (!IS_MSG_IN_PRE_PHASE(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): \n", __func__);
		if (_chk_lps_enter(ps, &macid))
			_enter_ps(ps, PS_MODE_LPS, macid);
		else if (_chk_ips_enter(ps))
			_enter_ps(ps, PS_MODE_IPS, 0xFFFF);
	}

	return MDL_RET_SUCCESS;
}

static enum phl_mdl_ret_code
_ps_mdl_hdl_internal_evt(void *dispr, struct cmd_ps *ps, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_CANNOT_IO;

	switch (MSG_EVT_ID_FIELD(msg->msg_id)) {
	case MSG_EVT_PHY_ON:
		ret = _phy_on_msg_hdlr(ps, msg);
		break;
	case MSG_EVT_PHY_IDLE:
		ret = _phy_idle_msg_hdlr(ps, msg);
		break;
	case MSG_EVT_PS_CAP_CHG:
		ret = _ps_cap_chg_msg_hdlr(ps, msg);
		break;
	case MSG_EVT_SW_TX_PAUSE:
	 	ret = _sw_tx_pause_msg_hdlr(ps, msg);
		break;
	default:
		ret = MDL_RET_CANNOT_IO;
		break;
	}

	return ret;
}

static enum phl_mdl_ret_code
_ps_mdl_msg_hdlr(void *dispr, void *priv, struct phl_msg *msg)
{
	enum phl_mdl_ret_code ret = MDL_RET_IGNORE;
	struct cmd_ps *ps = (struct cmd_ps *)priv;

	if (IS_MSG_FAIL(msg->msg_id)) {
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): MDL_ID(%d)-EVT_ID(%d) fail.\n", __func__,
		         MSG_MDL_ID_FIELD(msg->msg_id), MSG_EVT_ID_FIELD(msg->msg_id));
		return MDL_RET_SUCCESS;
	}

	switch (MSG_MDL_ID_FIELD(msg->msg_id)) {
		case PHL_MDL_POWER_MGNT:
			ret = _ps_mdl_hdl_internal_evt(dispr, ps, msg);
			break;
		default:
			ret = _ps_mdl_hdl_external_evt(dispr, ps, msg);
			break;
	}

	return ret;
}

static enum phl_mdl_ret_code
_ps_mdl_set_info(void *dispr, void *priv, struct phl_module_op_info *info)
{
	struct cmd_ps *ps = (struct cmd_ps *)priv;

	switch (info->op_code) {
	case PS_MDL_OP_CANCEL_PWR_REQ:
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): cancel pwr req, evt_id %d\n", __func__, *(u16 *)info->inbuf);
		if (ps->rej_pwr_req == false)
			_cancel_pwr_req(ps, *(u16 *)info->inbuf);
		break;
	case PS_MDL_OP_BTC_PWR_REQ:
		PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): btc req pwr %d\n", __func__, *(bool *)info->inbuf);
		ps->btc_req_pwr = *(bool *)info->inbuf;
		break;
	}

	return MDL_RET_SUCCESS;
}

static enum phl_mdl_ret_code
_ps_mdl_query_info(void *dispr, void *priv, struct phl_module_op_info *info)
{
	struct cmd_ps *ps = (struct cmd_ps *)priv;
	u8 pwr_lvl = PS_PWR_LVL_PWROFF;
	struct phl_cmd_ps_basic_info *basic_info = NULL;

	/* PHL_INFO("[PS_CMD], %s(): opcode %d.\n", __func__, info->op_code); */

	switch (info->op_code) {
	case PS_MDL_OP_CUR_PWR_LVL:
		pwr_lvl = ps->cur_pwr_lvl;
		_os_mem_cpy(phl_to_drvpriv(ps->phl_info), (void *)info->inbuf,
					&pwr_lvl, sizeof(pwr_lvl));
		break;
	case PS_MDL_OP_BASIC_INFO:
		basic_info = (struct phl_cmd_ps_basic_info *)info->inbuf;
		basic_info->ps_mode = ps->ps_mode;
		basic_info->cur_pwr_lvl = ps->cur_pwr_lvl;
		basic_info->rej_pwr_req = ps->rej_pwr_req;
		basic_info->runtime_ps_en = ps->runtime_ps_en;
		basic_info->ap_active = ps->mr_info.ap_active;
		basic_info->gc_active = ps->mr_info.gc_active;
		break;
	}

	return MDL_RET_SUCCESS;
}

enum rtw_phl_status phl_register_ps_module(struct phl_info_t *phl_info)
{
	enum rtw_phl_status phl_status = RTW_PHL_STATUS_FAILURE;
	struct phl_cmd_dispatch_engine *disp_eng = &(phl_info->disp_eng);
	struct phl_bk_module_ops bk_ops = {0};
	u8 i = 0;

	PHL_INFO("[PS_CMD], %s(): \n", __func__);

	bk_ops.init = _ps_mdl_init;
	bk_ops.deinit = _ps_mdl_deinit;
	bk_ops.start = _ps_mdl_start;
	bk_ops.stop = _ps_mdl_stop;
	bk_ops.msg_hdlr = _ps_mdl_msg_hdlr;
	bk_ops.set_info = _ps_mdl_set_info;
	bk_ops.query_info = _ps_mdl_query_info;

	for (i = 0; i < disp_eng->phy_num; i++) {
		phl_status = phl_disp_eng_register_module(phl_info, i,
						 PHL_MDL_POWER_MGNT, &bk_ops);
		if (phl_status != RTW_PHL_STATUS_SUCCESS) {
			PHL_ERR("register cmd PS module of phy%d failed.\n", i + 1);
			break;
		}
	}

	return phl_status;
}

u8 phl_ps_get_cur_pwr_lvl(struct phl_info_t *phl_info)
{
	struct phl_module_op_info op_info = {0};
	u8 pwr_lvl = PS_PWR_LVL_MAX;

	op_info.op_code = PS_MDL_OP_CUR_PWR_LVL;
	op_info.inbuf = (u8 *)&pwr_lvl;
	op_info.inlen = sizeof(pwr_lvl);

	phl_disp_eng_query_bk_module_info(phl_info, HW_BAND_0,
			PHL_MDL_POWER_MGNT, &op_info);

	PHL_TRACE(COMP_PHL_PS, _PHL_INFO_, "[PS_CMD], %s(): pwr lvl(%s)\n", __func__, phl_ps_pwr_lvl_to_str(pwr_lvl));

	return pwr_lvl;
}

bool phl_ps_is_datapath_allowed(struct phl_info_t *phl_info)
{
	struct phl_module_op_info op_info = {0};
	bool io_allowed = false;
	u8 pwr_lvl = PS_PWR_LVL_MAX;

	op_info.op_code = PS_MDL_OP_CUR_PWR_LVL;
	op_info.inbuf = (u8 *)&pwr_lvl;
	op_info.inlen = sizeof(pwr_lvl);

	phl_disp_eng_query_bk_module_info(phl_info, HW_BAND_0,
			PHL_MDL_POWER_MGNT, &op_info);

	if (pwr_lvl == PS_PWR_LVL_PWRON)
		io_allowed = true;

	return io_allowed;
}

static void _ps_set_rt_cap_done(void* priv, struct phl_msg* msg)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)priv;

	if (msg->inbuf && msg->inlen) {
		_os_mem_free(phl_to_drvpriv(phl_info),
			msg->inbuf, msg->inlen);
	}
}

void rtw_phl_ps_set_rt_cap(void *phl, u8 band_idx, bool ps_allow)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;
	struct phl_msg msg = {0};
	struct phl_msg_attribute attr = {0};
	bool *val = NULL;

	val = (bool *)_os_mem_alloc(phl_to_drvpriv(phl_info), sizeof(val));
	if (val == NULL) {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): fail to alloc memory.\n", __func__);
		return;
	}

	*val = ps_allow;

	SET_MSG_MDL_ID_FIELD(msg.msg_id, PHL_MDL_POWER_MGNT);
	SET_MSG_EVT_ID_FIELD(msg.msg_id, MSG_EVT_PS_CAP_CHG);
	msg.band_idx = band_idx;
	msg.inbuf = (u8 *)val;
	msg.inlen = sizeof(*val);

	attr.completion.completion = _ps_set_rt_cap_done;
	attr.completion.priv = phl_info;

	if (phl_disp_eng_send_msg(phl_info, &msg, &attr, NULL) !=
				RTW_PHL_STATUS_SUCCESS) {
		PHL_TRACE(COMP_PHL_PS, _PHL_ERR_, "[PS_CMD], %s(): fail to notify batter change.\n", __func__);
		goto cmd_fail;
	}

	return;

cmd_fail:
	_os_mem_free(phl_to_drvpriv(phl_info), val, sizeof(val));
}

enum rtw_phl_status
rtw_phl_ps_set_rf_state(void *phl, u8 band_idx, enum rtw_rf_state rf_state)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;
	enum phl_msg_evt_id evt_id = (rf_state == RTW_RF_ON) ? MSG_EVT_RF_ON : MSG_EVT_RF_OFF;

	phl_cmd_enqueue(phl_info, band_idx, evt_id, NULL, 0, NULL, PHL_CMD_WAIT, 0);

	return RTW_PHL_STATUS_SUCCESS;
}

enum rtw_phl_status phl_ps_hal_pwr_req(struct rtw_phl_com_t *phl_com, u8 src, bool pwr_req)
{
	enum rtw_phl_status status = RTW_PHL_STATUS_FAILURE;
	struct phl_info_t *phl_info = (struct phl_info_t *)phl_com->phl_priv;
	struct phl_module_op_info op_info = {0};

	if (src != HAL_BTC_PWR_REQ)
		return RTW_PHL_STATUS_FAILURE;

	op_info.op_code = PS_MDL_OP_BTC_PWR_REQ;
	op_info.inbuf = (u8 *)&pwr_req;
	status = phl_disp_eng_set_bk_module_info(phl_info, HW_BAND_0,
				PHL_MDL_POWER_MGNT, &op_info);

	return status;
}
