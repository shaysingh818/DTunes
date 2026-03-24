<script setup lang="ts">
import NavigationBar from "./components/shared/NavigationBar.vue";
import AudioPlayer from "./components/shared/AudioPlayer.vue";
import { audioStore } from "./api/AudioFile";
import { audioQueueStore } from './api/AudioQueue'; 

</script>



<template>

   <div class="flex">
    <NavigationBar />
    <AudioPlayer
      v-if="audioQueueStore.active == true"
      :audioFileId="audioQueueStore.currAudioFile.audio_file_id"
      :title="audioQueueStore.currAudioFile.file_name"
      :datePosted="audioQueueStore.currAudioFile.date_created"
      :duration="audioQueueStore.currAudioFile.duration"
      :filePath="audioQueueStore.currAudioFile.file_path"
      :lastModified="audioQueueStore.currAudioFile.last_modified"
      :plays="audioQueueStore.currAudioFile.plays"
      :sampleRate="audioQueueStore.currAudioFile.sample_rate"
      :thumbnail="audioQueueStore.currAudioFile.thumbnail"
    /> 
  </div>

  <main class = "ml-32 p-2">
    <RouterView />
  </main>
</template>

<style>

</style>
