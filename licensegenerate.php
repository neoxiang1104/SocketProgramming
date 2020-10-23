<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>License Generator</title>
</head>
<body style="background-color:#4682B4;">

	<?php
		date_default_timezone_set('Asia/Taipei');
		$queryNow = date("Y-m-d H:i:s");
	?>
	
	<div id="date">Updated Time: <?php echo $queryNow;?></div><br>
	<div style="text-align:center; color:#FFFFFF; font-family:fantasy; font-size:36px">License Generator</div>
	<br><br><br><br><br>
    
	<!-- File Upload -->
	<div style="border-width:3px;border-style:outset;border-color:#FFFFFF;padding:5px;">
	<form action="fileupload.php" method="post" enctype="multipart/form-data">
      <input type="hidden" name=todo value='upload'>                                               
      <div style="color:#FFFFFF"> File Select：<input type="file" name="uploadfile[]" style="display: block; margin-bottom: 5px;" multiple></br>
      <input type="submit" value="Upload"></div>
    </form>
	<div style="color:#FFFFFF"> - Upload Activate Code file</div>
	<div style="color:#FFFFFF"> - Support upload multiple file</div>
	</div>
	<br><br><br>
    
	<!-- License Generator -->
	
	<div style="border-width:3px;border-style:outset;border-color:#FFFFFF;padding:5px;">
	<div style="color:#FFFFFF"; id="startprocess">
		<input type="button" value="Start Processing" style="height: 30px; width: 180px" onclick="LicenseGenerate()"><br>
		<div style="color:#FFFFFF">  - Click "Start Processing" to generate License file</div>
		
		<button onclick="StopFunction()">Stop Processing</button>
		<div style="color:#FFFFFF">  - If processing has no respond, click to stop</div>
	</div>
	</div>
	<br><br><br>
    
	<!-- Download File -->
	<div style="border-width:3px;border-style:outset;border-color:#FFFFFF;padding:5px;">
	<div style="color:#FFFFFF"; id="startprocess">
		<input type="button" value="Download License file" style="height: 30px; width: 180px" onclick="download()"><br>
		<div style="color:#FFFFFF">  - Click button can download the License file, after finished license generate</div>
		<div style="color:#FFFFFF">  - License file will compression into ZIP file</div>
	</div>
	</div>
	<br><br><br>
	
	<!-- Cleanup License Folder -->
	<div style="border-width:3px;border-style:outset;border-color:#FFFFFF;padding:5px;">
	<div style="color:#FFFFFF"; id="startprocess">
		<input type="button" value="Clean Folder" style="height: 30px; width: 180px" onclick="cleanup()"><br>
		<div style="color:#FFFFFF">  - Click button to clean up the license file folder</div>
		<div style="color:#FFFFFF">  - If there is upload file issue, also can cleanup all file and restart</div>
	</div>
	</div>

<script type="text/javascript">

    function LicenseGenerate()
	{
		location.href = "run.php";
		return 0; 
	}
	function StopFunction() 
	{
		location.href = "stop.php";
		alert("Click 'Clean Folder' and re-start process");
		return 0; 
    }
	
	function download()
	{
		location.href = "download.php";
		return 0; 
	}
	function cleanup()
	{
		location.href = "cleanup.php";
		return 0; 
	}
</script>

</body>
</html>
