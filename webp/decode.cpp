#include <string.h>
#include "version.h"
#include "decode.h"

using namespace emscripten;

val decoder_version()
{
	return get_version(WebPGetDecoderVersion());
}
 

val decodeRGB(std::string data, int width, int height) {
	size_t data_size = data.size();
	uint8_t* output = WebPDecodeRGB((uint8_t*)data.c_str(), data_size, &width, &height);
	return val::global("Uint8Array").new_(typed_memory_view(3 * width * height, output));
}

val decodeRGBA(std::string data, int width, int height) {
	size_t data_size = data.size();
	uint8_t* output = WebPDecodeRGB((uint8_t*)data.c_str(), data_size, &width, &height);
	return val::global("Uint8Array").new_(typed_memory_view(4 * width * height, output));
}

val decode(std::string data, int width, int height, bool has_alpha)
{
	WebPDecoderConfig config;
	WebPInitDecoderConfig(&config);
	config.input.has_alpha = has_alpha;
	config.output.width = width;
	config.output.height = height;
	// only support ARGB and RGB !!!
	config.output.colorspace = has_alpha ? MODE_ARGB : MODE_RGB;

	size_t data_size = data.size();
	auto success = WebPDecode((uint8_t*)data.c_str(), data_size, &config);
	if (!success) {
		WebPFreeDecBuffer(&config.output);
		return val::null();
	}
	WebPFreeDecBuffer(&config.output);
	val decoded_data = val::global("Uint8Array").new_(typed_memory_view(config.output.u.RGBA.size, config.output.u.RGBA.rgba));
	return decoded_data;
}