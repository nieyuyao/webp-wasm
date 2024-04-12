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
		if (!success) {
			return val::null();
		}
	}

	WebPData webp_data;
	WebPAnimEncoderAssemble(enc, &webp_data);
	val encoded_data = Uint8Array.new_(typed_memory_view(webp_data.size, webp_data.bytes));
	WebPAnimEncoderDelete(enc);
	return encoded_data;
}