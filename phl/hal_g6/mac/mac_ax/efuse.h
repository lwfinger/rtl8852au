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

#ifndef _MAC_AX_EFUSE_H_
#define _MAC_AX_EFUSE_H_

#include "../type.h"
#include "fwcmd.h"

#define RSVD_EFUSE_SIZE		16
#define RSVD_CS_EFUSE_SIZE	24
#define EFUSE_WAIT_CNT		10000
#define EFUSE_C2HREG_WAIT_CNT	10000
#define EFUSE_C2HREG_RETRY_WAIT_US 1
#define EFUSE_FW_DUMP_WAIT_CNT	100000
#define OTP_PHY_SIZE		0x800

#define BT_1B_ENTRY_SIZE	0x80
#define UNLOCK_CODE		0x69

#define XTAL_SI_PWR_CUT		0x10
#define XTAL_SI_SMALL_PWR_CUT	BIT(0)
#define XTAL_SI_BIG_PWR_CUT	BIT(1)

#define XTAL_SI_LOW_ADDR	0x62
#define XTAL_SI_LOW_ADDR_SH	0
#define XTAL_SI_LOW_ADDR_MSK	0xFF

#define XTAL_SI_CTRL			0x63
#define XTAL_SI_MODE_SEL_SH	6
#define XTAL_SI_MODE_SEL_MSK	0x3
#define XTAL_SI_RDY		BIT(5)
#define XTAL_SI_HIGH_ADDR_SH	0
#define XTAL_SI_HIGH_ADDR_MSK	0x7

#define XTAL_SI_READ_VAL	0x7A
#define XTAL_SI_WRITE_VAL	0x60
#define XTAL_SI_WRITE_DATA_SH	0
#define XTAL_SI_WRITE_DATA_MSK	0xFF

/**
 * @struct mac_efuse_tbl
 * @brief mac_efuse_tbl
 *
 * @var mac_efuse_tbl::lock
 * Please Place Description here.
 */
struct mac_efuse_tbl {
	mac_ax_mutex lock;
};

/**
 * @struct mac_efuse_hidden_h2creg
 * @brief mac_efuse_hidden_h2creg
 *
 * @var mac_efuse_hidden_h2creg::rsvd0
 * Please Place Description here.
 */
struct mac_efuse_hidden_h2creg {
	/* dword0 */
	u32 rsvd0:16;
};

/**
 * @enum efuse_map_sel
 *
 * @brief efuse_map_sel
 *
 * @var efuse_map_sel::EFUSE_MAP_SEL_PHY_WL
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_PHY_BT
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_LOG
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_LOG_BT
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_PHY_OTP
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_LAST
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_MAX
 * Please Place Description here.
 * @var efuse_map_sel::EFUSE_MAP_SEL_INVALID
 * Please Place Description here.
 */
enum efuse_map_sel {
	EFUSE_MAP_SEL_PHY_WL,
	EFUSE_MAP_SEL_PHY_BT,
	EFUSE_MAP_SEL_LOG,
	EFUSE_MAP_SEL_LOG_BT,
	EFUSE_MAP_SEL_PHY_OTP,
	EFUSE_MAP_SEL_PHY_DAV,
	EFUSE_MAP_SEL_LOG_DAV,

	/* keep last */
	EFUSE_MAP_SEL_LAST,
	EFUSE_MAP_SEL_MAX = EFUSE_MAP_SEL_LAST,
	EFUSE_MAP_SEL_INVALID = EFUSE_MAP_SEL_LAST,
};

/**
 * @enum mac_info_offset
 *
 * @brief mac_info_offset
 *
 * @var mac_info_offset::OFS_ADDR_AU
 * Please Place Description here.
 * @var mac_info_offset::OFS_PID_AU
 * Please Place Description here.
 * @var mac_info_offset::OFS_VID_AU
 * Please Place Description here.
 * @var mac_info_offset::OFS_ADDR_AE
 * Please Place Description here.
 * @var mac_info_offset::OFS_DID_AE
 * Please Place Description here.
 * @var mac_info_offset::OFS_VID_AE
 * Please Place Description here.
 * @var mac_info_offset::OFS_SVID_AE
 * Please Place Description here.
 * @var mac_info_offset::OFS_SMID_AE
 * Please Place Description here.
 * @var mac_info_offset::OFS_ADDR_AS
 * Please Place Description here.
 */
