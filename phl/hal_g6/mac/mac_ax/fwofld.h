/** @file */
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

#ifndef _MAC_AX_FW_OFLD_H_
#define _MAC_AX_FW_OFLD_H_

#include "../type.h"
#include "fwcmd.h"
#include "fwofld.h"
#include "trx_desc.h"

#define READ_OFLD_MAX_LEN 2000
#define WRITE_OFLD_MAX_LEN 2000
#define CONF_OFLD_MAX_LEN 2000
#define CMD_OFLD_MAX_LEN 2000

#define CONF_OFLD_RESTORE 0
#define CONF_OFLD_BACKUP 1

#define CMD_OFLD_SIZE sizeof(struct fwcmd_cmd_ofld)

/* Generate 8-bit mask for a 4-byte alignment offset */
#define GET_W8_MSK(offset) \
	(0xFF << ((offset) & 0x3 ? (8 * ((offset) & 0x3)) : 0))

/* Generate 16-bit mask for a 4-byte alignment offset */
#define GET_W16_MSK(offset) \
	(0xFFFF << ((offset) & 0x2 ? 16 : 0))

#define MAC_REG_W8_OFLD(offset, val, lc) \
	write_mac_reg_ofld(adapter, offset & 0xFFFC, GET_W8_MSK(offset), val, lc)

#define MAC_REG_W16_OFLD(offset, val, lc) \
	write_mac_reg_ofld(adapter, offset & 0xFFFD, GET_W16_MSK(offset), val, lc)

#define MAC_REG_W32_OFLD(offset, val, lc) \
	write_mac_reg_ofld(adapter, offset, 0xFFFFFFFF, val, lc)

#define MAC_REG_W_OFLD(offset, mask, val, lc) \
	write_mac_reg_ofld(adapter, offset, mask, val, lc)

#define MAC_REG_P_OFLD(offset, mask, val, lc) \
	poll_mac_reg_ofld(adapter, offset, mask, val, lc)

#define DELAY_OFLD(val, lc) \
	poll_mac_reg_ofld(adapter, val, lc)

/**
 * @enum PKT_OFLD_OP
 *
 * @brief PKT_OFLD_OP
 *
 * @var PKT_OFLD_OP::PKT_OFLD_OP_ADD
 * Please Place Description here.
 * @var PKT_OFLD_OP::PKT_OFLD_OP_DEL
 * Please Place Description here.
 * @var PKT_OFLD_OP::PKT_OFLD_OP_READ
 * Please Place Description here.
 * @var PKT_OFLD_OP::PKT_OFLD_OP_MAX
 * Please Place Description here.
 */
enum PKT_OFLD_OP {
	PKT_OFLD_OP_ADD = 0,
	PKT_OFLD_OP_DEL = 1,
	PKT_OFLD_OP_READ = 2,
	PKT_OFLD_OP_MAX
};

/**
 * @enum FW_OFLD_OP
 *
 * @brief FW_OFLD_OP
 *
 * @var FW_OFLD_OP::FW_OFLD_OP_DUMP_EFUSE
 * Please Place Description here.
 * @var FW_OFLD_OP::FW_OFLD_OP_PACKET_OFLD
 * Please Place Description here.
 * @var FW_OFLD_OP::FW_OFLD_OP_READ_OFLD
 * Please Place Description here.
 * @var FW_OFLD_OP::FW_OFLD_OP_WRITE_OFLD
 * Please Place Description here.
 * @var FW_OFLD_OP::FW_OFLD_OP_CONF_OFLD
 * Please Place Description here.
 * @var FW_OFLD_OP::FW_OFLD_OP_MAX
 * Please Place Description here.
 */
enum FW_OFLD_OP {
	FW_OFLD_OP_DUMP_EFUSE = 0,
	FW_OFLD_OP_PACKET_OFLD = 1,
	FW_OFLD_OP_READ_OFLD = 2,
	FW_OFLD_OP_WRITE_OFLD = 3,
	FW_OFLD_OP_CONF_OFLD = 4,
	FW_OFLD_OP_MAX
};

/**
 * @struct mac_ax_conf_ofld_hdr
 * @brief mac_ax_conf_ofld_hdr
 *
 * @var mac_ax_conf_ofld_hdr::pattern_count
 * Please Place Description here.
 * @var mac_ax_conf_ofld_hdr::rsvd
 * Please Place Description here.
 */
struct mac_ax_conf_ofld_hdr {
	u16 pattern_count;
	u16 rsvd;
};

/**
 * @struct mac_ax_pkt_ofld_hdr
 * @brief mac_ax_pkt_ofld_hdr
 *
 * @var mac_ax_pkt_ofld_hdr::pkt_idx
 * Please Place Description here.
 * @var mac_ax_pkt_ofld_hdr::pkt_op
 * Please Place Description here.
 * @var mac_ax_pkt_ofld_hdr::rsvd
 * Please Place Description here.
 * @var mac_ax_pkt_ofld_hdr::pkt_len
 * Please Place Description here.
 */
