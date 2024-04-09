#include "src/webp/encode.h"
#include "src/webp/mux.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"

struct SimpleWebPConfig
{
	int lossless;
  float quality;   
};

emscripten::val encoder_version();

emscripten::val encodeRGB(std::string rgb, int width, int height, int quality_factor);

emscripten::val encodeRGBA(std::string rgba, int width, int height, int quality_factor);

emscripten::val encode(std::string data, int width, int height, bool use_alpha, SimpleWebPConfig config);

emscripten::val encodeAnimation(int width, int height, bool has_alpha, emscripten::val durations, std::string data);