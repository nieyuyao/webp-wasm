#include <string.h>
#include "version.h"
#include "decode.h"

using namespace emscripten;

val decoder_version()
{
  return get_version(WebPGetDecoderVersion());
}

val decodeRGB(std::string data)
{
  size_t data_size = data.size();
  int width;
  int height;
  uint8_t *output = WebPDecodeRGB((uint8_t *)data.c_str(), data_size, &width, &height);
  val obj = val::object();
  obj.set("width", val(width));
  obj.set("height", val(height));
  obj.set("data", val::global("Uint8Array").new_(typed_memory_view(3 * width * height, output)));
  return obj;
}

val decodeRGBA(std::string data)
{
  size_t data_size = data.size();
  int width;
  int height;
  uint8_t *output = WebPDecodeRGBA((uint8_t *)data.c_str(), data_size, &width, &height);
  val obj = val::object();
  obj.set("width", width);
  obj.set("height", height);
  obj.set("data", val::global("Uint8Array").new_(typed_memory_view(4 * width * height, output)));
  return obj;
}

val decode(std::string data, bool has_alpha)
{
  uint8_t* buf = (uint8_t *)data.c_str();
  int data_size = data.size();
  WebPBitstreamFeatures features;
  if (!WebPGetFeatures(buf, data_size, &features))
  {
    return val::null();
  }
  WebPDecoderConfig config;
  config.input.has_alpha = has_alpha;
  config.output.width = features.width;
  config.output.height = features.height;
  // only support ARGB and RGB !!!
  config.output.colorspace = has_alpha ? MODE_ARGB : MODE_RGB;

  auto success = WebPDecode(buf, data_size, &config);
  if (!success)
  {
    WebPFreeDecBuffer(&config.output);
    return val::null();
  }
  WebPFreeDecBuffer(&config.output);
  val raw = val::global("Uint8Array").new_(typed_memory_view(config.output.u.RGBA.size, config.output.u.RGBA.rgba));
  val obj = val::object();
  obj.set("width", features.width);
  obj.set("height", features.height);
  obj.set("data", raw);
  return obj;
}

val decodeAnimation(std::string data, bool has_alpha)
{
  WebPData webp_data;
  size_t data_size = data.size();
  webp_data.bytes = (uint8_t *)data.c_str();
  webp_data.size = data_size;
  WebPAnimDecoderOptions dec_options;
  dec_options.color_mode = has_alpha ? MODE_RGBA : MODE_RGB;
  WebPAnimDecoderOptionsInit(&dec_options);
  WebPAnimDecoder *dec = WebPAnimDecoderNew(&webp_data, &dec_options);
  if (dec == nullptr)
  {
    return val::null();
  }
  WebPAnimInfo anim_info;
  WebPAnimDecoderGetInfo(dec, &anim_info);
  val frames = val::array();
  size_t frame_size = (has_alpha ? 4 : 3) * anim_info.canvas_width * anim_info.canvas_height;
  size_t frame_num = 0;
  int prev_timestamp = 0;
  while (WebPAnimDecoderHasMoreFrames(dec))
  {
    uint8_t *buf;
    int timestamp;
    WebPAnimDecoderGetNext(dec, &buf, &timestamp);
    val frame = val::object();
    val raw = val::global("Uint8Array").new_(typed_memory_view(frame_size, buf));
    frame.set("width", anim_info.canvas_width);
    frame.set("height", anim_info.canvas_height);
    frame.set("data", raw);
    frame.set("duration", timestamp - prev_timestamp);
    frames.set(frame_num, frame);
    frame_num++;
    prev_timestamp = timestamp;
  }
  WebPAnimDecoderDelete(dec);
  return frames;
}