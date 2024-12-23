<script>
import ArtistCreate from '../../components/artist/ArtistCreate.vue';
import ArtistGrid from '../../components/artist/ArtistGrid.vue';
import SearchComponent from '../../components/shared/SearchComponent.vue';
import { artistStore } from '../../api/Artist';

export default {
  components: { ArtistGrid, SearchComponent, ArtistCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async artistSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("ARTIST SEARCH TERM ", searchTerm); 
      await artistStore.searchArtist(searchTerm)
    }
  },
}
</script>

<script setup>
import { ref } from 'vue'

const open = ref(false)
</script>

<template>
  <div class="page-container">
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