enum mac_info_offset {
	/*USB*/
	OFS_ADDR_AU = 0x438,
	OFS_PID_AU = 0x432,
	OFS_VID_AU = 0x430,
	/*PCIE*/
	OFS_ADDR_AE = 0x400,
	OFS_DID_AE = 0x408,
	OFS_VID_AE = 0x406,
	OFS_SVID_AE = 0x40A,
	OFS_SMID_AE = 0x40C,
	/*SDIO*/
	OFS_ADDR_AS = 0x41A,
};

/**
 * @enum mac_info_length
 *
 * @brief mac_info_length
 *
 * @var mac_info_length::LEN_ADDR_AU
 * Please Place Description here.
 * @var mac_info_length::LEN_PID_AU
 * Please Place Description here.
 * @var mac_info_length::LEN_VID_AU
 * Please Place Description here.
 * @var mac_info_length::LEN_ADDR_AE
 * Please Place Description here.
 * @var mac_info_length::LEN_DID_AE
 * Please Place Description here.
 * @var mac_info_length::LEN_VID_AE
 * Please Place Description here.
 * @var mac_info_length::LEN_SVID_AE
 * Please Place Description here.
 * @var mac_info_length::LEN_SMID_AE
 * Please Place Description here.
 * @var mac_info_length::LEN_ADDR_AS
 * Please Place Description here.
 */
enum mac_info_length {
	/*USB*/
	LEN_ADDR_AU = 6,
	LEN_PID_AU = 2,
	LEN_VID_AU = 2,
	/*PCIE*/
	LEN_ADDR_AE = 6,
	LEN_DID_AE = 2,
	LEN_VID_AE = 2,
	LEN_SVID_AE = 2,
	LEN_SMID_AE = 2,
	/*SDIO*/
	LEN_ADDR_AS = 6,
};

/**
 * @enum mac_info_default_value
 *
 * @brief mac_info_default_value
 *
 * @var mac_info_default_value::VAL_ADDR_AU
 * Please Place Description here.
 * @var mac_info_default_value::VAL_PID_AU
 * Please Place Description here.
 * @var mac_info_default_value::VAL_VID_AU
 * Please Place Description here.
 * @var mac_info_default_value::VAL_ADDR_AE
 * Please Place Description here.
 * @var mac_info_default_value::VAL_DID_AE
 * Please Place Description here.
 * @var mac_info_default_value::VAL_VID_AE
 * Please Place Description here.
 * @var mac_info_default_value::VAL_SVID_AE
 * Please Place Description here.
 * @var mac_info_default_value::VAL_SMID_AE
 * Please Place Description here.
 * @var mac_info_default_value::VAL_ADDR_AS
 * Please Place Description here.
 */
enum mac_info_default_value {
	/*USB*/
	VAL_ADDR_AU = 0x0,
	VAL_PID_AU = 0x5A,
	VAL_VID_AU = 0xDA,
	/*PCIE*/
	VAL_ADDR_AE = 0x0,
	VAL_DID_AE = 0x52,
	VAL_VID_AE = 0xEC,
	VAL_SVID_AE = 0xEC,
	VAL_SMID_AE = 0x52,
	/*SDIO*/
	VAL_ADDR_AS = 0x0,
};

/**
 * @enum mac_checksum_offset
 *
 * @brief mac_checksum_offset
 *
 * @var mac_checksum_offset::chksum_offset_1
 * Please Place Description here.
 * @var mac_checksum_offset::chksum_offset_2
 * Please Place Description here.
 */
enum mac_checksum_offset {
	chksum_offset_1 = 0x1AC,
	chksum_offset_2 = 0x1AD,
};

