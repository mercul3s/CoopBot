from datetime import datetime
from calendar import timegm
from pymongo import MongoClient

# one collection for all events
# one doc for each chicken, door, and nesting box
# update doc with each event, stored by month

mongo_client = MongoClient()
database     = mongo_client.ioc
collection   = database.events


class Chicken(object):
    
    def __init__(self, name, rfid_tag, dob):
        self.name      = name
        self.rfid_tag  = rfid_tag
        self.dob       = dob

    def json(self):
        return {
                "name" : self.name, 
                "dob" : self.dob, 
                "rfid_tag" : self.rfid_tag, 
                "events" : {}
                }

    def lay_egg(self, collection, timestamp):
        collection.update(self.name)
        pass
    # update document with new chicken egg event
    # make sure document exists
    # collection.update(chicken.name)

