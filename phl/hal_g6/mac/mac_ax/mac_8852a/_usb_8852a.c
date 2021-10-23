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
#include "_usb_8852a.h"
#include "../../mac_ax.h"

#if MAC_AX_USB_SUPPORT

u8 reg_read8_usb_8852a(struct mac_ax_adapter *adapter, u32 addr)
{
	u8 offset, count = 0;
	u32 val;

	offset = addr % MAC_REG_OFFSET;
	val = PLTFM_REG_R32(addr - offset);
	if (addr >= R_AX_CMAC_FUNC_EN && addr < R_AX_CMAC_REG_END) {
		while (count < MAC_REG_POOL_COUNT) {
			if (val != MAC_AX_R32_DEAD)
				break;

			PLTFM_MSG_ERR("[ERR]addr 0x%x = 0xdeadbeef\n", addr + offset);
			PLTFM_REG_W32(R_AX_CK_EN, CMAC_CLK_ALLEN);
			val = PLTFM_REG_R32(addr);
			count++;
		}
	}

	return (u8)(val >> (offset * MAC_REG_OFFSET_SH));
}

void reg_write8_usb_8852a(struct mac_ax_adapter *adapter, u32 addr, u8 val)
{
	PLTFM_REG_W8(addr, val);
}

u16 reg_read16_usb_8852a(struct mac_ax_adapter *adapter, u32 addr)
{
	u8 offset, count = 0;
	u32 val;

	offset = addr % MAC_REG_OFFSET;
	val = PLTFM_REG_R32(addr - offset);
	if (addr >= R_AX_CMAC_FUNC_EN && addr < R_AX_CMAC_REG_END) {
		while (count < MAC_REG_POOL_COUNT) {
			if (val != MAC_AX_R32_DEAD)
				break;

			PLTFM_MSG_ERR("[ERR]addr 0x%x = 0xdeadbeef\n", addr + offset);
			PLTFM_REG_W32(R_AX_CK_EN, CMAC_CLK_ALLEN);
			val = PLTFM_REG_R32(addr);
			count++;
		}
	}

	return (u16)(val >> (offset * MAC_REG_OFFSET_SH));
}

void reg_write16_usb_8852a(struct mac_ax_adapter *adapter, u32 addr, u16 val)
{
	PLTFM_REG_W16(addr, val);
}

u32 reg_read32_usb_8852a(struct mac_ax_adapter *adapter, u32 addr)
{
	u8 count = 0;
	u32 val = PLTFM_REG_R32(addr);

	if (addr >= R_AX_CMAC_FUNC_EN && addr < R_AX_CMAC_REG_END) {
		while (count < MAC_REG_POOL_COUNT) {
			if (val != MAC_AX_R32_DEAD)
				break;

			PLTFM_MSG_ERR("[ERR]addr 0x%x = 0xdeadbeef\n", addr);
			PLTFM_REG_W32(R_AX_CK_EN, CMAC_CLK_ALLEN);
			val = PLTFM_REG_R32(addr);
			count++;
		}
	}

	return val;
}

void reg_write32_usb_8852a(struct mac_ax_adapter *adapter, u32 addr, u32 val)
{
	PLTFM_REG_W32(addr, val);
}