/**
 * @struct mac_bank_efuse_info
 * @brief mac_bank_efuse_info
 *
 * @var mac_bank_efuse_info::phy_map
 * Please Place Description here.
 * @var mac_bank_efuse_info::log_map
 * Please Place Description here.
 * @var mac_bank_efuse_info::phy_map_valid
 * Please Place Description here.
 * @var mac_bank_efuse_info::log_map_valid
 * Please Place Description here.
 * @var mac_bank_efuse_info::efuse_end
 * Please Place Description here.
 * @var mac_bank_efuse_info::phy_map_size
 * Please Place Description here.
 * @var mac_bank_efuse_info::log_map_size
 * Please Place Description here.
 */
struct mac_bank_efuse_info {
	/* efuse_param */
	u8 **phy_map;
	u8 **log_map;
	u8 *phy_map_valid;
	u8 *log_map_valid;
	u32 *efuse_end;
	/* hw_info */
	u32 *phy_map_size;
	u32 *log_map_size;
	u32 *efuse_start;
};

/**
 * @enum mac_defeature_offset
 *
 * @brief mac_defeature_offset
 *
 * @var mac_defeature_offset::rx_spatial_stream
 * Please Place Description here.
 * @var mac_defeature_offset::rx_spatial_stream_sh
 * Please Place Description here.
 * @var mac_defeature_offset::rx_spatial_stream_msk
 * Please Place Description here.
 * @var mac_defeature_offset::bandwidth
 * Please Place Description here.
 * @var mac_defeature_offset::bandwidth_sh
 * Please Place Description here.
 * @var mac_defeature_offset::bandwidth_msk
 * Please Place Description here.
 * @var mac_defeature_offset::tx_spatial_stream
 * Please Place Description here.
 * @var mac_defeature_offset::tx_spatial_stream_sh
 * Please Place Description here.
 * @var mac_defeature_offset::tx_spatial_stream_msk
 * Please Place Description here.
 * @var mac_defeature_offset::protocol_80211
 * Please Place Description here.
 * @var mac_defeature_offset::protocol_80211_sh
 * Please Place Description here.
 * @var mac_defeature_offset::protocol_80211_msk
 * Please Place Description here.
 * @var mac_defeature_offset::NIC_router
 * Please Place Description here.
 * @var mac_defeature_offset::NIC_router_sh
 * Please Place Description here.
 * @var mac_defeature_offset::NIC_router_msk
 * Please Place Description here.
 */
enum mac_defeature_offset {
	rx_spatial_stream = 0xB,
	rx_spatial_stream_sh = 0x4,
	rx_spatial_stream_msk = 0x7,
	bandwidth = 0xD,
	bandwidth_sh = 0x0,
	bandwidth_msk = 0x7,
	tx_spatial_stream = 0xD,
	tx_spatial_stream_sh = 0x4,
	tx_spatial_stream_msk = 0x7,
	protocol_80211 = 0x11,
	protocol_80211_sh = 0x2,
	protocol_80211_msk = 0x3,
	NIC_router = 0x11,
	NIC_router_sh = 0x6,
	NIC_router_msk = 0x3,
};

/**
 * @enum mac_cntlr_mode_sel
 *
 * @brief mac_cntlr_mode_sel
 *
 * @var mac_cntlr_mode_sel::MODE_READ
 * Please Place Description here.
 * @var mac_cntlr_mode_sel::MODE_AUTOLOAD_EN
 * Please Place Description here.
 * @var mac_cntlr_mode_sel::MODE_WRITE
 * Please Place Description here.
 * @var mac_cntlr_mode_sel::MODE_CMP
 * Please Place Description here.
 */
