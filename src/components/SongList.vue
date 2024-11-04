
<template>
    <div class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="image-container">
                <img :src="thumbnail" :alt="alt" class="image p-1" />
            </div>
        </div>
        <div class="flex-auto w-64">
            <div class="text-container">
              <h2>{{ title }}</h2>
              <p>{{ datePosted }}</p>
            </div>
        </div>
        <div class="flex-auto w-32">
          <div class="trailing-container">
            <div class="grid grid-flow-col auto-cols-max space-x-4">
              <div><p1>{{ duration }}</p1></div>
              <div class="hover:bg-stone-400"><i :class="['fas', 'fa-list-ul', 'text-red-800']"></i></div>
              <div class="hover:bg-stone-400"><i :class="['fas', 'fa-play', 'text-red-800']"></i></div>
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

.trailing-container {
  display: flex; 
  height: 100%;
  align-items: center;
  justify-content: center; 
}

h2 {
    color: white;
    font-weight: bold;
}

p {
    font-size: 12px;
    color: rgb(209 213 219);
}

p1 {
    font-size: 14px;
    color: rgb(209 213 219);
}


hr {
  border: 1px solid gray 
}



</style>