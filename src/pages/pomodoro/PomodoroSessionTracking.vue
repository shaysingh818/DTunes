<script>
import { pomodoroTrackingStore } from '../../api/Pomodoro';
import PomodoroMetricCard from '../../components/pomodoro/PomodoroMetricCard.vue'; 
import { Bar } from 'vue-chartjs'; 
import { 
  Chart as ChartJS, 
  BarElement, 
  CategoryScale, 
  LinearScale, 
  Tooltip, 
  Legend 
} from 'chart.js';


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
      totalHours: 0,
      weeklyHours: 0,
      monthlyHours: 0,
      dailyHours: 0,
      data: {
        labels,
        datasets: [
          {
            label: 'Hours Focused',
            data: [],
            backgroundColor: 'rgba(153, 27, 27, 0.8)'
          }
        ]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false
      }
    }
  },
  async mounted() {
    
    await pomodoroTrackingStore.retrieveTrackingMonthlyUsage();

    this.totalHours = await pomodoroTrackingStore.retrieveTotalHours();
    this.monthlyHours = await pomodoroTrackingStore.retrieveMonthlyHoursAverage();
    this.weeklyHours = await pomodoroTrackingStore.retrieveWeeklyHoursAverage();
    this.dailyHours = await pomodoroTrackingStore.retrieveDailyHoursAverage();

    const myData = pomodoroTrackingStore.monthly_usage_sessions
      .map(item => item.hours_per_day);

    this.data = {
    ...this.data,
    datasets: [
      {
        ...this.data.datasets[0],
        data: myData
      }
    ]
  };

  }
}

</script>

<template>

  <div class="page-container">

    <div class="metric-cards">

      <PomodoroMetricCard
        metricTitle="Total Number Hours Focused"
        :metricNumber=totalHours
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Week"
        :metricNumber=weeklyHours
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Month"
        :metricNumber=monthlyHours
      />

      <PomodoroMetricCard
        metricTitle="Average Hours Focused Per Day"
        :metricNumber=dailyHours
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
