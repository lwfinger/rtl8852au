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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
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

/*Image2HeaderVersion: R3 2.0.1.2*/
#ifndef _HALBB_HWIMG_RAW_DATA_8852A_H
#define _HALBB_HWIMG_RAW_DATA_8852A_H


/******************************************************************************
 *                           phy_reg.TXT
 ******************************************************************************/

const u32 array_mp_8852a_phy_reg[] = {
		0xF0FF0000, 0x00000000,
		0xF03300FF, 0x00000001,
		0xF03500FF, 0x00000002,
		0x804, 0x601E0100,
		0x814, 0x00000000,
		0x818, 0x13332333,
		0x814, 0x00010000,
		0x820, 0x20000000,
		0x1000, 0x00000000,
		0x1004, 0xCA014000,
		0x1008, 0xC751D4F0,
		0x100C, 0x44511475,
		0x1010, 0x00000000,
		0x1014, 0x00000000,
		0x1018, 0x00000000,
		0x101C, 0x00000001,
		0x1020, 0x8C30C30C,
		0x1024, 0x4C30C30C,
		0x1028, 0x0C30C30C,
		0x102C, 0x0C30C30C,
		0x1030, 0x0C30C30C,
		0x1034, 0x0C30C30C,
		0x1038, 0x28A28A28,
		0x103C, 0x28A28A28,
		0x1040, 0x28A28A28,
		0x1044, 0x28A28A28,
		0x1048, 0x28A28A28,
		0x104C, 0x28A28A28,
		0x1050, 0x06666666,
		0x1054, 0x33333333,
		0x1058, 0x33333333,
		0x105C, 0x33333333,
		0x1060, 0x00000031,
		0x1064, 0x5100600A,
		0x1068, 0x18363113,
		0x106C, 0x1D976DDC,
		0x1070, 0x1C072DD7,
		0x1074, 0x1127CDF4,
		0x1078, 0x1E37BDF1,
		0x107C, 0x1FB7F1D6,
		0x1080, 0x1EA7DDF9,
		0x1084, 0x1FE445DD,
		0x1088, 0x1F97F1FE,
		0x108C, 0x1FF781ED,
		0x1090, 0x1FA7F5FE,
		0x1094, 0x1E07B913,
		0x1098, 0x1FD7FDFF,
		0x109C, 0x1E17B9FA,
		0x10A0, 0x19A66914,
		0x10A4, 0x10F65598,
		0x10A8, 0x14A5A111,
		0x10AC, 0x1D3765DB,
		0x10B0, 0x17C685CA,
		0x10B4, 0x1107C5F3,
		0x10B8, 0x1B5785EB,
		0x10BC, 0x1F97ED8F,
		0x10C0, 0x1BC7A5F3,
		0x10C4, 0x1FE43595,
		0x10C8, 0x1EB7D9FC,
		0x10CC, 0x1FE65DBE,
		0x10D0, 0x1EC7D9FC,
		0x10D4, 0x1976FCFF,
		0x10D8, 0x1F77F5FF,
		0x10DC, 0x1976FDEC,
		0x10E0, 0x198664EF,
		0x10E4, 0x11062D93,
		0x10E8, 0x10C4E910,
		0x10EC, 0x1CA759DB,
		0x10F0, 0x1335A9B5,
		0x10F4, 0x1097B9F3,
		0x10F8, 0x17B72DE1,
		0x10FC, 0x1F67ED42,
		0x1100, 0x18074DE9,
		0x1104, 0x1FD40547,
		0x1108, 0x1D57ADF9,
		0x110C, 0x1FE52182,
		0x1110, 0x1D67B1F9,
		0x1114, 0x14860CE1,
		0x1118, 0x1EC7E9FE,
		0x111C, 0x14860DD6,
		0x1120, 0x195664C7,
		0x1124, 0x0005E58A,
		0x1128, 0x00000000,
		0x112C, 0x00000000,
		0x1130, 0x7A000000,
		0x1134, 0x0F9F3D7A,
		0x1138, 0x0040817C,
		0x113C, 0x00E10204,
		0x1140, 0x227D94CD,
		0x1144, 0x084238E3,
		0x1148, 0x00000010,
		0x114C, 0x0011A200,
		0x1150, 0x0060B002,
		0x1154, 0x9A8249A8,
		0x1158, 0x26A1469E,
		0x115C, 0x2099A824,
		0x1160, 0x2359461C,
		0x1164, 0x1631A675,
		0x1168, 0x2C6B1D63,
		0x116C, 0x0000000E,
		0x1170, 0x00000001,
		0x1174, 0x00000001,
		0x1178, 0x00000000,
		0x117C, 0x0000000C,
		0x1180, 0x00000000,
		0x1184, 0x00000000,
		0x1188, 0x0418317C,
		0x118C, 0x00D6135C,
		0x1190, 0x00000000,
		0x1194, 0x00000000,
		0x1198, 0x00000000,
		0x119C, 0x00000000,
		0x11A0, 0x00000000,
		0x11A4, 0x00000000,
		0x11A8, 0x00000000,
		0x11AC, 0x00000000,
		0x11B0, 0x00000000,
		0x11B4, 0xB4026000,
		0x11B8, 0x00000960,
		0x11BC, 0x02024008,
		0x11C0, 0x00000000,
		0x11C4, 0x00000000,
		0x11C8, 0x22CE803C,
		0x11CC, 0x32000000,
		0x11D0, 0xBD67D67D,
		0x11D4, 0x02AAAF59,
		0x11D8, 0x00000000,
		0x11DC, 0x00000000,
		0x11E0, 0x00000004,
		0x11E4, 0x00000001,
		0x11E8, 0x61861800,
		0x11EC, 0x830C30C3,
		0x11F0, 0xC30C30C3,
		0x11F4, 0x830C30C3,
		0x11F8, 0x051450C3,
		0x11FC, 0x05145145,
		0x1200, 0x05145145,
		0x1204, 0x05145145,
		0x1208, 0x0F0C3145,
		0x120C, 0x030C30CF,
		0x1210, 0x030C30C3,
		0x1214, 0x030CF3C3,
		0x1218, 0x030C30C3,
		0x121C, 0x0F3CF3C3,
		0x1220, 0x0F3CF3CF,
		0x1224, 0x0F3CF3CF,
		0x1228, 0x0F3CF3CF,
		0x122C, 0x0F3CF3CF,
		0x1230, 0x030C10C3,
		0x1234, 0x051430C3,
		0x1238, 0x051490CB,
		0x123C, 0x030CD151,
		0x1240, 0x050C50C7,
		0x1244, 0x051492CB,
		0x1248, 0x05145145,
		0x124C, 0x05145145,
		0x1250, 0x05145145,
		0x1254, 0x05145145,
		0x1258, 0x090CD3CF,
		0x125C, 0x071491C5,
		0x1260, 0x073CF143,
		0x1264, 0x071431C3,
		0x1268, 0x0F3CF1C5,
		0x126C, 0x0F3CF3CF,
		0x1270, 0x0F3CF3CF,
		0x1274, 0x0F3CF3CF,
		0x1278, 0x0F3CF3CF,
		0x127C, 0x090C91CF,
		0x1280, 0x11243143,
		0x1284, 0x9777A777,
		0x1288, 0xBB7BAC95,
		0x128C, 0xB667B889,
		0x1290, 0x7B9B8899,
		0x1294, 0x7A5567C8,
		0x1298, 0x2278CCCC,
		0x129C, 0x00037C22,
		0x12A0, 0x001CCCCC,
		0x12A4, 0x00000000,
		0x12A8, 0x00000008,
		0x12AC, 0x00000000,
		0x12B0, 0x00000000,
		0x12B4, 0x00000000,
		0x12B8, 0x10000800,
		0x12BC, 0x00401802,
		0x12C0, 0x00061004,
		0x12C4, 0x000024D8,
		0x12C8, 0x10000020,
		0x12CC, 0x20000200,
		0x12D0, 0x00000000,
		0x12D4, 0x04000000,
		0x12D8, 0x44000100,
		0x12DC, 0x60804060,
		0x12E0, 0x44204210,
		0x12E4, 0x82108082,
		0x12E8, 0x82108402,
		0x12EC, 0xC8082108,
		0x12F0, 0x48202084,
		0x12F4, 0x44208208,
		0x12F8, 0x84108204,
		0x12FC, 0xD0108104,
		0x1300, 0xF8210108,
		0x1304, 0x6431E930,
		0x1308, 0x02109468,
		0x130C, 0x10C61C22,
		0x1310, 0x02109469,
		0x1314, 0x10C61C22,
		0x1318, 0x00041049,
		0x131C, 0x00000000,
		0x1320, 0x00000000,
		0x1324, 0xA0000000,
		0x1328, 0x04204000,
		0x132C, 0x00000000,
		0x1330, 0x00000000,
		0x1334, 0x00000000,
		0x1338, 0x00000000,
		0x133C, 0x25F642F8,
		0x1340, 0xA80668A0,
		0x1344, 0x60900820,
		0x1348, 0x9F08518C,
		0x134C, 0x32488A62,
		0x1350, 0x9C6E36DC,
		0x1354, 0x0000B53B,
		0x1358, 0x00000000,
		0x135C, 0x0801442E,
		0x1360, 0x000120B8,
		0x1364, 0x00000000,
		0x1368, 0x00000000,
		0x136C, 0x00000000,
		0x1370, 0x00000000,
		0x1374, 0x00000000,
		0x1378, 0x2A0A6040,
		0x137C, 0x0A0A6829,
		0x1380, 0x00000004,
		0x1384, 0x00000000,
		0x1388, 0x80000000,
		0x138C, 0x10000000,
		0x1390, 0xA0000000,
		0x1394, 0x0000001E,
		0x1398, 0x00009397,
		0x139C, 0x00000001,
		0x13A0, 0x00000001,
		0x13A4, 0x00000000,
		0x13A8, 0x00000000,
		0x13AC, 0x00000000,
		0x13B0, 0x00000000,
		0x13B4, 0x00000000,
		0x13B8, 0x00000000,
		0x13BC, 0x00000000,
		0x13C0, 0x00000000,
		0x13C4, 0x00000000,
		0x13C8, 0x00000000,
		0x13CC, 0x00000000,
		0x13D0, 0x00000000,
		0x13D4, 0x00000000,
		0x13D8, 0x00000000,
		0x13DC, 0x00000000,
		0x13E0, 0x00000000,
		0x13E4, 0x00000000,
		0x13E8, 0x00000000,
		0x13EC, 0x00000000,
		0x13F0, 0x00000000,
		0x13F4, 0x00000000,
		0x13F8, 0x00000000,
		0x13FC, 0x00000000,
		0x1400, 0x00000000,
		0x1404, 0x00000000,
		0x1408, 0x00000000,
		0x140C, 0x00000000,
		0x1410, 0x00000000,
		0x1414, 0x00000000,
		0x1418, 0x00000000,
		0x141C, 0x00000000,
		0x1420, 0x00000000,
		0x1424, 0x4EA20631,
		0x1428, 0x000005C8,
		0x142C, 0x000000FF,
		0x1430, 0x00000000,
		0x1434, 0x00000000,
		0x1438, 0x00000000,
		0x143C, 0x00000000,
		0x1440, 0x00000000,
		0x1444, 0x00000000,
		0x1448, 0x00000000,
		0x144C, 0x00000000,
		0x1450, 0x00000000,
		0x1454, 0x4060001A,
		0x1458, 0x40000000,
		0x145C, 0x00000000,
		0x1460, 0x20000000,
		0x1464, 0x00800406,
		0x1468, 0x00022270,
		0x146C, 0x0002024B,
		0x1470, 0x00009B40,
		0x1474, 0x00000000,
		0x1478, 0x00000063,
		0x147C, 0x30000000,
		0x1480, 0x00000001,
		0x1484, 0x05000000,
		0x1488, 0xE0000800,
		0x148C, 0x03FC0000,
		0x1490, 0x00000001,
		0x1494, 0x00000000,
		0x1498, 0x00000700,
		0x149C, 0x00500000,
		0x14A0, 0xC0000048,
		0x14A4, 0x1C909200,
		0x14A8, 0x00000010,
		0x14AC, 0x00000000,
		0x14B0, 0x00F20006,
		0x14B4, 0x000000E9,
		0x14B8, 0x00000000,
		0x14BC, 0x00000000,
		0x14C0, 0x000CE000,
		0x14C4, 0x0007E0AB,
		0x14C8, 0x00024051,
		0x14CC, 0x00000012,
		0x14D0, 0x00000000,
		0x14D4, 0x1212100B,
		0x14D8, 0x00000006,
		0x14DC, 0x00000000,
		0x14E0, 0x00000872,
		0x14E4, 0x00000003,
		0x14E8, 0x000003D9,
		0x18D0, 0x00000000,
		0x18D4, 0x00000001,
		0x14EC, 0x00000000,
		0x14F0, 0x00000001,
		0x14F4, 0x25294992,
		0x14F8, 0x1CE62A52,
		0x14FC, 0x1CF739CE,
		0x1500, 0x5CE739CE,
		0x1504, 0x0002908E,
		0x1508, 0x00000001,
		0x150C, 0x00000077,
		0x1510, 0x00000028,
		0x1514, 0x00000004,
		0x1518, 0x00000000,
		0x151C, 0x00000000,
		0x1520, 0x00E2E100,
		0x1524, 0xCB00B6B6,
		0x1528, 0x59100FCA,
		0x152C, 0x08882550,
		0x1530, 0x08CC2660,
		0x1534, 0x09102660,
		0x1538, 0x00000154,
		0x153C, 0x31BF0400,
		0x1540, 0x2C4F346D,
		0x1544, 0x3D7C6D6D,
		0x1548, 0x009CFDF0,
		0x154C, 0xE2003F80,
		0x1550, 0x02CA8D00,
		0x1554, 0x41D7F306,
		0x1558, 0x75DC3DBA,
		0x155C, 0xB136EECE,
		0x1560, 0xBFE341BB,
		0x1564, 0x78910FB6,
		0x1568, 0xB7C5FEF8,
		0x156C, 0x2A72ACD7,
		0x1570, 0xC2544FB2,
		0x1574, 0x4C823404,
		0x1578, 0x9084C800,
		0x157C, 0x9889314F,
		0x1580, 0x5ECC7FF4,
		0x1584, 0x6388ECAE,
		0x1588, 0xF8434706,
		0x158C, 0x6505D59A,
		0x1590, 0x5B6D6494,
	0x80ff0000,	0x00000000,	0x40000000,	0x00000000,
		0x1594, 0x5CE6C536,
	0x903300ff,	0x00000000,	0x40000000,	0x00000000,
		0x1594, 0x5CE6C576,
	0x903500ff,	0x00000000,	0x40000000,	0x00000000,
		0x1594, 0x5CE6C576,
	0xA0000000,	0x00000000,
		0x1594, 0x5CE6C536,
	0xB0000000,	0x00000000,
		0x1598, 0x05967F3D,
		0x18A8, 0xF9D7BA9F,
		0x18AC, 0x4F563411,
		0x18B0, 0x00000067,
		0x18C0, 0x00000009,
		0x18C8, 0x00000008,
		0x159C, 0x00000000,
		0x15A0, 0x00000000,
		0x15A4, 0x00000219,
		0x15A8, 0x00000000,
		0x15AC, 0x00000000,
		0x15B0, 0x00000001,
		0x15B4, 0x00000001,
		0x15B8, 0x00000000,
		0x15BC, 0x00000000,
		0x15C0, 0x00000151,
		0x15C4, 0x00000498,
		0x15C8, 0x00000498,
		0x15CC, 0x00000000,
		0x15D0, 0x00000000,
		0x15D4, 0x00001146,
		0x15D8, 0x00000000,
		0x15DC, 0x00000000,
		0x15E0, 0x00E2E100,
		0x15E4, 0xCB00B6B6,
		0x15E8, 0x59100FCA,
		0x15EC, 0x08882550,
		0x15F0, 0x08CC2660,
		0x15F4, 0x09102660,
		0x15F8, 0x00000154,
		0x15FC, 0x31BF0400,
		0x1600, 0x2C4F346D,
		0x1604, 0x3D7C6D6D,
		0x1608, 0x009CFDF0,
		0x160C, 0xE2003F80,
		0x1610, 0x02CA8D00,
		0x1614, 0x41CBE306,
		0x1618, 0x73DC3D79,
		0x161C, 0xB136DECE,
		0x1620, 0xBFD731B9,
		0x1624, 0x78910F75,
		0x1628, 0xB7C5FEF8,
		0x162C, 0x2A72ACD7,
		0x1630, 0xC2544FB2,
		0x1634, 0x4C823404,
		0x1638, 0x9084C800,
		0x163C, 0x9889314F,
		0x1640, 0x5ECC7FF4,
		0x1644, 0x6388ECAE,
		0x1648, 0x78434706,
		0x164C, 0x6505D1AA,
		0x1650, 0x5B6D6494,
	0x80ff0000,	0x00000000,	0x40000000,	0x00000000,
		0x1654, 0x5CE6C536,
	0x903300ff,	0x00000000,	0x40000000,	0x00000000,
		0x1654, 0x5CE6C576,
	0x903500ff,	0x00000000,	0x40000000,	0x00000000,
		0x1654, 0x5CE6C576,
	0xA0000000,	0x00000000,
		0x1654, 0x5CE6C536,
	0xB0000000,	0x00000000,
		0x1658, 0x05967F3D,
		0x18B4, 0xF9D7B99E,
		0x18B8, 0x4F553311,
		0x18BC, 0x00000067,
		0x18C4, 0x00000009,
		0x18CC, 0x00000008,
		0x165C, 0x00000000,
		0x1660, 0x00000000,
		0x1664, 0x00000219,
		0x1668, 0x00000000,
		0x166C, 0x00000000,
		0x1670, 0x00000001,
		0x1674, 0x00000001,
		0x1678, 0x00000000,
		0x167C, 0x00000000,
		0x1680, 0x00000151,
		0x1684, 0x00000498,
		0x1688, 0x00000498,
		0x168C, 0x00000000,
		0x1690, 0x00000000,
		0x1694, 0x00001147,
		0x1698, 0x632103FE,
		0x169C, 0xB20A7B28,
		0x16A0, 0xC6A7B14F,
		0x16A4, 0x0000002B,
		0x16A8, 0x009B902A,
		0x16AC, 0x009B902A,
		0x16B0, 0x98682C18,
		0x16B4, 0x6308C4C1,
		0x16B8, 0x9248C631,
		0x16BC, 0x922A8153,
		0x16C0, 0x00000005,
		0x16C4, 0x00001759,
		0x16C8, 0x4B802000,
		0x16CC, 0x831408BE,
		0x16D0, 0x9ABBCACB,
		0x16D4, 0x56767578,
		0x16D8, 0xBBCCBBB3,
		0x16DC, 0x57887789,
		0x16E0, 0x00000F45,
		0x16E4, 0x27039CE9,
		0x16E8, 0x31413432,
		0x16EC, 0x62658342,
		0x16F0, 0x00000005,
		0x16F4, 0x00000005,
		0x16F8, 0xC7013016,
		0x16FC, 0x84413016,
		0x1700, 0x84413016,
		0x1704, 0x8C413016,
		0x1708, 0x8C40B028,
		0x170C, 0x3140B028,
		0x1710, 0x2940B028,
		0x1714, 0x8440B028,
		0x1718, 0x2318C610,
		0x171C, 0x45336753,
		0x1720, 0x236A7A88,
		0x1724, 0x576DF814,
		0x1728, 0xA08877AC,
		0x172C, 0x0000087A,
		0x1730, 0xBCEB4A14,
		0x1734, 0x000A3A4A,
		0x1738, 0xBCEB4A14,
		0x173C, 0x000A3A4A,
		0x1740, 0xBCBDAC97,
		0x1744, 0x0CABB99A,
		0x1748, 0x38384242,
		0x174C, 0x0086402E,
		0x1750, 0x06E066AA,
		0x1754, 0x00008A62,
		0x1758, 0x00000008,
		0x175C, 0x009B902A,
		0x1760, 0x009B902A,
		0x1764, 0x98682C18,
		0x1768, 0x6308C4C1,
		0x176C, 0x6248C631,
		0x1770, 0x922A8253,
		0x1774, 0x00000005,
		0x1778, 0x00001759,
		0x177C, 0x4B802000,
		0x1780, 0x831408BE,
		0x1784, 0x9898A8BB,
		0x1788, 0x54535368,
		0x178C, 0x999999B3,
		0x1790, 0x35555589,
		0x1794, 0x00000745,
		0x1798, 0x27039CE9,
		0x179C, 0x31413432,
		0x17A0, 0x62658342,
		0x17A4, 0x00000005,
		0x17A8, 0x00000005,
		0x17AC, 0xC7013016,
		0x17B0, 0x84413016,
		0x17B4, 0x84413016,
		0x17B8, 0x8C413016,
		0x17BC, 0x8C40B028,
		0x17C0, 0x3140B028,
		0x17C4, 0x2940B028,
		0x17C8, 0x8440B028,
		0x17CC, 0x2318C610,
		0x17D0, 0x45334753,
		0x17D4, 0x236A6A88,
		0x17D8, 0x576DF814,
		0x17DC, 0xA08877AC,
		0x17E0, 0x0000007A,
		0x17E4, 0xBCEB4A14,
		0x17E8, 0x000A3A4A,
		0x17EC, 0xBCEB4A14,
		0x17F0, 0x000A3A4A,
		0x17F4, 0x9A8A8A97,
		0x17F8, 0x0CA3B99A,
		0x17FC, 0x38384242,
		0x1800, 0x0086402E,
		0x1804, 0x06E066AA,
		0x1808, 0x00008A62,
		0x180C, 0x00000008,
		0x1810, 0x80040000,
		0x1814, 0x80040000,
		0x1818, 0xFE800000,
		0x181C, 0x834C0000,
		0x1820, 0x00000000,
		0x1824, 0x00000000,
		0x1828, 0x00000000,
		0x182C, 0x00000000,
		0x1830, 0x00000000,
		0x1834, 0x00000000,
		0x1838, 0x00000000,
		0x183C, 0x00000000,
		0x1840, 0x00000000,
		0x1844, 0x00000000,
		0x1848, 0x00000000,
		0x184C, 0x04065800,
		0x1850, 0x32004080,
		0x1854, 0x0E1E3E05,
		0x1858, 0x0A163068,
		0x185C, 0x00206040,
		0x1860, 0x02020202,
		0x1864, 0x00A16020,
		0x1868, 0x031F4284,
		0x186C, 0x00A10285,
		0x1870, 0x00000005,
		0x1874, 0x00000000,
		0x1878, 0x800CD62D,
		0x187C, 0x00000103,
		0x1880, 0x00000000,
		0x1884, 0x00000000,
		0x1888, 0x00000000,
		0x188C, 0x00000000,
		0x1890, 0x00000000,
		0x1894, 0x00000000,
		0x1898, 0x00000000,
		0x189C, 0x00000000,
		0x18A0, 0x00000000,
		0x18A4, 0x00000000,
		0xD80, 0x10002250,
		0xD94, 0x00000010,
		0x1454, 0x4060001A,
		0x244, 0x2314283C,
		0x244, 0x2323193C,
		0xD94, 0x00000010,
		0xA3C, 0x2840E1BF,
		0xA40, 0x00000000,
		0xA44, 0x00000007,
		0xA48, 0x410E4000,
		0xA54, 0x1001436E,
		0xA58, 0x41000000,
		0x830, 0x00000002,
		0xA60, 0x017FFFF2,
		0xA64, 0x0010A130,
		0xA64, 0x0010A130,
		0xA68, 0x100000FF,
		0xA64, 0x0010A130,
		0xA54, 0x1FE14372,
		0xA6C, 0x00000020,
		0x808, 0x00000000,
		0xA6C, 0x00000020,
		0xF84, 0x0043F01D,
		0x4CD0, 0x00000000,
		0x4CEC, 0x888CA72B,
		0x4DD0, 0x00000000,
		0x4DEC, 0x888CA72B,
		0x4540, 0xF64FA0F7,
		0x4544, 0x0410063F,
		0x4548, 0x0000FF7F,
		0x454C, 0x00000000,
		0x4550, 0xF64FA0F7,
		0x4554, 0x04100437,
		0x4558, 0x0000FF7F,
		0x455C, 0x00000000,
		0x4560, 0x00000000,
		0x4564, 0x00000000,
		0x4570, 0x0000001F,
		0x4590, 0x000003FF,
		0x4594, 0x00000000,
		0x4598, 0x0000003F,
		0x459C, 0x00000000,
		0x45A0, 0x000003FF,
		0x45A4, 0x00000000,
		0x45A8, 0x0000003F,
		0x45AC, 0x00000000,
		0x4500, 0x77777777,
		0x4504, 0xBBBBBBBB,
		0x4508, 0xBBBBBBBB,
		0x450C, 0x00000070,
		0x4510, 0x20110900,
		0x4510, 0x20110FFF,
		0x457C, 0x001D050C,
		0x4584, 0x00000207,
		0x4518, 0x50209D00,
		0x4580, 0x00804100,
		0x814, 0x0001004F,
		0x804, 0x601E00FD,
		0x810, 0xF3810000,
		0x000, 0x0580801F,
		0x000, 0x8580801F,
		0xB34, 0xFFFFFFFF,
		0xB3C, 0x55000000,
		0xB40, 0x00005555,
		0x824, 0x00111201,
		0x7C68, 0xA9550000,
		0x7C70, 0x33221100,
		0x7C74, 0x77665544,
		0x7C78, 0xBBAA9988,
		0x7C7C, 0xFFEEDDCC,
		0x7C80, 0x76543210,
		0x7C84, 0xFEDCBA98,
		0x7C88, 0x00000000,
		0x7C8C, 0x00000000,
		0x7C94, 0x00000008,
		0x7D68, 0xA9550000,
		0x7D70, 0x33221100,
		0x7D74, 0x77665544,
		0x7D78, 0xBBAA9988,
		0x7D7C, 0xFFEEDDCC,
		0x7D80, 0x76543210,
		0x7D84, 0xFEDCBA98,
		0x7D88, 0x00000000,
		0x7D8C, 0x00000000,
		0x7D94, 0x00000008,
		0xE0C, 0x00000003,
		0xA70, 0x00000400,
		0x800, 0x00000030,
		0x804, 0x601E00FF,
		0x804, 0x601E00FD,
		0x804, 0x601E00FF,
		0x7C6C, 0x000000F0,
		0x7C6C, 0x000000E0,
		0x7C6C, 0x000000D0,
		0x7C6C, 0x000000C0,
		0x7C6C, 0x000000B0,
		0x7C6C, 0x000000A0,
		0x7C6C, 0x00000090,
		0x7C6C, 0x00000080,
		0x7C6C, 0x00000070,
		0x7C6C, 0x00000060,
		0x7C6C, 0x00000050,
		0x7C6C, 0x00000040,
		0x7C6C, 0x00000030,
		0x7C6C, 0x00000020,
		0x7C6C, 0x00000010,
	0x80ff0000,	0x00000000,	0x40000000,	0x00000000,
		0x7C6C, 0x00000000,
	0x903300ff,	0x00000000,	0x40000000,	0x00000000,
		0x7C6C, 0x00D00000,
	0x903500ff,	0x00000000,	0x40000000,	0x00000000,
		0x7C6C, 0x00D00000,
	0xA0000000,	0x00000000,
		0x7C6C, 0x00000000,
	0xB0000000,	0x00000000,
		0x7D6C, 0x000000F0,
		0x7D6C, 0x000000E0,
		0x7D6C, 0x000000D0,
		0x7D6C, 0x000000C0,
		0x7D6C, 0x000000B0,
		0x7D6C, 0x000000A0,
		0x7D6C, 0x00000090,
		0x7D6C, 0x00000080,
		0x7D6C, 0x00000070,
		0x7D6C, 0x00000060,
		0x7D6C, 0x00000050,
		0x7D6C, 0x00000040,
		0x7D6C, 0x00000030,
		0x7D6C, 0x00000020,
		0x7D6C, 0x00000010,
	0x80ff0000,	0x00000000,	0x40000000,	0x00000000,
		0x7D6C, 0x00000000,
	0x903300ff,	0x00000000,	0x40000000,	0x00000000,
		0x7D6C, 0x00D00000,
	0x903500ff,	0x00000000,	0x40000000,	0x00000000,
		0x7D6C, 0x00D00000,
	0xA0000000,	0x00000000,
		0x7D6C, 0x00000000,
	0xB0000000,	0x00000000,
		0x7C64, 0x080801FF,
		0x7D64, 0x080801FF,
		0xA60, 0x017FFFF3,
		0xA6C, 0x00000021,
		0x7CAC, 0x08000000,
		0x7DAC, 0x08000000,
		0x7C64, 0x180801FF,
		0x7D64, 0x180801FF,
		0xA60, 0x017FFFF3,
		0xA60, 0x017FFFFB,
		0xAE0, 0x013FFF0A,
		0xA70, 0x00000600,
		0xA70, 0x00000660,
		0xA6C, 0x00001021,
		0x7CAC, 0x08000000,
		0x7DAC, 0x08000000,
		0x7C64, 0x100801FF,
		0x7D64, 0x100801FF,
		0x7C64, 0x000801FF,
		0x7D64, 0x000801FF,
		0x804, 0x601C01FF,
		0x7CD4, 0x3401FE00,
		0x7DD4, 0x3401FE00,
		0x7CF0, 0x000401FF,
		0x7DF0, 0x000401FF,
		0x7CF0, 0x400401FF,
		0x7DF0, 0x400401FF,
		0x4CA8, 0x333378A5,
		0x4DA8, 0x333378A5,
		0x7800, 0x02748790,
		0x7804, 0x00558670,
		0x7808, 0x002883F0,
		0x780C, 0x00090120,
		0x7810, 0x00000000,
		0x7814, 0x06000000,
		0x7818, 0x00000000,
		0x781C, 0x00000000,
		0x7820, 0x03020100,
		0x7824, 0x07060504,
		0x7828, 0x0B0A0908,
		0x782C, 0x0F0E0D0C,
		0x7830, 0x13121110,
		0x7834, 0x17161514,
		0x7838, 0x0C700022,
		0x783C, 0x0A05298F,
		0x7840, 0x0005298E,
		0x7844, 0x0006318A,
		0x7848, 0xA006318A,
		0x784C, 0x80039CE7,
		0x7850, 0x80039CE7,
		0x7854, 0x0005298F,
		0x7858, 0x0015296E,
		0x785C, 0x0C07FC31,
		0x7860, 0x0219A6AE,
		0x7864, 0xE4F624C3,
		0x7868, 0x53626F15,
		0x786C, 0x48000000,
		0x7870, 0x48000000,
		0x7874, 0x034C0000,
		0x7878, 0x202401B5,
		0x787C, 0x00F7000E,
		0x7880, 0x0F0A1111,
		0x7884, 0x30D9000F,
		0x7888, 0x0400EA02,
		0x788C, 0x003CB061,
		0x7890, 0x69C00000,
		0x7894, 0x00000000,
		0x7898, 0x000000F0,
		0x789C, 0x0001FFFF,
		0x78A0, 0x00C80064,
		0x78A4, 0x0190012C,
		0x78A8, 0x001917BE,
		0x78AC, 0x07308A00,
		0x78B0, 0x0001D5B0,
		0x78B4, 0x000285D2,
		0x78B8, 0x00000000,
		0x78BC, 0x00000000,
		0x78C0, 0x00000000,
		0x78C4, 0x00000000,
		0x78C8, 0x00000000,
		0x78CC, 0x00000000,
		0x78D0, 0x00000000,
		0x78D4, 0x00000000,
		0x78D8, 0x00000000,
		0x78DC, 0x00000000,
		0x78E0, 0x00000000,
		0x78E4, 0x00000000,
		0x78E8, 0x00000000,
		0x78EC, 0x00000000,
		0x78F0, 0x00000000,
		0x78F4, 0x00000000,
		0x78F8, 0x00000000,
		0x78FC, 0x00000000,
		0xF84, 0x0043F01D,
		0xB00, 0xF303E313,
		0xB04, 0x0CE31333,
		0xB08, 0x0C0C0C0C,
		0xB0C, 0x0C0C0C0C,
		0xB10, 0x80012000,
		0xB14, 0x0001A000,
		0xB18, 0x20022002,
		0xB1C, 0xE0008001,
		0xB20, 0xE000E000,
		0xB24, 0xE000E000,
		0xB28, 0xE000E000,
		0xB2C, 0xE000E000,
		0x4CBC, 0x10104041,
		0x4CC0, 0x14411111,
		0x4DBC, 0x10104041,
		0x4DC0, 0x14411111,
		0x010, 0x0005FFFF,
		0x204, 0x1E1E1E28,
		0x250, 0x00200888,
		0x220, 0x00141350,
		0xB5C, 0x000004C4,
	0x80ff0000,	0x00000000,	0x40000000,	0x00000000,
		0x7C04, 0x04237040,
		0x7D04, 0x04237040,
		0x7C08, 0x04237040,
		0x7D08, 0x04237040,
	0x903300ff,	0x00000000,	0x40000000,	0x00000000,
		0x7C04, 0x04231040,
		0x7D04, 0x04231040,
		0x7C08, 0x04231040,
		0x7D08, 0x04231040,
	0x903500ff,	0x00000000,	0x40000000,	0x00000000,
		0x7C04, 0x04231040,
		0x7D04, 0x04231040,
		0x7C08, 0x04231040,
		0x7D08, 0x04231040,
	0xA0000000,	0x00000000,
		0x7C04, 0x04237040,
		0x7D04, 0x04237040,
		0x7C08, 0x04237040,
		0x7D08, 0x04237040,
	0xB0000000,	0x00000000,
		0x4CA0, 0x24903156,
		0x4DA0, 0x24903156,
		0x240, 0x0A0A0A1E,
		0x4CB8, 0x30020000,
		0x4CAC, 0x02333121,
		0xDA4, 0x0000001C,
		0x224, 0x02010301,
		0x228, 0x00010202,
		0x7C00, 0x03FF807F,
		0x7D00, 0x03FF807F,
		0x0F0, 0x00000001,
		0x0F4, 0x00000035,
		0x0F8, 0x20200805,

};


#endif	/* _HALBB_HWIMG_RAW_DATA_8852A_H */