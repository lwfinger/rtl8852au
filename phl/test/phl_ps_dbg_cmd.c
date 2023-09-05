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
#define _PHL_PS_DBG_CMD_C_
#include "../phl_headers.h"

struct phl_ps_cmd_info {
	char name[16];
	u8 id;
};

enum PHL_PS_CMD_ID {
	PHL_PS_HELP,
	PHL_PS_SHOW,
	PHL_PS_TEST
};

struct phl_ps_cmd_info phl_ps_cmd_i[] = {
	{"-h", PHL_PS_HELP},
	{"show", PHL_PS_SHOW},
	{"test", PHL_PS_TEST}
};

/* echo phl ps show */
static void _phl_ps_cmd_show(struct phl_info_t *phl_info, u32 *used, char input[][MAX_ARGV],
			u32 input_num, char *output, u32 out_len)
{
	if (phl_info == NULL)
		return;

	phl_ps_dbg_dump(phl_info, used, input, input_num, output, out_len);
}

void phl_ps_cmd_parser(struct phl_info_t *phl_info, char input[][MAX_ARGV],
			u32 input_num, char *output, u32 out_len)
{
	u32 used = 0;
	u8 id = 0;
	u32 i;
	u32 array_size = sizeof(phl_ps_cmd_i) / sizeof(struct phl_ps_cmd_info);

	PS_CNSL(out_len, used, output + used, out_len - used, "\n");

	/* Parsing Cmd ID */
	if (input_num) {
		for (i = 0; i < array_size; i++) {
			if (!_os_strcmp(phl_ps_cmd_i[i].name, input[1])) {
				id = phl_ps_cmd_i[i].id;
				break;
			}
		}
	}

	switch (id) {
	case PHL_PS_TEST:
		break;
	case PHL_PS_SHOW:
		_phl_ps_cmd_show(phl_info, &used, input, input_num,
					output, out_len);
		break;
	default:
		PS_CNSL(out_len, used, output + used, out_len - used,
			 "command not supported !!\n");

		fallthrough; /* fall through */
	case PHL_PS_HELP:
		PS_CNSL(out_len, used, output + used, out_len - used,
			 "PS cmd ==>\n");

		for (i = 0; i < array_size - 1; i++)
			PS_CNSL(out_len, used, output + used, out_len - used,
				 " %s\n", phl_ps_cmd_i[i + 1].name);
		break;
	}
}

void phl_ps_dbg_dump(struct phl_info_t *phl_info, u32 *used,
	char input[][MAX_ARGV], u32 input_num, char *output, u32 out_len)
{
	struct phl_module_op_info op_info = {0};
	struct phl_cmd_ps_basic_info info = {0};

	PS_CNSL(out_len, *used, output + *used, out_len - *used,
		"========== PHL PS Info ==========\n");

	op_info.op_code = PS_MDL_OP_BASIC_INFO;
	op_info.inbuf = (u8 *)&info;
#ifdef CONFIG_CMD_DISP
	if(phl_disp_eng_query_bk_module_info(phl_info, HW_BAND_0,
			PHL_MDL_POWER_MGNT, &op_info) != RTW_PHL_STATUS_SUCCESS)
		return;

	PS_CNSL(out_len, *used, output + *used, out_len - *used,
		"ps_mode: %s, cur_pwr_lvl: %s\
		\nreject all pwr req: %s\
		\nruntime ps capability: %s\
		\nap_active: %s, gc_active: %s\n",
		phl_ps_ps_mode_to_str(info.ps_mode), phl_ps_pwr_lvl_to_str(info.cur_pwr_lvl),
		(info.rej_pwr_req == true ? "yes" : "no"),
		(info.runtime_ps_en == true ? "allowed" : "disallowed"),
		(info.ap_active == true ? "yes" : "no"), (info.gc_active == true ? "yes" : "no"));
#else
	PS_CNSL(out_len, *used, output + *used, out_len - *used,
		"Not support.\n");
#endif
}
