
<template>
  <div class="container">
    <div class="hero-section">

      <div class="hero-text-section">

        <h1> A cross platform, open source audio application with a built in pomodoro
          focus timer </h1>

      </div>

      <div class="download-hero-section">
        <button @click="downloadApplication" class="download-button-hero" type="button">Download</button> 
      </div>

    </div>

    <div class="supported-platforms-info">
      <div class="supported-platforms-header">
        <h1> Supported Operating Systems </h1>
      </div>

      <div class="supported-platforms-icons">

        <div class="windows">
          <img src="../assets/windows.svg" alt="Your Logo" title="Your Logo" />
        </div>


        <div class="linux">
          <img src="../assets/linux.svg" alt="Your Logo" title="Your Logo" />
        </div>


        <div class="linux">
          <img src="../assets/apple.svg" alt="Your Logo" title="Your Logo" />
        </div>

      </div>
    </div>
  </div>
</template>

<script>
let domain = "https://github.com";
let user = "shaysingh818"; 
let project = "DTunes/releases/download";
let releaseName = "dtunes-v";
let versionName = "1.1.0"; 
let appName = "dtunes-audio-app";
let baseUrl = `${domain}/${user}/${project}/${releaseName}`

export default {
  name: 'Home',
  methods: {
    async downloadApplication() {
      console.log("Detecting operating system... "); 
      const userAgent = navigator.userAgent;
      if(userAgent.indexOf('Win') !== -1) {
        this.downloadLink = `${baseUrl}${versionName}/${appName}_${versionName}_x64-setup.exe`;
        window.location.href = this.downloadLink;
      } else if (userAgent.indexOf('Mac') !== -1) {
        this.downloadLink = `${baseUrl}${versionName}/${appName}_${versionName}_aarch64.dmg`;
        window.location.href = this.downloadLink;
      } else if (userAgent.indexOf('Linux') !== -1) {
        this.downloadLink = `${baseUrl}${versionName}/${appName}_${versionName}_amd64.deb`;
        window.location.href = this.downloadLink;
      } else {
        alert("Not on supported operating system");
      }

    }
  },
  data() {
    return {
      downloadLink: `${domain}/${user}/releases/tag/dtunes-v${versionName}`
    };
  }
}

</script>