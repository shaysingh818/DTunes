<template>

  <BackBar 
    v-if="genre && genre.genre_id"
    :itemId="genre.genre_id" 
    :title="genre.genre_name"
    :thumbnail="genre.genre_thumbnail"
    :dateCreated="genre.date_created"
  />
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files To Add To Genre" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
        <GenreAudioFileListView 
          v-if="genre && genre.genre_id"
          :id="genre.genre_id"
        />
      </div>
    </div>
  </div>
<EditGenreMetadata
  v-if="genre"
  :id="genre.genre_id"
  :name="genre.genre_name"
  :thumbnail="genre.genre_thumbnail"
 /> 
</template>

<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { open, save } from "@tauri-apps/plugin-dialog"
import { genreStore } from '../../api/Genre';
import SearchComponent from '../../components/shared/SearchComponent.vue';
import GenreCreate from '../../components/genre/GenreCreate.vue';
import GenreAudioFileListView from '../../components/genre/GenreAudioFileListView.vue';
import EditGenreMetadata from './EditGenreMetadata.vue';
import BackBar from '../../components/shared/BackBar.vue';


export default {
  components: { 
    SearchComponent, 
    GenreCreate, 
    GenreAudioFileListView,
    EditGenreMetadata,
    BackBar
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async audioFileSearch(){
      const searchTerm = document.getElementById('search-term').value;
      console.log("GENRE SEARCH TERM ", searchTerm); 
      await genreStore.searchGenreAudioFiles(
        this.genre.genre_id.toString(), 
        searchTerm
      );
    },
    goBack() {
      this.$router.go(-1); 
    },
  },
  data() {
    return {
      genre: {
        genreId: 0,
        genreName: '',
        genreThumbnail: '',
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    this.genre = await genreStore.viewGenre(id);
    console.log(`MY GENRE ${this.genre.genre_thumbnail}`);  
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


#genre-thumbnail-upload {
  display: none;
}



</style>