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