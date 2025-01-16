<template>

    <div class="container">
        <div class="about-hero-section">
            <h1>Noticing issues with DTunes?</h1>
        </div>

        <div>
            <HeaderTextSection 
                header="File a GitHub Issue"
                text="If you notice bugs or encounter errors with application, you can
                file an issue on the GitHub repository: https://github.com/shaysingh818/dtunes/issues"
            />
        </div>

        <div>
            <HeaderTextSection 
                header="Contact The Maintainer"
                text="Since this project is new and doesn't have a big user base, 
                you can contact the maintainer if you have ideas for enhancements: shalinsingh818@gmail.com"
            />

            
        </div>

        <div>
            <HeaderTextSection 
                header="Make A Contribution!"
                text="This is listed last because we do not expect all our
                users to fix issues with our software. However, for our very technical users,
                contributions to the source code for DTunes are more than welcomed and appreciated!"
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
