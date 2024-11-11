<script setup lang="ts">
</script>


<template>
    <div class="audio-bar-container">

        <div class="flex flex-row gap-2">

            <div class="song-info">
                <div class="flex flex-row gap-2"> 
                    <div class="image-container">
                        <img :src="thumbnail" :alt="alt" class="image p-1" />
                    </div>

                    <div class="flex flex-col">
                        <div>
                            <h1>{{ title  }}</h1>
                        </div>
                        <div>
                            <p>{{ datePosted }}</p>
                        </div>
                    </div>

                </div>
            </div>

            <div class="player-controls">

                <div class="flex flex-col gap-2">
                    <div class="player-buttons">
                        <div>
                            <i :class="['fas', 'fa-backward', 'text-white']"></i>
                        </div>
                        <div class="play-button">
                            <i :class="['fas', 'fa-play', 'text-white']"></i>
                        </div>
                        <div>
                            <i :class="['fas', 'fa-forward', 'text-white']"></i>
                        </div>
                    </div>
                    <div class="audio-duration">
                        <div class="w-full bg-gray-200 rounded-full h-1 dark:bg-gray-700">
                            <div class="bg-red-600 h-1 rounded-full" style="width: 45%"></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>


<script>
export default {
  name: 'SongList',
  props: {
    title: {
      type: String,
      required: true,
    },
    datePosted: {
      type: String,
      required: true,
    },
    duration: {
      type: String,
      required: true,
    }, 
    thumbnail: {
      type: String,
      required: true,
    },
  },
}
</script>


<style scoped>

.audio-bar-container {
    position: fixed;
    bottom: 0;
    left: 0;
    width: 100%;
    height: 75px; 
    background-color: #333;
    color: white;
    padding: 10px;
}

.song-info {
    display: flex;
    justify-content: left;
    align-items: left;
    height: 50px; 
    width: 30%; 
    /* border: 1px solid white  */
}

.player-controls {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 50px;
    width: 70%;  
}


.image-container {
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
}

.image {
  display: flex;
  border: 1px; 
  border-radius: 15%; /* Optional: Rounded corners */
  height: 60px;
  width: 60px;  
}

.text-container {
  padding: 4px; 
  height: 100%;
  justify-content: left;
}

.play-button {
    border-radius: 50%;
    display: flex;
    align-items: center;
    background-color: red;
    width: 30px;
    height: 30px;
    justify-content: center; 
}

.audio-duration {
    width: 400px;
}

.player-buttons {
    display: flex;
    flex-direction: row;
    gap: 1rem; 
    justify-content: center;
    align-items: center;
}

hr {
  border: 1px solid gray 
}

p {
    font-size: 10px;
}

</style>