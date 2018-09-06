function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletUVLightV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your UV Light Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    uvl = handle(BrickletUVLightV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register UV index callback to function cb_uvi
    set(uvl, 'UVICallback', @(h, e) cb_uvi(e));

    % Configure threshold for UV index "greater than 3"
    % with a debounce period of 1s (1000ms)
    uvl.setUVICallbackConfiguration(1000, false, '>', 3*10, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for UV index callback
function cb_uvi(e)
    fprintf('UV Index: %g\n', e.uvi/10.0);
    fprintf('UV index > 3. Use sunscreen!\n');
end
