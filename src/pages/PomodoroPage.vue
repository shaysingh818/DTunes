<script>
import SearchComponent from '../components/shared/SearchComponent.vue';
import PomodoroGrid from '../components/pomodoro/PomodoroGrid.vue';
import PomodoroCreate from '../components/pomodoro/PomodoroCreate.vue';

export default {
  components: {SearchComponent, PomodoroGrid, PomodoroCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    pomodoroSearch(){
      alert("Pomodoro Search Button Clicked");
    }
  },
}
</script>

<template>
  <div class="pomodoro-page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Pomodoro Sessions On DTunes" 
          :onClick="pomodoroSearch"
        />
      </div>
      <div class="grid-container">
        <PomodoroGrid /> 
      </div>
    </div>
  </div>
  <PomodoroCreate  />
</template>

<style scoped>

.artist-page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}

.grid-container {
    max-width: 100%; 
    height: 100%;
    margin: 0 auto;
}

.search-box-container {
    max-width: 100%; 
    height: 75px;
    margin: 0 auto; 
}
</style>