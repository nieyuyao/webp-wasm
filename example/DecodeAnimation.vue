<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { decoderVersion, decodeAnimation } from '../src/'

const canvasRef = ref<HTMLCanvasElement | null>(null)
let isUploaded = false
let webpData: Uint8Array | null = null
let timeoutId = 0
const onChange = (event) => {
	const files = (event.target as HTMLInputElement).files
	if (!files?.length) {
		return
	}
	isUploaded = true
	const file = files[0]
	const fr = new FileReader()
	fr.onload = () => {
		if (!fr.result) {
			return
		}
		webpData = fr.result as Uint8Array
	}
	fr.readAsArrayBuffer(file)
}

const drawWebp = async () => {
	if (!isUploaded) {
		alert('Please upload one webp image')
		return
	}
  window.clearTimeout(timeoutId)
	const canvas = canvasRef.value
	if (!canvas || !webpData) {
		return
	}
	const frames = await decodeAnimation(webpData, true)
  if (!frames?.length) {
    return
  }
  const { width, height } = frames[0]
  canvas.style.width = `${width}px`
  canvas.style.height = `${height}px`
  canvas.width = width
  canvas.height = height
  const ctx = canvas.getContext('2d')!
  const loopFrame = (i: number) => {
    const frame = frames[i]
    ctx.putImageData(new ImageData(new Uint8ClampedArray(frame.data), frame.width), 0, 0)
    timeoutId = window.setTimeout(() => {
      loopFrame((i + 1) % frames.length)
    }, frame.duration)
  }

  loopFrame(0)
}

onMounted(async () => {
	console.log('decoder version is', await decoderVersion())
})
onMounted(() => {
	webpData = null
  window.clearTimeout(timeoutId)
})
</script>

<template>
	<div>
		<input type="file" @change="onChange" accept=".webp" />
		<button @click="drawWebp">Decode</button>
		<canvas ref="canvasRef" />
	</div>
</template>

<style scoped>
canvas {
	display: block;
}
button {
	margin-top: 12px;
	display: block;
}
</style>
