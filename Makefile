EXTRA_CFLAGS += $(USER_EXTRA_CFLAGS)
EXTRA_CFLAGS += -O1
#EXTRA_CFLAGS += -O3
#EXTRA_CFLAGS += -Wall
#EXTRA_CFLAGS += -Wextra
#EXTRA_CFLAGS += -Werror
#EXTRA_CFLAGS += -pedantic
#EXTRA_CFLAGS += -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

EXTRA_CFLAGS += -Wno-unused-variable
#EXTRA_CFLAGS += -Wno-unused-value
EXTRA_CFLAGS += -Wno-unused-label
#EXTRA_CFLAGS += -Wno-unused-parameter
#EXTRA_CFLAGS += -Wno-unused-function
EXTRA_CFLAGS += -Wno-unused
#EXTRA_CFLAGS += -Wno-uninitialized

GCC_VER_49 := $(shell echo `$(CC) -dumpversion | cut -f1-2 -d.` \>= 4.9 | bc )

EXTRA_CFLAGS += -I$(src)/include

EXTRA_LDFLAGS += --strip-debug

CONFIG_AUTOCFG_CP = n

SUBARCH := $(shell uname -m | sed -e "s/i.86/i386/; s/ppc.*/powerpc/; s/armv.l/arm/; s/aarch64/arm64/;")
ARCH ?= $(SUBARCH)

ifeq ("","$(wildcard MOK.der)")
NO_SKIP_SIGN := y
endif

########################## WIFI IC ############################
CONFIG_RTL8852A = y
CONFIG_RTL8852B = n
CONFIG_RTL8852C = n
######################### Interface ###########################
CONFIG_USB_HCI = y
CONFIG_PCI_HCI = n
CONFIG_SDIO_HCI = n
CONFIG_GSPI_HCI = n
########################## Features ###########################
CONFIG_MP_INCLUDED = n
CONFIG_CONCURRENT_MODE = n
CONFIG_POWER_SAVING = n
CONFIG_POWER_SAVE = n
CONFIG_IPS_MODE = default
CONFIG_LPS_MODE = default
CONFIG_BTC = n
CONFIG_WAPI_SUPPORT = n
CONFIG_EFUSE_CONFIG_FILE = y
CONFIG_EXT_CLK = n
CONFIG_TRAFFIC_PROTECT = n
CONFIG_LOAD_PHY_PARA_FROM_FILE = y
# Remember to set CONFIG_FILE_FWIMG when set CONFIG_FILE_FWIMG to y,
# or driver will fail on ifconfig up because can't find firmware file
CONFIG_FILE_FWIMG = n
CONFIG_TXPWR_BY_RATE = n
CONFIG_TXPWR_BY_RATE_EN = auto
CONFIG_TXPWR_LIMIT = n
CONFIG_TXPWR_LIMIT_EN = auto
CONFIG_RTW_CHPLAN = 0xFF
CONFIG_RTW_ADAPTIVITY_EN = disable
CONFIG_RTW_ADAPTIVITY_MODE = normal
CONFIG_SIGNAL_SCALE_MAPPING = n
CONFIG_80211W = y
CONFIG_REDUCE_TX_CPU_LOADING = n
CONFIG_BR_EXT = y
CONFIG_TDLS = n
CONFIG_WIFI_MONITOR = y
CONFIG_MCC_MODE = n
CONFIG_APPEND_VENDOR_IE_ENABLE = n
CONFIG_RTW_NAPI = y
CONFIG_RTW_GRO = y
CONFIG_RTW_NETIF_SG = y
CONFIG_RTW_IPCAM_APPLICATION = n
CONFIG_ICMP_VOQ = n
CONFIG_IP_R_MONITOR = n #arp VOQ and high rate
# user priority mapping rule : tos, dscp
CONFIG_RTW_UP_MAPPING_RULE = tos

CONFIG_PHL_ARCH = y
CONFIG_FSM = n
CONFIG_CMD_DISP = y

CONFIG_HWSIM = n

CONFIG_PHL_TEST_SUITE = n
CONFIG_WIFI_6 = y

RTW_PHL_RX = y
RTW_PHL_TX = y
RTW_PHL_BCN = y
DIRTY_FOR_WORK = y

CONFIG_DRV_FAKE_AP = n

CONFIG_DBG_AX_CAM = n

