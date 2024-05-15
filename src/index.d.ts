import type {
  WebPConfig,
  Nullable,
  WebPAnimationFrame,
  WebPDecodedImageData,
  DecodedWebPAnimationFrame,
} from './types'
export declare const encoderVersion: () => Promise<string>
export declare const encodeRGB: (
  rgb: Uint8Array,
  width: number,
  height: number,
  quality?: number
) => Promise<Nullable<Uint8Array>>
export declare const encodeRGBA: (
  rgba: Uint8Array,
  width: number,
  height: number,
  quality?: number
) => Promise<Nullable<Uint8Array>>
export declare const encode: (
  data: Uint8Array,
  width: number,
  height: number,
  hasAlpha: boolean,
  config: Partial<WebPConfig>
) => Promise<Nullable<Uint8Array>>
export declare const encodeAnimation: (
  width: number,
  height: number,
  hasAlpha: boolean,
  frames: WebPAnimationFrame[]
) => Promise<Nullable<Uint8Array>>
export declare const decoderVersion: () => Promise<string>
export declare const decodeRGB: (data: Uint8Array) => Promise<Nullable<WebPDecodedImageData>>
export declare const decodeRGBA: (data: Uint8Array) => Promise<Nullable<WebPDecodedImageData>>
export declare const decodeAnimation: (
  data: Uint8Array,
  hasAlpha: boolean
) => Promise<Nullable<DecodedWebPAnimationFrame[]>>
export type * from './types'
