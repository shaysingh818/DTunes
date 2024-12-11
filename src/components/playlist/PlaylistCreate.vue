
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
                                <h1>Create Playlist</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6">
                    <div>
                        <input type="text" id="playlist-name" name="playlist-name" placeholder="Enter Playlist Name"><br>
                    </div>
                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <input @click="selectThumbnailImage" type="button" id="playlist-thumbnail-upload" name="filename">
                            <label for="playlist-thumbnail-upload">Upload Image</label>
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
                        <button @click="submitForm" class="upload-button" type="button">Create</button> 
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
import { playlistStore } from '../../api/Playlist';

let openWindow = ref(false);

/* grab form values  */
async function submitForm() {

  const playlistName = document.getElementById('playlist-name');
  const playlistThumbnailPath = document.getElementById('playlist-thumbnail-upload');

  console.log("PLAYLIST NAME: ", playlistName.value);
  console.log("IMAGES PATH: ", playlistThumbnailPath.value);

  const playlistNameValidation = playlistName.value.length > 0;
  const thumbnailValidation = playlistThumbnailPath.value.length > 0;

  if(playlistNameValidation && thumbnailValidation) {

    const response = await playlistStore.createPlaylist(
      playlistName.value,
      playlistThumbnailPath.value
    );

    if(response == "Success") {
      console.log("INSERT SUCCESSFUL");
      alert("Success");
    } else {
      console.log("SOMETHING WENT WRONG");
    }
  } else if(!playlistNameValidation) {
    alert("Must provide name for playlist");
  } else if(!thumbnailValidation) {
    alert("Thumbnail image cannot be empty");
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
  const audioFileThumbnailPath = document.getElementById('playlist-thumbnail-upload');
  audioFileThumbnailPath.value = selectImagePath; 
}
</script>


<script>

export default {
    name: 'PlaylistCreate'
}
</script>

<style scoped>

h1 {
  color: rgb(153 27 27);
  font-weight: bold;
  font-size: 30px;
  padding: 4px;  
}

input {
    border-radius: 3%;
    height: 50px;
    width: 300px;
    padding: 8px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}

input[type="file"] {
    border-radius: 3%;
    height: 50px;
    width: 300px;
    padding: 8px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}

.form-button {
    border-radius: 5%;
    width: 50px; 
    color: white;
    padding: 8px;
    border: 1px solid #ccc;
    align-items: center;
    justify-content: center;
}


.close-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(41 37 36);
    color: rgb(153 27 27);
}

/* file button upload styling */ 
label {
  background-color: rgb(41 37 36);
  color: rgb(153 27 27);
  padding: 12px 24px;
  border-radius: 5px;
  font-size: 14px;
  cursor: pointer;
  font-weight: bold;
}

.upload-btn:hover {
  background-color: #0056b3;
}

#playlist-thumbnail-upload {
  display: none;
}

</style>

