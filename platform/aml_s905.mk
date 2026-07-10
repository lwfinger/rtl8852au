ifeq ($(CONFIG_PLATFORM_AML_S905), y)
ccflags-y += -DCONFIG_PLATFORM_AML_S905
ccflags-y += -DCONFIG_LITTLE_ENDIAN
ccflags-y += -DCONFIG_IOCTL_CFG80211 -DRTW_USE_CFG80211_STA_EVENT
ccflags-y += -DCONFIG_RADIO_WORK
ccflags-y += -DCONFIG_CONCURRENT_MODE

# default setting for Android
# config CONFIG_RTW_ANDROID in main Makefile

ARCH ?= arm64
CROSS_COMPILE ?= /home/Jimmy/amlogic_905x4+8852AS/skw-rtk/cross_compile_toolchain/gcc-linaro-6.3.1-2017.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
ifndef KSRC
KSRC := /home/Jimmy/amlogic_905x4+8852AS/skw-rtk/kernel
# To locate output files in a separate directory.
KSRC += O=/home/Jimmy/amlogic_905x4+8852AS/skw-rtk/kernel_obj
endif

ifeq ($(CONFIG_PCI_HCI), y)
ccflags-y += -DCONFIG_PLATFORM_OPS
_PLATFORM_FILES := platform/platform_linux_pc_pci.o
OBJS += $(_PLATFORM_FILES)
endif

ifeq ($(CONFIG_RTL8852A), y)
ifeq ($(CONFIG_SDIO_HCI), y)
CONFIG_RTL8852AS ?= m
USER_MODULE_NAME := 8852as
endif
ifeq ($(CONFIG_PCI_HCI), y)
CONFIG_RTL8852AE ?= m
USER_MODULE_NAME := 8852ae
endif
endif

endif
