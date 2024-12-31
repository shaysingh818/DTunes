<script setup lang="ts">
import NavigationBar from "./components/shared/NavigationBar.vue";
import AudioPlayer from "./components/shared/AudioPlayer.vue";
import { audioStore } from "./api/AudioFile";
// import { invoke } from "@tauri-apps/api/core";

// const greetMsg = ref("");
// const name = ref("");

// async function greet() {
//   // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
//   greetMsg.value = await invoke("greet", { name: name.value });
// }
</script>


<template>

   <div class="flex">
    <NavigationBar />
    <AudioPlayer
      v-if="(audioStore.audioFilePlaying && audioStore.audioFilePlaying.audio_file_id) || audioStore.playing == true"
      :audioFileId="audioStore.audioFilePlaying.audio_file_id"
      :title="audioStore.audioFilePlaying.file_name"
      :datePosted="audioStore.audioFilePlaying.date_created"
      :duration="audioStore.audioFilePlaying.duration"
      :filePath="audioStore.audioFilePlaying.file_path"
      :lastModified="audioStore.audioFilePlaying.last_modified"
      :plays="audioStore.audioFilePlaying.plays"
      :sampleRate="audioStore.audioFilePlaying.sample_rate"
      :thumbnail="audioStore.audioFilePlaying.thumbnail"
    /> 
  </div>

  <main class = "ml-32 p-2">
    <RouterView />
  </main>
</template>

<style>

</style>