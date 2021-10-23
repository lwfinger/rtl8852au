/******************************************************************************
 *
 * Copyright(c) 2019 Realtek Corporation. All rights reserved.
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
 ******************************************************************************/

#include "status.h"

u32 mac_get_ft_status(struct mac_ax_adapter *adapter,
		      enum mac_ax_feature mac_ft, enum mac_ax_status *stat,
		      u8 *buf, const u32 size, u32 *ret_size)

{
	struct mac_ax_ft_status *ft_stat;

	ft_stat = mac_get_ft(adapter, mac_ft);

	if (!ft_stat)
		return MACNOITEM;

	if (!buf)
		return MACNPTR;

	*stat = ft_stat->status;
	if (ft_stat->size) {
		if (size < ft_stat->size)
			return MACNOBUF;
		PLTFM_MEMCMP(buf, ft_stat->buf, ft_stat->size);
		*ret_size = ft_stat->size;
	}

	return MACSUCCESS;
}

struct mac_ax_ft_status *mac_get_ft(struct mac_ax_adapter *adapter,
				    enum mac_ax_feature mac_ft)
{
	struct mac_ax_ft_status *ft_stat = adapter->ft_stat;

	for (; ft_stat->mac_ft != MAC_AX_FT_MAX; ft_stat++) {
		if (ft_stat->mac_ft == mac_ft)
			return ft_stat;
	}

	PLTFM_MSG_ERR("The mac feature is not supported\n");

	return NULL;
}
