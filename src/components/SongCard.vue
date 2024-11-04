
<template>
    <div class="hover:bg-stone-900">
        <div class="card-container">
            <div class="image-container">
                <img :src="thumbnail" :alt="alt" class="image" />
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
    display: flex;
    align-items: center;
    justify-content: center; 
    height: 200px;
    width: 200px;
    /* border: 1px solid #ccc; */  
}

.image-container {
    height: 100%;
    width: 100%; /* Ensure the container is also set to 100% width */
}

.image {
    border-radius: 3%;
    height: 100%;
    width: 100%; /* Fill the width of the container */
    object-fit: cover; /* Maintain aspect ratio, fill the container */
}

</style>