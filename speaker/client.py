import requests
import json

url = http://172.24.176.51/speaker
speaker_data = {"speaker_num":1}

headers = {"content-type":"application/json"}
speaker_json = json.dumps(speaker_data).encode("utf-8")

request = requests.put(url,speaker_json,headers=headers)
responce_data = requests.json()


