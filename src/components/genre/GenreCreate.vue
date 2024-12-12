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
                                <h1>Create Genre</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6">
                    <div>
                        <input type="text" id="genre-name" name="genre-name" placeholder="Enter Genre Name"><br>
                    </div>
                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <input @click="selectThumbnailImage" type="button" id="genre-thumbnail-upload" name="filename">
                            <label for="genre-thumbnail-upload">Choose Genre Image</label>
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
import { genreStore } from '../../api/Genre';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"

const openWindow = ref(false)


async function submitForm() {

  const genreName = document.getElementById('genre-name');
  const genreThumbnailPath = document.getElementById('genre-thumbnail-upload');

  console.log("GENRE NAME: ", genreName.value);
  console.log("IMAGES PATH: ", genreThumbnailPath.value);

  const genreNameValidation = genreName.value.length > 0;
  const thumbnailValidation = genreThumbnailPath.value.length > 0;

  if(genreNameValidation && thumbnailValidation) {

    const response = await genreStore.createGenre(
      genreName.value,
      genreThumbnailPath.value
    );

    if(response == "Success") {
      console.log("INSERT SUCCESSFUL");
      alert("Success");
    } else {
      console.log("SOMETHING WENT WRONG");
    }
  } else if(!genreNameValidation) {
    alert("Must provide name for genre");
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
  const genreThumbnailPath = document.getElementById('genre-thumbnail-upload');
  genreThumbnailPath.value = selectImagePath; 
}

</script>

<script>

export default {
    name: 'GenreCreate'
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

#genre-thumbnail-upload {
  display: none;
}

</style>