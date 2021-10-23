/******************************************************************************
 *
 * Copyright(c) 2007 - 2020  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
#include "halbb_precomp.h"

#ifdef HALBB_LA_MODE_SUPPORT
#define LAMODE_ECHO_CMD 1
#define LAMODE_MAIN 1
#define SET_BB_TRIG_RULE 1
#define SET_BB_DMA_FMT 1
#define SET_MAC_CFG 1
#define SET_MAC_TRIG 1
#define SET_MAC_GET_BUF_RPT 1

#if (SET_MAC_GET_BUF_RPT)

u8 halbb_la_ptrn_chk(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;
	u32 i = 0, idx = 0;
	u8 ptrn_match = 1;
	u8 ptrn_match_num = 0;
	u32 shift = 0;
	u32 point_tmp = 0;
	u32 mask_tmp = 0;
	u32 val_tmp = 0;
	u32 data_msb_tmp = 0;

	for (i = 0; i < LA_CHK_PTRN_NUM; i++) {
		BB_TRACE("[%d] point=%05d, chk_mask=0x%08x, chk_val=0x%x\n",
			 i, la->la_ptrn_chk_i[i].smp_point,
			 la->la_ptrn_chk_i[i].la_ptrn_chk_mask,
			 la->la_ptrn_chk_i[i].la_ptrn_chk_val);
	}
	BB_TRACE("==========================>\n");

	for (i = 0; i < LA_CHK_PTRN_NUM; i++) {
		if (la->la_ptrn_chk_i[i].la_ptrn_chk_mask == 0)
			continue;

		point_tmp = la->la_ptrn_chk_i[i].smp_point;
		mask_tmp = la->la_ptrn_chk_i[i].la_ptrn_chk_mask;
		val_tmp = la->la_ptrn_chk_i[i].la_ptrn_chk_val;

		idx = point_tmp << 1;
		data_msb_tmp = buf->octet[idx + 1];
		BB_TRACE("[%d] [Point:%d] %08x | %08x\n",
			 i, point_tmp, buf->octet[idx + 1], buf->octet[idx]);

		if (mask_tmp != MASKDWORD)
			shift = halbb_cal_bit_shift(mask_tmp);

		if ((data_msb_tmp & mask_tmp) == (val_tmp << shift)) {
			ptrn_match_num++;
			BB_TRACE("pattern[%d] match\n", i);
		} else {
			ptrn_match &= 0;
			BB_TRACE("pattern[%d] NOT match\n", i);
		}
	}

	if (ptrn_match_num == 0)
		ptrn_match = 0;

	BB_TRACE("pattern_match=%d, ptrn_match_num=%d\n", ptrn_match, ptrn_match_num);

	return ptrn_match;
}

void halbb_la_rpt_buf_get(struct bb_info *bb, u16 finish_ofst, bool is_round_up)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;
	bool la_ptrn_match;
	u32 i = 0;
	u32 addr = 0, start_addr = 0, finish_addr = 0; /* @(unit: Byte)*/
	u32 round_up_size = 0;
	u32 finish_ofst_byte = 0;
	u8 *la_data;

	if (la->la_mode_state != LA_STATE_GET_DLE_BUF) {
		BB_WARNING("[%s]\n", __func__);
		return;
	}

	BB_TRACE("[%s]\n", __func__);

	/*@==== [Get LA Report] ==============================================*/
	finish_ofst_byte = ((u32)finish_ofst) << 3;
	finish_addr = buf->start_pos + finish_ofst_byte;

	BB_TRACE("start_addr = ((0x%x)), end_addr = ((0x%x)), buf_size = ((0x%x))\n",
		 buf->start_pos, buf->end_pos, buf->buffer_size);

	if (is_round_up) {
		start_addr = finish_addr;
		round_up_size = buf->end_pos - start_addr;

		BB_TRACE("[Round_up:1] round_up_point=(%d)\n", finish_ofst); /*@Byte to 8Byte*/
		BB_TRACE("buf_start(0x%x)|----2---->|finish_addr(0x%x)|----1---->|buf_end(0x%x)\n",
			 buf->start_pos, finish_addr, buf->end_pos);

		la->smp_number = buf->smp_number_max;
	} else {
		start_addr = buf->start_pos;

		BB_TRACE("[Round_up:0]\n");
		BB_TRACE("buf_start(0x%x)|------->|finish_addr(0x%x)             |buf_end(0x%x)\n",
			 buf->start_pos, finish_addr, buf->end_pos);

		la->smp_number = DIFF_2(start_addr, finish_addr) >> 3;
	}
	BB_TRACE("smp_num=(%d)\n", la->smp_number);

	/*@==== [Get LA Patterns in TXFF] ====================================*/
	BB_TRACE("Dump_Start\n");

	if (!buf->octet)
		return;

	la_data = (u8 *)buf->octet;

	if (is_round_up) {
		/*BB_TRACE("0x%x + 0x%x = 0x%x, 0x%x\n", round_up_size, finish_ofst_byte + 8, (round_up_size + finish_ofst_byte + 8), buf->buffer_size);*/
		rtw_hal_mac_get_buffer_data(bb->hal_com, start_addr, la_data, round_up_size, 1);
		la_data += round_up_size;
	}
	rtw_hal_mac_get_buffer_data(bb->hal_com, buf->start_pos, la_data, finish_ofst_byte, 1);

	la_ptrn_match = halbb_la_ptrn_chk(bb);

	BB_TRACE("[Dump_End], la_ptrn_match=%d\n", la_ptrn_match);
}
#endif

#if (SET_MAC_CFG)

void halbb_la_set_mac_trig_time(struct bb_info *bb, u32 trig_time, u8 *unit, u8 *unit_num)
{
	u32 ref_time = 128;
	u8 time_unit_num = 0;
	u8 i;

	/*mac_la_tgr_tu_sel:   0 ~ 2^[0~15] */
	/*mac_la_tgr_time_val: 0 ~ 2^7      */

	if (trig_time > 0x400000)
		trig_time = 0x400000;

	for (i = 0; i < 16; i++) {
		if (trig_time <= (ref_time << i)) {
			*unit = i;
			break;
		}
	}

	*unit_num = (u8)(trig_time >> *unit);

	BB_DBG(bb, DBG_DBG_API, "2. [Set Trig-Time] Time=%d * unit=2^%d us\n",
	       *unit_num, *unit);
}

bool halbb_la_mac_cfg_buf(struct bb_info *bb, enum la_buff_mode_t mode)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_mac_cfg_info	*cfg = &la->la_mac_cfg_i;
	struct la_string_info *buf = &la->la_string_i;
	u32 addr_start = 0;
	u32 addr_end = 0;
	u32 buf_size_tmp = 0;

	switch (bb->ic_type) {
	case BB_RTL8852AA:
	case BB_RTL8852A:
		if (mode == LA_BUFF_256K)
			buf->buffer_size = 0x40000; /*2^18=(2^8)*(2^10)=256K Byte*/
		else if(mode == LA_BUFF_192K)
			buf->buffer_size = 0x30000;
		else
			buf->buffer_size = 0;
		break;
	case BB_RTL8852B:
		if (mode == LA_BUFF_128K)
			buf->buffer_size = 0x20000; /*2^17=(2^7)*(2^10)=128K Byte*/
		else
			buf->buffer_size = 0;
		break;
	default:
		BB_WARNING("[%s] IC\n", __func__);
		buf->buffer_size = 0;
		break;
	}

	if (buf->buffer_size == 0) {
		BB_WARNING("[%s] Buf=0\n", __func__);
		return false;
	}

	rtw_hal_mac_lamode_cfg_buf(bb->hal_com, cfg->mac_la_buf_sel, &addr_start, &addr_end);

	buf->start_pos = addr_start;
	buf->end_pos = addr_end;
	buf_size_tmp = buf->end_pos - buf->start_pos;

	if (buf_size_tmp != buf->buffer_size) {
		BB_WARNING("buf_size_tmp=0x%x, buffer_size=0x%x\n",
			   buf_size_tmp, buf->buffer_size);
		return false;
	}
	buf->smp_number_max = buf->buffer_size >> 3;

	BB_TRACE("addr=[0x%x ~ 0x%x], buf_size=(%d K), smp_num_max=(%d)\n",
		 buf->start_pos, buf->end_pos, (buf->buffer_size >> 10),
		 buf->smp_number_max);
	return true;
}

bool halbb_la_mac_cfg_buf_default(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	enum la_buff_mode_t mode;

	switch (bb->ic_type) {
	case BB_RTL8852A:
		mode = LA_BUFF_256K;
		break;
	case BB_RTL8852B:
		if (bb->hal_com->cv == CAV)
			mode = LA_BUFF_128K;
		else
			mode = LA_BUFF_64K;
		break;
	case BB_RTL8852C:
		mode = LA_BUFF_256K; /*TBD*/
		break;
	default:
		return false;
	}
	BB_TRACE("Auto Init MAC BUF CR, mode=(%d)K\n", 64 * (mode + 1));

	return halbb_la_mac_cfg_buf(bb, mode);
}

void halbb_la_mac_cfg_cmn(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_mac_cfg_info	*cfg = &la->la_mac_cfg_i;
	u8 t_unit = 0, t_unit_num = 0;

	cfg->mac_la_en = (cfg->mac_la_buf_sel == LA_BUF_DISABLE) ? 0 : 1;

	halbb_la_set_mac_trig_time(bb, cfg->la_trigger_time, &t_unit, &t_unit_num);

	rtw_hal_mac_lamode_cfg(bb->hal_com, cfg->mac_la_en, cfg->mac_la_restart_en,
			cfg->mac_la_timeout_en, cfg->mac_la_timeout_val,
			cfg->mac_la_data_loss_imr, t_unit, t_unit_num);

}

void halbb_la_mac_init(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_mac_cfg_info	*cfg = &la->la_mac_cfg_i;

	/*[MAC Buf]*/
	cfg->mac_la_buf_sel = LA_BUFF_256K;
	//halbb_la_mac_cfg_buf(bb, cfg->mac_la_buf_sel);

	/*[MAC Common]*/
	halbb_la_mac_cfg_cmn(bb);
}

#endif

#if SET_MAC_TRIG
void halbb_la_mac_set_adv_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_trig_mac_info *trig_mac = &la->la_trig_mac_i;

	halbb_mem_set(bb, trig_mac, 0, sizeof(struct la_trig_mac_info));
}

void halbb_la_mac_set_trig(struct bb_info *bb, bool mac_trig_en)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_trig_mac_info	*trig_mac = &la->la_trig_mac_i;
	struct bb_la_cr_info *cr = &la->bb_la_cr_i;

	BB_TRACE(" *[%s]\n", __func__);

	if (!mac_trig_en) {
		/*MAC_AND0*/
		halbb_set_reg(bb, cr->la_mac_and0_en, cr->la_mac_and0_en_m, 0);
		/*MAC_AND1*/
		halbb_set_reg(bb, cr->la_mac_and1_en, cr->la_mac_and1_en_m, 0);
		/*MAC_AND2*/
		halbb_set_reg(bb, cr->la_mac_and2_en, cr->la_mac_and2_en_m, 0);
		return;
	}

	halbb_set_reg(bb, cr->la_mac_and0_en, cr->la_mac_and0_en_m,
		      trig_mac->la_mac_and0_en);
	halbb_set_reg(bb, cr->la_mac_and0_sel, cr->la_mac_and0_sel_m,
		      trig_mac->la_mac_and0_sel);
	halbb_set_reg(bb, cr->la_mac_and0_mac_sel, BIT(16), 0);
	if (trig_mac->la_mac_and0_sel == 1)
		halbb_set_reg(bb, cr->la_mac_and0_mac_sel, BIT(22),
			      trig_mac->la_mac_and0_mac_sel);
	else if (trig_mac->la_mac_and0_sel == 2)
		halbb_set_reg(bb, cr->la_mac_and0_mac_sel, BIT(23),
			      trig_mac->la_mac_and0_mac_sel);
	halbb_set_reg(bb, cr->la_mac_and1_en, cr->la_mac_and1_en_m,
		      trig_mac->la_mac_and1_en);
	halbb_set_reg(bb, cr->la_mac_and2_en, cr->la_mac_and2_en_m,
		      trig_mac->la_mac_and2_en);
	halbb_set_reg(bb, 0xd04, 0xf, trig_mac->la_mac_and2_frame_sel); /*HW error*/
}
#endif

#if (SET_BB_DMA_FMT)

