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
        <ArtistAudioFileListView 
          v-if="artist && artist.artist_id"
          :id="artist.artist_id"
        />
      </div>
    </div>
  </div>
<EditArtistMetadata
  v-if="artist"
  :id="artist.artist_id"
  :name="artist.artist_name"
  :thumbnail="artist.artist_thumbnail"
 /> 
</template>

<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { open, save } from "@tauri-apps/plugin-dialog"
import { artistStore } from '../../api/Artist';
import SearchComponent from '../../components/shared/SearchComponent.vue';
import ArtistCreate from '../../components/artist/ArtistCreate.vue';
import ArtistAudioFileListView from '../../components/artist/ArtistAudioFileListView.vue';
import EditArtistMetadata from './EditArtistMetadata.vue';
import BackBar from '../../components/shared/BackBar.vue';


export default {
  components: { 
    SearchComponent, 
    ArtistCreate, 
    ArtistAudioFileListView,
    EditArtistMetadata,
    BackBar
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async audioFileSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("GENRE SEARCH TERM ", searchTerm); 
      await artistStore.searchArtistAudioFiles(
        this.artist.artist_id.toString(), 
        searchTerm
      );
    },
    goBack() {
      this.$router.go(-1); 
    },
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
    this.artist = await artistStore.viewArtist(id);
    console.log(`MY ARTIST ${this.artist.artist_thumbnail}`);  
  }
}
</script>

<style scoped>

.close-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
}

input {
    border-radius: 3%;
    height: 50px;
    width: 100%;
    padding: 8px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}


/* file button upload styling */ 
label {
  background-color: rgb(28 25 23);
  color: rgb(153 27 27);
  padding: 16px 24px;
  border-radius: 5px;
  font-size: 14px;
  cursor: pointer;
  font-weight: bold;
  /* border: 1px solid white; */
}


p {
    font-size: 12px;
    color: rgb(209 213 219);
}


h1 {
    color: white;
    font-weight: bold;
}


#artist-thumbnail-upload {
  display: none;
}



</style>