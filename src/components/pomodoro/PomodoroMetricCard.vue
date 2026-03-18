<template>

  <div class="card-component">

    <div class="card-container">

      <div class="flex flex-col gap-2">

        <div class="title-header">
            <h1> {{ metricTitle }} </h1>
        </div>

        <div class="metric-header">
            <h1> {{ metricNumber }} </h1>
        </div>

      </div>

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
  width: 400px;
  height: 100%; 
  border: 1px solid white; 
}

.card-container {
  border-radius: 3%;
  display: flex;
  height: 250px; 
  width: 300px; 
  background-color: rgb(28 25 23);
}

.title-header {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 60px; 
  padding: 6px; 
  white-space: nowrap;
  overflow: hidden; 
  text-overflow: ellipsis;
}


.metric-header {

}

</style>
