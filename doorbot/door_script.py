import RPi.GPIO as gpio

def setup():
    gpio.setmode(gpio.BOARD)
    mode = gpio.getmode()
    print "Mode is: ", mode

    out_chan_list = [24, 26]
    in_chan_list = [18, 22] 
    gpio.setup(out_chan_list, gpio.OUT)
    gpio.setup(in_chan_list, gpio.IN)
    door_state = ""




# pin 24 is connected to input B on the motor board.
def open():
    while door_state == "CLOSED":
        gpio.output(24, gpio.HIGH)
    gpio.output(24, gpio.LOW)


# pin 26 is connected to input A on the motor board.
def close():
    gpio.output(26, gpio.HIGH)

# need to debounce the switches
def check_switch_states():
    top_read_data = gpio.input(22)
    bottom_read_data = gpio.input(18)
    print "top switch: {}".format(top_read_data)
    print "bottom switch: {}".format(bottom_read_data)
    if top_read_data == 0 and bottom_read_data == 1:
        door_state = "OPEN"
    elif top_read_data == 1 and bottom_read_data == 0:
        door_state = "CLOSED"

def debounce_reed_switch(pin_num):
    switch_val1 = gpio.input(pin_num)
    time.sleep(.01)
    switch_val2 = gpio.input(pin_num)
    if switch_val1 == switch_val2:
            


def loop_read():
    while True:
        check_switch_states()


def shutdown():
    gpio.cleanup()
