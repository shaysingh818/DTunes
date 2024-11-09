
<template>
    <div class="audio-files-container">
        <div class="flex flex-col">
            <div>
                <SongList 
                    title="Through the fire and flames" 
                    datePosted="Monday, November 4, 2024" 
                    duration="2:52"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <SongList 
                    title="Pirates of the Carribean" 
                    datePosted="Monday, November 4, 2024" 
                    duration="3:01"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <SongList 
                    title="Super Mario Bros" 
                    datePosted="Monday, November 4, 2024" 
                    duration="1:25"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <SongList 
                    title="Through the fire and flames" 
                    datePosted="Monday, November 4, 2024" 
                    duration="2:52"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <SongList 
                    title="Pirates of the Carribean" 
                    datePosted="Monday, November 4, 2024" 
                    duration="3:01"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <SongList 
                    title="Super Mario Bros" 
                    datePosted="Monday, November 4, 2024" 
                    duration="1:25"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
        </div>
    </div>
</template>

<script>

import SongList from './SongList.vue';

export default {
  components: { SongList},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
  },
}
</script>