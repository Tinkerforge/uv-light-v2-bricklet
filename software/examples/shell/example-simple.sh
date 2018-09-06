#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your UV Light Bricklet 2.0

# Get current UV-A
tinkerforge call uv-light-v2-bricklet $uid get-uva

# Get current UV-B
tinkerforge call uv-light-v2-bricklet $uid get-uvb

# Get current UV index
tinkerforge call uv-light-v2-bricklet $uid get-uvi
