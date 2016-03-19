# web-control-center

Use web server to execute defined action via php http request on ubuntu or raspberry pi.
- Use your apache server and php
- Easy to install
- Easy to customize for your propose

### Version
1.0.0

### Tech

web-control-center uses a number of open source projects to work properly:
* [libJson] - a powerful json lib.

For build-in demo
* [jQuery] - a powerful ui framework on javascript.
* [libbluetooth-dev] -BlueZ is the official Linux Bluetooth protocol stack. It is an Open Source project distributed under GNU General Public License (GPL).



### Installation

Compile
```sh
$ cd /src
$ make
```

pack deb
```sh
$ cd ..
$ ./pack.sh
```
Install deb
```sh
$ ./install.sh
```
Uninstall:
```sh
$ ./unistall.sh
```
Reinstall:
```sh
$ ./reistall.sh
```

### Start Service
```sh
$ sudo service WccService start 
```

### Stop Service
```sh
$ sudo service WccService stop 
```

### Useage
- Modify cmd_map.xml to add your Action 

```sh
<CMD_MAP_TABLE>
<CMD_MAP>
<NAME>Rotate_1</NAME><!--Your action name -->
<CMD>echo 0=61 &gt; /dev/servoblaster</CMD><!--Your action cmd line want to be exec-->
</CMD_MAP>
</CMD_MAP_TABLE>
```
- Re-start web-control-center service.
```sh
sudo service WccService restart 
```
- Use http request to launch your action!
```sh
$ http://your_ip:1226/api/index.php?cmd=CMD_NAME&param=PARAM'
```

### Buind-In DEMO
```sh
$ http://your_ip:1226/index.html'
```
- Use web page to control USB Bluetooth dungle to scan device and show result

### Source code guide 
- [MyServ] A daemon to handle the command from php and exec target action defined in cmd_map.xml. 

### Source code for build-in demo guide
- [btconnect] 
-- Connect for bluetooth device
- [btscan] --
Scan for bluetooth device
- [getBTLinkKeys]--
Get pair list for bluetooth device
- [IsProcessExist] --
Check if process is running
- [KillProcess]--
Stop a process



License
----
MIT

