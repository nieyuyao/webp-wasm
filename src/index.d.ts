import type { WebPConfig, Nullable, WebPAnimationFrame } from './types'
export declare const encoderVersion: () => Promise<string>
export declare const encodeRGB: (
	rgb: Uint8ClampedArray,
	width: number,
	height: number,
	quality: number
) => Promise<Nullable<Uint8ClampedArray>>
export declare const encodeRGBA: (
	rgba: Uint8ClampedArray,
	width: number,
	height: number,
	quality: number
) => Promise<Nullable<Uint8ClampedArray>>
export declare const encode: (
	data: Uint8ClampedArray,
	width: number,
	height: number,
	hasAlpha: boolean,
	config: Partial<WebPConfig>
) => Promise<Nullable<Uint8ClampedArray>>
export declare const encodeAnimation: (
	width: number,
	height: number,
	hasAlpha: boolean,
	frames: WebPAnimationFrame[]
) => Promise<Nullable<Uint8ClampedArray>>
export declare const decoderVersion: () => Promise<string>
export declare const decodeRGB: (rgb: Uint8ClampedArray) => Promise<Nullable<ImageData>>
export declare const decodeRGBA: (rgba: Uint8ClampedArray) => Promise<Nullable<ImageData>>
