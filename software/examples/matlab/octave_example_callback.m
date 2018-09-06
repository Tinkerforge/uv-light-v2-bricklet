function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your UV Light Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    uvl = javaObject("com.tinkerforge.BrickletUVLightV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register UV index callback to function cb_uvi
    uvl.addUVICallback(@cb_uvi);

    % Set period for UVI callback to 1s (1000ms) without a threshold
    uvl.setUVICallbackConfiguration(1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for UV index callback
function cb_uvi(e)
    fprintf("UV Index: %g\n", e.uvi/10.0);
end
