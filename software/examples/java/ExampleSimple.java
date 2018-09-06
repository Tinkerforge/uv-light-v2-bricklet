import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletUVLightV2;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your UV Light Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletUVLightV2 uvl = new BrickletUVLightV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current UV-A
		int uva = uvl.getUVA(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("UV-A: " + uva/10.0 + " mW/m²");

		// Get current UV-B
		int uvb = uvl.getUVB(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("UV-B: " + uvb/10.0 + " mW/m²");

		// Get current UV index
		int uvi = uvl.getUVI(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("UV Index: " + uvi/10.0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
