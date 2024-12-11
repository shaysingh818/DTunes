<template>
    <div>
        <div class="flex flex-col" v-for="(item, index) in audioStore.audioFiles" :key="index">
            <div>
                <PlaylistAudioFile
                    :playlistId="playlistId"
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :lastModified="item.last_modified"
                    :plays="item.plays"
                    :sampleRate="item.sample_rate"
                    :playlistRemove=false
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { audioStore } from '../../api/AudioFile';
</script>

<script>
import { audioStore } from '../../api/AudioFile';
import PlaylistAudioFile from './PlaylistAudioFile.vue';

export default {
  components: { PlaylistAudioFile},
   props: {
    playlistId: {
      type: Number,
      required: true,
    },
  },
  async mounted() {
    console.log(`Loading audio files from store ${this.playlistId}`); 
    await audioStore.loadAudioFiles();
    console.log("AUDIO FILE STORE LOADED: ", audioStore.audioFiles); 
  },    
}
</script>