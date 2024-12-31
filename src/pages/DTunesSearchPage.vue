<script>
import BackBar from '../components/shared/BackBar.vue';
import { audioStore } from '../api/AudioFile';
import AudioFileListView from '../components/audio_file/AudioFileListView.vue';
import { useRoute } from 'vue-router';


export default {
  components: { AudioFileListView, BackBar},
  async mounted() {
    const route  = useRoute();
    const searchTerm = route.params.searchTerm;
    await audioStore.searchAudioFiles(searchTerm);
  },
  setup() {
    const route = useRoute();
    const searchTerm = route.params.searchTerm;

    const currentDate = new Date();
    const formattedDate = currentDate.getFullYear() + "-" +
    String(currentDate.getMonth() + 1).padStart(2, '0') + "-" +
    String(currentDate.getDate()).padStart(2, '0');

    return { searchTerm, formattedDate };
  }
}
</script>

<template>

  <BackBar 
    :itemId="0" 
    :title="searchTerm"
    thumbnail="default_dtunes_thumbnail.webp"
    :dateCreated="formattedDate"
  />
  <br>
  <div class="page-container">
    <div class="flex flex-col">
      <div>
         <AudioFileListView />
      </div>
    </div>
  </div>
</template>