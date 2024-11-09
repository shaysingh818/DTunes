
<script>
import ArtistCard from './ArtistCard.vue';

export default {
    components: { ArtistCard},
    methods: {
        goToAbout() {
            this.$router.push('/about')
        },
    },
}
</script>

<template>
    <div class="cards">
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="10000"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="1000"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg" 
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
      <div>
        <ArtistCard 
          name="NLE Choppa" 
          datePosted="Monday, November 4, 2024" 
          thumbnail="https://www.w3schools.com/html/pic_trulli.jpg"
          songCount="100"
        />
      </div>
    </div>
</template>


<style scoped>

.cards {
    display: grid;
    gap: 1rem;
    margin: 0 auto; 
    overflow: auto; 
}

@media (min-width: 600px) {
  .cards { grid-template-columns: repeat(2, 1fr); }
}

@media (min-width: 800px) {
  .cards { grid-template-columns: repeat(3, 1fr); }
}

@media (min-width: 1000px) {
  .cards { grid-template-columns: repeat(4, 1fr); }
}

@media (min-width: 1200px) {
  .cards { grid-template-columns: repeat(5, 1fr); }
}

@media (min-width: 1500px) {
  .cards { grid-template-columns: repeat(6, 1fr); }
}

@media (min-width: 1900px) {
  .cards { grid-template-columns: repeat(8, 1fr); max-width: 1800px; }
}

</style>