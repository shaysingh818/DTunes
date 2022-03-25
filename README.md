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

2. Install port sf and the compiler
	* Be able to compile with header file and c file
	* Be able to player an audio buffer
	* Play an mp3 file
	* Extract sound values
	* Read the audio programming book and learn about portsf

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
	* Create test binaries for portsf



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














