var mqtt = require('mqtt');
var net = require('net');
var opt = {
	port:1883,
	client:'vm1'
};
var delay = require('delay');
var first_state = "0";//first sned task state

var client = mqtt.connect('mqtt://192.168.0.106', opt);


client.on('connect',function(){
	client.subscribe('vm1_state');
});


	client.on('message',function (topic,msg){
	var fs = require('fs');
	fs.stat('/home/lab1323/桌面/transcodingFile/finish.txt',function(err, stat){
	var str = msg.toString();
	if(topic == "vm1_state"){
	//console.log(topic);
	if(first_state=="0"){ //first sned task state?
		client.publish('vm1_state_response','finish');
		console.log("First_send");
		first_state="1";
	}
	else if(!(stat&&stat.isFile())){//finish.txt file no exist,the task working
			client.publish('vm1_state_response','working1');
			console.log(msg.toString()+" no exist");	
		}

	else if(stat&&stat.isFile()){//finish.txt file exist,the task 	finish
		//client.publish('vm1_state','finish');
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
		//delay(2000);
		client.publish('vm1_state_response','finish');
		console.log(msg.toString()+" exist");		
		}
	}
	str="";
	});});

/*function getState(){
	var fs = require('fs');
	fs.stat('/home/lab1323/桌面/transcodingFile/finish.txt',function(err, stat){
		var socket_client = new net.Socket();
		if(!(stat&&stat.isFile())&&first_state=="1")
		{
			socket_client.connect(3216, '127.0.0.1', function() {
			console.log('Connected');
			socket_client.write('working\n');
			socket_client.write('transcode');
			socket_client.destroy(); // kill client after server's response
			//socket_client.disconnect();
 			});
		}
		else if(stat&&stat.isFile())
		{
			socket_client.connect(3216, '127.0.0.1', function() {
			console.log('Connected');
			socket_client.write('no_work\n');
			socket_client.write('no_job');
			socket_client.destroy(); // kill client after server's response
			//socket_client.disconnect();
			});		
		}
	});
	
}

setInterval(getState,10000);*/


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
