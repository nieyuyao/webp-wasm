<script lang="ts" setup>
import { ref, onMounted, onUnmounted } from 'vue'
import { encodeAnimation } from '../src/'

let rafId = 0
const canvasRef = ref<HTMLCanvasElement | null>(null)

let duration = 20 // ms
let delta = 0
let lastTime = Date.now()
let frames: any[] = []

const stop = () => {
	cancelAnimationFrame(rafId)
}

const drawBall = (x = 0) => {
	if (!canvasRef.value) {
		return
	}
	if (x >= 90) {
		stop()
		return
	}
	if (delta <= duration) {
		const now = Date.now()
		delta += now - lastTime
		lastTime = now
		rafId = requestAnimationFrame(() => drawBall(x))
		return
	}
	const ctx = canvasRef.value.getContext('2d')!
	ctx.clearRect(0, 0, 100, 100)
	ctx.fillStyle = 'red'
	ctx.beginPath()
	ctx.arc(x, 50, 10, 0, 2 * Math.PI)
	ctx.closePath()
	ctx.fill()
	x += 10
	frames.push({
		data: ctx.getImageData(0, 0, 100, 100).data,
		duration: 20
	})
	rafId = requestAnimationFrame(() => drawBall(x))
}

const exportWebP = async () => {
	const result = await encodeAnimation(100, 100, true, frames)
	const blob = new Blob([result!], {type: 'image/webp'})
	const blobURL = URL.createObjectURL(blob);
	const a = document.createElement('a')
	a.download = '1.webp'
	a.href = blobURL
	document.body.appendChild(a)
	a.click()
	a.remove()
}

onMounted(drawBall)

onUnmounted(() => {
	stop()
	frames = []
})
</script>

<template>
	<canvas ref="canvasRef" width="100" height="100" style="width: 100px;height:100px;"></canvas>
	<button @click="exportWebP">Export WebP</button>
</template>

<style scoped>
canvas {
	display: block;
}
</style>
