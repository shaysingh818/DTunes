use rusqlite::{Connection, Result};
use clap::{Args, Subcommand};
use crate::playlist::Playlist;
use crate::audio_file::AudioFile; 
use crate::constants::*; 


static DB_PATH: &str = "../../db/dtunes.db";


#[derive(Debug, Args)]
pub struct PlaylistCommand {

    #[clap(subcommand)]
    pub command: PlaylistSubcommand,
}


#[derive(Debug, Subcommand)]
pub enum PlaylistSubcommand {

    // Create playlist on DTunes
    Create(CreatePlaylist),

    // View playlist by name
    View(ViewPlaylist),

    // Update playlist name
    Update(UpdatePlaylist),

    // Delete playlist
    Delete(DeletePlaylist),

    // Add File to Playlist
    AddFile(AddFilePlaylist),

    // View Audio Files in playlist
    ViewFiles(ViewFilesPlaylist),

    // Queue files from playlist
    Queue(ViewFilesPlaylist),

}

#[derive(Debug, Args)]
pub struct CreatePlaylist {

    // Name of playlist
    pub name: String,
}

#[derive(Debug, Args)]
pub struct ViewPlaylist {

    // Name of playlist
    pub name: String,
}

#[derive(Debug, Args)]
pub struct UpdatePlaylist {

    // name of playlist
    pub name: String,
}


#[derive(Debug, Args)]
pub struct DeletePlaylist {

    // name of playlist
    pub name: String,
}

#[derive(Debug, Args)]
pub struct AddFilePlaylist {

    // name of playlist
    pub playlist_name: String,

    // name of audio file
    pub file_name: String,
}


#[derive(Debug, Args)]
pub struct ViewFilesPlaylist {

    // name of playlist
    pub playlist_name: String,

}


pub fn handle_playlist_command(playlist: PlaylistCommand) {
    let command = playlist.command;
    match command {
        PlaylistSubcommand::Create(playlist) => {
            let _ = create_playlist(playlist);
        }
        PlaylistSubcommand::Update(playlist) => {
            let _ = update_playlist(playlist);
        }
        PlaylistSubcommand::Delete(delete_entity) => {
            let _ = delete_playlist(delete_entity);
        }
        PlaylistSubcommand::View(playlist) => {
            let _ = view_playlist(playlist);
        }
        PlaylistSubcommand::AddFile(playlist) => {
            let _ = add_file_playlist(playlist);
        }
        PlaylistSubcommand::ViewFiles(playlist) => {
            let _ = view_files_playlist(playlist);
        }
        PlaylistSubcommand::Queue(playlist) => {
            let _ = queue_files_playlist(playlist);
        }
    }
}

pub fn create_playlist(playlist: CreatePlaylist) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    let mut my_playlist : Playlist = Playlist::new(&playlist.name);
    my_playlist.insert(&conn)?;

    println!("{} Inserted playlist: {:?}", DB_SUCCESS_EMOJI, playlist.name);

    Ok(())

}


pub fn update_playlist(playlist: UpdatePlaylist) -> Result<()> {

    println!("Updating playlist: {:?}", playlist);
    let conn = Connection::open(DB_PATH)?;

    let mut my_playlist : Playlist = Playlist::new(&playlist.name);
    my_playlist.update(&conn)?;

    println!("Updated playlist: {:?} ;)", &playlist.name);
    
    Ok(())

}


pub fn delete_playlist(playlist: DeletePlaylist) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    if playlist.name == "all" {

        conn.execute("DELETE FROM PLAYLIST", [])?;
        println!("❌ Deleted all playlist from DTunes"); 

    } else {

        Playlist::delete(&conn, &playlist.name)?; 
        println!("❌ Deleted {:?}", playlist.name); 
    }

    Ok(())
}


pub fn view_playlist(playlist : ViewPlaylist) -> Result<()> {


    let conn = Connection::open(DB_PATH)?;
    if playlist.name == "all" {
        let playlists : Vec<Playlist> = Playlist::retrieve(&conn)?;
        for p in playlists {
            println!("🎵 {:?} : {:?}", p.name, p.date_created); 
        }
    } else {

        let my_playlist = Playlist::view(&conn, &playlist.name)?; 
        println!("🎵 {:?} : {:?}", my_playlist.name, my_playlist.date_created); 

    }

    Ok(())

}


pub fn add_file_playlist(playlist : AddFilePlaylist) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    let mut my_playlist = Playlist::view(&conn, &playlist.playlist_name)?;
    let audio_file = AudioFile::view(&conn, &playlist.file_name)?; 
    my_playlist.add_audio_file(&conn, &audio_file)?; 
    Ok(())

}



pub fn view_files_playlist(playlist : ViewFilesPlaylist) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    let mut my_playlist = Playlist::view(&conn, &playlist.playlist_name)?;
    let playlist_files : Vec<AudioFile> = my_playlist.retrieve_audio_files(&conn)?;
    println!("Viewing files for: {:?} \n", playlist.playlist_name); 
    for item in playlist_files {
        println!("🎵  {:?}", item.file_name); 
    }
    Ok(())

}

pub fn queue_files_playlist(playlist: ViewFilesPlaylist) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    let mut my_playlist = Playlist::view(&conn, &playlist.playlist_name)?;
    my_playlist.queue_playlist_files(&conn)?;
    Ok(())
}

