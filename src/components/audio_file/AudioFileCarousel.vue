<template>
    <div class="carousel-container">
        <div class="flex flex-row gap-2" v-for="(item, index) in audioStore.audioFiles" :key="index">
            <div>
                <SongCard 
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { audioStore } from '../../api/AudioFIle';
</script>

<script>
import SongCard from './SongCard.vue';
import { BaseDirectory, dataDir, appDataDir } from '@tauri-apps/api/path';
import { invoke } from "@tauri-apps/api/core";
import { audioStore } from '../../api/AudioFIle';

export default {
    components: {SongCard},
    name: 'AudioFileCarousel',
    async mounted() {
        console.log("Loading audio files from store"); 
        await audioStore.loadAudioFiles();
        console.log("AUDIO FILE STORE LOADED: ", audioStore.audioFiles);
    }
}

</script>


<style scoped>

.carousel-container {
    /* border: 1px solid #ccc; */
    display: flex;
    align-items: center;
    justify-content: center;
    overflow: auto;
    gap: 1rem;
    margin-right: 5%;
    margin-left: 5%;
}

</style>