# Authors : Bibek Koirala, Rodrigo Baranda
# Name : script.py
# Description : Script that receives data from rotation detector device over bluetooth and changes screen orientation accordingly.

import os
import sys
import bluetooth
import platform
import win32api as win32 
import win32con 

class BaseCommunication:
    def __init__(self):
        self.__endpoint = None
        self.__callback = None
        self.__extra_parameters = None

    def set_endpoint(self, endpoint):
        self.__endpoint = endpoint

    def get_endpoint(self):
        return self.__endpoint

    def set_callback(self, callback):
        self.__callback = callback

    def get_callback(self):
        return self.__callback

    def set_extra_parameters(self, parameters):
        self.__extra_parameters = parameters

    def get_extra_parameters(self):
        return self.__extra_parameters
		
		

class BluetoothCommunication(BaseCommunication):

    def listen(self):
        btDevice = self.__connect()
        while True:
            data = btDevice.recv(10).decode()    # Receive vytes from rotation detector and convert to string
            if data == "":
                continue
            print("received: " + data)
            self.get_callback()(data, self.get_extra_parameters())

    def __connect(self):
        btDevice = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        btDevice.setblocking(True)
        btDevice.connect((self.get_endpoint(), 1))

        return btDevice
		
		

endpoint = sys.argv[1]        # provide address of bluetooth device as command line arguments
reversed = bool(int(sys.argv[2]))      # provide 0 or 1 as command line argument.

def linux(to, reversed):
    if to == "0":
        os.system("xrandr --output HDMI-1 --rotate normal")
    elif (to == "1" and not reversed) or (to == "2" and reversed):
        os.system("xrandr --output HDMI-1 --rotate right")
    elif (to == "1" and reversed) or (to == "2" and not reversed):
        os.system("xrandr --output HDMI-1 --rotate left")


def windows(to, reversed):
    DISPLAY_NUMBER = 0 # Display number Id of the display to be used with rotation detector
    device = win32.EnumDisplayDevices(None,DISPLAY_NUMBER); 
    print("Rotate device %s (%s)"%(device.DeviceString,device.DeviceName)); 

    dm = win32.EnumDisplaySettings(device.DeviceName,win32con.ENUM_CURRENT_SETTINGS) 
    if to == "0":
        dm.DisplayOrientation = win32con.DMDO_DEFAULT 
    elif (to == "1" and not reversed) or (to == "2" and reversed):
        dm.DisplayOrientation = win32con.DMDO_270        
    elif (to == "1" and reversed) or (to == "2" and not reversed):
        dm.DisplayOrientation = win32con.DMDO_90
		
    dm.PelsWidth, dm.PelsHeight = dm.PelsHeight, dm.PelsWidth 
    win32.ChangeDisplaySettingsEx(device.DeviceName,dm)


	
def switch_display(to, parameters):
    reversed = parameters['reversed']
    operating_system = platform.system()
    if (operating_system == "Linux"):
	    linux(to, reversed)
    elif (operating_system == "Windows"):
        windows(to, reversed)

communicator = BluetoothCommunication()
communicator.set_endpoint(endpoint)
communicator.set_callback(switch_display)
communicator.set_extra_parameters({'reversed': reversed})
communicator.listen()
