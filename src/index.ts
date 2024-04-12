import type {
  WebPConfig,
  Nullable,
  WebPAnimationFrame,
  WebPDecodedImageData,
  DecodedWebPAnimationFrame,
} from './types'
// @ts-ignore
import Module from './webp-wasm'

// default webp config
const defaultWebpConfig: WebPConfig = {
  lossless: 0,
  quality: 100,
}

export const encoderVersion = async (): Promise<string> => {
  const module = await Module()
  return module.encoder_version()
}

export const encodeRGB = async (
  rgb: Uint8Array,
  width: number,
  height: number,
  quality?: number
): Promise<Nullable<Uint8Array>> => {
  const module = await Module()
  quality = typeof quality !== 'number' ? 100 : Math.min(100, Math.max(0, quality))
  return module.encodeRGB(rgb, width, height, quality)
}

export const encodeRGBA = async (
  rgba: Uint8Array,
  width: number,
  height: number,
  quality?: number
): Promise<Nullable<Uint8Array>> => {
  const module = await Module()
  quality = typeof quality !== 'number' ? 100 : Math.min(100, Math.max(0, quality))
  return module.encodeRGBA(rgba, width, height, quality)
}

export const encode = async (
  data: Uint8Array,
  width: number,
  height: number,
  hasAlpha: boolean,
  config: Partial<WebPConfig>
): Promise<Nullable<Uint8Array>> => {
  const module = await Module()
  const webpConfig = {
    ...defaultWebpConfig,
    ...config,
  }
  webpConfig.lossless = Math.min(1, Math.max(0, webpConfig.lossless))
  webpConfig.quality = Math.min(100, Math.max(0, webpConfig.quality))
  return module.encode(data, width, height, hasAlpha, webpConfig)
}

export const encodeAnimation = async (
  width: number,
  height: number,
  hasAlpha: boolean,
  frames: WebPAnimationFrame[]
): Promise<Nullable<Uint8Array>> => {
  const module = await Module()
  const durations: number[] = []
  const dataLength = frames.reduce((acc, frame) => {
    acc += frame.data.length
    return acc
  }, 0)
  const data: Uint8Array = new Uint8Array(dataLength)
  let offset = 0
  frames.forEach((frame) => {
    data.set(frame.data, offset)
    offset += frame.data.length
    durations.push(frame.duration)
  })
  return module.encodeAnimation(width, height, hasAlpha, durations, data)
}

export const decoderVersion = async (): Promise<string> => {
  const module = await Module()
  return module.decoder_version()
}

export const decodeRGB = async (data: Uint8Array): Promise<Nullable<WebPDecodedImageData>> => {
  const module = await Module()
  return module.decodeRGB(data)
}

export const decodeRGBA = async (data: Uint8Array): Promise<Nullable<WebPDecodedImageData>> => {
  const module = await Module()
  return module.decodeRGBA(data)
}

// TODO:
// export const decode = async (data: Uint8Array, hasAlpha: boolean) => {
// 	const module = await Module()
// 	return module.decode(data, hasAlpha)
// }

export const decodeAnimation = async (
  data: Uint8Array,
  hasAlpha: boolean
): Promise<Nullable<DecodedWebPAnimationFrame[]>> => {
  const module = await Module()
  return module.decodeAnimation(data, hasAlpha)
}