u8 get_bulkout_id_8852a(struct mac_ax_adapter *adapter, u8 ch_dma, u8 mode)
{
	u8 bulkout_id = 0;

	if (mode == 0 && adapter->usb_info.ep5 && adapter->usb_info.ep6 &&
	    adapter->usb_info.ep12) {
		switch (ch_dma) {
		case MAC_AX_DMA_ACH0:
			bulkout_id = BULKOUTID3;
			break;
		case MAC_AX_DMA_ACH2:
			bulkout_id = BULKOUTID5;
			break;
		case MAC_AX_DMA_ACH4:
			bulkout_id = BULKOUTID4;
			break;
		case MAC_AX_DMA_ACH6:
			bulkout_id = BULKOUTID6;
			break;
		case MAC_AX_DMA_B0MG:
		case MAC_AX_DMA_B0HI:
			bulkout_id = BULKOUTID0;
			break;
		case MAC_AX_DMA_B1MG:
		case MAC_AX_DMA_B1HI:
			bulkout_id = BULKOUTID1;
			break;
		case MAC_AX_DMA_H2C:
			bulkout_id = BULKOUTID2;
			break;
		default:
			return USBEPMAPERR;
		}
	} else if ((mode == 1) && adapter->usb_info.ep5 &&
			adapter->usb_info.ep6 && adapter->usb_info.ep12) {
		switch (ch_dma) {
		case MAC_AX_DMA_ACH0:
			bulkout_id = BULKOUTID2;
			break;
		case MAC_AX_DMA_ACH2:
			bulkout_id = BULKOUTID4;
			break;
		case MAC_AX_DMA_ACH4:
			bulkout_id = BULKOUTID3;
			break;
		case MAC_AX_DMA_ACH6:
			bulkout_id = BULKOUTID5;
			break;
		case MAC_AX_DMA_B0MG:
		case MAC_AX_DMA_B0HI:
		case MAC_AX_DMA_B1MG:
		case MAC_AX_DMA_B1HI:
			bulkout_id = BULKOUTID0;
			break;
		case MAC_AX_DMA_H2C:
			bulkout_id = BULKOUTID2;
			break;
		default:
			bulkout_id = USBEPMAPERR;
		}
	} else {
		bulkout_id = USBEPMAPERR;
	}
	return bulkout_id;
}

u32 usb_pre_init_8852a(struct mac_ax_adapter *adapter, void *param)
{
	u32 val32 = 0;

	val32 = PLTFM_REG_R32(R_AX_USB_HOST_REQUEST_2) | B_AX_R_USBIO_MODE;
	PLTFM_REG_W32(R_AX_USB_HOST_REQUEST_2, val32);
	// fix USB IO hang suggest by chihhanli@realtek.com
	val32 = PLTFM_REG_R32(R_AX_USB_WLAN0_1) & ~(B_AX_USBRX_RST | B_AX_USBTX_RST);
	PLTFM_REG_W32(R_AX_USB_WLAN0_1, val32);

	val32 = PLTFM_REG_R32(R_AX_HCI_FUNC_EN);
	val32 &= ~B_AX_HCI_RXDMA_EN;
	val32 &= ~B_AX_HCI_TXDMA_EN;
	PLTFM_REG_W32(R_AX_HCI_FUNC_EN, val32);
	val32 |= B_AX_HCI_RXDMA_EN;
	val32 |= B_AX_HCI_TXDMA_EN;
	PLTFM_REG_W32(R_AX_HCI_FUNC_EN, val32);
	// fix USB TRX hang suggest by chihhanli@realtek.com

	val32 = PLTFM_REG_R32(R_AX_USB_ENDPOINT_3);
	if ((val32 & B_AX_BULKOUT0) == B_AX_BULKOUT0)
		adapter->usb_info.ep5 = ENABLE;
	if ((val32 & B_AX_BULKOUT1) == B_AX_BULKOUT1)
		adapter->usb_info.ep6 = ENABLE;
	if (((PLTFM_REG_R32(R_AX_USB_ENDPOINT_3) >> B_AX_AC_BULKOUT_SH) &
		B_AX_AC_BULKOUT_MSK) == 1)
		adapter->usb_info.ep10 = ENABLE;
	if (((PLTFM_REG_R32(R_AX_USB_ENDPOINT_3) >> B_AX_AC_BULKOUT_SH) &
		B_AX_AC_BULKOUT_MSK) == 2) {
		adapter->usb_info.ep10 = ENABLE;
		adapter->usb_info.ep11 = ENABLE;
	}
	if (((PLTFM_REG_R32(R_AX_USB_ENDPOINT_3) >> B_AX_AC_BULKOUT_SH) &
		B_AX_AC_BULKOUT_MSK) == 3) {
		adapter->usb_info.ep10 = ENABLE;
		adapter->usb_info.ep11 = ENABLE;
		adapter->usb_info.ep12 = ENABLE;
	}
	return MACSUCCESS;
}

