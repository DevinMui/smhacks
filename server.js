var express = require("express")
var bodyParser = require("body-parser")
var Bot = require("slackbots")
var mongoose = require("mongoose")
var morse = require('morse')
var secret = require('./secret.js')
var ToneAnalyzerV3 = require('watson-developer-cloud/tone-analyzer/v3');

var app = express()

mongoose.connect('mongodb://localhost/smhacks'); 

var tone_analyzer = new ToneAnalyzerV3({
  username: secret.username,
  password: secret.password,
  version_date: '2016-05-19'
});

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
    token: secret.token,
    name: 'smart-watch'
};
var bot = new Bot(settings);

bot.on('start', function() {
	// bot.postMessageToGroup('particle-smart-watch', 'hello group chat!');
	// tone_analyzer.tone({ text: "you are amazing!" }, // get tone of text
	// 	function(err, tone) {
	// 	    if (err)
	// 	    	console.log(err);
	// 	    else {
	// 	      	console.log(JSON.stringify(tone, null, 2));
	// 	      	var tones = tone["document_tone"]["tone_categories"][0]["tones"]
	// 	     	var high = 0
	// 	     	var index = 0
	// 	     	for(var i=0;i<tones.length;i++){
	// 	     		if(tones[i]["score"] > high){
	// 	     			index = i
	// 	     			high = tones[i]["score"]
	// 	     		}
	// 	     	}
	// 	     	if(tones[index]["tone_id"] === "anger" || tones[index]["tone_id"] === "disgust" || tones[index]["tone_id"] === "fear" || tones[index]["tone_id"] === "sadness"){
	// 	     		// tone = bad
	// 	     		console.log("bad")
	// 	     	} else {
	// 	     		// tone = good
	// 	     		console.log("good")
	// 	     	}
	// 	    }
	// });
});

bot.on('message', function(data){
    // data.channel, data.text, data.user
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
				tone_analyzer.tone({ text: "you are amazing!" }, // get tone of text
				function(err, tone) {
				    if (err)
				    	console.log(err);
				    else {
				      	var tones = tone["document_tone"]["tone_categories"][0]["tones"]
				     	var high = 0
				     	var index = 0
				     	for(var i=0;i<tones.length;i++){
				     		if(tones[i]["score"] > high){
				     			index = i
				     			high = tones[i]["score"]
				     		}
				     	}
				     	var t = null
				     	if(tones[index]["tone_id"] === "anger" || tones[index]["tone_id"] === "disgust" || tones[index]["tone_id"] === "fear" || tones[index]["tone_id"] === "sadness"){
				     		// tone = bad
				     		console.log("bad")
				     		t = "bad"
				     	} else {
				     		// tone = good
				     		console.log("good")
				     		t = "good"
				     	}
				     	new Message({
							groupId: group.groupId,
							text: data.text,
							user: data.user,
							color: group.color,
							tone: t
						}).save()
				    }
				});
				
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
		var read = message.read
		if(err) throw err
		message.read = true
		message.save(function(){
			var encoded = morse.encode(message.text);
			res.json({morse: encoded, text: message.text, groupId: message.groupId, tone: message.tone, read: read })
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