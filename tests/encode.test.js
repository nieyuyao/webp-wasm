import { encoderVersion, encodeRGB } from '../'

describe('encode', () => {
	test('version', async () => {
		await expect(encoderVersion()).resolves.toBe('1.3.2')
	})

	test('encodeRGB', async () => {
		await expect(encodeRGB()).resolves.toBe('1.3.2')
	})
})