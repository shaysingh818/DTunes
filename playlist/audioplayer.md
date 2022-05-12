# Playlist (Audio Collections)
This is the feature document for the playlist audio collections library. The audio collection library is meant to simulate playlist functionality, similar to spotify, youtube and other forms of media. The playlist functionality should allow you to add songs to container or collection. The goal of this library is to work with songs of certains genre's and perform specific algorithms to a set of audio files. 

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

1. For creating a playlist, the user should specify the name of the playlist that they want to create. The name is used as a way to identify a collection of songs in a group. The name can represent a group of your favorite songs from a genre, or a group of songs used to emulate some DSP concept or training a machine learning model. 

2. For testing the create playlist functionality, the system should ensure that invalid names/empty parameters can't be passed. For now, the testing is very simple for this feature. 

## Updating a playlist

1. For updating a playlist, the user should be able to change the name of the given playlist. When the user changes the name, it should update the ```dateCreated``` field and populate it with the time that the field was updated. 

2. There should also be some sort of metric that measures how often the playlist is listened to. The playlist will have a relation that has audio files added to it. There should be a number or some sort of value that indicates how often the playlist is used. This should be a part of updating playlist contents. 


## Deleting a playlist

1. The playlist should be able to be deleted by a unique identifer (UUID) and the name of the playlist. If the playlist is being deleted by a name, it should account for the fact that more than one playlist can have the same name. 


## Methods for viewing a playlist

1. There should be a general method to view all the playlists in the system. It should view all the names and the unique identifiers associated with it. It would be nice to have filters for viewing a playlist by UUID and viewing a playlist by name. 



