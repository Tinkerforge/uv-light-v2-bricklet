#include "bindings/hal_common.h"
#include "bindings/bricklet_uv_light_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your UV Light Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for UV index callback
static void uvi_handler(TF_UVLightV2 *device, int32_t uvi, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("UV Index: %d 1/%d\n", uvi, 10.0);
}

static TF_UVLightV2 uvl;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_uv_light_v2_create(&uvl, UID, hal), "create device object");

	// Register UV index callback to function uvi_handler
	tf_uv_light_v2_register_uvi_callback(&uvl,
	                                     uvi_handler,
	                                     NULL);

	// Set period for UVI callback to 1s (1000ms) without a threshold
	tf_uv_light_v2_set_uvi_callback_configuration(&uvl, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
