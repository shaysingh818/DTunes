<script>
import SongList from '../components/audio_file/SongList.vue';
import SongCard from '../components/audio_file/SongCard.vue';
import SearchComponent from '../components/shared/SearchComponent.vue';
import AudioFileCarousel from '../components/audio_file/AudioFileCarousel.vue';
import { syncMetadata } from '../api/Utilities';

export default {
  components: { 
    SongList, 
    SongCard, 
    SearchComponent, 
    AudioFileCarousel,
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async search() {
      const searchTerm = document.getElementById('search-term').value;
      if(searchTerm) {
        this.$router.push(`/search/${searchTerm}`);
      }
    }
  },
}
</script>

<script setup>
import { syncMetadata } from '../api/Utilities';
</script>

<template>
    <div class="page-container">

      <button class="add-button" @click="syncMetadata()">
        <i :class="['fas', 'fa-download', 'text-white-800']"></i>
      </button>

      <div class="grid-container">

        <div class="flex flex-col gap-4">

          <div class="search-box-container">
            <SearchComponent 
              text="Explore DTunes" 
              :onClick="search"
            />
          </div>

          <div class="flex flex-col gap-2">
            <div class="carousel-header">
              <h1> Most Played Songs </h1>
            </div>
            <div>
              <AudioFileCarousel /> 
            </div>
          </div>

        </div>
      </div>
  </div>

</template>


<style scoped>


.playlist-section {
  margin-top: 1%; 
  margin-left: 5%;
  margin-right: 5%;
  /* border: 1px solid white; */
}

</style>
