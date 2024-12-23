<template>
    <div class="card-component">
        <div class="card-container">

            <div class="flex flex-col gap-2">

              <div class="pomodoro-header">
                <h1>{{ name }}</h1>
              </div>
              
              <div class="pomodoro-information">
                <div class="flex flex-col">
                  <div>
                      <p>Duration: {{ duration }}</p>
                  </div>
                  <div>
                      <p>Duration Limit: {{ durationLimit }}</p>
                  </div>
                  <div>
                      <p>Short Break: {{ shortBreak }}</p>
                  </div>
                  <div>
                      <p>Long Break: {{ longBreak }}</p>
                  </div>
                  <div>
                      <p>Created: {{ datePosted }}</p>
                  </div>
                  <div>
                      <p>Last Modified: {{ lastModified }}</p>
                  </div>
                </div>
              </div>

              <div class="actions-bar">
                  <div class="flex flex-row gap-4">
                    <div @click="addAudioFile">
                      <i :class="['fas', 'fa-add', 'text-red-800']"></i>
                    </div>
                    <div @click="detailPage">
                      <i :class="['fas', 'fa-play', 'text-red-800']"></i>
                    </div>
                    <div @click="removeSession">
                      <i :class="['fas', 'fa-trash', 'text-red-800']"></i>
                    </div>
                  </div>
              </div>

            </div>
        </div>
    </div>
</template>


<script>
import { pomodoroStore } from '../../api/Pomodoro';

export default {
  name: 'PomodoroCard',
  props: {
    sessionId: {
      type: Number,
      required: true,
    },
    name: {
      type: String,
      required: true,
    },
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
    datePosted: {
      type: String,
      required: true,
    },
    lastModified: {
      type: String,
      required: true,
    },
  },
  methods: {
    async removeSession() {
      const userChoice = await window.confirm(`Are you sure you want to delete: ${this.title}`);
      if(userChoice) {
        const deleteResult = await pomodoroStore.deleteSession(this.sessionId.toString());
        if(deleteResult == "Success") {
          alert("Successfully deleted: ", this.name);
          this.$router.push('pomodoro/');
          await this.$nextTick(); 
          await pomodoroStore.loadSessions();
        } 
      } else {
        alert("Could not delete genre: ", deleteResult);  
      }
    },
    async detailPage() {
      this.$router.push({ path: `/pomodoro/edit/${this.sessionId}`})
    },
    async addAudioFile() {
      this.$router.push({ path: `/pomodoro/add-audio-file/${this.sessionId}`});
    }
  },
  async mounted() {

    console.log(new Date(this.lastModified).toString()); 

  }
}
</script>


<style scoped>

.card-component {
  width: 185px;
}

.card-container {
    border-radius: 3%;
    display: flex;
    height: 230px;
    width: 185px;
    background-color: rgb(28 25 23);
}


.pomodoro-header {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 185px;
  padding: 6px; 
  white-space: nowrap; 
  overflow: hidden; 
  text-overflow: ellipsis; 
}

.actions-bar {
  display: flex;
  align-items: center;
  justify-content: center;
  /* border: 1px solid red; */
  height: 40px; 
  width: 185px;
  margin-top: auto;  
}

.pomodoro-information {
  display: flex; 
  justify-content: left;
  align-items: left;
  /* border: 1px solid red; */
  padding: 10px; 
}


h1 {
  color: rgb(153 27 27);
  font-weight: bold; 
}

p {
  color: white;
  font-size: 12px;
  font-weight: bold; 
}


</style>