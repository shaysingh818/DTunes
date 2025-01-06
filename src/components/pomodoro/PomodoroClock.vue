<template>
    <div class="pomodoro-gradient-background">
      <div class="pomodoro-clock-container">
        <div class="pomodoro-break-options">
          <div @click="selectFocusDuration()" class="flex hover:bg-stone-700">
            <p style="color: white;">Pomodoro</p>
          </div>
          <div @click="selectShortBreak()" class="flex hover:bg-stone-700">
            <p style="color: white">Short Break</p>
          </div>
          <div @click="selectLongBreak()" class="flex hover:bg-stone-700">
            <p style="color: white">Long Break</p>
          </div>
        </div>
        <div class="pomodoro-duration-display">
            <h1>{{ timerValue }}</h1>
        </div>

        <div v-if="startCondition()" class="pomodoro-button-container">  
          <button @click="startTimer()" class="pomodoro-button" type="button">Start</button> 
        </div>

        <div v-if="resumeCondition()" class="pomodoro-button-container">  
          <button @click="resumeTimer()" class="pomodoro-button" type="button">Resume</button> 
        </div>
 
        <div  v-if="pauseCondition()" class="pomodoro-button-container">  
          <button @click="pauseTimer()" class="pomodoro-button" type="button">Pause</button> 
        </div>

      </div>
    </div>
</template>

<script setup>
import { pomodoroStore, PomodoroTimer } from '../../api/Pomodoro';
</script>


<script>
import { ref,  onBeforeMount, onUnmounted } from 'vue'
import { pomodoroStore } from '../../api/Pomodoro';
import { onBeforeRouteLeave } from 'vue-router';


let duration = ref(0);


export default {
  name: 'PomodoroClock',
  props: {
    duration: {
      type: Number,
      required: true,
    },
    durationLimit: {
      type: Number,
      required: true,
    },
    shortBreak: {
      type: Number,
      required: true,
    },
    longBreak: {
      type: Number,
      required: true,
    },
  },
  data() {
    return {
      timerValue: "0:00",
      timer: null
    }
  },
  methods: {
    async selectShortBreak() {
      duration = this.shortBreak * 60; 
      console.log(`Setting duration ${duration}`); 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      ); 
    }, 
    async selectLongBreak() {
      duration = this.longBreak * 60;
      console.log(`Setting duration ${duration}`); 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      ); 
    },
    async selectFocusDuration() { 
      duration = this.duration * 60; 
      console.log(`Setting duration ${duration}`); 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      ); 
    },
    async startTimer() {

      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.start();
        console.log(pomodoroStore.pomodoroTimer.isPlaying());  
      } else {
        console.log("Timer not defined"); 
      }
    },
    async pauseTimer() {
      if(pomodoroStore.pomodoroTimer) {
        console.log("Pausing Timer"); 
        pomodoroStore.pomodoroTimer.pause(); 
      }
    },
    async resumeTimer() {
      if(pomodoroStore.pomodoroTimer) {
        console.log("Resuming timer"); 
        pomodoroStore.pomodoroTimer.resume(); 
      }
    },
    async mounted() { 
      duration = this.duration * 60; 
      console.log(`Setting duration ${duration}`); 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        'dtunes-alarm-sound.mp3',
        this.updateTimerValue,
      ); 
    },
    startCondition() {
      if(pomodoroStore.pomodoroTimer) {
        const play = pomodoroStore.pomodoroTimer.isPlaying();
        const paused = pomodoroStore.pomodoroTimer.isPaused(); 
        return !play && !paused;
      }
    },
    pauseCondition() {
      if(pomodoroStore.pomodoroTimer) {
        const play = pomodoroStore.pomodoroTimer.isPlaying();
        const paused = pomodoroStore.pomodoroTimer.isPaused(); 
        return play && !paused;
      }
    },
    resumeCondition() {
      if(pomodoroStore.pomodoroTimer) {
        const play = pomodoroStore.pomodoroTimer.isPlaying();
        const paused = pomodoroStore.pomodoroTimer.isPaused(); 
        return play && paused;
      }
    },
    updateTimerValue(newTime) {
      this.timerValue = newTime; 
    },
  },
  beforeUnmount() {
    console.log("TIMER PAGE HAS BEEN UNMOUNTED");
    if(pomodoroStore.pomodoroTimer) {
      pomodoroStore.pomodoroTimer.stop();
      console.log("TIMER has been stopped");  
    } 
  },
}



</script>