#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "emscripten/emscripten.h"
#include "webp.h"


EMSCRIPTEN_KEEPALIVE
int encoder_version() {
  return WebPGetEncoderVersion();
}

EMSCRIPTEN_KEEPALIVE
int decoder_version() {
  return WebPGetDecoderVersion();
}



EMSCRIPTEN_KEEPALIVE
uint8_t* encode(const uint8_t* data, int width, int height, WebPConfig* config) {
    WebPConfig webp_config;
    WebPConfigInit(&webp_config);
    WebPPicture pic;
    WebPPictureInit(&pic);
    uint8_t* ptr = malloc((size_t)(1));
    printf("%d", width);
    printf("%d", height);
    printf("%s", data);
    return ptr;
}

EMSCRIPTEN_KEEPALIVE
uint8_t* decode(const uint8_t* webp_data) {
    WebPBitstreamFeatures input;
    uint8_t* output = NULL;
    VP8StatusCode status;
    size_t webp_data_size = 1;
    status = WebPGetFeatures(webp_data, webp_data_size, &input);
    int width;
    int height;
    width = input.width;
    height = input.height;
    WebPDecodeBGRA(webp_data, webp_data_size, &width, &height);

    uint8_t* ptr = malloc((size_t)(1));
    return ptr;
}