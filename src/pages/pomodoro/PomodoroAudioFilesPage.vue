<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import PomodoroAddAudioFileListView from '../../components/pomodoro/PomodoroAddAudioFileListView.vue';
import BackBar from '../../components/shared/BackBar.vue';
import { audioStore } from '../../api/AudioFile';
import { pomodoroStore } from '../../api/Pomodoro';
import { useRoute } from 'vue-router';


export default {
  components: { SearchComponent, PomodoroAddAudioFileListView, BackBar},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async audioFileSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("SEARCH TERM ", searchTerm); 
      await audioStore.searchAudioFiles(searchTerm)
    }
  },
  data() {
    return {
      session: {
        genreId: 0,
        genreName: '',
        genreThumbnail: '',
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    const session = await pomodoroStore.viewSession(id); 
    console.log("MY SESSION: ", session); 
    this.session = session; 
  }
}
</script>

<template>
  <BackBar
    v-if="session && session.session_id"
    :itemId="session.session_id" 
    :title="session.session_name"
    thumbnail="default_dtunes_thumbnail.webp"
    :dateCreated="session.date_created"
  />
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files To Add To Pomodoro session" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
         <PomodoroAddAudioFileListView 
          v-if="session && session.session_id"
          :sessionId="session.session_id" 
         />
      </div>
    </div>
  </div>
</template>