pub mod playlist;
pub mod audio_file;
pub mod pomodoro;
pub mod source;
pub mod args; 
pub mod interface; 

use args::{DTunesArgs, EntityType};
use interface::playlist::handle_playlist_command; 
use clap::Parser; 

fn main()  {
    let args = DTunesArgs::parse();
    
     match args.entity_type {
        EntityType::Playlist(playlist) => handle_playlist_command(playlist),
    };
}