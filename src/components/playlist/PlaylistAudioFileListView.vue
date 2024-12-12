<template>
    <div>
        <div class="flex flex-col" v-for="(item, index) in playlistStore.audioFiles" :key="index">
            <div>
                <PlaylistAudioFile
                    :playlistId="id"
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :lastModified="item.last_modified"
                    :plays="item.plays"
                    :sampleRate="item.sample_rate"
                    :playlistRemove=true
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { playlistStore } from '../../api/Playlist';
</script>

<script>
import { playlistStore } from '../../api/Playlist';
import PlaylistAudioFile from './PlaylistAudioFile.vue';

export default {
  components: { PlaylistAudioFile},
   props: {
    id: {
      type: Number,
      required: true,
    },
  },
  data() {
    return {
      audioFiles: []
    }
  },
  async mounted() {
    console.log("PLAYLIST ID", this.id)
    await playlistStore.viewPlaylistAudioFiles(this.id.toString());
    console.log("PLAYLIST AUDIO FILE STORE LOADED: ", playlistStore.audioFiles); 
  },    
}
</script>