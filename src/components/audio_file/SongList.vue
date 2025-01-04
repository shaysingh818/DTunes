
<template>
    <div @click="queueAudio()" class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="list-view-image-container">
                <img :src="thumbnail" :alt="thumbnail" :id="audioFileId.toString()" class="list-view-image p-1" />
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
              <div><p :id="`${audioFileId.toString()}-duration`" >{{ duration }}</p></div>
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
import { audioStore } from "../../api/AudioFile";
import AudioFileCreate from "./AudioFileCreate.vue";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { updateAudioPlayerInformation } from "../../api/AudioFile";

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

    async queueAudio() {
        
        const audioFile =  await audioStore.viewAudioFile(this.audioFileId.toString());
        if(audioStore.queuedAudioFiles.length == 0) {
          console.log("NO songs on queue... adding"); 
          audioStore.queueAudioFiles(audioFile.audio_file_id.toString());
        } 

        if(audioStore.playing) {
          audioStore.pauseAudio();
          audioStore.playAudio(audioFile); 
        } else {
          audioStore.playAudio(audioFile);
        }
    },

    async playFile() {

      const audioFile = new AudioFile({
          audio_file_id: this.audioFileId,
          date_created: this.datePosted,
          duration: this.duration,
          file_name: this.title,
          file_path: this.filePath,
          last_modified: this.lastModified,
          plays: this.plays,
          sample_rate: this.sampleRate,
          thumbnail: this.thumbnail
        });

        if(audioStore.playing) {
          audioStore.pauseAudio();
          audioStore.playAudio(audioFile); 
        } else {
          audioStore.playAudio(audioFile);
        }

        await updateAudioPlayerInformation(
          this.audioFileId.toString(), 
          this.thumbnail,
          parseInt(this.duration)
        );
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