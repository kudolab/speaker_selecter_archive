from flask import Flask,request
import RPi.GPIO as GPIO

app = Flask(__name__)

@app.route("/speaker",methods=["PUT"])
def put_speaker_num():
	if request.method == "PUT":
		speaker_num = request.json["speaker_num"]
	return "ok"

app.run()
