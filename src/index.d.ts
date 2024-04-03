import { Nullable, WebPConfig } from './types'

export declare class Webp {
	static encoderVersion(): Promise<string>;
	static encodeRGB(rgb: Uint8Array, width: number, height: number, quality: number): Promise<Nullable<Uint8Array>>;
	static encodeRGBA(rgba: Uint8Array, width: number, height: number, quality: number): Promise<Nullable<Uint8Array>>;
	static encode(data: Uint8Array, width: number, height: number, config: WebPConfig): Promise<Nullable<Uint8Array>>;
	static decoderVersion(): Promise<any>;
	static decodeRGB(rgb: Uint8Array, width: number, height: number, quality: number): void;
	static decodeRGBA(rgb: Uint8Array, width: number, height: number, quality: number): void;
	static decode(data: Uint8Array, width: number, height: number, config: WebPConfig): void;
}