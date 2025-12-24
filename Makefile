SRC = webp
CODEC_DIR = libwebp

# Use EMSDK env var if set, otherwise use local submodule
EMSDK_PATH ?= $(if $(EMSDK),$(EMSDK),./emsdk)
EMSDK_INCLUDE_DIR = $(EMSDK_PATH)/upstream/emscripten/cache/sysroot/include
EMCC = $(EMSDK_PATH)/upstream/emscripten/emcc
EMPP = $(EMSDK_PATH)/upstream/emscripten/em++
EMCMAKE = $(EMSDK_PATH)/upstream/emscripten/emcmake

.PHONY: clean

webp-wasm.js: webp.o decode.o encode.o version.o ${CODEC_DIR}/libwebp.a ${CODEC_DIR}/libsharpyuv.a ${CODEC_DIR}/libwebpmux.a ${CODEC_DIR}/libwebpdemux.a
	$(EMCC) \
		-lembind \
		-s EXPORT_ES6=$(EXPORT_ES6) \
		-s MODULARIZE \
		-s ALLOW_MEMORY_GROWTH=1 \
		-o $(WASM_OUT_DIR)/$@ \
		$+ \
		-v

%.o: ${SRC}/%.cpp
	$(EMPP) -c \
	-std=c++17 \
	-I ${CODEC_DIR} \
	-I ${EMSDK_INCLUDE_DIR} \
	-o $@ \
	-v \
	$<

%/libwebp.a %/libsharpyuv.a %/libwebpmux.a %/libwebpdemux.a: %/Makefile
	$(MAKE) -C $(@D)

$(CODEC_DIR)/Makefile: ${CODEC_DIR}/CMakeLists.txt
	$(EMCMAKE) cmake \
		-DCMAKE_DISABLE_FIND_PACKAGE_Threads=1 \
		-DWEBP_BUILD_CWEBP=ON \
		-DWEBP_BUILD_DWEBP=ON \
		-DWEBP_BUILD_GIF2WEBP=0 \
		-DWEBP_BUILD_IMG2WEBP=0 \
		-DWEBP_BUILD_VWEBP=0 \
		-DWEBP_BUILD_EXTRAS=0 \
		-DWEBP_BUILD_WEBPINFO=0 \
		-B $(@D) \
		$(<D)

clean:
	$(RM) $(CODEC_DIR)/Makefile
	$(RM) $(CODEC_DIR)/*.a
	$(RM) *.o
	$(RM) -rf $(CLEAN_WASM_DIR)
