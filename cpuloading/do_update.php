<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$vm=$_REQUEST['vm'];

function tailFile($filepath, $lines) {
	return trim(implode("", array_slice(file($filepath), $lines)));
}
// 拿最後一行
$line = tailFile("/home/anthonychen/Desktop/".$vm, -1);
// 切割
$cols = array_filter(explode(' ', $line));
// 拿最後一欄cpu loading
$cpu = end($cols);
echo 100.00 - $cpu;

?>