USE_TRUE_PHY = y
CONFIG_I386_BUILD_VERIFY = n
CONFIG_RTW_MBO = n
# AMSDU in security network
CONFIG_SEC_AMSDU_MODE = non-spp
########################## Android ###########################
# CONFIG_RTW_ANDROID - 0: no Android, 4/5/6/7/8/9/10 : Android version
CONFIG_RTW_ANDROID = 0

ifeq ($(shell test $(CONFIG_RTW_ANDROID) -gt 0; echo $$?), 0)
EXTRA_CFLAGS += -DCONFIG_RTW_ANDROID=$(CONFIG_RTW_ANDROID)
endif

########################## Debug ###########################
CONFIG_RTW_DEBUG = 0
# default log level is _DRV_WARNING_ = 3,
# please refer to "How_to_set_driver_debug_log_level.doc" to set the available level.
CONFIG_RTW_LOG_LEVEL = 3

# enable /proc/net/rtlxxxx/ debug interfaces
CONFIG_PROC_DEBUG = n

######################## Wake On Lan ##########################
CONFIG_WOWLAN = n
#bit2: deauth, bit1: unicast, bit0: magic pkt.
CONFIG_WAKEUP_TYPE = 0x7
CONFIG_WOW_LPS_MODE = default
#bit0: disBBRF off, #bit1: Wireless remote controller (WRC)
CONFIG_SUSPEND_TYPE = 0
CONFIG_WOW_STA_MIX = n
CONFIG_GPIO_WAKEUP = n
# Please contact with RTK support team first. After getting the agreement from RTK support team, 
# you are just able to modify the CONFIG_WAKEUP_GPIO_IDX with customized requirement.
CONFIG_WAKEUP_GPIO_IDX = default
CONFIG_HIGH_ACTIVE_DEV2HST = n
######### only for USB #########
CONFIG_ONE_PIN_GPIO = n
CONFIG_HIGH_ACTIVE_HST2DEV = n
CONFIG_PNO_SUPPORT = n
CONFIG_PNO_SET_DEBUG = n
CONFIG_AP_WOWLAN = n
######### Notify SDIO Host Keep Power During Syspend ##########
CONFIG_RTW_SDIO_PM_KEEP_POWER = y
###################### MP HW TX MODE FOR VHT #######################
CONFIG_MP_VHT_HW_TX_MODE = n

###################### ROAMING #####################################
CONFIG_LAYER2_ROAMING = y
#bit0: ROAM_ON_EXPIRED, #bit1: ROAM_ON_RESUME, #bit2: ROAM_ACTIVE
CONFIG_ROAMING_FLAG = 0x3

###################### Platform Related #######################
CONFIG_PLATFORM_I386_PC = y
CONFIG_PLATFORM_RTL8198D = n
CONFIG_PLATFORM_ANDROID_X86 = n
CONFIG_PLATFORM_ANDROID_INTEL_X86 = n
CONFIG_PLATFORM_NV_TK1 = n
CONFIG_PLATFORM_NV_TK1_UBUNTU = n
CONFIG_PLATFORM_ARM_SUNxI = n
CONFIG_PLATFORM_RTK1319 = n
CONFIG_PLATFORM_AML_S905 = n

########### CUSTOMER ################################

CONFIG_DRVEXT_MODULE = n

export TopDIR ?= $(shell pwd)

########### COMMON  #################################
ifeq ($(CONFIG_GSPI_HCI), y)
HCI_NAME = gspi
endif

ifeq ($(CONFIG_SDIO_HCI), y)
HCI_NAME = sdio
endif

ifeq ($(CONFIG_USB_HCI), y)
HCI_NAME = usb
endif

ifeq ($(CONFIG_PCI_HCI), y)
HCI_NAME = pci
endif

ifeq ($(CONFIG_HWSIM), y)
	HAL = hal_sim
else
	HAL = phl
endif

ifeq ($(CONFIG_PLATFORM_RTL8198D), y)
DRV_PATH = $(src)
else
DRV_PATH = $(TopDIR)
endif

########### HAL_RTL8852A #################################
ifeq ($(CONFIG_RTL8852A), y)
IC_NAME := rtl8852a
ifeq ($(CONFIG_USB_HCI), y)
MODULE_NAME = 8852au
endif
ifeq ($(CONFIG_PCI_HCI), y)
MODULE_NAME = 8852ae
endif
ifeq ($(CONFIG_SDIO_HCI), y)
MODULE_NAME = 8852as
endif

endif

