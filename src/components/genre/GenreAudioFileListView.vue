<template>
    <div>
        <div class="flex flex-col" v-for="(item, index) in genreStore.audioFiles" :key="index">
            <div>
                <GenreAudioFile
                    :genreId="id"
                    :audioFileId="item.audio_file_id" 
                    :title="item.file_name" 
                    :datePosted="item.date_created" 
                    :duration="item.duration"
                    :thumbnail="item.thumbnail"
                    :filePath="item.file_path"
                    :lastModified="item.last_modified"
                    :plays="item.plays"
                    :sampleRate="item.sample_rate"
                    :genreRemove=true
                />
            </div>
        </div>
    </div>
</template>

<script setup>
import { genreStore } from '../../api/Genre';
</script>

<script>
import { genreStore } from '../../api/Genre';
import GenreAudioFile from './GenreAudioFile.vue';

export default {
  components: { GenreAudioFile},
   props: {
    id: {
      type: Number,
      required: true,
    },
  },
  async mounted() {
    console.log("GENRE ID", this.id)
    await genreStore.viewGenreAudioFiles(this.id.toString());
    console.log("GENRE AUDIO FILE STORE LOADED: ", genreStore.audioFiles); 
  },    
}
</script>