struct mac_ax_pkt_ofld_hdr {
	u8 pkt_idx;
	u8 pkt_op:3;
	u8 rsvd:5;
	u16 pkt_len;
};

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */
/**
 * @brief mac_reset_fwofld_state
 *
 * @param *adapter
 * @param op
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_reset_fwofld_state(struct mac_ax_adapter *adapter, u8 op);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_check_fwofld_done
 *
 * @param *adapter
 * @param op
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_check_fwofld_done(struct mac_ax_adapter *adapter, u8 op);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_clear_write_request
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_clear_write_request(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_add_write_request
 *
 * @param *adapter
 * @param *req
 * @param *value
 * @param *mask
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_add_write_request(struct mac_ax_adapter *adapter,
			  struct mac_ax_write_req *req,
			  u8 *value, u8 *mask);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_write_ofld
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_ofld(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_clear_conf_request
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_clear_conf_request(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_add_conf_request
 *
 * @param *adapter
 * @param *req
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_add_conf_request(struct mac_ax_adapter *adapter,
			 struct mac_ax_conf_ofld_req *req);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_conf_ofld
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_conf_ofld(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_read_pkt_ofld
 *
 * @param *adapter
 * @param id
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_pkt_ofld(struct mac_ax_adapter *adapter, u8 id);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_del_pkt_ofld
 *
 * @param *adapter
 * @param id
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_del_pkt_ofld(struct mac_ax_adapter *adapter, u8 id);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_add_pkt_ofld
 *
 * @param *adapter
 * @param *pkt
 * @param len
 * @param *id
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_add_pkt_ofld(struct mac_ax_adapter *adapter, u8 *pkt, u16 len, u8 *id);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_pkt_ofld_packet
 *
 * @param *adapter
 * @param **pkt_buf
 * @param *pkt_len
 * @param *pkt_id
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pkt_ofld_packet(struct mac_ax_adapter *adapter,
			u8 **pkt_buf, u16 *pkt_len, u8 *pkt_id);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_dump_efuse_ofld
 *
 * @param *adapter
 * @param efuse_size
 * @param is_hidden
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_efuse_ofld(struct mac_ax_adapter *adapter, u32 efuse_size,
			bool is_hidden);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_efuse_ofld_map
 *
 * @param *adapter
 * @param *efuse_map
 * @param efuse_size
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_efuse_ofld_map(struct mac_ax_adapter *adapter, u8 *efuse_map,
		       u32 efuse_size);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_clear_read_request
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_clear_read_request(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_add_read_request
 *
 * @param *adapter
 * @param *req
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_add_read_request(struct mac_ax_adapter *adapter,
			 struct mac_ax_read_req *req);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_read_ofld
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_ofld(struct mac_ax_adapter *adapter);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_read_ofld_value
 *
 * @param *adapter
 * @param **val_buf
 * @param *val_len
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_ofld_value(struct mac_ax_adapter *adapter,
			u8 **val_buf, u16 *val_len);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_general_pkt_ids
 *
 * @param *adapter
 * @param *ids
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_general_pkt_ids(struct mac_ax_adapter *adapter,
			struct mac_ax_general_pkt_ids *ids);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_add_cmd_ofld
 *
 * This is the function for FW IO offload.
 * Users could call the function to add write BB/RF/MAC REG command.
 * When the aggregated commands are full or the command is last,
 * FW would receive a H2C containing aggreated IO command.
 *
 * @param *adapter
 * @param *cmd
 * @return 0 for success. Others are fail.
 * @retval u32
 */
u32 mac_add_cmd_ofld(struct mac_ax_adapter *adapter, struct rtw_mac_cmd *cmd);
/**
 * @}
 * @}
 */

/**
 * @addtogroup Firmware
 * @{
 * @addtogroup FW_Offload
 * @{
 */

/**
 * @brief mac_get_fw_cap
 *
 * @param *adapter
 * @param *val
 * @return This function would set FW capability in *val.
 * return fail while FW is NOT ready
 * @retval u32
 */
u32 mac_get_fw_cap(struct mac_ax_adapter *adapter, u32 *val);
/**
 * @}
 * @}
 */

u32 write_mac_reg_ofld(struct mac_ax_adapter *adapter,
		       u16 offset, u32 mask, u32 val, u8 lc);

u32 poll_mac_reg_ofld(struct mac_ax_adapter *adapter,
		      u16 offset, u32 mask, u32 val, u8 lc);

u32 delay_ofld(struct mac_ax_adapter *adapter, u32 val);
/**
 * @brief mac_ccxrpt_parsing
 *
 * @param *adapter
 * @param *buf
 * @param *info
 * @return Please Place Description here.
 * @retval u32
 */

u32 mac_ccxrpt_parsing(struct mac_ax_adapter *adapter,
		       u8 *buf, struct mac_ax_ccxrpt *info);

#endif
