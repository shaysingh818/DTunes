<script>
import SearchComponent from '../components/shared/SearchComponent.vue';
import AudioFileListView from '../components/audio_file/AudioFileListView.vue';
import AudioFileCreate from '../components/audio_file/AudioFileCreate.vue';
import { useRoute } from 'vue-router';

export default {
  components: { SearchComponent, AudioFileListView, AudioFileCreate},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    audioFileSearch(){
      alert("Audio File Search Button Clicked");
    },
    goBack() {
      this.$router.go(-1); 
    }
  },
  setup() {
    const route  = useRoute();
    const id = route.params.id; 
    console.log("ID OF AUDIO FILE: ", id); 
  }
}
</script>

<template>

<div class="back-bar">

  <div class="flex">

    <div class="flex-none w-16">
      <div class="back-buttons"> 
        <div>
            <i @click="goBack()" :class="['fas', 'fa-arrow-left', 'text-red-800']" style="font-size: 25px;"></i>
        </div>
      </div>
    </div>

    <div class="grow h-14">
      <div class="page-header">
        <div class="flex flex-row gap-4">
          <div class="image-container">
          </div>
          <div class="text-content">
            <div class="flex flex-col">
              <div>
                <h1>Audio File Name</h1>
              </div>
              <div>
                <p>Date Posted</p>
              </div>
            </div>

          </div>
        </div>
      </div>
    </div>

    <div class="flex-none w-32">
      <div class="trailing-icons">
        <i :class="['fas', 'fa-question', 'text-red-800']" style="font-size: 20px;"></i>
      </div>
    </div>
  </div>
</div>


  <div class="artist-page-container">
    <div class="form-container">
      <div class="song-title">
        <div class="flex flex-col gap-1">
          <div style="width: 100%;">
            <p>Edit audio file name</p>
          </div>
          <div style="width: 100%;">
            <input type="text" id="audio-file-name" name="audio-file-name" placeholder="Enter Audio File Name"><br>
          </div>
        </div>
      </div>

      <div class="song-title">
        <div class="flex flex-col gap-1">
          <div style="width: 100%;">
            <p>Change existing image for audio file</p>
          </div>
          <div class="flex flex-row gap-1" style="width: 100%;">
            <div>
              <input type="text" id="audio-file-name" name="audio-file-name" placeholder="Enter Audio File Name"><br>
            </div>
            <div>
              <input type="text" id="audio-file-name" name="audio-file-name" placeholder="Enter Audio File Name"><br>
            </div>
          </div>
        </div>
      </div>

      <div class="song-title">
        <div class="flex flex-col gap-1">
          <div style="width: 100%;">
            <p>Upload new audio file</p>
          </div>
          <div style="width: 100%;">
            <input type="text" id="audio-file-name" name="audio-file-name" placeholder="Enter Audio File Name"><br>
          </div>
        </div>
      </div>


    </div>
  </div>
</template>

<style scoped>

.artist-page-container {
  display: flex;
  justify-content: center;
  align-items: center;
}


.grid-container {
    max-width: 100%; 
    height: 100%;
    margin: 0 auto;
}

.back-bar {
    width: 100%; 
    height: 60px;
    background-color: rgb(28 25 23);
    top: fixed;
    padding: 2px;
}


.back-buttons {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100%; 
}


.page-header {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 100%;
    height: 100%; 
}

.trailing-icons {
  height: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
}

.image-container {
  border-radius: 10%;
  height: 50px;
  width: 50px;
  background-image: url("https://www.w3schools.com/html/pic_trulli.jpg");
  background-size: cover;
  background-position: center; 
}

.form-container {
  display: flex;
  flex-direction: column;
  gap: 2.0rem; /* This is equivalent to gap-2 in Tailwind (usually 0.5rem or 8px depending on your config) */
  width: 100%;
  padding: 20px;
}



input {
    border-radius: 3%;
    height: 50px;
    width: 100%;
    padding: 8px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}


p {
    font-size: 12px;
    color: rgb(209 213 219);
}


h1 {
    color: white;
    font-weight: bold;
}

</style>