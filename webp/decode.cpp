#include <string.h>
#include "version.h"
#include "decode.h"

using namespace emscripten;

val decoder_version()
{
	return get_version(WebPGetDecoderVersion());
}

thread_local const val ImageData = val::global("ImageData");

val decodeRGB(std::string data) {
	size_t data_size = data.size();
	int width;
	int height;
	uint8_t* output = WebPDecodeRGB((uint8_t*)data.c_str(), data_size, &width, &height);
	return ImageData.new_(
		val::global("Uint8ClampedArray").new_(typed_memory_view(3 * width * height, output)),
		width
	);
}

val decodeRGBA(std::string data) {
	size_t data_size = data.size();
	int width;
	int height;
	uint8_t* output = WebPDecodeRGBA((uint8_t*)data.c_str(), data_size, &width, &height);
	return ImageData.new_(
		val::global("Uint8ClampedArray").new_(typed_memory_view(4 * width * height, output)),
		width
	);
}

val decode(std::string data, bool has_alpha)
{
	size_t data_size = data.size();
	WebPBitstreamFeatures features;
	if (!WebPGetFeatures((uint8_t*)data.c_str(), data_size, &features)) {
		return val::null();
	}
	WebPDecoderConfig config;
	WebPInitDecoderConfig(&config);

	config.input.has_alpha = has_alpha;
	config.output.width = features.width;
	config.output.height = features.height;
	// only support ARGB and RGB !!!
	config.output.colorspace = has_alpha ? MODE_ARGB : MODE_RGB;

	auto success = WebPDecode((uint8_t*)data.c_str(), data_size, &config);
	if (!success) {
		WebPFreeDecBuffer(&config.output);
		return val::null();
	}
	WebPFreeDecBuffer(&config.output);
	val decoded_data = val::global("Uint8ClampedArray").new_(typed_memory_view(config.output.u.RGBA.size, config.output.u.RGBA.rgba));
	return ImageData.new_(
		decoded_data,
		features.width
	);
}