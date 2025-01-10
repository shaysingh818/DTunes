import { createMemoryHistory, createRouter, type RouteRecordRaw } from 'vue-router'

import Support from "./components/Support.vue";
import Home from "./components/Home.vue";
import About from "./components/About.vue";
import Features from "./components/Features.vue";
import UserGuide from "./components/UserGuide.vue";

const routes: Array<RouteRecordRaw> =  [
  { path: '/', component: Home },
  { path: '/about', component: About },
  { path: '/support', component: Support },
  { path: '/features', component: Features },
  { path: '/user_guide', component: UserGuide },
]

const router = createRouter({
  history: createMemoryHistory(),
  routes,
})

export default router