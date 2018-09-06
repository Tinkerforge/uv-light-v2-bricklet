#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your UV Light Bricklet 2.0

# Handle incoming UV index callbacks
tinkerforge dispatch uv-light-v2-bricklet $uid uvi &

# Set period for UVI callback to 1s (1000ms) without a threshold
tinkerforge call uv-light-v2-bricklet $uid set-uvi-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
