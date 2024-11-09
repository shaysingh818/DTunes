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
  },
}
</script>

<template>
  <div class="artist-page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent />
      </div>
      <div class="grid-container">
         <AudioFileListView />
      </div>
    </div>
  </div>
   <AudioFileCreate /> 
</template>

<style scoped>

.artist-page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}
</style>