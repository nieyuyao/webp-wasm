DIST_DIR="./dist"
WASM_OUT_DIR="./dist/cjs"
EXPORT_ES6=0
CLEAN_WASM_DIR=${WASM_OUT_DIR}

if [ ! -d $DIST_DIR ]; then
  mkdir $DIST_DIR
fi


if [ $1 == "-dev" ]; then
	WASM_OUT_DIR="./src"
	CLEAN_WASM_DIR="${WASM_OUT_DIR}/webp-wasm.*"
	EXPORT_ES6=1
elif [ $1 == "-es6" ]; then
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