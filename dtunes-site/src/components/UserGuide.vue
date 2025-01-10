<template>

    <div class="container">
        <div class="about-hero-section">
            <h1>DTunes User Guide</h1>
        </div>

        <div>
            <HeaderTextSection 
                header="First Time Setup"
                text="If it's your first time setting up DTunes, you'll need to
                run the sync button on the home page. This will create the required
                metadata storage on your computer to store audio files, images and a local database.
                Once you've run it, if you click it again, it will say you've already completed the setup."
            />

             <video width="800" controls>
                <source src="../assets/first_time_setup.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

        <div>
            <HeaderTextSection 
                header="Add Audio Files"
                text="Now that you're first time setup is complete, you can store audio files. 
                Adding an audio file just requires and mp3 file and a corresponding image or thumbnail."
            />

             <video width="800" controls>
                <source src="../assets/create_song.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>

        <div>
            <HeaderTextSection 
                header="Categorize your audio files"
                text="Once you have some audio files on the application, you can start categorizing
                by playlist, genre, artist or pomodoro sessions. All of these entities allow you to add audio
                files in a similar manner."
            />

             <video width="800" controls>
                <source src="../assets/audio_categorization.webm" type="video/mp4">
                    Your browser does not support the video tag.
            </video>
        </div>


    </div>



</template>

<script>
import HeaderTextSection from './shared/HeaderTextSection.vue';

export default {
    components: {HeaderTextSection},
    name: 'UserGuide',
}
</script>