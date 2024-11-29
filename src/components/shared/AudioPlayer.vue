<script setup lang="ts">
</script>


<template>
    <div class="audio-bar-container">

        <div class="flex flex-row gap-6">

            <div class="song-info">
                <div class="flex flex-row gap-2"> 
                    <div class="image-container">
                        <img :src="thumbnail" :alt="thumbnail" class="image p-1" :id="`${audioFileId.toString()}-player`" />
                    </div>

                    <div class="flex flex-col">
                        <div>
                            <h1>{{ title  }}</h1>
                        </div>
                        <div>
                            <p>{{ datePosted }}</p>
                        </div>
                    </div>

                </div>
            </div>

            <div class="player-controls">

                <div class="flex flex-col gap-2">
                    <div class="player-buttons">
                        <div @click="audioStore.rewind()">
                            <i :class="['fas', 'fa-backward', 'text-white']"></i>
                        </div>
                        <div v-if="audioStore.playing == true" class="play-button" @click="pauseFile()">
                            <i :class="['fas', 'fa-pause', 'text-white']"></i>
                        </div>
                        <div v-if="audioStore.resume == true && audioStore.playing == false" class="play-button" @click="resumeFile()">
                            <i :class="['fas', 'fa-play', 'text-white']"></i>
                        </div>
                        <div @click="audioStore.forward()">
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
import { audioStore, AudioFile } from '../../api/AudioFIle';
</script>

<script>
import { audioStore, AudioFile } from '../../api/AudioFIle';
import { invoke } from "@tauri-apps/api/core";
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


        const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
            baseDir: BaseDirectory.Data,
        });

        const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
        let imageElem = document.getElementById(`${this.audioFileId.toString()}-player`);
        if(imageElem) {
          imageElem.src = imageUrl;
        } else {
          console.log(`${this.audioFileId} not found`)
        }

        audioStore.playAudio(audioFile);
    },
    async pauseFile() {
        audioStore.pauseAudio();
    },
    async resumeFile() {
        audioStore.resumeAudio();
    },
  },
  async mounted() { 

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });

    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL: ", imageUrl)
    let imageElem = document.getElementById(`${this.audioFileId.toString()}-player`);
    if(imageElem) {
      imageElem.src = imageUrl;
    } else {
      console.log(`${this.audioFileId} not found`)
    }
  },
}

const updateDuration = () => {
  const durationElement = document.getElementById("duration-tracker");
  const currentTime = audioStore.currentTime; 
  if(durationElement && currentTime > 0 && audioStore.playing)  {
    const value = (currentTime/audioStore.duration) * 100;
    console.log("CURRENT DURATION: ", value);  
    durationElement.style.width = `${value}%`;
  }
};

setInterval(updateDuration, 1000);

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

.song-info {
    display: flex;
    justify-content: left;
    align-items: left;
    height: 50px; 
    width: 30%; 
    /* border: 1px solid white  */
}

.player-controls {
    display: flex;
    justify-content: left;
    align-items: left;
    height: 50px;
    width: 70%;  
}


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

.play-button {
    border-radius: 50%;
    display: flex;
    align-items: center;
    background-color: red;
    width: 30px;
    height: 30px;
    justify-content: center; 
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