<template>

  <div class="card-component">

    <div class="card-container">

      <h1> {{ metricTitle }} </h1>

      <h2> {{ metricNumber }} </h2>


    </div>

  </div>

</template>

<script>

export default {
  name: 'PomodoroMetricCard',
  props: {
    metricTitle: {
      type: String,
      required: true,
    },
    metricNumber: {
      type: Number,
      required: true,
    }
  },
}

</script>

<style scoped>

.card-component {
  width: 100%;
  height: 100%;
  padding: 10px; 
}

.card-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  text-align: center; 
  justify-content: center;
  height: 100%;
  width: 100%;
  border-radius: 3%;
  background-color: rgb(28 25 23);
  padding: 20px;
}

h1 {
  color: white;
  font-weight: bold; 
}

h2 {
  color: rgb(153 27 27);
  font-weight: bold;
  font-size: 30px; 
}

</style>
