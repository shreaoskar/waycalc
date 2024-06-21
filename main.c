#include <stdint.h>
#include <stdio.h>
// #include <assert.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>

struct our_state {
	struct wl_compositor *compositor;
	struct wl_shm *shm;
};

static void registry_handle_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version) {
	struct our_state *state = data;
	if (strcmp(interface, wl_compositor_interface.name) == 0) {
		state->compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
	}
	if (strcmp(interface, wl_shm_interface.name) == 0) {
		state->shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
	}
}

static void registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
	// deliberately left blank
}

static const struct wl_registry_listener
registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

int main(int argc, char** argv) {
	struct our_state state = { 0 };
	struct wl_display *display = wl_display_connect(NULL);
	struct wl_registry *registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, &state);
	struct wl_surface *surface = wl_compositor_create_surface(state.compositor);
	wl_display_roundtrip(display);
	return 0;
}
