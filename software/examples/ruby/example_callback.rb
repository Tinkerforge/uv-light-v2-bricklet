#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_uv_light_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your UV Light Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
uvl = BrickletUVLightV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register UV index callback
uvl.register_callback(BrickletUVLightV2::CALLBACK_UVI) do |uvi|
  puts "UV Index: #{uvi/10.0}"
end

# Set period for UVI callback to 1s (1000ms) without a threshold
uvl.set_uvi_callback_configuration 1000, false, 'x', 0, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
