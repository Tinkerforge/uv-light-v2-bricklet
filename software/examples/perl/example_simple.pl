#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletUVLightV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your UV Light Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $uvl = Tinkerforge::BrickletUVLightV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current UV-A
my $uva = $uvl->get_uva();
print "UV-A: " . $uva/10.0 . " mW/m²\n";

# Get current UV-B
my $uvb = $uvl->get_uvb();
print "UV-B: " . $uvb/10.0 . " mW/m²\n";

# Get current UV index
my $uvi = $uvl->get_uvi();
print "UV Index: " . $uvi/10.0 . "\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
