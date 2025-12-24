#!/bin/bash

EMSDK_VERSION="3.1.74"

# Setup emsdk: use env var or local submodule
if [ -z "$EMSDK" ]; then
    echo "EMSDK not set, using local submodule..."

    # Initialize submodule if needed
    if [ ! -f "./emsdk/emsdk.py" ]; then
        echo "Initializing emsdk submodule..."
        git submodule update --init emsdk
    fi

    # Install and activate if not already done
    if [ ! -d "./emsdk/upstream" ]; then
        echo "Installing emsdk ${EMSDK_VERSION}..."
        ./emsdk/emsdk install ${EMSDK_VERSION}
        ./emsdk/emsdk activate ${EMSDK_VERSION}
    fi

    # Source emsdk environment
    source ./emsdk/emsdk_env.sh
fi

echo "Using EMSDK: $EMSDK"

DIST_DIR="./dist"
WASM_OUT_DIR="./dist/cjs"
EXPORT_ES6=0
CLEAN_WASM_DIR=${WASM_OUT_DIR}

if [ ! -d $DIST_DIR ]; then
  mkdir $DIST_DIR
fi


if [ "$1" == "-dev" ]; then
	WASM_OUT_DIR="./src"
	CLEAN_WASM_DIR="${WASM_OUT_DIR}/webp-wasm.*"
	EXPORT_ES6=1
elif [ "$1" == "-es6" ]; then
	WASM_OUT_DIR="./dist/esm"
	EXPORT_ES6=1
fi

# create ./dist directory
make -C ./ clean WASM_OUT_DIR=$WASM_OUT_DIR
mkdir $WASM_OUT_DIR

# build wasm
make -C \
	./ WASM_OUT_DIR=$WASM_OUT_DIR \
	EXPORT_ES6=$EXPORT_ES6 \
	CLEAN_WASM_DIR=$CLEAN_WASM_DIR
