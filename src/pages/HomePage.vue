<script>
import SongList from '../components/audio_file/SongList.vue';
import SongCard from '../components/audio_file/SongCard.vue';
import ArtistCard from '../components/artist/ArtistCard.vue';
import SearchComponent from '../components/shared/SearchComponent.vue';

export default {
  components: { SongList, SongCard, ArtistCard, SearchComponent},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
  },
}
</script>

<!-- Testing list view component here -->
<template>
    <!-- Instance of song list component -->

    <div class="page-container">
      <div class="grid-container">

        <div class="flex flex-col gap-2">

          <div class="search-box-container">
            <SearchComponent 
              text="Explore DTunes" 
              :onClick="genreSearch"
            />
          </div>



          <div class="grid grid-flow-col auto-cols-max gap-4 p-4">

            <div>
              <SongCard 
                title="Pirates of the Carribean" 
                datePosted="Monday, November 4, 2024" 
                duration="3:01"
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
              />
            </div>

            <div>
              <SongCard 
                title="Pirates of the Carribean" 
                datePosted="Monday, November 4, 2024" 
                duration="3:01"
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
              />
            </div>

            <div>
              <SongCard 
                title="Pirates of the Carribean" 
                datePosted="Monday, November 4, 2024" 
                duration="3:01"
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
              />
            </div>

            <div>
              <SongCard 
                title="Pirates of the Carribean" 
                datePosted="Monday, November 4, 2024" 
                duration="3:01"
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
              />
            </div>

          </div>


          <SongList 
            title="Through the fire and flames" 
            datePosted="Monday, November 4, 2024" 
            duration="2:52"
            thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          />

          <SongList 
            title="Pirates of the Carribean" 
            datePosted="Monday, November 4, 2024" 
            duration="3:01"
            thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          />

          <SongList 
            title="Super Mario Bros" 
            datePosted="Monday, November 4, 2024" 
            duration="1:25"
            thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          />


          <div class="grid grid-flow-col auto-cols-max gap-4 p-4">

            <div>
              <ArtistCard 
                name="Post Malone" 
                datePosted="Monday, November 4, 2024" 
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                songCount="100"
              />
            </div>

            <div>
              <ArtistCard 
                name="Billie Eillish" 
                datePosted="Monday, November 4, 2024" 
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                songCount="100"
              />
            </div>

            <div>
              <ArtistCard 
                name="Polo G" 
                datePosted="Monday, November 4, 2024" 
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                songCount="100"
              />
            </div>

            <div>
              <ArtistCard 
                name="NLE Choppa" 
                datePosted="Monday, November 4, 2024" 
                thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                songCount="100"
              />
            </div>

          </div>
        </div>
      </div>
        </div>

</template>


<style scoped>

.page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}

.grid-container {
    max-width: 100%; 
    height: 100%;
    margin: 0 auto;
}

.search-box-container {
    max-width: 100%; 
    height: 75px;
    margin: 0 auto; 
}

</style>