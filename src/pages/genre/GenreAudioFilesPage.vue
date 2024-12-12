<script>
import SearchComponent from '../../components/shared/SearchComponent.vue';
import GenreAddAudioFileListView from '../../components/genre/GenreAddAudioFileListView.vue';
import BackBar from '../../components/shared/BackBar.vue';
import { genreStore } from '../../api/Genre';
import { useRoute } from 'vue-router';

export default {
  components: { SearchComponent, GenreAddAudioFileListView, BackBar},
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
    const genre = await genreStore.viewGenre(id); 
    console.log("MY GENRE: ", genre); 
    this.genre = genre; 
  }
}
</script>

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
          text="Search Audio Files To Add To Artist" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
         <GenreAddAudioFileListView 
          v-if="genre && genre.genre_id"
          :genreId="genre.genre_id" 
         />
      </div>
    </div>
  </div>
</template>