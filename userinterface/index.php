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
    <title>Player</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta charset="utf-8">
	  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	  <link rel="stylesheet" type="text/css" href="style.css">
	  <script src=js/home.js></script> 
	  <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.4/angular.min.js"></script>
	  <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Karma">
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

<div class="w3-container w3-padding-32" id="projects">
    <h1 class="w3-border-bottom w3-border-light-grey w3-padding-16">熱門強檔</h1>
</div>

  <div class="w3-row-padding w3-padding-16 w3-center" id="hot">
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/race8.jpg" alt="Sandwich" style="width:100%">
      <h3>玩命關頭8</h3>
      <p>一觸即發</p>
    </div>
   </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/wonder women.jpg" alt="Steak" style="width:100%">
      <h3>神力女超人</h3>
      <p>正義從她開始</p>
    </div>
   </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/logan.jpg" alt="Cherries" style="width:100%">
      <h3>羅根</h3>
      <p>英雄終章，最後結局</p>
    </div>
   </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/galaxy2.jpg" alt="Pasta and Wine" style="width:100%">
      <h3>星際異攻隊2</h3>
      <p>復仇者聯盟最狂隊友</p>
    </div>
   </a>
</div>

<div class="w3-container w3-padding-32" id="animation">
    <h1 class="w3-border-bottom w3-border-light-grey w3-padding-16">最強動畫</h1>
</div>

<div class="w3-row-padding w3-padding-16 w3-center" id="food">
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/yourname.jpg" alt="Sandwich" style="width:100%">
      <h3>你的名字</h3>
      <p>當我們相遇，一定能一眼認出彼此</p>
    </div>
   </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/frozne.jpg" alt="Steak" style="width:100%">
      <h3>冰雪奇緣</h3>
      <p>進入真愛與勇氣的冰雪世界</p>
    </div>
   </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/father3.jpg" alt="Cherries" style="width:100%">
      <h3>神偷奶爸3</h3>
      <p>黃色旋風再次來襲</p>
    </div>
  </a>
  <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/pokemon.jpg" alt="Pasta and Wine" style="width:100%">
      <h3>精靈寶可夢:就決定是你了</h3>
      <p>踏上冒險的旅程</p>
    </div>
  </a>
</div>

 <div class="w3-container w3-padding-32" id="funny">
    <h1 class="w3-border-bottom w3-border-light-grey w3-padding-16">幽默輕鬆</h1>
</div>

<div class="w3-row-padding w3-padding-16 w3-center" id="food">
  <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/kingsman.jpg" alt="Sandwich" style="width:100%">
      <h3>金牌特務</h3>
      <p>特務出擊</p>
    </div>
  </a>
  <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/nowyouseeme.jpg" alt="Steak" style="width:100%">
      <h3>出神入化2</h3>
      <p>上一次，你以為看清一切</p>
	  <p>這一次，要你大開眼界</p>
    </div>
  </a>
  <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/blackwizard.jpg" alt="Cherries" style="width:100%">
      <h3>黑魔女</h3>
      <p>無法想像的邪惡力量</p>
    </div>
  </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/3idons.jpg" alt="Pasta and Wine" style="width:100%">
      <h3>三個傻瓜</h3>
      <p>ALL IS WELL</p>
    </div>
   </a>
</div>

<div class="w3-container w3-padding-32" id="nervous">
    <h1 class="w3-border-bottom w3-border-light-grey w3-padding-16">緊張懸疑</h1>
</div>

<div class="w3-row-padding w3-padding-16 w3-center" id="food">
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/it.jpg" alt="Sandwich" style="width:100%">
      <h3>牠</h3>
      <p>直擊內心最深層的恐懼</p>
    </div>
  </a>
  <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/train.jpg" alt="Steak" style="width:100%">
      <h3>屍速列車</h3>
      <p>屍裡逃生</p>
    </div>
  </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/donottrust.jpg" alt="Cherries" style="width:100%">
      <h3>別相信任何人</h3>
      <p>你敢相信誰</p>
    </div>
  </a>
   <a href="streaming.php">
    <div class="w3-quarter">
      <img src="image/split.jpg" alt="Pasta and Wine" style="width:100%">
      <h3>分裂</h3>
      <p>難以預料</p>
    </div>
  </a>
</div>
</div>

</body>
</html>

