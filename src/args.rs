
use clap::{Parser, Subcommand};
use crate::interface::playlist::PlaylistCommand;
use crate::interface::audio_file::AudioFileCommand;

/// 🎵 Simple command line audio player with built in pomodoro timer
#[derive(Parser, Debug)]
#[clap(author= "Shay Singh", version, about, long_about = None)]
pub struct DTunesArgs {

    /// map to module type
    #[clap(subcommand)]
    pub entity_type: EntityType,

}


#[derive(Debug, Subcommand)]
pub enum EntityType {

    /// Work with playlists on DTunes
    Playlist(PlaylistCommand),

    // /// View Audio files on Dtunes
    AudioFile(AudioFileCommand)

    // /// Create pomodoro work sessions
    // Pomodoro(PomodoroCommand)

}