u32 usb_init_8852a(struct mac_ax_adapter *adapter, void *param)
{
	u32 val32;
	u8 val8;

	adapter->usb_info.max_bulkout_wd_num = GET_FIELD
		(PLTFM_REG_R32(R_AX_CH_PAGE_CTRL), B_AX_PREC_PAGE_CH011);

	val32 = PLTFM_REG_R32(R_AX_USB3_MAC_NPI_CONFIG_INTF_0);
	val32 &= ~B_AX_SSPHY_LFPS_FILTER;
	PLTFM_REG_W32(R_AX_USB3_MAC_NPI_CONFIG_INTF_0, val32);

	val32 = get_usb_mode(adapter);
	if (val32 == MAC_AX_USB3)
		PLTFM_REG_W8(R_AX_RXDMA_SETTING, USB3_BULKSIZE);
	else if (val32 == MAC_AX_USB2)
		PLTFM_REG_W8(R_AX_RXDMA_SETTING, USB2_BULKSIZE);
	else if (val32 == MAC_AX_USB11)
		PLTFM_REG_W8(R_AX_RXDMA_SETTING, USB11_BULKSIZE);
	else
		return MACHWNOSUP;

	mac_read_log_efuse_plus(adapter, R_AX_SYS_SDIO_CTRL, EFUSESIZE, &val8);
	if (val8 & B_AX_RES_USB_MASS_STORAGE_DESC)
		adapter->usb_info.usb_mode = MAC_AX_USB_AUTOINSTALL;
	else
		adapter->usb_info.usb_mode = MAC_AX_USB_NORM;

	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP5, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP6, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP7, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP9, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP10, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP11, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	val8 = PLTFM_REG_R8(R_AX_USB_ENDPOINT_0);
	val8 = SET_CLR_WORD(val8, EP12, B_AX_EP_IDX);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_0, val8);
	PLTFM_REG_W8(R_AX_USB_ENDPOINT_2 + 1, NUMP);
	return MACSUCCESS;
}

u32 usb_deinit_8852a(struct mac_ax_adapter *adapter, void *param)
{
	return MACSUCCESS;
}

u32 read_usb2phy_para_8852a(struct mac_ax_adapter *adapter, u16 offset)
{
	u32 value32 = 0;
	u8 rdata = 0;

	value32 = SET_CLR_WORD(value32, offset - phyoffset,
			       B_AX_USB_SIE_INTF_ADDR);
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_REG_EN;
	value32 |= B_AX_USB_REG_STATUS;
	PLTFM_REG_W32(R_AX_USB_SIE_INTF, value32);

	while (PLTFM_REG_R32(R_AX_USB_SIE_INTF) & B_AX_USB_REG_EN)
		;

	rdata = GET_FIELD(PLTFM_REG_R32(R_AX_USB_SIE_INTF),
			  B_AX_USB_SIE_INTF_RD);

	//DD-Yingli suggest that shall clear it if read operation is done.
	PLTFM_REG_W32(R_AX_USB_SIE_INTF, 0);
	return rdata;
}

u32 write_usb2phy_para_8852a(struct mac_ax_adapter *adapter, u16 offset, u8 val)
{
	u32 value32 = 0;

	value32 = SET_CLR_WORD(value32, val, B_AX_USB_SIE_INTF_WD);
	value32 = SET_CLR_WORD(value32, offset, B_AX_USB_SIE_INTF_ADDR);
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_WRITE_EN;
	value32 |= B_AX_USB_REG_EN;
	value32 |= B_AX_USB_REG_STATUS;

	PLTFM_REG_W32(R_AX_USB_SIE_INTF, value32);
	while (PLTFM_REG_R32(R_AX_USB_SIE_INTF) & B_AX_USB_REG_EN)
		;

	//DD-Yingli suggest that shall clear it if write operation is done.
	PLTFM_REG_W32(R_AX_USB_SIE_INTF, 0);

	return MACSUCCESS;
}

