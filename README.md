# web-control-center
Use web server to execute defined action via php on [ubuntu] / [raspberry pi]

<Files introduce>
1.bash files
[pack.sh] after build all components, use this to pake a install pack dep.
[install.sh]install dep to system 
[uninstall.sh]unistall dep to system
[reinstall.sh]reinsstall dep to system

2.web php/html for apache are put at
web-control-center/var/web-control-center

3.executable files are put at 
web-control-center/usr/bin/web-control-center
[btconnect] use to connct bluetooth devices.
[btscan] use to scan bluetooth devices via usb dungle
[cmd_map.xml] use xml format to define action key for php api. 
[getBTLinkKeys]use to get Bluetooth pair list.

<Make and test>
1.enter src folder , type "make" to compile all exectable files.
2.back to top folder , type "./pach.sh" to pack into a "web-control-center.deb" file.
3.type "install.sh" to install to your raspberry pi/ubuntu.
4.Open web browser open url "http://your_ip:1226/index.html"

<customize for your purpose>
modify cmd_map.xml to add your Action and re-start web-control-center service.
use http request to get http://your_ip:1226/api/index.php?cmd=CMD_KEY&param=PARAM' to launch your action!


<example 1>
I want to define a action for rotate a servo module.

1.the action command line is "echo 0=61 &gt; /dev/servoblaster"
2.I give a name "Rotate_1" for this action.

so I modify cmd_map.xml

<CMD_MAP_TABLE>
    <CMD_MAP>
        <NAME>Rotate_1</NAME>
        <CMD>echo 0=61 &gt; /dev/servoblaster</CMD>
    </CMD_MAP>
</CMD_MAP_TABLE>

3.use http request to get http://your_ip:1226/api/index.php?cmd=Rotate_1&param=""'

<example 2>
I want to check if a process named "MyServ" "is exist.

1.the action command line is "/usr/bin/web-control-center/IsProcessExist MyServ"
2.I give a name "IsProcessExist" for this action.

so I modify cmd_map.xml

<CMD_MAP_TABLE>
    <CMD_MAP>
        <NAME>IsProcessExist</NAME>
        <CMD>/usr/bin/web-control-center/IsProcessExist</CMD>
    </CMD_MAP>
</CMD_MAP_TABLE>

3.use http request to get http://your_ip:1226/api/index.php?cmd=IsProcessExist&param="MyServ"'
