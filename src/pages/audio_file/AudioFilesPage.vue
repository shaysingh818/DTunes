<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import AudioFileListView from '../../components/audio_file/AudioFileListView.vue';
import AudioFileCreate from '../../components/audio_file/AudioFileCreate.vue';
import { audioStore } from '../../api/AudioFile';

export default {
  components: { SearchComponent, AudioFileListView, AudioFileCreate},
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
}
</script>

<template>
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files Across DTunes" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
         <AudioFileListView />
      </div>
    </div>
  </div>
   <AudioFileCreate /> 
</template>