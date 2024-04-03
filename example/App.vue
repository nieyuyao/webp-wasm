<script setup>
import { ref, onMounted } from 'vue'
import { Webp } from '../dist/esm/index'

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
	const result = await Webp.encodeRGBA(imgData.data, canvas.width, canvas.height, 100)
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
	console.log('encoder version is', await Webp.encoderVersion())
	console.log('decoder version is', await Webp.decoderVersion())
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
