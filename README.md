# Auto Screen Orient
 Auto Screen Orient is a system that automatically detects and applies the correct display orientation whenever screen is rotated by more than 45 degrees. The system consists of rotation detector device and daemon(python script) that changes the display orientation. This project was carried out as a part of Embedded Systems 101 course at Turku University of Applied Sciences. See [functional specification](docs/FunctionalSpecification.pdf) document for more info.

## Installation
####  Rotation Detector Device (RDD)
- Mount RDD on the back of monitor by following the instructions printed on RDD.
- Supply power to RDD with a standard mini USB cable.

#### Daemon
Depending on the operating system, follow the steps below :
##### Windows
 * Install [Python 3](https://www.python.org/downloads/windows/)
 * Open Git Bash and type the following commands :
```
  $ cd C:
  $ mkdir autoScreenOrient
  $ cd autoScreenOrient
  $ git clone https://github.com/kanchho/autoScreenOrient.git .
```

 * Go to System -> Devices -> Bluetooth and Other devices. Click on Add new device and pair with HC-05 with the passkey ```1234```
 * Go to C:\autoScreenOrient\src\daemon\windows and double-click on setup.bat file.


##### Linux
 * Fire up the terminal and type the following commands :
```
  $ mkdir autoScreenOrient
  $ cd autoScreenOrient
  $ git clone https://github.com/kanchho/autoScreenOrient.git .
  $ sudo bin/bash autoScreenOrient/src/daemon/linux/setup.sh
```
 * Follow the instructions in terminal.

## Usage  
After the installation process is completed, the user doesn't have to interact with the system as it works in the background. Leave the bluetooth service on so that the system works even after reboot.

## Project Members
* **Bibek Koirala** - *bibek.koirala@edu.turkuamk.fi*
* **Rodrigo Baranda** - *rodrigo.baranda@edu.turkuamk.fi*