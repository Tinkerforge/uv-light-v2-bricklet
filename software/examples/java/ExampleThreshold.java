import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletUVLightV2;

public class ExampleThreshold {
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

		// Add UV index listener
		uvl.addUVIListener(new BrickletUVLightV2.UVIListener() {
			public void uvi(int uvi) {
				System.out.println("UV Index: " + uvi/10.0);
				System.out.println("UV index > 3. Use sunscreen!");
			}
		});

		// Configure threshold for UV index "greater than 3"
		// with a debounce period of 1s (1000ms)
		uvl.setUVICallbackConfiguration(1000, false, '>', 3*10, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