########### HAL_RTL8852B #################################
ifeq ($(CONFIG_RTL8852B), y)
IC_NAME := rtl8852b
ifeq ($(CONFIG_USB_HCI), y)
MODULE_NAME = 8852bu
endif
ifeq ($(CONFIG_PCI_HCI), y)
MODULE_NAME = 8852be
endif
ifeq ($(CONFIG_SDIO_HCI), y)
MODULE_NAME = 8852bs
endif

endif

########### HAL_RTL8852C #################################
ifeq ($(CONFIG_RTL8852C), y)
IC_NAME := rtl8852c
ifeq ($(CONFIG_USB_HCI), y)
MODULE_NAME = 8852cu
endif
ifeq ($(CONFIG_PCI_HCI), y)
MODULE_NAME = 8852ce
endif
ifeq ($(CONFIG_SDIO_HCI), y)
MODULE_NAME = 8852cs
endif

endif

########### AUTO_CFG  #################################

ifeq ($(CONFIG_AUTOCFG_CP), y)
$(shell cp $(DRV_PATH)/autoconf_$(IC_NAME)_$(HCI_NAME)_linux.h $(DRV_PATH)/include/autoconf.h)
endif

########### END OF PATH  #################################
ifeq ($(CONFIG_MP_INCLUDED), y)
#MODULE_NAME := $(MODULE_NAME)_mp
EXTRA_CFLAGS += -DCONFIG_MP_INCLUDED
CONFIG_PHL_TEST_SUITE = y
endif

ifeq ($(CONFIG_FSM), y)
EXTRA_CFLAGS += -DCONFIG_FSM
endif

ifeq ($(CONFIG_CMD_DISP), y)
EXTRA_CFLAGS += -DCONFIG_CMD_DISP
endif

ifeq ($(CONFIG_PHL_TEST_SUITE), y)
EXTRA_CFLAGS += -DCONFIG_PHL_TEST_SUITE
endif

ifeq ($(CONFIG_CONCURRENT_MODE), y)
EXTRA_CFLAGS += -DCONFIG_CONCURRENT_MODE
endif

ifeq ($(CONFIG_POWER_SAVING), y)
ifneq ($(CONFIG_IPS_MODE), default)
EXTRA_CFLAGS += -DRTW_IPS_MODE=$(CONFIG_IPS_MODE)
endif
ifneq ($(CONFIG_LPS_MODE), default)
EXTRA_CFLAGS += -DRTW_LPS_MODE=$(CONFIG_LPS_MODE)
endif
ifneq ($(CONFIG_WOW_LPS_MODE), default)
EXTRA_CFLAGS += -DRTW_WOW_LPS_MODE=$(CONFIG_WOW_LPS_MODE)
endif
EXTRA_CFLAGS += -DCONFIG_POWER_SAVING
endif
ifeq ($(CONFIG_POWER_SAVE), y)
EXTRA_CFLAGS += -DCONFIG_POWER_SAVE
endif

ifeq ($(CONFIG_BTC), y)
EXTRA_CFLAGS += -DCONFIG_BTC
endif

ifeq ($(CONFIG_WAPI_SUPPORT), y)
EXTRA_CFLAGS += -DCONFIG_WAPI_SUPPORT
endif

ifeq ($(CONFIG_WIFI_6), y)
EXTRA_CFLAGS += -DCONFIG_WIFI_6
endif

ifeq ($(CONFIG_EFUSE_CONFIG_FILE), y)
EXTRA_CFLAGS += -DCONFIG_EFUSE_CONFIG_FILE

#EFUSE_MAP_PATH
USER_EFUSE_MAP_PATH ?=
ifneq ($(USER_EFUSE_MAP_PATH),)
EXTRA_CFLAGS += -DEFUSE_MAP_PATH=\"$(USER_EFUSE_MAP_PATH)\"
else
EXTRA_CFLAGS += -DEFUSE_MAP_PATH=\"/system/etc/wifi/wifi_efuse_$(MODULE_NAME).map\"
endif

#WIFIMAC_PATH
USER_WIFIMAC_PATH ?=
ifneq ($(USER_WIFIMAC_PATH),)
EXTRA_CFLAGS += -DWIFIMAC_PATH=\"$(USER_WIFIMAC_PATH)\"
else
EXTRA_CFLAGS += -DWIFIMAC_PATH=\"/data/wifimac.txt\"
endif

endif

