# create ./dist directory
if [ ! -d "./wasm" ]; then
    mkdir ./wasm
    else
    make -C ./ clean
fi

# build wasm
make -C ./

# copy ./wasm to ./dist/wasm
cp -r ./wasm/ ./dist/wasm/