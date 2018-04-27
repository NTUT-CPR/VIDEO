var mqtt = require('mqtt');
var opt = {
	port:1883,
	client:'nodejs'
};
var videoorg2="";
var video2="";
var QP2="";
var tag=0;

var client = mqtt.connect('mqtt://192.168.0.106', opt);

client.on('connect', function(){
	console.log('is connected');
	client.subscribe('videoorg2');
	client.subscribe('video2');
	client.subscribe('QP2');
});

function loop(){
	if(tag==1)
	{
	tag=0;
		var callfile = require('child_process');
			callfile.execFile('/home/lab1323/桌面/mqtt/transcoding.sh',['-O',videoorg2,'-V',video2,'-Q',QP2],null, function (err, stdout, stderr) {
			console.log(stdout);
			if(stderr.length>1)
			console.log("err");
			else
			console.log("succeed");
			    //callback(err, stdout, stderr);
			});
	}
}

client.on('message',function (topic,msg){
	
	if(topic == "videoorg2"){
	videoorg2 = msg.toString();
	console.log(videoorg2);
	}	
	else if(topic == "video2"){
	video2 = msg.toString();
	console.log(video2);
	}
   	else if(topic == "QP2"){
	QP2 = msg.toString();
	console.log(QP2);
	tag=1;
	}
	loop();
	//console.log('topic: ' + topic + '\nmessage: ' + msg.toString());
	//if(msg.toString() == "working")
	//	client.publish('#','isDone');
});

