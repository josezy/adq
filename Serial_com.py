import serial, time
from datetime import datetime
import json
while True:
	try:
		new_data = {
			"dt": str(datetime.now()),
			"pos": {
			    "lat":6.7,
			    "lon": 6.7,
			    "alt":6.7
			}
  	}
		arduino = serial.Serial("/dev/ttyACM0", 115200)
		JSON=arduino.readline().decode("utf-8")
		JSON=json.loads(JSON)
		new_data.update(JSON)
		print(new_data) 
		arduino.close()
	except ValueError as e:
		print("BAD JSON")

