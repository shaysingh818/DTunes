
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
              <div @click="playFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-play', 'text-red-800']"></i>
              </div>
              <div v-if="genreRemove == false" @click="addFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-add', 'text-red-800']"></i>
              </div>
              <div v-if="genreRemove == true" @click="removeFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-remove', 'text-red-800']"></i>
              </div>
            </div>
          </div>
        </div>
    </div>


</template>


<script setup>
import { audioStore } from "../../api/AudioFile";
import { genreStore } from "../../api/Genre";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { genreStore } from "../../api/Genre";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { artistStore } from "../../api/Artist";
import { updateAudioPlayerInformation } from "../../api/AudioFile";

export default {
  name: 'GenreAudioFile',
  props: {
    genreId: {
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
    genreRemove: {
      type: Boolean,
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
 
    async removeFile() {
      console.log(`GENRE ID: ${this.genreId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const removeResult = await genreStore.removeAudioFileGenre(this.genreId.toString(), this.audioFileId);
      if(removeResult == "Success") {
        alert("Removed Song From Genre");
        // this.$router.push(`/playlists/edit/${this.playlistId}`);
        this.$forceUpdate(); 
      } else {
        alert("Could not remove song from genre", removeResult); 
      }
    },

    async addFile() {
      console.log(`GENRE ID: ${this.genreId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const addResult = await genreStore.addAudioFileGenre(this.genreId.toString(), this.audioFileId);
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