void halbb_la_bb_set_dma_type(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct bb_la_cr_info *cr = &la->bb_la_cr_i;
	u32 la_en = 1;
	u32 r_dma_rdrdy = 0;

	if (la->la_smp_rate == LA_SMP_CLK_160) {
		dma->dma_a_ck160_dly_en = 1;
		dma->dma_b_ck160_dly_en = 1;
		dma->dma_c_ck160_dly_en = 1;
		dma->dma_d_ck160_dly_en = 1;
	} else {
		dma->dma_a_ck160_dly_en = 0;
		dma->dma_b_ck160_dly_en = 0;
		dma->dma_c_ck160_dly_en = 0;
		dma->dma_d_ck160_dly_en = 0;
	}

	halbb_set_reg(bb, cr->la_en, cr->la_en_m, la_en);
	halbb_set_reg(bb, cr->dma_dbgport_base_n, cr->dma_dbgport_base_n_m,
		      dma->dma_dbgport_base_n);
	halbb_set_reg(bb, cr->dma_a_path_sel, cr->dma_a_path_sel_m,
		      dma->dma_a_path_sel);
	halbb_set_reg(bb, cr->dma_b_path_sel, cr->dma_b_path_sel_m,
		      dma->dma_b_path_sel);
	halbb_set_reg(bb, cr->dma_c_path_sel, cr->dma_c_path_sel_m,
		      dma->dma_c_path_sel);
	halbb_set_reg(bb, cr->dma_d_path_sel, cr->dma_d_path_sel_m,
		      dma->dma_d_path_sel);
	halbb_set_reg(bb, cr->dma_a_src_sel, cr->dma_a_src_sel_m,
		      dma->dma_a_src_sel);
	halbb_set_reg(bb, cr->dma_b_src_sel, cr->dma_b_src_sel_m,
		      dma->dma_b_src_sel);
	halbb_set_reg(bb, cr->dma_c_src_sel, cr->dma_c_src_sel_m,
		      dma->dma_c_src_sel);
	halbb_set_reg(bb, cr->dma_d_src_sel, cr->dma_d_src_sel_m,
		      dma->dma_d_src_sel);
	halbb_set_reg(bb, cr->dma_hdr_sel_63, cr->dma_hdr_sel_63_m,
		      dma->dma_hdr_sel_63);
	halbb_set_reg(bb, cr->dma_hdr_sel_62, cr->dma_hdr_sel_62_m,
		      dma->dma_hdr_sel_62);
	halbb_set_reg(bb, cr->dma_hdr_sel_61, cr->dma_hdr_sel_61_m,
		      dma->dma_hdr_sel_61);
	halbb_set_reg(bb, cr->dma_hdr_sel_60, cr->dma_hdr_sel_60_m,
		      dma->dma_hdr_sel_60);
	halbb_set_reg(bb, cr->dma_a_ck160_dly_en, cr->dma_a_ck160_dly_en_m,
		      dma->dma_a_ck160_dly_en);
	halbb_set_reg(bb, cr->dma_b_ck160_dly_en, cr->dma_b_ck160_dly_en_m,
		      dma->dma_b_ck160_dly_en);
	halbb_set_reg(bb, cr->dma_dbgport_phy_sel, cr->dma_dbgport_phy_sel_m,
		      dma->dma_dbgport_phy_sel);
	halbb_set_reg(bb, cr->dma_data_type, cr->dma_data_type_m,
		      dma->dma_data_type);
	halbb_set_reg(bb, cr->r_dma_rdrdy, cr->r_dma_rdrdy_m, r_dma_rdrdy);
}

void halbb_la_bb_set_dma_type_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	
	dma->dma_dbgport_base_n = 0;
	dma->dma_a_path_sel = 0;
	dma->dma_b_path_sel = 1;
	dma->dma_c_path_sel = 2;
	dma->dma_d_path_sel = 3;
	dma->dma_a_src_sel = 3;
	dma->dma_b_src_sel = 3;
	dma->dma_c_src_sel = 3;
	dma->dma_d_src_sel = 3;
	dma->dma_dbgport_phy_sel = 0;
	dma->dma_hdr_sel_63 = 1;
	dma->dma_hdr_sel_62 = 4;
	dma->dma_hdr_sel_61 = 8;
	dma->dma_hdr_sel_60 = 13;
	dma->dma_data_type = 1;
}

#endif

#if SET_BB_TRIG_RULE
void halbb_la_bb_set_adv_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;

	halbb_mem_set(bb, adv, 0, sizeof(struct la_adv_trig_info));
}

void halbb_la_bb_set_cmn_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;

	la->la_trigger_cnt = 0;
	la->la_dbg_port = 0x10205;
	la->la_trigger_edge = 0;
	la->la_smp_rate = LA_SMP_CLK_80;
	la->la_mac_cfg_i.la_trigger_time = 404;
	la->la_polling_cnt = 20;
	la->la_and0_disable = true;
}

void halbb_la_bb_set_re_trig_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_re_trig_info *re_trig = &la->la_re_trig_i;

	halbb_mem_set(bb, re_trig, 0, sizeof(struct la_re_trig_info));

	if (bb->ic_type == BB_RTL8852AA)
		re_trig->la_re_and0_sel = 2;

}

void halbb_la_bb_set_re_trig(struct bb_info *bb, bool re_trig_en)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_re_trig_info *re_trig = &la->la_re_trig_i;
	struct rtw_hal_com_t *hal_i = bb->hal_com;
	struct bb_la_cr_info *cr = &la->bb_la_cr_i;

	/*Since HW bug, 52A CAV use re-trigger to set LA mode stop condition*/
	if (bb->ic_type != BB_RTL8852AA) {
		if (!re_trig_en) {
			halbb_set_reg(bb, cr->la_re_and1_sel,
				      cr->la_re_and1_sel_m, 0);/*1b'1*/
			halbb_set_reg(bb, cr->la_re_and1_inv,
				      cr->la_re_and1_inv_m, 1);
			return;
		}
	}

	halbb_set_reg(bb, cr->la_re_trig_edge, cr->la_re_trig_edge_m,
		      re_trig->la_re_trig_edge);
	halbb_set_reg(bb, cr->la_re_and1_sel, cr->la_re_and1_sel_m,
		      re_trig->la_re_and0_sel);
	if (re_trig->la_re_and0_sel == 0xf)
		halbb_set_reg(bb, cr->la_brk_sel, cr->la_brk_sel_m,
		      re_trig->la_re_and0_val);
	else
		halbb_set_reg(bb, cr->la_re_and1_val, cr->la_re_and1_val_m,
			      re_trig->la_re_and0_val);
	halbb_set_reg(bb, cr->la_re_and1_inv, cr->la_re_and1_inv_m,
		      re_trig->la_re_and0_inv);
}
void halbb_la_bb_set_trig(struct bb_info *bb, bool and0_trig_disable, bool adv_trig_en, bool not_stop_trig_en)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;
	struct bb_la_cr_info *cr = &la->bb_la_cr_i;
	u32	trig_sel = 0;

	BB_TRACE(" {and0_trig_disable, adv_trig, not_stop_trig} = {%d, %d, %d}\n",
		 and0_trig_disable, adv_trig_en, not_stop_trig_en);

	/*===== [And-0 Trigger] ==============================================*/
	if (and0_trig_disable) {
		halbb_set_reg(bb, cr->and0_trig_disable,
			      cr->and0_trig_disable_m, 1);/*disable=1*/
	} else {
		if (not_stop_trig_en)
			trig_sel = 0; /*set to unchanged BB debug port bit*/
		else
			trig_sel = la->la_and0_bit_sel;

		halbb_set_reg(bb, cr->and0_trig_disable,
			      cr->and0_trig_disable_m, 0); /*disable=0*/
		halbb_set_reg(bb, cr->la_and0_bit_sel, cr->la_and0_bit_sel_m,
			      trig_sel); /*debug port bit*/
	}

	BB_TRACE(" *Set dbg_port[BIT] = %d\n", trig_sel);
	
	/*===== [And-1~7 Trigger] ============================================*/
	if (!adv_trig_en) { /*normal LA mode & back to default*/
		/*AND1*/
		halbb_set_reg(bb, cr->la_and1_mask, cr->la_and1_mask_m, 0);
		/*AND2*/
		halbb_set_reg(bb, cr->la_and2_en, cr->la_and2_en_m, 0);
		/*AND3*/
		halbb_set_reg(bb, cr->la_and3_en, cr->la_and3_en_m, 0);
		/*AND4*/
		halbb_set_reg(bb, cr->la_and4_en, cr->la_and4_en_m, 0);
		/*AND5*/
		halbb_set_reg(bb, cr->la_and5_sel, cr->la_and5_sel_m, 0);
		/*AND6*/
		halbb_set_reg(bb, cr->la_and6_sel, cr->la_and6_sel_m, 0);
		/*AND7*/
		halbb_set_reg(bb, cr->la_and7_sel, cr->la_and7_sel_m, 0);
		return;
	}
	/*AND1*/
	halbb_set_reg(bb, cr->la_and1_mask, cr->la_and1_mask_m,
		      adv->la_and1_mask);
	halbb_set_reg(bb, cr->la_and1_inv, cr->la_and1_inv_m, adv->la_and1_inv);
	halbb_set_reg(bb, cr->la_and1_val, cr->la_and1_val_m, adv->la_and1_val);
	/*AND2*/
	halbb_set_reg(bb, cr->la_and2_en, cr->la_and2_en_m, adv->la_and2_en);
	halbb_set_reg(bb, cr->la_and2_inv, cr->la_and2_inv_m, adv->la_and2_inv);
	halbb_set_reg(bb, cr->la_and2_val, cr->la_and2_val_m, adv->la_and2_val);
	halbb_set_reg(bb, cr->la_and2_mask, cr->la_and2_mask_m,
		      adv->la_and2_mask);
	halbb_set_reg(bb, cr->la_and2_sign, cr->la_and2_sign_m,
		      adv->la_and2_sign);
	/*AND3*/
	halbb_set_reg(bb, cr->la_and3_en, cr->la_and3_en_m, adv->la_and3_en);
	halbb_set_reg(bb, cr->la_and3_inv, cr->la_and3_inv_m, adv->la_and3_inv);
	halbb_set_reg(bb, cr->la_and3_val, cr->la_and3_val_m, adv->la_and3_val);
	halbb_set_reg(bb, cr->la_and3_mask, cr->la_and3_mask_m,
		      adv->la_and3_mask);
	halbb_set_reg(bb, cr->la_and3_sign, cr->la_and3_sign_m,
		      adv->la_and3_sign);
	/*AND4*/
	halbb_set_reg(bb, cr->la_and4_en, cr->la_and4_en_m, adv->la_and4_en);
	halbb_set_reg(bb, cr->la_and4_inv, cr->la_and4_inv_m, adv->la_and4_inv);
	halbb_set_reg(bb, cr->la_and4_rate, cr->la_and4_rate_m,
		      adv->la_and4_rate);
	/*AND5*/
	halbb_set_reg(bb, cr->la_and5_sel, cr->la_and5_sel_m, adv->la_and5_sel);
	halbb_set_reg(bb, cr->la_and5_inv, cr->la_and5_inv_m, adv->la_and5_inv);
	if (adv->la_and5_sel == 0xf)
		halbb_set_reg(bb, cr->la_brk_sel, cr->la_brk_sel_m,
			      adv->la_and5_val);
	else
		halbb_set_reg(bb, cr->la_and5_val, cr->la_and5_val_m,
			      adv->la_and5_val);

	/*AND6*/
	halbb_set_reg(bb, cr->la_and6_sel, cr->la_and6_sel_m, adv->la_and6_sel);
	halbb_set_reg(bb, cr->la_and6_inv, cr->la_and6_inv_m, adv->la_and6_inv);
	if (adv->la_and6_sel == 0xf)
		halbb_set_reg(bb, cr->la_brk_sel, cr->la_brk_sel_m,
			      adv->la_and6_val);
	else
		halbb_set_reg(bb, cr->la_and6_val, cr->la_and6_val_m,
			      adv->la_and6_val);

	/*AND7*/
	halbb_set_reg(bb, cr->la_and7_sel, cr->la_and7_sel_m, adv->la_and7_sel);
	halbb_set_reg(bb, cr->la_and7_inv, cr->la_and7_inv_m, adv->la_and7_inv);
	if (adv->la_and7_sel == 0xf)
		halbb_set_reg(bb, cr->la_brk_sel, cr->la_brk_sel_m,
			      adv->la_and7_val);
	else
		halbb_set_reg(bb, cr->la_and7_val, cr->la_and7_val_m,
			      adv->la_and7_val);
}

void halbb_la_bb_set_dbg_port(struct bb_info *bb, bool not_stop_trig_en)
{
	
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	u32	trig_sel = la->la_and0_bit_sel;
	u32	dbg_port = la->la_dbg_port;

	/*===== [And-0 Trigger] ====================*/
	/*set to unchanged BB debug port*/
	if (not_stop_trig_en) {
		dbg_port = 0xf;
		trig_sel = 0;
		BB_TRACE("[BB Setting] not stop trigger!\n");
	}

	if (halbb_bb_dbg_port_racing(bb, DBGPORT_PRI_3)) {
		halbb_set_bb_dbg_port_ip(bb, (dbg_port & 0xff0000) >> 16);
		halbb_set_bb_dbg_port(bb, dbg_port & 0xffff);
		BB_TRACE(" *Set dbg_port=(0x%x)\n", dbg_port);
	} else {
		dbg_port = halbb_get_bb_dbg_port_idx(bb);
		BB_TRACE("[Set dbg_port fail!] Curr-DbgPort=0x%x\n", dbg_port);
	}
}

