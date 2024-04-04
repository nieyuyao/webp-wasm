import { Webp } from '../dist/esm'

describe('encode', () => {
	test('encodeVersion', async () => {
		await expect(Webp.encoderVersion()).resolves.toBe('1.3.2')
	})
})