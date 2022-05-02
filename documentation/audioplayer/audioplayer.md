# Personal Audio Player
This is the feature document for the personal audio player aspect of DTunes. This will include the implementation for creating audio playlists, mp3/wav players and audio queueing. The playlist will be implemented using the relational sqlite3 database. The mp3/wav players will be created by making use of portsf or any temporary audio player for C. The audio queueing will be made with any in memory data strucutres at our disposal. 


## Playlists (Audio Collections)

1. The audio playlist will be associated with a name and date. The playlist represents a collection of songs that we like to listen to or process. The songs in the playlist will also keep track of how many time the song has been listened to (for personal use). Overall, the playlist is meant to group audio files together in a database location for training data, data labeling. 

2. We'll need functionality to add a song to a playlist. The playlist has a ONE to MANY relationship for songs. One playlist can have many songs, but songs cannot have many playlists. When creating the insert method, we'll need a table that associates a song id with a playlist. All the playlist and song data should have unique identifiers with it. It would be catastrophic to have a song with the same name added to a playlist. 

3. There will need to be a method for viewing all the songs associated with a playlist. This should execute a query that gets all the songs associated with a playlist id of some sort. Again, we'll need some type of unique ID generation in order for this to work.

4. We'll need functionality for creating a (ONE TO MANY) relationship for songs and playlists. A playlist can have many songs and vice versa. We'll need to associate each of the songs and playlists using unique uuid's. When looking for songs in a certain collection, we'll search the (ONE TO MANY) relationship table and isolate the results using the uuid.  

5. For deleting playlists, we'll need to build a cascading effect, similar to web frameworks like Django and Flask. When we delete a playlist, we'll need to delete all the (ONE TO MANY) relationships associated with it. There will be table that holds the relationship (mentioned above). Before the playlist instance is deleted, it will search all the occurences of the playlist collection to avoid extra (ONE TO MANY) relationships being left in the database. We'll also need a method to remove a song from a playlist (in general). 

6. Since the (ONE TO MANY) relationship is simply the realtion of ids, we need methods to view each entity by UUID. This is similar to a detail view. For both the song and playlist, we need a method that allows us to pass in the given UUID from the database and return the entity in the structure. 
