
<template>
    <button class="add-button" @click="openWindow = true">
      <i :class="['fas', 'fa-add', 'text-white-800']"></i>
    </button>
    <Teleport to="body">
      <div v-if="openWindow" class="modal">
        <div class="modal-container">
          <div class="flex flex-col gap-2">
              <div>
                <div class="form-header">
                    <div class="flex flex-row gap-10">
                        <div>
                            <div class="form-title">
                                <h1>Upload Audio File</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6 gap-y-8">
                    <div>
                        <!-- <label for="lname">Artist Name</label> -->
                        <input type="text" id="audio-file-name" name="audio-file-name" placeholder="Enter Audio File Name"><br>
                    </div>

                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <!-- <button id="upload-btn" class="upload-btn">Upload Artist Image</button> -->
                            <input @click="selectThumbnailImage" type="button" id="audio-thumbnail-upload" name="audio-thumbnail-upload">
                            <label for="audio-thumbnail-upload">Choose Image (Optional)</label>
                        </form>
                    </div>

                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <!-- <button id="upload-btn" class="upload-btn">Upload Artist Image</button> -->
                            <input @click="selectAudioFileUpload" type="button" id="audio-file-upload" name="audio-file-upload">
                            <label for="audio-file-upload">Choose Audio File</label>
                        </form>
                    </div>

                </div>
              </div>
              <br>
              <div class="form-button-container">
                <div class="flex flex-row gap-2">
                    <div>
                        <button @click="openWindow = false" class="close-button" type="button">Close</button> 
                    </div>
                    <div>
                        <button @click="submitForm()" class="upload-button" type="button">Upload</button> 
                    </div>
                </div>
              </div>
          </div>
        </div>

      </div>
  </Teleport>
</template>

<script setup>
import { ref } from 'vue'
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import { audioStore } from '../../api/AudioFile';

let openWindow = ref(false);

/* grab form values  */
async function submitForm() {

  const audioFileName = document.getElementById('audio-file-name');
  const audioFileThumbnailPath = document.getElementById('audio-thumbnail-upload');
  const audioFilePath = document.getElementById('audio-file-upload');

  console.log("FILE NAME: ", audioFileName.value);
  console.log("IMAGES PATH: ", audioFileThumbnailPath.value);
  console.log("AUDIO PATH ", audioFilePath.value);

  const fileNameValidation = audioFileName.value.length > 0;
  const thumbnailValidation = audioFileThumbnailPath.value.length > 0;
  const filePathValidation = audioFilePath.value.length > 0;

  if(fileNameValidation && thumbnailValidation && filePathValidation) {

    const response = await audioStore.createAudioFile(
      audioFileName.value,
      audioFilePath.value,
      audioFileThumbnailPath.value
    );

    if(response == "Success") {
      console.log("INSERT SUCCESSFUL");
      alert("Success");
    } else {
      console.log("SOMETHING WENT WRONG");
    }
  } else if(!fileNameValidation) {
    alert("Must provide file name alias");
  } else if(!thumbnailValidation) {
    alert("Thumbnail image cannot be empty"); 
  } else if(!filePathValidation) {
    alert("Must provide path to audio file to upload"); 
  }


}

async function selectThumbnailImage() {
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
}


async function selectAudioFileUpload() {
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




</script>

<script>

export default {
    name: 'AudioFileCreate'
}

</script>

<style scoped>

h1 {
  color: rgb(153 27 27);
  font-weight: bold;
  font-size: 30px;
  padding: 4px;  
}

.upload-btn:hover {
  background-color: #0056b3;
}


</style>

