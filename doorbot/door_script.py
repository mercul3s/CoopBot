import RPIO as rpio

def setup():
    rpio.setmode(rpio.BOARD)
    mode = rpio.getmode()
    print "Mode is: ", mode

    out_chan_list = [24, 26]
    in_chan_list = [18, 22] 
    rpio.setup(out_chan_list, rpio.OUT)
    rpio.setup(in_chan_list, rpio.IN)
    door_state = ""


# pin 24 is connected to input B on the motor board.
# maybe use wait_for_interrupts or add_interrupt_callback here 
def open():
    while door_state == "CLOSED":
        rpio.output(24, rpio.HIGH)
    rpio.output(24, rpio.LOW)


# pin 26 is connected to input A on the motor board.
def close():
    rpio.output(26, rpio.HIGH)

# need to debounce the switches
def check_switch_states():
    top_read_data = rpio.input(22)
    bottom_read_data = rpio.input(18)
    print "top switch: {}".format(top_read_data)
    print "bottom switch: {}".format(bottom_read_data)
    if top_read_data == 0 and bottom_read_data == 1:
        door_state = "OPEN"
    elif top_read_data == 1 and bottom_read_data == 0:
        door_state = "CLOSED"

def debounce_reed_switch(pin_num):
    switch_val1 = rpio.input(pin_num)
    time.sleep(.01)     # sleep 10 milliseconds
    switch_val2 = rpio.input(pin_num)
    if switch_val1 == switch_val2:
        print "switch values equal"
            


def loop_read():
    while True:
        check_switch_states()


def shutdown():
    rpio.cleanup()
