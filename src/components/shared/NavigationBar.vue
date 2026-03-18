
<script setup lang="ts">
import NavigationItem from "./NavigationItem.vue";
</script>


<template>

    <!-- Sidebar Navigation -->
    <aside class="fixed inset-y-0 left-0 w-32 bg-stone-900 text-gray-200 p-2">
      <nav class="flex flex-col space-y-3">

        <RouterLink to="/" class="block p-2">
          <NavigationItem icon="fa-home" nav-title="home"/>
        </RouterLink>

        <RouterLink to="/audio-files" class="block p-2">
          <NavigationItem icon="fa-music" nav-title="audio files" />
        </RouterLink>

        <RouterLink to="/pomodoro" class="block p-2"> 
          <NavigationItem icon="fa-clock" nav-title="pomodoro" />
        </RouterLink>

        <RouterLink to="/pomodoro-session-tracking" class="block p-2"> 
          <NavigationItem icon="fa-chart-bar" nav-title="metrics" />
        </RouterLink>
      </nav>
    </aside>

</template>
