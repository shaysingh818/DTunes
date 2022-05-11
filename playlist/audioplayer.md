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


1. For creating a playlist, the user simply needs to specify the name. The name is used as a way to identify a collection of songs in a group. The name can represent a group of your favorite songs from a genre, or a group of songs used to emulate some DSP concept or training a machine learning model. 

2. For testing the create playlist functionality, the system should ensure that invalid names/empty parameters can't be passed. For now, the testing is very simple for this feature. 

## Viewing a playlist

```int viewPlaylists()```

1. For viewing a playlist, the system allocates playlist structures and determines the amount by using a ```count``` query in the database. The system returns a list of playlist structures. This is a helper method and is not what is used in the main implementation for viewing playlist. 

2. The view playlists method uses the ```initPlaylists(int limit)``` method to retrieve a list of playlists from the database. Using the returned array, the user can dereference the structure fields and view it on the terminal. 