void halbb_la_bb_set_general(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct bb_la_cr_info *cr = &la->bb_la_cr_i;
	u32 rdrdy_3_phase_en = 0;
	u32 la_top_trig = 1;

	BB_TRACE("3. [BB Setting] Edge=(%s), smp_rate=(%dM), Dma_type=(%d)\n",
		 (la->la_trigger_edge == 0) ? "P" : "N",
		 80 >> la->la_smp_rate, dma->dma_data_type);

	rdrdy_3_phase_en = (dma->dma_data_type == DMA13_MPHS_1s_3p_10b) ? 1 : 0;

	halbb_set_reg(bb, cr->la_trigger_edge, cr->la_trigger_edge_m,
		      la->la_trigger_edge);
	halbb_set_reg(bb, cr->rdrdy_3_phase_en, cr->rdrdy_3_phase_en_m,
		      rdrdy_3_phase_en);
	halbb_set_reg(bb, cr->la_smp_rate, cr->la_smp_rate_m, la->la_smp_rate);
	halbb_set_reg(bb, cr->la_trigger_cnt, cr->la_trigger_cnt_m,
		      la->la_trigger_cnt);
	halbb_set_reg(bb, cr->la_clk_en, cr->la_clk_en_m, la_top_trig);
}

#endif
#if (LAMODE_MAIN)
void
halbb_la_drv_buf_release(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;

	if (buf->length != 0 && buf->octet) {
		halbb_mem_free(bb, buf->octet, buf->buffer_size);
		buf->length = 0;
	}
}

bool
halbb_la_drv_buf_allocate(struct bb_info *bb)
{
	
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;
	bool ret = true;

	BB_TRACE("[LA mode BufferAllocate]\n");

	if (buf->length == 0) {
		buf->octet = (u32 *)halbb_mem_alloc(bb, buf->buffer_size);

		if (!buf->octet)
			ret = false;

		if (ret)
			buf->length = buf->buffer_size;
	}

	return ret;
}

void halbb_la_stop(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;

	la->la_mode_state = LA_STATE_IDLE;
}

void halbb_la_main(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct la_mac_cfg_info *cfg = &la->la_mac_cfg_i;
	u8 mac_rpt_state = LA_HW_IDLE;
	u32 mac_trig_fail;
	u8 tmp_u1b = 0;
	u8 i = 0;
	u16 finish_ofst = 0;
	bool round_up = 0;
	bool loss_data;

	if (la->la_mode_state != LA_STATE_MAIN) {
		halbb_la_stop(bb);
		BB_WARNING("[%s]\n", __func__);
	}

	BB_TRACE("1. [BB Setting] dbg_port = ((0x%x)), Trig_Edge = ((%d)), smp_rate = ((%d)), Trig_Bit_Sel = ((0x%x)), Dma_type = ((%d))\n",
		 la->la_dbg_port, la->la_trigger_edge,
		 la->la_smp_rate, la->la_and0_bit_sel, dma->dma_data_type);

	halbb_la_bb_set_general(bb);
	halbb_la_bb_set_dma_type(bb);
	halbb_la_bb_set_dbg_port(bb, la->not_stop_trig);
	halbb_la_bb_set_trig(bb, la->la_and0_disable,
			     la->adv_trig_i.adv_trig_en, la->not_stop_trig);
	halbb_la_bb_set_re_trig(bb, la->la_re_trig_i.re_trig_en);
	halbb_la_mac_set_trig(bb, la->la_trig_mac_i.la_mac_trig_en);

	halbb_la_mac_cfg_cmn(bb);

	if (la->not_stop_trig) {
		halbb_delay_ms(bb, 100);
		halbb_la_bb_set_dbg_port(bb, false);
	}

	mac_trig_fail = rtw_hal_mac_lamode_trig(bb->hal_com, 1);

	if (!mac_trig_fail) {
		do { /*Polling time always use 100ms, when it exceed 2s, break loop*/
			rtw_hal_mac_get_lamode_st(bb->hal_com, (u8 *)&mac_rpt_state,
						  &finish_ofst, &round_up,
						  &loss_data);

			BB_TRACE("[%d] rpt=((0x%x)), finish_ofst =((%d)), round_up =((%d)) \n", i, mac_rpt_state, finish_ofst, round_up);

			if (mac_rpt_state == LA_HW_RE_START) {
				BB_TRACE("[Restart]\n");
				break;
			} else if (mac_rpt_state == LA_HW_FINISH_STOP) {
				BB_TRACE("[LA Query OK]\n");
				break;
			} else if (mac_rpt_state == LA_HW_START) {
				halbb_delay_ms(bb, 100);
				i++;
				continue;
			} else if (mac_rpt_state == LA_HW_FINISH_TIMEOUT) {
				BB_TRACE("[LA HW timeout]\n");
				break;
			} else { /*LA_HW_IDLE)*/
				break;
			}
		} while (i < la->la_polling_cnt);

		if (mac_rpt_state == LA_HW_FINISH_STOP) {
			la->la_mode_state = LA_STATE_GET_DLE_BUF;
			halbb_la_rpt_buf_get(bb, finish_ofst, round_up);
		} else if (mac_rpt_state == LA_HW_RE_START) {
			la->la_mode_state = LA_STATE_WAIT_RESTART;
			la->la_count = 0;
			return;
		} else {
			BB_TRACE("[Polling Fail]\n");
		}
	}

	halbb_la_stop(bb);
	
	BB_TRACE("[LA mode] la_count = ((%d))\n", la->la_count);
	if (la->la_count == 0) {
		BB_TRACE("LA Dump finished ---------->\n\n\n");
		halbb_release_bb_dbg_port(bb);
	} else {
		la->la_count--;
		BB_TRACE("LA Dump more ---------->\n\n\n");
		halbb_la_run(bb);
	}
}

void halbb_la_re_trig_watchdog(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;

	if (la->la_mode_state != LA_STATE_WAIT_RESTART) {
		return;
	}
	la->la_re_trig_i.re_trig_wait_cnt++;
	BB_TRACE("re_trig_wait_cnt=(%d)\n", la->la_re_trig_i.re_trig_wait_cnt);

	la->la_mode_state = LA_STATE_MAIN;
	halbb_la_main(bb);
}

void halbb_la_run(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;
	bool is_set_success = true;

	BB_TRACE("[%s] LA_State=(%d)\n", __func__, la->la_mode_state);

	if (!la->la_mac_cfg_i.mac_alloc_success) {
		la->la_mac_cfg_i.mac_alloc_success = halbb_la_mac_cfg_buf_default(bb);
		if (!la->la_mac_cfg_i.mac_alloc_success) {
			BB_WARNING("MAC BUF CR set fail)\n");
			return;
		}
	}

	if (la->la_mode_state != LA_STATE_IDLE) {
		halbb_la_stop(bb);
		return;
	}
	
	if (buf->length == 0)
		is_set_success = halbb_la_drv_buf_allocate(bb);

	if (!is_set_success) {
		BB_WARNING("LA_BUf_alloc fail)\n");
		return;
	}

	halbb_mem_set(bb, buf->octet, 0, buf->buffer_size);

	la->la_re_trig_i.re_trig_wait_cnt = 0;
	la->la_mode_state = LA_STATE_MAIN;
	halbb_la_main(bb);
}

void halbb_la_deinit(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;

	halbb_la_stop(bb);
	halbb_la_drv_buf_release(bb);
}

void halbb_la_reset(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *la_string = &la->la_string_i;

	la->la_mode_state = LA_STATE_IDLE;
	la->la_print_i.is_la_print = false;
	la->not_stop_trig = false;

	halbb_la_bb_set_cmn_reset(bb);
	halbb_la_bb_set_dma_type_reset(bb);
	halbb_la_bb_set_adv_reset(bb);
	halbb_la_mac_set_adv_reset(bb);
	halbb_la_bb_set_re_trig_reset(bb);

	halbb_la_mac_init(bb);
}

void halbb_la_init(struct bb_info *bb)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_string_info *buf = &la->la_string_i;

	la->la_mac_cfg_i.mac_alloc_success = false;
	buf->length = 0;
	halbb_la_reset(bb);
	halbb_mem_set(bb, la->la_ptrn_chk_i, 0, sizeof(struct la_ptrn_chk_info) * LA_CHK_PTRN_NUM);
}

