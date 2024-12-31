
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
                                <h1>Edit Playlist</h1>
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
                        <input type="text" id="playlist-name" name="playlist-name" :placeholder="name"><br>
                    </div>

                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <!-- <button id="upload-btn" class="upload-btn">Upload Artist Image</button> -->
                            <input @click="selectThumbnailImage" type="button" id="playlist-thumbnail-upload" name="playlist-thumbnail-upload">
                            <label for="playlist-thumbnail-upload">Choose Image (Optional)</label>
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
</script>

<script>
import { ref } from 'vue'
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { open, save } from "@tauri-apps/plugin-dialog"
import { audioStore } from '../../api/AudioFile';
import { playlistStore } from '../../api/Playlist';


let openWindow = ref(false);

export default {
    name: 'EditPlaylistMetadata',
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
        playlistAudioFileSearch(){
            alert("Audio File Search Button Clicked");
        },
        async submitForm() {

            const playlistName = document.getElementById('playlist-name');
            const playlistThumbnailPath = document.getElementById('playlist-thumbnail-upload');

            console.log("PLAYLIST NAME: ", this.name);
            console.log("IMAGES PATH: ", this.thumbnail);

            const playlistNameValidation = playlistName.value.length > 0;
            const thumbnailValidation = playlistThumbnailPath.value.length > 0;

            let playlistNameSet = '';
            let thumbnailPath = '';

            if(!playlistNameValidation) { 
                playlistNameSet = this.name; 
            } else {
                playlistNameSet = playlistName.value;
            }

            if(!thumbnailValidation) { 
                thumbnailPath = this.thumbnail 
            } else {
                thumbnailPath = playlistThumbnailPath.value; 
            }

            console.log("PLAYLIST NAME: ", playlistNameSet);
            console.log("IMAGES PATH: ", thumbnailPath);

            const response = await playlistStore.editPlaylist(
                this.id.toString(),
                playlistNameSet,
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
            const playlistThumbnailPath = document.getElementById('playlist-thumbnail-upload');
            playlistThumbnailPath.value = selectImagePath; 
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


#playlist-thumbnail-upload {
  display: none;
}

</style>
