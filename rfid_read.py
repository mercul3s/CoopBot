from serial import Serial

rfid_reader = Serial('/dev/ttyUSB0')

while True:
    message = rfid_reader.readline()
    print message
