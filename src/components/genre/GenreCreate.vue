<template>
    <button class="add-button" @click="open = true">
      <i :class="['fas', 'fa-add', 'text-white-800']"></i>
    </button>
    <Teleport to="body">
      <div v-if="open" class="modal">
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
                        <input type="text" id="search-artist" name="search-artist" placeholder="Enter Genre Name"><br>
                    </div>
                    <div style="justify-content: center; display:flex ; align-items: center;">
                        <form action="/action_page.php">
                            <input type="file" id="file-upload" name="filename">
                            <label for="file-upload">Choose Genre Image</label>
                        </form>
                    </div>
                </div>
              </div>
              <br>
              <div class="form-button-container">
                <div class="flex flex-row gap-2">
                    <div>
                        <button @click="open = false" class="close-button" type="button">Close</button> 
                    </div>
                    <div>
                        <button class="upload-button" type="button">Create</button> 
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

const open = ref(false)
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

#file-upload {
  display: none;
}

</style>