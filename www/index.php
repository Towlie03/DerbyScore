<HTML>
  <HEAD>
    <script type="text/javascript" src="jquery-3.3.1.min.js"></script>
  </HEAD>

  <div id="stats" >
  </div>

  <script language="javascript" type="text/javascript">
    function loadStats(){
      $('#stats').load('getGameStats.php',function(){
        $(this).unwrap();
      });
    }

    loadStats();
    setInterval(function(){loadStats()},500);
  </script>
</HTML>
