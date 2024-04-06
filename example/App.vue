<script setup lang="ts">
import { computed } from 'vue';
import {  useRoute } from 'vue-router'
import { routes } from './router'

const route = useRoute()
const isHome = computed(() => {
	return route.path === '/'
})
</script>

<template>
	<div>
		<div v-if="isHome">
			<router-link
				v-for="(route, i) in routes.filter(route => route.path !== '/')"
				:key="i"
				:to="route.path"
			>
				{{ route.name }}
			</router-link>
		</div>
		<router-view v-if="!isHome"></router-view>
	</div>
</template>

<style scoped>
a {
	display: block;
}
</style>

