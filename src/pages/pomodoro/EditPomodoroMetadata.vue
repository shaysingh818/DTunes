
<template>
    <button class="edit-button" @click="openWindow = true">
      <i :class="['fas', 'fa-edit', 'text-white-800']"></i>
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
                                <h1>Edit Pomodoro Session</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6 gap-y-8">
                    <div>
                        <input type="text" id="session_name" name="session_name" :placeholder="sessionName"><br>
                    </div>
                    <div>
                        <input type="text" id="duration" name="duration" :placeholder="duration"><br>
                    </div>
                    <div>
                        <input type="text" id="duration_limit" name="duration_limit" :placeholder="durationLimit"><br>
                    </div>
                    <div>
                        <input type="text" id="short_break" name="short_break" :placeholder="shortBreak"><br>
                    </div>
                    <div>
                        <input type="text" id="long_break" name="long_break" :placeholder="longBreak"><br>
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
                        <button @click="submitForm" class="upload-button" type="button">Submit</button> 
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
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import { audioStore } from '../../api/AudioFile';

let openWindow = ref(false);

</script>

<script>
import { ref } from 'vue'
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import { audioStore } from '../../api/AudioFile';
import { pomodoroStore } from '../../api/Pomodoro';

let openWindow = ref(false);

export default {
    name: 'EditPomodoroMetadata',
    props: {
        sessionId: {
            type: Number,
            required: true,
        },
        sessionName: {
            type: String,
            required: true,
        },
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
    methods: {
        playlistAudioFileSearch(){
            alert("Audio File Search Button Clicked");
        },
        async submitForm() {

            const sessionName = document.getElementById('session_name');
            const duration = document.getElementById('duration');
            const durationLimit = document.getElementById('duration_limit');
            const shortBreak = document.getElementById('short_break');
            const longBreak = document.getElementById('long_break');

            const sessionNameValidation = sessionName.value.length > 0;
            const durationValidation = duration.value.length > 0;
            const durationLimitValidation = durationLimit.value.length > 0;
            const shortBreakValidation = shortBreak.value.length > 0;
            const longBreakValidation = longBreak.value.length > 0;

            let sessionNameSet = '';
            let durationSet = 0; 
            let durationLimitSet = 0; 
            let shortBreakSet = 0; 
            let longBreakSet = 0;
            
            if(!sessionNameValidation) { 
                sessionNameSet = this.sessionName; 
            } else {
                sessionNameSet = sessionName.value;
            }

            if(!durationValidation) { 
                durationSet = this.duration; 
            } else {
                durationSet = duration.value;
            }

            if(!durationLimitValidation) { 
                durationLimitSet = this.durationLimit; 
            } else {
                durationLimitSet = durationLimit.value;
            }

            if(!shortBreakValidation) { 
                shortBreakSet = this.shortBreak; 
            } else {
                shortBreakSet = shortBreak.value;
            }

            if(!longBreakValidation) { 
                longBreakSet = this.longBreak; 
            } else {
                longBreakSet = longBreak.value;
            }

            const response = await pomodoroStore.editSession(
                this.sessionId.toString(),
                sessionNameSet,
                parseInt(durationSet),
                parseInt(durationLimitSet),
                parseInt(shortBreakSet),
                parseInt(longBreakSet)
            );

            if(response == "Success") {
                console.log("Successfully Updated Audio File");
                alert("Success");
            } else {
                console.log("SOMETHING WENT WRONG");
                alert(response);
            }
        }
    }
}

</script>

<style scoped>

h1 {
  color: rgb(153 27 27);
  font-weight: bold;
  font-size: 30px;
  padding: 4px;  
}

</style>