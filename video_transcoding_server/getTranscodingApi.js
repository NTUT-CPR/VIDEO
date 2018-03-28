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
	var fs = require('fs');
	var cfgQP = Filename+"_qp.txt";
	fs.writeFile(cfgQP, QP, function(err){
			if(err){
			   console.log(err);		
			}else{
			   console.log("The file was saved!");		
		}
	});
	
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
	
	res.end("successful!");
});
//getData();
//while(true){
//function doing(){

function getData(){
	var mqtt = require('mqtt');
	var server = mqtt.connect('mqtt://localhost');
	
	console.log("     "+Filename_Queue_arr.length);
	server.subscribe('#');
	if(Filename_Queue_arr.length!=0){
	server.publish('vm1_state','sned');
	//console.log(Filename_Queue_arr.length!=0);
	
	server.on('message',function(topic, message){
		console.log(message.toString());		
			//server.publish('vm1_state','sned');
			
			//sleep.sleep(2000);
		if(topic=='vm1_state')
		{
			if(message.toString() == "finish" && (Filename_Queue_arr.length!=0))

			{
				console.log('vm start Working!');
				server.publish('video1',Filename_Queue_arr.pop());
				server.publish('QP1',QP_Queue_arr.pop());
			}
			else if(message.toString() == "working")
			{
				console.log('vm working waiting!');
				
				//console.log(q.pop());
				
			}
		}
		else{}
	});
	}
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
