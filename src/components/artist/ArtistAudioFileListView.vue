<template>
    <div>
        <div class="flex flex-col" v-for="(item, index) in artistStore.audioFiles" :key="index">
            <div>
                <ArtistAudioFile
                    :artistId="id"
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :lastModified="item.last_modified"
                    :plays="item.plays"
                    :sampleRate="item.sample_rate"
                    :artistRemove=true
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { artistStore } from '../../api/Artist';
</script>

<script>
import { artistStore } from '../../api/Artist';
import ArtistAudioFile from './ArtistAudioFile.vue';

export default {
  components: { ArtistAudioFile},
   props: {
    id: {
      type: Number,
      required: true,
    },
  },
  data() {
    return {
      audioFiles: []
    }
  },
  async mounted() {
    console.log("ARTIST ID", this.id)
    await artistStore.viewArtistAudioFiles(this.id.toString());
    console.log("PLAYLIST AUDIO FILE STORE LOADED: ", artistStore.audioFiles); 
  },    
}
</script>