use rusqlite::{Connection, Result};
use clap::{Args, Subcommand};
use crate::source::Source;
use crate::constants::*; 

static DB_PATH: &str = "../../db/dtunes.db";


#[derive(Debug, Args)]
pub struct SourceCommand {

    #[clap(subcommand)]
    pub command: SourceSubcommand,
}


#[derive(Debug, Subcommand)]
pub enum SourceSubcommand {

    /// Sync Audio sources from config file
    Sync(SyncSourceConfig),

    /// View sources in DTunes database
    View(ViewSources),

}

#[derive(Debug, Args)]
pub struct SyncSourceConfig {

    /// Name of playlist
    pub config_file: String,
}

#[derive(Debug, Args)]
pub struct ViewSources {

    /// Name of playlist
    pub name: String,
}


pub fn handle_source_command(source: SourceCommand) {
    let command = source.command;
    match command {
        SourceSubcommand::Sync(source) => {
            let _ = sync_source(source);
        }
        SourceSubcommand::View(source) => {
            let _ = view_source(source);
        }
    }
}


pub fn sync_source(source: SyncSourceConfig) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    Source::sync_config_file(&conn, &source.config_file)?;  
    println!("{} Synced sources from {:?}", DB_SUCCESS_EMOJI, &source.config_file);
    Ok(())

}


pub fn view_source(source: ViewSources) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    if source.name == "all" {
        let sources : Vec<Source> = Source::retrieve(&conn)?;
        for s in sources {
            println!(" ℹ️   {:?} : {:?}", s.source_name, s.date_created); 
        }
    } else {

        let my_source = Source::view(&conn, &source.name)?; 
        println!("ℹ️ {:?} : {:?}", my_source.source_name, my_source.date_created); 
    }
    
    Ok(())

}