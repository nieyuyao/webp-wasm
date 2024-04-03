export interface WebPConfig {
	lossless: number
	quality: number
	method: number
	image_hint: number
	target_size: number
	target_PSNR: number
	segments: number
	sns_strength: number
	filter_strength: number
	filter_sharpness: number
	filter_type: number
	autofilter: number
	alpha_compression: number
	alpha_filtering: number
	alpha_quality: number
	pass: number
	show_compressed: number
	preprocessing: number
	partitions: number
	partition_limit: number
	emulate_jpeg_size: number
	thread_level: number
	low_memory: number
	near_lossless: number
	exact: number
	use_delta_palette: number
	use_sharp_yuv: number
	qmin: number
	qmax: number
}

export type Nullable<T> = T | null