ifeq ($(CONFIG_EXT_CLK), y)
EXTRA_CFLAGS += -DCONFIG_EXT_CLK
endif

ifeq ($(CONFIG_TRAFFIC_PROTECT), y)
EXTRA_CFLAGS += -DCONFIG_TRAFFIC_PROTECT
endif

ifeq ($(CONFIG_LOAD_PHY_PARA_FROM_FILE), y)
EXTRA_CFLAGS += -DCONFIG_LOAD_PHY_PARA_FROM_FILE
#EXTRA_CFLAGS += -DREALTEK_CONFIG_PATH_WITH_IC_NAME_FOLDER
EXTRA_CFLAGS += -DREALTEK_CONFIG_PATH=\"/lib/firmware/\"
endif

ifeq ($(CONFIG_FILE_FWIMG), y)
EXTRA_CFLAGS += -DCONFIG_FILE_FWIMG
# default external firmware path is [CONFIG_FIRMWARE_PATH][ic_name]/[fw_name]
# ex. Take 8852AE as example:
#     normal firmware is [CONFIG_FIRMWARE_PATH]rtl8852ae/rtl8852afw.bin
#     WOW firmware is [CONFIG_FIRMWARE_PATH]rtl8852ae/rtl8852afw_wowlan.bin
EXTRA_CFLAGS += -DCONFIG_FIRMWARE_PATH=\"\"
# EXTRA_CFLAGS += -DCONFIG_FIRMWARE_PATH=\"/lib/firmware/\"
endif

ifeq ($(CONFIG_TXPWR_BY_RATE), n)
EXTRA_CFLAGS += -DCONFIG_TXPWR_BY_RATE=0
else ifeq ($(CONFIG_TXPWR_BY_RATE), y)
EXTRA_CFLAGS += -DCONFIG_TXPWR_BY_RATE=1
endif
ifeq ($(CONFIG_TXPWR_BY_RATE_EN), n)
EXTRA_CFLAGS += -DCONFIG_TXPWR_BY_RATE_EN=0
else ifeq ($(CONFIG_TXPWR_BY_RATE_EN), y)
EXTRA_CFLAGS += -DCONFIG_TXPWR_BY_RATE_EN=1
else ifeq ($(CONFIG_TXPWR_BY_RATE_EN), auto)
EXTRA_CFLAGS += -DCONFIG_TXPWR_BY_RATE_EN=2
endif

ifeq ($(CONFIG_TXPWR_LIMIT), n)
EXTRA_CFLAGS += -DCONFIG_TXPWR_LIMIT=0
else ifeq ($(CONFIG_TXPWR_LIMIT), y)
EXTRA_CFLAGS += -DCONFIG_TXPWR_LIMIT=1
endif
ifeq ($(CONFIG_TXPWR_LIMIT_EN), n)
EXTRA_CFLAGS += -DCONFIG_TXPWR_LIMIT_EN=0
else ifeq ($(CONFIG_TXPWR_LIMIT_EN), y)
EXTRA_CFLAGS += -DCONFIG_TXPWR_LIMIT_EN=1
else ifeq ($(CONFIG_TXPWR_LIMIT_EN), auto)
EXTRA_CFLAGS += -DCONFIG_TXPWR_LIMIT_EN=2
endif

ifneq ($(CONFIG_RTW_CHPLAN), 0xFF)
EXTRA_CFLAGS += -DCONFIG_RTW_CHPLAN=$(CONFIG_RTW_CHPLAN)
endif

ifeq ($(CONFIG_CALIBRATE_TX_POWER_BY_REGULATORY), y)
EXTRA_CFLAGS += -DCONFIG_CALIBRATE_TX_POWER_BY_REGULATORY
endif

ifeq ($(CONFIG_CALIBRATE_TX_POWER_TO_MAX), y)
EXTRA_CFLAGS += -DCONFIG_CALIBRATE_TX_POWER_TO_MAX
endif

ifeq ($(CONFIG_RTW_ADAPTIVITY_EN), disable)
EXTRA_CFLAGS += -DCONFIG_RTW_ADAPTIVITY_EN=0
else ifeq ($(CONFIG_RTW_ADAPTIVITY_EN), enable)
EXTRA_CFLAGS += -DCONFIG_RTW_ADAPTIVITY_EN=1
endif

