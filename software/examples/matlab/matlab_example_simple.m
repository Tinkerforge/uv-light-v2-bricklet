function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletUVLightV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your UV Light Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    uvl = handle(BrickletUVLightV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current UV-A
    uva = uvl.getUVA();
    fprintf('UV-A: %g mW/m²\n', uva/10.0);

    % Get current UV-B
    uvb = uvl.getUVB();
    fprintf('UV-B: %g mW/m²\n', uvb/10.0);

    % Get current UV index
    uvi = uvl.getUVI();
    fprintf('UV Index: %g\n', uvi/10.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
