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

# Get current UV-A
uva = uvl.get_uva
puts "UV-A: #{uva/10.0} mW/m²"

# Get current UV-B
uvb = uvl.get_uvb
puts "UV-B: #{uvb/10.0} mW/m²"

# Get current UV index
uvi = uvl.get_uvi
puts "UV Index: #{uvi/10.0}"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
