#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;
static GFont s_time_font;
static GFont s_text_font;
static BitmapLayer *s_battery_layer;
static GBitmap *s_battery_bitmap;
#ifdef PBL_COLOR
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
#endif

static void update_time() {
	// Get a tm structure
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer
	static char buffer[] = "00:00";

	// Write the current hours and minutes into the buffer
	if(clock_is_24h_style() == true) {
		// Use 24 hour format
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} else {
		// Use 12 hour format
		strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}

	// Display this time on the TextLayer
	text_layer_set_text(s_time_layer, buffer);
}

static void update_date() {
	// Get a tm structure
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);

	// Create a long-lived buffer
	static char date_buffer[] = "xx. Xxxxxxxxxx";
	static char day_buffer[] = "Xxxxxxxxxx";
	
	strftime(date_buffer, sizeof("xx. Xxxxxxxxxx"), "%d. %B", tick_time);
	strftime(day_buffer, sizeof("Xxxxxxxxxx"), "%A", tick_time);
	
	// Display this time on the TextLayer
	text_layer_set_text(s_date_layer, date_buffer);
	text_layer_set_text(s_day_layer, day_buffer);
}

/*
 * Battery Status
 */
static void battery_handler(BatteryChargeState charge_state) {

	/*if (charge_state.is_charging) {
		snprintf(s_battery_buffer, sizeof(s_battery_buffer), "charging");
	} else {
		snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%% charged", charge_state.charge_percent);
	}*/
	
	switch (charge_state.charge_percent) {
		case 10: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_010); break;
		case 20: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_020); break;
		case 30: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_030); break;
		case 40: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_040); break;
		case 50: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_050); break;
		case 60: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_060); break;
		case 70: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_070); break;
		case 80: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_080); break;
		case 90: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_090); break;
		case 100: s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_100); break;
	}

	
	bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
}

static void main_window_load(Window *window) {
	// Create GFont
	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OSP_DIN_BOLD_72));
	s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OSP_DIN_BOLD_24));	
	
	s_battery_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAT_000_BLACK_BLACK);
	s_battery_layer = bitmap_layer_create(GRect(10, 118, 9, 16));
	bitmap_layer_set_bitmap(s_battery_layer, s_battery_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_battery_layer));

#ifdef PBL_COLOR	
	// Create GBitmap, then set to created BitmapLayer
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
#endif
	
	// Apply to TextLayer
	text_layer_set_font(s_time_layer, s_time_font);
	text_layer_set_font(s_date_layer, s_text_font);
	text_layer_set_font(s_day_layer, s_text_font);
	
	// TIME
	// Create time TextLayer
	s_time_layer = text_layer_create(GRect(0, 38, 140, 72));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorBlack);

	// Improve the layout to be more like a watchface
	//text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_font(s_time_layer, s_time_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	
	// DATE
	// Create date TextLayer
	s_date_layer = text_layer_create(GRect(0, 25, 140, 50));
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorBlack);

	// Improve the layout to be more like a watchface
	text_layer_set_font(s_date_layer, s_text_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
	
	// DAY
	// Create date TextLayer
	s_day_layer = text_layer_create(GRect(0, 116, 140, 50));
	text_layer_set_background_color(s_day_layer, GColorClear);
	text_layer_set_text_color(s_day_layer, GColorBlack);

	// Improve the layout to be more like a watchface
	text_layer_set_font(s_day_layer, s_text_font);
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentRight);

	// Add it as a child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_day_layer);
	
	// Unload GFont
	fonts_unload_custom_font(s_time_font);
	fonts_unload_custom_font(s_text_font);

#ifdef PBL_COLOR
	// Destroy GBitmap
	gbitmap_destroy(s_background_bitmap);

	// Destroy BitmapLayer
	bitmap_layer_destroy(s_background_layer);
#endif
}

static void tick_handler_time(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}
static void tick_handler_date(struct tm *tick_time, TimeUnits units_changed) {
	update_date();
}

static void init() {
	// Create main Window element and assign to pointer
	s_main_window = window_create();

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});

	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);
	
	// Make sure the time is displayed from the start
	update_time();
	update_date();
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_time);
	tick_timer_service_subscribe(DAY_UNIT, tick_handler_date);
	
	battery_state_service_subscribe(battery_handler);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
