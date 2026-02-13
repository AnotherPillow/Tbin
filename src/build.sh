emcc -I. main.cpp tbin/Map.cpp \
    -O3 \
    -o build/tbin.js \
    -sMODULARIZE=1 \
    -sEXPORT_ES6=1 \
    -sEXPORT_NAME=TBIN \
    -sEXPORTED_FUNCTIONS=_malloc,_free,_map_create,_map_destroy,_map_load_file,_map_log_props,_map_get_id,_map_prop_count,_map_props_keys_list,_map_free_string_list,_map_prop_get_value,_map_tilesheets_count,_map_tilesheets_get_name,_map_tilesheets_get_image,_map_tilesheets_prop_count,_map_tilesheet_prop_get_value,_map_tilesheets_tile_x,_map_tilesheets_tile_y,_map_tilesheets_image_px_x,_map_tilesheets_image_px_y,_map_tilesheet_prop_keys_list,_map_layers_count,_map_layer_get_name,_map_layer_get_tile_width,_map_layer_get_tile_height,_map_layer_get_visiblity,_map_layer_get_tile_width_px_x,_map_layer_get_tile_width_px_y \
    -sEXPORTED_RUNTIME_METHODS=FS,ccall,cwrap,UTF8ToString,HEAPU32,getValue