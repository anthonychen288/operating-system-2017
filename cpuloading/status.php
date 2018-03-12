<html>
	<?php
	ini_set('display_errors', 1);
	ini_set('display_startup_errors', 1);
	error_reporting(E_ALL);
	?>
	<head>
		<meta http-equiv = "content-type" content="text/html ;charset = utf-8"/>
		<title>VM Loading Controller</title>
		<!--load google charts-->
		<script type="text/javascript" src="https:https://www.gstatic.com/charts/loader.js"><script/>
		<script type="text/javascript">
		// load(version_name, chart type to display)
		google.charts.load('current', {packages: ['corechart']});
		// Set a callback to run when the Google Visualization API is loaded.
		google.charts.setOnLoadCallback(drawChart);
		
		function drawChart(){
			var data = new google.visualization.DataTable();
			data.addColumn('number', 'time');
			data.addColumn('number', 'master');
			data.addColumn('number', 'slave1');
			data.addColumn('number', 'slave2');
			data.addRows([0, 0, 0, 0]);
		}
		var options = {'title':'cpu loading chart'};
		var chart = new google.visualization.LineChart(document.getElementById('chart'));
		chart.draw(data, options);
		/*
		function do_load()
		{
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatchange = function(){
				if(this.readyState == 4 && this.status == 200)
				{
					;
				}
			};
			var i = document.getElementById("ip").value;
			var l = document.getElementById("limit").value;
			var t = document.getElementById("time").value;
			xhttp.open("GET", "do_load.php?ip="+i+"&limit="+l+"&time="+t, true);
			xhttp.send();
			
			document.getElementById("ip").value = "";
			document.getElementById("limit").value = "";
			document.getElementById("time").value = "";
		}
		function do_update(str){
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatchange = function(){
				if(this.readyState == 4 && this.status == 200)
				{
					document.getElementById(str).innerHTML = this.responseText;
				}
			};
			xhttp.open("GET", "do_update.php?vm="+str, true);
			xhttp.send();
		}*/
		setInterval(do_update, 1000, "v1");
		setInterval(do_update, 1000, "v2");
	</script>
	</head>
	<body>
		<h1>VM Loading Controller</h1>
		<form name = "form">
			IP address:<input type = "text" id="ip"/> <br><br>
			CPU Loading:<input type="text" id="limit"/> <br><br>
			Execution Time:<input type="text" id="time"/> <br><br>
			<button type="button" onclick="do_load()">generate</button>
		</form>
		<p id="message"></p>
		<h3>VM list</h3>
		<table border="1">
			<thead>
				<tr>
					<td>ID</td>
					<td>Name</td>
					<td>IP address</td>
					<td>State</td>
					<td>CPU loading (%)</td>
				</tr>
			</thead>
			<tbody >
				<tr>
					<td>1</td>
					<td>centos7.0</td>
					<td>192.168.122.113</td>
					<td>running</td>
					<td id="v1">0</td>
				</tr>
				<tr>
					<td>2</td>
					<td>centos7.0-2</td>
					<td>192.168.122.65</td>
					<td>running</td>
					<td id="v2">0</td>
				</tr>
			</tbody>
		</table>
	</body>
</html>