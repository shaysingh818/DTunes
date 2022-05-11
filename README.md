# DTunes: 
DTunes is a CLI based audio repository. The idea and purpose of this project is to combine the concepts of music theory and computer science. This is a project that I plan to work on as I continue to verse myself in music theory concepts using "The Audio Programming Book" By Richard Boulanger and Victor Lazzarani. Along with music theory concepts, I'll be employing graph based algorithms from the book "Graph Algorithms" by O'Riley. This readme contains all the concepts I plan to research and build on top of once I implement the basic features of a simple music player.  If there are any questions regarding the semantics/logistics of the project, feel free to email: shalinsingh818@gmail.com. At this time, i'm not open to any contributions unless, the developer has extensive experience with C/Audio Programming. 

## Current Features

1. **Audio File Storage**:
	* Audio files can be stored in sqlite3 database
	* Database model contains, file path, name, date created etc. 
	* Files can be downloaded from youtube and stored using database model

2. **MP3 Dataset Generation:**
	* Dataset library built on top of ```youtubedl```
	* Files are extracted using youtubedl script
	* Library includes multithreading to speed up download speeds



