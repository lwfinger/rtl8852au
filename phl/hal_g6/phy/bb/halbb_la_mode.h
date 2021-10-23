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
#ifndef __HALBB_LA_MODE_H__
#define __HALBB_LA_MODE_H__

/*@--------------------------[Define] ---------------------------------------*/
#define LA_CHK_PTRN_NUM	4
/*@--------------------------[Enum]------------------------------------------*/

enum la_mac_polling_state {
	LA_HW_IDLE		= 0,
	LA_HW_START		= 1,
	LA_HW_FINISH_STOP	= 2,
	LA_HW_FINISH_TIMEOUT	= 3,
	LA_HW_RE_START		= 4
};

enum la_mode_state_t {
	LA_STATE_IDLE		= 0,
	LA_STATE_MAIN		= 1,
	LA_STATE_GET_DLE_BUF	= 2,
	LA_STATE_WAIT_RESTART	= 3
};

enum la_buff_mode_t {
	LA_BUFF_64K	= 0,
	LA_BUFF_128K	= 1,
	LA_BUFF_192K	= 2,
	LA_BUFF_256K	= 3,
	LA_BUFF_320K	= 4,
	LA_BUF_DISABLE = 0xff
};

enum la_bb_smp_clk {
	LA_SMP_CLK_80	= 0,
	LA_SMP_CLK_40	= 1,
	LA_SMP_CLK_20	= 2,
	LA_SMP_CLK_10	= 3,
	LA_SMP_CLK_5	= 4,
	LA_SMP_CLK_2_5	= 5,
	LA_SMP_CLK_1_25	= 6,
	LA_SMP_CLK_160	= 7
};

enum la_dma_data_type_t {
	DMA00_NRML_1s_14b	= 0,
	DMA01_NRML_2s_12b	= 1,
	DMA02_NRML_2s_13b	= 2,
	DMA03_NRML_2s_14b	= 3,
	DMA04_NRML_3s_08b	= 4,
	DMA05_NRML_3s_09b	= 5,
	DMA06_NRML_3s_10b	= 6,
	DMA07_NRML_4s_07b	= 7,
	DMA08_NRML_4s_08b	= 8,
	DMA09_DUAL_4s_12b	= 9,
	DMA10_DUAL_4s_13b	= 10,
	DMA11_DUAL_4s_14b	= 11,
	DMA12_MPHS_1s_2p_12b	= 12,
	DMA13_MPHS_1s_3p_10b	= 13,
	DMA14_MPHS_1s_4p_08b	= 14,
	DMA15_MPHS_2s_2p_08b	= 15,
	DMA16_DBG		= 16,
	DMA17_DUAL_WB_1s_14b	= 17,
	DMA18_DUAL_WB_2s_14b	= 18,
	DMA19_DUAL_WB_3s_14b	= 19,
	DMA20_DUAL_WB_4s_14b 	= 20
};

/*@--------------------------[Structure]-------------------------------------*/

struct la_ptrn_chk_info {
	u32	smp_point;
	u32	la_ptrn_chk_mask; /*if mask=0: disable pattern chk, for MSB 32bit only*/
	u32	la_ptrn_chk_val;
};

struct la_print_info {
	bool	is_la_print;
	u8	print_len; /*0: all, 1:partial*/
	u8	print_mode; /*0: hex, 1:unsign, 2:sign*/
	u8	print_lsb;
	u8	print_msb;
};

struct la_dma_info {
	u8 dma_dbgport_base_n;
	u8 dma_a_path_sel;
	u8 dma_b_path_sel;
	u8 dma_c_path_sel;
	u8 dma_d_path_sel;
	u8 dma_a_src_sel;
	u8 dma_b_src_sel;
	u8 dma_c_src_sel;
	u8 dma_d_src_sel;
	u8 dma_hdr_sel_63;
	u8 dma_hdr_sel_62;
	u8 dma_hdr_sel_61;
	u8 dma_hdr_sel_60;
	bool dma_a_ck160_dly_en;
	bool dma_b_ck160_dly_en;
	bool dma_c_ck160_dly_en;
	bool dma_d_ck160_dly_en;
	bool dma_dbgport_phy_sel;
	enum la_dma_data_type_t dma_data_type;
};

struct la_string_info {
	u32			*octet;
	u32			length;
	u32			buffer_size;	/*Byte*/
	u32			start_pos;
	u32			end_pos;	/*buf addr*/
	u32			smp_number_max; /*number of LA sample*/
};

