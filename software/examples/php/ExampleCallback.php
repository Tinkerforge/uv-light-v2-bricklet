<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletUVLightV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletUVLightV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your UV Light Bricklet 2.0

// Callback function for UV index callback
function cb_uvi($uvi)
{
    echo "UV Index: " . $uvi/10.0 . "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$uvl = new BrickletUVLightV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register UV index callback to function cb_uvi
$uvl->registerCallback(BrickletUVLightV2::CALLBACK_UVI, 'cb_uvi');

// Set period for UVI callback to 1s (1000ms) without a threshold
$uvl->setUVICallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
