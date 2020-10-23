<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>License Generator</title>
</head>
<body style="background-color:#4682B4;">
<?php
    ob_start();
	error_reporting(E_ALL);
    ini_set('display_errors',1);
	
	set_time_limit (0);
	$max_file_size = 1000000;
    @$todo=$_POST['todo'];
    $uploaddir = 'C:\\LicenseGenerate_Client\\0_Activate_Code\\';
	
	while(@list($key,$value) = each($_FILES['uploadfile']['name']))
    {
		$file_name=$value;
		$ActivateCode_File = $uploaddir.$file_name;
		//echo "File: $ActivateCode_File <br>";
		//echo '<br>';
		
        if(copy($_FILES['uploadfile']['tmp_name'][$key], $ActivateCode_File))//upload the file to the server
		{
            //echo 'print_r($_FILES):';
            //print_r($_FILES);
        
            //echo '<br>';
			echo "<script>alert('Upload Success');</script>";
		}
		
		else
		{
			echo "<script>alert('Upload Failed');</script>";
		}
	}
	header("Refresh:0 ;url=licensegenerate.php");
    ob_end_flush();
?>

</body>
</html>