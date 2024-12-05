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
    <div class="grid-cards">
      <div v-for="(item, index) in genres" :key="index">
        <GenreCard 
            :name="item.name" 
            :datePosted="item.datePosted" 
            :thumbnail="item.thumbnail"
        />
      </div>
    </div>
</template>

