#include <string.h>
#include "version.h"
#include "encode.h"

using namespace emscripten;
thread_local val Uint8Array = val::global("Uint8Array");

val encoder_version()
{
	return get_version(WebPGetEncoderVersion());
}

val encodeRGB(std::string rgb, int width, int height, int quality_factor) {
	uint8_t* output;
	size_t size = WebPEncodeRGB((uint8_t*)rgb.c_str(), width, height, 3 * width, quality_factor, &output);
	return Uint8Array.new_(typed_memory_view(size, output));
}

val encodeRGBA(std::string rgba, int width, int height, int quality_factor) {
	uint8_t* output;
	size_t size = WebPEncodeRGBA((uint8_t*)rgba.c_str(), width, height, 4 * width, quality_factor, &output);
	return Uint8Array.new_(typed_memory_view(size, output));
}

val encode(std::string data, int width, int height, bool has_alpha, SimpleWebPConfig config)
{
	WebPConfig webp_config;
	WebPConfigInit(&webp_config);

	webp_config.quality = config.quality;
	webp_config.lossless = config.lossless;

	WebPMemoryWriter wrt;
	WebPMemoryWriterInit(&wrt);

	WebPPicture pic;
	if (!WebPPictureInit(&pic))
	{
		return val::null();
	}
	// only support argb!!!
	pic.use_argb = 1;
	pic.width = width;
	pic.height = height;
	if (!WebPPictureAlloc(&pic))
	{
		WebPPictureFree(&pic);
		return val::null();
	}
	pic.writer = WebPMemoryWrite;
	pic.custom_ptr = &wrt;
	has_alpha
		? WebPPictureImportRGBA(&pic, (uint8_t*)data.c_str(), width * 4)
		: WebPPictureImportRGB(&pic, (uint8_t*)data.c_str(), width * 3);
	int success = WebPEncode(&webp_config, &pic);
	if (!success)
	{
		return val::null();
	}
	val encoded_data = Uint8Array.new_(typed_memory_view(wrt.size, wrt.mem));
	WebPPictureFree(&pic);
	WebPMemoryWriterClear(&wrt);
	return encoded_data;
}

val encodeAnimation(int width, int height, bool has_alpha, val durations, std::string data)
{
	WebPAnimEncoderOptions enc_options;
	WebPAnimEncoderOptionsInit(&enc_options);
	WebPAnimEncoder* enc = WebPAnimEncoderNew(width, height, &enc_options);
	auto frame_durations = vecFromJSArray<int>(durations);
	int frames = frame_durations.size();
	int frame_data_size = (has_alpha ? 4 : 3) * width * height;
	int stride = (has_alpha ? 4 : 3) * width;
	int timestamp = 0;
	for (int i = 0; i < frames; i++)
	{
		WebPConfig config;
		WebPConfigInit(&config);
		WebPPicture pic;
		if (!WebPPictureInit(&pic))
		{
			WebPPictureFree(&pic);
			return val::null();
		}
		auto pos = i * frame_data_size;
		auto pic_data = data.substr(pos, frame_data_size);
		pic.use_argb = 1;
		pic.width = width;
		pic.height = height;
		has_alpha
			? WebPPictureImportRGBA(&pic, (uint8_t*)pic_data.c_str(), stride)
			: WebPPictureImportRGB(&pic, (uint8_t*)pic_data.c_str(), stride);
		int success = WebPAnimEncoderAdd(enc, &pic, timestamp, &config);
		timestamp = timestamp + frame_durations[i];
		WebPPictureFree(&pic);
		if (!success) {
			WebPAnimEncoderDelete(enc);
			return val::null();
		}
	}

	WebPAnimEncoderAdd(enc, NULL, timestamp, NULL);

	WebPData webp_data;
	WebPDataInit(&webp_data);
	if (!WebPAnimEncoderAssemble(enc, &webp_data)) {
		WebPAnimEncoderDelete(enc);
		return val::null();
	}
	val encoded_data = Uint8Array.new_(typed_memory_view(webp_data.size, webp_data.bytes));
	WebPDataClear(&webp_data);
	WebPAnimEncoderDelete(enc);
	return encoded_data;
}

val encodeAnimationEx(
	int width,
	int height,
	bool has_alpha,
	val durations,
	val frame_configs,
	std::string data,
	AnimationOptions options
)
{
	WebPAnimEncoderOptions enc_options;
	WebPAnimEncoderOptionsInit(&enc_options);
	enc_options.minimize_size = options.minimize_size;
	enc_options.allow_mixed = options.allow_mixed;
	if (options.kmax > 0) {
		enc_options.kmin = options.kmin;
		enc_options.kmax = options.kmax;
	}
	enc_options.anim_params.loop_count = options.loop_count;
	enc_options.anim_params.bgcolor = options.bgcolor;

	WebPAnimEncoder* enc = WebPAnimEncoderNew(width, height, &enc_options);
	if (!enc) {
		return val::null();
	}

	auto frame_durations = vecFromJSArray<int>(durations);
	int frames = frame_durations.size();
	int frame_data_size = (has_alpha ? 4 : 3) * width * height;
	int stride = (has_alpha ? 4 : 3) * width;
	int timestamp = 0;

	for (int i = 0; i < frames; i++)
	{
		WebPConfig config;
		WebPConfigInit(&config);
		if (frame_configs.isArray() && i < frame_configs["length"].as<int>()) {
			val fc = frame_configs[i];
			if (!fc["use_default"].as<bool>()) {
				config.lossless = fc["lossless"].as<int>();
				config.quality = fc["quality"].as<float>();
			}
		}

		WebPPicture pic;
		if (!WebPPictureInit(&pic))
		{
			WebPAnimEncoderDelete(enc);
			return val::null();
		}

		auto pos = i * frame_data_size;
		auto pic_data = data.substr(pos, frame_data_size);
		pic.use_argb = 1;
		pic.width = width;
		pic.height = height;

		int import_success = has_alpha
			? WebPPictureImportRGBA(&pic, (uint8_t*)pic_data.c_str(), stride)
			: WebPPictureImportRGB(&pic, (uint8_t*)pic_data.c_str(), stride);

		if (!import_success) {
			WebPPictureFree(&pic);
			WebPAnimEncoderDelete(enc);
			return val::null();
		}

		int success = WebPAnimEncoderAdd(enc, &pic, timestamp, &config);
		timestamp = timestamp + frame_durations[i];
		WebPPictureFree(&pic);

		if (!success) {
			WebPAnimEncoderDelete(enc);
			return val::null();
		}
	}

	WebPAnimEncoderAdd(enc, NULL, timestamp, NULL);

	WebPData webp_data;
	WebPDataInit(&webp_data);
	if (!WebPAnimEncoderAssemble(enc, &webp_data)) {
		WebPAnimEncoderDelete(enc);
		return val::null();
	}

	val encoded_data = Uint8Array.new_(typed_memory_view(webp_data.size, webp_data.bytes));
	WebPDataClear(&webp_data);
	WebPAnimEncoderDelete(enc);
	return encoded_data;
}