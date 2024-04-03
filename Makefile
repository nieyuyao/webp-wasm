SRC = src
CODEC_DIR = libwebp
WASM_OUT_DIR = dist
EMSDK_INCLUDE_DIR = emsdk/upstream/emscripten/cache/sysroot/include

.PHONY: clean

webp-wasm.js: webp.o decode.o encode.o version.o ${CODEC_DIR}/libwebp.a ${CODEC_DIR}/libsharpyuv.a
	emcc \
		-lembind \
		-s EXPORT_ES6=1 \
		-s ALLOW_MEMORY_GROWTH=1 \
		-o $(WASM_OUT_DIR)/$@ \
		$+ \
		-v

%.o: src/%.cpp
	em++ -c \
	-std=c++17 \
	-I ${CODEC_DIR} \
	-I ${EMSDK_INCLUDE_DIR} \
	-o $@ \
	-v \
	$<

%/libwebp.a %/libsharpyuv.a: %/Makefile
	$(MAKE) -C $(@D)

$(CODEC_DIR)/Makefile: ${CODEC_DIR}/CMakeLists.txt
	emcmake cmake \
		-DCMAKE_DISABLE_FIND_PACKAGE_Threads=1 \
		-DWEBP_BUILD_ANIM_UTILS=0 \
		-DWEBP_BUILD_CWEBP=0 \
		-DWEBP_BUILD_DWEBP=0 \
		-DWEBP_BUILD_GIF2WEBP=0 \
		-DWEBP_BUILD_IMG2WEBP=0 \
		-DWEBP_BUILD_VWEBP=0 \
		-DWEBP_BUILD_WEBPINFO=0 \
		-DWEBP_BUILD_WEBPMUX=0 \
		-DWEBP_BUILD_EXTRAS=0 \
		-B $(@D) \
		$(<D)

clean:
	$(RM) $(CODEC_DIR)/Makefile
	$(RM) $(CODEC_DIR)/*.a
	$(RM) *.o
	$(RM) $(WASM_OUT_DIR)/webp-wasm.js $(WASM_OUT_DIR)/webp-wasm.wasm
