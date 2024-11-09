

<template>
    <div class="card-component">
        <div class="card-container">
          <div class="card-container-content">
            <div class="text-content">
              <div class="flex flex-col gap-2">
                <div>
                  <h1>{{ name }}</h1>
                </div>              
                <div class="flex flex-row gap-2" style="display: flex; align-items: center; justify-content: center;">
                  <div>
                    <i :class="['fas', 'fa-music', 'text-white']"></i> 
                  </div>
                  <div>
                    <p style="font-weight: bold;">{{ songCount }}</p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script>
export default {
  name: 'ArtistCard',
  props: {
    name: {
      type: String,
      required: true,
    },
    datePosted: {
      type: String,
      required: true,
    },
    thumbnail: {
      type: String,
      required: true,
    },
    songCount: {
      type: String,
      required: true,
    },
  },
}
</script>


<style scoped>

.card-component {
  width: 185px;
}

.card-container {
    border-radius: 3%;
    display: flex;
    height: 185px;
    width: 185px;
    background-image: url("https://www.w3schools.com/html/pic_trulli.jpg");
}

.card-container-content {
    border-radius: 3%;
    width: 100%;
    height: 100%;
    background: rgb(0, 0, 0); /* Fallback color */
    background: rgba(0, 0, 0, 0.5); /* Black background with 0.5 opacity */
}

.text-content {
    display: flex;
    height: 100%;
    justify-content: center;
    align-items: center; 
}

.icon-content {
    padding: 8px; 
}

h1 {
  color: white;
  font-weight: bold; 
}

p {
  font-size: 12px;
  color: rgb(209 213 219);
}

</style>