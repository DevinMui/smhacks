var express = require("express")
var bodyParser = require("body-parser")
var Bot = require("slackbots")
var mongoose = require("mongoose")
var morse = require('morse')
var secret = require('secret.js')
// var ToneAnalyzerV3 = require('watson-developer-cloud/tone-analyzer/v3');

var app = express()

mongoose.connect('mongodb://localhost/smhacks');

// var tone_analyzer = new ToneAnalyzerV3({
//   username: secret.username,
//   password: secret.password,
//   version_date: '2016-05-19'
// });

var messagesSchema = mongoose.Schema({
	groupId: String,
	text: String,
	user: String,
	color: String,
	read: false,
	tone: String
},{
	timestamps: true
});

var groupsSchema = mongoose.Schema({
	groupId: String,
	color: String
})

var Group = mongoose.model('Group', groupsSchema)
var Message = mongoose.model('Message', messagesSchema);

var settings = {
    token: 'xoxb-127038972225-O613Qzf7g692j9BBsh2JF9Er',
    name: 'smart-watch'
};
var bot = new Bot(settings);

bot.on('start', function() {
	// bot.postMessageToGroup('particle-smart-watch', 'hello group chat!');
});

bot.on('message', function(data){
    // data.channel, data.text, data.user
	// tone_analyzer.tone({ text: data.text }, // get tone of text
	// 	function(err, tone) {
	// 	    if (err)
	// 	    	console.log(err);
	// 	    else
	// 	      	console.log(JSON.stringify(tone, null, 2));
	// });
	if(data.text){
		console.log(data)
		Group.findById(data.channel, function(err, group){
			if(!group){ // if group doesnt exist
				new Group({
					groupId: data.channel,
					color: "red"
				}).save(function(err, group){
					new Message({
						groupId: group.groupId,
						text: data.text,
						user: data.user,
						color: group.color
					}).save()
				})
			} else {
				new Message({
					groupId: group.groupId,
					text: data.text,
					user: data.user,
					color: group.color
				}).save()
			}
		})
	}
})

// send random meme to group
app.get('/send_meme', function(req, res){
	// var groupId = req.param.group
	Message.findOne({}, { sort: {'created_at': -1}}, function(err, data){
		if(err) throw err
		postMessage(groupId, "meme", function(){
			res.send("k")
		})
	})
})

app.get('/messages', function(req, res){ // method done
	Message.find({read: false}, function(err, data){
		if(err) throw err 
		for(var i=0;i<data.length;i++){
			// set to read
			data.read = true
			data.save()
		}
		res.json(data) // res.send(data)
	})
})

app.get('/messages/last', function(req, res){ // give last message
	Message.findOne({}, {}, { sort: { 'created_at' : -1 } }, function(err, message) {
		if(err) throw err
		message.read = true
		message.save(function(){
			var encoded = morse.encode(message.text);
			res.json({morse: encoded, text: message.text, groupId: message.groupId, tone: message.tone })
		})
	});
})

app.get('/ngrok', function(req, res){
	res.json({
		"name": "fag69",
		"action": "kys",
		"text": "fuck off cunt"
	})
})

app.get('/messages/:messageId', function(req, res){

})

app.get('/messages/respond', function(req, res){
	// randomly respond using ibm watson on the last message

})

app.listen(3000)