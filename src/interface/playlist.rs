use clap::{Args, Parser, Subcommand};
use crate::playlist::Playlist;
use rusqlite::{Connection, Result};


static DB_PATH: &str = "db/dtunes.db";


#[derive(Debug, Args)]
pub struct PlaylistCommand {

    #[clap(subcommand)]
    pub command: PlaylistSubcommand,
}


#[derive(Debug, Subcommand)]
pub enum PlaylistSubcommand {

    /// Create playlist on DTunes
    Create(CreatePlaylist),

    /// View playlist by name
    View(ViewPlaylist),

    /// Update playlist name
    Update(UpdatePlaylist),

    /// Delete playlist
    Delete(DeletePlaylist),

    /// Show playlist
    Show(ViewPlaylist),

}

#[derive(Debug, Args)]
pub struct CreatePlaylist {

    /// Name of playlist
    pub name: String,
}

#[derive(Debug, Args)]
pub struct ViewPlaylist {

    /// Name of playlist
    pub name: String,
}

#[derive(Debug, Args)]
pub struct UpdatePlaylist {

    /// name of playlist
    pub name: String,
}


#[derive(Debug, Args)]
pub struct DeletePlaylist {

    /// name of playlist
    pub name: String,
}


pub fn handle_playlist_command(playlist: PlaylistCommand) {
    let command = playlist.command;
    match command {
        PlaylistSubcommand::Create(playlist) => {
            create_playlist(playlist);
        }
        PlaylistSubcommand::Update(playlist) => {
            update_playlist(playlist);
        }
        PlaylistSubcommand::Delete(delete_entity) => {
            delete_playlist(delete_entity);
        }
        PlaylistSubcommand::View(playlist) => {
            show_playlists();
        }
        PlaylistSubcommand::Show(playlist) => {
            show_playlists();
        }
    }
}

pub fn create_playlist(playlist: CreatePlaylist) -> Result<()> {

    let martini_emoji = '\u{2705}';
    println!("{} Creating playlist: {:?}", martini_emoji, playlist.name);
    let conn = Connection::open(DB_PATH)?;

    let mut my_playlist : Playlist = Playlist::new(&playlist.name);
    my_playlist.insert(&conn)?;

    println!("Inserted playlist: {:?} ;)", &playlist.name); 

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


pub fn delete_playlist(playlist: DeletePlaylist) {

    println!("Deleting playlist: {:?}", playlist);

}


pub fn show_playlists() {

    println!("Showing playlist: {:?}", "playlist");

}