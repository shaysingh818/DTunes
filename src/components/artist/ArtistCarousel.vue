<template>
    <div class="carousel-container">
        <div class="flex flex-row gap-2" v-for="(item, index) in artists" :key="index">
            <div>
                <ArtistCard 
                    :name="item.name" 
                    :datePosted="item.datePosted" 
                    :thumbnail="item.thumbnail"
                    :songCount="item.songCount"
                />
            </div>
        </div>
    </div>
</template>


<script>
import ArtistCard from './ArtistCard.vue';

export default {
    components: {ArtistCard},
    name: 'ArtistCarousel',
    data() {
        return {
            artists: [
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
                {
                    name: "NLE Choppa",
                    datePosted: "Monday, November 4, 2024", 
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg",
                    songCount: "100"
                },
            ]
        }
    }
}

</script>


<style scoped>

.carousel-container {
    /* border: 1px solid #ccc; */
    display: flex;
    align-items: center;
    justify-content: center;
    overflow: auto;
    gap: 1rem;
    margin-right: 5%;
    margin-left: 5%;
}

</style>