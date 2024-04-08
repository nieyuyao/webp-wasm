import path from 'path'
import fs from 'fs'
import url from 'url'
import { createCanvas, Image } from '@napi-rs/canvas'
import { encoderVersion, encodeRGB, encodeRGBA } from '../dist/esm'
import { matchBuffer } from './utils'

const __dirname = path.dirname(url.fileURLToPath(import.meta.url))

describe('encode', () => {
	test('version', async () => {
		await expect(encoderVersion()).resolves.toBe('1.3.2')
	})

	test('encodeRGB', async () => {
		const canvas = createCanvas(24, 24)
		const ctx = canvas.getContext('2d')
		const img = new Image()
		await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/test1.png'))
		})
		ctx.drawImage(img, 0, 0)
		const source = ctx.getImageData(0, 0, img.width, img.height)
		const buf = Buffer.alloc(3 * img.width * img.height)
		let j = 0
		// remove alpha
		source.data.forEach((pixel, i) => {
			if ((i + 1) % 4 === 0) {
				return
			}
			buf[j] = pixel
			j++
		})
		const webpData = await encodeRGB(buf, img.width, img.height)
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/test1-rgb.webp')))
		).toBeTruthy()
	})

	test('encodeRGBA', async () => {
		const canvas = createCanvas(24, 24)
		const ctx = canvas.getContext('2d')
		const img = new Image()
		await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/test1.png'))
		})
		ctx.drawImage(img, 0, 0)
		const source = ctx.getImageData(0, 0, img.width, img.height)
		const webpData = await encodeRGBA(source.data.buffer, img.width, img.height)
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/test1.webp')))
		).toBeTruthy()
	})
})
