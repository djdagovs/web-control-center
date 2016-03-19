<?php

$cmd = $_GET['cmd'];
$param = $_GET['param'];

set_time_limit(1800);
ob_implicit_flush(true);


$exe_command = 'sudo /usr/bin/web-control-center/'.$cmd.' '.$param;
//$exe_command = 'ping www.google.com';
//echo $exe_command;
//if(strlen($cmd)>0)
//echo $exe_command;
$strResult = '';
$descriptorspec = array(
    0 => array("pipe", "r"),  // stdin
    1 => array("pipe", "w"),  // stdout -> we use this
    2 => array("pipe", "w")   // stderr 
);

$process = "";
//echo "1\n";
$process = proc_open($exe_command, $descriptorspec, $pipes);
//echo "2\n";
if (is_resource($process))
{
//    echo "3\n";
    $temp = "";
    while( ! feof($pipes[1]))
    {
	//echo "4\n";
        $return_message = fgets($pipes[1], 1024);
        
        if (strlen($return_message) == 0) 
	    continue;

	echo $return_message;
//	$temp = $return_message;        
	ob_flush();
        flush();
    }
  //  $strResult = $strResult.$temp;
}
//echo $strResult;
?>

