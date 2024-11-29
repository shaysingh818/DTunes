
<template>
    <div @click="playFile()" class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="image-container">
                <img :src="thumbnail" :alt="thumbnail" :id="audioFileId.toString()" class="image p-1" />
            </div>
        </div>
        <div class="flex-auto w-64">
            <div class="text-container">
              <h2>{{ title }}</h2>
              <p>{{ datePosted }}</p>
            </div>
        </div>
        <div class="flex-auto w-32">
          <div class="trailing-container">
            <div class="grid grid-flow-col auto-cols-max space-x-4">
              <div><p1 :id="`${audioFileId.toString()}-duration`" >{{ duration }}</p1></div>
              <div @click="editFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-edit', 'text-red-800']"></i>
              </div>
              <div @click="deleteFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-trash', 'text-red-800']"></i>
              </div>
            </div>
          </div>
        </div>
    </div>


</template>


<script setup>
import { audioStore } from "../../api/AudioFIle";
import AudioFileCreate from "./AudioFileCreate.vue";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFIle";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'SongList',
  props: {
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
  },
  components: {AudioFileCreate},
  methods: {

    async playFile() {

      const audioFile = new AudioFile({
          audioFileId: this.audioFileId,
          dateCreated: this.datePosted,
          duration: this.duration,
          fileName: this.title,
          filePath: this.filePath,
          lastModified: this.lastModified,
          plays: this.plays,
          sampleRate: this.sampleRate,
          thumbnail: this.thumbnail
        });

        if(audioStore.playing) {
          audioStore.pauseAudio();
          audioStore.playAudio(audioFile); 
        } else {
          audioStore.playAudio(audioFile);
        }
    },
    
    async deleteFile() {
      const userChoice = await window.confirm(`Are you sure you want to delete: ${this.title}`);
      if(userChoice) {
        const deleteResult = await audioStore.deleteAudioFile(this.audioFileId.toString()); 
        if(deleteResult == "Success") {
          alert("Successfully deleted: ", this.title);
          this.$router.push('audio-files/');
          await this.$nextTick(); 
          await audioStore.loadAudioFiles(); 
        } 
      } else {
        alert("Could not delete audio file: ", deleteResult);  
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
    console.log("IMAGE URL: ", imageUrl)

    let imageElem = document.getElementById(this.audioFileId.toString());
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

.image-container {
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
}

.image {
  display: flex;
  border: 1px; 
  border-radius: 15%; /* Optional: Rounded corners */
  height: 60px;
  width: 60px;  
}

.text-container {
  padding: 4px; 
  height: 100%;
  justify-content: left;
}

.trailing-container {
  display: flex; 
  height: 100%;
  align-items: center;
  justify-content: center; 
}

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


hr {
  border: 1px solid gray 
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

</style>