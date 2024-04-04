export const webpConfig = {
	lossless: {
		type: 'input',
		min: 0,
		max: 1
	},
	quality: {
		type: 'input',
		min: 0,
		max: 100,
	},
	method: {
		type: 'input',
		min: 0,
		max: 6,
	},
	image_hint: {
		type: 'input',
		min: 0,
		max: 4,
	},
	target_size: {
		type: 'select',
		options: [0, 1]
	},
	target_PSNR: {
		type: 'select',
		options: [0, 1]
	},
	segments: {
		type: 'input',
		min: 1,
		max: 4,
	},
	sns_strength: {
		type: 'input',
		min: 0,
		options: 100,
	},
	filter_strength: {
		type: 'input',
		min: 0,
		options: 100,
	},
	filter_sharpness: {
		type: 'input',
		min: 0,
		max: 7,
	},
	filter_type: {
		type: 'select',
		options: [0, 1]
	},
	autofilter: {
		type: 'select',
		options: [0, 1]
	},
	alpha_compression: {
		type: 'select',
		options: [0, 1]
	},
	alpha_filtering: {
		type: 'select',
		options: [0, 1, 2]
	},
	alpha_quality: {
		type: 'input',
		min: 0,
		options: 100,
	},
	pass: {
		type: 'input',
		min: 1,
		max: 10,
	},
	show_compressed: {
		type: 'select',
		options: [0, 1]
	},
	preprocessing: {
		type: 'select',
		options: [0, 1, 2]
	},
	partitions: {
		type: 'select',
		options: [0, 1, 2, 3]
	},
	partition_limit: {
		type: 'input',
		min: 1,
		max: 100,
	},
	emulate_jpeg_size: {
		type: 'select',
		options: [0, 1]
	},
	thread_level: {
		type: 'select',
		options: [0, 1]
	},
	low_memory: {
		type: 'select',
		options: [0, 1]
	},
	near_lossless: {
		type: 'input',
		min: 1,
		max: 100,
	},
	exact: {
		type: 'select',
		options: [0, 1]
	},
	use_delta_palette: {
		type: 'select',
		options: [0, 1]
	},
	use_sharp_yuv: {
		type: 'select',
		options: [0, 1]
	},
	qmin: {
		type: 'input',
		min: 0,
		max: 100,
	},
	qmax: {
		type: 'input',
		min: 0,
		max: 100,
	},
}