
<template>
    <div class="audio-files-container">
        <div class="flex flex-col" v-for="(item, index) in audioStore.audioFiles" :key="index">
            <div>
                <SongList
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :lastModified="item.last_modified"
                    :plays="item.plays"
                    :sampleRate="item.sample_rate"
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { audioStore } from '../../api/AudioFile';
</script>

<script>

import SongList from './SongList.vue';
import { audioStore } from '../../api/AudioFile';

export default {
  components: { SongList},
  async mounted() {
    console.log("Loading audio files from store"); 
    await audioStore.loadAudioFiles();
    console.log("AUDIO FILE STORE LOADED: ", audioStore.audioFiles); 
  },    
}
</script>