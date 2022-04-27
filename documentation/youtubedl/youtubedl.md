# MP3 Dataset Generation

DTunes requires a large dataset of mp3 files for testing the portsf sound processing library. This feature is responsible for downloading mp3 files from youtube using the "youtubedl" library. The feature will have options for multi-threading, grouping and saving files to the database. 

1. We'll need to create a function that calls a script for downloading a youtube url with a provided url. The provided url is a command line argument passed to the script. The C code base will call that script using the ```system()``` function call provided by the standard library. 


2. The videos that we want to download are youtube urls that are freely available from the website. DTunes has a table/schema set up for storing urls with the current date that they were added to the database. The urls can be added and viewed using the following commands
	* ```./dtunes vu``` to view urls in the database
	* ``` ./dtunes cu <url> ``` to insert url into the database

3. For generating large datasets, we'll need a function that can export the urls saved in the database to a csv or json file. We'll need a function that can export the data with a data format as a parameter. The json/csv file doesn't contain the raw video data, only the urls themselves. This is so that other machines can backup/redownload videos from previous versions. 


4. To optimize download speeds, we'll incorporate multithreaded techniques to download videos in parallel. Rather than linearly iterating through a list of urls, we'll split the urls into subsets and download with parallel routines. The feature or function should have an parameter for specifying how many threads will be used in the backup sequence. If the urls are the same, we will run into serious issues. The file names will be the same and "youtubedl" will try to replace an existing filename concurrently, which will cause the script to crash. 

