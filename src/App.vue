<script setup lang="ts">
import NavigationBar from "./components/shared/NavigationBar.vue";
import AudioPlayer from "./components/shared/AudioPlayer.vue";
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
      title="Pirates of the Carribean" 
      datePosted="Monday, November 4, 2024" 
      duration="3:01"
      thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
    /> 
  </div>

  <main class = "ml-32 p-2">
    <RouterView />
  </main>
</template>

<style>

</style>