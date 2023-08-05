pub mod playlist;
pub mod audio_file;
pub mod pomodoro;
pub mod args; 
pub mod interface;
pub mod constants;  

use args::{DTunesArgs, EntityType};
use clap::Parser; 

use interface::playlist::handle_playlist_command;
use interface::audio_file::handle_audio_file_commands; 

fn main()  {
    let args = DTunesArgs::parse();
    
     match args.entity_type {
        EntityType::Playlist(playlist) => handle_playlist_command(playlist),
        EntityType::AudioFile(audio_file) => handle_audio_file_commands(audio_file),
    };
}