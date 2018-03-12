<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

// $vm=$_REQUEST['vm'];

function tailFile($filepath, $lines) {
	return trim(implode("", array_slice(file($filepath), $lines)));
}
// 拿最後一行
$line1 = tailFile("/home/mpiuser/m", -1);
$line2 = tailFile("/home/mpiuser/s1", -1);
$line3 = tailFile("/home/mpiuser/s2", -1);
// 切割
$cols1 = array_filter(explode(' ', $line1));
$cols2 = array_filter(explode(' ', $line2));
$cols3 = array_filter(explode(' ', $line3));
// 拿最後一欄cpu loading
$cpu1 = end($cols1);
$cpu2 = end($cols2);
$cpu3 = end($cols3);

/*$cpu1 = rand(0, 10);
$cpu2 = rand(0, 10);
$cpu3 = rand(0, 10);*/

echo "0 ".(100-$cpu1)." ".(100-$cpu2)." ".(100-$cpu3);
?>
