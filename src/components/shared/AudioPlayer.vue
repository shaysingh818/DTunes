<script setup lang="ts">
</script>


<template>
    <div class="audio-bar-container">

        <div class="flex flex-row">

            <div class="song-info">
                <div class="player-image-container">
                    <img :src="thumbnail" :alt="thumbnail" class="image p-1" :id="`${audioFileId.toString()}-player`" />
                </div>

                <div class="player-audio-file-info">
                    <div class="audio-title">
                        <h1>{{ title  }}</h1>
                    </div>
                    <div class="player-icons">

                      <div class="player-icon-item">
                        <div>
                          <i :class="['fas', 'fa-play', 'text-red-800']"></i>
                        </div>
                        <div>
                          <p>{{ plays }}</p>
                        </div>
                      </div>

                      <div class="player-icon-item">
                        <div>
                          <i :class="['fas', 'fa-clock', 'text-red-800']"></i>
                        </div>
                        <div>
                          <p :id="`${audioFileId}-player-duration`">{{ duration }}</p>
                        </div>
                      </div>

                    </div>
                </div>
            </div>

            <div class="player-controls">

                <div class="flex flex-col gap-2">
                    <div class="player-buttons">
                        <div @click="prevFile()">
                            <i :class="['fas', 'fa-backward', 'text-white']"></i>
                        </div>
                        <div @click="audioQueueStore.rewind()">
                            <i :class="['fas', 'fa-rotate-left', 'text-white']"></i>
                        </div>
                        <div v-if="audioQueueStore.isPaused()" class="play-button" @click="pauseFile()">
                            <i :class="['fas', 'fa-pause', 'text-white']"></i>
                        </div>
                        <div v-if="audioQueueStore.isPlaying()" class="play-button" @click="playFile()">
                            <i :class="['fas', 'fa-play', 'text-white']"></i>
                        </div>
                        <div @click="audioQueueStore.forward()">
                            <i :class="['fas', 'fa-rotate-right', 'text-white']"></i>
                        </div>
                        <div @click="nextFile()">
                            <i :class="['fas', 'fa-forward', 'text-white']"></i>
                        </div>
                    </div>
                    <div class="audio-duration">
                        <div class="w-full bg-gray-200 rounded-full h-1 dark:bg-gray-700">
                            <div class="bg-red-600 h-1 rounded-full" id="duration-tracker"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>


<script setup>
import { audioStore, AudioFile } from '../../api/AudioFile';
import { audioQueueStore } from '../../api/AudioQueue'; 
</script>

<script>
import { audioStore } from '../../api/AudioFile';
import { audioQueueStore } from '../../api/AudioQueue'; 
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'AudioPlayer',
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
      await audioQueueStore.playAudio();
    },
    async pauseFile() {
        audioQueueStore.pauseAudio();
    },
    async nextFile() {
      await audioQueueStore.nextAudioFile();
      await this.populatePlayerInformation();
    },
    async prevFile() {
      await audioQueueStore.prevAudioFile();
      await this.populatePlayerInformation();
    },
    async populatePlayerInformation() {

      if(audioQueueStore.currAudioFile) {

        const fileBuffer = await readFile(`dtunes-audio-app/images/${audioQueueStore.currAudioFile.thumbnail}`, {
            baseDir: BaseDirectory.Data,
        });

        const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
        let imageElem = document.getElementById(`${audioQueueStore.currAudioFile.audio_file_id.toString()}-player`);
        if(imageElem && imageElem instanceof HTMLImageElement) {
          imageElem.src = imageUrl;
        } else {
          console.log(`${audioQueueStore.currAudioFile.audio_file_id} not found`)
        }
   
        let durationElem = document.getElementById(`${audioQueueStore.currAudioFile.audio_file_id.toString()}-player-duration`);
        if(durationElem) {
          durationElem.innerHTML = await audioStore.convertSecondsToMinutes(parseInt(audioQueueStore.currAudioFile.duration)); 
        }

      }
    },
  },
  async mounted() {
    await this.populatePlayerInformation();
  },
}
</script>


<style scoped>

.audio-bar-container {
  position: fixed;
  bottom: 0;
  left: 0;
  width: 100%;
  height: 75px; 
  background-color: #333;
  color: white;
  padding: 10px;
}


.player-image-container {
  border-radius: 10%;
  height: 55px;
  width: 55px;
  background-size: cover;
  background-position: center;
}

.song-info {
  display: flex;
  flex-direction: row;
  gap: 0.5rem; 
  justify-content: left;
  align-items: left;
  height: 15%; 
  width: 35%;
}

.player-controls {
  display: flex;
  justify-content: left;
  align-items: left;
  height: 20%;
  width: 55%;  
}

.image {
  border: 1px; 
  border-radius: 15%; 
  height: 100%;
  width: 100%;   
}

.text-container {
  padding: 4px; 
  height: 100%;
  justify-content: left;
}

.play-button {
  border-radius: 50%;
  display: flex;
  align-items: center;
  background-color: red;
  width: 30px;
  height: 30px;
  justify-content: center; 
}

.player-icons {
  display: flex;
  flex-direction: row;
  gap: 1.0rem; 
}

.player-icon-item {
  display: flex;
  flex-direction: row;
  gap: 0.5rem;
  justify-content: center; 
  align-items: center;
}

.player-audio-file-info {
  display: flex;
  flex-direction: column;
}

.audio-title {
  max-width: 175px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}

.audio-duration {
    width: 400px;
}

.player-buttons {
  display: flex;
  flex-direction: row;
  gap: 1rem; 
  justify-content: center;
  align-items: center;
}

hr {
  border: 1px solid gray 
}

h1 {
    font-size: 15px;
}

p {
    font-size: 10px;
}

</style>
