var net = require('net');

var client = new net.Socket();
client.connect(3214, '127.0.0.1', function() {
	console.log('Connected');
	client.write('working\n');
	client.write('hhh');
	client.destroy(); // kill client after server's response
});

