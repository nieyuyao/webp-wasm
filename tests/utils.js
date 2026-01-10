/**
 * 
 * @param {Buffer} source 
 * @param {Buffer} target 
 */
export const matchBuffer = (source, target, limit = 0) => {
	if (source.length !== target.length) {
		return false
	}
	let diffCount = 0
	source.forEach((u8, idx) => {
		if (u8 !== target[idx]) {
			diffCount++
		}
	})
	return diffCount / source.length <= limit
}

export const getLoopCount = (webpData) => {
	const animSignature = [0x41, 0x4E, 0x49, 0x4D] // 'ANIM'
	for (let i = 0; i < webpData.length - 14; i++) {
		if (webpData[i] === animSignature[0] &&
			webpData[i + 1] === animSignature[1] &&
			webpData[i + 2] === animSignature[2] &&
			webpData[i + 3] === animSignature[3]) {
			const loopCountOffset = i + 12
			return webpData[loopCountOffset] | (webpData[loopCountOffset + 1] << 8)
		}
	}
	return null
}