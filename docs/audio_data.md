# Audio Data Management System
DTunes requires a system to manage large amount of audio filed stored in the system. Whatever external audio data source is being utilized, there should be a standard file system that relfects how playlists, datasets and songs are stored within DTunes. While DTunes has a database for storing meta-data, audio files should be stored persistent on disk and it's meta data should be syncrhonized with the database. There should also be fault tolerant methods to back up/sync the standard file system across other computers/devices. There also needs to be some form of ledger/logger that indicates if the file has been synchronized with the database, or general file location activity. In summary, this is an epic for building a audio file system.   


## Managing Data Sources

1. **Scripting for specific data sources**: Since DTunes is built on low level system languages, High level scripting languages will be used to download external audio files from data sources. For starters, YouTube can be the main data source for audio files. The legalities are unclear but it's a viable source for now. We could write other scripts for paid sources like "Spotify" or other companies willing to give out free music. 

2. **Importing Audio Datasets for Research**: Some audio file sources will offer large datasets for research purpose. This could come from places like kaggle, or previous data compeitions. We'll want to DTunes to have easy functionality for adding audio datasets to the existing system. 

3. **Python YouTube-DL Script**: Create a script that can download audio files to the collections created on DTunes. The script should be able to take csv/json inputs with youtube urls supplied. It would be useful to have multithreaded functionality for downloading youtube urls in parallel. 


## Database Syncronization
The local file system has sync it's meta data to the database instance. The contents in the current file system should replicate what's in the database. This feature will depend on the file ledger/logger for keeping the file system contents synced with the database. For database syncing to work, the file structure will need to be specific for certain data models. This feature will involve having a data model specified for a "audio collection". A collection is a folder with audio files stored within it. 
	
1. **Audio File Collection**: For the audio data management system, the database will need to a store a representation of audio files stored in a "folder". The persistent disk folder is the equivalent of a playlist/collection of audio files. For this model, these are the relevant fields needed (Similar to the existing playlist model)
	* **Name**: Name of the audio collection
	* **Date Created**: Date the collection was created
	* **UUID**: Unique identifier for the collection
	* **Disk Space**: Disk space the collection takes up
	* **Number of files**: Number of audio files in the collection

2. Add CRUD operations the audio collections model. We should be able to create, read, update and delete collections in the system.  When a collection is created/deleted, a corresponding folder should show up or dissapear in the system. Create test cases to understand the limit on how many collections can be created at a time. 

3. Create a general view for seeing audio files stored within a collection. When the user selects a collection, they should be able to see all the audio files stored within it. Allow audio files from the view to be queued into phaedra. This should take all the files from the audio collection and play all the files on "autoplay" without having to play the file each time. 



## File Replication
To prevent the loss of audio files, we'll need to have the ability to replicate files/collections for backup. We should have configuration settings for backing up certain collections using the database. The replication process should be networked and able to send replicas to other devices with DTunes installed. Essentially, we'll need our file system to be able to communicate over sockets to transfer data to other devices. 


1. Create a system that can always monitor the current directory tree structure of the file system that we want to sync up. There needs to be some way to detected changes in the file structure if files are added or removed. If possible, make this run in the background and always be able to detect changes. 

2. Create a library for transfering files/folder content to another device. This should be similar to a FTP protocol but use sockets to transfer file bytes over a local area network. This will work in tandem with the "directory tree" monitor to sync up files across devices. 


1. Explore these later for this epic
	* **Music Scheduling System**: https://en.wikipedia.org/wiki/Music_scheduling_system
	* **Playlist MarkUp Language**: https://en.wikipedia.org/wiki/Playlist_markup_language

