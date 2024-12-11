
<template>
    <div class="hover:bg-stone-900">
        <div class="audio-file-card-container" :id="`audio-card-image-${audioFileId.toString()}`">
          <div class="audio-file-card-container-content">
            <div class="audio-file-text-content">
              <h1>{{ title }}</h1>
              <p>{{ datePosted }}</p>
            </div>
            <div class="audio-file-card-icon-content">
              <div class="grid grid-flow-col auto-cols-max space-x-4">
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-list-ul', 'text-white']"></i></div>
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-play', 'text-white']"></i></div>
                <div><p>{{ duration }}</p></div>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'SongCard',
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
    thumbnail: {
      type: String,
      required: true,
    },
    filePath: {
      type: String,
      required: true,
    },
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