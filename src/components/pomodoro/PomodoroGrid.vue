
<script>
import PomodoroCard from './PomodoroCard.vue';

export default {
    components: { PomodoroCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
    data() {
        return {
            sessions: [
                {
                    name: "Deep Work Classical",
                    duration: "50",
                    durationLimit: "50",
                    shortBreak: "3",
                    longBreak: "15",
                    datePosted: "11/09/2024",
                    lastModified: "11/09/2024"
                },
                {
                    name: "Deep Work Rain",
                    duration: "50",
                    durationLimit: "50",
                    shortBreak: "3",
                    longBreak: "15",
                    datePosted: "11/09/2024",
                    lastModified: "11/09/2024"
                },
                {
                    name: "Hip Hop",
                    duration: "50",
                    durationLimit: "50",
                    shortBreak: "3",
                    longBreak: "15",
                    datePosted: "11/09/2024",
                    lastModified: "11/09/2024"
                }
            ]
        }
    }
}
</script>


<template>
    <div class="cards">
      <div v-for="(item, index) in sessions" :key="index">
        <PomodoroCard 
            :name="item.name"
            :duration="item.duration"
            :durationLimit="item.durationLimit"
            :shortBreak="item.shortBreak"
            :longBreak="item.longBreak"
            :datePosted="item.datePosted"
            :lastModified="item.lastModified"
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