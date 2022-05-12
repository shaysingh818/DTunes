# Playlist (Audio Collections)
This is the feature document for the playlist audio collections library. The audio collection library is meant to simulate playlist functionality, similar to spotify, youtube and other forms of media. The playlist functionality should allow you to add songs to container or collection. 


## The Data Model
The data model for a playlist is fairly simple for now. Currently it stores, the name of the playlist, the date it was created, and some sort of unique identifier. Down below is the current structure in the library

```c

struct AudioPlaylist {
    uuid_t playlistId;
    char name[100];
    char dateCreated[30];
};

```

## Creating A Playlist

```int createPlaylist(playlist_t* playlist);```

The create playlist function takes in an instance of a playlist structure. The playlist structure can be populated by allocating memory and copying the string values to the field. Down below is an example of a test case that creates a playlist.

```c

char *mytime = getCurrentTime(); // function from the db repo
// create playlist instance
playlist_t *newPlaylist;
newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));
strcpy(newPlaylist->name, "test-playlist"); // copy string value
strcpy(newPlaylist->dateCreated, mytime); // copy date value
int insertDbResult = createPlaylist(newPlaylist);
```


1. For creating a playlist, the user should specify the name of the playlist that they want to create. The name is used as a way to identify a collection of songs in a group. The name can represent a group of your favorite songs from a genre, or a group of songs used to emulate some DSP concept or training a machine learning model. 

2. For testing the create playlist functionality, the system should ensure that invalid names/empty parameters can't be passed. For now, the testing is very simple for this feature. 

## Viewing a playlist

```int viewPlaylists()```


```c 
int viewPlaylists(); 
```

1. **Purpose**: This function is responsible for viewing all the playlist entites that are currently in the database. 


## Deleting a playlist

```c 
int deletePlaylist(char *playlistUuid);
```

1. **Purpose**: This function deletes a playlist using a UUID generated from inserting a playlist. The UUID is a unique identifer for deleting/viewing an individual playlist.

2. **Parameters**:
	 * ```char *playlistUuid``` : UUID (unique identifer of the playlist


```c
int deletePlaylistByName(char *playlistName);
```  

1. **Purpose**: Delete a playlist by specifying the name. This is similar to the delete playlist method using the UUID. .

2. **Parameters**:
	* ```char *playlistName``` : Name of the playlist, no testing has been done for deleting multiple playlists with the same name. 


## Add song to playlist

```c 
int addSongToPlaylist(char *songUuid, char *playlistUuid);
```
1. **Purpose**: Adds a song to a playlist using the unique identifer for the given playlist and the song to be added. This function can be called through the CLI or using the API.  
2. **Parameters**
	* ```char *playlistUuid``` : UUID (unique identifer of the playlist	
	* ```char *songUuid``` : UUID (unique identifer of the song to add to the playlist
