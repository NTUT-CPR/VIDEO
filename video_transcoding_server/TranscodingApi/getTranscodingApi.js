var express = require('express');
var app = express();

app.listen(8888);
var Filename;
var Bitrate;
var QP;
var Resolution;
var VideoType;
var FrameRate;
var Duration;
var FrameToBeEncode;
var SourceWidth;
var SourceHeight
var Filename_Queue_arr=[];
//var queue = require('../');
//var q = queue();
var QP_Queue_arr = [];
var sleep = require('sleep');
//while(1){
//sleep.sleep(2000);


app.get('/myapi', function (req, res) {
	//console.log(req.query.videoinformation);
	var myJson = JSON.parse(req.query.videoinformation);
	for (var i in myJson) {
		console.log(i + " = " + myJson[i]);
		if(i="Filename")
		{
			Filename = myJson[i];
		}
		if(i="Bitrate")
		{
			Bitrate = myJson[i];
		}
		if(i="QP")
		{
			QP = myJson[i];
		}
		if(i="Resolution")
		{
			Resolution = myJson[i];
		}
		if(i="VideoType")
		{
			VideoType = myJson[i];
		}
	}
	/*var fs = require('fs');
	var cfgQP = Filename+"_qp.txt";
	fs.writeFile(cfgQP, QP, function(err){
			if(err){
			   console.log(err);		
			}else{
			   console.log("The file was saved!");		
		}
	});*/
	
	var http = require("http");
	var url = 'http://192.168.0.117:3000/VideoOrgInformation/'+Filename;
	http.get(url, function(response){
	  var data = '';
	  response.on('data',function(chunk){
	    data+=chunk;
 	  });
	  response.on('end', function(){
	    data = JSON.parse(data);
	
	for (var i in data) {
		//console.log(i + " = " + data[i]);
		if(i="FrameRate")
		{
			FrameRate = data[i];
		}
		if(i="FrameToBeEncode")
		{
			FrameToBeEncode = data[i];
		}
		if(i="Duration")
		{
			Duration = data[i];
			if(Duration=="360")
			{
                           	SourceWidth = 640;
				SourceHeight = 360;				
			}
			else if(Duration=="480")
			{
                           	SourceWidth = 854;
				SourceHeight = 480;				
			}
			else if(Duration=="720")
			{
                           	SourceWidth = 1280;
				SourceHeight = 720;				
			}
			else if(Duration=="1080")
			{
                           	SourceWidth = 1920;
				SourceHeight = 1080;				
			}
		}
	}
  	  });
	}).on('err', function(e){
	   console.log("error: ",e);
	});
	
	var callfile = require('child_process');
	callfile.execFile('/home/lab1323/桌面/TranscodingApi/decodeFile.sh',['-V', Filename],null,function (err, stdout, stderr) {
	if(stderr.length>1)
		console.log("err");
	else
		console.log("succeed");
	});

	setTimeout(function(){
	var fs = require('fs');
	var cfgName = "/home/lab1323/桌面/NFS_Server/videocfg/"+Filename+".cfg";
	var cfgContent = "InputFile           : "+"/mnt/nfs/video/"+Filename+".yuv"+"\n"+
                        "InputBitDepth       : 8\n"+
                        "InputChromaFormat   : 420\n"+
			   "FrameRate           : "+FrameRate+"\n"+
			   "FrameSkip           : 0\n"+
			   "SourceWidth         : "+SourceWidth+"\n"+
			   "SourceHeight        : "+SourceHeight+"\n"+
		          "FramesToBeEncoded   : "+FrameToBeEncode+"\n"+
			   "Level               : 4\n";
	fs.writeFile(cfgName, cfgContent, function(err){
		if(err){
		   console.log(err);		
		}else{
		   console.log("The file was saved!");		
		}
	});
	},3000);
	Filename_Queue_arr.push(Filename);
	QP_Queue_arr.push(QP);
	res.setHeader('Access-Control-Allow-Origin', '*');
	res.json({"foo":"bar"});
	res.end("successful");
});
//getData();
//while(true){
//function doing(){
var fs = require('fs');
function fsExistsSync(path) {
	try{
	  fs.accessSync(path,fs.F_OK);
	}catch(e){
	  console.log("false");
	  return false;	
	}
	console.log("true");
	return true;
}


