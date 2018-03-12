<?php
set_include_path(get_include_path() . PATH_SEPARATOR . 'phpseclib');
include('Net/SSH2.php');

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$ip = $_REQUEST['ip'];
$limit = $_REQUEST['limit'];
$time = $_REQUEST['time'];

$ssh = new Net_SSH2($ip);
$cmd = './loop '.$limit.' '.$time.' &';

if(!$ssh->login('root', 'anthony0556097'))
{
	exit('login fail');
}
echo $ssh->exec($cmd);

?>