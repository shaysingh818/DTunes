<template>

    <div class="container">
        <div class="about-hero-section">
            <h1>Available features on DTunes?</h1>
        </div>

        <div>
            <FeatureDemoSection
                header="Audio file categorization"
                text="DTunes allows you to store audio files and categorize by playlist,
                genre or artist. Once a user stores audio files on the applications, they can create
                genres, artist or playlists and associate audio files with it."
                :videoSrc="audioCategorizationVideo"
            />
        </div>

        <div>
            <FeatureDemoSection
                header="Pomodoro Timer"
                text="DTunes comes with a built in pomodoro timer. This is a feature for those that like to listen to music to enhance focus! Users can customize their
                focus duration, short break and long break duration. When the timer is finished, it will play the default DTunes alarm sound."
                :videoSrc="pomodoroTimerVideo"
            />
        </div>

        <div>
            <FeatureDemoSection
                header="Local Metadata Storage"
                text="DTunes does not host or stream data centrally. All your audio file related data is stored on a lightweight database. Users have
                complete control their data and can use the application without internet access."
                :videoSrc="metadataStorageVideo"
            />
        </div>

    </div>

</template>

<script>
import HeaderTextSection from './shared/HeaderTextSection.vue';
import FeatureDemoSection from './shared/FeatureDemoSection.vue';
import audioCategorizationVideo from '../assets/audio_categorization.webm';
import pomodoroTimerVideo from '../assets/pomodoro_sessions.webm';
import metadataStorageVideo from '../assets/create_song.webm';

export default {
    components: {HeaderTextSection, FeatureDemoSection},
    name: 'Features',
    data() {
        return {
            audioCategorizationVideo,
            pomodoroTimerVideo,
            metadataStorageVideo
        };
    }
}
</script>