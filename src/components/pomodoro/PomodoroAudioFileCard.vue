
<template>
    <div class="hover:bg-stone-900">
        <div class="audio-file-card-container" :id="`audio-card-image-${audioFileId.toString()}`">
          <div class="audio-file-card-container-content">
            <div class="audio-file-card-text-content">
              <h1>{{ title }}</h1>
            </div>
            <div class="audio-file-card-icon-content">
                <div @click="editFile()" class="hover:bg-stone-400">
                  <i :class="['fas', 'fa-edit', 'text-white']"></i>
                </div>
                <div @click="queueAudio()" class="hover:bg-stone-400">
                  <i :class="['fas', 'fa-play', 'text-white']"></i>
                </div> 
                <div v-if="pomodoroRemove == true" @click="removeFile()" class="hover:bg-stone-400">
                    <i :class="['fas', 'fa-remove', 'text-white']"></i>
                </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { audioQueueStore } from "../../api/AudioQueue"; 
import { pomodoroStore } from "../../api/Pomodoro";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'PomodoroAudioFileCard',
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
    thumbnail: {
      type: String,
      required: true,
    },
    filePath: {
      type: String,
      required: true,
    },
    pomodoroRemove: {
      type: Boolean,
      required: true,
    },
  },
  methods: {

    async queueAudio() {

        const audioFile =  await audioStore.viewAudioFile(this.audioFileId.toString());

        if(audioQueueStore.active == false) {
          await audioQueueStore.queue([audioFile]);
          await audioQueueStore.setCurrAudioFile();
          await audioQueueStore.initPlayer();
        } else {
          await audioQueueStore.queue([audioFile]);
          await audioQueueStore.setCurrAudioFile();
        } 
        
    },

    async removeFile() {
      const removeResult = await pomodoroStore.removeAudioFilePomodoro(this.sessionId.toString(), this.audioFileId);
      if(removeResult == "Success") {
        alert("Removed Song From Pomodoro Session");
        await this.$nextTick();
        await pomodoroStore.viewPomodoroAudioFiles(this.sessionId.toString());
      } else {
        alert("Could not remove song from genre", removeResult); 
      }
    },
    async editFile() {
      this.$router.push({ path: `/audio-files/edit/${this.audioFileId}`})
    }
  },
  async mounted() {

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });
    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));

    let imageElem = document.getElementById(`audio-card-image-${this.audioFileId.toString()}`);
    if(imageElem) {
      imageElem.style.backgroundImage = `url(${imageUrl})`; 
    } else {
      console.log(`${this.audioFileId} not found AUDIO FILE`)
    }

  },
}
</script>


<style scoped>


h1 {
  font-weight: bold;
  color: white;

}

p {
  font-size: 12px;
  color: rgb(209 213 219);
  overflow: hidden;
}

</style>
