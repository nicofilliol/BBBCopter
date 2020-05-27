# ArduPilot Installation

In this section, we will go over the steps to prepare the BeagleBone Blue as well as the installation process of ArduPilot.

## BeagleBone Blue Setup

Make sure for all the following steps that the BeagleBone Blue is connected to a strong enough power supply (i.e. not powered over USB by computer). 

1. Get the latest image for the BeagleBone Blue frome this [link](https://rcn-ee.net/rootfs/bb.org/testing/). Choose the __most recent date__, then the folder __buster-console__ and download the file called _bone-debian-XX.X-console-armhf-YEAR-MONTH-DAY-1gb.img.xz \
Direct Link to file used in this tutorial: [Bone Debian 10.4](https://rcn-ee.net/rootfs/bb.org/testing/2020-05-18/buster-console/bone-debian-10.4-console-armhf-2020-05-18-1gb.img.xz)

2. Next, you'll need to flash the image to a microSD card. The easiest way to do this is by using [Etcher](https://www.balena.io/etcher/). Follow the steps in the app, the process itself should then be self-explanatory.

3. We now have to connect to the BeagleBone over SSH. There are a few different options to do this (please also refer to the official [Getting Started](https://beagleboard.org/getting-started) page). \
_Option 1_: Connect the BeagleBoard to your computer over USB and install [drivers](https://beagleboard.org/getting-started#troubleshooting) according to your operating system. Then SSH into the BB using:
    ```shell
    ssh debian@192.168.6.2 # for Linux/Mac OS or 192.168.7.2 for Windows
    ```
    When asked, enter the default password which is `temppwd`. \
    _Option 2_: Connect to the Wifi-Network created by your BeagleBone (password: `BeagleBone`). To connect over ssh, use:
    ```shell
    ssh debian@192.168.8.1 # default password: temppwd
    ```

4. In order to make the process less tedious, we give the debian user (subsequent) sudo access without re-entering the password every time.
    ```shel
    echo "debian ALL=(ALL) NOPASSWD: ALL" | sudo tee -a /etc/sudoers.d/debian >/dev/null
    ```

5. We have to connect the BBBlue to the internet so that we can easily install updates and new software. Take note of your router's SSID and WiFi password. Then type:
    ```shell
    sudo -s
    connmanctl services | grep '<your SSID>' | grep -Po 'wifi_[^ ]+'
    ```
    The respnose will be a hash that looks something like *wifi_38d279e099a8_4254487562142d4355434b_managed_psk*. 

    Now using this hash, we're going to edit a file using nano. Create the file and open the editor with:
    ```shell
    nano /var/lib/connman/wifi.config
    ``` 
    Now enter the following contents into the file:
    ```
    [service_<your hash>]
    Type = wifi
    Security = wpa2
    Name = <your SSID>
    Passphrase = <your WiFi password>
    ```

    Click CTRL-O to save the file (make sure to confirm using enter) and CTRL-C to quit the editor.

    A prominent green LED should start to light up, signifying that the BBlue is connected to the WiFi. In order to get its IP address, type:
    ```shell
    ip addr show wlan0
    ``` 
    Now try SSHing into your BBBlue using its WiFi IP address. Make sure that your computer is now connected to the same WiFi network. So in case you've been connected to the BeagleBone's WiFi hotspot, make sure to change networks first.

6. In order to install and update required software, run the following commands:
    ```shell
    sudo apt-get -y update
    sudo apt-get -y dist-upgrade
    sudo apt-get install -y cpufrequtils git
    ```

7. Update the scripts: 
    ```shell 
    cd /opt/scripts && git pull
    ```

8. Specify real-time kernel 4_19: 
    ```shell 
    sudo /opt/scripts/tools/update_kernel.sh --lts-4_19 --bone-rt-channel 
    ```

9. Specify the device tree binary used at startup:
    ```shell
    sudo sed -i 's/#dtb=/dtb=am335x-boneblue.dtb/g' /boot/uEnv.txt
    ```

10. Specifiy device tree overlays:
    ```shell
    sudo sed -i 's|#dtb_overlay=/lib/firmware/<file8>.dtbo|dtb_overlay=/lib/firmware/BB-I2C1-00A0.dtbo\n#dtb_overlay=/lib/firmware/BB-UART4-00A0.dtbo\n#dtb_overlay=/lib/firmware/BB-ADC-00A0.dtbo|g' /boot/uEnv.txt
    ```

11. Specifiy U-Boot overlay:
    ```shell
    sudo sed -i 's|uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-14-TI-00A0.dtbo|#uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-14-TI-00A0.dtbo|g' /boot/uEnv.txt
    sudo sed -i 's|#uboot_overlay_pru=/lib/firmware/AM335X-PRU-UIO-00A0.dtbo|uboot_overlay_pru=/lib/firmware/AM335X-PRU-UIO-00A0.dtbo|g' /boot/uEnv.txt
    ```

12. Set clock frequency:
    ```shell
    sudo sed -i 's/GOVERNOR="ondemand"/GOVERNOR="performance"/g' /etc/init.d/cpufrequtils
    ```

13. Reboot now:
    ```shell
    sudo reboot
    ```

*__Troubleshooting__: If you experience a `RCOutputAioPRU.cpp:SIGBUS error`, there are two things you could try. Wiping the eMMC boot sector with `sudo dd if=/dev/zero of=/dev/mmcblk1 bs=1M count=10` might potentially help, as well as updating the bootlaoder with `sudo /opt/scripts/tools/developers/update_bootloader.sh`.*