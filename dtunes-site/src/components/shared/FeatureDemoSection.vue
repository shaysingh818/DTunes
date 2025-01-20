<template>

    <div class="feature-demo-section">


        <div class="feature-text">
            <div>
                <h1> {{ header }} </h1>
            </div>
            <div>
                <p> {{ text }} </p>
            </div>
        </div>

        <div class="feature-video-section">
             <video width="600" height="600" controls>
                <source :src="videoSrc" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

    </div>

</template>

<script>

export default {
   props: {
    header: {
      type: String,
      required: true,
    },
    text: {
      type: String,
      required: true,
    },
    videoSrc: {
      type: String,
      required: true,
    }
  },
}

</script>

<style scoped>

h1 {
    font-size: 60px; 
}

p {
    font-size: 20px;
}


@media (max-width: 600px) {
  h1 {
    font-size: 35px; 
  }
} 



</style>