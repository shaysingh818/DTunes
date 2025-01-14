
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
                                <h1>Add Artist</h1>
                            </div>
                        </div>
                    </div>
                </div>
              </div>

              <br> 
              <div class="form-field-container">
                <div class="flex flex-col gap-6">
                    <div>
                        <!-- <label for="lname">Artist Name</label> -->
                        <input type="text" id="artist-name" name="artist-name" placeholder="Enter Artist Name"><br>
                    </div>
                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <!-- <button id="upload-btn" class="upload-btn">Upload Artist Image</button> -->
                            <input @click="selectThumbnailImage()" type="button" id="artist-thumbnail-upload" name="artist-thumbnail-upload">
                            <label for="artist-thumbnail-upload">Choose Artist Image</label>
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
                        <button @click="submitForm()" class="upload-button" type="button">Add Artist</button> 
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
import { artistStore } from '../../api/Artist';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"

const openWindow = ref(false);

</script>

<script>

export default {
    name: 'ArtistCreate',
    methods: {

      async submitForm() {

        const artistName = document.getElementById('artist-name');
        const artistThumbnailPath = document.getElementById('artist-thumbnail-upload');

        console.log("ARTIST NAME: ", artistName.value);
        console.log("IMAGES PATH: ", artistThumbnailPath.value);

        const artistNameValidation = artistName.value.length > 0;
        const thumbnailValidation = artistThumbnailPath.value.length > 0;

        if(artistNameValidation && thumbnailValidation) {

          const response = await artistStore.createArtist(
            artistName.value,
            artistThumbnailPath.value
          );

          if(response == "Success") {
            console.log("INSERT SUCCESSFUL");
            alert("Success");
            this.$router.push('artists/');
            await this.$nextTick(); 
            await artistStore.loadArtists(); 
          } else {
            console.log("SOMETHING WENT WRONG");
          }
        } else if(!artistNameValidation) {
          alert("Must provide name for artist");
        } else if(!thumbnailValidation) {
          alert("Thumbnail image cannot be empty");
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
        const artistThumbnailPath = document.getElementById('artist-thumbnail-upload');
        artistThumbnailPath.value = selectImagePath; 
      }

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

#artist-thumbnail-upload {
  display: none;
}

</style>

