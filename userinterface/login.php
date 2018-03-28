<?php	
	session_start();
	$servername = "localhost";
        $username = "root";
        $password = "106598041";
        $dbname = "cloudUser";
        $conn = new mysqli($servername, $username, $password, $dbname);

        if($conn->connect_error){
                die("Connection error:" . $conn->connect_error);
        }
        //echo "connect success~";

        $sql = "select * from user";
        $result = $conn->query($sql);

        if($result->num_rows > 0){
                while($row = $result->fetch_assoc()){
                        $arr[] = $row;
			if($row["account"] == $_POST["uname"]){
                        	if($row["password"] == $_POST["psw"]){
					$_SESSION['login'] = false;
					$_SESSION['logout'] = true;
					$_SESSION['timeout'] = time();
					$_SESSION['username'] =  $row["name"];
					header('Refresh:0; URL = index.php');
                        	}else{
					//echo "login failed";	
					header('Refresh:0; URL = loginfail.php');

				}
                	}else{
                        	 header('Refresh:0; URL = loginfail.php');//echo "login failed";
                	}
                }
                $json_respon = json_encode($arr,JSON_UNESCAPED_UNICODE);
                //echo $json_respon;
        }else{
                echo "0 result";
        }
        $conn->close();
?>
