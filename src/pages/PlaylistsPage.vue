<script>
import SearchComponent from '../components/shared/SearchComponent.vue';
import PlaylistCreate from '../components/playlist/PlaylistCreate.vue';
import PlaylistListView from '../components/playlist/PlaylistListView.vue';

export default {
  components: { SearchComponent, PlaylistCreate, PlaylistListView},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    playlistSearch(){
      alert("Playlist Search Button Clicked");
    }
  },
}
</script>

<template>

  <div class="playlist-page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Playlists Across DTunes" 
          :onClick="playlistSearch"
        />
      </div>
      <div>
         <PlaylistListView />
      </div>
    </div>
  </div>
   <PlaylistCreate /> 
</template>

<style scoped>

.playlist-page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}


.search-box-container {
    max-width: 100%; 
    height: 75px;
    margin: 0 auto; 
}
</style>