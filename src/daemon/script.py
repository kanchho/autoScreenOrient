# Authors : Bibek Koirala, Rodrigo Baranda
# Name : script.py
# Description : Script that receives data from rotation detector device over bluetooth and changes screen orientation accordingly.

import os
import sys
import bluetooth
import platform
import win32api as win32 
import win32con 
		

class BluetoothCommunication():
    def __init__(self):
        self.__endpoint = None
        self.__callback = None

    def set_endpoint(self, endpoint):
        self.__endpoint = endpoint

    def get_endpoint(self):
        return self.__endpoint

    def set_callback(self, callback):
        self.__callback = callback

    def get_callback(self):
        return self.__callback

    def listen(self):
        btDevice = self.__connect()
        while True:
            data = btDevice.recv(10).decode()    # Receive vytes from rotation detector and convert to string
            if data == "":
                continue
            print("received: " + data)
            self.get_callback()(data)

    def __connect(self):
        btDevice = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        btDevice.setblocking(True)
        btDevice.connect((self.get_endpoint(), 1))

        return btDevice
		
		
def linux(to):
    if to == "0":
        os.system("xrandr --output HDMI-1 --rotate normal")
    elif to == "1":
        os.system("xrandr --output HDMI-1 --rotate left")
    elif to == "2":
        os.system("xrandr --output HDMI-1 --rotate right")


def windows(to):
    DISPLAY_NUMBER = 0 # Display number Id of the display to be used with rotation detector
    device = win32.EnumDisplayDevices(None,DISPLAY_NUMBER); 
    print("Rotate device %s (%s)"%(device.DeviceString,device.DeviceName)); 

    dm = win32.EnumDisplaySettings(device.DeviceName,win32con.ENUM_CURRENT_SETTINGS) 
    if to == "0":
        dm.DisplayOrientation = win32con.DMDO_DEFAULT 
    elif to == "1":
        dm.DisplayOrientation = win32con.DMDO_90      
    elif to == "2":
        dm.DisplayOrientation = win32con.DMDO_270
		
    dm.PelsWidth, dm.PelsHeight = dm.PelsHeight, dm.PelsWidth 
    win32.ChangeDisplaySettingsEx(device.DeviceName,dm)


	
def switch_display(to):
    operating_system = platform.system()
    if (operating_system == "Linux"):
	    linux(to)
    elif (operating_system == "Windows"):
        windows(to)

communicator = BluetoothCommunication()
communicator.set_endpoint("98:D3:51:F5:AB:13")
communicator.set_callback(switch_display)
communicator.listen()
