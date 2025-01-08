import { createMemoryHistory, createRouter, type RouteRecordRaw } from 'vue-router'


import Home from "./components/Home.vue";
import About from './components/About.vue';

const routes: Array<RouteRecordRaw> =  [
  { path: '/', component: Home },
  { path: '/about', component: About },
]

const router = createRouter({
  history: createMemoryHistory(),
  routes,
})

export default router