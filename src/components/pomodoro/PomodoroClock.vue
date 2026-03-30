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
import { audioQueueStore } from '../../api/AudioQueue';
</script>


<script>
import { ref,  onBeforeMount, onUnmounted } from 'vue'
import { pomodoroStore } from '../../api/Pomodoro';
import { onBeforeRouteLeave } from 'vue-router';
import { audioQueueStore } from '../../api/AudioQueue';

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
    sessionId: {
      type: Number,
      required: true,
    },
  },
  data() {
    return {
      timerValue: "0:00",
      timer: null,
      audioQueue: null
    }
  },
  methods: {
    async selectShortBreak() {

      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.reset(); 
      }

      duration = this.shortBreak * 60; 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      );
      pomodoroStore.pomodoroTimer.setPomodoro(false); 
    }, 
    async selectLongBreak() {

      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.reset(); 
      }

      duration = this.longBreak * 60;
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      );
      pomodoroStore.pomodoroTimer.setPomodoro(false); 
    },
    async selectFocusDuration() {

      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.reset(); 
      }

      duration = this.duration * 60; 
      pomodoroStore.pomodoroTimer = new PomodoroTimer(
        duration, 
        this.updateTimerValue
      );
      pomodoroStore.pomodoroTimer.setPomodoro(true); 
    },
    async startTimer() {

      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.start();
        console.log(pomodoroStore.pomodoroTimer.isPlaying());  
      } else {
        console.log("Timer not defined"); 
      }

      if(audioQueueStore.isPlaying()) {
        await audioQueueStore.playAudio(); 
      }

    },
    async pauseTimer() {
      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.pause(); 
      }

      if(audioQueueStore.isPaused()) {
        audioQueueStore.pauseAudio();
      }

    },
    async resumeTimer() {
      if(pomodoroStore.pomodoroTimer) {
        pomodoroStore.pomodoroTimer.resume(); 
      }


      if(audioQueueStore.isPlaying()) {
        await audioQueueStore.playAudio(); 
      }

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
  async mounted() {
    duration = this.duration * 60; 
  }
}



</script>
