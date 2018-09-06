#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletUVLightV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your UV Light Bricklet 2.0

# Callback subroutine for UV index callback
sub cb_uvi
{
    my ($uvi) = @_;

    print "UV Index: " . $uvi/10.0 . "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $uvl = Tinkerforge::BrickletUVLightV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register UV index callback to subroutine cb_uvi
$uvl->register_callback($uvl->CALLBACK_UVI, 'cb_uvi');

# Set period for UVI callback to 1s (1000ms) without a threshold
$uvl->set_uvi_callback_configuration(1000, 0, 'x', 0, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
