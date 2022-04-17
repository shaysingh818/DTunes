# DTunes: 
DTunes is a CLI based audio repository. The idea and purpose of this project is to combine the concepts of music theory and computer science. This is a project that I plan to work on as I continue to verse myself in music theory concepts using "The Audio Programming Book" By Richard Boulanger and Victor Lazzarani. Along with music theory concepts, I'll be employing graph based algorithms from the book "Graph Algorthms" by O'Riley. This readme contains all the concepts I plan to reasearch and build on top of once I implement the basic features of a simple music player. If there are any questions regarding the semantics/logistics of the project, feel free to email: shalinsingh818@gmail.com. At this time, i'm not open to any contributions unless, the developer has extensive experience with C/Audio Programming. 


## Advertisement Free Personal Music Player
1. DTunes creates a AD-free personal music experience. The downfalls of free personal audio players like YouTube and Spotify, is constant interruptions with advertisements.
2. Autoplay: Background music cycles in the background. After a song is done playing, it randomly selects the next song in the cycle or, cycles to the next in the current playlist being listened to. 

## Deep Work Audio Framework
1. The vision of DTunes is to create a system/algorithm that monitors the users productivity. The system should reccommend playlists/groups of songs that increases the user's productivity. Down below is an example use case of how this would work. 
	* System reccommends groups of certain classical songs based on personal interest.  
	* Metric of productivity is based on the user changing the tracks less often and focusing on tasks instead. 
	* Goal is to allow songs to play without the user having to focus on the music app in the background. 
  	* System should overall support the users productivity and not distract them while listening to music

## Digital Signal Processing
1. Modify/Manipulate audio files
	* Work with audio/digital signal processing concepts
	* Extract digital signal data from audio files

## Audio Visualization
1. Create custom audio visualizers
	* Research audio visualization concepts/algorithms
	* Create a line base visualizer (Sine Wave) (Only audio visualizer I know of )

## Personal Audio File Data Store
1. Create a data collection center for audiofiles
	* Automatically cluster songs by certain genres
	* Create an easy to use library for syncing the dataset to other PCs
  
## Graph Algorithm Research
1. Apply concepts in graph theory with the audio dataset
	* Use graphs for clustering/organizing data in certain categories
	* Use graphs for modeling behavior with audio data
	* Use graphs to generate playlists/productivity playlists


## Graph Based Community Detection For Playlist Suggestions. 
Implement various community detection algorithms for finding similarities in audio files. Results should auto create playlists of "communities" that have been found in the graph. Figure out how to properly label the communities. 

## Centrality algorithms for finding songs that have high influence in the network
Implmement centrality algorothms that can detect which nodes are important or play a key factor in the audio application. Use centrality as a way to understand the data flowing through the system.


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




# Syncing Urls to DTUNES

1. Go through all the songs in the database. Iterate through all the youtube urls in the database. For each database url, it should download the mp3 file to the desired audiofile path. Once all the songs have been iterated, it should run the loadAudioFiles function and sync it all to the database. 








