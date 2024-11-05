
<template>
    <div class="hover:bg-stone-900">
        <div class="card-container">
          <div class="card-container-content">
            <div class="text-content">
              <h1>{{ title }}</h1>
              <p>{{ datePosted }}</p>
            </div>
            <div class="icon-content">
              <div class="grid grid-flow-col auto-cols-max space-x-4">
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-list-ul', 'text-white']"></i></div>
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-play', 'text-white']"></i></div>
                <div><p>{{ duration }}</p></div>
              </div>

            </div>
          </div>
        </div>
    </div>
</template>


<script>
export default {
  name: 'SongCard',
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

.card-container {
    border-radius: 3%;
    display: flex;
    height: 200px;
    width: 200px;
    background-image: url("https://www.w3schools.com/html/pic_trulli.jpg");
    align-items: flex-end;
}

.card-container-content {
    border-radius: 3%;
    width: 100%;
    height: 100px;
    background: rgb(0, 0, 0); /* Fallback color */
    background: rgba(0, 0, 0, 0.5); /* Black background with 0.5 opacity */
}

.text-content {
    /* border: 1px solid #ccc;   */
    width: 100%;
    position: relative;
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