void halbb_cr_cfg_la_init(struct bb_info *bb)
{
	struct bb_la_cr_info *cr = &bb->bb_cmn_hooker->bb_la_mode_i.bb_la_cr_i;

	switch (bb->cr_type) {

	#ifdef HALBB_52AA_SERIES
	case BB_52AA:
		cr->la_clk_en = LA_CKEN_52AA;
		cr->la_clk_en_m = LA_CKEN_52AA_M;
		cr->la_en = LA_EN_52AA;
		cr->la_en_m = LA_EN_52AA_M;
		cr->dma_dbgport_base_n = LA_DBGPORT_BASE_N_52AA;
		cr->dma_dbgport_base_n_m = LA_DBGPORT_BASE_N_52AA_M;
		cr->dma_a_path_sel = LA_TYPEA_PATH_SEL_52AA;
		cr->dma_a_path_sel_m = LA_TYPEA_PATH_SEL_52AA_M;
		cr->dma_b_path_sel = LA_TYPEB_PATH_SEL_52AA;
		cr->dma_b_path_sel_m = LA_TYPEB_PATH_SEL_52AA_M;
		cr->dma_c_path_sel = LA_TYPEC_PATH_SEL_52AA;
		cr->dma_c_path_sel_m = LA_TYPEC_PATH_SEL_52AA_M;
		cr->dma_d_path_sel = LA_TYPED_PATH_SEL_52AA;
		cr->dma_d_path_sel_m = LA_TYPED_PATH_SEL_52AA_M;
		cr->dma_a_src_sel = LA_TYPEA_SRC_SEL_52AA;
		cr->dma_a_src_sel_m = LA_TYPEA_SRC_SEL_52AA_M;
		cr->dma_b_src_sel = LA_TYPEB_SRC_SEL_52AA;
		cr->dma_b_src_sel_m = LA_TYPEB_SRC_SEL_52AA_M;
		cr->dma_c_src_sel = LA_TYPEC_SRC_SEL_52AA;
		cr->dma_c_src_sel_m = LA_TYPEC_SRC_SEL_52AA_M;
		cr->dma_d_src_sel = LA_TYPED_SRC_SEL_52AA;
		cr->dma_d_src_sel_m = LA_TYPED_SRC_SEL_52AA_M;
		cr->la_smp_rate = LA_SMP_RT_SEL_52AA;
		cr->la_smp_rate_m = LA_SMP_RT_SEL_52AA_M;
		cr->rdrdy_3_phase_en = LA_RDRDY_3PHASE_EN_52AA;
		cr->rdrdy_3_phase_en_m = LA_RDRDY_3PHASE_EN_52AA_M;
		cr->la_trigger_edge = LA_EDGE_SEL_52AA;
		cr->la_trigger_edge_m = LA_EDGE_SEL_52AA_M;
		cr->dma_hdr_sel_63 = LA_HDR_SEL_63_52AA;
		cr->dma_hdr_sel_63_m = LA_HDR_SEL_63_52AA_M;
		cr->dma_hdr_sel_62 = LA_HDR_SEL_62_52AA;
		cr->dma_hdr_sel_62_m = LA_HDR_SEL_62_52AA_M;
		cr->dma_hdr_sel_61 = LA_HDR_SEL_61_52AA;
		cr->dma_hdr_sel_61_m = LA_HDR_SEL_61_52AA_M;
		cr->dma_hdr_sel_60 = LA_HDR_SEL_60_52AA;
		cr->dma_hdr_sel_60_m = LA_HDR_SEL_60_52AA_M;
		cr->dma_a_ck160_dly_en = LA_TYPEA_CK160_DLY_EN_52AA;
		cr->dma_a_ck160_dly_en_m = LA_TYPEA_CK160_DLY_EN_52AA_M;
		cr->dma_b_ck160_dly_en = LA_TYPEB_CK160_DLY_EN_52AA;
		cr->dma_b_ck160_dly_en_m = LA_TYPEB_CK160_DLY_EN_52AA_M;
		cr->dma_dbgport_phy_sel = LA_DBGPORT_SRC_SEL_52AA;
		cr->dma_dbgport_phy_sel_m = LA_DBGPORT_SRC_SEL_52AA_M;
		cr->dma_data_type = LA_DATA_52AA;
		cr->dma_data_type_m = LA_DATA_52AA_M;
		cr->r_dma_rdrdy = LA_RDRDY_52AA;
		cr->r_dma_rdrdy_m = LA_RDRDY_52AA_M;
		cr->la_and0_bit_sel = LA_TRIG_52AA;
		cr->la_and0_bit_sel_m = LA_TRIG_52AA_M;
		cr->la_trigger_cnt = LA_TRIG_CNT_52AA;
		cr->la_trigger_cnt_m = LA_TRIG_CNT_52AA_M;
		cr->and0_trig_disable = LA_TRIG_NEW_ONLY_52AA;
		cr->and0_trig_disable_m = LA_TRIG_NEW_ONLY_52AA_M;
		cr->la_and1_inv = LA_TRIG_AND1_INV_52AA;
		cr->la_and1_inv_m = LA_TRIG_AND1_INV_52AA_M;
		cr->la_and2_en = LA_TRIG_AND2_EN_52AA;
		cr->la_and2_en_m = LA_TRIG_AND2_EN_52AA_M;
		cr->la_and2_inv = LA_TRIG_AND2_INV_52AA;
		cr->la_and2_inv_m = LA_TRIG_AND2_INV_52AA_M;
		cr->la_and3_en = LA_TRIG_AND3_EN_52AA;
		cr->la_and3_en_m = LA_TRIG_AND3_EN_52AA_M;
		cr->la_and3_inv = LA_TRIG_AND3_INV_52AA;
		cr->la_and3_inv_m = LA_TRIG_AND3_INV_52AA_M;
		cr->la_and4_en = LA_TRIG_AND4_EN_52AA;
		cr->la_and4_en_m = LA_TRIG_AND4_EN_52AA_M;
		cr->la_and4_rate = LA_TRIG_AND4_VAL_52AA;
		cr->la_and4_rate_m = LA_TRIG_AND4_VAL_52AA_M;
		cr->la_and4_inv = LA_TRIG_AND4_INV_52AA;
		cr->la_and4_inv_m = LA_TRIG_AND4_INV_52AA_M;
		cr->la_and1_mask = LA_TRIG_AND1_BIT_EN_52AA;
		cr->la_and1_mask_m = LA_TRIG_AND1_BIT_EN_52AA_M;
		cr->la_and1_val = LA_TRIG_AND1_VAL_52AA;
		cr->la_and1_val_m = LA_TRIG_AND1_VAL_52AA_M;
		cr->la_and2_mask = LA_TRIG_AND2_MASK_52AA;
		cr->la_and2_mask_m = LA_TRIG_AND2_MASK_52AA_M;
		cr->la_and2_val = LA_TRIG_AND2_VAL_52AA;
		cr->la_and2_val_m = LA_TRIG_AND2_VAL_52AA_M;
		cr->la_and3_mask = LA_TRIG_AND3_MASK_52AA;
		cr->la_and3_mask_m = LA_TRIG_AND3_MASK_52AA_M;
		cr->la_and3_val = LA_TRIG_AND3_VAL_52AA;
		cr->la_and3_val_m = LA_TRIG_AND3_VAL_52AA_M;
		cr->la_and5_sel = LA_TRIG_AND5_52AA;
		cr->la_and5_sel_m = LA_TRIG_AND5_52AA_M;
		cr->la_and5_val = LA_TRIG_AND5_VAL_52AA;
		cr->la_and5_val_m = LA_TRIG_AND5_VAL_52AA_M;
		cr->la_and5_inv = LA_TRIG_AND5_INV_52AA;
		cr->la_and5_inv_m = LA_TRIG_AND5_INV_52AA_M;
		cr->la_and6_sel = LA_TRIG_AND6_52AA;
		cr->la_and6_sel_m = LA_TRIG_AND6_52AA_M;
		cr->la_and6_val = LA_TRIG_AND6_VAL_52AA;
		cr->la_and6_val_m = LA_TRIG_AND6_VAL_52AA_M;
		cr->la_and6_inv = LA_TRIG_AND6_INV_52AA;
		cr->la_and6_inv_m = LA_TRIG_AND6_INV_52AA_M;
		cr->la_and7_sel = LA_TRIG_AND7_52AA;
		cr->la_and7_sel_m = LA_TRIG_AND7_52AA_M;
		cr->la_and7_val = LA_TRIG_AND7_VAL_52AA;
		cr->la_and7_val_m = LA_TRIG_AND7_VAL_52AA_M;
		cr->la_and7_inv = LA_TRIG_AND7_INV_52AA;
		cr->la_and7_inv_m = LA_TRIG_AND7_INV_52AA_M;
		cr->la_mac_and1_en = LA_M_AND1_EN_52AA;
		cr->la_mac_and1_en_m = LA_M_AND1_EN_52AA_M;
		cr->la_mac_and2_en = LA_M_AND2_EN_52AA;
		cr->la_mac_and2_en_m = LA_M_AND2_EN_52AA_M;
		cr->la_mac_and0_sel = LA_M_AND0_SEL_52AA;
		cr->la_mac_and0_sel_m = LA_M_AND0_SEL_52AA_M;
		cr->la_mac_and0_en = LA_M_AND0_EN_52AA;
		cr->la_mac_and0_en_m = LA_M_AND0_EN_52AA_M;
		cr->la_and2_sign = LA_SIGN2_52AA;
		cr->la_and2_sign_m = LA_SIGN2_52AA_M;
		cr->la_and3_sign = LA_SIGN3_52AA;
		cr->la_and3_sign_m = LA_SIGN3_52AA_M;
		cr->la_re_trig_edge = LA_RE_INIT_POLARITY_52AA;
		cr->la_re_trig_edge_m = LA_RE_INIT_POLARITY_52AA_M;
		cr->la_re_and1_sel = LA_RE_INIT_AND1_52AA;
		cr->la_re_and1_sel_m = LA_RE_INIT_AND1_52AA_M;
		cr->la_re_and1_val = LA_RE_INIT_AND1_VAL_52AA;
		cr->la_re_and1_val_m = LA_RE_INIT_AND1_VAL_52AA_M;
		cr->la_re_and1_inv = LA_RE_INIT_AND1_INV_52AA;
		cr->la_re_and1_inv_m = LA_RE_INIT_AND1_INV_52AA_M;
		break;

	#endif
	#ifdef HALBB_COMPILE_AP_SERIES
	case BB_AP:
		cr->la_clk_en = LA_CKEN_A;
		cr->la_clk_en_m = LA_CKEN_A_M;
		cr->la_en = LA_EN_A;
		cr->la_en_m = LA_EN_A_M;
		cr->dma_dbgport_base_n = LA_DBGPORT_BASE_N_A;
		cr->dma_dbgport_base_n_m = LA_DBGPORT_BASE_N_A_M;
		cr->dma_a_path_sel = LA_TYPEA_PATH_SEL_A;
		cr->dma_a_path_sel_m = LA_TYPEA_PATH_SEL_A_M;
		cr->dma_b_path_sel = LA_TYPEB_PATH_SEL_A;
		cr->dma_b_path_sel_m = LA_TYPEB_PATH_SEL_A_M;
		cr->dma_c_path_sel = LA_TYPEC_PATH_SEL_A;
		cr->dma_c_path_sel_m = LA_TYPEC_PATH_SEL_A_M;
		cr->dma_d_path_sel = LA_TYPED_PATH_SEL_A;
		cr->dma_d_path_sel_m = LA_TYPED_PATH_SEL_A_M;
		cr->dma_a_src_sel = LA_TYPEA_SRC_SEL_A;
		cr->dma_a_src_sel_m = LA_TYPEA_SRC_SEL_A_M;
		cr->dma_b_src_sel = LA_TYPEB_SRC_SEL_A;
		cr->dma_b_src_sel_m = LA_TYPEB_SRC_SEL_A_M;
		cr->dma_c_src_sel = LA_TYPEC_SRC_SEL_A;
		cr->dma_c_src_sel_m = LA_TYPEC_SRC_SEL_A_M;
		cr->dma_d_src_sel = LA_TYPED_SRC_SEL_A;
		cr->dma_d_src_sel_m = LA_TYPED_SRC_SEL_A_M;
		cr->la_smp_rate = LA_SMP_RT_SEL_A;
		cr->la_smp_rate_m = LA_SMP_RT_SEL_A_M;
		cr->rdrdy_3_phase_en = LA_RDRDY_3PHASE_EN_A;
		cr->rdrdy_3_phase_en_m = LA_RDRDY_3PHASE_EN_A_M;
		cr->la_trigger_edge = LA_EDGE_SEL_A;
		cr->la_trigger_edge_m = LA_EDGE_SEL_A_M;
		cr->dma_hdr_sel_63 = LA_HDR_SEL_63_A;
		cr->dma_hdr_sel_63_m = LA_HDR_SEL_63_A_M;
		cr->dma_hdr_sel_62 = LA_HDR_SEL_62_A;
		cr->dma_hdr_sel_62_m = LA_HDR_SEL_62_A_M;
		cr->dma_hdr_sel_61 = LA_HDR_SEL_61_A;
		cr->dma_hdr_sel_61_m = LA_HDR_SEL_61_A_M;
		cr->dma_hdr_sel_60 = LA_HDR_SEL_60_A;
		cr->dma_hdr_sel_60_m = LA_HDR_SEL_60_A_M;
		cr->dma_a_ck160_dly_en = LA_TYPEA_CK160_DLY_EN_A;
		cr->dma_a_ck160_dly_en_m = LA_TYPEA_CK160_DLY_EN_A_M;
		cr->dma_b_ck160_dly_en = LA_TYPEB_CK160_DLY_EN_A;
		cr->dma_b_ck160_dly_en_m = LA_TYPEB_CK160_DLY_EN_A_M;
		cr->dma_dbgport_phy_sel = LA_DBGPORT_SRC_SEL_A;
		cr->dma_dbgport_phy_sel_m = LA_DBGPORT_SRC_SEL_A_M;
		cr->dma_data_type = LA_DATA_A;
		cr->dma_data_type_m = LA_DATA_A_M;
		cr->r_dma_rdrdy = LA_RDRDY_A;
		cr->r_dma_rdrdy_m= LA_RDRDY_A_M;
		cr->la_and0_bit_sel = LA_TRIG_A;
		cr->la_and0_bit_sel_m = LA_TRIG_A_M;
		cr->la_trigger_cnt = LA_TRIG_CNT_A;
		cr->la_trigger_cnt_m = LA_TRIG_CNT_A_M;
		cr->and0_trig_disable = LA_TRIG_NEW_ONLY_A;
		cr->and0_trig_disable_m = LA_TRIG_NEW_ONLY_A_M;
		cr->la_and1_inv = LA_TRIG_AND1_INV_A;
		cr->la_and1_inv_m = LA_TRIG_AND1_INV_A_M;
		cr->la_and2_en = LA_TRIG_AND2_EN_A;
		cr->la_and2_en_m = LA_TRIG_AND2_EN_A_M;
		cr->la_and2_inv = LA_TRIG_AND2_INV_A;
		cr->la_and2_inv_m = LA_TRIG_AND2_INV_A_M;
		cr->la_and3_en = LA_TRIG_AND3_EN_A;
		cr->la_and3_en_m = LA_TRIG_AND3_EN_A_M;
		cr->la_and3_inv = LA_TRIG_AND3_INV_A;
		cr->la_and3_inv_m = LA_TRIG_AND3_INV_A_M;
		cr->la_and4_en = LA_TRIG_AND4_EN_A;
		cr->la_and4_en_m = LA_TRIG_AND4_EN_A_M;
		cr->la_and4_rate = LA_TRIG_AND4_VAL_A;
		cr->la_and4_rate_m = LA_TRIG_AND4_VAL_A_M;
		cr->la_and4_inv = LA_TRIG_AND4_INV_A;
		cr->la_and4_inv_m = LA_TRIG_AND4_INV_A_M;
		cr->la_and1_mask = LA_TRIG_AND1_BIT_EN_A;
		cr->la_and1_mask_m = LA_TRIG_AND1_BIT_EN_A_M;
		cr->la_and1_val = LA_TRIG_AND1_VAL_A;
		cr->la_and1_val_m = LA_TRIG_AND1_VAL_A_M;
		cr->la_and2_mask = LA_TRIG_AND2_MASK_A;
		cr->la_and2_mask_m = LA_TRIG_AND2_MASK_A_M;
		cr->la_and2_val = LA_TRIG_AND2_VAL_A;
		cr->la_and2_val_m = LA_TRIG_AND2_VAL_A_M;
		cr->la_and3_mask = LA_TRIG_AND3_MASK_A;
		cr->la_and3_mask_m = LA_TRIG_AND3_MASK_A_M;
		cr->la_and3_val = LA_TRIG_AND3_VAL_A;
		cr->la_and3_val_m = LA_TRIG_AND3_VAL_A_M;
		cr->la_and5_sel = LA_TRIG_AND5_A;
		cr->la_and5_sel_m = LA_TRIG_AND5_A_M;
		cr->la_and5_val = LA_TRIG_AND5_VAL_A;
		cr->la_and5_val_m = LA_TRIG_AND5_VAL_A_M;
		cr->la_and5_inv = LA_TRIG_AND5_INV_A;
		cr->la_and5_inv_m = LA_TRIG_AND5_INV_A_M;
		cr->la_and6_sel = LA_TRIG_AND6_A;
		cr->la_and6_sel_m = LA_TRIG_AND6_A_M;
		cr->la_and6_val = LA_TRIG_AND6_VAL_A;
		cr->la_and6_val_m = LA_TRIG_AND6_VAL_A_M;
		cr->la_and6_inv = LA_TRIG_AND6_INV_A;
		cr->la_and6_inv_m = LA_TRIG_AND6_INV_A_M;
		cr->la_and7_sel = LA_TRIG_AND7_A;
		cr->la_and7_sel_m = LA_TRIG_AND7_A_M;
		cr->la_and7_val = LA_TRIG_AND7_VAL_A;
		cr->la_and7_val_m = LA_TRIG_AND7_VAL_A_M;
		cr->la_and7_inv = LA_TRIG_AND7_INV_A;
		cr->la_and7_inv_m = LA_TRIG_AND7_INV_A_M;
		cr->la_brk_sel = BRK_R_BRK_SEL_FOR_CNT_A;
		cr->la_brk_sel_m =BRK_R_BRK_SEL_FOR_CNT_A_M;
		cr->la_mac_and1_en = LA_M_AND1_EN_A;
		cr->la_mac_and1_en_m = LA_M_AND1_EN_A_M;
		cr->la_mac_and2_en = LA_M_AND2_EN_A;
		cr->la_mac_and2_en_m = LA_M_AND2_EN_A_M;
		cr->la_mac_and0_sel = LA_M_AND0_SEL_A;
		cr->la_mac_and0_sel_m = LA_M_AND0_SEL_A_M;
		cr->la_mac_and0_en = LA_M_AND0_EN_A;
		cr->la_mac_and0_en_m = LA_M_AND0_EN_A_M;
		cr->la_mac_and0_mac_sel = INTF_R_MAC_SEL_A;
		cr->la_mac_and0_mac_sel_m = INTF_R_MAC_SEL_A_M;
		cr->la_and2_sign = LA_SIGN2_A;
		cr->la_and2_sign_m = LA_SIGN2_A_M;
		cr->la_and3_sign = LA_SIGN3_A;
		cr->la_and3_sign_m = LA_SIGN3_A_M;
		cr->la_re_trig_edge = LA_RE_INIT_POLARITY_A;
		cr->la_re_trig_edge_m = LA_RE_INIT_POLARITY_A_M;
		cr->la_re_and1_sel = LA_RE_INIT_AND1_A;
		cr->la_re_and1_sel_m = LA_RE_INIT_AND1_A_M;
		cr->la_re_and1_val = LA_RE_INIT_AND1_VAL_A;
		cr->la_re_and1_val_m = LA_RE_INIT_AND1_VAL_A_M;
		cr->la_re_and1_inv = LA_RE_INIT_AND1_INV_A;
		cr->la_re_and1_inv_m = LA_RE_INIT_AND1_INV_A_M;
		break;

	#endif
	#ifdef HALBB_COMPILE_CLIENT_SERIES
	case BB_CLIENT:
		cr->la_clk_en = LA_CKEN_C;
		cr->la_clk_en_m = LA_CKEN_C_M;
		cr->la_en = LA_EN_C;
		cr->la_en_m = LA_EN_C_M;
		cr->dma_dbgport_base_n = LA_DBGPORT_BASE_N_C;
		cr->dma_dbgport_base_n_m = LA_DBGPORT_BASE_N_C_M;
		cr->dma_a_path_sel = LA_TYPEA_PATH_SEL_C;
		cr->dma_a_path_sel_m = LA_TYPEA_PATH_SEL_C_M;
		cr->dma_b_path_sel = LA_TYPEB_PATH_SEL_C;
		cr->dma_b_path_sel_m = LA_TYPEB_PATH_SEL_C_M;
		cr->dma_c_path_sel = LA_TYPEC_PATH_SEL_C;
		cr->dma_c_path_sel_m = LA_TYPEC_PATH_SEL_C_M;
		cr->dma_d_path_sel = LA_TYPED_PATH_SEL_C;
		cr->dma_d_path_sel_m = LA_TYPED_PATH_SEL_C_M;
		cr->dma_a_src_sel = LA_TYPEA_SRC_SEL_C;
		cr->dma_a_src_sel_m = LA_TYPEA_SRC_SEL_C_M;
		cr->dma_b_src_sel = LA_TYPEB_SRC_SEL_C;
		cr->dma_b_src_sel_m = LA_TYPEB_SRC_SEL_C_M;
		cr->dma_c_src_sel = LA_TYPEC_SRC_SEL_C;
		cr->dma_c_src_sel_m = LA_TYPEC_SRC_SEL_C_M;
		cr->dma_d_src_sel = LA_TYPED_SRC_SEL_C;
		cr->dma_d_src_sel_m = LA_TYPED_SRC_SEL_C_M;
		cr->la_smp_rate = LA_SMP_RT_SEL_C;
		cr->la_smp_rate_m = LA_SMP_RT_SEL_C_M;
		cr->rdrdy_3_phase_en = LA_RDRDY_3PHASE_EN_C;
		cr->rdrdy_3_phase_en_m = LA_RDRDY_3PHASE_EN_C_M;
		cr->la_trigger_edge = LA_EDGE_SEL_C;
		cr->la_trigger_edge_m = LA_EDGE_SEL_C_M;
		cr->dma_hdr_sel_63 = LA_HDR_SEL_63_C;
		cr->dma_hdr_sel_63_m = LA_HDR_SEL_63_C_M;
		cr->dma_hdr_sel_62 = LA_HDR_SEL_62_C;
		cr->dma_hdr_sel_62_m = LA_HDR_SEL_62_C_M;
		cr->dma_hdr_sel_61 = LA_HDR_SEL_61_C;
		cr->dma_hdr_sel_61_m = LA_HDR_SEL_61_C_M;
		cr->dma_hdr_sel_60 = LA_HDR_SEL_60_C;
		cr->dma_hdr_sel_60_m = LA_HDR_SEL_60_C_M;
		cr->dma_a_ck160_dly_en = LA_TYPEA_CK160_DLY_EN_C;
		cr->dma_a_ck160_dly_en_m = LA_TYPEA_CK160_DLY_EN_C_M;
		cr->dma_b_ck160_dly_en = LA_TYPEB_CK160_DLY_EN_C;
		cr->dma_b_ck160_dly_en_m = LA_TYPEB_CK160_DLY_EN_C_M;
		cr->dma_dbgport_phy_sel = LA_DBGPORT_SRC_SEL_C;
		cr->dma_dbgport_phy_sel_m = LA_DBGPORT_SRC_SEL_C_M;
		cr->dma_data_type = LA_DATA_C;
		cr->dma_data_type_m = LA_DATA_C_M;
		cr->r_dma_rdrdy = LA_RDRDY_C;
		cr->r_dma_rdrdy_m= LA_RDRDY_C_M;
		cr->la_and0_bit_sel = LA_TRIG_C;
		cr->la_and0_bit_sel_m = LA_TRIG_C_M;
		cr->la_trigger_cnt = LA_TRIG_CNT_C;
		cr->la_trigger_cnt_m = LA_TRIG_CNT_C_M;
		cr->and0_trig_disable = LA_TRIG_NEW_ONLY_C;
		cr->and0_trig_disable_m = LA_TRIG_NEW_ONLY_C_M;
		cr->la_and1_inv = LA_TRIG_AND1_INV_C;
		cr->la_and1_inv_m = LA_TRIG_AND1_INV_C_M;
		cr->la_and2_en = LA_TRIG_AND2_EN_C;
		cr->la_and2_en_m = LA_TRIG_AND2_EN_C_M;
		cr->la_and2_inv = LA_TRIG_AND2_INV_C;
		cr->la_and2_inv_m = LA_TRIG_AND2_INV_C_M;
		cr->la_and3_en = LA_TRIG_AND3_EN_C;
		cr->la_and3_en_m = LA_TRIG_AND3_EN_C_M;
		cr->la_and3_inv = LA_TRIG_AND3_INV_C;
		cr->la_and3_inv_m = LA_TRIG_AND3_INV_C_M;
		cr->la_and4_en = LA_TRIG_AND4_EN_C;
		cr->la_and4_en_m = LA_TRIG_AND4_EN_C_M;
		cr->la_and4_rate = LA_TRIG_AND4_VAL_C;
		cr->la_and4_rate_m = LA_TRIG_AND4_VAL_C_M;
		cr->la_and4_inv = LA_TRIG_AND4_INV_C;
		cr->la_and4_inv_m = LA_TRIG_AND4_INV_C_M;
		cr->la_and1_mask = LA_TRIG_AND1_BIT_EN_C;
		cr->la_and1_mask_m = LA_TRIG_AND1_BIT_EN_C_M;
		cr->la_and1_val = LA_TRIG_AND1_VAL_C;
		cr->la_and1_val_m = LA_TRIG_AND1_VAL_C_M;
		cr->la_and2_mask = LA_TRIG_AND2_MASK_C;
		cr->la_and2_mask_m = LA_TRIG_AND2_MASK_C_M;
		cr->la_and2_val = LA_TRIG_AND2_VAL_C;
		cr->la_and2_val_m = LA_TRIG_AND2_VAL_C_M;
		cr->la_and3_mask = LA_TRIG_AND3_MASK_C;
		cr->la_and3_mask_m = LA_TRIG_AND3_MASK_C_M;
		cr->la_and3_val = LA_TRIG_AND3_VAL_C;
		cr->la_and3_val_m = LA_TRIG_AND3_VAL_C_M;
		cr->la_and5_sel = LA_TRIG_AND5_C;
		cr->la_and5_sel_m = LA_TRIG_AND5_C_M;
		cr->la_and5_val = LA_TRIG_AND5_VAL_C;
		cr->la_and5_val_m = LA_TRIG_AND5_VAL_C_M;
		cr->la_and5_inv = LA_TRIG_AND5_INV_C;
		cr->la_and5_inv_m = LA_TRIG_AND5_INV_C_M;
		cr->la_and6_sel = LA_TRIG_AND6_C;
		cr->la_and6_sel_m = LA_TRIG_AND6_C_M;
		cr->la_and6_val = LA_TRIG_AND6_VAL_C;
		cr->la_and6_val_m = LA_TRIG_AND6_VAL_C_M;
		cr->la_and6_inv = LA_TRIG_AND6_INV_C;
		cr->la_and6_inv_m = LA_TRIG_AND6_INV_C_M;
		cr->la_and7_sel = LA_TRIG_AND7_C;
		cr->la_and7_sel_m = LA_TRIG_AND7_C_M;
		cr->la_and7_val = LA_TRIG_AND7_VAL_C;
		cr->la_and7_val_m = LA_TRIG_AND7_VAL_C_M;
		cr->la_and7_inv = LA_TRIG_AND7_INV_C;
		cr->la_and7_inv_m = LA_TRIG_AND7_INV_C_M;
		cr->la_brk_sel = BRK_R_BRK_SEL_FOR_CNT_C;
		cr->la_brk_sel_m =BRK_R_BRK_SEL_FOR_CNT_C_M;
		cr->la_mac_and1_en = LA_M_AND1_EN_C;
		cr->la_mac_and1_en_m = LA_M_AND1_EN_C_M;
		cr->la_mac_and2_en = LA_M_AND2_EN_C;
		cr->la_mac_and2_en_m = LA_M_AND2_EN_C_M;
		cr->la_mac_and0_sel = LA_M_AND0_SEL_C;
		cr->la_mac_and0_sel_m = LA_M_AND0_SEL_C_M;
		cr->la_mac_and0_en = LA_M_AND0_EN_C;
		cr->la_mac_and0_en_m = LA_M_AND0_EN_C_M;
		cr->la_mac_and0_mac_sel = INTF_R_MAC_SEL_C;
		cr->la_mac_and0_mac_sel_m = INTF_R_MAC_SEL_C_M;
		cr->la_and2_sign = LA_SIGN2_C;
		cr->la_and2_sign_m = LA_SIGN2_C_M;
		cr->la_and3_sign = LA_SIGN3_C;
		cr->la_and3_sign_m = LA_SIGN3_C_M;
		cr->la_re_trig_edge = LA_RE_INIT_POLARITY_C;
		cr->la_re_trig_edge_m = LA_RE_INIT_POLARITY_C_M;
		cr->la_re_and1_sel = LA_RE_INIT_AND1_C;
		cr->la_re_and1_sel_m = LA_RE_INIT_AND1_C_M;
		cr->la_re_and1_val = LA_RE_INIT_AND1_VAL_C;
		cr->la_re_and1_val_m = LA_RE_INIT_AND1_VAL_C_M;
		cr->la_re_and1_inv = LA_RE_INIT_AND1_INV_C;
		cr->la_re_and1_inv_m = LA_RE_INIT_AND1_INV_C_M;
		break;
	#endif

	default:
		break;
	}

}
#endif