u32 static polling_usb_sie_ready(struct mac_ax_adapter *adapter)
{
#define MAC_AX_POLL_SIE_CNT 1000
#define MAC_AX_POLL_SIE_WAIT_US 50
	u32 cnt = MAC_AX_POLL_SIE_CNT;
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);

	while (cnt--) {
		if (!(MAC_REG_R32(R_AX_USB_SIE_INTF) & B_AX_USB_REG_EN))
			return MACSUCCESS;
		PLTFM_DELAY_US(MAC_AX_POLL_SIE_WAIT_US);
	}

	PLTFM_MSG_ERR("%s: polling SIE timeout\n", __func__);

	return MACPOLLTO;
}

u32 read_usb3phy_para_8852a(struct mac_ax_adapter *adapter, u16 offset, u8 b_sel)
{
	u32 value32 = 0, ret;
	u16 rdata = 0;
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);

	value32 = SET_CLR_WORD(value32, offset + USBPHYOFFSET,
			       B_AX_USB_SIE_INTF_ADDR);
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_REG_EN;
	if (b_sel)
		value32 |= B_AX_USB_PHY_BYTE_SEL;

	MAC_REG_W32(R_AX_USB_SIE_INTF, value32);
	ret = polling_usb_sie_ready(adapter);
	if (ret != MACSUCCESS)
		PLTFM_MSG_ERR("%s: polling timeout\n");
	rdata = GET_FIELD(PLTFM_REG_R32(R_AX_USB_SIE_INTF),
			  B_AX_USB_SIE_INTF_RD);

	return rdata;
}

u32 write_usb3phy_para_8852a(struct mac_ax_adapter *adapter,
			     u16 offset, u8 b_sel, u8 val)
{
	u32 value32 = 0, ret;
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);

	value32 = SET_CLR_WORD(value32, val, B_AX_USB_SIE_INTF_WD);
	value32 = SET_CLR_WORD(value32, offset + USBPHYOFFSET,
			       B_AX_USB_SIE_INTF_ADDR);
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_REG_SEL;
	value32 |= B_AX_USB_WRITE_EN;
	value32 |= B_AX_USB_REG_EN;
	if (b_sel)
		value32 |= B_AX_USB_PHY_BYTE_SEL;
	MAC_REG_W32(R_AX_USB_SIE_INTF, value32);

	ret = polling_usb_sie_ready(adapter);
	if (ret != MACSUCCESS)
		PLTFM_MSG_ERR("%s: polling timeout\n");

	return ret;
}

u32 u2u3_switch_8852a(struct mac_ax_adapter *adapter)
{
	u32 ret = 0;

	PLTFM_REG_W8(R_AX_PAD_CTRL2 + 1, USB_SWITCH_DELAY);
	ret = get_usb_mode(adapter);
	if (ret == MAC_AX_USB2)
		PLTFM_REG_W8(R_AX_PAD_CTRL2 + 2, U2SWITCHU3);
	else if (ret == MAC_AX_USB3)
		PLTFM_REG_W8(R_AX_PAD_CTRL2 + 2, U3SWITCHU2);
	else
		PLTFM_REG_W8(R_AX_PAD_CTRL2 + 2, U2SWITCHU3);

	return MACSUCCESS;
}

u32 get_usb_support_ability_8852a(struct mac_ax_adapter *adapter)
{
	u32 u2force = 0;
	u32 u3force = 0;

	u2force = PLTFM_REG_R32(R_AX_USB_HOST_REQUEST_2) &
		  B_AX_R_FORCE_U3MAC_HS_MODE;
	u3force = PLTFM_REG_R32(R_AX_PAD_CTRL2) &
		  B_AX_USB3_USB2_TRANSITION;

	if (u2force == B_AX_R_FORCE_U3MAC_HS_MODE)
		return FORCEUSB2MODE;
	else if (u3force == B_AX_USB3_USB2_TRANSITION)
		return SWITCHMODE;
	else
		return FORCEUSB3MODE;
}

