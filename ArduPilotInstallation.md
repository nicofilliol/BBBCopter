# ArduPilot Installation

In this section, we will go over the steps to prepare the BeagleBone Blue as well as the installation process of ArduPilot.

## BeagleBone Blue Setup

Make sure for all the following steps that the BeagleBone Blue is connected to a strong enough power supply (i.e. not powered over USB by computer). 

1. Get the latest image for the BeagleBone Blue frome this [link](https://rcn-ee.net/rootfs/bb.org/testing/). Choose the __most recent date__, then the folder __buster-console__ and download the file called _bone-debian-XX.X-console-armhf-YEAR-MONTH-DAY-1gb.img.xz \
Direct Link to file used in this tutorial: [Bone Debian 10.4](https://rcn-ee.net/rootfs/bb.org/testing/2020-05-18/buster-console/bone-debian-10.4-console-armhf-2020-05-18-1gb.img.xz)

2. Next, you'll need to flash the image to a microSD card. The easiest way to do this is by using [Etcher](https://www.balena.io/etcher/). Follow the steps in the app, the process itself should then be self-explanatory.

3. We now have to connect to the BeagleBone over SSH. There are a few different options to do this (please also refer to the official [Getting Started](https://beagleboard.org/getting-started) page). \
_Option 1_: Connect the BeagleBoard to your computer over USB and install [drivers](https://beagleboard.org/getting-started#troubleshooting) according to your operating system. Then ssh into the BB using:
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