#if LAMODE_ECHO_CMD
void
halbb_la_buffer_print(struct bb_info *bb, char input[][16], u32 *_used,
		      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct la_string_info *buf = &la->la_string_i;
	struct la_print_info  *print = &la->la_print_i;
	u64 la_pattern_msb, la_pattern_lsb;
	u64 la_pattern, la_pattern_part;
	s64 tmp_s64;
	u64 mask = 0xffffffff;
	u8 mask_length = 0;
	u32 i;
	u32 idx;
	u32 var[10] = {0};

	if (!buf->octet || buf->length == 0 || buf->length < la->smp_number)
		return;

	HALBB_SCAN(input[2], DCMD_DECIMAL, &var[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &var[1]);
	HALBB_SCAN(input[4], DCMD_DECIMAL, &var[2]);
	HALBB_SCAN(input[5], DCMD_DECIMAL, &var[3]);

	BB_TRACE("echo lamode 1 %d %d %d 0 %x %d %d %d\n\n",
		 la->la_and0_bit_sel, dma->dma_data_type,
		 la->la_mac_cfg_i.la_trigger_time,
		 la->la_dbg_port, la->la_trigger_edge, la->la_smp_rate,
		 la->la_count);
	BB_TRACE("[LA Data Dump] smp_number = %d\n", la->smp_number);
	BB_TRACE("Dump_Start\n");

	print->print_len = (u8)var[0];

	if (var[0] == 0) {
		for (i = 0; i < la->smp_number; i++) {
			idx = i << 1;
			BB_TRACE("%08x%08x\n", buf->octet[idx + 1],
				 buf->octet[idx]);
		}
	} else if (var[0] == 1) {

		print->print_mode = (u8)var[1];
		print->print_lsb = (u8)var[2];
		print->print_msb = (u8)var[3];

		/*------------------------*/
		if (var[1] == 0)
			BB_TRACE("[Hex]\n");
		else if (var[1] == 1)
			BB_TRACE("[Dec unsigned]\n");
		else if (var[1] == 2)
			BB_TRACE("[Dec signed]\n");

		BB_TRACE("BIT[%d:%d]\n", var[3], var[2]);

		if (var[2] > var[3]) {
			BB_TRACE("[Warning] BIT_L > BIT_H\n");
			return;
		}

		mask_length = (u8)(var[3] - var[2] + 1);
		mask = halbb_gen_mask_from_0(mask_length) << var[2];
		/*------------------------*/
		for (i = 0; i < la->smp_number; i++) {
			idx = i << 1;
			la_pattern_msb = (u64)buf->octet[idx + 1];
			la_pattern_lsb = (u64)buf->octet[idx];
			la_pattern = (la_pattern_msb << 32) | la_pattern_lsb;
			la_pattern_part = (la_pattern & mask) >> var[2];

			if (var[1] == 0) {
				BB_TRACE("0x%llx\n", la_pattern_part);
			} else if (var[1] == 1) {
				BB_TRACE("%llu\n", la_pattern_part);
			} else if (var[1] == 2) {
				tmp_s64 = halbb_cnvrt_2_sign_64(la_pattern_part,
								mask_length);
				BB_TRACE("%lld\n", tmp_s64);
			}
		}
	}
	BB_TRACE("Dump_End\n\n");
}

void halbb_la_cmd_bb_show_cfg(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct la_re_trig_info *re = &la->la_re_trig_i;
	struct la_trig_mac_info	*trig_mac = &la->la_trig_mac_i;
	struct la_print_info	*print = &la->la_print_i;
	struct la_mac_cfg_info	*cfg = &la->la_mac_cfg_i;

	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "cmn {TrigTime:%d} {TrigCnt:%d} {DbgPort:%x} {Edge:P/N:%d} {f_smp:%d}\n", 
		    cfg->la_trigger_time, la->la_trigger_cnt,
		    la->la_dbg_port, la->la_trigger_edge, la->la_smp_rate);
#if 1
	/*BB DMA*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {0:dbgPort_base_N} {N:%d}\n",
		    dma->dma_dbgport_base_n);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {1:path_sel} {A:%d} {B:%d} {C:%d} {D:%d}\n",
		    dma->dma_a_path_sel, dma->dma_b_path_sel,
		    dma->dma_c_path_sel, dma->dma_d_path_sel);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {2:src_sel} {A:%d} {B:%d} {C:%d} {D:%d}\n",
		    dma->dma_a_src_sel, dma->dma_b_src_sel,
		    dma->dma_c_src_sel, dma->dma_d_src_sel);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {3:hdr_sel} {B63:%d} {B62:%d} {B61:%d} {B60:%d}\n",
		    dma->dma_hdr_sel_63, dma->dma_hdr_sel_62,
		    dma->dma_hdr_sel_61, dma->dma_hdr_sel_60);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {4:phy_sel} {phy:%d}\n", dma->dma_dbgport_phy_sel);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "dma {5:dma_sel} {type:%d}\n", dma->dma_data_type);

	/*BB -Trig*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {0:And0} {disable:%d} {bit_num:%d}\n",
		    la->la_and0_disable, la->la_and0_bit_sel);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {1:And1} {en_bitmap:%d} {inv:%d} {bitmap:%d}\n",
		    adv->la_and1_mask, adv->la_and1_inv, adv->la_and1_val);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {2:And2} {en:%d} {inv:%d} {val:%d} {mask(0x%x)} {sign:%d}\n", 
		    adv->la_and2_en, adv->la_and2_inv, adv->la_and2_val,
		    adv->la_and2_mask, adv->la_and2_sign);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {3:And3} {en:%d} {inv:%d} {val:%d} {mask(0x%x)} {sign:%d}\n", 
		    adv->la_and3_en, adv->la_and3_inv, adv->la_and3_val,
		    adv->la_and3_mask, adv->la_and3_sign);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {4:And4} {en:%d} {inv:%d} {rate_idx:%d}\n",
		    adv->la_and4_en, adv->la_and4_inv, adv->la_and4_rate);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {5:And5} {en/sel:%d} {inv:%d} {val:%d}\n",
		    adv->la_and5_sel, adv->la_and5_inv, adv->la_and5_val);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {6:And6} {en/sel:%d} {inv:%d} {val:%d}\n",
		    adv->la_and6_sel, adv->la_and6_inv, adv->la_and6_val);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "bb {7:And7} {en/sel:%d} {inv:%d} {val:%d}\n",
		    adv->la_and7_sel, adv->la_and7_inv, adv->la_and7_val);
	/*MAC Adv-Trig*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "mac {0:And0} {en:%d} {0:cca,1:er,2:ok:%d} {0:tmac,1:pmac:%d}\n",
		    trig_mac->la_mac_and0_en, trig_mac->la_mac_and0_sel,
		    trig_mac->la_mac_and0_mac_sel);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "mac {1:And1} {en:%d}\n", trig_mac->la_mac_and1_en);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "mac {2:And2} {en:%d} {frame_type(%x)}\n",
		    trig_mac->la_mac_and2_en, trig_mac->la_mac_and2_frame_sel);
	/*BB Re-Trig*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "re {mac_en:%d} {en/sel:%d} {inv:%d} {val:%d} {edge:%d}\n",
		    cfg->mac_la_restart_en, re->la_re_and0_sel,
		    re->la_re_and0_inv, re->la_re_and0_val,
		    re->la_re_trig_edge);
	/*Buffer*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "buf {0:64K, 1:128K, 2:192K, 3:256K, 4:320K:%d}\n",
		    cfg->mac_la_buf_sel);

	/*control*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "poll {la_polling_cnt:%d} (polling time = %d * 100ms)\n",
		    la->la_polling_cnt, la->la_polling_cnt);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "tmac {timeout_en:%d} {timeout_val:%d} (timeout= 1s << %d )\n",
		    cfg->mac_la_timeout_en, cfg->mac_la_timeout_val,
		    cfg->mac_la_timeout_val);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "get {finish_ofst} {is_round_up} (get buffer data for timeout)\n");

	/*Print*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "print {0:all(Hex):%d}\n", print->print_len);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "print {1:partial:%d} {0:hex,1:dec,2:s-dec:%d} {bit_L:%d} {bit_H:%d}\n",
		    print->print_len, print->print_mode, print->print_lsb,
		    print->print_msb);

	/*Setting*/
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "set {1:Fake Trig} {en:%d}\n", la->not_stop_trig);
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		    "set {2:Auto Print} {en:%d}\n",
		    print->is_la_print);
