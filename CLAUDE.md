# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

webp-wasm is a pure WebAssembly/JavaScript port of libwebp that enables WebP encoding/decoding (including animated WebP) in browsers and Node.js without native dependencies.

## Build Commands

```bash
# Full build (ESM with TypeScript)
npm run build

# Development build (outputs to src/ for hot reload)
npm run build-wasm:dev

# Build variants
npm run build:esm      # ESM module build
npm run build:cjs      # CommonJS build

# Run tests
npm test

# Development server (Vite)
npm run dev
```

## Architecture

### Build Pipeline

```
libwebp (C) → Emscripten → webp-wasm.wasm + webp-wasm.js → TypeScript wrapper → dist/
```

1. **libwebp/** - Vendored libwebp v1.3.2 source (git submodule)
2. **webp/** - C++ wrapper using Emscripten bindings (`emscripten/bind.h`)
3. **src/index.ts** - TypeScript API that loads and wraps the WASM module
4. **Makefile** - Orchestrates WASM compilation using emcc/em++

### Key Files

| File | Purpose |
|------|---------|
| `webp/webp.cpp` | Emscripten bindings (EMSCRIPTEN_BINDINGS macro) |
| `webp/encode.cpp` | WebP encoding including animation |
| `webp/decode.cpp` | WebP decoding including animation |
| `src/index.ts` | Public TypeScript API |
| `src/types.d.ts` | TypeScript type definitions |
| `build.sh` | Build script with emsdk auto-setup |

### Dependencies (Git Submodules)

| Submodule | Version | Source |
|-----------|---------|--------|
| libwebp | v1.3.2 | chromium.googlesource.com/webm/libwebp |
| emsdk | 3.1.74 | github.com/emscripten-core/emsdk |

The build script automatically installs emsdk if `EMSDK` environment variable is not set.

## API Structure

The library exports async functions that load the WASM module on first call:

- **Encoding**: `encodeRGB`, `encodeRGBA`, `encode`, `encodeAnimation`
- **Decoding**: `decodeRGB`, `decodeRGBA`, `decodeAnimation`
- **Utilities**: `encoderVersion`, `decoderVersion`

Animation encoding supports per-frame quality settings via `WebPAnimationFrame.config`.

## Testing

Tests use Jest with experimental WASM module support:
```bash
npm test                           # Run all tests
npm test -- tests/encode.test.js   # Run single test file
```

Test fixtures are PNG images in `tests/fixtures/`.
