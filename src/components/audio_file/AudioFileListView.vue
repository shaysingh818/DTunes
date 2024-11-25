
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
import { audioStore } from '../../api/AudioFIle';
</script>

<script>

import SongList from './SongList.vue';
import { BaseDirectory, dataDir, appDataDir } from '@tauri-apps/api/path';
import { invoke } from "@tauri-apps/api/core";
import { open, exists } from '@tauri-apps/plugin-fs';
import { convertFileSrc } from '@tauri-apps/api/core';
import { audioStore } from '../../api/AudioFIle';

export default {
  components: { SongList},
  async mounted() {
    console.log("Loading audio files from store"); 
    await audioStore.loadAudioFiles();
    console.log("AUDIO FILE STORE LOADED: ", audioStore.audioFiles); 
  }
}
</script>

<style scoped>

.audio-files-container {
  margin-bottom: 75px;
}
</style>