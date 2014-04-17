<html>
<body>
<?php
//echo "check2";
$filename = "mydata.txt";
$fp = fopen($filename,"w");
if(isset($_POST['news'])) {
	$data = $_POST['news'];
	if($fp) {
		fwrite ($fp, $data);
		fclose ($fp);
		echo ("File written");
	}
	else {
		echo ("File was not written");
	} 
//    $ret = file_put_contents('mydata.txt', $data, FILE_APPEND | LOCK_EX);
//    if($ret === false) {
//        die('There was an error writing this file');
//    }
//    else {
//        echo "$ret bytes written to file";
//    }
	$li=shell_exec("./php_root");
	print_r($li);
}
else {
   die('no post data to process');
}
header("Location: about.php");
?>
</body>
</html>
