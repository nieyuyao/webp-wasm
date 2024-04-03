#include "emscripten/emscripten.h"
#include "emscripten/bind.h"
#include "encode.h"
#include "decode.h"

EMSCRIPTEN_BINDINGS(my_module)
{
	function("encoder_version", &encoder_version);
	function("decoder_version", &decoder_version);
	function("encodeRGB", &encodeRGB);
	function("encodeRGBA", &encodeRGBA);
	function("encode", &encode);
	function("decode", &decode);
}