#endif
}

void halbb_la_cmd_bb_cmn(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	u32 val[10] = {0};

	HALBB_SCAN(input[2], DCMD_DECIMAL, &val[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &val[1]);
	HALBB_SCAN(input[4], DCMD_HEX, &val[2]);
	HALBB_SCAN(input[5], DCMD_DECIMAL, &val[3]);
	HALBB_SCAN(input[6], DCMD_DECIMAL, &val[4]);

	la->la_mac_cfg_i.la_trigger_time = val[0];
	la->la_trigger_cnt = (u8)val[1];
	la->la_dbg_port = val[2];
	la->la_trigger_edge = (u8)val[3];
	la->la_smp_rate = (enum la_bb_smp_clk)val[4];
}

void halbb_la_cmd_bb_dma(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	u32 val[10] = {0};

	HALBB_SCAN(input[2], DCMD_DECIMAL, &val[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &val[1]);
	HALBB_SCAN(input[4], DCMD_DECIMAL, &val[2]);
	HALBB_SCAN(input[5], DCMD_DECIMAL, &val[3]);
	HALBB_SCAN(input[6], DCMD_DECIMAL, &val[4]);

	if (val[0] == 0) {
		dma->dma_dbgport_base_n = (u8)val[1];
	} else if (val[0] == 1) {
		dma->dma_a_path_sel = (u8)val[1];
		dma->dma_b_path_sel = (u8)val[2];
		dma->dma_c_path_sel = (u8)val[3];
		dma->dma_d_path_sel = (u8)val[4];
	} else if (val[0] == 2) {
		dma->dma_a_src_sel = (u8)val[1];
		dma->dma_b_src_sel = (u8)val[2];
		dma->dma_c_src_sel = (u8)val[3];
		dma->dma_d_src_sel = (u8)val[4];
	} else if (val[0] == 3) {
		dma->dma_hdr_sel_63 = (u8)val[1];
		dma->dma_hdr_sel_62 = (u8)val[2];
		dma->dma_hdr_sel_61 = (u8)val[3];
		dma->dma_hdr_sel_60 = (u8)val[4];
	} else if (val[0] == 4) {
		dma->dma_dbgport_phy_sel = (bool)val[1];
	} else if (val[0] == 5) {
		dma->dma_data_type = (enum la_dma_data_type_t)val[1];
	}
}

void halbb_la_cmd_bb_trig(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;
	u32 val[10] = {0};

	HALBB_SCAN(input[2], DCMD_HEX, &val[0]);
	HALBB_SCAN(input[3], DCMD_HEX, &val[1]);
	HALBB_SCAN(input[4], DCMD_HEX, &val[2]);
	HALBB_SCAN(input[5], DCMD_HEX, &val[3]);
	HALBB_SCAN(input[6], DCMD_HEX, &val[4]);
	HALBB_SCAN(input[7], DCMD_HEX, &val[5]);
	HALBB_SCAN(input[8], DCMD_HEX, &val[6]);

	if (val[0] == 0) {
		la->la_and0_disable = (bool)val[1];
		la->la_and0_bit_sel  = val[2];
	} else if (val[0] == 1) {
		adv->la_and1_mask = val[1];
		adv->la_and1_inv = (bool)val[2];
		adv->la_and1_val = val[3];
	} else if (val[0] == 2) {
		adv->la_and2_en = (bool)val[1];
		adv->la_and2_inv = (bool)val[2];
		adv->la_and2_val = val[3];
		adv->la_and2_mask = val[4];
		adv->la_and2_sign = (u8)val[5];
	} else if (val[0] == 3) {
		adv->la_and3_en = (bool)val[1];
		adv->la_and3_inv = (bool)val[2];
		adv->la_and3_val = val[3];
		adv->la_and3_mask = val[4];
		adv->la_and3_sign = (u8)val[5];
	} else if (val[0] == 4) {
		adv->la_and4_en = (bool)val[1];
		adv->la_and4_inv = (bool)val[2];
		adv->la_and4_rate = (u16)val[3];
	} else if (val[0] == 5) {
		adv->la_and5_sel = (u8)val[1];
		adv->la_and5_inv = (bool)val[2];
		adv->la_and5_val = (u8)val[3];
	} else if (val[0] == 6) {
		adv->la_and6_sel = (u8)val[1];
		adv->la_and6_inv = (bool)val[2];
		adv->la_and6_val = (u8)val[3];
	} else if (val[0] == 7) {
		adv->la_and7_sel = (u8)val[1];
		adv->la_and7_inv = (bool)val[2];
		adv->la_and7_val = (u8)val[3];
	}

	if (adv->la_and1_mask == 0 &&
	    !adv->la_and2_en && !adv->la_and3_en && !adv->la_and4_en &&
	    adv->la_and5_sel == 0 &&
	    adv->la_and6_sel == 0 &&
	    adv->la_and7_sel == 0) {
		adv->adv_trig_en = false;
	} else {
		adv->adv_trig_en = true;
	}

	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		 "[Adv_trig_en=%d]\n\n", adv->adv_trig_en);
}

void halbb_la_cmd_bb_re_trig(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_re_trig_info *re = &la->la_re_trig_i;
	u32 val[10] = {0};

	HALBB_SCAN(input[2], DCMD_DECIMAL, &val[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &val[1]);
	HALBB_SCAN(input[4], DCMD_DECIMAL, &val[2]);
	HALBB_SCAN(input[5], DCMD_DECIMAL, &val[3]);

	la->la_mac_cfg_i.mac_la_restart_en = (u8)val[0];
	re->re_trig_en = (bool)val[0];

	re->la_re_and0_sel = (u8)val[1];
	re->la_re_and0_inv = (bool)val[2];
	re->la_re_and0_val = (u8)val[3];
	re->la_re_trig_edge = (bool)val[4];
}


void halbb_la_cmd_mac_trig(struct bb_info *bb, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_trig_mac_info	*trig_mac = &la->la_trig_mac_i;
	u32 val[10] = {0};

	HALBB_SCAN(input[2], DCMD_DECIMAL, &val[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &val[1]);
	HALBB_SCAN(input[4], DCMD_HEX, &val[2]);
	HALBB_SCAN(input[5], DCMD_HEX, &val[3]);

	if (val[0] == 0) {
		trig_mac->la_mac_and0_en = (bool)val[1];
		trig_mac->la_mac_and0_sel = (u8)val[2];
		trig_mac->la_mac_and0_mac_sel = (u8)val[3];
	} else if (val[0] == 1) {
		trig_mac->la_mac_and1_en = (bool)val[1];
	} else if (val[0] == 2) {
		trig_mac->la_mac_and2_en = (bool)val[1];
		trig_mac->la_mac_and2_frame_sel = (u8)val[2];
	}

	if (!trig_mac->la_mac_and0_en &&
	    !trig_mac->la_mac_and1_en &&
	    !trig_mac->la_mac_and2_en)
		trig_mac->la_mac_trig_en = false;
	else
		trig_mac->la_mac_trig_en = true;
	
}

void halbb_la_cmd_fast(struct bb_info *bb, char input[][16], u32 *_used,
			    char *output, u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;
	struct la_re_trig_info *re_trig = &la->la_re_trig_i;
	u32 trig_time_cca = 0;
	s32 val_sign32_tmp = 0;
	u32 var[10] = {0};
	enum channel_width bw = bb->hal_com->band[0].cur_chandef.bw;

	if (bw > 2) {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "Not Support for BW > %dM\n", 20 << bw);
		return;
	}
	HALBB_SCAN(input[2], DCMD_DECIMAL, &var[0]);

	trig_time_cca = ((la->la_string_i.smp_number_max >> (bw + 1)) / 10)
			- (2 << (2 - bw)) - (2 - bw);

	if (var[0] < 10) {
	/*=== [Type: 0 ~ 10] : CCA P-edge trigger ==========================*/
		/*--- Basic Trigger Setting --------------------------------*/
		la->la_mac_cfg_i.la_trigger_time = trig_time_cca;
		la->la_trigger_edge = 0;
		la->la_smp_rate = 2 - bw;
		la->la_count = 0;
		la->la_trigger_cnt = 0;
		if (var[0] == 0) { /*CCA*/
			if (bb->ic_type == BB_RTL8852AA) {
				la->la_and0_disable = true;
				la->la_dbg_port = 0x10205;
				halbb_la_bb_set_dma_type_reset(bb);
				halbb_la_bb_set_adv_reset(bb);
				halbb_la_mac_set_adv_reset(bb);
				halbb_la_bb_set_re_trig_reset(bb);
				adv->adv_trig_en = false;
				BB_TRACE("[Fast trigger 0: CCA]\n");
			} else if (bb->ic_type == BB_RTL8852A) {
				la->la_and0_disable = true;
				la->la_dbg_port = 0x10205;
				halbb_la_bb_set_dma_type_reset(bb);
				halbb_la_bb_set_adv_reset(bb);
				halbb_la_mac_set_adv_reset(bb);
				halbb_la_bb_set_re_trig_reset(bb);
				adv->adv_trig_en = false;
				adv->la_and5_sel = 2;
				adv->la_and5_inv = false;
				adv->la_and5_val = 0;
				BB_TRACE("[Fast trigger 0: OFDM CCA]\n");
			}
		} else {
			BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
				    "Not Support\n");
			return;
		}
		
	} else {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "Not Support\n");
		return;
	}

	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		 "[Basic-Trigger]\n");
