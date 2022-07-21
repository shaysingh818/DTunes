# Audio Data Management System

Originally, DTunes had a "urls" module that was responsible for downloading audio files using youtube urls. This module was using youtube-dl and calling a python script to download videos to a desired directory. Now that we have a working audio player engine with autoplay and a solid data model set up, creating a intricate process for downloading youtube-urls is pretty risky. YouTube is a third party source and if they're no longer a reliable source for audio files, a lot of the code and infrastructure could go to waste. Instead, DTunes should focus more on retaining audio files, rather than downloading them from external sources. I'm going to pivot and priortize creating a audio data management/ingestion engine rather than a fancy youtube downloader. 

## Managing Data Sources

1. **Scripting for specific data sources**: Since DTunes is built on low level system languages, I'm going to make use of high level scripting languages to download external audio files from data sources. For youtube, I'll use python and youtube-dl to get audio files for free. The legalities are unclear but it's a viable source for now. We could write other scripts for paid sources like "Spotify" or other companies willing to give me free music.

2. **Importing Audio Datasets for Research**: Some audio file sources will offer large datasets for research purpose. This could come from places like kaggle, or previous data compeitions. We'll want to DTunes to have easy functionality for adding audio datasets to the existing system. 


## Audio File Management System
Whatever audio data source i'm utilizing, there should be a standard file system that relfects how I store, playlists, datasets and songs within DTunes. While DTunes has a database for storing meta-data, audio files should be stored persistent on disk and it's meta data should be syncrhonized with the database. There should also be fault tolerant methods to back up/sync the standard file system across other computers/devices. There also needs to be some form of ledger/logger that indicates if the file has been synchronized with the database, or general file location activity.  


1. **Database Syncing**: The local file system has sync it's meta data to the database instance. The contents in the current file system should replicate what's in the database. This feature will depend on the file ledger/logger for keeping the file system contents synced with the database. For database syncing to work, the file structure will need to be specific for certain data models
	* **Audio File Collection**: For the audio data management system, the database will need to a store a representation of audio files stored in a "folder". The persistent disk folder is the equivalent of a playlist/collection of audio files. For this model, these are the relevant fields needed (Similar to the existing playlist model)
	* **Name**: Name of the audio collection
	* **Date Created**: Date the collection was created
	* **UUID**: Unique identifier for the collection
	* **Disk Space**: Disk space the collection takes up
	* **Number of files**: Number of audio files in the collection
	 

2. **File Replication**: To prevent the loss of audio files, we'll need to have the ability to replicate files/collections for backup. We should have configuration settings for backing up certain collections using the database. The replication process should be networked and able to send replicas to other devices with DTunes installed. Essentially, we'll need our file system to be able to communicate over sockets to transfer data to other devices. 

3. **File Ledger/Logger**: To have full transparency of the file system, there should be a module dedicated to logging the activites that happen inside the file system. The logger should send reports to the database and local files on the system. There should be logs for certain types of actions that happen within the file system. These are the types of logs that should be generated. 
	* **New File**: New file created/inserted into the system
	* **File Moves**: Alert if a file has moved locations
	* **File Deleted**: Alert if a file has been deleted
	* **Folder Created**: Alert if a file collection has been created


**Music Scheduling System**: https://en.wikipedia.org/wiki/Music_scheduling_system
**Playlist MarkUp Language**: https://en.wikipedia.org/wiki/Playlist_markup_language

