emcc -I. main.cpp tbin/Map.cpp \
    -O3 \
    -o build/tbin.js \
    -sMODULARIZE=1 \
    -sEXPORT_ES6=1 \
    -sEXPORT_NAME=TBIN \
    -sEXPORTED_FUNCTIONS=_map_create,_map_destroy,_map_load_file,_map_log_props,_map_get_id \
    -sEXPORTED_RUNTIME_METHODS=FS,ccall,cwrap