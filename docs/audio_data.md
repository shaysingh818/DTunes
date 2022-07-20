# Audio Data Management System

Originally, DTunes had a "urls" module that was responsible for downloading audio files using youtube urls. This module was using youtube-dl and calling a python script to download videos to a desired directory. Now that we have a working audio player engine with autoplay and a solid data model set up, creating a intricate process for downloading youtube-urls is pretty risky. YouTube is a third party source and if they're no longer a reliable source for audio files, a lot of the code and infrastructure could go to waste. Instead, DTunes should focus more on retaining audio files, rather than downloading them from external sources. I'm going to pivot and priortize creating a audio data management/ingestion engine rather than a fancy youtube downloader. 

## Managing Data Sources

1. **Scripting for specific data sources**: Since DTunes is built on low level system languages, I'm going to make use of high level scripting languages to download external audio files from data sources. For youtube, I'll use python and youtube-dl to get audio files for free. The legalities are unclear but it's a viable source for now. We could write other scripts for paid sources like "Spotify" or other companies willing to give me free music.

2. **Importing Audio Datasets for Research**: Some audio file sources will offer large datasets for research purpose. This could come from places like kaggle, or previous data compeitions. We'll want to DTunes to have easy functionality for adding audio datasets to the existing system. 


## Audio File Management System
Whatever audio data source i'm utilizing, there should be a standard file system that relfects how I store, playlists, datasets and songs within DTunes. While DTunes has a database for storing meta-data, audio files should be stored persistent on disk and it's meta data should be syncrhonized with the database. There should also be fault tolerant methods to back up/sync the standard file system across other computers/devices. There also needs to be some form of ledger/logger that indicates if the file has been synchronized with the database, or general file location activity.  


1. **Database Syncing**: The local file system has sync it's meta data to the database instance. The contents in the current file system should replicate what's in the database. This feature will depend on the file ledger/logger for keeping the file system contents synced with the database. For database syncing to work, the file structure will need to be specific for certain data models
	* **Audio File Collection Model**: For the audio data management system, the database will need to a store a representation of audio files stored in a "folder". The persistent disk folder is the equivalent of a playlist/collection of audio files.
	 

  

2. **File Replication**: 

3. **File Ledger/Logger**: 

4. **Dataset Generation**: 



# Exporting/Importing dataset files
1. This is a feature that isn't urgent. We'll need to rewrite our own version of pandas or some type of library that can read/process csv files. A seperate feature document will be rewritten for the custom csv file library. The current temporary solutions are bad and not worth shipping into the main code base yet. 

3. For generating large datasets, we'll need a function that can export the urls saved in the database to a csv or json file. We'll need a function that can export the data with a data format as a parameter. The json/csv file doesn't contain the raw video data, only the urls themselves. This is so that other machines can backup/redownload videos from previous versions. 

4. We need to be able to take all the urls from the database and write them to a cvs file wwith specific name format. The name format should be written like this, <date of export><urlcount>.csv. The csv file should contain the youtube url, and date it was inserted into the database. Overall, this should be able to export all the urls from the database to a file. 

5. For importing csv files, we'll need to read the url from each column in the csv file and throw it into a massive array pool. The array pool will then be thrown to to the "thread argument builder" and start the multithreaded download process. This should do the same thing as downloading a bunch of urls from the database, except with a csv file. The csv file should also be thrown in as a command line argument. We also might want to add some sort of validation to make the csv file is correctly read. This section is for importing csv files directly to the threaded downloader

6. We'll also want something that can take in a csv file and sync it to the database. This should go through each section in the csv file and add the given url to the database. There should be some type of method that ensures the csv file is formatted correctly for the database schema. 


