#include "src/webp/encode.h"
#include "src/webp/mux.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"

struct SimpleWebPConfig
{
	int lossless;
	float quality;
};

struct AnimationOptions
{
	int minimize_size;
	int kmin;
	int kmax;
	int allow_mixed;
	int loop_count;
	unsigned int bgcolor;
};

struct FrameConfig
{
	int lossless;
	float quality;
	int use_default;
};

emscripten::val encoder_version();

emscripten::val encodeRGB(std::string rgb, int width, int height, int quality_factor);

emscripten::val encodeRGBA(std::string rgba, int width, int height, int quality_factor);

emscripten::val encode(std::string data, int width, int height, bool use_alpha, SimpleWebPConfig config);

emscripten::val encodeAnimation(int width, int height, bool has_alpha, emscripten::val durations, std::string data);

emscripten::val encodeAnimationEx(
	int width,
	int height,
	bool has_alpha,
	emscripten::val durations,
	emscripten::val frame_configs,
	std::string data,
	AnimationOptions options
);