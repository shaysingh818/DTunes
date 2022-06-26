# DTunes: 
The commercialization of music has it's downfalls. With advancements of sound therapy, titration of classical music to improve focus, music is a utility that should be freely researched. Not only should music be easily accessible, but it should be easily reproduced and remixed. While it's enjoyable to create music, It would be interesting to investigate the different techniques of composition within genres of music. DTunes is my idea of how music should be represented in a software system. Applications like Spotify, YouTube don't give the user control of the audio that is at their disposable. DTunes aims not only to be a personal audio player, but a system that allows you to create music and investigate and understand how other songs were created. What I aim to achieve with DTunes, is to walk through life versing myself in the concept of music theory and combine it with the concepts of computer science. Using "The Audio Programming Book" By Richard Boulanger and Victor Lazzarani, we can create a system that reads audio frequencies and allows us to understand the computational representation of an audio file. Concepts in graph theory will also be applied to create algorithms that investigate Behavorial traits in audio files. The graph theory concepts will be referenced using the "Graph Algorithms" book by O'Riley. Overall, DTunes intends to be a project to apply algorithms to understand the composition of music and create the ultimate audio experience for a computer scientist. 

<p float="center">
  <img src="https://github.com/shaysingh818/DTunes/blob/master/screenshots/menu.png" width=100% height=10% />
</p>

# Features
Current features for DTunes include, Audio File Storage, MP3 dataset generation and Playlist Creation. 

## Audio File Storage 
Audio files can be stored in sqlite3 database. The library contains a structure that stores all the needed information such as, the file path, name, date created etc. The file storage structure is also used for in memory operations needed for the internal toolset. The audio file library has basic functionality such as creating, reading, updating and deleting files from the database. 

## MP3 Dataset Generation 
DTunes requires audio files of all types, MP3, wav, AIFF. I wanted to create some sort of "scraper" that could get audio files from a large repository. YouTube was the next best option. Using the well known library, ```youtubedl```, a wrapper was implemented for a MP3 downloader. The wrapper includes functionality for multithreading to optimize download speeds. The wrapper is a C program that uses a python script for downloading youtube videos. The wrapper calls the script and multithreads the runtime so that the script is not downloading urls linearly. 

## Playlist (Audio Collection)
For audio players, playlists are usually implemented to save a batch of songs to listen to or process. The playlist collection library allows you to add a subset of songs from the repository using a ONE TO MANY relationship. A playlist can store many songs and the user can view all the songs in the playist using the name or the UUID. 

# Getting Started
DTunes is currently working only on UNIX based systems. For windows users, you can use something like WSL, or a ubuntu virtual machine. 

## Installing
Clone the github repository and save the folder to a desired location. 
```bash
git clone https://github.com/shaysingh818/DTunes
```

cd into the ```/DTunes``` directory where the Makefile is located
```bash
cd DTunes/
```

Before compiling the source code, there are some required libraries needed to run the initial build. Run the ```install.sh``` with the following command. 

``` bash
./install.sh
```
 After all the libraries are installed, you can compile the source with these commands. 
 ```bash
 make clean
 make clean_binary 
 make
 ```
 
The use of ```make clean``` and ```make clean_binary``` is in case this is your second time compiling the source. The two different commands clean the object files and the binary. If anything goes wrong during the compilation process, feel free to E-mail issues to shalinsingh818@gmail.com.

## Command Line Reference
| Command Line Argument                          | Purpose                                                                          |
|------------------------------------------------|----------------------------------------------------------------------------------|
| ```./dtunes help```                            | View help menu for all command line interface options.                           |
| ```./dtunes tu```                              | Run unit tests for mp3 dataset generation library                                |
| ```./dtunes tp```                              | Run unit tests for playlist creation library                                     |
| ```./dtunes ts```                              | Run unit tests for audio file storage                                            |
| ```./dtunes tdb```                             | Run unit tests for functions interacting with the database repository            |
| ```./dtunes cu <youtube url>```                | Insert youtube url to the database. This is for the backup process               |
| ```./dtunes vu```                              | View all youtube urls in the database                                            |
| ```./dtunes du <youtube url>```                | Delete specific youtube url in the database                                      |
| ```./dtunes dus ```                            | Delete all youtube urls in the database                                          |
| ```./dtunes yb ```                             | Download all youtube urls saved in the database                                  |
| ```./dtunes cs <youtube url>```                | Add a song to the database directly, using a youtube url.                        |
| ```./dtunes vs ```                             | View current audio files stored in the database                                  |
| ```./dtunes ds <SONG UUID> ```                 | Delete song using UUID, you can view uuids by using the vs command.              |
| ```./dtunes dsn <SONG NAME> ```                | Delete song by name, this is not advised since some songs can have the same name |
| ```./dtunes das ```                            | Delete all songs in the database                                                 |
| ```./dtunes cp <PLAYLIST NAME> ```             | Create a playlist collection                                                     |
| ```./dtunes vp```                              | View all playlist collections on dtunes                                          |
| ```./dtunes asp <PLAYLIST UUID> <SONG UUID>``` | Add song to playlist with playlist UUID and SONG uuid.                           |
| ```./dtunes vsp <PLAYLIST UUID>```             | View songs in playlist using the playlist UUID                                   |
| ```./dtunes dp <PLAYLIST UUID>```              | Delete playlist by UUID.                                                         |



