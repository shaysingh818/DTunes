<template>

    <div class="container">
        <div class="about-hero-section">
            <h1>Available features on DTunes?</h1>
        </div>

        <div>
            <HeaderTextSection 
                header="Audio File Storage"
                text="DTunes allows you to store Audio Files on the application,
                the current accepted file formats are .mp3 .wav and .flacc"
            />

             <video width="800" controls>
                <source src="../assets/create_song.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

        <div>
            <HeaderTextSection 
                header="Categorize Audio Files"
                text="For any existing audio files stores in DTunes, you can
                create Artists, Genres or Playlists to further categorize the audio files you have."
            />

             <video width="800" controls>
                <source src="../assets/audio_categorization.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

        <div>
            <HeaderTextSection 
                header="Pomodoro Timer"
                text="This is listed last because we do not expect all our
                users to fix issues with our software. However, for our very technical users,
                contributions to the source code for DTunes are more than welcomed and appreciated!"
            />
             <video width="800" controls>
                <source src="../assets/pomodoro_sessions.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

        <div>
            <HeaderTextSection 
                header="Background Audio Playing"
                text="Similar to other audio streaming services, once you click
                an audio file, it will queue the rest in the background and play on a loop of 
                all the files stored on the application."
            />
        </div>

    </div>



</template>

<script>
import HeaderTextSection from './shared/HeaderTextSection.vue';

export default {
    components: {HeaderTextSection},
    name: 'Support',
}
</script>