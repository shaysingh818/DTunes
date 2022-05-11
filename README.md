# DTunes: 

DTunes is a CLI based audio repository. The idea and purpose of this project is to combine the concepts of music theory and computer science. This is a project that I plan to work on as I continue to verse myself in music theory concepts using "The Audio Programming Book" By Richard Boulanger and Victor Lazzarani. Along with music theory concepts, I'll be employing graph based algorithms from the book "Graph Algorithms" by O'Riley. This readme contains all the concepts I plan to research and build on top of once I implement the basic features of a simple music player.  If there are any questions regarding the semantics/logistics of the project, feel free to email: shalinsingh818@gmail.com. At this time, i'm not open to any contributions unless, the developer has 
extensive experience with C/Audio Programming. 



<p float="center">
  <img src="https://github.com/shaysingh818/DTunes/blob/master/screenshots/menu.png" width=100% height=10% />
</p>

# Features
Current features for DTunes include, Audio File Storage, MP3 dataset generation and Playlist Creation. 

## Audio File Storage 
Audio files can be stored in sqlite3 database. The library contains a structure that stores all the needed information such as, the file path, name, date created etc. The file storage structure is also used for in memory operations needed for the internal toolset. The audio file library has basic functionality such as creating, reading, updating and deleting files from the database. 

## MP3 Dataset Generation 
DTunes requires audio files of all types, MP3, wav, AIFF. I wanted to create some sort of "scraper" that could get audio files from a large repository. YouTube was the next best option. Using the well known library, ```youtubedl```, a wrapper was implemented for a MP3 downloader. The wrapper includes functionality for multithreading to optimize download speeds. The wrapper is a C program that uses a python script for downloading youtube videos. The wrapper calls the script and multithreads the runtime so that the script is not downloading urls linearly. 

## Playlist (Audio Collection)
For audio players, playlists are usually implemented to save a batch of songs to listen to or process. The playlist collection library allows you to add a subset of songs from the repository using a ONE TO MANY relationship. A playlist can store many songs and the user can view all the songs in the playist using the name or the UUID. 

# Getting Started
## Usage
### Installing
To install DTunes, the following libraries are requires
	* sqlite3
	* portsf

