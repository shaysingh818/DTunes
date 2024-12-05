
<template>
    <div class="flex hover:bg-stone-900">
        <div class="flex-none w-24 pr-2">
            <div class="list-view-image-container">
                <img :src="thumbnail" :alt="alt" class="list-view-image p-1" />
            </div>
        </div>
        <div class="flex-auto w-64">
            <div class="list-view-text-container">
              <h2>{{ title }}</h2>
              <p>{{ datePosted }}</p>
            </div>
        </div>
        <div class="flex-auto w-32">
          <div class="list-view-trailing-container">
            <div class="grid grid-flow-col auto-cols-max space-x-4">

              <div class="hover:bg-stone-400"><i :class="['fas', 'fa-play', 'text-red-800']"></i></div>
              <div class="flex flex-row gap-2">
                <div class="hover:bg-stone-400"><i :class="['fas', 'fa-music', 'text-red-800']"></i></div>     
                <div><p1>100</p1></div>
              </div>
            </div>
          </div>
        </div>
    </div>
</template>


<script>
export default {
  name: 'PlaylistList',
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