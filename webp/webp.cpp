#include "emscripten/emscripten.h"
#include "emscripten/bind.h"
#include "encode.h"
#include "decode.h"

EMSCRIPTEN_BINDINGS(module)
{
	emscripten::value_object<SimpleWebPConfig>("SimpleWebPConfig")
		.field("lossless", &SimpleWebPConfig::lossless)
		.field("quality", &SimpleWebPConfig::quality);

	function("encoder_version", &encoder_version);
	function("encodeRGB", &encodeRGB);
	function("encodeRGBA", &encodeRGBA);
	function("encode", &encode);
	function("encodeAnimation", &encodeAnimation);

	function("decoder_version", &decoder_version);
	function("decodeRGB", &decodeRGB);
	function("decodeRGBA", &decodeRGBA);
	function("decode", &decode);
}
