#!/usr/bin/env python3
import sys
import json

import RPi.GPIO as GPIO
from flask import Flask, request, jsonify

GPIO.setmode(GPIO.BCM)
app = Flask(__name__)


@app.route("/health", methods=["GET"])
def health():
    return "ok"

@app.route("/speaker", methods=["GET"])
def get_speaker_num():
    with open('speaker.json', mode='rt', encoding='utf-8') as f:
        speaker_json = json.load(f)
        return jsonify(speaker_json)

@app.route("/speaker", methods=["PUT"])
def put_speaker_num():
    if request.json is None:
        msg = f"Error: request body is invalid"
        print(msg, file=sys.stderr)
        return jsonify({"message": msg}), 400

    if not "speaker_num" in request.json:
        msg = f"Error: request body is invalid"
        print(msg, file=sys.stderr)
        return jsonify({"message": msg}), 400


    speaker_num = request.json["speaker_num"]
    # Error process
    if speaker_num < 1 or speaker_num > 19:
        msg = f"Error: speaker_num must be in 1-19, got: {speaker_num}"
        print(msg, file=sys.stderr)
        return jsonify({"message": msg}), 400
    # Speaker select
    onNo = 2
    print(f"Present speaker number ... {speaker_num}", file=sys.stderr)
    for i in range(1, 19 + 1):
        GPIO.setup(i, GPIO.OUT)
        GPIO.output(i, 0)
    onNo = speaker_num + 1
    GPIO.output(onNo, 1)

    with open('speaker.json', mode='wt', encoding='utf-8') as f:
        json.dump(request.json, f)

    # Success message
    msg = f"speaker_num changed to {speaker_num}"
    print(msg, file=sys.stderr)
    return jsonify({"message": msg})


app.run(host="0.0.0.0", port=80)
