# PiLiteBSD
Demo and library to work with Pi-Lite on Raspberry Pi with FreeBSD

The **Pi-Lite** is a very versatile 9 x 14 LED matrix display with an on-board Arduino ATmega 328 processor. Each pixel is individually addressable.

**pilite.c** - programm to send commands to Pi-Lite
* all-off - command to clear screen
* all-on - command to fill screen
* smile.cat - commands to draw a smile

Some demo programms with direct work with Pi-Lite:
* smile.c - programm on C to draw a smile
* smile.py - programm on Python to draw a smile
* waves.py - programm on Python to draw two waves

**libpilite** - small library to work with Pi-Lite
* demo.c - demo programm for libpilite

**libpilite** has this functions:
* pilite_init - to initialize Pi-Lite
* pilite_close - to close Pi-Lite
* pilite_clear - to clear the screen
* pilite_fill - to fill the screen
* pilite_point - to draw a point on the screen
* pilite_line - to draw a line on the screen
* pilite_box - to draw a box on the screen
* pilite_fillbox - to draw a filled box on the screen
* pilite_circle - to draw a circle on the screen
* pilite_fillcircle - to draw a filled circle on the screen
