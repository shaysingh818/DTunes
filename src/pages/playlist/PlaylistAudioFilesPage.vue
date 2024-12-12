<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import PlaylistAddAudioFileListView from '../../components/playlist/PlaylistAddAudioFileListView.vue';
import BackBar from '../../components/shared/BackBar.vue';
import { playlistStore } from '../../api/Playlist';
import { useRoute } from 'vue-router';

export default {
  components: { SearchComponent, PlaylistAddAudioFileListView, BackBar},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    audioFileSearch(){
      alert("Audio File Search Button Clicked");
    }
  },
  data() {
    return {
      playlist: {
        playlistId: 0,
        playlistName: '',
        playlistThumbnail: '',
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    const playlist = await playlistStore.viewPlaylist(id); 
    console.log("MY PLAYLIST: ", playlist); 
    this.playlist = playlist; 
  }
}
</script>

<template>
  <BackBar
    v-if="playlist && playlist.playlist_id"
    :itemId="playlist.playlist_id" 
    :title="playlist.playlist_name"
    :thumbnail="playlist.playlist_thumbnail"
    :dateCreated="playlist.date_created"
  />
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files To Add To Playlist" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
         <PlaylistAddAudioFileListView 
          v-if="playlist && playlist.playlist_id"
          :playlistId="playlist.playlist_id" 
         />
      </div>
    </div>
  </div>
</template>