
import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router'

const routesArray: Array<RouteRecordRaw> = [
	{
		path: '/',
		name: 'home',
		component: () => import('./App.vue')
	},
	{
		path: '/encode',
		name: 'Encode',
		component: () => import('./Encode.vue')
	},
	{
		path: '/encode-animation',
		name: 'EncodeAnimation',
		component: () => import('./EncodeAnimation.vue')
	},
	{
		path: '/decode',
		name: 'Decode',
		component: () => import('./Decode.vue')
	},
  {
		path: '/decode-animation',
		name: 'DecodeAnimation',
		component: () => import('./DecodeAnimation.vue')
	},
]


const router = createRouter({
	history: createWebHistory('/'),
	routes: routesArray
})

export const routes = routesArray

export default router
