#include <string.h>
#include "version.h"
#include "decode.h"

using namespace emscripten;

val decoder_version()
{
	return get_version(WebPGetDecoderVersion());
}
 

val decodeRGB(std::string data, int width, int height) {
	// TODO:
}

val decodeRGBA(std::string data, int width, int height) {
	// TODO:
}

val decode(std::string data, int width, int height, WebPConfig config)
{
	// TODO:
	return val::null();
}