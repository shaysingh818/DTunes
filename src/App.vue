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
      v-if="(audioStore.audioFilePlaying && audioStore.audioFilePlaying.audioFileId) || audioStore.playing == true"
      :audioFileId="audioStore.audioFilePlaying.audioFileId"
      :title="audioStore.audioFilePlaying.fileName"
      :datePosted="audioStore.audioFilePlaying.dateCreated"
      :duration="audioStore.audioFilePlaying.duration"
      :filePath="audioStore.audioFilePlaying.filePath"
      :lastModified="audioStore.audioFilePlaying.lastModified"
      :plays="audioStore.audioFilePlaying.plays"
      :sampleRate="audioStore.audioFilePlaying.sampleRate"
      :thumbnail="audioStore.audioFilePlaying.thumbnail"
    /> 
  </div>

  <main class = "ml-32 p-2">
    <RouterView />
  </main>
</template>

<style>

</style>