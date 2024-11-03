<script setup lang="ts">
import { ref } from "vue";
import NavigationItem from "./components/NavigationItem.vue";
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
    <!-- Sidebar Navigation -->
    <aside class="fixed inset-y-0 left-0 w-32 bg-stone-900 text-gray-200 p-2">
      <nav class="flex flex-col space-y-3">

        <RouterLink to="/" class="block p-2">
          <NavigationItem icon="fa-home" nav-title="home"/>
        </RouterLink>

        <RouterLink to="/playlists" class="block p-2 content-center">
          <NavigationItem icon="fa-list" nav-title="playlists" />
        </RouterLink>

        <RouterLink to="/artists" class="block p-2">
          <NavigationItem icon="fa-user" nav-title="artists" />
        </RouterLink>

        <RouterLink to="/genres" class="block p-2">
          <NavigationItem icon="fa-layer-group" nav-title="genres" />
        </RouterLink>

        <RouterLink to="/audio-files" class="block p-2">
          <NavigationItem icon="fa-music" nav-title="audio files" />
        </RouterLink>

        <RouterLink to="/pomodoro" class="block p-2"> 
          <NavigationItem icon="fa-clock" nav-title="pomodoro" />
        </RouterLink>
      </nav>
    </aside>
  </div>

  <main class = "ml-32 p-2">
    <RouterView />
  </main>
</template>

<style>

</style>