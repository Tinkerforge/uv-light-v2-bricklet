var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your UV Light Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var uvl = new Tinkerforge.BrickletUVLightV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for UVI callback to 1s (1000ms) without a threshold
        uvl.setUVICallbackConfiguration(1000, false, 'x', 0, 0);
    }
);

// Register UV index callback
uvl.on(Tinkerforge.BrickletUVLightV2.CALLBACK_UVI,
    // Callback function for UV index callback
    function (uvi) {
        console.log('UV Index: ' + uvi/10.0);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
