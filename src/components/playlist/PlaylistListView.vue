
<template>
        <div class="flex flex-col">
            <div>
                <PlaylistList 
                    title="Through the fire and flames" 
                    datePosted="Monday, November 4, 2024" 
                    duration="2:52"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <PlaylistList 
                    title="Pirates of the Carribean" 
                    datePosted="Monday, November 4, 2024" 
                    duration="3:01"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <PlaylistList 
                    title="Super Mario Bros" 
                    datePosted="Monday, November 4, 2024" 
                    duration="1:25"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <PlaylistList 
                    title="Through the fire and flames" 
                    datePosted="Monday, November 4, 2024" 
                    duration="2:52"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <PlaylistList 
                    title="Pirates of the Carribean" 
                    datePosted="Monday, November 4, 2024" 
                    duration="3:01"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
            <div>
                <PlaylistList 
                    title="Super Mario Bros" 
                    datePosted="Monday, November 4, 2024" 
                    duration="1:25"
                    thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
                />
            </div>
    </div>
</template>

<script>

import PlaylistList from './PlaylistList.vue';

export default {
  components: { PlaylistList},
  methods: {
    goToAbout() {
      this.$router.push('/about')
    },
  },
}
</script>