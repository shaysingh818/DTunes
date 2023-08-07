use std::fs;
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

    /// Sync Audio files from raw data directory to database
    Sync(SyncProps),

    /// View audio files in DTunes database
    View(ViewAudioFiles),

    /// Queue up all audio files in datbase
    Queue(QueueAudioFiles),

}

#[derive(Debug, Args)]
pub struct SyncProps {

    // input path to sync files from
    pub input_path: String,

    // output path to store audio files
    pub output_path: String,

}

#[derive(Debug, Args)]
pub struct ViewAudioFiles {

    /// Name of playlist
    pub name: String,
}


#[derive(Debug, Args)]
pub struct QueueAudioFiles {

    /// Name of playlist
    pub option: Option<String>,
}


pub fn handle_audio_file_commands(audio_file: AudioFileCommand) {
    let command = audio_file.command;
    match command {
        AudioFileSubcommand::Sync(audio_file) => {
            let _ = sync_files(audio_file);
        }
        AudioFileSubcommand::View(audio_file) => {
            let _ = view_audio_files(audio_file);
        }
        AudioFileSubcommand::Queue(audio_file) => {
            let _ = queue_audio_files(audio_file);
        }
    }
}


pub fn sync_files(audio_file: SyncProps) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    let file_names = AudioFile::prepare_raw_file_names(&audio_file.input_path).unwrap();  

    /* call raw file ingestion method */ 
    AudioFile::raw_file_insertion(
        &conn, 
        file_names, 
        &audio_file.input_path,
        &audio_file.output_path
    )?;

    //fs::remove_dir_all(&audio_file.input_path).unwrap(); 
    //fs::create_dir(&audio_file.input_path).unwrap(); 

    Ok(())
}


pub fn view_audio_files(audio_file: ViewAudioFiles) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    if audio_file.name == "all" {
        let files : Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        for f in files {
            println!(" ℹ️  {:?}", f.file_name); 
        }
    } else {

        let my_file = AudioFile::view(&conn, &audio_file.name)?; 
        println!("ℹ️ {:?}", my_file.file_name); 
    }    
    Ok(())
}


pub fn queue_audio_files(_audio_file: QueueAudioFiles) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    AudioFile::queue_audio_files(&conn)?;

    Ok(())
}