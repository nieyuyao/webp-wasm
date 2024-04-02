#include "src/webp/encode.h"
#include "src/webp/decode.h"
#include "emscripten/emscripten.h"
#include "emscripten/bind.h"

emscripten::val encoder_version();

emscripten::val decoder_version();

emscripten::val encodeRGB(std::string rgb, int width, int height, int quality_factor);

emscripten::val encodeRGBA(std::string rgba, int width, int height, int quality_factor);

emscripten::val encode(std::string data, int width, int height, bool use_alpha, WebPConfig config);

emscripten::val decodeRGB(std::string data, int width, int height);

emscripten::val decodeRGBA(std::string data, int width, int height);

emscripten::val decode(std::string data, int width, int height, WebPConfig config);