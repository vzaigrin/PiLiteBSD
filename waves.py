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
s.write("$$$F000100100001001000010010000001001000000100100000010010000100100001001000010010000001001000000100100000010010000100100001001000\r")

