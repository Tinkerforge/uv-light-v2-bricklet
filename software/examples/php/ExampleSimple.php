<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletUVLightV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletUVLightV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your UV Light Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$uvl = new BrickletUVLightV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current UV-A
$uva = $uvl->getUVA();
echo "UV-A: " . $uva/10.0 . " mW/m²\n";

// Get current UV-B
$uvb = $uvl->getUVB();
echo "UV-B: " . $uvb/10.0 . " mW/m²\n";

// Get current UV index
$uvi = $uvl->getUVI();
echo "UV Index: " . $uvi/10.0 . "\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
