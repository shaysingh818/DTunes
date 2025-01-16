<template>

    <div class="header-text-section">

        <div class="header-section">
            <h1> {{ header }} </h1>
        </div>

        <div class="text-section">
            <p> {{ text }} </p>
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
  },
}

</script>

<style scoped>

h1 {
    font-size: 40px; 
}


@media (max-width: 600px) {
  h1 {
    font-size: 25px;
  }
}

p {
    font-size: 20px;
}


@media (max-width: 600px) {
  p {
    font-size: 15px;
    width: 500px;
  }
}

</style>