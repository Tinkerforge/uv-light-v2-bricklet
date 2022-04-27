// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_uv_light_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_UVLightV2 uvl;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_uv_light_v2_create(&uvl, NULL, hal), "create device object");

	// Get current UV-A
	int32_t uva;
	check(tf_uv_light_v2_get_uva(&uvl, &uva), "get UV-A");

	tf_hal_printf("UV-A: %d 1/%d mW/m²\n", uva, 10);

	// Get current UV-B
	int32_t uvb;
	check(tf_uv_light_v2_get_uvb(&uvl, &uvb), "get UV-B");

	tf_hal_printf("UV-B: %d 1/%d mW/m²\n", uvb, 10);

	// Get current UV index
	int32_t uvi;
	check(tf_uv_light_v2_get_uvi(&uvl, &uvi), "get UV index");

	tf_hal_printf("UV Index: %d 1/%d\n", uvi, 10);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
