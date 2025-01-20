<template>

    <div class="container">
        <div class="about-hero-section">
            <h1>DTunes User Guide</h1>
        </div>

        <div>

            <HeaderTextSection 
                header="Virus Scan"
                text="This application has not been code signed yet, to use it you'll have
                to allow it past windows smart screen/windows defender. For MACOS it might require allowing it
                past whatever antivirus/security software could potentially block it. 
                Code signing for windows costs money and I'm broke. As for MACOS and Linux, more work still needs 
                to done to certify the application."
            />
       
        </div>

        <div>
            <FeatureDemoSection
                header="1.) First Time Setup"
                text="If it's your first time setting up DTunes, you'll need to
                run the sync button on the home page. This will create the required
                metadata storage on your computer to store audio files, images and a local database.
                Once you've run it, if you click it again, it will say you've already completed the setup."
                :videoSrc="firstTimeSetup"
            />
        </div>

        <div>

            <FeatureDemoSection
                header="2.) Add Audio Files"
                text="Now that you're first time setup is complete, you can store audio files. 
                Adding an audio file just requires a mp3, wav or flacc file and a corresponding image thumbnail. The
                thumbnail accepts the following image formats, jpeg, webp, png, jpg."
                :videoSrc="firstTimeSetup"
            />

        </div>

        <div>
            <FeatureDemoSection
                header="3.) Categorize Audio Files"
                text="Once you have some audio files on the application, you can start categorizing
                by playlist, genre, artist or pomodoro sessions. All of these entities allow you to add audio
                files in a similar manner."
                :videoSrc="audioCategorization"
            />
        </div>


    </div>



</template>

<script>
import HeaderTextSection from './shared/HeaderTextSection.vue';
import firstTimeSetup from '../assets/first_time_setup.webm';
import FeatureDemoSection from './shared/FeatureDemoSection.vue';
import createSong from '../assets/create_song.webm';
import audioCategorization from '../assets/audio_categorization.webm'; 

export default {
    components: {HeaderTextSection, FeatureDemoSection},
    name: 'UserGuide',
    data() {
        return {
            firstTimeSetup,
            createSong,
            audioCategorization
        };
    }
}
</script>