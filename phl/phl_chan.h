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
#ifndef _PHL_CHANA_H_
#define _PHL_CHANA_H_

u8 phl_chanctx_get_rnum(struct phl_info_t *phl_info,
					struct phl_queue *chan_ctx_queue,
					struct rtw_chan_ctx *chanctx);

u8 phl_chanctx_get_rnum_with_lock(struct phl_info_t *phl_info,
			struct phl_queue *chan_ctx_queue,
			struct rtw_chan_ctx *chanctx);

enum rtw_phl_status
phl_chanctx_free(struct phl_info_t *phl_info, struct hw_band_ctl_t *band_ctl);

enum rtw_phl_status
phl_set_ch_bw(struct rtw_wifi_role_t *wifi_role,
		 struct rtw_chan_def *chdef, bool do_rfk);


#ifdef CONFIG_CMD_DISP
enum rtw_phl_status
phl_cmd_set_ch_bw_hdl(struct phl_info_t *phl_info, u8 *param);
enum rtw_phl_status
rtw_phl_cmd_set_ch_bw(struct rtw_wifi_role_t *wifi_role,
                      struct rtw_chan_def *chdef,
                      bool do_rfk,
                      enum phl_cmd_type cmd_type,
                      u32 cmd_timeout);
enum rtw_phl_status
phl_cmd_dfs_tx_pause_hdl(struct phl_info_t *phl_info, u8 *param);
#endif

#ifdef DBG_PHL_CHAN
void phl_chan_dump_chandef(const char *caller, const int line, bool show_caller,
						struct rtw_chan_def *chandef);
#define PHL_DUMP_CHAN_DEF(_chandef) phl_chan_dump_chandef(__FUNCTION__, __LINE__, false, _chandef);
#define PHL_DUMP_CHAN_DEF_EX(_chandef) phl_chan_dump_chandef(__FUNCTION__, __LINE__, true, _chandef);
#else
#define PHL_DUMP_CHAN_DEF(_chandef)
#define PHL_DUMP_CHAN_DEF_EX(_chandef)
#endif

#ifdef CONFIG_DBCC_SUPPORT
enum rtw_phl_status
rtw_phl_dbcc_test(void *phl, enum dbcc_test_id id, void *param);
#endif

enum band_type rtw_phl_get_band_type(u8 chan);
u8 rtw_phl_get_center_ch(u8 ch,
	enum channel_width bw, enum chan_offset offset);
u8
rtw_phl_get_operating_class(
	struct rtw_chan_def chan_def
);

bool
rtw_phl_get_chandef_from_operating_class(
	u8 channel,
	u8 operating_class,
	struct rtw_chan_def *chan_def
);
#endif /*_PHL_CHANA_H_*/
