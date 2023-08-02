use rusqlite::{Connection, Result};
use clap::{Args, Subcommand};
use crate::audio_file::AudioFile;

static DB_PATH: &str = "../../db/dtunes.db";


#[derive(Debug, Args)]
pub struct AudioFileCommand {

    #[clap(subcommand)]
    pub command: AudioFileSubcommand,
}


#[derive(Debug, Subcommand)]
pub enum AudioFileSubcommand {

    /// Sync Audio sources from config file
    AddFileFromSource(AddSourceConfig),

    /// View sources in DTunes database
    View(ViewAudioFiles),

}

#[derive(Debug, Args)]
pub struct AddSourceConfig {

    /// Name of playlist
    pub source_name: String,

    // URL of audio file from source 
    pub url: String
}

#[derive(Debug, Args)]
pub struct ViewAudioFiles {

    /// Name of playlist
    pub name: String,
}


pub fn handle_audio_file_commands(audio_file: AudioFileCommand) {
    let command = audio_file.command;
    match command {
        AudioFileSubcommand::AddFileFromSource(audio_file) => {
            let _ = add_file_from_source(audio_file);
        }
        AudioFileSubcommand::View(audio_file) => {
            let _ = view_audio_files(audio_file);
        }
    }
}


pub fn add_file_from_source(_audio_file: AddSourceConfig) -> Result<()> {

    let _conn = Connection::open(DB_PATH)?;
    Ok(())
}


pub fn view_audio_files(audio_file: ViewAudioFiles) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    if audio_file.name == "all" {
        let files : Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        for f in files {
            println!(" ℹ️   {:?} : {:?}", f.file_name, f.date_created); 
        }
    } else {

        let my_file = AudioFile::view(&conn, &audio_file.name)?; 
        println!("ℹ️ {:?} : {:?}", my_file.file_name, my_file.date_created); 
    }    
    Ok(())
}