<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { encoderVersion, decoderVersion, encode } from '../src/'

const canvasRef = ref<HTMLCanvasElement | null>(null)
let isUploaded = false
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
		const img = document.createElement('img')
		img.onload = () => drawImage(img)
		img.src = fr.result as string
	}
	fr.readAsDataURL(file)
}
const downloadWebp = async () => {
	if (!isUploaded) {
		alert('Please upload one image')
		return
	}
	if (!canvasRef.value) {
		return
	}
	const canvas = canvasRef.value
	const ctx = canvas.getContext('2d')!
	const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height)
  // const buf = new Uint8Array(3 * canvas.width * canvas.height)
  // let j = 0
  // imgData.data.forEach((pixel, i) => {
  //   if ((i + 1) % 4 === 0) {
  //     return
  //   }
  //   buf[j] = pixel
  //   j++
  // })
	const result = await encode((imgData.data) as unknown as Uint8Array, canvas.width, canvas.height, true, { lossless: 1, quality: 100 })
	const blob = new Blob([result!], {type: 'image/webp'})
	const blobURL = URL.createObjectURL(blob);
	const a = document.createElement('a')
	a.download = '1.webp'
	a.href = blobURL
	document.body.appendChild(a)
	a.click()
	a.remove()
}

const drawImage = (img) => {
	const canvas = canvasRef.value
	if (!canvas) {
		return
	}
	canvas.style.width = `${img.width}px`
	canvas.style.height = `${img.height}px`
	canvas.width = img.width
	canvas.height = img.height
	const ctx = canvas.getContext('2d')
	ctx!.drawImage(img, 0, 0)
}

onMounted(async () => {
	console.log('encoder version is', await encoderVersion())
})
</script>

<template>
	<div>
		<input type="file" @change="onChange" accept=".png,.jpg,.jpeg" />
		<canvas ref="canvasRef" />
		<button @click="downloadWebp">Download webp</button>
	</div>
</template>

<style scoped>
canvas {
	display: block;
}
</style>
