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
        // Get current UV-A
        uvl.getUVA(
            function (uva) {
                console.log('UV-A: ' + uva/10.0 + ' mW/m²');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );

        // Get current UV-B
        uvl.getUVB(
            function (uvb) {
                console.log('UV-B: ' + uvb/10.0 + ' mW/m²');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );

        // Get current UV index
        uvl.getUVI(
            function (uvi) {
                console.log('UV Index: ' + uvi/10.0);
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
