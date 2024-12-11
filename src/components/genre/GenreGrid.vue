<script setup>
import { genreStore } from '../../api/Genre';
</script>

<script>
import GenreCard from './GenreCard.vue';
import { genreStore } from '../../api/Genre';

export default {
    components: { GenreCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
    async mounted() {
        console.log("Loading genres from store");
        await genreStore.loadGenres();
        console.log("GENRE STORE LOADED ", genreStore.genres);
    }
}
</script>


<template>
    <div class="grid-cards">
      <div v-for="(item, index) in genreStore.genres" :key="index">
        <GenreCard 
            :genreId="item.genre_id"
            :name="item.genre_name" 
            :datePosted="item.datePosted" 
            :thumbnail="item.genre_thumbnail"
        />
      </div>
    </div>
</template>

