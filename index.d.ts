// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
interface WasmModule {
  _encode(_0: number, _1: number, _2: number, _3: number): number;
  _decode(_0: number): number;
}

export type MainModule = WasmModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
