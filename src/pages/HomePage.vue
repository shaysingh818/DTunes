<script>
import SongList from '../components/audio_file/SongList.vue';
import SongCard from '../components/audio_file/SongCard.vue';
import ArtistCard from '../components/artist/ArtistCard.vue';

export default {
  components: { SongList, SongCard, ArtistCard},
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


</template>