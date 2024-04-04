#include <string.h>
#include "version.h"
#include "encode.h"

using namespace emscripten;
thread_local val Uint8Array = val::global("Uint8Array");

void merge_webp_config(WebPConfig target, WebPConfig source)
{
	target.quality = source.quality;
	target.lossless = source.lossless;
	target.method = source.method;
	target.image_hint = source.image_hint;
	target.target_size = source.target_size;
	target.target_PSNR = source.target_PSNR;
	target.segments = source.segments;
	target.sns_strength = source.sns_strength;
	target.filter_strength = source.filter_strength;
	target.filter_sharpness = source.filter_sharpness;
	target.filter_type = source.filter_type;
	target.autofilter = source.autofilter;
	target.alpha_compression = source.alpha_compression;
	target.alpha_filtering = source.alpha_filtering;
	target.alpha_quality = source.alpha_quality;
	target.pass = source.pass;
	target.show_compressed = source.show_compressed;
	target.preprocessing = source.preprocessing;
	target.partitions = source.partitions;
	target.partition_limit = source.partition_limit;
	target.emulate_jpeg_size = source.emulate_jpeg_size;
	target.thread_level = source.thread_level;
	target.near_lossless = source.near_lossless;
	target.exact = source.exact;
	target.use_delta_palette = source.use_delta_palette;
	target.use_sharp_yuv = source.use_sharp_yuv;
	target.qmin = source.qmin;
	target.qmax = source.qmax;
}

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

val encode(std::string data, int width, int height, bool has_alpha, WebPConfig config)
{
	WebPConfig webp_config;
	merge_webp_config(webp_config, config);

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
		? WebPPictureImportRGB(&pic, (uint8_t*)data.c_str(), width * 3)
		: WebPPictureImportRGBA(&pic, (uint8_t*)data.c_str(), width * 4);
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
