<script>
import PomodoroMetricCard from '../../components/pomodoro/PomodoroMetricCard.vue'; 
import { Bar } from 'vue-chartjs'; 
import { Chart as ChartJS, BarElement, CategoryScale, LinearScale, Tooltip, Legend } from 'chart.js';

ChartJS.register(BarElement, CategoryScale, LinearScale, Tooltip, Legend)

export default {
  components: { PomodoroMetricCard, Bar },
  data() {
    const labels = [];
    for (let i = 29; i >= 0; i--) {
      const date = new Date();
      date.setDate(date.getDate() - i);
      labels.push(date.toLocaleDateString('en-US', { month: 'short', day: 'numeric' }));
    }

    return {
      data: {
        labels,
        datasets: [
          {
            label: 'Hours Focused',
            data: [2, 4, 3, 5, 1, 2, 3, 4, 2, 1, 5, 3, 4, 2, 3, 2, 6, 7, 9, 3, 4, 2, 5, 1, 3, 6, 4, 2, 5],
            backgroundColor: 'rgba(153, 27, 27, 0.8)'
          }
        ]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false
      }
    }
}
}

</script>

<template>

  <div class="page-container">

    <div class="metric-cards">

      <PomodoroMetricCard
        metricTitle="Total Number Hours Focused"
        :metricNumber=24
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Week"
        :metricNumber=10
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Month"
        :metricNumber=37
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Day"
        :metricNumber=3
      />

    </div>

    <div class="bar-chart">
      <Bar :data="data" :options="options" />
    </div>

  </div>


</template>


<style scoped>

.page-container {
    flex-direction: column;
    min-height: 100vh;
    margin: 0; 
}


.metric-cards {
  height: 200px;
  width: 100%; 
  flex-direction: row;
  display: flex; 
}

.bar-chart {
  flex: 1 1 0; 
  width: 100%;
  min-height: 0; 
}



</style>
