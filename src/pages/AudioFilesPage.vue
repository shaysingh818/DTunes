<script>
import SearchComponent from '../components/shared/SearchComponent.vue';
import AudioFileListView from '../components/audio_file/AudioFileListView.vue';
import AudioFileCreate from '../components/audio_file/AudioFileCreate.vue';

export default {
  components: { SearchComponent, AudioFileListView, AudioFileCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    audioFileSearch(){
      alert("Audio File Search Button Clicked");
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