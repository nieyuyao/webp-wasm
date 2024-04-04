import { Webp } from '../dist/esm'

describe('decode', () => {
	test('decodeRGBA', async () => {
		await expect(Webp.decoderVersion()).resolves.toBe('1.3.2')
	})
})