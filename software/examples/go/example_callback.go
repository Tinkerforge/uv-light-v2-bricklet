package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/uv_light_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your UV Light Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	uvl, _ := uv_light_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	uvl.RegisterUVICallback(func(uvi int32) {
		fmt.Printf("UV Index: %d\n", float64(uvi)/10.0)
	})

	// Set period for UVI callback to 1s (1000ms) without a threshold.
	uvl.SetUVICallbackConfiguration(1000, false, 'x', 0, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
