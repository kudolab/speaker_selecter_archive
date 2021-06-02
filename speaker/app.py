from flask import Flask,request,jsonify
import RPi.GPIO as GPIO
import sys

GPIO.setmode(GPIO.BCM)
app = Flask(__name__)

@app.route("/speaker",methods=["PUT"])
def put_speaker_num():
	speaker_num = request.json["speaker_num"]
	# Error process
	if speaker_num<1 or speaker>18:
		msg = f"Error: speaker_num must be in 1-18, got: {speaker_num}"
		print(msg,file=sys.stderr)
		return jsonify({"message":msg}), 400
	# Speaker select
	print("##########################################")
	print("Controlling now...")
	print(f"Present speaker number ... {speaker_num}")
	print("##########################################")
	for i in range(1,19):
		GPIO.setup(i,GPIO.OUT)
		GPIO.output(i,0)
	onNo = speaker_num + 1
	GPIO.output(onNo,1)
	# Success message
	msg = f"speaker_num changed to {speaker_num}"
	print(msg,file=sys.stderr)
	return jsonify({"message":msg})

app.run(0.0.0.0)
