#include "webp.h"

using namespace emscripten;

thread_local val Uint8Array = val::global("Uint8Array");

int encoder_version()
{
	return WebPGetEncoderVersion();
}

int decoder_version()
{
	return WebPGetDecoderVersion();
}

val encode(std::string rgb, int width, int height, WebPConfig config)
{
	WebPConfig webp_config;
	WebPConfigInit(&webp_config);
	webp_config.quality = config.quality;
	webp_config.lossless = config.lossless;

	WebPMemoryWriter wrt;
	WebPMemoryWriterInit(&wrt);

	WebPPicture pic;
	WebPPictureInit(&pic);
	pic.width = width;
	pic.height = height;

	if (!WebPPictureInit(&pic))
	{
		return val::null();
	}
	if (!WebPPictureAlloc(&pic)) {
		return val::null();
	}
	pic.writer = WebPMemoryWrite;
	pic.custom_ptr = &wrt;

	int ok = WebPEncode(&webp_config, &pic);

	if (!ok) {
		return  val::null();
	}
	return val::global("Uint8Array").new_(typed_memory_view(wrt.size, wrt.mem));
}

val decode(std::string data, int width, int height, WebPConfig config)
{
	return val::null();
}

EMSCRIPTEN_BINDINGS(my_module)
{
	function("encoder_version", &encoder_version);
	function("decoder_version", &decoder_version);
	function("encode", &encode);
	function("decode", &decode);
}
