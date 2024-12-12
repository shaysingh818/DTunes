
<template>
    <button class="edit-button" @click="openWindow = true">
      <i :class="['fas', 'fa-edit', 'text-white-800']"></i>
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
                                <h1>Edit Genre</h1>
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
                        <input type="text" id="genre-name" name="genre-name" :placeholder="name"><br>
                    </div>

                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <!-- <button id="upload-btn" class="upload-btn">Upload Artist Image</button> -->
                            <input @click="selectThumbnailImage" type="button" id="genre-thumbnail-upload" name="genre-thumbnail-upload">
                            <label for="genre-thumbnail-upload">Choose Image (Optional)</label>
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
                        <button @click="submitForm" class="upload-button" type="button">Submit</button> 
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
import { genreStore } from '../../api/Genre';

let openWindow = ref(false);

</script>

<script>
import { ref } from 'vue'
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import { audioStore } from '../../api/AudioFile';
import { genreStore } from '../../api/Genre';


let openWindow = ref(false);

export default {
    name: 'EditGenreMetadata',
    props: {
        id: {
            type: Number,
            required: true,
        },
        name: {
            type: String,
            required: true,
        },
        thumbnail: {
            type: String,
            required: true,
        },
    },
    methods: {
        artistAudioFileSearch(){
            alert("Audio File Search Button Clicked");
        },
        async submitForm() {

            const genreName = document.getElementById('genre-name');
            const genreThumbnailPath = document.getElementById('genre-thumbnail-upload');

            console.log("GENRE NAME: ", this.name);
            console.log("IMAGES PATH: ", this.thumbnail);

            const genreNameValidation = genreName.value.length > 0;
            const thumbnailValidation = genreThumbnailPath.value.length > 0;

            let genreNameSet = '';
            let thumbnailPath = '';

            if(!genreNameValidation) { 
                genreNameSet = this.name; 
            } else {
                genreNameSet = genreName.value;
            }

            if(!thumbnailValidation) { 
                thumbnailPath = this.thumbnail 
            } else {
                thumbnailPath = genreThumbnailPath.value; 
            }

            console.log("GENRE NAME: ", genreNameSet);
            console.log("IMAGES PATH: ", thumbnailPath);

            const response = await genreStore.editGenre(
                this.id.toString(),
                genreNameSet,
                thumbnailPath
            );

            if(response == "Success") {
                console.log("Successfully Updated Genre");
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
            const genreThumbnailPath = document.getElementById('genre-thumbnail-upload');
            genreThumbnailPath.value = selectImagePath; 
        },
    }
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
