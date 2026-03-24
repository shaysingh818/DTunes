import { createMemoryHistory, createRouter, RouteRecordRaw } from 'vue-router'

import HomePage from './pages/HomePage.vue'
import AudioFilesPage from './pages/audio_file/AudioFilesPage.vue'
import PomodoroPage from './pages/pomodoro/PomodoroPage.vue'
import EditAudioFilePage from './pages/audio_file/EditAudioFilePage.vue'
import PomodoroDetailPage from './pages/pomodoro/PomodoroDetailPage.vue'
import PomodoroAudioFilesPage from './pages/pomodoro/PomodoroAudioFilesPage.vue'
import PomodoroSessionTracking from './pages/pomodoro/PomodoroSessionTracking.vue'
import DTunesSearchPage from './pages/DTunesSearchPage.vue'

const routes: Array<RouteRecordRaw> =  [
  { path: '/', component: HomePage },
  { path: '/search/:searchTerm', component: DTunesSearchPage},

  { path: '/audio-files', component: AudioFilesPage },
  { path: '/audio-files/edit/:id', component: EditAudioFilePage},

  { path: '/pomodoro', component: PomodoroPage },
  { path: '/pomodoro/edit/:id', component: PomodoroDetailPage, props: true},
  { path: '/pomodoro/add-audio-file/:id', component: PomodoroAudioFilesPage},


  { path: '/pomodoro-session-tracking', component: PomodoroSessionTracking},

]

const router = createRouter({
  history: createMemoryHistory(),
  routes,
})

export default router
