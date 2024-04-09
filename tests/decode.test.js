import path from 'path'
import fs from 'fs'
import url from 'url'
import { createCanvas, Image } from '@napi-rs/canvas'
import { decoderVersion, decodeRGBA, decodeRGB } from '../dist/esm'
import { matchBuffer } from './utils'

const __dirname = path.dirname(url.fileURLToPath(import.meta.url))

describe('decode', () => {
	test('version', async () => {
		await expect(decoderVersion()).resolves.toBe('1.3.2')
	})

  test('decodeRGB', async () => {
    const webpData = fs.readFileSync(path.resolve(__dirname, './imgs/success-rgb.webp'))
    const decoded = await decodeRGB(webpData)
    const canvas = createCanvas(decoded.width, decoded.height)
		const ctx = canvas.getContext('2d')
    const img = new Image()
    await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './fixtures/success.png'))
		})
    const buf = new Uint8Array(3 * img.width * img.height)
    ctx.drawImage(img, 0, 0)
    const target = ctx.getImageData(0, 0, img.width, img.height)
    let j = 0
    target.data.forEach((pixel, i) => {
      if ((i + 1) % 4 === 0) {
        return
      }
      buf[j] = pixel
      j++
    })
    expect(
			matchBuffer(decoded.data, buf)
		).toBeTruthy()
  })

  test('decodeRGBA', async () => {
    const webpData = fs.readFileSync(path.resolve(__dirname, './imgs/success.webp'))
    const decoded = await decodeRGBA(webpData)
    const canvas = createCanvas(decoded.width, decoded.height)
		const ctx = canvas.getContext('2d')
    const img = new Image()
    await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './fixtures/success.png'))
		})
    ctx.drawImage(img, 0, 0)
    const target = ctx.getImageData(0, 0, img.width, img.height)
    expect(
			matchBuffer(decoded.data, target.data)
		).toBeTruthy()
  })
})