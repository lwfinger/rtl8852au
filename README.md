This repo is was started with the code from the Realtek USB driver
RTL8852AU_WiFi_linux_v1.15.0.1-0-g487ee886.20210714. The current code improves
on the Realtek code by reworking the debug output to avoid spamming the logs.
In the current settings, messages from RTW_ERR(), RTW_WARNING(), and
RTW_WARNING() will be output.

If you want more output, increase the value of CONFIG_RTW_LOG_LEVEL in Makefile.
This parameter should probably be one that can be set at module load time,
but that is a matter for another time.

The driver supports rtl8832au/rtl8852au chipsets.

This driver currently handles the following devices:

* BUFFALO WI-U3-1200AX2(/N) with USB ID 0411:0312
* ASUS USB-AX56 with USB ID 0b05:1997
* ASUS USB-AX56 with USB ID 0b05:1a62
* EDUP EP-AX1696GS with USB ID 0bda:8832
* Fenvi FU-AX1800P with USB ID 0bda:885c
* Realtek Demo Board with USB ID 0bda:8832
* Realtek Demo Board with USB ID 0bda:885a
* Realtek Demo Board with USB ID 0bda:885c
* D-Link DWA-X1850 with USB ID 2001:3321
* TP-Link AX1800 with USB ID 2357:013f
* ipTIME AX2000U with USB ID 0bda:8832
* ELECOM WDC-X1201DU3 with USB ID 056e:4020

The D-Link DWA-X1850 comes with a configuration that appears to be a USB disk,
which contains a Windows driver. If a 'lsusb' command shows the ID 0bda:1a2b,
then this disk is mounted. The way to avoid this is to edit either file
/usr/lib/udev/rules.d/40-usb_modeswitch.rules, or
/lib/udev/rules.d/40-usb_modeswitch.rules, whichever is on your system, and add
the following lines:

# D-Link DWA-X1850 Wifi Dongle
ATTR{idVendor}=="0bda", ATTR{idProduct}=="1a2b", RUN+="usb_modeswitch '/%k'"

### Installation instruction
##### Requirements
You will need to install "make", "gcc", "kernel headers", "kernel build essentials", and "git".

For **Ubuntu**: You can install them with the following command
```bash
sudo apt-get update
sudo apt-get install make gcc linux-headers-$(uname -r) build-essential git
```
For **Fedora**: You can install them with the following command
```bash
sudo dnf install kernel-headers kernel-devel
sudo dnf group install "C Development Tools and Libraries"
```
For **openSUSE**: Install necessary headers with
```bash
sudo zypper install make gcc kernel-devel kernel-default-devel git libopenssl-devel
```
For **Arch**: After installing the necessary kernel headers and base-devel,
```bash
git clone https://aur.archlinux.org/rtw89-dkms-git.git
cd rtw89-dkms-git
makepkg -sri
```
If any of the packages above are not found check if your distro installs them like that.

##### Installation
When a USB device is plugged in, or detected at boot, this rule causes the utulity
usb_modeswitch to unload any 0bda:1a2b devices that it finds. If you have a
device with different ID, change the rule accordingly.

The build this driver, do the following:

For all distros:
```bash
git clone https://github.com/lwfinger/rtl8852au.git
cd rtl8852au
make
sudo make install

When you get a new kernel, you will need to rebuild the driver. Do the following:
cd rtl8852au
git pull
make
sudo make install
```

When your kernel is updated, then do a 'git pull' and redo the make commands.

##### Installation with module signing for SecureBoot
For all distros:
```bash
git clone git://github.com/lwfinger/rtl8852au.git
cd rtl8852au
make
sudo make sign-install
```
You will be promted for a password, please keep it in mind and use it in next steps.

Reboot to activate the new installed module.
In the MOK managerment screen:
1. Select "Enroll key" and enroll the key created by above sign-install step
2. When promted, enter the password you entered when create sign key. 

If you enter wrong password, your computer won't not rebootable. In this case,
   use the BOOT menu from your BIOS, to boot into your OS then do below steps:

```bash
sudo mokutil --reset
```
Restart your computer
Use BOOT menu from BIOS to boot into your OS
In the MOK managerment screen, select reset MOK list
Reboot then retry from the step make sign-install

## Adding modules to DKMS for Debian/Ubuntu

DKMS automatically rebuilds the driver module for each kernel update. (So that you don't have to `make; make install` at every update)

Build and Installation (For currently active kernel)

```bash
# Add module to dkms tree
sudo dkms add .

# Build 
sudo dkms build rtl8852au -v 1.15.0.1

# Install 
sudo dkms install rtl8852au -v 1.15.0.1

# Check installation
modinfo 8852au

# Load driver 
modprobe 8852au
```




Larry Finger
