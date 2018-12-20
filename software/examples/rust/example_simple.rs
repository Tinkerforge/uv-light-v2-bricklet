use std::{io, error::Error};

use tinkerforge::{ip_connection::IpConnection, 
                  uv_light_v2_bricklet::*};


const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let uvl = UvLightV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
    // Don't use device before ipcon is connected.

		// Get current UV-A.
let uva = uvl.get_uva().recv()?;
		println!("UV-A: {} mW/m²", uva as f32 /10.0);

		// Get current UV-B.
let uvb = uvl.get_uvb().recv()?;
		println!("UV-B: {} mW/m²", uvb as f32 /10.0);

		// Get current UV index.
let uvi = uvl.get_uvi().recv()?;
		println!("UV Index: {}", uvi as f32 /10.0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
