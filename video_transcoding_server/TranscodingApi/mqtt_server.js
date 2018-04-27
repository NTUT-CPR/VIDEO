var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://localhost');

client.on('connect',function(){
	client.subscribe('#');
	client.publish('work','wroking');
});

client.on('message',function(topic, message){
	console.log(message.toString());
});
