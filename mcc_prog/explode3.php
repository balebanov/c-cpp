<?php
Header( 'Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0' );
Header("Pragma: no-cache");
Header("Last-Modified: " . gmdate("D, d M Y H:i:s") . "GMT");
header("Expires: " . date("r"));
?>
<!DOCTYPE html>
<html>
<meta http-equiv="Cache-Control" content="no-cache">
<head>

<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js"></script>

<script type="text/javascript">

function mode() {
	
    $.ajax({
        url: 'voltage.txt',
        success: function(data) {
       	arr_values = data.split (';') ;
			$('#val_1').html(arr_values[0]);
			$('#val_2').html(arr_values[1]);
			$('#val_3').html(arr_values[2]);
			$('#val_4').html(arr_values[3]);
        }
    });
	}
	var timeInterval = 300;
	setInterval(mode, timeInterval);

</script>
</head>
	
<body>
<table border='1'>
	<tr>
		<td>Канал 0</td>
		<td>Канал 1</td>
		<td>Канал 2</td>
		<td>Канал 3</td>
	</tr>
	<tr>
		<td id='val_1'></td>
		<td id='val_2'></td>
		<td id='val_3'></td>
		<td id='val_4'></td>
	</tr>
</table>
</body>
</html>
