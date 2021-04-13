<?php 
 $connect = mysqli_connect("localhost", "root", "", "hetl");   
 $result = mysqli_query($connect, "select unit, sum(unitsold) as units_total from orders group by unit"); 
 ?> 

 <!DOCTYPE html> 
 <html> 
    <head>
            <title>Web DB Excercise</title>
      </head>
      <body>
           <div style="width:900px;"> 
                <h3 align="center">Pie Chart using Google Chart API with PHP MySQL</h3>
                <h4 align="center">(Peter Greene - CSCI440 – 2/17/2021)</h4>
                <br /> 
                <div id="piechart" style="width: 900px; height: 500px;"></div> 
           </div> 
           <br /><br /> 
           <script type = "text/javascript" src="https://www.gstatic.com/charts/loader.js"></script> 
           <script type="text/javascript"> 
           google.charts.load('current', {'packages':['corechart']}); 
           google.charts.setOnLoadCallback(drawChart); 

           function drawChart() 
           { 
                var data = google.visualization.arrayToDataTable([ 
                          ['unit', 'units_total'], 
                          <?php 
                          while($row = mysqli_fetch_array($result)) 
                          { 
                               echo "['".$row["unit"]."', ".$row["units_total"]."],"; 
                          } 
                          ?> 
                     ]); 
                var options = {title: 'Units Sold',
				               is3D: true,}; 
                var chart = new google.visualization.PieChart(document.getElementById('piechart')); 
                chart.draw(data, options); 
           } 
           </script>   
      </body> 
 </html>