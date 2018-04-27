var express = require('express');
var app = express();
var Duration="";
var LessonURL="";
var VideoID="";
var Category="";
var Quantization_Parameter="";
var Category="";
var FrameRate="";
var FrameToBeEncode="";
var str="";
var async = require('async');

app.listen(3000);

app.get('/video/:id', function (req, res) {
	var id = req.params.id;
 	var mysql = require('mysql');
	var connection = mysql.createConnection({
        host : '127.0.0.1',
        user : 'root',
        password : 'lab1323',
        database : 'Video'
});
console.log(id);
var sql = 'SELECT * FROM `SegmentVideo` WHERE VID = ?';
	connection.query(sql,[id],
        function(error, rows, fields){
                if(error)
                {
                        throw error;
                }
                for(key in rows)
                {
                        console.log(rows[key].Duration);
			VideoID = rows[key].VideoID.toString();
			Category = rows[key].Category.toString();
			Quantization_Parameter = rows[key].Quantization_Parameter.toString();
			Duration = rows[key].Duration.toString();
			LessonURL = rows[key].LessonURL.toString();
                }
        });
	
	setTimeout(function(){
	console.log(LessonURL);
	var arr = {VideoID:VideoID,Category:Category,Quantization_Parameter:Quantization_Parameter,Duration:Duration,LessonURL:LessonURL};
        str = JSON.stringify(arr);},1000);

	setTimeout(function(){
	res.write(str);
	res.end();},2000);
});


app.get('/videoAll', function (req, res) {
	var id = req.params.id;
 	var mysql = require('mysql');
	var connection = mysql.createConnection({
        host : '127.0.0.1',
        user : 'root',
        password : 'lab1323',
        database : 'Video'
});
console.log(id);
var sql = 'SELECT * FROM `SegmentVideo`';
	connection.query(sql,
        function(error, rows, fields){
                if(error)
                {
                        throw error;
                }
		str = "";
                for(key in rows)
                {
                        console.log(rows[key].Duration);
			VideoID = rows[key].VideoID.toString();
			Category = rows[key].Category.toString();
			Quantization_Parameter = rows[key].Quantization_Parameter.toString();
			Duration = rows[key].Duration.toString();
			LessonURL = rows[key].LessonURL.toString();
			var arr = {VideoID:VideoID,Category:Category,Quantization_Parameter:Quantization_Parameter,Duration:Duration, LessonURL:LessonURL};
			str += JSON.stringify(arr)+"\n";
                }
        });

	setTimeout(function(){
	res.write(str);
	res.end();},2000);
});

app.get('/videoOrgInformation/:id', function (req, res) {
	var id = req.params.id;
 	var mysql = require('mysql');
	var connection = mysql.createConnection({
        host : '127.0.0.1',
        user : 'root',
        password : 'lab1323',
        database : 'Video'
});
console.log(id);
var sql = 'SELECT * FROM `VideoOrgInformation` WHERE VideoID = ?';
	connection.query(sql,[id],
        function(error, rows, fields){
                if(error)
                {
                        throw error;
                }
		str = "";
                for(key in rows)
                {
                        console.log(rows[key].Duration);
			VideoID = rows[key].VideoID.toString();
			Category = rows[key].Category.toString();
			Duration = rows[key].Duration.toString();
			FrameRate = rows[key].FrameRate.toString();
			FrameToBeEncode = rows[key].FrameToBeEncode.toString();
                }
        });
	
	setTimeout(function(){
	console.log(LessonURL);
	var arr = {VideoID:VideoID,Category:Category,Duration:Duration,FrameRate:FrameRate,FrameToBeEncode:FrameToBeEncode};
        str = JSON.stringify(arr);},1000);

	setTimeout(function(){
	res.write(str);
	res.end();},2000);
});

/*var mysql = require('mysql');
var connection = mysql.createConnection({
        host : '127.0.0.1',
        user : 'root',
        password : 'lab1323',
        database : 'Video'
});
connection.query('select VideoID from SegmentVideo', 
        function(error, rows, fields){
                if(error)
                {
                        throw error;
                }
                for(key in rows)
                {
                        console.log(rows[key]);
                }
        });*/
