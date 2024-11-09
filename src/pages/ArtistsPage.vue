<script>
import ArtistCreate from '../components/artist/ArtistCreate.vue';
import ArtistGrid from '../components/artist/ArtistGrid.vue';
import SearchComponent from '../components/shared/SearchComponent.vue';

export default {
  components: { ArtistGrid, SearchComponent, ArtistCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    artistSearch(){
      alert("Artist Search Button Clicked");
    }
  },
}
</script>

<script setup>
import { ref } from 'vue'

const open = ref(false)
</script>

<template>
  <div class="artist-page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Artists Across DTunes" 
          :onClick="artistSearch"
        />
      </div>
      <div class="grid-container">
        <ArtistGrid /> 
      </div>
    </div>
  </div>
  <ArtistCreate />
</template>



<style scoped>

.artist-page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}

.grid-container {
    max-width: 100%; 
    height: 100%;
    margin: 0 auto;
}

.search-box-container {
    max-width: 100%; 
    height: 75px;
    margin: 0 auto; 
}

</style>