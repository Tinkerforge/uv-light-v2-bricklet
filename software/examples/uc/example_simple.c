#include "bindings/hal_common.h"
#include "bindings/bricklet_uv_light_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your UV Light Bricklet 2.0

void check(int rc, const char* msg);

TF_UVLightV2 uvl;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_uv_light_v2_create(&uvl, UID, hal), "create device object");

	// Get current UV-A
	int32_t uva;
	check(tf_uv_light_v2_get_uva(&uvl, &uva), "get UV-A");

	tf_hal_printf("UV-A: %d 1/%d mW/m²\n", uva, 10.0);

	// Get current UV-B
	int32_t uvb;
	check(tf_uv_light_v2_get_uvb(&uvl, &uvb), "get UV-B");

	tf_hal_printf("UV-B: %d 1/%d mW/m²\n", uvb, 10.0);

	// Get current UV index
	int32_t uvi;
	check(tf_uv_light_v2_get_uvi(&uvl, &uvi), "get UV index");

	tf_hal_printf("UV Index: %d 1/%d\n", uvi, 10.0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
