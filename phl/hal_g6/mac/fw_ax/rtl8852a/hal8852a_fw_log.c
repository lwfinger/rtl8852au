/******************************************************************************
 *
 * Copyright(c) 2012 - 2020 Realtek Corporation.
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

#include "./hal8852a_fw_log.h"

struct mac_fw_msg fw_log_8852a[] = {
	{MSG_8852A_FIRST, "RESERVED"},
	{MSG_8852A_SAMPLE_NO_PARAMETER, "Sample without parameter."},
	{MSG_8852A_SAMPLE_PARAMETER, "Sample string:%s."},
	{MSG_8852A_SAMPLE_INTEGER, "Sample integer:%d."},
	{MSG_8852A_SAMPLE_INTEGER_X, "Sample integer:0x%x."},
	{MSG_8852A_SAMPLE_INTEGER_2, "Sample integer1:%d, integer2:%d."},
	{MSG_8852A_FCUN_NAME, "%s()"},
	{MSG_8852A_MAIN_1, "FW from host"},
	{MSG_8852A_MAIN_2, "Skip DL FW handler"},
	{MSG_8852A_MAIN_3, "FW from flash"},
	{MSG_8852A_MAIN_4, "Invalid boot mode"},
	{MSG_8852A_FWDLHDL_1, "Invalid boot reason"},
	{MSG_8852A_FWDLHDL_2, "Enter DL FW handler"},
	{MSG_8852A_FWDLHDL_3, "FWHDR H2C done"},
	{MSG_8852A_FLASHBOOT, "Enter flash FW loader"},
	{MSG_8852A_IRAMENTRN, "Load FW RAM code OK"},
	{MSG_8852A_FIRSTH2C_1, "First H2C Enqueue"},
	{MSG_8852A_FIRSTH2C_2, "H2C node malloc fail"},
	{MSG_8852A_FIRSTH2C_3, "H2C node content malloc fail"},
	{MSG_8852A_H2CDEQ_1, "H2C Dequeue"},
	{MSG_8852A_H2CDEQ_2, "H2CPKT enqueue fail"},
	{MSG_8852A_H2CPKT_1, "content = 0x%x  0x%x  0x%x  0x%x, %d %d %d %d, 0x%x %d 0x%x %d"},
	{MSG_WLANDUMP_0, "Tx ok packet cnt: %d\n Tx fail packet cnt: %d\n Txcmd success cnt: %d\n Txcmd abort (MU-RTS/RTS fail) cnt: %d\n Txcmd abort (over SP) cnt: %d\n"},
	{MSG_WLANDUMP_1, "Txcmd sounding abort cnt: %d\n Txcmd abort (pri user fail) cnt: %d\n Txcmd abort ( cca or medium busy)  cnt: %d\n Txcmd RU/MU2SU cnt: %d\n"},
	{MSG_WLANDUMP_2, "chkcmd_sts cnt: %d %d %d %d %d %d\n"},
	{MSG_WLANDUMP_3, "TBD"},
	{MSG_WLANDUMP_4, "DL Txcmd in last TXOP cnt: %d\n UL Txcmd in last TXOP cnt: %d\n Empty DL SS2F report cnt: %d\n Empty UL SS2F report cnt: %d\n DL TxcmdQ empty cnt: %d\n"},
	{MSG_WLANDUMP_5, "Issue DL SU Txcmd cnt: %d\n Issue DL MU Txcmd cnt: %d\n Issue DL RU Txcmd cnt: %d\n Issue UL Txcmd cnt: %d\n"},
	{MSG_WLANDUMP_6, "Decision MU2SU cnt: %d\n Decision RU2SU cnt: %d\n DLDecision Result record: %x\n Decision SU_FORCESU cnt: %d\n Decision MU_FORCEMU cnt: %d\n"}, 
	{MSG_WLANDUMP_7, "Decision SU_FORCEMU_FAIL cnt: %d\n Decision SU_FORCERU_FAIL cnt: %d\n Decision SU_FORCERU_RUARST_RU2SU cnt: %d\n Decision SU_NOT4_USER cnt: %d\n Decision RU_FORCERU_RUSRST_FIXTBL cnt: %d\n"},
	{MSG_WLANDUMP_8, "Decision RU_FORCERU cnt: %d\n Decision SU_WDINFO_USERATE cnt: %d\n Decision SU_PRINULLWD cnt: %d\n ecision MU_BYPASS_MUTPCOMPARE cnt: %d\n Decision SU_MUTXTIME_PASS_MU_NOTSUPPORT cnt: %d\n"},
	{MSG_WLANDUMP_9, "Decision SU_MUTXTIME_FAIL_RU_NOTSUPPORT cnt: %d\n Decision SU_RUARST_RU2SU cnt: %d\n Decision RU_RUARST_FIXTBL cnt: %d\n Decision MU_TPCOMPARE_RST cnt: %d\n Decision RU_TPCOMPARE_RST cnt: %d\n Decision SU_TPCOMPARE_RST cnt: %d\n"},
	{MSG_WLANDUMP_10, "fw txcmdQ[0] cmd num: %d, fw txcmdQ[1] cmd num: %d,fw txcmdQ[2] cmd num: %d,fw txcmdQ[3] cmd num: %d,fw txcmdQ[4] cmd num: %d\n"},
	{MSG_WLANDUMP_11, "fw txcmdQ[5] cmd num: %d, fw txcmdQ[6] cmd num: %d,fw txcmdQ[7] cmd num: %d,fw txcmdQ[8] cmd num: %d,fw txcmdQ[9] cmd num: %d, fw txcmdQ[10] cmd num: %d\n"},
    {MSG_WLANDUMP_12, "TBD"},
	{MSG_WLANDUMP_13, "mbid0~7: %d %d %d %d %d %d %d %d\n"},
	{MSG_WLANDUMP_14, "macid  %d info:\n isHESTA =%d\n AID12 =%d\n DL_BW =%d\n DL_T_PE =%d\n"},
	{MSG_WLANDUMP_15, "TF_MAC_Padding =%d\n force tx su =%d\n force tx mu =%d\n fw force mu2su TH = %d\n force tx ru =%d\n"},
	{MSG_WLANDUMP_16, "txok_pkt =%d\n txfail pkt cnt: %d\n retry_cnt =%d\n mbssid_idx =%d\n ulgroup_bitmap =%d\n"},
	{MSG_WLANDUMP_17, "ul_sw_grp_bitmap =%d\n AMPDU_max_txtime =%d\n mu_doppler_ctrl =%d\n mu_gi_ltf =%d\n mu decision txtime_bypass =%d\n mu decision mutp_bypass =%d\n"},
	{MSG_WLANDUMP_18, "mu_maxinitrate = %x\n mu_maxfinalrate = %x\n mu_mininitrate = %x\n mu_minfinalrate = %x\n mu primary cnt = %d\n"},
	{MSG_WLANDUMP_19, "PwrBit =%d\n TP =%d Mbps\n Max Txlen = %x (unit23 byte)\n Min Txlen = %x (unit23 byte)\n Last Txlen = %x (unit23 byte)\n Last BSRlen = %x (unit256 byte) AC=%d\n"},
	{MSG_WLANDUMP_20, "mu sta %d info\n mu sta macid= %d\n"},
	{MSG_WLANDUMP_21, "mu sta rate array(8bit) 0-4: %x %x %x %x %x"},
	{MSG_WLANDUMP_22, "mu sta rate array(8bit) 5-9: %x %x %x %x %x"},
	{MSG_WLANDUMP_23, "su ok pkt cnt = %d\n su fail pkt cnt = %d\n su_norsp_pktcnt = %d\n SU TOTALPKTNUM 1~16/17~32/33~64/>65 : %d %d %d %d, OK=0: %d\n SU TXCNT 1/<6/<16/>=16 : %d %d %d %d\n"},
	{MSG_WLANDUMP_24, "mu ok pkt cnt = %d\n mu fail pkt cnt = %d\n mu_norsp_pktcnt = %d\n mu2su ok pkt cnt= %d\n mu2su fail pkt cnt= %d\n"},
	{MSG_WLANDUMP_25, "ru ok pkt cnt = %d\n ru fail pkt cnt = %d\n"},
	{MSG_WLANDUMP_26, "MU TOTALPKTNUM  1~16/17~32/33~64/>65 :  %d %d %d %d, OK=0: %d\n MU DIFF <4/<8/<12/>=12 : %d %d %d %d\n MU TXCNT 1/<6/<16/>=16: %d %d %d %d\n"},
	{MSG_WLANDUMP_27, "MU TXCNT 1/<6/<16/>=16: %d %d %d %d\n"},
	{MSG_WLANDUMP_28, "pktmaxtxcnt(su base) [0]= %d [1]= %d [2]= %d \n pktmaxtxcnt(mu base) [0]= %d [1]= %d [2]= %d \n pktmaxtxcnt(mu2nd base) [0]= %d [1]= %d [2]= %d \n"},
	{MSG_WLANDUMP_29, "TWT %d info:\n Txcmd overSP= %d\n Txcmd pri user fail= %d\n DL cmdCnt inSP= %d\n UL cmdCnt inSP= %d\n"},
	{MSG_WLANDUMP_30, "DL cmdCnt all= %d\n UL cmdCnt all= %d\n UL fixmode = %x\n cur cmd num = %x\n SP 1st TF fail cnt = %x\n"},
	{MSG_8852A_LAST, "Please add your log above."},
};

