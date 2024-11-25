
<template>
    <div class="flex hover:bg-stone-900">
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
              <div><p1>{{ duration }}</p1></div>
              <div class="hover:bg-stone-400"><i :class="['fas', 'fa-list-ul', 'text-red-800']"></i></div>
              <div class="hover:bg-stone-400">
                <i @click="playFile()" :class="['fas', 'fa-play', 'text-red-800']"></i>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script setup>
import { audioStore } from "../../api/AudioFIle";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFIle";

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

        audioStore.playAudio(audioFile);
    }
    
  },
  async mounted() {

    console.log("AUDIO FILE ID: ", this.audioFileId.toString()); 
    const base64Image = await invoke('read_image_from_data_dir', {imageName:  this.thumbnail});
    let imageElem = document.getElementById(this.audioFileId.toString());
    if(imageElem) {
      imageElem.src = `data:image/jpeg;base64,${base64Image}`;
    } else {
      console.log(`${this.audioFileId} not found`)
    }

  }
}

async function readImage(imageName) {
    const base64Image = await invoke('read_image_from_data_dir', {imageName:  imageName});
    return `data:image/jpeg;base64,${base64Image}`;
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



</style>