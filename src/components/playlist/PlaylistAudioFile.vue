
<template>
    <div class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="list-view-image-container">
                <img :src="thumbnail" :alt="thumbnail" :id="`${audioFileId.toString()}-playlist-audio-file`" class="list-view-image p-1" />
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
              <div @click="queueAudio" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-play', 'text-red-800']"></i>
              </div>
              <div v-if="playlistRemove == false" @click="addFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-add', 'text-red-800']"></i>
              </div>
              <div v-if="playlistRemove == true" @click="removeFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-remove', 'text-red-800']"></i>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script setup>
import { audioStore, updateAudioPlayerInformation } from "../../api/AudioFile";
</script>

<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile } from "../../api/AudioFile";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { playlistStore } from "../../api/Playlist";
import { updateAudioPlayerInformation } from "../../api/AudioFile";

export default {
  name: 'PlaylistAudioFile',
  props: {
    playlistId: {
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
    playlistRemove: {
      type: Boolean,
      required: true,
    }, 
  },
  methods: {

    async queueAudio() {
        
        const audioFile =  await audioStore.viewAudioFile(this.audioFileId.toString());
        if(audioStore.queuedAudioFiles.length == 0) {
          playlistStore.queuePlaylistAudioFiles(
            this.playlistId, 
            audioFile.audio_file_id.toString()
          ); 
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
      console.log(`PLAYLIST ID: ${this.playlistId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const removeResult = await playlistStore.removeAudioFilePlaylist(this.playlistId.toString(), this.audioFileId);
      if(removeResult == "Success") {
        alert("Removed Song From Playlist");
        // this.$router.push(`/playlists/edit/${this.playlistId}`);
        this.$forceUpdate(); 
      } else {
        alert("Could not remove song from playlist", removeResult); 
      }
    },

    async addFile() {
      console.log(`PLAYLIST ID: ${this.playlistId}`);
      console.log(`AUDIO FILE ID ${this.audioFileId}`); 
      const addResult = await playlistStore.addAudioFilePlaylist(this.playlistId.toString(), this.audioFileId);
      if(addResult == "Success") {
        alert("Added Song To Playlist")
      } else {
        alert("Could not add song to playlist", addResult); 
      }
    }
  },
  async mounted() {

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });

    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL: ", imageUrl)

    console.log("PLAYLIST REMOVE", this.playlistRemove)

    let imageElem = document.getElementById(`${this.audioFileId.toString()}-playlist-audio-file`);
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