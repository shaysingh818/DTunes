<template>
    <div class="carousel-container">
        <div class="flex flex-row gap-2" v-for="(item, index) in artistStore.artists" :key="index">
            <div>
                <ArtistCard
                    :artistId="item.artist_id" 
                    :name="item.artist_name" 
                    :datePosted="item.datePosted" 
                    :thumbnail="item.artist_thumbnail"
                    :songCount="item.songCount"
                />
            </div>
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
    components: {ArtistCard},
    name: 'ArtistCarousel',
    async mounted() {
      console.log("Loading artists from store");
      await artistStore.loadArtists();
      console.log("ARTIST STORE LOADED ", artistStore.artists)
    }
}

</script>