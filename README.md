# DTunes: 
DTunes is a CLI based audio repository. This audio repository will be used to expirment with algorithms related to audio data/processing. 

## The personal music application 
There are multiple features/aspects to DTunes. One feature that DTunes has, is the ability to act as personal audio player.  


# Main features of DTUNES
1. Be able to listen to music without interuptions
	* AD free (no interruptions)
	* Autoplay, songs play in queue right after eachother
	* Create an audio library that supports "deep work" mindset
	* Detect patterns of songs that increase productivity
2. Modify/Manipulate audio files
	* Work with audio/digital signal processing concepts
	* Extract digital signal data from audio files
3. Create audio visualizers
	* Create line base visualizer
5. Create a data collection center for audiofiles
	* Automatically cluster songs by certain genres
	* Create an easy to use library for syncing the dataset to other PCs
6. Apply concepts in graph theory with the audio dataset
	* Use graphs for clustering/organizing data in certain categories
	* Use graphs for modeling behavior with audio data
	* Use graphs to generate playlists


## To Do
1. Need to be able to update songs with a new name
	* Argument pattern takes 3
	* dtunes, existing song name, new song name

2. Create a custom local endpoint library
	* Properly write out the purpose/functions of the endpoint library
	* Change all instance of ```struct Endpoint  *e``` to endpoint_t *e
	* Implement custom argument patterns for endpoints
	* Create a default endpoint
	* Make it easy to test the endpoints
	* Auto generate cli command for test endpoint functions
	* Use a linked list to append endpoints to the system
	* Migrate endpoint system to DTunes
	* Migrate endpoints for syncing audio files
	* Make the exec endpoints functions execute all functions with different numbers of parameters passed

3. Create a view for youtube urls
	* see all the current urls stored in db

4. Create youtube url download backup process
	* Go through all stored urls in the database
	* Download each youtube url
	* Eliminate duplicates
	* Create a function to export urls to a csv or json file
	
5. Create one to many entity relationship for Playlist -> Song
	* Be able to add song to an existing playlist in the database
	* Create a table for adding multiple songs to ONE playlist
	* View songs in playlist

6. Create makefiles that can generate different binaries
	* Create binaries that test individual componenets of the system
	* Test song, playlist and endpoint library



## Audio File Syncing
Read files from local directory into sqlite3 database

1. Read file information in directory and rename files with desired format
	* File names need to parse out anything larger than 15 characters
	* Assign each audio file with current time for song insert
	* Parse file extension
	* Need to grab the current path that we can stream/listen to the audio file from
	* Somehow need to extract subtitles from audio file if possible

2. Integrate youtube dl python script in c code base
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



## Streaming/Playing Audio

1. Figure out how to play/stream audio file in c
	* Read audio file from local path
	* Play audio file and view audio signal output

2. Audio Queueing
	* Create a custom library for listening to audio files
	* Be able to play audio files in certain selected order
	* Use data structures to store temporary queues etc. 


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














