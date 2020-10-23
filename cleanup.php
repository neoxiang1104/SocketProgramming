<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>License Generator</title>
</head>
<body style="background-color:#4682B4;">

<?php
	$dir="C:\\LicenseGenerate_Client\\0_Activate_Code\\";
	$files = glob($dir.'*');
	foreach ($files as $file) 
	if(@unlink($file))
	{
		echo "Cleanup the File" .$file."Success<br>";
	}
	else
	{
		echo "Cleanup the File" .$file."Failed<br>";
	}
	
	$dir2="C:\\LicenseGenerate_Client\\1_License_File\\";
	$files2 = glob($dir2.'*');
	foreach ($files2 as $file2) 
	if(@unlink($file2))
	{
		echo "Cleanup the File" .$file2."Success<br>";
	}
	else
	{
		echo "Cleanup the File" .$file2."Failed<br>";
	}
	
	$dir3="C:\\LicenseGenerate_Client\\license_File.zip";
	$files3 = glob($dir3.'*');
	foreach ($files3 as $file3) 
	if(@unlink($file3))
	{
		echo "Cleanup the File" .$file3."Success<br>";
	}
	else
	{
		echo "Cleanup the File" .$file3."Failed<br>";
	}

	header("Refresh:0 ;url=licensegenerate.php");
    exit();
?>

</body>
</html>
