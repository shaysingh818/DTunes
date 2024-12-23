<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import ArtistAddAudioFileListView from '../../components/artist/ArtistAddAudioFileListView.vue';
import BackBar from '../../components/shared/BackBar.vue';
import { artistStore } from '../../api/Artist';
import { audioStore } from '../../api/AudioFile';
import { useRoute } from 'vue-router';

export default {
  components: { SearchComponent, ArtistAddAudioFileListView, BackBar},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async audioFileSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("SEARCH TERM ", searchTerm); 
      await audioStore.searchAudioFiles(searchTerm)
    }
  },
  data() {
    return {
      artist: {
        artistId: 0,
        artistName: '',
        artistThumbnail: '',
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    const artist = await artistStore.viewArtist(id); 
    console.log("MY ARTIST: ", artist); 
    this.artist = artist; 
  }
}
</script>

<template>
  <BackBar
    v-if="artist && artist.artist_id"
    :itemId="artist.artist_id" 
    :title="artist.artist_name"
    :thumbnail="artist.artist_thumbnail"
    :dateCreated="artist.date_created"
  />
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files To Add To Artist" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
         <ArtistAddAudioFileListView 
          v-if="artist && artist.artist_id"
          :artistId="artist.artist_id" 
         />
      </div>
    </div>
  </div>
</template>