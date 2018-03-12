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
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		<script type="text/javascript">
		// load(version_name, chart type to display)
		google.charts.load('current', {'packages':['line']});
		// Set a callback to run when the Google Visualization API is loaded.
		google.charts.setOnLoadCallback(drawChart);
		
		var state = [[-10, 0, 0, 0],
					[-9, 0, 0, 0],
					[-8, 0, 0, 0],
					[-7, 0, 0, 0],
					[-6, 0, 0, 0],
					[-5, 0, 0, 0],
					[-4, 0, 0, 0],
					[-3, 0, 0, 0],
					[-2, 0, 0, 0],
					[-1, 0, 0, 0],
					[0, 0, 0, 0]];
		var chart;
		var flag=0;
		
		function drawChart() {
			
			var data = new google.visualization.DataTable();
			data.addColumn('number', 'time');
			data.addColumn('number', 'Master');
			data.addColumn('number', 'slave1');
			data.addColumn('number', 'slave2');
			
			data.addRows(state);
			
			var options = {
				chart: {
					title: 'CPU loading',
					subtitle: 'Executiing Monte Carlo',
				},
				width: 800,
				height: 500,
				vAxis: {
					minValue:0,
					maxValue:100
				},
				hAxis:{
					format:'percent'
				}
			};
			chart = new google.charts.Line(document.getElementById('linechart'));
			chart.draw(data, options);
		}
		
		
		
		function do_update(){
			var xhr = new XMLHttpRequest();
			xhr.onreadystatechange = function(){
				if(this.readyState == 4 && this.status == 200)
				{
					// document.getElementById('message').innerHTML = this.responseText;
					state_update();
					state.splice(0, 1);
					state.splice(10, 0, xhr.responseText.split(" ").map(Number));
					drawChart();
				}
			};
			xhr.open("GET", "getstate.php", true);
			xhr.send();
		}
		function state_update(){
			var i = 0;
			for(i=0;i<=10;i++){
				state[i][0] = state[i][0] - 1;
			}
		}
		setInterval(do_update, 1000);
		</script>
	</head>
	<body>
		<h1>VM Loading</h1>
		<p id="message"></p>
		<div id="linechart"></div>
	</body>
</html>
