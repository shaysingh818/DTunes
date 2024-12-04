
<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router';
import { audioStore, AudioFile } from '../api/AudioFile';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import SearchComponent from '../components/shared/SearchComponent.vue';
import AudioFileListView from '../components/audio_file/AudioFileListView.vue';
import AudioFileCreate from '../components/audio_file/AudioFileCreate.vue';


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
    },
    async submitForm() {

      const audioFileName = document.getElementById('audio-file-name');
      const audioFileThumbnailPath = document.getElementById('audio-thumbnail-upload');
      const audioFilePath = document.getElementById('audio-file-upload');

      const fileNameValidation = audioFileName.value.length > 0;
      const thumbnailValidation = audioFileThumbnailPath.value.length > 0;
      const filePathValidation = audioFilePath.value.length > 0;

      let fileName = '';
      let thumbnailPath = '';
      let filePath = '';

      if(!fileNameValidation) { 
        fileName = this.audioFile.file_name; 
      } else {
        fileName = audioFileName.value;
      }

      if(!thumbnailValidation) { 
        thumbnailPath = this.audioFile.thumbnail 
      } else {
        thumbnailPath = audioFileThumbnailPath.value; 
      }

      if(!filePathValidation) { 
        filePath = this.audioFile.file_path 
      } else {
        filePath = audioFilePath.value;
      }

      console.log("FILE NAME: ", fileName);
      console.log("IMAGES PATH: ", thumbnailPath);
      console.log("AUDIO PATH ", filePath);

      const response = await audioStore.editAudioFile(
        this.audioFile.audio_file_id.toString(),
        fileName,
        filePath,
        thumbnailPath
      );

      if(response == "Success") {
        console.log("Successfully Updated Audio File");
        alert("Success");
      } else {
        console.log("SOMETHING WENT WRONG");
        alert(response);
      }

    },
    async selectThumbnailImage() {
      const selectImagePath = await open({
        multiple: false,
        filters: [
          {
            name: 'Image Filter',
            extensions: ['png', 'jpeg', 'jpg', 'webp']
          }
        ]
      });
      const audioFileThumbnailPath = document.getElementById('audio-thumbnail-upload');
      audioFileThumbnailPath.value = selectImagePath; 
    },
    async selectAudioFileUpload() {
      const selectAudioFilePath = await open({
        multiple: false,
        filters: [
          {
            name: 'Audio Format Filter',
            extensions: ['mp3', 'wav', 'aac', 'ogg', 'wma', 'flac']
          }
        ]
      });
      const audioFilePath = document.getElementById('audio-file-upload');
      audioFilePath.value = selectAudioFilePath; 
    }
  },
  data() {
    return {
      audioFile: AudioFile
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    console.log("ID OF AUDIO FILE: ", id);
    const audioFile = await audioStore.viewAudioFile(id); 
    this.audioFile = audioFile; 
    console.log(audioFile); 
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
                <h1>{{ audioFile.file_name }}</h1>
              </div>
              <div>
                <p>{{ audioFile.date_created }}</p>
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


  <div class="page-container">
    <div class="form-container">
      <div class="song-title">
        <div class="flex flex-col gap-1">
          <div style="width: 100%;">
            <p>Edit audio file name</p>
          </div>
          <div style="width: 100%;">
            <input type="text" id="audio-file-name" name="audio-file-name" :placeholder="this.audioFile.file_name"><br>
          </div>
        </div>
      </div>

      <div class="thumbnail-edit">
        <div class="flex flex-col">
          <div style="width: 100%;">
            <p>Change existing image for audio file</p>
          </div>
          <div class="file-form-section">
            <div>
              <input @click="selectThumbnailImage" type="button" id="audio-thumbnail-upload" name="audio-thumbnail-upload">
              <label for="audio-thumbnail-upload">Replace Thumbnail</label>
            </div>
            <div>
              <input type="text" id="audio-file-name" name="audio-file-name" :value="audioFile.thumbnail" readonly><br>
            </div>
          </div>

        </div>
      </div>

      <div class="thumbnail-edit">
        <div class="flex flex-col">
          <div style="width: 100%;">
            <p>Edit audio file storage location</p>
          </div>
          <div class="file-form-section">
            <div>
              <input @click="selectAudioFileUpload" type="button" id="audio-file-upload" name="audio-file-upload">
              <label for="audio-file-upload">Upload Audio File</label>
            </div>
            <div>
              <input type="text" id="audio-file-name" name="audio-file-name" :value="audioFile.file_path" readonly><br>
            </div>
          </div>
        </div>
      </div>


      <div style="width: 100%;">
        <p>Submit and save final changes</p>
      </div>

      <div class="form-submit">
        <div>
            <button @click="goBack()" class="close-button" type="button">Close</button> 
        </div>
        <div>
            <button @click="submitForm" class="upload-button" type="button">Submit</button> 
        </div>
      </div>



    </div>
  </div>
</template>

<style scoped>

.close-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
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


/* file button upload styling */ 
label {
  background-color: rgb(28 25 23);
  color: rgb(153 27 27);
  padding: 16px 24px;
  border-radius: 5px;
  font-size: 14px;
  cursor: pointer;
  font-weight: bold;
  /* border: 1px solid white; */
}


p {
    font-size: 12px;
    color: rgb(209 213 219);
}


h1 {
    color: white;
    font-weight: bold;
}


#audio-thumbnail-upload {
  display: none;
}


#audio-file-upload {
  display: none;
}

</style>