#include "src/webp/encode.h"
#include "src/webp/decode.h"
#include "emscripten/emscripten.h"
#include "emscripten/bind.h"

emscripten::val encode(std::string rgb, int width, int height, WebPConfig config);

