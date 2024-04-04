#include "src/webp/decode.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"

emscripten::val decoder_version();

emscripten::val decodeRGB(std::string data, int width, int height);

emscripten::val decodeRGBA(std::string data, int width, int height);

emscripten::val decode(std::string data, int width, int height, bool has_alpha);