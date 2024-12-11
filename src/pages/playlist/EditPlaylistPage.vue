<template>

  <BackBar 
    v-if="playlist && playlist.playlist_id"
    :itemId="playlist.playlist_id" 
    :title="playlist.playlist_name"
    :thumbnail="playlist.playlist_thumbnail"
    :dateCreated="playlist.date_created"
  />
  <div class="page-container">
    <div class="flex flex-col gap-2">
      <div class="search-box-container">
        <SearchComponent 
          text="Search Audio Files To Add To Playlist" 
          :onClick="audioFileSearch"
        />
      </div>
      <div>
        <PlaylistAudioFileListView 
          v-if="playlist && playlist.playlist_id"
          :id="playlist.playlist_id"
        />
      </div>
    </div>
  </div>
<EditPlaylistMetadata
  v-if="playlist"
  :id="playlist.playlist_id"
  :name="playlist.playlist_name"
  :thumbnail="playlist.playlist_thumbnail"
 /> 
</template>

<script>
import { ref } from 'vue'
import { useRoute } from 'vue-router';
import { playlistStore, Playlist } from '../../api/Playlist';
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { open, save } from "@tauri-apps/plugin-dialog"
import SearchComponent from '../../components/shared/SearchComponent.vue';
import PlaylistListView from '../../components/playlist/PlaylistListView.vue';
import PlaylistCreate from '../../components/playlist/PlaylistCreate.vue';
import PlaylistAudioFileListView from '../../components/playlist/PlaylistAudioFileListView.vue';
import BackBar from '../../components/shared/BackBar.vue';
import EditPlaylistMetadata from './EditPlaylistMetadata.vue';

export default {
  components: { 
    SearchComponent, 
    PlaylistListView, 
    PlaylistCreate, 
    PlaylistAudioFileListView,
    EditPlaylistMetadata,
    BackBar
  },
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
    audioFileSearch(){
      alert("Audio File Search Button Clicked");
    },
    goBack() {
      this.$router.go(-1); 
    },
    async submitForm() {

      const playlistName = document.getElementById('playlist-name');
      const playlistThumbnailPath = document.getElementById('playlist-thumbnail-upload');

      console.log("PLAYLIST NAME: ", playlistName.value);
      console.log("IMAGES PATH: ", playlistThumbnailPath.value);

      const playlistNameValidation = playlistName.value.length > 0;
      const thumbnailValidation = playlistThumbnailPath.value.length > 0;

      let playlistNameSet = '';
      let thumbnailPath = '';

      if(!playlistNameValidation) { 
        playlistNameSet = this.playlist.playlist_name; 
      } else {
        playlistNameSet = playlistName.value;
      }

      if(!thumbnailValidation) { 
        thumbnailPath = this.playlist.playlist_thumbnail 
      } else {
        thumbnailPath = playlistThumbnailPath.value; 
      }

      console.log("PLAYLIST NAME: ", playlistNameSet);
      console.log("IMAGES PATH: ", thumbnailPath);

      const response = await playlistStore.editPlaylist(
        this.playlist.playlist_id.toString(),
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
  },
  data() {
    return {
      playlist: {
        playlistId: 0,
        playlistName: '',
        playlistThumbnail: '',
        dateCreated: '',
        lastModified: ''
      }
    }
  },
  async mounted() {
    const route  = useRoute();
    const id = route.params.id; 
    this.playlist = await playlistStore.viewPlaylist(id);
    console.log(`MY PLAYLIST ${this.playlist.playlist_thumbnail}`);  

    // const fileBuffer = await readFile(`dtunes-audio-app/images/${playlist.playlistThumbnail}`, {
    //     baseDir: BaseDirectory.Data,
    // });
    // const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    // console.log("ELEMENT ID ", this.playlist.playlistId); 

    // let imageElem = document.getElementById(`${playlist.playlistId}`);
    // if(imageElem) {
    //   imageElem.style.backgroundImage = `url(${imageUrl})`; 
    // } else {
    //   console.log(`${this.playlist.playlistId} not found`)
    // }

  }
}
</script>

<style scoped>

.close-button {
    border-radius: 5%;
    width: 100px; 
    height:40px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
}

input {
    border-radius: 3%;
    height: 50px;
    width: 100%;
    padding: 8px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}


/* file button upload styling */ 
label {
  background-color: rgb(28 25 23);
  color: rgb(153 27 27);
  padding: 16px 24px;
  border-radius: 5px;
  font-size: 14px;
  cursor: pointer;
  font-weight: bold;
  /* border: 1px solid white; */
}


p {
    font-size: 12px;
    color: rgb(209 213 219);
}


h1 {
    color: white;
    font-weight: bold;
}


#playlist-thumbnail-upload {
  display: none;
}



</style>