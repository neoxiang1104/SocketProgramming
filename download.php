<?php
	$file = 'C:\\LicenseGenerate_Client\\license_File.zip';
	if (file_exists($file))
	{
		header('Pragma: public');
		header('Expires: 0');
		header('Content-Description: File Transfer');
		header('Content-Type: application/force-download');
		header('Content-Disposition: attachment; filename="' . basename($file) . '"');
		header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
		header('Content-Length: ' . filesize($file));
		header('Content-Transfer-Encoding: binary');
		header('Connection: close');
		readfile($file);
		fpassthru($file);
		fclose($file);
		exit;
	}
	
	else
	{
		echo "<script>alert('Cannot Find the ZIP File');</script>";
	}
	
	header("Refresh:0 ;url=licensegenerate.php");
	ob_clean();
    flush();
?>


