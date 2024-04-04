#include "src/webp/encode.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"

emscripten::val get_version(int hex_version);


void merge_webp_config(WebPConfig target, WebPConfig source);