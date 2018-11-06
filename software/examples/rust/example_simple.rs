use std::{error::Error, io};

use tinkerforge::{ipconnection::IpConnection, uv_light_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your UV Light Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let uv_light_v2_bricklet = UVLightV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current UV-A
    let uva = uv_light_v2_bricklet.get_uva().recv()?;
    println!("UV-A: {}{}", uva as f32 / 10.0, " mW/m²");

    // Get current UV-B
    let uvb = uv_light_v2_bricklet.get_uvb().recv()?;
    println!("UV-B: {}{}", uvb as f32 / 10.0, " mW/m²");

    // Get current UV index
    let uvi = uv_light_v2_bricklet.get_uvi().recv()?;
    println!("UV Index: {}", uvi as f32 / 10.0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
