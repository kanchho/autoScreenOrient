#!/bin/bash

MAC_ADDR="98:D3:51:F5:AB:13"

echo Installing bluetooth packages and python libraries...
sudo apt-get install bluetooth libbluetooth-dev bluez bluez-tools blueman

sudo apt install python3-pip
sudo pip3 install pybluez
echo
echo
echo

echo Setting up bluetooth...
sudo bluetoothctl
power on
agent on 
timeout 5 scan on
scan off
trust $MAC_ADDR
echo Please enter 1234 as passkey for pairing
pair $MAC_ADDR
quit
echo
echo

sudo killall rfcomm
sudo rfcomm bind 0 $MAC_ADDR 1
echo Bluetooth setup completed successfully
echo
echo
echo


echo Configuring program to run on startup....
sudo apt-get install cron
CMD="nohup python3 /home/autoScreenOrient/src/daemon/script.py &>/dev/null &"
(crontab -l ; echo "@reboot $CMD") 2>&1 | sed "s/no crontab for $(whoami)//"  | sort | uniq | crontab -            #Add cronjob to run python script on boot
sudo nohup python3 /home/autoScreenOrient/src/daemon/script.py &>/dev/null &                                           # Run python script now
clear

echo You can close this console now
echo Pyton script is running now
echo Python script is also setup to run on boot
echo Python script will continue to run on background even after the console is closed