ifeq ($(CONFIG_RTW_ADAPTIVITY_MODE), normal)
EXTRA_CFLAGS += -DCONFIG_RTW_ADAPTIVITY_MODE=0
else ifeq ($(CONFIG_RTW_ADAPTIVITY_MODE), carrier_sense)
EXTRA_CFLAGS += -DCONFIG_RTW_ADAPTIVITY_MODE=1
endif

ifeq ($(CONFIG_SIGNAL_SCALE_MAPPING), y)
EXTRA_CFLAGS += -DCONFIG_SIGNAL_SCALE_MAPPING
endif

ifeq ($(CONFIG_80211W), y)
EXTRA_CFLAGS += -DCONFIG_IEEE80211W
endif

ifeq ($(CONFIG_WOWLAN), y)
EXTRA_CFLAGS += -DCONFIG_WOWLAN -DRTW_WAKEUP_EVENT=$(CONFIG_WAKEUP_TYPE)
EXTRA_CFLAGS += -DRTW_SUSPEND_TYPE=$(CONFIG_SUSPEND_TYPE)
ifeq ($(CONFIG_WOW_STA_MIX), y)
EXTRA_CFLAGS += -DRTW_WOW_STA_MIX
endif
ifeq ($(CONFIG_SDIO_HCI), y)
EXTRA_CFLAGS += -DCONFIG_RTW_SDIO_PM_KEEP_POWER
endif
endif

ifeq ($(CONFIG_AP_WOWLAN), y)
EXTRA_CFLAGS += -DCONFIG_AP_WOWLAN
ifeq ($(CONFIG_SDIO_HCI), y)
EXTRA_CFLAGS += -DCONFIG_RTW_SDIO_PM_KEEP_POWER
endif
endif

ifeq ($(CONFIG_LAYER2_ROAMING), y)
	EXTRA_CFLAGS += -DCONFIG_LAYER2_ROAMING -DCONFIG_ROAMING_FLAG=$(CONFIG_ROAMING_FLAG)
endif

ifeq ($(CONFIG_PNO_SUPPORT), y)
EXTRA_CFLAGS += -DCONFIG_PNO_SUPPORT
ifeq ($(CONFIG_PNO_SET_DEBUG), y)
EXTRA_CFLAGS += -DCONFIG_PNO_SET_DEBUG
endif
endif

ifeq ($(CONFIG_GPIO_WAKEUP), y)
EXTRA_CFLAGS += -DCONFIG_GPIO_WAKEUP
ifeq ($(CONFIG_ONE_PIN_GPIO), y)
EXTRA_CFLAGS += -DCONFIG_RTW_ONE_PIN_GPIO
endif
ifeq ($(CONFIG_HIGH_ACTIVE_DEV2HST), y)
EXTRA_CFLAGS += -DHIGH_ACTIVE_DEV2HST=1
else
EXTRA_CFLAGS += -DHIGH_ACTIVE_DEV2HST=0
endif
endif

ifeq ($(CONFIG_HIGH_ACTIVE_HST2DEV), y)
EXTRA_CFLAGS += -DHIGH_ACTIVE_HST2DEV=1
else
EXTRA_CFLAGS += -DHIGH_ACTIVE_HST2DEV=0
endif

ifneq ($(CONFIG_WAKEUP_GPIO_IDX), default)
EXTRA_CFLAGS += -DWAKEUP_GPIO_IDX=$(CONFIG_WAKEUP_GPIO_IDX)
endif

ifeq ($(CONFIG_RTW_SDIO_PM_KEEP_POWER), y)
ifeq ($(CONFIG_SDIO_HCI), y)
EXTRA_CFLAGS += -DCONFIG_RTW_SDIO_PM_KEEP_POWER
endif
endif

ifeq ($(CONFIG_REDUCE_TX_CPU_LOADING), y)
EXTRA_CFLAGS += -DCONFIG_REDUCE_TX_CPU_LOADING
endif

ifeq ($(CONFIG_BR_EXT), y)
BR_NAME = br0
EXTRA_CFLAGS += -DCONFIG_BR_EXT
EXTRA_CFLAGS += '-DCONFIG_BR_EXT_BRNAME="'$(BR_NAME)'"'
endif


ifeq ($(CONFIG_TDLS), y)
EXTRA_CFLAGS += -DCONFIG_TDLS
endif

ifeq ($(CONFIG_WIFI_MONITOR), y)
EXTRA_CFLAGS += -DCONFIG_WIFI_MONITOR
endif

ifeq ($(CONFIG_MCC_MODE), y)
EXTRA_CFLAGS += -DCONFIG_MCC_MODE
endif

