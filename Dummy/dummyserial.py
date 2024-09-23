import serial
import time

# define serial port, baud rate, and timeout
port = "COM9"
baudrate = 38400
timeout = 0.1

arduino = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)


def write_read(x):
    arduino.write(bytes(x, "utf-8"))
    time.sleep(0.05)
    data = arduino.readline()
    return data


while True:
    dummy = open("F:\AIS RECEIVER\AISParser\Dummy\dummy.txt", "r")
    for x in dummy:
        # remove whitespace from string
        aisdat = x.rstrip()
        # concatenate string with new line
        write_read(aisdat + "\n")
