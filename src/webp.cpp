#include <string.h>
#include <stdio.h>
#include "webp.h"

using namespace emscripten;
thread_local val Uint8Array = val::global("Uint8Array");

val get_version(int hex_version)
{
	int rev_ver = hex_version & 7;
	int min_ver = (hex_version >> 8) & 7;
	int maj_ver = (hex_version >> 16) & 7;
	int vers[3] = {
		maj_ver,
		min_ver,
		rev_ver,
	};
	std::string ver = "";

	for (int i : vers)
	{
		ver.append(std::to_string(i)).append(".");
	}
	ver.pop_back();
	return val::u8string(ver.c_str());
}

val encoder_version()
{
	return get_version(WebPGetEncoderVersion());
}

val decoder_version()
{
	return get_version(WebPGetDecoderVersion());
}
 
val encodeRGB(std::string rgb, int width, int height, int quality_factor) {
	uint8_t* output;
	size_t size = WebPEncodeRGB((uint8_t*)rgb.c_str(), width, height, 3 * width, quality_factor, &output);
	return val::global("Uint8Array").new_(typed_memory_view(size, output));
}

val encodeRGBA(std::string rgba, int width, int height, int quality_factor) {
	uint8_t* output;
	size_t size = WebPEncodeRGBA((uint8_t*)rgba.c_str(), width, height, 4 * width, quality_factor, &output);
	return val::global("Uint8Array").new_(typed_memory_view(size, output));
}

val encode(std::string data, int width, int height, bool use_alpha, WebPConfig config)
{
	WebPConfig webp_config;
	webp_config.quality = config.quality;
	webp_config.lossless = config.lossless;

	WebPMemoryWriter wrt;
	WebPMemoryWriterInit(&wrt);

	WebPPicture pic;
	if (!WebPPictureInit(&pic))
	{
		return val::null();
	}
	pic.use_argb = 1;
	pic.width = width;
	pic.height = height;
	if (!WebPPictureAlloc(&pic))
	{
		return val::null();
	}
	pic.writer = WebPMemoryWrite;
	pic.custom_ptr = &wrt;

	use_alpha
		? WebPPictureImportRGB(&pic, (uint8_t*)data.c_str(), width * 3)
		: WebPPictureImportRGBA(&pic, (uint8_t*)data.c_str(), width * 4);

	int ok = WebPEncode(&webp_config, &pic);

	if (!ok)
	{
		return val::null();
	}
	return val::global("Uint8Array").new_(typed_memory_view(wrt.size, wrt.mem));
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

EMSCRIPTEN_BINDINGS(my_module)
{
	function("encoder_version", &encoder_version);
	function("decoder_version", &decoder_version);
	function("encodeRGB", &encodeRGB);
	function("encodeRGBA", &encodeRGBA);
	function("encode", &encode);
	function("decode", &decode);
}
