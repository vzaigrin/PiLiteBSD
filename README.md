# PiLiteBSD
Demo and library to work with Pi-Lite on Raspberry Pi with FreeBSD

The Pi-Lite is a very versatile 9 x 14 LED matrix display with an on-board Arduino ATmega 328 processor. Each pixel is individually addressable.

pilite.c - programm to send command to Pi-Lite
* all-off - command to clear screen on Pi-Lite
* all-on - command to fill screen on Pi-Lite
* smile.cat - command to draw smile on Pilite

Some demo programms with direct work with Pi-Lite:
* smile.c - programm on C to draw a smile
* smile.py - programm on Python to draw a smile
* waves.py - programm on Python to draw two waves

libpilite - small library to work with Pi-Lite
* demo.c - demo programm for libpilite
