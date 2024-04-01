# create ./dist directory
if [ ! -d "./dist" ]; then
    mkdir ./dist
    else
    rm -rf ./dist/*
fi

emcc ./src/webp.c \
    -s WASM=1 -s EXPORTED_FUNCTIONS="_encode,_decode,_say_hello" \
    -I libwebp libwebp/src/**/*.c \
    -o dist/hello.html