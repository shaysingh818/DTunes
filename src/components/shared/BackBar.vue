
<template>
    <div class="back-bar">
        <div class="flex">
            <div class="flex-none w-16">
                <div class="back-buttons"> 
                    <div>
                        <i @click="goBack()" :class="['fas', 'fa-arrow-left', 'text-red-800']" style="font-size: 25px;"></i>
                    </div>
                </div>
            </div>

            <div class="grow h-14">
                <div class="page-header">
                    <div class="flex flex-row gap-4">
                        <div class="image-container" :id="`backbar-image-${this.itemId}`"></div>
                    <div class="text-content">
                        <div class="flex flex-col">
                        <div>
                            <h1>{{ this.title }}</h1>
                        </div>
                        <div>
                            <p>{{ this.dateCreated }}</p>
                        </div>
                        </div>

                    </div>
                    </div>
                </div>
            </div>

            <div class="flex-none w-32">
                <div class="trailing-icons">
                    <i @click="openWindow = true" :class="['fas', 'fa-edit', 'text-red-800']" style="font-size: 20px;"></i>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import { appLocalDataDir, dataDir } from '@tauri-apps/api/path';
import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';

export default {
  name: 'BackBar',
  props: {
    itemId: {
      type: String,
      required: true,
    },
    title: {
      type: String,
      required: true,
    },
    thumbnail: {
      type: String,
      required: true,
    },
    dateCreated: {
      type: String,
      required: true,
    },
    openWindow: {
      type: Boolean,
      required: false,
    },
  },
  methods: {
    goBack() {
      this.$router.go(-1); 
    },
  },
  async mounted() {

    console.log("THUMBNAIL", this.thumbnail); 
    const fileBuffer = await readFile(`dtunes-audio-app/images/${this.thumbnail}`, {
        baseDir: BaseDirectory.Data,
    });
    const imageUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("IMAGE URL", imageUrl); 

    let imageElem = document.getElementById(`backbar-image-${this.itemId}`);
    if(imageElem) {
      imageElem.style.backgroundImage = `url(${imageUrl})`; 
    } else {
      console.log(`${this.itemId} not found`)
    }

  }
}
</script>


<style scoped>
p {
    font-size: 12px;
    color: rgb(209 213 219);
}


h1 {
    color: white;
    font-weight: bold;
}

</style>