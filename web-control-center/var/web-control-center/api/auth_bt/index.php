<?php

$cmd = $_GET['cmd'];
$param = $_GET['param'];

//$cmd="60:FA:CD:41:0B:BC";
set_time_limit(1800);
ob_implicit_flush(true);
//bluez-test-device remove

$exe_command = 'sudo echo 0000|sudo 
bluez-simple-agent hci0 '.$cmd.' '.$param;
//$exe_command = 'ping www.google.com';
//echo $exe_command;
//if(strlen($cmd)>0)
echo $exe_command;
$strResult = '';
$descriptorspec = array(
    0 => array("pipe", "r"),  // stdin
    1 => array("pipe", "w"),  // stdout -> we use this
    2 => array("pipe", "w")   // stderr 
);

$process = "";

$process = proc_open($exe_command, $descriptorspec, $pipes);
if (is_resource($process))
{
    $temp = "";
    while( ! feof($pipes[1]))
    {
        $return_message = fgets($pipes[1], 1024);
        if (strlen($return_message) == 0) 
	    break;

	$temp = $return_message;        
	ob_flush();
        flush();
    }
    $strResult = $strResult.$temp;
}
echo $strResult;
?>