ifeq ($(CONFIG_RTW_NAPI), y)
EXTRA_CFLAGS += -DCONFIG_RTW_NAPI
endif

ifeq ($(CONFIG_RTW_GRO), y)
EXTRA_CFLAGS += -DCONFIG_RTW_GRO
endif

ifeq ($(CONFIG_RTW_IPCAM_APPLICATION), y)
EXTRA_CFLAGS += -DCONFIG_RTW_IPCAM_APPLICATION
ifeq ($(CONFIG_WIFI_MONITOR), n)
EXTRA_CFLAGS += -DCONFIG_WIFI_MONITOR
endif
endif

ifeq ($(CONFIG_RTW_NETIF_SG), y)
EXTRA_CFLAGS += -DCONFIG_RTW_NETIF_SG
endif

ifeq ($(CONFIG_ICMP_VOQ), y)
EXTRA_CFLAGS += -DCONFIG_ICMP_VOQ
endif

ifeq ($(CONFIG_IP_R_MONITOR), y)
EXTRA_CFLAGS += -DCONFIG_IP_R_MONITOR
endif

ifeq ($(CONFIG_MP_VHT_HW_TX_MODE), y)
EXTRA_CFLAGS += -DCONFIG_MP_VHT_HW_TX_MODE
ifeq ($(CONFIG_PLATFORM_I386_PC), y)
## For I386 X86 ToolChain use Hardware FLOATING
EXTRA_CFLAGS += -mhard-float
else
## For ARM ToolChain use Hardware FLOATING
EXTRA_CFLAGS += -mfloat-abi=hard
endif
endif

ifeq ($(CONFIG_APPEND_VENDOR_IE_ENABLE), y)
EXTRA_CFLAGS += -DCONFIG_APPEND_VENDOR_IE_ENABLE
endif

ifeq ($(CONFIG_RTW_DEBUG), y)
EXTRA_CFLAGS += -DCONFIG_RTW_DEBUG
EXTRA_CFLAGS += -DRTW_LOG_LEVEL=$(CONFIG_RTW_LOG_LEVEL)
endif

ifeq ($(CONFIG_PROC_DEBUG), y)
EXTRA_CFLAGS += -DCONFIG_PROC_DEBUG
endif

ifeq ($(CONFIG_RTW_UP_MAPPING_RULE), dscp)
EXTRA_CFLAGS += -DCONFIG_RTW_UP_MAPPING_RULE=1
else
EXTRA_CFLAGS += -DCONFIG_RTW_UP_MAPPING_RULE=0
endif

EXTRA_CFLAGS += -DPLATFORM_LINUX

ifeq ($(USE_TRUE_PHY), y)
EXTRA_CFLAGS += -DUSE_TRUE_PHY
endif

ifeq ($(CONFIG_HWSIM), y)
EXTRA_CFLAGS += -DCONFIG_HWSIM

# To use pure sw beacon
EXTRA_CFLAGS += -DCONFIG_SWTIMER_BASED_TXBCN
EXTRA_CFLAGS += -DCONFIG_SUPPORT_MULTI_BCN
endif

ifeq ($(CONFIG_DRV_FAKE_AP), y)
EXTRA_CFLAGS += -DCONFIG_DRV_FAKE_AP
OBJS += core/rtw_fake_ap.o
endif

ifeq ($(CONFIG_DBG_AX_CAM), y)
EXTRA_CFLAGS += -DCONFIG_DBG_AX_CAM
endif

ifeq ($(CONFIG_I386_BUILD_VERIFY), y)
EXTRA_CFLAGS += -DCONFIG_I386_BUILD_VERIFY
endif

ifeq ($(CONFIG_RTW_MBO), y)
EXTRA_CFLAGS += -DCONFIG_RTW_MBO -DCONFIG_RTW_WNM -DCONFIG_RTW_BTM_ROAM
#EXTRA_CFLAGS += -DCONFIG_RTW_80211K
EXTRA_CFLAGS += -DCONFIG_RTW_80211R
endif

ifeq ($(CONFIG_SEC_AMSDU_MODE), non-spp)
EXTRA_CFLAGS += -DRTW_AMSDU_MODE=0
else ifeq ($(CONFIG_SEC_AMSDU_MODE), spp)
EXTRA_CFLAGS += -DRTW_AMSDU_MODE=1
else ifeq ($(CONFIG_SEC_AMSDU_MODE), disable)
EXTRA_CFLAGS += -DRTW_AMSDU_MODE=2
endif

