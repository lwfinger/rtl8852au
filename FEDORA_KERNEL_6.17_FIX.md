# Fix for Fedora 43 Kernel 6.17.9

This branch contains fixes to make the rtl8852au driver compile on Fedora 43 with kernel 6.17.9+.

## Problem

The mainline kernel 6.17+ changed the cfg80211 wireless API, adding a `radio_id` parameter to several functions. The original driver doesn't compile on these newer kernels.

## Changes Made

Added `radio_id` parameter to:
- `cfg80211_rtw_set_wiphy_params()`
- `cfg80211_rtw_set_txpower()`  
- `cfg80211_rtw_get_txpower()`

## Installation on Fedora 43
```bash
# Install dependencies
sudo dnf install kernel-devel kernel-headers gcc make git dkms

# Clone this fixed branch
git clone -b fedora-kernel-6.17-fix https://github.com/andreas-patsalos/rtl8852au.git
cd rtl8852au

# Compile and install
make
sudo make install

# IMPORTANT: Enable USB 3.0 mode for full performance
sudo bash -c 'echo "options 8852au rtw_switch_usb_mode=1" > /etc/modprobe.d/8852au.conf'

# Reboot
sudo reboot
```

## Post-Installation: Performance Optimization

### 1. Verify USB 3.0 Mode
```bash
lsusb -t | grep rtl8852au
# Should show "5000M" not "480M"
```

### 2. Disable Power Saving
```bash
sudo iw dev <your-interface> set power_save off
```

Make it permanent:
```bash
sudo bash -c 'cat > /etc/NetworkManager/dispatcher.d/disable-wifi-powersave << "SCRIPT"
#!/bin/bash
if [ "$2" = "up" ]; then
    /usr/sbin/iw dev $1 set power_save off
fi
SCRIPT'
sudo chmod +x /etc/NetworkManager/dispatcher.d/disable-wifi-powersave
```

### 3. Connect to 5GHz Network
For best speeds, use 5GHz band instead of 2.4GHz.

## Supported Devices

Tested on:
- TP-Link Archer TX20U Plus (USB ID: 2357:013f)

Should work with any RTL8832AU/RTL8852AU chipset adapter.

## Performance Notes

- **USB 3.0 is essential** - Plug into blue USB port
- Expected speeds: 150-250+ Mbps on 5GHz with good signal
- USB 2.0 will limit you to ~50 Mbps

## Troubleshooting

**Driver loads but no interface appears:**
```bash
sudo modprobe -r 8852au
sudo modprobe 8852au rtw_switch_usb_mode=1
```

**Stuck at USB 2.0 speeds:**
- Make sure you're in a USB 3.0 port (blue plastic inside)
- Verify `rtw_switch_usb_mode=1` is set in `/etc/modprobe.d/8852au.conf`

## Credits

- Original driver: [natimerry/rtl8852au](https://github.com/natimerry/rtl8852au)
- Kernel 6.17 fixes: Tested and documented through extensive troubleshooting on Fedora 43
