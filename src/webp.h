#include "src/webp/encode.h"
#include "src/webp/decode.h"

uint8_t* encode(const uint8_t* rgb, int width, int height, WebPConfig* config);

uint8_t* decode(const uint8_t* webp_data);
