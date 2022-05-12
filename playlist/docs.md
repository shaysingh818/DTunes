
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
