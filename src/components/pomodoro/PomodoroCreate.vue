
<template>
    <button class="bottom-right-button" @click="openWindow = true">
      <i :class="['fas', 'fa-add', 'text-white-800']"></i>
    </button>
    <Teleport to="body">
      <div v-if="openWindow" class="modal">
        <div class="modal-container">
          <div class="flex flex-col gap-2">
              <div>
                <div class="form-header">
                    <div class="flex flex-row gap-10">
                        <div>
                            <div class="form-title">
                                <h1>Create Pomodoro Session</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6">
                    <div>
                        <input type="text" id="session_name" name="session_name" placeholder="Enter Session Name"><br>
                    </div>
                    <div>
                        <input type="text" id="duration" name="duration" placeholder="Enter Focus Duration"><br>
                    </div>
                    <div>
                        <input type="text" id="duration_limit" name="duration_limit" placeholder="Enter Focus Duration Limit"><br>
                    </div>
                    <div>
                        <input type="text" id="short_break" name="short_break" placeholder="Enter Duration For Short Breaks"><br>
                    </div>
                    <div>
                        <input type="text" id="long_break" name="long_break" placeholder="Enter Duration For Long Breaks"><br>
                    </div>
                </div>
              </div>
              <br>
              <div class="form-button-container">
                <div class="flex flex-row gap-2">
                    <div>
                        <button @click="openWindow = false" class="close-button" type="button">Close</button> 
                    </div>
                    <div>
                        <button @click="submitForm" class="upload-button" type="button">Create</button> 
                    </div>
                </div>
              </div>
          </div>
        </div>

      </div>
  </Teleport>
</template>

<script setup>
import { ref } from 'vue'
import { pomodoroStore } from '../../api/Pomodoro';

const openWindow = ref(false);


</script>

<script>

export default {
    name: 'PomodoroCreate',
    methods: {

      async submitForm() {

        const sessionName = document.getElementById('session_name');
        const duration = document.getElementById('duration');
        const durationLimit = document.getElementById('duration_limit');
        const shortBreak = document.getElementById('short_break');
        const longBreak = document.getElementById('long_break');

        console.log("SESSION NAME: ", sessionName.value);
        console.log("DURATION: ", duration.value);
        console.log("DURATION LIMIT: ", durationLimit.value);
        console.log("SHORT BREAK: ", shortBreak.value);
        console.log("LONG BREAK: ", longBreak.value);

        try {

          const shortInt = parseInt(shortBreak.value);
          const longInt = parseInt(longBreak.value); 
          const durationInt = parseInt(duration.value); 
          const durationLimitInt = parseInt(durationLimit.value);
          
          const durationLengthRequirement = durationInt < 60;
          const shortBreakLengthRequirement = shortInt < 15;
          const longBreakLengthRequirement = longInt < 30;


          const sessionNameValidation = sessionName.value.length > 0;
          const durationValidation = durationInt > 0 && durationLengthRequirement;
          const durationLimitValidation = durationLimitInt > 0;
          const shortBreakValidation = shortInt > 0 && shortBreakLengthRequirement;
          const longBreakValidation = longInt > 0 && longBreakLengthRequirement;

          if(sessionNameValidation && durationValidation && durationLimitValidation && shortBreakValidation && longBreakValidation) {

            const response = await pomodoroStore.createSession(
              sessionName.value,
              durationInt,
              durationLimitInt,
              shortInt,
              longInt
            );

            if(response == "Success") {
              console.log("INSERT SUCCESSFUL");
              alert("Success");
              this.$router.push('pomodoro/');
              await this.$nextTick(); 
              await pomodoroStore.loadSessions();
            } else {
              console.log("SOMETHING WENT WRONG");
            }
          } else if(!sessionNameValidation) {
            alert("Must provide name for pomodoro session");
          } else if(!durationValidation) {
            alert("Duration must be less than 60 minutes");
          } else if(!shortBreakLengthRequirement) {
            alert("Short breaks must be less than 15 minutes");
          } else if(!longBreakLengthRequirement) {
            alert("Long breaks must be less than 30 minutes"); 
          }

        } catch (error) {
          alert("Unable to parse integer information");    
        }

      }

    }
}
</script>

<style scoped>

.bottom-right-button {
  position: fixed;  /* Fixed positioning relative to the viewport */
  bottom: 20px;     /* 20px from the bottom of the screen */
  right: 20px;      /* 20px from the right of the screen */
  padding: 10px 20px;
  font-size: 16px;
  background-color: rgb(153 27 27);
  color: white;
  border: none;
  border-radius: 50%;
  height: 70px;
  width: 70px;
  cursor: pointer;
}

.modal {
  border-radius: 3%;
  position: fixed;
  z-index: 999;
  top: 10%;
  left: 50%;
  width: 500px;
  height: 600px;
  margin-left: -160px;
  background-color: rgb(28 25 23); 
}

.modal-container {
  display: flex;
  justify-content: center;
  align-items: center;
}

.form-button-container {
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 4px; 
}

.form-header {
  /* border: 1px solid #ccc; */
  width: 500px;
  display: flex;
  justify-content: center;
  align-items: center;
}

.form-title {
  /* border: 1px solid #ccc; */
  padding: 6px; 
}

.form-field-container {
  display: flex;
  justify-content: center;
  align-items: center;
  /* border: 1px solid #ccc; */
}


h1 {
  color: rgb(153 27 27);
  font-weight: bold;
  font-size: 30px;
  padding: 4px;  
}

input {
    border-radius: 3%;
    height: 50px;
    width: 300px;
    padding: 8px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}

input[type="file"] {
    border-radius: 3%;
    height: 50px;
    width: 300px;
    padding: 8px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}

.form-button {
    border-radius: 5%;
    width: 50px; 
    color: white;
    padding: 8px;
    border: 1px solid #ccc;
    align-items: center;
    justify-content: center;
}

.add-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(153 27 27);
    color: white;
}

.close-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
}

/* file button upload styling */ 
label {
  background-color: rgb(41 37 36);
  color: rgb(153 27 27);
  padding: 12px 24px;
  border-radius: 5px;
  font-size: 14px;
  cursor: pointer;
  font-weight: bold;
}

.upload-btn:hover {
  background-color: #0056b3;
}

#file-upload {
  display: none;
}

</style>

