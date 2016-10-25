var express = require('express');
var app = express();
var bt = require('../Bluetooth/main.js')

bt.setup_bt();

app.use('/', express.static('www'));
app.get('/', function (req, res) {
  console.log('Main page requested');
});

app.get('/0', function(req, res) {
        console.log('Turned off');
        bt.send(bt.low);
        res.sendFile(__dirname +'/www/index.html');
})
app.get('/1', function(req, res) {
        console.log('Turned on');
        bt.send(bt.high);
        res.sendFile(__dirname +'/www/index.html');
})
app.get('/2', function(req, res) {
        console.log('Turned auto mode');
        bt.send(bt.auto);
        res.sendFile(__dirname +'/www/index.html');
})



app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
