
use clap::{Parser, Subcommand};
use crate::interface::playlist::PlaylistCommand;

/// Simple command line audio player with built in pomodoro timer
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
    Playlist(PlaylistCommand)

    // /// Create pomodoro work sessions
    // Pomodoro(PomodoroCommand)

    // /// Add audio sources to DTunes
    // Source(SourceCommand)

    // /// View Audio files on Dtunes
    // File(AudioFileCommands)

}

