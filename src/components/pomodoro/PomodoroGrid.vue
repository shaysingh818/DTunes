<script setup>
import { pomodoroStore } from '../../api/Pomodoro';
</script>

<script>
import PomodoroCard from './PomodoroCard.vue';
import { pomodoroStore } from '../../api/Pomodoro';

export default {
    components: { PomodoroCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
    async mounted() {
      console.log("Loading sessions from store");
      await pomodoroStore.loadSessions();
      console.log("SESSIONS STORE LOADED ", pomodoroStore.sessions);
    }
}
</script>


<template>
    <div class="cards">
      <div v-for="(item, index) in pomodoroStore.sessions" :key="index">
        <PomodoroCard 
            :sessionId="item.session_id"
            :name="item.session_name"
            :duration="item.duration"
            :durationLimit="item.duration_limit"
            :shortBreak="item.short_break"
            :longBreak="item.long_break"
            :datePosted="item.date_created"
            :lastModified="item.last_modified"
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