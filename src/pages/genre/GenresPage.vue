<script>
import GenreCreate from '../../components/genre/GenreCreate.vue';
import GenreGrid from '../../components/genre/GenreGrid.vue';
import SearchComponent from '../../components/shared/SearchComponent.vue';
import { genreStore } from '../../api/Genre';

export default {
  components: { GenreGrid, SearchComponent, GenreCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async genreSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("GENRE SEARCH TERM ", searchTerm); 
      await genreStore.searchGenres(searchTerm);
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
          text="Search Genres Across DTunes" 
          :onClick="genreSearch"
        />
      </div>
      <div class="grid-container">
        <GenreGrid /> 
      </div>
    </div>
  </div>
  <GenreCreate />
</template>