#if 0
	struct bb_la_mode_info *la = &bb->bb_la_mode_i;
	struct la_adv_trig_info *adv = &la->adv_trig_i;
	enum bb_mode_type bb_mode;
	const u8 ofdm_codeword[8] = {0xb, 0xf, 0xa, 0xe, 0x9, 0xd, 0x8, 0xc};
	u32 codeword;
	u8 rate_idx;
	u32 trig_time_cca = 0;
	s32 val_sign32_tmp = 0;
	u32 var[10] = {0};
	enum channel_width bw = bb->hal_com->band[0].cur_chandef.bw;

	if (bw > 2) {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "Not Support for BW > %dM\n", 20 << bw);
		return;
	}

	HALBB_SCAN(input[2], DCMD_DECIMAL, &var[0]);
	HALBB_SCAN(input[3], DCMD_DECIMAL, &var[1]);
	HALBB_SCAN(input[4], DCMD_DECIMAL, &var[2]);

	trig_time_cca = ((la->la_string_i.smp_number_max >> (bw + 1)) / 10)
			- (2 << (2 - bw)) - (2 - bw);

	if (var[0] < 10) {
	/*=== [Type: 0 ~ 10] : CCA P-edge trigger ==========================*/
		/*--- Basic Trigger Setting --------------------------------*/
		la->la_and0_bit_sel = 2;
		la->la_mac_cfg_i.la_trigger_time = trig_time_cca;
		la->la_trigger_edge = 0;
		la->la_smp_rate = 2 - bw;
		la->la_count = 0;
		if (var[0] == 0) { /*AGC*/
			dma->dma_data_type = 5;
			la->la_dbg_port = 0x870;
		} else if (var[0] == 1) { /*EVM*/
			dma->dma_data_type = 4;
			la->la_dbg_port = 0x392;
		} else if (var[0] == 2) { /*SNR*/
			dma->dma_data_type = 4;
			if (var[1] == 0)
				la->la_dbg_port = 0x89e;
			else
				la->la_dbg_port = 0xa9e;
		} else if (var[0] == 3) { /*CFO*/
			dma->dma_data_type = 4;
			if (var[1] == 0)
				la->la_dbg_port = 0x88c;
			else
				la->la_dbg_port = 0xa8c;
		}  else if (var[0] == 4) { /*ADC*/
			if (var[1] == 0) {
				dma->dma_data_type = 0;
				la->la_dbg_port = 0x880;
			} else {
				dma->dma_data_type = 1;
				la->la_dbg_port = 0xa80;
			}
		}
		/*--- Adv-Trigger Setting------------------------------------*/
		adv->adv_trig_en = false;
	} else if (var[0] < 20) {
	/*=== [Type: 10 ~ 19]: RX-EVM Trigger ===============================*/
		/*--- Basic Trigger Setting ---------------------------------*/
		la->la_and0_bit_sel = 0;
		la->la_trigger_edge = 0;
		la->la_smp_rate = 2 - bw;
		la->la_count = 0;
		dma->dma_data_type = 4;
		la->la_dbg_port = 0x392;

		/*--- Adv-Trigger Setting -----------------------------------*/
		halbb_la_bb_set_adv_reset(bb);
		adv->adv_trig_en = true;

		/*And[0]*/
		la->la_and0_disable = true;

		/*And[1]*/
		adv->la_and1_inv = 0;
		adv->la_and1_sel = 4; /*RX-state*/
		if (var[2] == 0) {
			/*L-preamble 8+8+4 = 20*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 20;
			/*Legacy Data*/
			adv->la_and1_val = 5;
		} else if (var[2] == 1) {
			/*HT-preamble (8+8+4) + (8+4+4*Nrx) = 32 + Nrx * 4*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 32 -
					       (bb->num_rf_path * 4);
			/*HT Data*/
			adv->la_and1_val = 18;
		} else {
			/*VHT-preamble (8+8+4) + (8+4+4*Nrx) +4 = 36 + Nrx * 4*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 36 -
					       (bb->num_rf_path * 4);
			/*VHT Data*/
			adv->la_and1_val = 18;
		}

		/*And[2]*/
		adv->la_and2_inv = 0;
		adv->la_and2_sel = 0; /*Disable*/

		/*And[3]*/
		adv->la_and2_inv = 0;
		adv->la_and3_sel = 0; /*Disable*/

		/*And[4]*/
		adv->la_and4_inv = 0;

		if (var[0] == 11) {
			/*[>= -X dB]*/
			if (var[1] == 2) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0x1;
			} else if (var[1] == 4) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0x3;
			} else if (var[1] == 8) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0x7;
			} else if (var[1] == 16) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0xf;
			} else if (var[1] == 32) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0x1f;
			} else if (var[1] == 64) {
				adv->la_and4_bitmap = 0;
				adv->la_and4_mask = 0x3f;
			} else {
				BB_DBG_CNSL(*_out_len, *_used, output + *_used,
					 *_out_len - *_used,
					 "Not Support >= -%d dB\n", var[1]);
				return;
			}
		} else if (var[0] == 10) {
			/*[<= -X dB]*/
			if (var[1] == 2) {
				adv->la_and4_bitmap = 0x7e;
				adv->la_and4_mask = 0x7e;
			} else if (var[1] == 4) {
				adv->la_and4_bitmap = 0x7c;
				adv->la_and4_mask = 0x7c;
			} else if (var[1] == 8) {
				adv->la_and4_bitmap = 0x78;
				adv->la_and4_mask = 0x78;
			} else if (var[1] == 16) {
				adv->la_and4_bitmap = 0x70;
				adv->la_and4_mask = 0x70;
			} else if (var[1] == 32) {
				adv->la_and4_bitmap = 0x60;
				adv->la_and4_mask = 0x60;
			} else if (var[1] == 64) {
				adv->la_and4_bitmap = 0x40;
				adv->la_and4_mask = 0x40;
			} else {
				BB_DBG_CNSL(*_out_len, *_used, output + *_used,
					 *_out_len - *_used,
					 "Not Support <= -%d dB\n", var[1]);
				return;
			}
		} else if (var[0] == 12) {
			/*[= -X dB]*/
			val_sign32_tmp = 0 - (s32)var[1];
			adv->la_and4_bitmap = (u32)(val_sign32_tmp & 0x7f);
			adv->la_and4_mask = 0x7f;
		}
	} else if (var[0] < 30) {
	/*=== [Type: 20 ~ 29]: RX-Rate Trigger ==============================*/
		/*--- Basic Trigger Setting ---------------------------------*/
		la->la_and0_bit_sel = 0;
		la->la_trigger_edge = 0;
		la->la_smp_rate = 2 - bw;
		la->la_count = 0;
		dma->dma_data_type = 4;

		rate_idx = (u8)var[1];

		/*--- Adv-Trigger Setting -----------------------------------*/
		halbb_la_bb_set_adv_reset(bb);
		adv->adv_trig_en = true;

		/*And[0]*/
		la->la_and0_disable = true;

		/*And[1]*/
		adv->la_and1_inv = 0;
		adv->la_and1_sel = 4; /*RX-state*/

		if (rate_idx <= BB_54M && rate_idx >= BB_06M) {
			bb_mode = BB_LEGACY_MODE;
			codeword = (u32)ofdm_codeword[rate_idx - BB_06M];
			la->la_dbg_port = 0x3a9;
			/*L-preamble 8+8 = 16*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 20;
			/*Legacy Data*/
			adv->la_and1_val = 5;
		} else if (rate_idx <= BB_HT_MCS(31)) {
			bb_mode = BB_HT_MODE;
			codeword = (u32)(rate_idx - BB_HT_MCS(0));
			la->la_dbg_port = 0x3aa;
			/*HT-preamble (8+8+4) + (8+4+4*Nrx) = 32 + Nrx * 4*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 32 -
					       (bb->num_rf_path * 4);
			/*HT,VHT Data*/
			adv->la_and1_val = 18;
		} else if (rate_idx <= BB_VHT_4SS_MCS(9)) {
			bb_mode = BB_VHT_MODE;
			codeword = (u32)halbb_rate_order_compute(bb, rate_idx);
			codeword--;
			la->la_dbg_port = 0x3ab;
			/*VHT-preamble (8+8+4) + (8+4+4*Nrx) = 36 + Nrx * 4*/
			la->la_mac_cfg_i.la_trigger_time = trig_time_cca - 36 -
					       (bb->num_rf_path * 4);
			/*HT,VHT Data*/
			adv->la_and1_val = 18;
		} else {
			BB_DBG_CNSL(*_out_len, *_used, output + *_used,
				 *_out_len - *_used,
				 "Not Support\n");
			return;
		}

		/*And[2]*/
		adv->la_and2_inv = 0;
		adv->la_and2_sel = 0; /*Disable*/

		/*And[3]*/
		adv->la_and2_inv = 0;
		adv->la_and3_sel = 0; /*Disable*/

		/*And[4]*/
		adv->la_and4_inv = 0;

		if (var[0] == 20) {
			if (bb_mode == BB_LEGACY_MODE) {
				adv->la_and4_bitmap = codeword;
				adv->la_and4_mask = 0x3000000f;
			} else if (bb_mode == BB_HT_MODE) {
				adv->la_and4_bitmap = (2 << 28) | codeword;
				adv->la_and4_mask = 0x3000003f;
			}  else { /* BB_VHT_MODE*/
				adv->la_and4_bitmap = (1 << 28) |
						      (codeword << 4);
				adv->la_and4_mask = 0x300000f0;
			}
		} else {
			BB_DBG_CNSL(*_out_len, *_used, output + *_used,
				 *_out_len - *_used,
				 "Not Support\n");
			return;
		}
	} else {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "Not Support\n");
		return;
	}
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		 "[Basic-Trigger]\n");
	BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
		 "  *echo lamode 1 %d %d %d 0 %x %d %d %d\n\n",
		 la->la_and0_bit_sel, dma->dma_data_type,
		 la->la_mac_cfg_i.la_trigger_time,
		 la->la_dbg_port, la->la_trigger_edge, la->la_smp_rate,
		 la->la_count);
	BB_TRACE("echo lamode 1 %d %d %d 0 %x %d %d %d\n\n",
		 la->la_and0_bit_sel, dma->dma_data_type,
		 la->la_mac_cfg_i.la_trigger_time,
		 la->la_dbg_port, la->la_trigger_edge, la->la_smp_rate,
		 la->la_count);

	if (adv->adv_trig_en) {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "[Adv-Trigger]\n");
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "  *And0 Disable=%d\n", la->la_and0_disable);
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "  *And1{sel,val,inv}={0x%x,0x%x,%d}\n  *And2{sel,val,inv}={0x%x,0x%x,%d}\n  *And3{sel,val,inv}={0x%x,0x%x,%d}\n",
			 adv->la_and1_sel, adv->la_and1_val, adv->la_and1_inv,
			 adv->la_and2_sel, adv->la_and2_val, adv->la_and2_inv,
			 adv->la_and3_sel, adv->la_and3_val, adv->la_and3_inv);
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			 "  *And4{mask,bitmap,inv}={0x%x,0x%x,%d}\n",
			 adv->la_and4_mask, adv->la_and4_bitmap,
			 adv->la_and4_inv);
	}
