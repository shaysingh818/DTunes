<template>
    <div class="grid-cards">
      <div v-for="(item, index) in artistStore.artists" :key="index">
        <ArtistCard
          :artistId="item.artist_id" 
          :name="item.artist_name" 
          :datePosted="item.date_posted" 
          :thumbnail="item.artist_thumbnail"
          :songCount="10000"
        />
      </div>
    </div>
</template>

<script setup>
import { artistStore } from '../../api/Artist';
</script>


<script>
import ArtistCard from './ArtistCard.vue';
import { artistStore } from '../../api/Artist';

export default {
    components: { ArtistCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
    data() {
      return {
        artists: []
      }
    },
    async mounted() {
      console.log("Loading artists from store");
      await artistStore.loadArtists();
      console.log("ARTIST STORE LOADED ", artistStore.artists)
    }
}
</script>