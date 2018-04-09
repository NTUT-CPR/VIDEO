var mqtt = require('mqtt');
var opt = {
	port:1883,
	client:'nodejs'
};
var video1="";
var QP1="";

var client = mqtt.connect('mqtt://192.168.0.106', opt);

client.on('connect', function(){
	console.log('is connected');
	client.subscribe('video1');
	client.subscribe('QP1');
});

client.on('message',function (topic,msg){
	
	if(topic == "video1"){
	video1 = msg.toString()+".cfg";
	console.log(video1);
	}
       else if(topic == "QP1"){
	QP1 = msg.toString()
	console.log(QP1);
	}
	
	var callfile = require('child_process');

	callfile.execFile('/home/lab1323/桌面/mqtt/transcoding.sh',['-V', video1, '-Q', QP1],null,function (err, stdout, stderr) {
	console.log(stdout);
	if(stderr.length>1)
	console.log("err");
	else
	console.log("succeed");
	    //callback(err, stdout, stderr);
	});	

	//console.log('topic: ' + topic + '\nmessage: ' + msg.toString());
	//if(msg.toString() == "working")
	//	client.publish('#','isDone');  
});