SUBARCH := $(shell uname -m | sed -e "s/i.86/i386/; s/ppc.*/powerpc/; s/armv.l/arm/; s/aarch64/arm64/;")
ARCH ?= $(SUBARCH)

########### PLATFORM OPS  ##########################
# Import platform assigned KSRC and CROSS_COMPILE
include $(wildcard $(DRV_PATH)/platform/*.mk)

# Import platform specific compile options
EXTRA_CFLAGS += -I$(src)/platform
#_PLATFORM_FILES := platform/platform_ops.o
OBJS += $(_PLATFORM_FILES)

########### CUSTOMER ################################
USER_MODULE_NAME ?=
ifneq ($(USER_MODULE_NAME),)
MODULE_NAME := $(USER_MODULE_NAME)
endif

ifneq ($(KERNELRELEASE),)
########### COMMON #################################
include $(src)/common.mk

EXTRA_CFLAGS += -DPHL_PLATFORM_LINUX
EXTRA_CFLAGS += -DCONFIG_PHL_ARCH

ifeq ($(RTW_PHL_RX), y)
EXTRA_CFLAGS += -DRTW_PHL_RX
endif

ifeq ($(RTW_PHL_TX), y)
EXTRA_CFLAGS += -DRTW_PHL_TX
endif

ifeq ($(RTW_PHL_BCN), y)
EXTRA_CFLAGS += -DRTW_PHL_BCN
endif

ifeq ($(DIRTY_FOR_WORK), y)
EXTRA_CFLAGS += -DDIRTY_FOR_WORK
endif

include $(src)/phl/phl.mk


obj-$(CONFIG_RTL8852AU) := $(MODULE_NAME).o
obj-$(CPTCFG_RTL8852AE) := $(MODULE_NAME).o
$(MODULE_NAME)-y = $(OBJS)

else

export CONFIG_RTL8852AU = m

SUBARCH := $(shell uname -m | sed -e "s/i.86/i386/; s/ppc.*/powerpc/; s/armv.l/arm/; s/aarch64/arm64/;")
ARCH ?= $(SUBARCH)

all: modules

modules:
	#rm -f .symvers.$(MODULE_NAME)

	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(shell pwd)  modules

	#cp Module.symvers .symvers.$(MODULE_NAME)

strip:
	$(CROSS_COMPILE)strip $(MODULE_NAME).ko --strip-unneeded

install:
	@mkdir -p $(MODDESTDIR)realtek/rtw89/
	install -p -m 644 $(MODULE_NAME).ko  $(MODDESTDIR)realtek/rtw89/
	/sbin/depmod -a ${KVER}
	@cp suspend_rtw8852au /usr/lib/systemd/system-sleep/.

uninstall:
	rm -f $(MODDESTDIR)realtek/rtw89/$(MODULE_NAME).ko
	/sbin/depmod -a ${KVER}


.PHONY: modules clean sign

sign:
	@openssl req -new -x509 -newkey rsa:2048 -keyout MOK.priv -outform DER -out MOK.der -nodes -days 36500 -subj "/CN=Custom MOK/"
	@mokutil --import MOK.der
	@$(KSRC)/scripts/sign-file sha256 MOK.priv MOK.der 8852au.ko

sign-install: all sign install

clean:
	#$(MAKE) -C $(KSRC) M=$(shell pwd) clean
	cd $(HAL) ; rm -fr */*/*/*/*.mod.c */*/*/*/*.mod */*/*/*/*.o */*/*/*/.*.cmd */*/*/*/*.ko
	cd $(HAL) ; rm -fr */*/*/*.mod.c */*/*/*.mod */*/*/*.o */*/*/.*.cmd */*/*/*.ko
	cd $(HAL) ; rm -fr */*/*.mod.c */*/*.mod */*/*.o */*/.*.cmd */*/*.ko
	cd $(HAL) ; rm -fr */*.mod.c */*.mod */*.o */.*.cmd */*.ko
	cd $(HAL) ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd core ; rm -fr */*.mod.c */*.mod */*.o */.*.cmd */*.ko
	cd core ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd os_dep/linux ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd os_dep/linux/hwsim ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd os_dep ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd platform ; rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	rm -fr Module.symvers ; rm -fr Module.markers ; rm -fr modules.order
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -fr .tmp_versions
endif

