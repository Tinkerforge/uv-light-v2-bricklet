package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/uv_light_v2_bricklet"
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
		fmt.Printf("UV Index: %f\n", float64(uvi)/10.0)
		fmt.Println("UV index > 3. Use sunscreen!")
	})

	// Configure threshold for UV index "greater than 3"
	// with a debounce period of 1s (1000ms).
	uvl.SetUVICallbackConfiguration(1000, false, '>', 3*10, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
