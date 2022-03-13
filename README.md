# DTunes: 
This is a personal audio repository. Can be used as a personal music application, similar to spotify, youtube soundcloud etc. (Have to get the songs from youtube tho). The bigger picture is being able to create algorithms that can group/cluster songs in playlists based on certain attributes. The system auto-creates playlists groups based on certain similarties and reccomends them to the user. It could also be used as a search engine for certain audio files. Using the clustering algorithms, the application is able to index/find groups of auio files based on what the user wants. 



# Goals
1. Listen to music with out AD's
2. Modify/Manipulate audio files
3. Create audio visualizers
4. Create a sick audio library that kicks spotify/youtube's ass. 
5. Collect a shit ton of audiofiles
6. Learn about audio signal processing

## To Do
1. Make command line args shorter
2. Run through every feature and make sure it works
	* Hook youtube dl process to insert-song
3. Make functions to update names of songs
4. Make add song function insert youtube url AND SONG information in one call


## Streaming/Playing Audio

1. Figure out how to play/stream audio file in c
	* Read audio file from local path
	* Play audio file and view audio signal output

2. Audio Queueing
	* Create a custom library for listening to audio files
	* Be able to play audio files in certain selected order
	* Use data structures to store temporary queues etc. 

4. Create one to many entity relationship for Playlist -> Song
	* Be able to add song to an existing playlist in the database
	* Create a table for adding multiple songs to ONE playlist
	* View songs in playlist

5. Use youtube DL as data source for inital songs/videos to store
	* Create a python script that downloads youtube videos to a local directory
	* Store all the videos in a local folder for the src code
	* Be able to filter file extension, quality, playback speed.

8. Create test cases for everything
	* Test all functions in playlist library
	* Test all functions in song library



## Audio File Syncing
Read files from local directory into sqlite3 database

1. Read file information in directory and rename files with desired format
	* File names need to parse out anything larger than 15 characters
	* Assign each audio file with current time for song insert
	* Parse file extension
	* Need to grab the current path that we can stream/listen to the audio file from
	* Somehow need to extract subtitles from audio file if possible

2. Integreate youtube dl python script in c code base
	* Store youtube urls in db
	* Don't allow duplicate downloads, skip existing youtube urls
	* Refactor python script to take youtube url as command line arg
	* Create function or process to monitor youtube downloads

3. Need to be able to update names
	* Specify the song id and update name
	* File names are too long 
	* Figure out way to automatically rename them shorter

4. Download songs using backup YT urls in database
	* Be able to download songs using urls in db


## Community detection for auto generating playlists
Implement various community detection algorithms for finding similarities in audio files. Results should auto create playlists of "communities" that have been found in the graph. Figure out how to properly label the communities. 

## Centrality algorithms for finding songs that have high influence in the network
Implmement centrality algorothms that can detect which nodes are important or play a key factor in the audio application. Use centrality as a way to understand the data flowing through the system.




## Fixes/Improvements/BUGS

1. Unique ID generation for database tables
	* Unique ID generation is not built in with C
	* Keep track of the last insert row id
	* Use last row insert id + 1 as next row id

2. Blank space for date column
	* Date created columns in structures add blank spaces on entries



# Graph Algorithms

1. BFS/DFS (Breadth/Depth first search)
2. Minimum Spanning Tree
3. Random Walk
4. All pairs shortest path
5. Single Source shortest path


## Centrality 
1. Degree Centrality
2. Closeness Centrality
3. Betweeness Centrality
4. Page Rank


## Community Detection
1. Triangle Count and Clustering Coeif
2. Local/Global Clustering Coefficient
3. Label Propogation
4. Louvain Modularity














