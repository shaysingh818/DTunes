<script>
import PomodoroMetricCard from '../../components/pomodoro/PomodoroMetricCard.vue'; 
</script>

<template>

  <div class="page-container">

    <div class="card-grid">

      <div class="cards">

        <PomodoroMetricCard
          metricTitle="Total Number Hours Focused"
          metricNumber=24
        />

        <PomodoroMetricCard
          metricTitle="Average Hours Focused Per Week"
          metricNumber=10
        />

        <PomodoroMetricCard
          metricTitle="Average Hours Focused Per Month"
          metricNumber=37
        />

        <PomodoroMetricCard
          metricTitle="Average Hours Focused Per Day"
          metricNumber=3
        />

      </div>

    </div> 



  </div>


</template>


<style scoped>

.card-grid {
  height: 200px;
  width: 100%; 
  border: 1px solid white; 
}

.cards {
  display: grid; 
  gap: 1rem; 
  margin: 0 auto; 
  overflow: auto;
  border: 1px solid white; 
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

</style>
