<?php

$cmd = $_GET['cmd'];

set_time_limit(1800);
ob_implicit_flush(true);


$exe_command = '/usr/bin/web-control-center/MyServ';

if(strlen($cmd)>0)
$exe_command = $exe_command." send ".$cmd;
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

