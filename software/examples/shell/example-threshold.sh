#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your UV Light Bricklet 2.0

# Handle incoming UV index callbacks
tinkerforge dispatch uv-light-v2-bricklet $uid uvi\
 --execute "echo UV Index: {uvi}/10. UV index > 3. Use sunscreen!" &

# Configure threshold for UV index "greater than 3"
# with a debounce period of 1s (1000ms)
tinkerforge call uv-light-v2-bricklet $uid set-uvi-callback-configuration 1000 false threshold-option-greater 30 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
