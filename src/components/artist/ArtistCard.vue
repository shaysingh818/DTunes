

<template>
    <div class="card-component">
        <div class="card-container" :id="`artist-card-${artistId}`">
          <div class="card-container-content">
            <div class="text-content">
              <div class="flex flex-col gap-2">
                <div>
                  <h1>{{ name }}</h1>
                </div>              
                <div class="flex flex-row gap-3" style="display: flex; align-items: center; justify-content: center;">
                  <div class="hover:bg-stone-400">
                    <i @click="addAudioFile" :class="['fas', 'fa-add', 'text-white']"></i> 
                  </div>
                  <div @click="editArtist" class="hover:bg-stone-400">
                    <i :class="['fas', 'fa-edit', 'text-white']"></i> 
                  </div>
                  <div @click="removeArtist" class="hover:bg-stone-400">
                    <i :class="['fas', 'fa-trash', 'text-white']"></i> 
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script setup>
import { artistStore } from '../../api/Artist';
</script>

<script>
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { invoke } from "@tauri-apps/api/core";
import { artistStore } from '../../api/Artist';

export default {
  name: 'ArtistCard',
  props: {
    artistId: {
      type: Number,
      required: true,
    },
    name: {
      type: String,
      required: true,
    },
    datePosted: {
      type: String,
      required: true,
    },
    thumbnail: {
      type: String,
      required: true,
    },
    songCount: {
      type: String,
      required: true,
    },
  },
  methods: {
    async removeArtist() {
      const userChoice = await window.confirm(`Are you sure you want to delete: ${this.title}`);
      if(userChoice) {
        const deleteResult = await artistStore.deleteArtist(this.artistId.toString()); 
        if(deleteResult == "Success") {
          alert("Successfully deleted: ", this.title);
          this.$router.push('artists/');
          await this.$nextTick(); 
          await artistStore.loadArtists(); 
        } 
      } else {
        alert("Could not delete artist: ", deleteResult);  
      }
    },
    async editArtist() {
      this.$router.push({ path: `/artist/edit/${this.artistId}`})
    },
    async addAudioFile() {
      this.$router.push({ path: `/artist/add-audio-file/${this.artistId}`});
    }
  },
  async mounted() {

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });

    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL ARTIST: ", imageUrl)

    let imageElem = document.getElementById(`artist-card-${this.artistId}`);
    if(imageElem) {
      imageElem.style.backgroundImage = `url(${imageUrl})`;
    } else {
      console.log(`${this.artistId} not found ARTIST`)
    }

  }
}
</script>


<style scoped>

.card-component {
  width: 185px;
}

.card-container {
    border-radius: 3%;
    display: flex;
    height: 185px;
    width: 185px;
    background-size: cover;
    background-position: center;
}

.card-container-content {
    border-radius: 3%;
    width: 100%;
    height: 100%;
    background: rgb(0, 0, 0); /* Fallback color */
    background: rgba(0, 0, 0, 0.5); /* Black background with 0.5 opacity */
}

.text-content {
    display: flex;
    height: 100%;
    justify-content: center;
    align-items: center;
    white-space: nowrap; 
    text-overflow: ellipsis;
    overflow: hidden; 
}

.icon-content {
    padding: 8px; 
}

h1 {
  color: white;
  font-weight: bold; 
}

p {
  font-size: 12px;
  color: rgb(209 213 219);
}

</style>