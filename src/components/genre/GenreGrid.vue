<script>
import GenreCard from './GenreCard.vue';

export default {
    components: { GenreCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
    data() {
        return {
            genres: [
                {
                    name: "Hip Hop",
                    datePosted: "Monday, November 4, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    name: "Rap",
                    datePosted: "Tuesday, November 5, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    name: "Classical",
                    datePosted: "Wednesday, November 6, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    name: "Rock",
                    datePosted: "Wednesday, November 6, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    name: "Punk",
                    datePosted: "Wednesday, November 6, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    name: "Beatbox",
                    datePosted: "Wednesday, November 6, 2024",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                }
            ] 
        }
    }
}
</script>


<template>
    <div class="cards">
      <div v-for="(item, index) in genres" :key="index">
        <GenreCard 
            :name="item.name" 
            :datePosted="item.datePosted" 
            :thumbnail="item.thumbnail"
        />
      </div>
    </div>
</template>


<style scoped>

.cards {
    display: grid;
    gap: 1rem;
    margin: 0 auto; 
    overflow: auto; 
}

@media (min-width: 600px) {
  .cards { grid-template-columns: repeat(2, 1fr); }
}

@media (min-width: 800px) {
  .cards { grid-template-columns: repeat(3, 1fr); }
}

@media (min-width: 1000px) {
  .cards { grid-template-columns: repeat(4, 1fr); }
}

@media (min-width: 1200px) {
  .cards { grid-template-columns: repeat(5, 1fr); }
}

@media (min-width: 1500px) {
  .cards { grid-template-columns: repeat(6, 1fr); }
}

@media (min-width: 1900px) {
  .cards { grid-template-columns: repeat(8, 1fr); max-width: 1800px; }
}

</style>