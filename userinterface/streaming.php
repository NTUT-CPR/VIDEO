<?php
//start session
session_start();
?>
<?php
if( $_SESSION["username"] == "")
        $_SESSION["login"] = true;
?>

<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta charset="utf-8" />
    <title>player</title>
    <script src="js/dash.all.min.js"></script>
    <link href="https://unpkg.com/video.js@6.1.0/dist/video-js.css" rel="stylesheet">
    <script src="https://unpkg.com/video.js/dist/video.js"></script>
    <script src="https://unpkg.com/videojs-contrib-dash/dist/videojs-dash.js"></script>
    <script src="https://unpkg.com/silvermine-videojs-quality-selector/dist/js/silvermine-videojs-quality-selector.min.js"></script>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script src=js/home.js></script> 
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.4/angular.min.js"></script>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Karma">
     <!-- Font-awesome -->
    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet" integrity="sha384-wvfXpqpZZVQGK6TAh5PVlGOfQNHSoD2xbE+QkPxCAFlNEevoEH3Sl0sibVcOQVnN" crossorigin="anonymous">
    <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
    <script src="js/data.js"></script>
    <script>
         set_data_push({
			    'uv_box': '#show_uv',
			    'temperature_box': '#show_temperature',
			    'county_box': '#show_country',
			    'aqi_box': '#show_aqi'
			  });
        set_area_id(1);
        star_push();
    </script>
</head>
<style>
body,h1,h2,h3,h4,h5,h6,p {font-family: fantasy, "Microsoft JhengHei"}
.w3-bar-block .w3-bar-item {padding:20px}
</style>
<body ng-app="">

<div class="w3-top">
<ul class="topnav">
  <li class="left"><?php echo $_SESSION["username"] ?></li>
  <li class="left">紫外線指數:&nbsp;<span id="show_uv"></span></li>
  <li class="left">溫度:&nbsp;<span id="show_temperature"></span></li>
  <li class="left">城市:&nbsp;<span id="show_country"></span></li>
  <li class="left">空氣品質:&nbsp;<span id="show_aqi"></span></li>
  <li class="right">
    <button ng-if="<?php echo $_SESSION["login"] ?>" onclick="login()" style="width:auto">Login</button></a>
    <button ng-if="<?php echo $_SESSION["logout"] ?>" onclick="logout()" style="width:auto">Logout</button></a>
  </li>
</ul>
</div>

<div id="id01" class="modal">
  <form class="modal-content animate" action="login.php" method="post" >
    <div class="container">
          <h2>使用者登入</h2>
      <label><b>Username</b></label>
      <input type="text" placeholder="Enter Username" name="uname" required>

      <label><b>Password</b></label>
      <input type="password" placeholder="Enter Password" name="psw" required>

      <button type="submit">Login</button>
    </div>

    <div class="container" style="background-color:#f1f1f1">
      <button type="button" onclick="document.getElementById('id01').style.display='none'" class="cancelbtn">Cancel</button>
    </div>
  </form>
</div>


<div class="w3-main w3-content w3-padding" style="max-width:1200px;margin-top:100px">
    <video id="videoPlayer" class="video-js vjs-default-skin vjs-16-9 vjs-big-play-centered" controls data-setup='{}'></video>
</div>

   <script>
	 videojs('videoPlayer', {}, function(){
		var player = this;
		resizeManager:false;
		player.controlBar.addChild('QualitySelector');
		player.src([
			{
				src:'https://140.124.182.20:2222/dream/qp22/qp22_elephants_dream_1080p24.mpd',
				type: 'application/dash+xml',
				label: '1080P',
			},
			{
				src: 'https://140.124.182.20:2222/dream/qp27/qp27_elephants_dream_1080p24.mpd',
				type: 'application/dash+xml',
				label: '720P',

			},
			{
				src: 'https://140.124.182.20:2222/dream/qp32/qp32_elephants_dream_1080p24.mpd',
				type: 'application/dash+xml',
				label: '480P'
			},
			{
				src: 'https://140.124.182.20:2222/dream/qp37/qp37_elephants_dream_1080p24.mpd',
				type: 'application/dash+xml',
				label: '360P'
			},
			{
        src: 'https://140.124.182.20:2222/dream/elephants_dream_auto/auto_elephants_dream_1080p24.mpd',
        type: 'application/dash+xml',
        label: 'auto',
				selected: 'true'
      }

		]);
		player.play();
	});
    </script>
</body>
</html>
