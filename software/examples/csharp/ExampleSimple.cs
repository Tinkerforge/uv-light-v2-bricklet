using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletUVLightV2 uvl = new BrickletUVLightV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current UV-A
		int uva = uvl.GetUVA();
		Console.WriteLine("UV-A: " + uva/10.0 + " mW/m²");

		// Get current UV-B
		int uvb = uvl.GetUVB();
		Console.WriteLine("UV-B: " + uvb/10.0 + " mW/m²");

		// Get current UV index
		int uvi = uvl.GetUVI();
		Console.WriteLine("UV Index: " + uvi/10.0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
