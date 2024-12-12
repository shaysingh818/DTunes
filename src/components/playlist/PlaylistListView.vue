
<template>
    <div class="flex flex-col" v-for="(item, index) in playlistStore.playlists" :key="index">
        <div>
            <PlaylistList
                :playlistId="item.playlist_id"
                :title="item.playlist_name" 
                :datePosted="item.date_created" 
                :lastModified="item.last_modified" 
                :thumbnail="item.playlist_thumbnail"
            />
        </div>
    </div>
</template>


<script setup>
import { playlistStore } from '../../api/Playlist';
</script>

<script>

import PlaylistList from './PlaylistList.vue';

export default {
  components: { PlaylistList},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
  },
  async mounted() {
    console.log("Loading playlists from store"); 
    await playlistStore.loadPlaylists();
    console.log("PLAYLIST STORE LOADED: ", playlistStore.playlists); 
  },    
}
</script>