<?php
  $con = @mysqli_connect('localhost','scorekeeper','derbs','derby');
  if(!$con){
    echo "err";
  }
  if($result = mysqli_query($con,"SELECT * FROM gameTBL")){
     while($row=mysqli_fetch_array($result)){
	     echo("<table border=1 style=\"font-size:50px\">");
	     echo("<tr><td>Period Clock</td><td>".$row['periodTime']."</td></tr>");
	     echo("<tr><td>Jam Clock</td><td>".$row['jamTime']."</td></tr>");
	     echo("<tr><td>".$row['whiteName']."</td><td>".$row['whiteScore']."</td></tr>");
	     echo("<tr><td>".$row['blackName']."</td><td>".$row['blackScore']."</td></tr>");
	    echo("</table>");
    }
    mysqli_free_result($result);
  }	  
  mysqli_close($con);
?>
