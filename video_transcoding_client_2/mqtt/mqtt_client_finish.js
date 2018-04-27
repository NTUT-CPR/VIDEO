var mqtt = require('mqtt');
var opt = {
	port:1883,
	client:'vm2'
};

var first_state = "0";//first sned task state

var client = mqtt.connect('mqtt://192.168.0.106', opt);


client.on('connect',function(){
	client.subscribe('vm2_state');
});


	client.on('message',function (topic,msg){
	var fs = require('fs');
	fs.stat('/home/lab1323/桌面/transcodingFile/finish.txt',function(err, stat){
	var str = msg.toString();
	if(topic == "vm2_state"){
	//console.log(topic);
	if(first_state=="0"){ //first sned task state?
		client.publish('vm2_state_response','finish');
		console.log("VM2_First_send");
		first_state="1";
	}
	else if(!(stat&&stat.isFile())){//finish.txt file no exist,the task working
			client.publish('vm2_state_response','working2');
			console.log(msg.toString()+" no exist");		
		}

	else if(stat&&stat.isFile()){//finish.txt file exist,the task 	finish
		//client.publish('vm2_state','finish');
		console.log("finish");
		var callfile = require('child_process');
		callfile.execFile('/home/lab1323/桌面/mqtt/deleteFinish.sh',null,function (err, stdout, stderr) {
	console.log(stdout);
	if(stderr.length>1)
	console.log("err");
	else if(stderr.length>1)
	console.log("succeed");
	    //callback(err, stdout, stderr);
		
});
		client.publish('vm2_state_response','finish');
		console.log(msg.toString()+" exist");		
		}
	}
	str="";
	});});



/*function myLoop(){
	setTimeout(function(){
	var fs = require('fs');
	fs.stat('/home/lab1323/桌面/transcodingFile/finish.txt',function(err, stat){
	client.on('message',function (topic,msg){
	
	if(topic == "vm1_state"){
	//console.log(topic);
	if(!(stat&&stat.isFile())){
			//client.publish('vm1_state','working');
			console.log("not exist");		
		}

	else if(stat&&stat.isFile()){
		//client.publish('vm1_state','finish');
		console.log("exist");		
		}
	}
	});
		
		
	});
	myLoop();
	},2000);}
myLoop();*/
