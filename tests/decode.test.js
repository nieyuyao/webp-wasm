import { decoderVersion } from '../'

describe('decode', () => {
	test('version', async () => {
		await expect(decoderVersion()).resolves.toBe('1.3.2')
	})
})