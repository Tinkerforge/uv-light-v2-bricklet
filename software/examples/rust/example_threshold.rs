use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, uv_light_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let uv_light_v2_bricklet = UVLightV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for UV index events.
    let uvi_listener = uv_light_v2_bricklet.get_uvi_receiver();
    // Spawn thread to handle received events. This thread ends when the uv_light_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in uvi_listener {
            println!("UV Index: {}", event as f32 / 10.0);
            println!("UV index > 3. Use sunscreen!");
        }
    });

    // Configure threshold for UV index "greater than 3"
    // with a debounce period of 1s (1000ms)
    uv_light_v2_bricklet.set_uvi_callback_configuration(1000, false, '>', 3 * 10, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