function getData(){
	var mqtt = require('mqtt');
	var server = mqtt.connect('mqtt://localhost');
	
	console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
	server.subscribe('#');
	
	var finishFile = "";
	var Nowtask = "xxx";
	if(Filename_Queue_arr.length!=0)
	{
		Nowtask = Filename_Queue_arr[0];
	}
	finishFile = '/home/lab1323/桌面/TranscodingApi/'+Nowtask;
	finishFile = finishFile+'_finish.txt';

	if(Filename_Queue_arr.length!=0 && fsExistsSync(finishFile)){
	server.publish('vm1_state','sned1');
	//console.log(Filename_Queue_arr.length!=0);
	
	server.on('message',function(topic, message){
		console.log(message.toString());		
			//server.publish('vm1_state','sned');
			
			//sleep.sleep(2000);
		
		
		//console.log(finishFile);

		if(topic=='vm1_state')//check VM1 state
		{
			console.log('vm1 state');
			if(message.toString() == "finish" && (Filename_Queue_arr.length!=0) && fsExistsSync(finishFile))
			{
				
				var callfile = require('child_process');
				callfile.execFile('/home/lab1323/桌面/TranscodingApi/deleteDecodeFinish.sh',['-V', Nowtask],null,function (err, stdout, stderr){
	if(stderr.length>1)
	console.log("err");
	else
	console.log("delete succeed!");
});
				console.log('vm1 start Working!');
				server.publish('video1',Filename_Queue_arr[0]);
				server.publish('QP1',QP_Queue_arr[0]);
				console.log(Filename_Queue_arr[0]);
				console.log("123");
				console.log("test:"+QP_Queue_arr[0]);
				Filename_Queue_arr.shift();
				QP_Queue_arr.shift();
			}
			else if(message.toString() == "working1")
			{
				console.log('vm1 working waiting!');
				console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
				if(Filename_Queue_arr.length!=0 && fsExistsSync(finishFile)){//if VM1 working, check VM2 state 
	server.publish('vm2_state','sned2');
	//console.log(Filename_Queue_arr.length!=0);
	
	server.on('message',function(topic, message){
		console.log(message.toString());		
			//server.publish('vm1_state','sned');
			
			//sleep.sleep(2000);
		
		
		//console.log(finishFile);

		if(topic=='vm2_state')
		{
			console.log('vm2 state');		
			if(message.toString() == "finish" && (Filename_Queue_arr.length!=0) && fsExistsSync(finishFile))
			{
				var callfile = require('child_process');
				callfile.execFile('/home/lab1323/桌面/TranscodingApi/deleteDecodeFinish.sh',['-V', Nowtask],null,function (err, stdout, stderr){
	if(stderr.length>1)
	console.log("err");
	else
	console.log("delete succeed!");
});
				console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
				console.log('vm2 start Working!');
				server.publish('video2',Filename_Queue_arr[0]);
				server.publish('QP2',QP_Queue_arr[0]);
				Filename_Queue_arr.shift();
				QP_Queue_arr.shift();
			}
			else if(message.toString() == "working2")
			{
				console.log('vm2 working waiting!');
				console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
				//console.log(q.pop());
				
			}
		}
	});

	}
				
				//console.log(q.pop());
				
			}
		}	
	});
   }
	//
}
	setInterval(getData,5000);
//}

 //getData();
 //sleep.sleep(50000);
//}
//	
//input
//http://127.0.0.1:7777/myapi?videoinformation={"myip":"192.168.1.1","apple":"Macintosh","mic":"123"}

//output
/*
{"myip":"192.168.1.1","apple":"Macintosh","mic":"123"}
myip = 192.168.1.1
apple = Macintosh
mic = 123
*/
