from astral import Astral, Location
from datetime import datetime
import os
import pytz

town = os.environ.get('TOWN')
lat = os.environ.get('LAT')
lon = os.environ.get('LONG')
location = Location(
            (town, 
            'USA', 
            lat, 
            lon, 
            'US/Pacific', 
            328.084)
        )
def get_sun_times():
    sun = location.sun()
    return sun['sunrise'], sun['dusk']

def get_local_time():
    tz = pytz.timezone('US/Pacific')
    current_datetime = datetime.now()
    local_dt = tz.localize(current_datetime)
    return local_dt 

def is_after_sunrise(sunrise_time, current_time):
    return sunrise_time < current_time

def is_after_sunset(sunset_time, current_time):
    return sunset_time < current_time
