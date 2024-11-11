<template>
    <div class="carousel-container">
        <div class="flex flex-row gap-2" v-for="(item, index) in audioFiles" :key="index">
            <div>
                <SongCard 
                    :title="item.title" 
                    :datePosted="item.datePosted" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                />
            </div>
        </div>
    </div>
</template>


<script>
import SongCard from './SongCard.vue';

export default {
    components: {SongCard},
    name: 'AudioFileCarousel',
    data() {
        return {
            audioFiles: [
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                },
                {
                    title: "Pirates of the Carribean",
                    datePosted: "Monday, November 4, 2024", 
                    duration: "3:01",
                    thumbnail: "https://www.w3schools.com/html/pic_trulli.jpg"
                }
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