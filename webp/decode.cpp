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


val webp_decode(const uint8_t *data, size_t data_size, bool has_alpha) {
  WebPBitstreamFeatures features;
  if (!WebPGetFeatures(data, data_size, &features))
  {
    return val::null();
  }
  WebPDecoderConfig config;
  config.input.has_alpha = has_alpha;
  config.output.width = features.width;
  config.output.height = features.height;
  // only support ARGB and RGB !!!
  config.output.colorspace = has_alpha ? MODE_ARGB : MODE_RGB;

  auto success = WebPDecode(data, data_size, &config);
  if (!success)
  {
    WebPFreeDecBuffer(&config.output);
    return val::null();
  }
  WebPFreeDecBuffer(&config.output);
  val decoded_data = val::global("Uint8Array").new_(typed_memory_view(config.output.u.RGBA.size, config.output.u.RGBA.rgba));
  val obj = val::object();
  obj.set("width", features.width);
  obj.set("height", features.height);
  obj.set("data", decoded_data);
  return obj;
}

val decode(std::string data, bool has_alpha)
{
  return webp_decode((uint8_t *)data.c_str(), data.size(), has_alpha);
}

val decodeAnimation(std::string data, bool has_alpha)
{
  WebPData wep_data;
  size_t data_size = data.size();
  wep_data.bytes = (uint8_t *)data.c_str();
  WebPDemuxer *demux = WebPDemux(&wep_data);
  uint32_t width = WebPDemuxGetI(demux, WEBP_FF_CANVAS_WIDTH);
  uint32_t height = WebPDemuxGetI(demux, WEBP_FF_CANVAS_HEIGHT);
  WebPIterator iter;
  val frames = val::array();
  if (WebPDemuxGetFrame(demux, 1, &iter))
  {
    do
    {
      val frame = webp_decode(iter.fragment.bytes, iter.fragment.size, has_alpha);
      if (frame.isNull()) {
        // skip
        continue;
      }
      frames.set(iter.frame_num, frame);
    } while (WebPDemuxNextFrame(&iter));
    WebPDemuxReleaseIterator(&iter);
  } else {
    return val::null();
  }
  return frames;
}