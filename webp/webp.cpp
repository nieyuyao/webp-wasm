#include "emscripten/emscripten.h"
#include "emscripten/bind.h"
#include "encode.h"
#include "decode.h"


EMSCRIPTEN_BINDINGS(module)
{
	emscripten::value_object<SimpleWebPConfig>("SimpleWebPConfig")
		.field("lossless", &SimpleWebPConfig::lossless)
		.field("quality", &SimpleWebPConfig::quality);

	emscripten::value_object<AnimationOptions>("AnimationOptions")
		.field("minimize_size", &AnimationOptions::minimize_size)
		.field("kmin", &AnimationOptions::kmin)
		.field("kmax", &AnimationOptions::kmax)
		.field("allow_mixed", &AnimationOptions::allow_mixed)
		.field("loop_count", &AnimationOptions::loop_count)
		.field("bgcolor", &AnimationOptions::bgcolor);

	emscripten::value_object<FrameConfig>("FrameConfig")
		.field("lossless", &FrameConfig::lossless)
		.field("quality", &FrameConfig::quality)
		.field("use_default", &FrameConfig::use_default);

	function("encoder_version", &encoder_version);
	function("encodeRGB", &encodeRGB);
	function("encodeRGBA", &encodeRGBA);
	function("encode", &encode);
	function("encodeAnimation", &encodeAnimation);
	function("encodeAnimationEx", &encodeAnimationEx);

	function("decoder_version", &decoder_version);
	function("decodeRGB", &decodeRGB);
	function("decodeRGBA", &decodeRGBA);
	function("decode", &decode);
	function("decodeAnimation", &decodeAnimation);
}
