<script>
import SongList from '../components/audio_file/SongList.vue';
import SongCard from '../components/audio_file/SongCard.vue';
import ArtistCard from '../components/artist/ArtistCard.vue';
import SearchComponent from '../components/shared/SearchComponent.vue';
import AudioFileCarousel from '../components/audio_file/AudioFileCarousel.vue';
import ArtistCarousel from '../components/artist/ArtistCarousel.vue';
import GenreCarousel from '../components/genre/GenreCarousel.vue';
import PlaylistListView from '../components/playlist/PlaylistListView.vue';

export default {
  components: { 
    SongList, 
    SongCard, 
    PlaylistListView,
    ArtistCard, 
    SearchComponent, 
    AudioFileCarousel,
    ArtistCarousel,
    GenreCarousel
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    async search() {
      const searchTerm = document.getElementById('search-term').value;
      if(searchTerm) {
        this.$router.push(`/search/${searchTerm}`);
      }
    }
  },
}
</script>

<template>
    <div class="page-container">
      <div class="grid-container">

        <div class="flex flex-col gap-4">

          <div class="search-box-container">
            <SearchComponent 
              text="Explore DTunes" 
              :onClick="search"
            />
          </div>

          <div class="flex flex-col gap-2">
            <div class="carousel-header">
              <h1> Popular Songs </h1>
            </div>
            <div>
              <AudioFileCarousel /> 
            </div>
          </div>

          <div class="flex flex-col gap-2">
            <div class="carousel-header">
                <h1>Popular Artists</h1>
            </div>
            <div>
              <ArtistCarousel />
            </div>
          </div>

          <div class="flex flex-col gap-2">
            <div class="carousel-header">
                <h1>Music Genres</h1>
            </div>
            <div>
              <GenreCarousel />
            </div>
          </div>


          <div class="flex flex-col gap-2">
            <div class="carousel-header">
                <h1>Your Playlists</h1>
            </div>
            <div class="playlist-section">
              <PlaylistListView />
            </div>
          </div>


        </div>
      </div>
  </div>

</template>


<style scoped>


.playlist-section {
  margin-top: 1%; 
  margin-left: 5%;
  margin-right: 5%;
  /* border: 1px solid white; */
}

</style>