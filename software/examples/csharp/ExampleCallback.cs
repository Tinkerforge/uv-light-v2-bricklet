using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0

	// Callback function for UV index callback
	static void UVICB(BrickletUVLightV2 sender, int uvi)
	{
		Console.WriteLine("UV Index: " + uvi/10.0);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletUVLightV2 uvl = new BrickletUVLightV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register UV index callback to function UVICB
		uvl.UVICallback += UVICB;

		// Set period for UVI callback to 1s (1000ms) without a threshold
		uvl.SetUVICallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
