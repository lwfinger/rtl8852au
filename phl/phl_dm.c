
/******************************************************************************
 *
 * Copyright(c) 2020 Realtek Corporation.
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
#define _PHL_DM_C_
#include "phl_headers.h"

void rtw_phl_set_edcca_mode(void *phl, enum rtw_edcca_mode mode)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;

	PHL_INFO("[Cert], set phl_com edcca_mode : %d !! \n", mode);
	phl_info->phl_com->edcca_mode = mode;
}

enum rtw_edcca_mode rtw_phl_get_edcca_mode(void *phl)
{
	struct phl_info_t *phl_info = (struct phl_info_t *)phl;

	return phl_info->phl_com->edcca_mode;
}
