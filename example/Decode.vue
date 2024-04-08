<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { decoderVersion, decodeRGBA } from '../src/'

const canvasRef = ref<HTMLCanvasElement | null>(null)
let isUploaded = false
let webpData: Uint8ClampedArray | null = null
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
		webpData = fr.result as Uint8ClampedArray
	}
	fr.readAsArrayBuffer(file)
}

const drawWebp = async () => {
	if (!isUploaded) {
		alert('Please upload one webp image')
		return
	}
	const canvas = canvasRef.value
	if (!canvas || !webpData) {
		return
	}
	const result = await decodeRGBA(webpData)
  if (!result) {
    return
  }
	const ctx = canvas.getContext('2d')!
	ctx.clearRect(0, 0, canvas.width, canvas.height)
	canvas.style.width = `${result.width}px`
	canvas.style.height = `${result.height}px`
	canvas.width = result.width
	canvas.height = result.height
	ctx.putImageData(result, 0, 0)
}

onMounted(async () => {
	console.log('decoder version is', await decoderVersion())
})
onMounted(() => {
	webpData = null
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
