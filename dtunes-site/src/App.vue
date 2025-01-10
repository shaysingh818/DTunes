<script setup lang="ts">
import NavigationBar from "./components/navigation/NavigationBar.vue";
</script>


<template>

   <div class="flex">
    <NavigationBar />
  </div>

  <main class="p-2">
    <RouterView />
  </main>
</template>