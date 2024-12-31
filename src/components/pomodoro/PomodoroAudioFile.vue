
<template>
    <div class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="list-view-image-container">
                <img :src="thumbnail" :alt="thumbnail" :id="`${audioFileId.toString()}-genre-audio-file`" class="list-view-image p-1" />
            </div>
        </div>
        <div class="flex-auto w-64">
            <div class="list-view-text-container">
              <h2>{{ title }}</h2>
              <p>{{ datePosted }}</p>
            </div>
        </div>
        <div class="flex-auto w-32">
          <div class="list-view-trailing-container">
            <div class="grid grid-flow-col auto-cols-max space-x-4">
              <div><p1 :id="`${audioFileId.toString()}-duration`" >{{ duration }}</p1></div>
              <div v-if="pomodoroRemove == false" @click="addFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-add', 'text-red-800']"></i>
              </div>
              <div v-if="pomodoroRemove == true" @click="removeFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-remove', 'text-red-800']"></i>
              </div>
            </div>
          </div>
        </div>
    </div>


</template>


<script setup>
import { audioStore } from "../../api/AudioFile";
import { pomodoroStore } from "../../api/Pomodoro";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { pomodoroStore } from "../../api/Pomodoro";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'PomodoroAudioFile',
  props: {
    sessionId: {
      type: Number,
      required: true,
    },
    audioFileId: {
      type: Number,
      required: true,
    },
    title: {
      type: String,
      required: true,
    },
    datePosted: {
      type: String,
      required: true,
    },
    duration: {
      type: String,
      required: true,
    }, 
    filePath: {
      type: String,
      required: true,
    },
    lastModified: {
      type: String,
      required: true,
    },
    plays: {
      type: Number,
      required: true,
    },
    sampleRate: {
      type: String,
      required: true,
    },
    thumbnail: {
      type: String,
      required: true,
    },
    pomodoroRemove: {
      type: Boolean,
      required: true,
    }, 
  },
  methods: {
 
    async removeFile() {
      console.log(`SESSION ID: ${this.sessionId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const removeResult = await pomodoroStore.removeAudioFilePomodoro(this.sessionId.toString(), this.audioFileId);
      if(removeResult == "Success") {
        alert("Removed Song From Genre");
        this.$forceUpdate(); 
      } else {
        alert("Could not remove song from genre", removeResult); 
      }
    },

    async addFile() {
      console.log(`SESSION ID: ${this.genreId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const addResult = await pomodoroStore.addAudioFilePomodoro(this.sessionId.toString(), this.audioFileId);
      if(addResult == "Success") {
        alert("Added Song To Genre")
      } else {
        alert("Could not add song to genre", addResult); 
      }
    }

  },
  async mounted() {

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });

    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL: ", imageUrl)

    let imageElem = document.getElementById(`${this.audioFileId.toString()}-genre-audio-file`);
    if(imageElem) {
      imageElem.src = imageUrl;
    } else {
      console.log(`${this.audioFileId} not found`)
    }

    let durationElem = document.getElementById(`${this.audioFileId.toString()}-duration`);
    durationElem.innerHTML = await audioStore.convertSecondsToMinutes(this.duration); 

  }
}


</script>

<style scoped>


h2 {
    color: white;
    font-weight: bold;
}

p {
    font-size: 12px;
    color: rgb(209 213 219);
}

p1 {
    font-size: 14px;
    color: rgb(209 213 219);
}

</style>