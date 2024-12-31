<template>
    <div class="carousel-container">
        <div class="flex flex-row gap-2" v-for="(item, index) in pomodoroStore.audioFiles" :key="index">
            <div>
                <PomodoroAudioFileCard
                    :sessionId="sessionId" 
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :pomodoroRemove=true
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { audioStore } from '../../api/AudioFile';
</script>

<script>
import PomodoroAudioFileCard from './PomodoroAudioFileCard.vue';
import { BaseDirectory, dataDir, appDataDir } from '@tauri-apps/api/path';
import { invoke } from "@tauri-apps/api/core";
import { audioStore } from '../../api/AudioFile';
import { pomodoroStore } from '../../api/Pomodoro';

export default {
    components: { PomodoroAudioFileCard },
    name: 'PomodoroAudioFileCarousel',
    props: {
        sessionId: {
            type: Number,
            required: true,
        },
    },
    async mounted() {
        console.log("Loading audio files from store"); 
        await pomodoroStore.viewPomodoroAudioFiles(this.sessionId.toString());
        console.log("AUDIO FILE STORE LOADED: ", pomodoroStore.audioFiles);
    }
}
</script>