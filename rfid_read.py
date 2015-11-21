from serial import Serial
from datetime import datetime
from calendar import timegm
from pymongo import MongoClient

mongo_client = MongoClient()
database     = mongo_client.ioc
collection   = database.events
door_rfid_reader = Serial('/dev/ttyUSB0')

# {'dob': '02/2015', 'rfid_tag': 2349879, 'name': 'Henrietta', 'events': {}}

while True:
    tag = door_rfid_reader.readline()
    if tag:
	    timestamp = timegm(datetime.utcnow().utctimetuple())
	    collection.update('rfid_tag' : tag, {'set' : {'events' : {"event" : "laid egg", "timestamp" : timestamp}}})
    print tag
