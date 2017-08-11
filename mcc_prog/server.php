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
	<style type="text/css">
		table{
			border: 3px solid #dc0;
		}
	</style>

	<script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.6.1/jquery.min.js"></script>

	<script type="text/javascript">
		function mode() {
	
		$.ajax({
		url: 'text.txt',
        success: function(data) {
       	arr_values = data.split (';') ;
			$('#val_1').html(arr_values[0]);
			$('#val_2').html(arr_values[1]);
			$('#val_3').html(arr_values[2]);
			$('#val_4').html(arr_values[3]);
			$('#val_5').html(arr_values[4]);
			$('#val_6').html(arr_values[5]);
			$('#val_7').html(arr_values[6]);
			$('#val_8').html(arr_values[7]);
			}
			});
		}
		var timeInterval = 300;
		setInterval(mode, timeInterval);
	</script>
	</head>
	
<body>
	<table align='center' border=1>
		<tr>
			<td>Канал 0</td>
			<td>Канал 1</td>
			<td>Канал 2</td>
			<td>Канал 3</td>
			<td>Канал 4</td>
			<td>Канал 5</td>
			<td>Канал 6</td>
			<td>Канал 7</td>
		</tr>
		<tr>
			<td id='val_1'></td>
			<td id='val_2'></td>
			<td id='val_3'></td>
			<td id='val_4'></td>
			<td id='val_5'></td>
			<td id='val_6'></td>
			<td id='val_7'></td>
			<td id='val_8'></td>
		</tr>
	</table>
	<center><form action="index.php" method="post">
		<p>Frequency: <input type="text" size="10" name="freq"></p>
		<p>Number of samples: <input type="text" size="5" name="number"></p>
		<p>Range: <select size="4" name="range">
			<option value="0">+/-10 V</option>
			<option value="1">+/-5 V</option>
			<option value="2">+/-2 V</option>
			<option value="3">+/-1 V</option>
		</select></p>
		<p><input type="submit" value="Submit"> 
		<input type="reset" value="Clear"></p>
	</form></center>
</body>
</html>

<?php
$freq = htmlspecialchars($_POST['freq']);
$number = htmlspecialchars($_POST['number']);
$range = htmlspecialchars($_POST['range']);

$was_read = 0;

$file = "settings.php";
$data = "$freq\n$number\n$range\n$was_read";
file_put_contents($file, $data);
?>
