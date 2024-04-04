import type { WebPConfig, Nullable } from './types'
// @ts-ignore
import Module from '../wasm/webp-wasm'

// default webp config
const defaultWebpConfig: WebPConfig = {
	lossless: 0,
	quality: 100,
	method: 4,
	image_hint: 0,
	target_size: 0,
	target_PSNR: 0,
	segments: 4,
	sns_strength: 50,
	filter_strength: 60,
	filter_sharpness: 0,
	filter_type: 1,
	autofilter: 0,
	alpha_compression: 1,
	alpha_filtering: 1,
	alpha_quality: 1,
	pass: 1,
	show_compressed: 0,
	preprocessing: 0,
	partitions: 0,
	partition_limit: 0,
	emulate_jpeg_size: 0,
	thread_level: 0,
	low_memory: 0,
	near_lossless: 100,
	exact: 0,
	use_delta_palette: 0,
	use_sharp_yuv: 0,
	qmin: 0,
	qmax: 100,
}

export class Webp {
	static async encoderVersion(): Promise<string> {
		const module = await Module()
		return module.encoder_version()
	}
	static async encodeRGB(rgb: Uint8Array, width: number, height: number, quality: number): Promise<Nullable<Uint8Array>> {
		const module = await Module()
		quality = Math.min(Math.max(0, quality))
		return module.encodeRGB(rgb, width, height, quality)
	}
	static async encodeRGBA(rgba: Uint8Array, width: number, height: number, quality: number): Promise<Nullable<Uint8Array>> {
		const module = await Module()
		quality = Math.min(Math.max(0, quality))
		return module.encodeRGBA(rgba, width, height, quality)
	}
	static async encode(data: Uint8Array, width: number, height: number, config: WebPConfig): Promise<Nullable<Uint8Array>> {
		const module = await Module()
		const webpConfig = {
			defaultWebpConfig,
			...config,
		}
		return module.encode(data, width, height, webpConfig)
	}

	static async decoderVersion() {
		const module = await Module()
		return module.decoder_version()
	}
	static async decodeRGB(rgb: Uint8Array, width: number, height: number) {
		const module = await Module()
		return module.decodeRGB(rgb, width, height)
	}
	static async decodeRGBA(rgba: Uint8Array, width: number, height: number) {
		const module = await Module()
		return module.decodeRGBA(rgba, width, height)
	}
	// TODO:
	// static decode(data: Uint8Array, width: number, height: number, config: WebPConfig) {
	// 	const module = await Module()
	// 	quality = Math.min(Math.max(0, quality))
	// 	return module.decode(rgb, width, height, quality)
	// }
}
