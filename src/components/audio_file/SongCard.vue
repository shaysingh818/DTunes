
<template>
    <div class="hover:bg-stone-900">
        <div class="card-container" :id="audioFileId.toString()">
          <div class="card-container-content">
            <div class="text-content">
              <h1>{{ title }}</h1>
              <p>{{ datePosted }}</p>
            </div>
            <div class="icon-content">
              <div class="grid grid-flow-col auto-cols-max space-x-4">
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-list-ul', 'text-white']"></i></div>
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-play', 'text-white']"></i></div>
                <div><p>{{ duration }}</p></div>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script>
import { invoke } from "@tauri-apps/api/core";

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
  async mounted() {

    const base64Image = await invoke('read_image_from_data_dir', {imageName:  this.thumbnail});
    let imageElem = document.getElementById(this.audioFileId.toString());
    if(imageElem) {
      imageElem.style.backgroundImage = `url(data:image/jpeg;base64,${base64Image})`; 
    } else {
      console.log(`${this.audioFileId} not found`)
    }

  },
}
</script>


<style scoped>

.card-container {
    border-radius: 3%;
    display: flex;
    height: 200px;
    width: 200px;
    align-items: flex-end;
    background-size: cover;
    background-position: center; 
}

.card-container-content {
    border-radius: 3%;
    width: 100%;
    height: 100px;
    background: rgb(0, 0, 0); /* Fallback color */
    background: rgba(0, 0, 0, 0.5); /* Black background with 0.5 opacity */
}

.text-content {
    /* border: 1px solid #ccc;   */
    width: 100%;
    position: relative;
    overflow: hidden;
    white-space: nowrap;
    padding: 4px; 
}

.icon-content {
    padding: 8px; 
}


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