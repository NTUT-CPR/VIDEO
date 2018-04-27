var express = require('express');
var app = express();
var delay = require('delay');
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
var SourceHeight;
var Filename_Queue_arr=[];
//var queue = require('../');
//var q = queue();
var QP_Queue_arr = [];
var Filenameorg_Queue_arr = [];
var sleep = require('sleep');
var tag=0;
var t1=0;
var t2=0;
var index=0;
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
	
	var GOP_Index = 0;
	setTimeout(function(){
		var fs = require('fs');			
		var cfgName = "/home/lab1323/桌面/NFS_Server/videocfg/"+Filename+".cfg";
		var cfgContent = 
				"InputFile           : "+"/mnt/nfs/video/"+Filename+".yuv"+"\n"+ 					"InputBitDepth       : 8\n"+
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
	GOP_Index = Math.ceil(parseInt(FrameToBeEncode,10)/9);
	console.log(FrameToBeEncode);
	console.log(GOP_Index);
	for(var i=0;i<GOP_Index;i++){
	var GOP_Filename = Filename+i.toString();
	Filenameorg_Queue_arr.push(Filename);
	Filename_Queue_arr.push(GOP_Filename);
	QP_Queue_arr.push(QP);
	/*var callfile = require('child_process');
	callfile.execFile('/home/lab1323/桌面/TranscodingApi/decodeFile.sh',['-V', GOP_Filename, '-Q', QP],null,function (err, stdout, stderr) {
		if(stderr.length>1)
			console.log("err");
		else
			console.log("succeed");
	});*/
}
	},3000);
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

var mqtt = require('mqtt');
	var server = mqtt.connect('mqtt://localhost');
	
	console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
	server.on('connect',function()
	{
		server.subscribe('vm1_state_response');
		server.subscribe('vm2_state_response');
	});

	var finishFile = "";
	var Nowtask = "xxx";
	var NowtaskQP = "xxx";
function getData(){
	console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
	console.log('===================================================');
	if(Filename_Queue_arr.length != 0)
	{
		if(index==0)
		{
			t1 = new Date().getTime();
			index=1;
		}
		Nowtask = Filename_Queue_arr[0];
		NowtaskQP = QP_Queue_arr[0];
	
	finishFile = '/home/lab1323/桌面/TranscodingApi/'+Nowtask;
	finishFile = finishFile+'_'+NowtaskQP+'_finish.txt';
	if(!fsExistsSync(finishFile)){
		var callfile = require('child_process');
	callfile.execFile('/home/lab1323/桌面/TranscodingApi/decodeFile.sh',['-V', Nowtask, '-Q', NowtaskQP],null,function (err, stdout, stderr) {
		if(stderr.length>1)
			console.log("err");
		else
			console.log("succeed");
	});
	}
}

	if(Filename_Queue_arr.length!=0 && fsExistsSync(finishFile)){
		server.publish('vm1_state','sned1');
		console.log("publish_vm1_state");
		if(tag==1)
		{
			if(Filename_Queue_arr.length != 0)
			{
				Nowtask = Filename_Queue_arr[0];
				NowtaskQP = QP_Queue_arr[0];
			}
			finishFile = '/home/lab1323/桌面/TranscodingApi/'+Nowtask;
			finishFile = finishFile+'_'+NowtaskQP+'_finish.txt';
			//if(Filename_Queue_arr.length!=0 && fsExistsSync(finishFile)){
			//server.publish('vm2_state','sned2');
			//console.log("publish_vm2_state");
			//}
		}
	}
}

setInterval(getData,5000);

	server.on('message',function(topic, message){
		//console.log(message.toString());		
		if(topic=='vm1_state_response')//check VM1 state
		{
			console.log('vm1 state_response');
			console.log(message.toString());
			if(message.toString() == "finish" && (Filename_Queue_arr.length!=0) && fsExistsSync(finishFile))
			{

				var callfile = require('child_process');
				callfile.execFile('/home/lab1323/桌面/TranscodingApi/deleteDecodeFinish.sh',['-V', Nowtask, '-Q', NowtaskQP],null,function (err, stdout, stderr){
					if(stderr.length>1)
						console.log("err");
					else
						console.log("vm1 delete succeed!");
				});
				console.log('vm1 start Working!');
				server.publish('videoorg1',Filenameorg_Queue_arr[0]);
				server.publish('video1',Filename_Queue_arr[0]);
				server.publish('QP1',QP_Queue_arr[0]);
				console.log(Filename_Queue_arr[0]);
				console.log("123");
				console.log("test:"+QP_Queue_arr[0]);
				Filenameorg_Queue_arr.shift();
				Filename_Queue_arr.shift();
				QP_Queue_arr.shift();
				if(Filename_Queue_arr.length==0)
				{
					t2 = new Date().getTime();
					var time = t2-t1;
					console.log("time: "+time);
				}
				tag=0;
				//console.log("TTTTTTTTTTTTT    "+Filename_Queue_arr.length);
			}
			else if(message.toString() == "working1")
			{
				console.log('vm1 working waiting!');
				console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
				tag=1;
			}
		}

if(Filename_Queue_arr.length != 0)

	{
		Nowtask = Filename_Queue_arr[0];
		NowtaskQP = QP_Queue_arr[0];
	
	finishFile = '/home/lab1323/桌面/TranscodingApi/'+Nowtask;
	finishFile = finishFile+'_'+NowtaskQP+'_finish.txt';
	if(!fsExistsSync(finishFile)){
		var callfile = require('child_process');
	callfile.execFile('/home/lab1323/桌面/TranscodingApi/decodeFile.sh',['-V', Nowtask, '-Q', NowtaskQP],null,function (err, stdout, stderr) {
		if(stderr.length>1)
			console.log("err");
		else
			console.log("succeed");
	});
	}
}

			if(topic=='vm2_state_response')//if VM1 working, check VM2 state
			{
				console.log('vm2 state_response');		
				if(message.toString() == "finish" && (Filename_Queue_arr.length!=0) && fsExistsSync(finishFile))
				{
					var callfile = require('child_process');
					callfile.execFile('/home/lab1323/桌面/TranscodingApi/deleteDecodeFinish.sh',['-V', Nowtask, '-Q', NowtaskQP],null,function (err, stdout, stderr){
					if(stderr.length>1)
						console.log("err");
					else
						console.log("vm2 delete succeed!");
					});
					console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
					console.log('vm2 start Working!');
					server.publish('videoorg2',Filenameorg_Queue_arr[0]);
					server.publish('video2',Filename_Queue_arr[0]);
					server.publish('QP2',QP_Queue_arr[0]);
					Filenameorg_Queue_arr.shift();
					Filename_Queue_arr.shift();
					QP_Queue_arr.shift();
					if(Filename_Queue_arr.length==0)
					{
					t2 = new Date().getTime();
					var time = t2-t1;
					console.log("time: "+time);
					}
				}
				else if(message.toString() == "working2")
				{
					console.log('vm2 working waiting!');
					console.log("Queue waiting Sequence: "+Filename_Queue_arr.length);
					//console.log(q.pop());
				}
			}
	});


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
