import { decoderVersion } from '../dist/esm'

describe('decode', () => {
	test('version', async () => {
		await expect(decoderVersion()).resolves.toBe('1.3.2')
	})
})