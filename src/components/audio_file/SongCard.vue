
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
                <div @click="deleteFile()" class="hover:bg-stone-400">
                    <i :class="['fas', 'fa-trash', 'text-white']"></i>
                </div>
            </div>


          </div>
        </div>
    </div>
</template>


<script>
import { invoke } from "@tauri-apps/api/core";
import { audioStore, AudioFile, updateAudioPlayerInformation } from "../../api/AudioFile";
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
  methods: {

    async queueAudio() {
        
        const audioFile =  await audioStore.viewAudioFile(this.audioFileId.toString());
        if(audioStore.queuedAudioFiles.length == 0) {
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
    async deleteFile() {
      const userChoice = await window.confirm(`Are you sure you want to delete: ${this.title}`);
      if(userChoice) {
        const deleteResult = await audioStore.deleteAudioFile(this.audioFileId.toString()); 
        if(deleteResult == "Success") {
          alert("Successfully deleted: ", this.title);
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