u32 usb_tx_agg_cfg_8852a(struct mac_ax_adapter *adapter,
			 struct mac_ax_usb_tx_agg_cfg *agg)
{
	u32 dw1 = ((struct wd_body_usb_8852a *)agg->pkt)->dword1;

	((struct wd_body_usb_8852a *)agg->pkt)->dword1 =
		SET_CLR_WORD(dw1, agg->agg_num, AX_TXD_DMA_TXAGG_NUM);
	return MACSUCCESS;
}

u32 usb_rx_agg_cfg_8852a(struct mac_ax_adapter *adapter,
			 struct mac_ax_rx_agg_cfg *cfg)
{
	u8 size;
	u8 timeout;
	u8 agg_en;
	u8 agg_mode;
	u8 pkt_num;
	u32 val32;
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);

	if (cfg->mode == MAC_AX_RX_AGG_MODE_DMA) {
		agg_en = ENABLE;
		agg_mode = ENABLE;
	} else if (cfg->mode == MAC_AX_RX_AGG_MODE_USB) {
		agg_en = ENABLE;
		agg_mode = DISABLE;
	} else {
		agg_en = DISABLE;
		agg_mode = DISABLE;
	}

	if (cfg->thold.drv_define == 0) {
		size = RXAGGSIZE;
		timeout = RXAGGTO;
		pkt_num = 0;
	} else {
		size = cfg->thold.size;
		timeout = cfg->thold.timeout;
		pkt_num = cfg->thold.pkt_num;
	}

	val32 = MAC_REG_R32(R_AX_RXAGG_0);
	MAC_REG_W32(R_AX_RXAGG_0, (agg_en ? B_AX_RXAGG_EN : 0) |
		    (agg_mode ? B_AX_RXAGG_DMA_STORE : 0) |
		    (val32 & B_AX_RXAGG_SW_EN) |
		    SET_WORD(pkt_num, B_AX_RXAGG_PKTNUM_TH) |
		    SET_WORD(timeout, B_AX_RXAGG_TIMEOUT_TH) |
		    SET_WORD(size, B_AX_RXAGG_LEN_TH));
	return MACSUCCESS;
}

u32 usb_pwr_switch_8852a(void *vadapter, u8 pre_switch, u8 on)
{
	return MACSUCCESS;
}

u32 set_usb_wowlan_8852a(struct mac_ax_adapter *adapter,
			 enum mac_ax_wow_ctrl w_c)
{
	struct mac_ax_intf_ops *ops = adapter_to_intf_ops(adapter);

	if (w_c == MAC_AX_WOW_ENTER) {
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) |
			    B_AX_WLOCK_1C_B6);
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) |
			    B_AX_R_DIS_PRST);
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) &
			    ~B_AX_WLOCK_1C_B6);
	} else if (w_c == MAC_AX_WOW_LEAVE) {
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) |
			    B_AX_WLOCK_1C_B6);
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) &
			    ~B_AX_R_DIS_PRST);
		MAC_REG_W32(R_AX_RSV_CTRL, MAC_REG_R32(R_AX_RSV_CTRL) &
			    ~B_AX_WLOCK_1C_B6);
	} else {
		PLTFM_MSG_ERR("[ERR] Invalid WoWLAN input.\n");
		return MACFUNCINPUT;
	}

	return MACSUCCESS;
}

u32 usb_get_txagg_num_8852a(struct mac_ax_adapter *adapter, u8 band)
{
	u32 quotanum = band ? adapter->dle_info.c1_tx_min : adapter->dle_info.c0_tx_min;

	return quotanum * PLE_PAGE_SIZE / (PINGPONG * (SINGLE_MSDU_SIZE + SEC_FCS_SIZE));
}
#endif /* #if MAC_AX_USB_SUPPORT */
