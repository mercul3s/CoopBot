import RPi.GPIO as gpio
import time

def setup():
    gpio.setmode(gpio.BOARD)
    mode = gpio.getmode()
    print "Mode is: ", mode

    out_chan_list = [24, 26]
    in_chan_list = [18, 22] 
    gpio.setup(out_chan_list, gpio.OUT)
    gpio.setup(in_chan_list, gpio.IN, pull_up_down=gpio.PUD_UP)
    door_state = ""
    gpio.add_event_detect(18, gpio.FALLING, callback=stop_motor_top, bouncetime=300)
    gpio.add_event_detect(22, gpio.FALLING, callback=stop_motor_bottom, bouncetime=300)


# pin 24 is connected to input B on the motor board.
# maybe use wait_for_interrupts or add_interrupt_callback here 
# http://raspi.tv/2013/how-to-use-interrupts-with-python-on-the-raspberry-pi-and-rpi-gpio
# http://raspi.tv/2013/how-to-use-interrupts-with-python-on-the-raspberry-pi-and-rpi-gpio-part-2
# http://raspi.tv/2013/how-to-use-interrupts-with-python-on-the-raspberry-pi-and-rpi-gpio-part-3

def open():
    print "opening door"
    gpio.output(24, gpio.HIGH)

def stop_motor_top(switch_pin):
    print "stopping motor top {}".format(switch_pin)
    check_switch_states()
    gpio.output(24, gpio.LOW)

def stop_motor_bottom(switch_pin):
    print "stopping motor bottom {}".format(switch_pin)
    check_switch_states()
    gpio.output(26, gpio.LOW)

# pin 26 is connected to input A on the motor board.
def close():
    print "closing door"
    gpio.output(26, gpio.HIGH)

def callback_function_print(input_pin):
    print "input on pin", input_pin

# need to debounce the switches
def check_switch_states():
    top_read_data = gpio.input(18)
    bottom_read_data = gpio.input(22)
    print "top switch: {}".format(top_read_data)
    print "bottom switch: {}".format(bottom_read_data)
    #if top_read_data == 0 and bottom_read_data == 1:
    #    door_state = "OPEN"
    #elif top_read_data == 1 and bottom_read_data == 0:
    #    door_state = "CLOSED"

def loop_read():
    for i in range(1,10):
        check_switch_states()
        time.sleep(1)


def shutdown():
    gpio.cleanup()
