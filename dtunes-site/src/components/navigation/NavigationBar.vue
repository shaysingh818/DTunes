
<script setup lang="ts">
</script>

<template>

  <div class="navigation-bar">

    <div @click="goToHome" class="logo-section">

      <div class="logo">
        <img src="../../assets/dtunes-logo.png" alt="Your Logo" title="Your Logo" />
      </div>
      <div>
        <h1> DTunes </h1>
      </div>
    </div>

    <div class="nav-items">

      <div @click="goToFeatures">
        <p>Features</p>
      </div>

      <div @click="goToUserGuide">
        <p>User Guide</p>
      </div>

      <div @click="goToSupport">
        <p>Support</p>
      </div>

      <div @click="goToAbout">
          <p>About</p>
      </div>

    </div>

    <div class="download-section">
      <a href="https://github.com/shaysingh818/DTunes/releases/tag/dtunes-v1.1.0">
         <button class="download-button" type="button">Download</button> 
      </a>
    </div>

  </div>

</template>

<script>
export default {
  methods: {
    goToHome() {
      this.$router.push('/');
    },
    goToSupport() {
      this.$router.push('/support');
    },
    goToAbout() {
      this.$router.push('/about');
    },
    goToFeatures() {
      this.$router.push('/features');
    },
    goToUserGuide() {
      this.$router.push('/user_guide');
    },
  },
};

</script>



<style scoped>
p {
  color: white; 
}

</style>