#endif
	halbb_la_run(bb);
}


void halbb_la_cmd_dbg(struct bb_info *bb, char input[][16], u32 *_used, char *output,
		  u32 *_out_len)
{
	struct bb_la_mode_info *la = &bb->bb_cmn_hooker->bb_la_mode_i;
	struct la_dma_info	*dma = &la->la_dma_i;
	struct la_trig_mac_info	*trig_mac = &la->la_trig_mac_i;
	struct la_mac_cfg_info	*cfg = &la->la_mac_cfg_i;
	struct la_string_info *buf = &la->la_string_i;
	u32 val[10] = {0};
	u32 used = *_used;
	u32 out_len = *_out_len;
	u8 i = 0;

	HALBB_SCAN(input[1], DCMD_DECIMAL, &val[0]);

	/*@dbg_print("echo cmd input_num = %d\n", input_num);*/

	if ((_os_strcmp(input[1], "-h") == 0)) {
		/*BB Basic Trigger*/
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "BB_trig:  1 0 {DbgPort Bit} {DMA#} {TrigTime} {TrigCnt}\n\t {DbgPort} {Edge:P/N} {f_smp} {Cap_num}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "MAC_trig: 1 1 {0:cca,1:ok_pmac,2:er_pmac,3:ok,4:er} {DMA#} {TrigTime} {trig_cnt}\n\t {DbgPort} {Edge:0(P),1(N)} {f_smp} {Cpture num}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "fast\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "buf {0~4: 64K/128K/192K/256K/320K}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "disable\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "show\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "reset\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "trig\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "ptrn_chk {show(END), rst(END), {ptrn_idx:0~3} {smp_point} {msb32_mask(hex)} {val(hex)}\n");        
#if 0
		/*Fast Trigger*/
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {0: CCA trig & AGC Dbg Port}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {1: CCA trig & EVM Dbg Port}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {2: CCA trig & SNR Dbg Port}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {3: CCA trig & CFO Dbg Port}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {4: CCA trig & ADC output Dbg Port}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {10: EVM>=-X dB, 11: EVM<=-X dB} {X=2/4/8/16/32/64} {0:Lgcy, 1:HT}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {12: EVM=-X dB} {X} {0:Lgcy, 1:HT}\n");
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			 "fast {20: RX-rate-idx=X} {X}\n");
#endif
		goto LA_END;
	}

	if (_os_strcmp(input[1], "ptrn_chk") == 0) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[0]);
		if (_os_strcmp(input[2], "show") == 0) {
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				    "[LA CHK Pattern] ===>\n");
		} else if (_os_strcmp(input[2], "rst") == 0) {
			halbb_mem_set(bb, la->la_ptrn_chk_i, 0, sizeof(struct la_ptrn_chk_info) * LA_CHK_PTRN_NUM);
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				    "ptrn_chk reset(Disable)\n");
		} else if (val[0] < LA_CHK_PTRN_NUM) {
			HALBB_SCAN(input[3], DCMD_DECIMAL, &val[1]);
			HALBB_SCAN(input[4], DCMD_HEX, &val[2]);
			HALBB_SCAN(input[5], DCMD_HEX, &val[3]);

			if (val[1] < la->la_string_i.smp_number_max) {
				la->la_ptrn_chk_i[val[0]].smp_point = val[1];
				la->la_ptrn_chk_i[val[0]].la_ptrn_chk_mask = val[2];
				la->la_ptrn_chk_i[val[0]].la_ptrn_chk_val = val[3];
			} else {
				BB_DBG_CNSL(out_len, used, output + used, out_len - used,
					    "[Err] smp_number_max=%d\n", la->la_string_i.smp_number_max);
				goto LA_END;
			}
		} else {
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				    "Set Err, idx=%d\n", val[0]);
			goto LA_END;
		}

		for (i = 0; i < LA_CHK_PTRN_NUM; i++) {
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				    "[%d] point=%05d, chk_mask=0x%08x, chk_val=0x%x\n",
				    i, la->la_ptrn_chk_i[i].smp_point,
				    la->la_ptrn_chk_i[i].la_ptrn_chk_mask,
				    la->la_ptrn_chk_i[i].la_ptrn_chk_val);
		}
	} else if ((_os_strcmp(input[1], "fast") == 0)) {
		halbb_la_cmd_fast(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "show") == 0)) {
		halbb_la_cmd_bb_show_cfg(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "reset") == 0)) {
		BB_DBG_CNSL(*_out_len, *_used, output + *_used, *_out_len - *_used,
			    "Reset\n");
		halbb_la_reset(bb);
	} else if ((_os_strcmp(input[1], "trig") == 0)) {
		halbb_la_run(bb);
	} else if ((_os_strcmp(input[1], "cmn") == 0)) {
		halbb_la_cmd_bb_cmn(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "dma") == 0)) {
		halbb_la_cmd_bb_dma(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "bb") == 0)) {
		halbb_la_cmd_bb_trig(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "mac") == 0)) {
		halbb_la_cmd_mac_trig(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "tmac") == 0)) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);
		HALBB_SCAN(input[3], DCMD_DECIMAL, &val[2]);
		cfg->mac_la_timeout_en = (u8)val[1];
		cfg->mac_la_timeout_val = (u8)val[2];
	} else if ((_os_strcmp(input[1], "poll") == 0)) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);
		la->la_polling_cnt = (u8)val[1];
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "la_polling_cnt = %d, polling time = %d * 100ms\n",
			    la->la_polling_cnt, la->la_polling_cnt);
	} else if ((_os_strcmp(input[1], "get") == 0)) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);
		HALBB_SCAN(input[3], DCMD_DECIMAL, &val[2]);
		la->la_mode_state = LA_STATE_GET_DLE_BUF;
		halbb_mem_set(bb, buf->octet, 0, buf->length);
		halbb_la_rpt_buf_get(bb, (u16)val[1], (bool)val[2]);
		halbb_la_stop(bb);
	} else if ((_os_strcmp(input[1], "re") == 0)) {
		halbb_la_cmd_bb_re_trig(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "buf") == 0)) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);
		la->la_mac_cfg_i.mac_la_buf_sel = (enum la_buff_mode_t)val[1];
		la->la_mac_cfg_i.mac_alloc_success = halbb_la_mac_cfg_buf(bb, la->la_mac_cfg_i.mac_la_buf_sel);

		if (!la->la_mac_cfg_i.mac_alloc_success)
			halbb_la_stop(bb);

		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "MAC_BUF_alloc_success=%d, Buff_mode=(%d)K\n",
			    la->la_mac_cfg_i.mac_alloc_success,
			    64 * (la->la_mac_cfg_i.mac_la_buf_sel + 1));
	} else if ((_os_strcmp(input[1], "set") == 0)) {
		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);

		if (val[1] == 1) {
			HALBB_SCAN(input[3], DCMD_DECIMAL, &val[2]);
			la->not_stop_trig = (bool)val[2];
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				 "not_stop_trig=(%d)\n", la->not_stop_trig);
		} else if (val[1] == 2) {
			HALBB_SCAN(input[3], DCMD_DECIMAL, &val[2]);
			la->la_print_i.is_la_print = (bool)val[2];
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				 "Auto print=(%d)\n", la->la_print_i.is_la_print);
		}
	} else if ((_os_strcmp(input[1], "print") == 0)) {
		halbb_la_buffer_print(bb, input, &used, output, &out_len);
	} else if ((_os_strcmp(input[1], "disable") == 0)) {
		halbb_la_stop(bb);
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "Disable LA mode\n");
	} else if (val[0] == 1) {
		if (!la->la_mac_cfg_i.mac_alloc_success) {
			BB_DBG_CNSL(out_len, used, output + used, out_len - used,
				    "Need to set MAC CR(buf) first: echo bb lamode buf {val}\n");
		}

		HALBB_SCAN(input[2], DCMD_DECIMAL, &val[1]);
		HALBB_SCAN(input[3], DCMD_DECIMAL, &val[2]);
		HALBB_SCAN(input[4], DCMD_DECIMAL, &val[3]);
		HALBB_SCAN(input[5], DCMD_DECIMAL, &val[4]);
		HALBB_SCAN(input[6], DCMD_HEX, &val[5]);
		HALBB_SCAN(input[7], DCMD_HEX, &val[6]);
		HALBB_SCAN(input[8], DCMD_DECIMAL, &val[7]);
		HALBB_SCAN(input[9], DCMD_DECIMAL, &val[8]);
		HALBB_SCAN(input[10], DCMD_DECIMAL, &val[9]);

		halbb_la_bb_set_adv_reset(bb);
		halbb_la_mac_set_adv_reset(bb);
		halbb_la_bb_set_re_trig_reset(bb);

		la->la_basic_mode_sel = (u8)val[1];
		la->la_and0_bit_sel = val[2];
		la->la_mac_cfg_i.la_trigger_time = val[4]; /*unit: us*/
		la->la_trigger_cnt= (u8)val[5];
		la->la_dbg_port = val[6];
		la->la_trigger_edge = (u8)val[7];
		la->la_smp_rate = (enum la_bb_smp_clk)(val[8]);
		la->la_count = val[9];

		if (la->la_basic_mode_sel == 0) {
			dma->dma_data_type = (u8)val[3];
			la->la_and0_disable = false;
			la->adv_trig_i.adv_trig_en = false;
			la->la_re_trig_i.re_trig_en = false;
			trig_mac->la_mac_trig_en = false;
		} else {
			la->la_and0_disable = true;
			la->adv_trig_i.adv_trig_en = false;
			la->la_re_trig_i.re_trig_en = false;
			trig_mac->la_mac_trig_en = true;
			trig_mac->la_mac_and0_en = true;
			trig_mac->la_mac_and0_sel = (u8)val[3];
		}

		BB_TRACE("echo bb lamode %d %d %d %d %d %d %x %d %d %d\n\n",
			 val[0], val[1], val[2], val[3], val[4],
			 val[5], val[6], val[7], val[8], val[9]);

		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "a.En= ((1)),  b.Mode = ((%d)), c.Trig_sel = ((0x%x)), d.Dma_type = ((%d))\n",
			 la->la_and0_bit_sel, la->la_basic_mode_sel,
			 dma->dma_data_type);
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "e.Trig_time = ((%dus)), f.Trig_cnt = ((%d)), g.Dbg_port = ((0x%x))\n",
			    la->la_mac_cfg_i.la_trigger_time, la->la_trigger_cnt,
			    la->la_dbg_port);
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "h.Trig_edge = ((%d)), i.La rate = ((%d MHz)), j.Cap_num = ((%d))\n\n",
			    la->la_trigger_edge, (80 >> la->la_smp_rate),
			    la->la_count);
		BB_DBG_CNSL(out_len, used, output + used, out_len - used,
			    "{And0_disable, adv_trig, re_trig, mac_trig}= {%d, %d, %d, %d}\n",
			    la->la_and0_disable, la->adv_trig_i.adv_trig_en,
			    la->la_re_trig_i.re_trig_en,
			    trig_mac->la_mac_trig_en);

		halbb_la_run(bb);
	}
LA_END:
	*_used = used;
	*_out_len = out_len;
}
#endif

#endif
