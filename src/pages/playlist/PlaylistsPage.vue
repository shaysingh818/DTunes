<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import PlaylistCreate from '../../components/playlist/PlaylistCreate.vue';
import PlaylistListView from '../../components/playlist/PlaylistListView.vue';
import { playlistStore } from '../../api/Playlist';

export default {
  components: { SearchComponent, PlaylistCreate, PlaylistListView},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async playlistSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("PLAYLIST SEARCH TERM ", searchTerm); 
      await playlistStore.searchPlaylists(searchTerm);
    }
  },
}
</script>

<template>

  <div class="page-container">
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
