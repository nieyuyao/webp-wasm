<script setup>
import { ref, onMounted } from 'vue'
import Module from '../dist/webp-wasm'

const canvasRef = ref(null)
let isUploaded = false
const onChange = (event) => {
	const files = event.target.files
	const file = files[0]
	const fr = new FileReader()
	fr.onload = () => {
		if (!fr.result) {
			return
		}
		const img = document.createElement('img')
		img.onload = () => {
			drawImage(img)
		}
		img.src = fr.result
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
	const ctx = canvas.getContext('2d')
	const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height)
	const module = await Module()
	const result = module.encodeRGBA(imgData.data, canvas.width, canvas.height, 100)
	const blob = new Blob([result], {type: 'image/webp'})
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
	isUploaded = true
	ctx.drawImage(img, 0, 0)
}

onMounted(async () => {
	const module = await Module()
	console.log('encoder version is', module.encoder_version())
	console.log('decoder version is', module.decoder_version())
})
</script>

<template>
	<div>
		<input type="file" @change="onChange" accept=".png,.jpg,.jpeg" />
		<canvas ref="canvasRef" />
		<button @click="downloadWebp">Download webp</button>
	</div>
</template>

<style>
canvas {
	display: block;
}
</style>
