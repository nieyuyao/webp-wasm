# WIP

# `webp.wasm`

webp.wasm is a pure Webassembly / Javascript port of libwebp.

![CI](https://github.com/nieyuyao/webp-wasm/workflows/CI/badge.svg)

## APIs

### Encode

#### encode

Get encoder encoderVersion.

`function encoderVersion(): Promise<string>`

##### Example

```javascript
const version = await encoderVersion()
console.log(version) // 1.3.2
```

#### encodeRGB

Encodes rgb bitmap an outputs webp.

`function encodeRGB(rgb: Uint8ClampedArray, width: number, height: number, quality?: number): Promise<Nullable<Uint8ClampedArray>>`

##### Example

```javascript
...
const ctx = canvas.getContext('2d')!
const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height)
const buf = new Uint8ClampedArray(3 * canvas.width, canvas.height) 
let j = 0
// remove alpha
imgData.data.forEach((pixel, i) => {
  if ((i + 1) % 4 === 0) {
    return
  }
  buf[j] = pixel
  j++
})
const result = await encodeRGB(buf, canvas.width, canvas.height)
const blob = new Blob([result!], {type: 'image/webp'})
const blobURL = URL.createObjectURL(blob);
// download webp
const a = document.createElement('a')
a.download = '1.webp'
a.href = blobURL
document.body.appendChild(a)
a.click()
a.remove()
```

#### encodeRGBA

Encodes rgba bitmap an outputs webp.

`function encodeRGBA(rgba: Uint8ClampedArray, width: number, height: number, quality?: number): Promise<Nullable<Uint8ClampedArray>>`

##### Example

```javascript
...
const ctx = canvas.getContext('2d')!
const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height)
const result = await encodeRGBA(imgData.data, canvas.width, canvas.height)
// download webp
...
```

#### encode

A more advanced API is based on the WebPConfig. <b>Only the lossless and quality parameters are supported now !!!</b>.

`function encodeRGBA(data: Uint8ClampedArray, width: number, height: number, hasAlpha: boolean,config: Partial<WebPConfig>): Promise<Nullable<Uint8ClampedArray>>`

- hasAlpha: boolean

Whether to include alpha chanel.

- WebPConfig.lossless: number

Lossless encoding (0=lossy(default), 1=lossless).

- WebPConfig.quality: number

Between 0 and 100.

##### Example

```javascript
...
const ctx = canvas.getContext('2d')!
const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height)
const result = await encode(imgData.data, canvas.width, canvas.height, true, { lossless: 0 })
// download webp
...
```

#### encodeAnimation

Encodes frame data an outputs animated webp.

`function encodeAnimation(width: number, height: number, hasAlpha: boolean, frames: WebPAnimationFrame[]): Promise<Nullable<Uint8ClampedArray>>`

- hasAlpha: boolean

Whether to include alpha chanel.

- WebPAnimationFrame.data: Uint8ClampedArray

Frame data.

- WebPAnimationFrame.duration: number

Duration of frame.

##### Example

```javascript
...
// record each frame
frames.push({
  data: ctx.getImageData(0, 0, 100, 100).data,
  duration: 20
})
const result = await encodeAnimation(100, 100, true, frames)
...
// download webp
```

### Decode

#### decoderVersion

Get decoder version.

`function decoderVersion(): Promise<string>`

##### Example

```javascript
const version = await decoderVersion()
console.log(version) // 1.3.2
```

#### decodeRGB

Decodes webp and outputs `ImageData` contains rgb bitmap.

`function decodeRGB(rgb: Uint8ClampedArray): Promise<Nullable<ImageData>>`

##### Example

```javascript
...
const fr = new FileReader()
fr.onload = () => {
  if (!fr.result) {
    return
  }
  webpData = fr.result as Uint8ClampedArray
  const result = await decodeRGB(webpData)
  // draw imageData
  const ctx = canvas.getContext('2d')!
	ctx.clearRect(0, 0, canvas.width, canvas.height)
	canvas.style.width = `${result.width}px`
	canvas.style.height = `${result.height}px`
	canvas.width = result.width
	canvas.height = result.height
	ctx.putImageData(result, 0, 0)
}
// read webp file
fr.readAsArrayBuffer(file)
...
```

#### decodeRGBA

Decodes webp and outputs `ImageData` contains rgba bitmap.

`function decodeRGB(rgb: Uint8ClampedArray): Promise<Nullable<ImageData>>`

##### Example

```javascript
...
const fr = new FileReader()
fr.onload = () => {
  if (!fr.result) {
    return
  }
  webpData = fr.result as Uint8ClampedArray
  const result = await decodeRGBA(webpData)
  // draw imageData
  ...
}
// webp file
fr.readAsArrayBuffer(file)
...
```

## Playing Examples

```shell
npm run dev
```

## Building

```shell
npm run build
```