enum mac_cntlr_mode_sel {
	MODE_READ,
	MODE_AUTOLOAD_EN,
	MODE_WRITE,
	MODE_CMP,
};

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_efuse_map_wl
 *
 * @param *adapter
 * @param cfg
 * @param *efuse_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_efuse_map_wl_plus(struct mac_ax_adapter *adapter,
			       enum mac_ax_efuse_read_cfg cfg, u8 *efuse_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_efuse_map_wl
 *
 * @param *adapter
 * @param cfg
 * @param *efuse_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_efuse_map_wl(struct mac_ax_adapter *adapter,
			  enum mac_ax_efuse_read_cfg cfg, u8 *efuse_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_efuse_map_bt
 *
 * @param *adapter
 * @param cfg
 * @param *efuse_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_efuse_map_bt(struct mac_ax_adapter *adapter,
			  enum mac_ax_efuse_read_cfg cfg, u8 *efuse_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_write_efuse
 *
 * @param *adapter
 * @param addr
 * @param val
 * @param bank
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_efuse_plus(struct mac_ax_adapter *adapter, u32 addr, u8 val,
			 enum mac_ax_efuse_bank bank);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_write_efuse
 *
 * @param *adapter
 * @param addr
 * @param val
 * @param bank
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_efuse(struct mac_ax_adapter *adapter, u32 addr, u8 val,
		    enum mac_ax_efuse_bank bank);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_efuse_plus
 *
 * @param *adapter
 * @param addr
 * @param size
 * @param *val
 * @param bank
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_efuse_plus(struct mac_ax_adapter *adapter, u32 addr, u32 size,
			u8 *val, enum mac_ax_efuse_bank bank);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_efuse
 *
 * @param *adapter
 * @param addr
 * @param size
 * @param *val
 * @param bank
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_efuse(struct mac_ax_adapter *adapter, u32 addr, u32 size, u8 *val,
		   enum mac_ax_efuse_bank bank);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_get_efuse_avl_size
 *
 * @param *adapter
 * @param *size
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_get_efuse_avl_size(struct mac_ax_adapter *adapter, u32 *size);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_get_efuse_avl_size_bt
 *
 * @param *adapter
 * @param *size
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_get_efuse_avl_size_bt(struct mac_ax_adapter *adapter, u32 *size);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_log_efuse
 *
 * @param *adapter
 * @param parser_cfg
 * @param cfg
 * @param *efuse_map
 * @param is_limit
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_log_efuse_plus(struct mac_ax_adapter *adapter,
			    enum mac_ax_efuse_parser_cfg parser_cfg,
			    enum mac_ax_efuse_read_cfg cfg,
			    u8 *efuse_map, bool is_limit);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_log_efuse
 *
 * @param *adapter
 * @param parser_cfg
 * @param cfg
 * @param *efuse_map
 * @param is_limit
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_log_efuse(struct mac_ax_adapter *adapter,
		       enum mac_ax_efuse_parser_cfg parser_cfg,
		       enum mac_ax_efuse_read_cfg cfg,
		       u8 *efuse_map, bool is_limit);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_log_efuse_plus
 *
 * @param *adapter
 * @param addr
 * @param size
 * @param *val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_log_efuse_plus(struct mac_ax_adapter *adapter, u32 addr, u32 size,
			    u8 *val);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_log_efuse
 *
 * @param *adapter
 * @param addr
 * @param size
 * @param *val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_log_efuse(struct mac_ax_adapter *adapter, u32 addr, u32 size,
		       u8 *val);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_write_log_efuse_plus
 *
 * @param *adapter
 * @param addr
 * @param val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_log_efuse_plus(struct mac_ax_adapter *adapter, u32 addr, u8 val);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_write_log_efuse
 *
 * @param *adapter
 * @param addr
 * @param val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_log_efuse(struct mac_ax_adapter *adapter, u32 addr, u8 val);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_dump_log_efuse_bt
 *
 * @param *adapter
 * @param parser_cfg
 * @param cfg
 * @param *efuse_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_dump_log_efuse_bt(struct mac_ax_adapter *adapter,
			  enum mac_ax_efuse_parser_cfg parser_cfg,
			  enum mac_ax_efuse_read_cfg cfg,
			  u8 *efuse_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_log_efuse_bt
 *
 * @param *adapter
 * @param addr
 * @param size
 * @param *val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_log_efuse_bt(struct mac_ax_adapter *adapter, u32 addr, u32 size,
			  u8 *val);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_write_log_efuse_bt
 *
 * @param *adapter
 * @param addr
 * @param val
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_write_log_efuse_bt(struct mac_ax_adapter *adapter, u32 addr, u8 val);

/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_efuse_by_map_plus
 *
 * @param *adapter
 * @param *info
 * @param cfg
 * @param part
 * @param is_limit
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_efuse_by_map_plus(struct mac_ax_adapter *adapter,
			     struct mac_ax_pg_efuse_info *info,
			     enum mac_ax_efuse_read_cfg cfg,
			     bool part, bool is_limit);

/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_efuse_by_map
 *
 * @param *adapter
 * @param *info
 * @param cfg
 * @param part
 * @param is_limit
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_efuse_by_map(struct mac_ax_adapter *adapter,
			struct mac_ax_pg_efuse_info *info,
			enum mac_ax_efuse_read_cfg cfg, bool part,
			bool is_limit);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_efuse_by_map_bt
 *
 * @param *adapter
 * @param *info
 * @param cfg
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_efuse_by_map_bt(struct mac_ax_adapter *adapter,
			   struct mac_ax_pg_efuse_info *info,
			   enum mac_ax_efuse_read_cfg cfg);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_mask_log_efuse
 *
 * @param *adapter
 * @param *info
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_mask_log_efuse(struct mac_ax_adapter *adapter,
		       struct mac_ax_pg_efuse_info *info);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_sec_data_by_map
 *
 * @param *adapter
 * @param *info
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_sec_data_by_map(struct mac_ax_adapter *adapter,
			   struct mac_ax_pg_efuse_info *info);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_cmp_sec_data_by_map
 *
 * @param *adapter
 * @param *info
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_cmp_sec_data_by_map(struct mac_ax_adapter *adapter,
			    struct mac_ax_pg_efuse_info *info);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_get_efuse_info
 *
 * @param *adapter
 * @param *efuse_map
 * @param id
 * @param *value
 * @param length
 * @param *autoload_status
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_get_efuse_info(struct mac_ax_adapter *adapter, u8 *efuse_map,
		       enum rtw_efuse_info id, void *value, u32 length,
		       u8 *autoload_status);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_set_efuse_info
 *
 * @param *adapter
 * @param *efuse_map
 * @param id
 * @param *value
 * @param length
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_set_efuse_info(struct mac_ax_adapter *adapter, u8 *efuse_map,
		       enum rtw_efuse_info id, void *value, u32 length);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_read_hidden_rpt
 *
 * @param *adapter
 * @param *rpt
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_read_hidden_rpt(struct mac_ax_adapter *adapter,
			struct mac_defeature_value *rpt);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_check_efuse_autoload
 *
 * @param *adapter
 * @param *autoload_status
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_check_efuse_autoload(struct mac_ax_adapter *adapter,
			     u8 *autoload_status);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_simulator_plus
 *
 * @param *adapter
 * @param *info
 * @param *phy_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_simulator_plus(struct mac_ax_adapter *adapter,
			  struct mac_ax_pg_efuse_info *info, u8 *phy_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_pg_simulator
 *
 * @param *adapter
 * @param *info
 * @param *phy_map
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_pg_simulator(struct mac_ax_adapter *adapter,
		     struct mac_ax_pg_efuse_info *info, u8 *phy_map);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_checksum_update
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_checksum_update(struct mac_ax_adapter *adapter);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_checksum_rpt
 *
 * @param *adapter
 * @param *chksum
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_checksum_rpt(struct mac_ax_adapter *adapter, u16 *chksum);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_set_efuse_ctrl
 *
 * @param *adapter
 * @param is_secure
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_set_efuse_ctrl(struct mac_ax_adapter *adapter, bool is_secure);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief mac_otp_test
 *
 * @param *adapter
 * @param is_OTP_test
 * @return Please Place Description here.
 * @retval u32
 */
u32 mac_otp_test(struct mac_ax_adapter *adapter, bool is_OTP_test);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief cfg_efuse_auto_ck
 *
 * @param *adapter
 * @param enable
 * @return Please Place Description here.
 * @retval void
 */
void cfg_efuse_auto_ck(struct mac_ax_adapter *adapter, u8 enable);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief efuse_tbl_init
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 efuse_tbl_init(struct mac_ax_adapter *adapter);
/**
 * @}
 */

/**
 * @addtogroup Efuse
 * @{
 */

/**
 * @brief efuse_tbl_exit
 *
 * @param *adapter
 * @return Please Place Description here.
 * @retval u32
 */
u32 efuse_tbl_exit(struct mac_ax_adapter *adapter);
/**
 * @}
 */
#endif
