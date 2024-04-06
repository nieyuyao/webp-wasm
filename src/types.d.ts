export interface WebPConfig {
	lossless: number
	quality: number
}

export interface WebPAnimationFrame {
	data: Uint8ClampedArray,
	duration: number
}

export type Nullable<T> = T | null