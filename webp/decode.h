#include "src/webp/decode.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"

emscripten::val decoder_version();

emscripten::val decodeRGB(std::string data);

emscripten::val decodeRGBA(std::string data);

emscripten::val decode(std::string data, bool has_alpha);