from serial import Serial

rfid_reader = Serial('/dev/tty.usbserial-AI02KGA6')

while True:
    message = rfid_reader.readline()
    print message