struct la_re_trig_info {
	bool			re_trig_en;
	u16			re_trig_wait_cnt;
	/*Re-trig*/
	bool			la_re_trig_edge;
	u8			la_re_and0_sel;
	u8			la_re_and0_val;
	bool			la_re_and0_inv;
};

struct la_adv_trig_info {
	bool			adv_trig_en;
	/*AND1*/
	u32			la_and1_mask; /*sel all 0 = disable*/
	u32			la_and1_val;
	bool			la_and1_inv;
	/*AND2*/
	bool			la_and2_en;
	bool			la_and2_inv;
	u32			la_and2_val;
	u32			la_and2_mask;
	u8			la_and2_sign; /*0: unsigned, 1:signed, 2:norm*/
	/*AND3*/
	bool			la_and3_en;
	bool			la_and3_inv;
	u32			la_and3_val;
	u32			la_and3_mask;
	u8			la_and3_sign; /*0: unsigned, 1:signed, 2:norm*/
	/*AND4*/
	bool			la_and4_en;
	u16			la_and4_rate; /*rate_idx*/
	bool			la_and4_inv;
	/*AND5*/
	u8			la_and5_sel;
	bool			la_and5_inv;
	u8			la_and5_val;
	/*AND6*/
	u8			la_and6_sel;
	bool			la_and6_inv;
	u8			la_and6_val;
	/*AND7*/
	u8			la_and7_sel;
	bool			la_and7_inv;
	u8			la_and7_val;
};

struct la_trig_mac_info {
	bool	la_mac_trig_en;  /*sw tag*/
	bool	la_mac_and0_en;
	u8	la_mac_and0_sel; /*0~2: cca, crc_er, crc_ok*/
	u8	la_mac_and0_mac_sel; /*0: true mac, 1: pmac*/
	bool	la_mac_and1_en;
	bool	la_mac_and2_en;
	u8	la_mac_and2_frame_sel; /*6-bit mac hdr*/
};

struct la_mac_cfg_info {
	enum la_buff_mode_t mac_la_buf_sel;
	bool mac_alloc_success;
	u32 la_trigger_time; /*mu sec*/
	u8 mac_la_en;
	u8 mac_la_restart_en;
	u8 mac_la_timeout_en;
	u8 mac_la_data_loss_imr;/*Error flag mask bit for LA data loss due to pktbuffer busy */
	u8 mac_la_timeout_val;	/*0:1s, 1:2s, 2:4s, 3:8s*/
};

