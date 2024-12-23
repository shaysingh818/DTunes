<template>
    <div class="pomodoro-gradient-background">
      <div class="pomodoro-clock-container">
        <div class="pomodoro-break-options">
          <div>
            <p style="color: white;">Pomodoro</p>
          </div>
          <div>
            <p style="color: white">Short Break</p>
          </div>
          <div>
            <p style="color: white">Long Break</p>
          </div>
        </div>
        <div class="pomodoro-duration-display">
            <h1>15:00</h1>
        </div>

        <div class="pomodoro-button-container">  
          <button @click="submitForm" class="pomodoro-button" type="button">Start</button> 
        </div>
      </div>
    </div>
</template>

<script>

export default {
  name: 'PomodoroClock',
  props: {
    duration: {
      type: String,
      required: true,
    },
    durationLimit: {
      type: String,
      required: true,
    },
    shortBreak: {
      type: String,
      required: true,
    },
    longBreak: {
      type: String,
      required: true,
    },
  },
  methods: {
    async detailPage() {
      this.$router.push({ path: `/pomodoro/edit/${this.pomodoroId}`})
    }
  }
}
</script>