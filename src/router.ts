import { createMemoryHistory, createRouter, RouteRecordRaw } from 'vue-router'

import HomePage from './pages/HomePage.vue'
import PlaylistsPage from './pages/PlaylistsPage.vue'
import AudioFilesPage from './pages/AudioFilesPage.vue'
import GenresPage from './pages/GenresPage.vue'
import ArtistsPage from './pages/ArtistsPage.vue'
import PomodoroPage from './pages/PomodoroPage.vue'

const routes: Array<RouteRecordRaw> =  [
  { path: '/', component: HomePage },
  { path: '/playlists', component: PlaylistsPage },
  { path: '/audio-files', component: AudioFilesPage },
  { path: '/genres', component: GenresPage },
  { path: '/artists', component: ArtistsPage },
  { path: '/pomodoro', component: PomodoroPage },
]

const router = createRouter({
  history: createMemoryHistory(),
  routes,
})

export default router