# ArduPilot Installation

In this section, we will go over the steps to prepare the BeagleBone Blue as well as the installation process of ArduPilot.

## BeagleBone Blue Setup

Make sure for all the following steps that the BeagleBone Blue is connected to a strong enough power supply (i.e. not powered over USB by computer). 

1. Get the latest image for the BeagleBone Blue frome this [link](https://rcn-ee.net/rootfs/bb.org/testing/). Choose the __most recent date__, then the folder __buster-console__ and download the file called _bone-debian-XX.X-console-armhf-YEAR-MONTH-DAY-1gb.img.xz \
Direct Link to file used in this tutorial: [Bone Debian 10.4](https://rcn-ee.net/rootfs/bb.org/testing/2020-05-18/buster-console/bone-debian-10.4-console-armhf-2020-05-18-1gb.img.xz)

2. Next, you'll need to flash the image to a microSD card. The easiest way to do this is by using [Etcher](https://www.balena.io/etcher/). Follow the steps in the app, the process itself should then be self-explanatory. Insert the SD card, then press the SD button in the corner of the BeagleBone and plug in the power cable to boot up the BBB.

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
    The response will be a hash that looks something like *wifi_38d279e099a8_4254487562142d4355434b_managed_psk*. 

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

    Click CTRL-O to save the file (make sure to confirm using enter) and CTRL-X to quit the editor.

    A prominent green LED should start to light up, signifying that the BBlue is connected to the WiFi. In order to get its IP address, type:
    ```shell
    ip addr show wlan0
    ``` 
    Now try SSHing into your BBBlue using its WiFi IP address (the one right next to inet without the `/XX`). Make sure that your computer is now connected to the same WiFi network. So in case you've been connected to the BeagleBone's WiFi hotspot, make sure to change networks first.

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

8. Specify real-time kernel 4_9: 
    ```shell 
    sudo /opt/scripts/tools/update_kernel.sh --lts-4_9 --bone-rt-channel
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

13. Maximize the microSD card's existing partition as you might run into storage problems later on otherwise: 
    ```shell 
    sudo /opt/scripts/tools/grow_partition.sh
    ```

14. Reboot now:
    ```shell
    sudo reboot
    ```

*__Troubleshooting__: If you experience a `RCOutputAioPRU.cpp:SIGBUS error`, there are two things you could try. Wiping the eMMC boot sector with `sudo dd if=/dev/zero of=/dev/mmcblk1 bs=1M count=10` might potentially help, as well as updating the bootlaoder with `sudo /opt/scripts/tools/developers/update_bootloader.sh`.*

## Installing ArduPilot
1. First step is to create the ArduPilot configuration file by typing:
    ```shell
    sudoedit /etc/default/ardupilot
    ```
    In this file, we'll add our configuration parameters: 
    ```
    TELEM1="-A udp:<target IP address>:14550" 
    GPS="-B /dev/ttyO2"
    TELEM2="-C /dev/ttyO1"
    ```

    * `-A` is a console switch. Usually this is a Wi-Fi link where you can specify the IP address of the device with the ground control station. It maps ArduPilot's _Console_ serial port (SERIAL0, default baud rate: 115'200) to the specified IP address. This allows to have MAVLink data coming over WiFi for test purposes and is reliably auto-sensed by Mission Planner and QGroundControl. 

    * `-B`is used to specify non default GPS. In this case, it maps ArduPilot's _GPS_ serial port (SERIAL 3, default baud rate: 57'600) to the BeagleBone's UART2 (the UART marked with GPS on the board).

    * `-C` is a telemetry switch. It maps ArduPilot's _Telem1_ serial port (SERIAL1, default baud rate: 57'600) to the BBBlue's UART1 where a telemetry module can be connected to. This is useful for a bidirectional data link between the drone and the groundstation.

    All the possibilities that exist can be seen in the following list:
    ```
    Switch -A  -->  "Console", SERIAL0, default 115200
    Switch -B  -->  "GPS", SERIAL3, default 57600
    Switch -C  -->  "Telem1", SERIAL1, default 57600
    Switch -D  -->  "Telem2", SERIAL2, default 38400
    Switch -E  -->  Unnamed, SERIAL4, default 38400
    Switch -F  -->  Unnamed, SERIAL5, default 57600
    ```
    Check out the official [ArduPilot documentation](https://ardupilot.org/plane/docs/parameters.html?highlight=parameters) for further details on serial ports and parameters.

2. Next, we'll create the ArduPilot _systemd service files_, in this case for ArduCopter. The process for ArduPlane and ArduRover looks accordingly, just switch names everywhere and adjust the _conflicts_ section.

    Create the file (using `sudoedit /lib/systemd/system/arducopter.service` or whatever tool you want to use) and add the following content:
    ```
    [Unit]
    Description=ArduCopter Service
    After=networking.service
    StartLimitIntervalSec=0
    Conflicts=arduplane.service ardurover.service antennatracker.service

    [Service]
    EnvironmentFile=/etc/default/ardupilot
    ExecStartPre=/usr/bin/ardupilot/aphw
    ExecStart=/usr/bin/ardupilot/arducopter $TELEM1 $TELEM2 $GPS

    Restart=on-failure
    RestartSec=1

    [Install]
    WantedBy=multi-user.target
    ```
    Make sure to provide the name of all environment variables created in step 1 on the `ExecStart` line.

3. Create a new directory:
    ```shell
    sudo mkdir -p /usr/bin/ardupilot
    ```

    Then we'll create a ArduPilot hardware configuration file with `sudoedit /usr/bin/ardupilot/aphw`, which is run by the services defined in step 2 prior to running the ArduPilot executables:

    ```
    #!/bin/bash
    # aphw
    # ArduPilot hardware configuration.

    /bin/echo 80 >/sys/class/gpio/export
    /bin/echo out >/sys/class/gpio/gpio80/direction
    /bin/echo 1 >/sys/class/gpio/gpio80/value
    /bin/echo pruecapin_pu >/sys/devices/platform/ocp/ocp:P8_15_pinmux/state
    ```
    Lines 5-7 switch on power to the BBBlue's +5V servo rail - i.e. for when you're using servos. This is not necessary for ESCs.
    Line 8 enables the PRU.

4. Let's make the file executable by setting the right permissions:
    ```shell
    sudo chmod 0755 /usr/bin/ardupilot/aphw
    ```

5. Now we have to get the latest ArduCopter (or ArduPlane, ArduRover) executable, built specifically for the BeagleBone Blue's ARM architecture. 

    Compiling on the BeagleBone itself works as follows but it will take quite some time:
    ```shell
    sudo apt-get install g++ make pkg-config python python-dev python-lxml python-pip
    sudo pip install future
    git clone https://github.com/ArduPilot/ardupilot
    cd ardupilot
    git branch -a  # <-- See all available branches.
    git checkout Copter-3.6.8-hotfix # <-- Select one of the ArduCopter branches.
    git submodule update --init --recursive
    ./waf configure --board=blue  # <-- BeagleBone Blue.
    ./waf
    sudo cp ./build/blue/bin/a* /usr/bin/ardupilot
    sudo chmod 0755 /usr/bin/ardupilot/a*
    ```

6. To start ArduPilot, type the following line and after a reboot, the ArduPilot should inflate automatically which can be seen by a flashing red LED. 
    ```shell
    sudo systemctl enable arducopter.service
    ``` 

    Of course, if you have done the previous steps for other services as well, you can also start ArduPlane OR ArduRover using:
    ```shell 
    sudo systemctl enable arduplane.service
    sudo systemctl enable ardurover.service
    ```

    Make sure to familiarise yourself with `systemctl`, espcially with following commands:
    ```shell
    sudo systemctl disable <name of service>
    sudo systemctl start <name of service>
    sudo systemctl stop <name of service>
    ```