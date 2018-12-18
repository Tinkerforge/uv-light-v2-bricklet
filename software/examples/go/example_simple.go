package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
	"github.com/tinkerforge/go-api-bindings/uv_light_v2_bricklet"
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

	// Get current UV-A.
	uva, _ := uvl.GetUVA()
	fmt.Printf("UV-A: %f mW/m²\n", float64(uva)/10.0)

	// Get current UV-B.
	uvb, _ := uvl.GetUVB()
	fmt.Printf("UV-B: %f mW/m²\n", float64(uvb)/10.0)

	// Get current UV index.
	uvi, _ := uvl.GetUVI()
	fmt.Printf("UV Index: %f\n", float64(uvi)/10.0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
