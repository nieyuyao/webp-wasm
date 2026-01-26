export interface WebPConfig {
	lossless: number
	quality: number
}

export interface WebPAnimationFrame {
	data: Uint8Array
	duration: number
	config?: WebPConfig
}

export interface DecodedWebPAnimationFrame extends WebPAnimationFrame {
  width: number
  height: number
}

export interface WebPDecodedImageData {
  width: number
  height: number
  data: Uint8Array
}

export type Nullable<T> = T | null