import { createMemoryHistory, createRouter, RouteRecordRaw } from 'vue-router'

import HomePage from './pages/HomePage.vue'
import PlaylistsPage from './pages/playlist/PlaylistsPage.vue'
import AudioFilesPage from './pages/audio_file/AudioFilesPage.vue'
import GenresPage from './pages/genre/GenresPage.vue'
import ArtistsPage from './pages/artist/ArtistsPage.vue'
import PomodoroPage from './pages/pomodoro/PomodoroPage.vue'
import EditAudioFilePage from './pages/audio_file/EditAudioFilePage.vue'
import EditPlaylistPage from './pages/playlist/EditPlaylistPage.vue'
import PlaylistAudioFilesPage from './pages/playlist/PlaylistAudioFilesPage.vue'
import EditArtistPage from './pages/artist/EditArtistPage.vue';
import ArtistAudioFilesPage from './pages/artist/ArtistAudioFilesPage.vue'
import GenreAudioFilesPage from './pages/genre/GenreAudioFilesPage.vue'
import EditGenrePage from './pages/genre/EditGenrePage.vue'
import PomodoroDetailPage from './pages/pomodoro/PomodoroDetailPage.vue'
import PomodoroAudioFilesPage from './pages/pomodoro/PomodoroAudioFilesPage.vue'
import DTunesSearchPage from './pages/DTunesSearchPage.vue'

const routes: Array<RouteRecordRaw> =  [
  { path: '/', component: HomePage },
  { path: '/search/:searchTerm', component: DTunesSearchPage},

  { path: '/playlists', component: PlaylistsPage },
  { path: '/playlists/edit/:id', component: EditPlaylistPage},
  { path: '/playlists/add-audio-file/:id', component: PlaylistAudioFilesPage},

  { path: '/audio-files', component: AudioFilesPage },
  { path: '/audio-files/edit/:id', component: EditAudioFilePage},

  { path: '/genres', component: GenresPage },
  { path: '/genre/edit/:id', component: EditGenrePage},
  { path: '/genre/add-audio-file/:id', component: GenreAudioFilesPage},

  { path: '/artists', component: ArtistsPage },
  { path: '/artist/edit/:id', component: EditArtistPage},
  { path: '/artist/add-audio-file/:id', component: ArtistAudioFilesPage},

  { path: '/pomodoro', component: PomodoroPage },
  { path: '/pomodoro/edit/:id', component: PomodoroDetailPage},
  { path: '/pomodoro/add-audio-file/:id', component: PomodoroAudioFilesPage},
]

const router = createRouter({
  history: createMemoryHistory(),
  routes,
})

export default router