<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>License Generator</title>
</head>
<body style="background-color:#4682B4;">
<?php
    ob_start();
	echo '<br>';
	$cmd = 'C:\\LicenseGenerate_Client\\bin\\Debug\\LicenseGenerate_Client.exe 192.168.195.130 NEC';
	//echo $cmd;
	echo '<br>';
	exec($cmd, $out ,$status);
	echo '<br>';
	//var_dump($out);
	echo '<br>';
	echo '<br>';
	//echo $status;
	echo '<br>';
	echo '<br>';
	
	$cmd2 = '7z a C:\LicenseGenerate_Client\\license_File.zip C:\\LicenseGenerate_Client\\1_License_File\\**';
	echo $cmd2;
	echo '<br>';
	exec($cmd2, $out2);
	echo '<br>';
	var_dump($out2);
	
	echo '<br>';
	print($status);
	
	if ($status > 1)
	{
		echo "<script>alert('Execution Error');</script>";
	}
	else if ($status > 0 and $status < 2 )
	{
		echo "<script>alert('Execution Error, click `Clean Folder` and re-start again');</script>";
	}

	else
	{
		echo "<script>alert('Execution Success');</script>";
	}
	
	header("Refresh:0 ;url=licensegenerate.php");
	ob_end_flush();
    exit();
?> 

</body>
</html>
