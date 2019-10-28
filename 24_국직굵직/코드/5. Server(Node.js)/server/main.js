async function main(audio_file,callback) {
  // Imports the Google Cloud client library
  const speech = require('@google-cloud/speech');
  const fs = require('fs');
  const Chatbot = require('./Chatbot');
  // Creates a client
  const client = new speech.SpeechClient();

  // The name of the audio file to transcribe
  //const fileName = './audio_files/11_low.mp3';

  const fileName = './uploads/'+audio_file.filename;
  // Reads a local audio file and converts it to base64
  const file = fs.readFileSync(fileName);
  const audioBytes = file.toString('base64');

  // The audio file's encoding, sample rate in hertz, and BCP-47 language code
  const audio = {
    content: audioBytes,
  };
  const config = {
    encoding: 'mp3',
    sampleRateHertz: 20000,
    languageCode: 'ko-KR',
	speechContexts: [
		{
			phrases: ["배차신청", "탄약","의무대","지통실","군의관","훈련","암구호","컴퓨터","프린터","국군대전병원","국군수도병원","직속상관","출장신청","어 난데","탄약 신청","개인신상","PX","사령관","자비스","전역일"]
		}
	]
  };
  const request = {
    audio: audio,
    config: config,
  };

  // Detects speech in the audio file
  const [response] = await client.recognize(request);
  const transcription = response.results
    .map(result => result.alternatives[0].transcript)
    .join('\n');
  console.log(`Transcription: ${transcription}`);
  var tr=transcription.replace(/\s/gi, "");
  //console.log(`${tr}`);
  if(tr == null || tr == '') {
	console.log("sound undetected");
    callback("Zzz","0. sound undetected");
  }
  else{
    Chatbot.chatbot(transcription,function(msg){
	  console.log(msg);
		//들리지 않았을때 예외처리
	  callback(transcription,msg);
    });
  }
}

var bodyParser = require('body-parser');
var express = require('express');
var path = require('path');
var app = express();
var server = app.listen(3000, function(){
    console.log("Express server has started on port 3000")
})
var locks = require('locks');
var multer = require('multer');
var upload = multer({ dest: 'uploads/' })
var upload_img = multer({ dest: 'image/' })
var talk_data = ""
var swi = 1;
app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 
//app.use(express.static('data'));
app.use(express.static(path.join(__dirname,'image')));
app.get('/app', (req, res) => {
	res.send(talk_data);
	console.log("ok");
});
app.get('/switch', (req, res) => {
	res.send("switch");
	swi=1-swi;
	console.log(swi);
});
app.post('/upload', upload.single('file'), function(req, res){
  main(req.file,function(tr, msg){
	  console.log(msg);
	  var str=msg.split(". ");
	  if(swi==0 && req.body.user=="1"){
		  res.send("Jarvis is turned off");
	  	  talk_data+=tr+"\nJarvis is turned off\n";
	  }
	  else if(req.body.user>=str[0]){
        res.send(str[1]);  
	    if(req.body.user=="1"){
	  	  talk_data+=tr+"\n"+str[1]+"\n";
	    }
	  }
	  else{
		if(req.body.user=="1") 
	  	  talk_data+=tr+"\npermission denied\n";
		res.send("permission denied")
	  }
  }).catch(console.error);
});
fs= require("fs");
app.post('/image', upload_img.single('file'), function(req, res){
	var fileName = req.file.filename;
	var path_='/workspace/nodejs_server/osam-guggicgulgic/server/image/';
	fs.rename(path_+fileName, path_+fileName+".jpeg", function(err) {
    	if ( err ) console.log('ERROR: ' + err);
	});
	console.log(fileName);
	res.send("ok");
});

app.get('/', function(req, res){
    //res.send('Hello Router, <img src="/logo.png">')
	const directoryPath = path.join(__dirname, 'image');
//passsing directoryPath and callback function
	fs.readdir(directoryPath, function (err, files) {
		//handling error
		if (err) {
			return console.log('Unable to scan directory: ' + err);
		} 
		//listing all files using forEach
		html_txt='<html><head><a href="http://192.168.123.40:8080/browserfs.html">CCTV 확인</a></head><BODY><TABLE>'
		var size = Object.keys(files).length;
		var i=0;
		files.forEach(function (file) {
			fs.stat(directoryPath+"/"+file, function(err, stats) {
				img_name="/"+file;
				time=stats.mtime;
				name="건철"
				html_txt+='<TR heigth=300><TD width=300> <img src="'+img_name+'" alt="My Image" height=300 width=300> </TD> <TD><font size=20> 시간:'+time+'<br/> 관등성명:'+name+'<br/></font> </TD></TR>';
				i++;
				if(i==size){
					html_txt+="</TABLE></BODY></html>"
					fs.writeFile('sample.html', html_txt, (err) => {
						// throws an error, you could also catch it here
						if (err) throw err;
						res.sendFile('sample.html', {root: __dirname });
					});
				}
			});
		});
	});
});