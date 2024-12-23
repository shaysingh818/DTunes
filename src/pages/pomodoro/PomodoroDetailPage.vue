<template>

  <BackBar 
    itemId="3" 
    title="My Pomodoro Session"
    thumbnail="default_dtunes_thumbnail.webp"
    dateCreated="December 15th 2025"
  />
  <div class="page-container">

    <div class="flex flex-col" style="width: 100%;">

      <div>
        <PomodoroClock 
          duration="50:00"
          durationLimit="4"
          shortBreak="5:00"
          longBreak="15:00"
        />
      </div>

      <br> 
      <div class="flex flex-col gap-2">
        <div class="carousel-header">
          <h1> Focus Session Songs </h1>
        </div>
        <div>
          <PomodoroAudioFileCarousel 
            v-if="session && session.session_id"
            :sessionId="session.session_id"
          />
        </div>

      </div>

    </div>

    <EditPomodoroMetadata
      :sessionId="session.session_id"
      :sessionName="session.session_name"
      :duration="session.duration"
      :durationLimit="session.duration_limit"
      :shortBreak="session.short_break"
      :longBreak="session.long_break"
    />


  </div>
</template>

<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { open, save } from "@tauri-apps/plugin-dialog"
import SearchComponent from '../../components/shared/SearchComponent.vue';
import BackBar from '../../components/shared/BackBar.vue';
import PomodoroClock from '../../components/pomodoro/PomodoroClock.vue';
import PomodoroAudioFileCarousel from '../../components/pomodoro/PomodoroAudioFileCarousel.vue';
import EditPomodoroMetadata from './EditPomodoroMetadata.vue';
import { pomodoroStore } from '../../api/Pomodoro';

let openWindow = ref(false);

export default {
  components: { 
    SearchComponent, 
    BackBar,
    PomodoroAudioFileCarousel,
    PomodoroClock,
    EditPomodoroMetadata
  },
  props: {
    id: {
      type: Number,
      required: true,
    },
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    goBack() {
      this.$router.go(-1); 
    },
  },
  data() {
    return {
      session: {
        sessionId: 0,
        sessionName: '',
        duration: 0,
        durationLimit: 0,
        shortBreak: 0,
        longBreak: 0,
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    this.session = await pomodoroStore.viewSession(id);
  }
}
</script>