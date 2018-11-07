use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, uv_light_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let uvl = UvLightV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let uvi_receiver = uvl.get_uvi_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `uvl` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for uvi in uvi_receiver {
            println!("UV Index: {}", uvi as f32 / 10.0);
            println!("UV index > 3. Use sunscreen!");
        }
    });

    // Configure threshold for UV index "greater than 3"
    // with a debounce period of 1s (1000ms).
    uvl.set_uvi_callback_configuration(1000, false, '>', 3 * 10, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
