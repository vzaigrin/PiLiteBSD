#!/usr/local/bin/python2

import serial, time, sys

s = serial.Serial()
s.baudrate = 9600
s.timeout = 0
s.port = "/dev/ttyu0"

try:
  s.open()
except serial.SerialException, e:
  sys.stderr.write("could not open port %r: %s\n" % (s.port, e))
  sys.exit(1)

s.write("$$$ALL,OFF\r")
s.write("$$$F111111111100000001100000001100000001100010001101001001100000101100000101101001001100010001100000001100000001100000001111111111\r")

