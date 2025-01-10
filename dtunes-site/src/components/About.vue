<template>

    <div class="container">
        <div class="about-hero-section">
            <h1> What is the purpose of DTunes?</h1>
        </div>

        <div>
            <HeaderTextSection 
                header="User Experience"
                text="DTunes aims to have a great user experience for people who
                want to listen to their favorite audio in the background with a built
                in pomodoro timer for aiding with focus."
            />
        </div>

        <div>
            <HeaderTextSection 
                header="Aiding Focus With Pomodoro"
                text="DTunes comes with a built in pomodoro timer. This allows you to set
                a duration to focus, duration for a short or long break. The pomodoro technique
                time management method that involves breaking work into focus periods, separated by short breaks."
            />
        </div>

        <div>
            <HeaderTextSection 
                header="Categorizing Audio"
                text="DTunes allows you to do more then store audio files. You can
                categorize them by your favorite artists, genres and playlists. The application
                allows you create and store these types of entities for categorizing audio files."
            />
        </div>

    </div>

</template>


<script>
import HeaderTextSection from './shared/HeaderTextSection.vue';

export default {
    components: {HeaderTextSection},
    name: 'About',
}
</script>