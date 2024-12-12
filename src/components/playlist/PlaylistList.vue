
<template>
    <div class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="list-view-image-container">
                <img :src="thumbnail" :alt="alt" :id="`playlist-image-${playlistId.toString()}`" class="list-view-image p-1" />
            </div>
        </div>
        <div class="flex-auto w-64">
            <div class="list-view-text-container">
              <h2>{{ title }}</h2>
              <p>{{ datePosted }}</p>
            </div>
        </div>
        <div class="flex-auto w-32">
          <div class="list-view-trailing-container">
            <div class="grid grid-flow-col auto-cols-max space-x-4">

              <div @click="addAudioFile()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-add', 'text-red-800']"></i>
              </div>
              <div @click="editPlaylist()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-edit', 'text-red-800']"></i>
              </div>
              <div @click="deletePlaylist()" class="hover:bg-stone-400">
                <i :class="['fas', 'fa-trash', 'text-red-800']"></i>
              </div>

            </div>
          </div>
        </div>
    </div>
</template>



<script>
import { invoke } from "@tauri-apps/api/core";
import { playlistStore, Playlist } from "../../api/Playlist";
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'PlaylistList',
  props: {
    playlistId: {
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
    lastModified: {
      type: String,
      required: true,
    }, 
    thumbnail: {
      type: String,
      required: true,
    },
  },
  methods: {
    async deletePlaylist() {
      const userChoice = await window.confirm(`Are you sure you want to delete: ${this.title}`);
      if(userChoice) {
        const deleteResult = await playlistStore.deletePlaylist(this.playlistId.toString()); 
        if(deleteResult == "Success") {
          alert("Successfully deleted: ", this.title);
          this.$router.push('playlists/');
          await this.$nextTick(); 
          await playlistStore.loadPlaylists(); 
        } 
      } else {
        alert("Could not delete playlist: ", deleteResult);  
      }
    },

    async addAudioFile() {
      this.$router.push({ path: `/playlists/add-audio-file/${this.playlistId}`})
    },
    async editPlaylist() {
      this.$router.push({ path: `/playlists/edit/${this.playlistId}`})
    }
  },
  async mounted() {

    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });

    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL PLAYLIST: ", imageUrl)

    let imageElem = document.getElementById(`playlist-image-${this.playlistId.toString()}`);
    if(imageElem) {
      imageElem.src = imageUrl;
    } else {
      console.log(`${this.playlistId} not found`)
    }

  }
}
</script>

<style scoped>

h2 {
    color: white;
    font-weight: bold;
}

p {
    font-size: 12px;
    color: rgb(209 213 219);
}

p1 {
    font-size: 14px;
    color: rgb(209 213 219);
}


hr {
  border: 1px solid gray 
}



</style>