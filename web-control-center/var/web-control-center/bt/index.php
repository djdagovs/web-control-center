<?php

$cmd = $_GET['cmd'];
$param = $_GET['param'];

set_time_limit(1800);
ob_implicit_flush(true);


$exe_command = 'sudo /usr/bin/web-control-center/btscan';
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
		echo "<br>";
	else
		echo $return_message;
        
	ob_flush();
        flush();
    }
}
?>

