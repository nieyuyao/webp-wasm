import path from 'path'
import fs from 'fs'
import url from 'url'
import { createCanvas, Image } from '@napi-rs/canvas'
import { encoderVersion, encodeRGB, encodeRGBA, encode, encodeAnimation } from '../dist/esm'
import { matchBuffer, getLoopCount } from './utils'

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
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/ok.png'))
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
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/ok-rgb.webp')))
		).toBeTruthy()
	})

	test('encodeRGBA', async () => {
		const canvas = createCanvas(24, 24)
		const ctx = canvas.getContext('2d')
		const img = new Image()
		await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/ok.png'))
		})
		ctx.drawImage(img, 0, 0)
		const source = ctx.getImageData(0, 0, img.width, img.height)
		const webpData = await encodeRGBA(source.data.buffer, img.width, img.height)
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/ok.webp')))
		).toBeTruthy()
	})

  test('encode using quality', async () => {
		const canvas = createCanvas(494, 494)
		const ctx = canvas.getContext('2d')
		const img = new Image()
		await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/dog.png'))
		})
		ctx.drawImage(img, 0, 0)
		const source = ctx.getImageData(0, 0, img.width, img.height)
		const webpData = await encode(source.data.buffer, img.width, img.height, true, { quality: 50 })
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/dog-quality-50.webp')))
		).toBeTruthy()
	})

  test('encode using lossless', async () => {
		const canvas = createCanvas(494, 494)
		const ctx = canvas.getContext('2d')
		const img = new Image()
		await new Promise((resolve) => {
			img.onload = resolve
			img.src = fs.readFileSync(path.resolve(__dirname, './imgs/dog.png'))
		})
		ctx.drawImage(img, 0, 0)
		const source = ctx.getImageData(0, 0, img.width, img.height)
		const webpData = await encode(source.data.buffer, img.width, img.height, true, { lossless: 0.8 })
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/dog-lossless-0.8.webp')))
		).toBeTruthy()
	})

  test('encode animated webp', async () => {
		const canvas = createCanvas(100, 100)
		const ctx = canvas.getContext('2d')
    const frames = []
    for (let x = 0; x <= 90; x += 10) {
      ctx.clearRect(0, 0, 100, 100)
      ctx.fillStyle = 'red'
      ctx.beginPath()
      ctx.arc(x, 50, 10, 0, 2 * Math.PI)
      ctx.closePath()
      ctx.fill()
      frames.push({
        data: ctx.getImageData(0, 0, 100, 100).data,
        duration: 1000
      })
    }
    const webpData = await encodeAnimation(100, 100, true, frames)
		expect(
			matchBuffer(webpData, fs.readFileSync(path.resolve(__dirname, './fixtures/running-ball.webp')))
		).toBeTruthy()
	})

  test('encode animated webp with loop_count=0 (infinite)', async () => {
    const canvas = createCanvas(50, 50)
    const ctx = canvas.getContext('2d')
    const frames = []
    for (let i = 0; i < 3; i++) {
      ctx.fillStyle = ['red', 'green', 'blue'][i]
      ctx.fillRect(0, 0, 50, 50)
      frames.push({
        data: ctx.getImageData(0, 0, 50, 50).data,
        duration: 100
      })
    }
    const webpData = await encodeAnimation(50, 50, true, frames, { loop_count: 0 })
    expect(webpData).not.toBeNull()
    expect(getLoopCount(webpData)).toBe(0)
  })

  test('encode animated webp with loop_count=3', async () => {
    const canvas = createCanvas(50, 50)
    const ctx = canvas.getContext('2d')
    const frames = []
    for (let i = 0; i < 3; i++) {
      ctx.fillStyle = ['red', 'green', 'blue'][i]
      ctx.fillRect(0, 0, 50, 50)
      frames.push({
        data: ctx.getImageData(0, 0, 50, 50).data,
        duration: 100
      })
    }
    const webpData = await encodeAnimation(50, 50, true, frames, { loop_count: 3 })
    expect(webpData).not.toBeNull()
    expect(getLoopCount(webpData)).toBe(3)
  })

  test('encode animated webp with loop_count=1 (play once)', async () => {
    const canvas = createCanvas(50, 50)
    const ctx = canvas.getContext('2d')
    const frames = []
    for (let i = 0; i < 2; i++) {
      ctx.fillStyle = i === 0 ? 'white' : 'black'
      ctx.fillRect(0, 0, 50, 50)
      frames.push({
        data: ctx.getImageData(0, 0, 50, 50).data,
        duration: 500
      })
    }
    const webpData = await encodeAnimation(50, 50, true, frames, { loop_count: 1 })
    expect(webpData).not.toBeNull()
    expect(getLoopCount(webpData)).toBe(1)
  })
})
