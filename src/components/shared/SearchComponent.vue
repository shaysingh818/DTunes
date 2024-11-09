<template>
    <div class="search-container">
        <div class="flex flex-row gap-2">
            <div>
                <input type="text" id="search-artist" name="search-artist" value="Search Artists Across DTunes"><br>
            </div>
            <div>
                 <button type="button">Search</button> 
            </div>
        </div>
    </div>
</template>


<style scoped>

.search-container {
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 10px;
}

button {
    border-radius: 5%;
    width: 100px; 
    height: 50px;
    background-color: rgb(153 27 27);
    color: white;
}

input {
    border-radius: 3%;
    height: 50px;
    width: 650px;
    padding: 8px;
    background-color: rgb(28 25 23);
    color: rgb(153 27 27);
    outline: rgb(153 27 27);
}


@media (min-width: 400px) {
  input { width: 200px; }
}

@media (min-width: 600px) {
  input { width: 200px; }
}

@media (min-width: 800px) {
  input { width: 400px; }
}

@media (min-width: 1000px) {
  input { width: 650px; }
}

@media (min-width: 1200px) {
  input { width: 800px; }
}



</style>