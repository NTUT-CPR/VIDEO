var mqtt = require('mqtt');
var opt = {
	port:1883,
	client:'nodejs'
};
var video1="";
var QP1="";
var videoorg1="";
var tag=0;

var client = mqtt.connect('mqtt://192.168.0.106', opt);

client.on('connect', function(){
	console.log('is connected');
	client.subscribe('videoorg1');
	client.subscribe('video1');
	client.subscribe('QP1');
});

function loop(){
	if(tag==1)
	{
	tag=0;
		var callfile = require('child_process');
			callfile.execFile('/home/lab1323/桌面/mqtt/transcoding.sh',['-O',videoorg1,'-V',video1,'-Q',QP1],null, function (err, stdout, stderr) {
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
	
	if(topic == "videoorg1"){
	videoorg1 = msg.toString();
	console.log(videoorg1);
	}
	else if(topic == "video1"){
	video1 = msg.toString();
	console.log(video1);
	}
   	else if(topic == "QP1"){
	QP1 = msg.toString();
	console.log(QP1);
	tag=1;
	}
	loop();
	//console.log('topic: ' + topic + '\nmessage: ' + msg.toString());
	//if(msg.toString() == "working")
	//	client.publish('#','isDone');
});