struct bb_la_cr_info {
	u32 la_clk_en;
	u32 la_clk_en_m;
	u32 la_en;
	u32 la_en_m;
	u32 dma_dbgport_base_n;
	u32 dma_dbgport_base_n_m;
	u32 dma_a_path_sel;
	u32 dma_a_path_sel_m;
	u32 dma_b_path_sel;
	u32 dma_b_path_sel_m;
	u32 dma_c_path_sel;
	u32 dma_c_path_sel_m;
	u32 dma_d_path_sel;
	u32 dma_d_path_sel_m;
	u32 dma_a_src_sel;
	u32 dma_a_src_sel_m;
	u32 dma_b_src_sel;
	u32 dma_b_src_sel_m;
	u32 dma_c_src_sel;
	u32 dma_c_src_sel_m;
	u32 dma_d_src_sel;
	u32 dma_d_src_sel_m;
	u32 la_smp_rate;
	u32 la_smp_rate_m;
	u32 rdrdy_3_phase_en;
	u32 rdrdy_3_phase_en_m;
	u32 la_trigger_edge;
	u32 la_trigger_edge_m;
	u32 dma_hdr_sel_63;
	u32 dma_hdr_sel_63_m;
	u32 dma_hdr_sel_62;
	u32 dma_hdr_sel_62_m;
	u32 dma_hdr_sel_61;
	u32 dma_hdr_sel_61_m;
	u32 dma_hdr_sel_60;
	u32 dma_hdr_sel_60_m;
	u32 dma_a_ck160_dly_en;
	u32 dma_a_ck160_dly_en_m;
	u32 dma_b_ck160_dly_en;
	u32 dma_b_ck160_dly_en_m;
	u32 dma_dbgport_phy_sel;
	u32 dma_dbgport_phy_sel_m;
	u32 dma_data_type;
	u32 dma_data_type_m;
	u32 r_dma_rdrdy;
	u32 r_dma_rdrdy_m;
	u32 la_and0_bit_sel;
	u32 la_and0_bit_sel_m;
	u32 la_trigger_cnt;
	u32 la_trigger_cnt_m;
	u32 and0_trig_disable;
	u32 and0_trig_disable_m;
	u32 la_and1_inv;
	u32 la_and1_inv_m;
	u32 la_and2_en;
	u32 la_and2_en_m;
	u32 la_and2_inv;
	u32 la_and2_inv_m;
	u32 la_and3_en;
	u32 la_and3_en_m;
	u32 la_and3_inv;
	u32 la_and3_inv_m;
	u32 la_and4_en;
	u32 la_and4_en_m;
	u32 la_and4_rate;
	u32 la_and4_rate_m;
	u32 la_and4_inv;
	u32 la_and4_inv_m;
	u32 la_and1_mask;
	u32 la_and1_mask_m;
	u32 la_and1_val;
	u32 la_and1_val_m;
	u32 la_and2_mask;
	u32 la_and2_mask_m;
	u32 la_and2_val;
	u32 la_and2_val_m;
	u32 la_and3_mask;
	u32 la_and3_mask_m;
	u32 la_and3_val;
	u32 la_and3_val_m;
	u32 la_and5_sel;
	u32 la_and5_sel_m;
	u32 la_and5_val;
	u32 la_and5_val_m;
	u32 la_and5_inv;
	u32 la_and5_inv_m;
	u32 la_and6_sel;
	u32 la_and6_sel_m;
	u32 la_and6_val;
	u32 la_and6_val_m;
	u32 la_and6_inv;
	u32 la_and6_inv_m;
	u32 la_and7_sel;
	u32 la_and7_sel_m;
	u32 la_and7_val;
	u32 la_and7_val_m;
	u32 la_and7_inv;
	u32 la_and7_inv_m;
	u32 la_brk_sel;
	u32 la_brk_sel_m;
	u32 la_mac_and1_en;
	u32 la_mac_and1_en_m;
	u32 la_mac_and2_en;
	u32 la_mac_and2_en_m;
	u32 la_mac_and0_sel;
	u32 la_mac_and0_sel_m;
	u32 la_mac_and0_en;
	u32 la_mac_and0_en_m;
	u32 la_mac_and0_mac_sel;
	u32 la_mac_and0_mac_sel_m;
	u32 la_and2_sign;
	u32 la_and2_sign_m;
	u32 la_and3_sign;
	u32 la_and3_sign_m;
	u32 la_re_trig_edge;
	u32 la_re_trig_edge_m;
	u32 la_re_and1_sel;
	u32 la_re_and1_sel_m;
	u32 la_re_and1_val;
	u32 la_re_and1_val_m;
	u32 la_re_and1_inv;
	u32 la_re_and1_inv_m;
};

struct bb_la_mode_info {
	struct bb_la_cr_info	bb_la_cr_i;
	struct la_string_info	la_string_i;
	enum la_mode_state_t	la_mode_state;
	u32			la_dbg_port;
	u32			la_count;
	u32			smp_number;
	u32			txff_page;
	bool			not_stop_trig; /*set impossible trigger condition*/
	u8			la_basic_mode_sel; /*0:bb_mode, mac mode*/
	/*[General setting]*/
	u8			la_polling_cnt;
	u8			la_trigger_cnt;
	u8			la_trigger_edge; /*0: p-edge, 1: n-edge*/
	enum la_bb_smp_clk	la_smp_rate;
	/*[AND-0 sel]*/
	bool			la_and0_disable;
	u32			la_and0_bit_sel; /*And0 trigger bit sel*/
	struct la_dma_info	la_dma_i;
	struct la_re_trig_info	la_re_trig_i;
	struct la_adv_trig_info	adv_trig_i;
	struct la_trig_mac_info	la_trig_mac_i;
	struct la_mac_cfg_info	la_mac_cfg_i; /*MAC CR Control*/
	struct la_print_info	la_print_i;
	struct la_ptrn_chk_info la_ptrn_chk_i[LA_CHK_PTRN_NUM];
};

struct bb_info;
/*@--------------------------[Prptotype]-------------------------------------*/
void halbb_la_re_trig_watchdog(struct bb_info *bb);
void halbb_la_run(struct bb_info *bb);
void halbb_la_deinit(struct bb_info *bb);
void halbb_la_init(struct bb_info *bb);
void halbb_cr_cfg_la_init(struct bb_info *bb);
void halbb_la_cmd_dbg(struct bb_info *bb, char input[][16], u32 *_used, char *output,
		  u32 *_out_len);
#endif
