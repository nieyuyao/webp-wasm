export interface WebPConfig {
	lossless: number
	quality: number
}

export interface WebPAnimationOptions {
	minimize_size?: boolean
	kmin?: number
	kmax?: number
	allow_mixed?: boolean
	loop_count?: number
	bgcolor?: number
}

export interface WebPFrameConfig {
	lossless?: number
	quality?: number
}

export interface WebPAnimationFrame {
	data: Uint8Array
	duration: number
	config?: WebPFrameConfig
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