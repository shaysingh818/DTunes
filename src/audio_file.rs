use soloud::*; 
use chrono; 
use rusqlite::{Connection, Result};

#[derive(Debug)]
pub struct AudioFile {
    pub name: String, 
    pub date_created: String,
    pub date_modified: String,
    pub file_count: usize,
    pub disk_space: usize 
}

pub fn play_wav(filepath: &str) -> Result<(), Box<dyn std::error::Error>> {

    let mut sl = Soloud::default()?; 
    let mut wav = audio::Wav::default();

    wav.load(&std::path::Path::new(filepath))?;

    sl.play(&wav);
    while sl.voice_count() > 0 {
        std::thread::sleep(std::time::Duration::from_millis(100));

        let duration = std::time::Duration::from_millis(100); 
        println!("Duration: {:?}", duration); 
    }
    
    println!("{:?} Done playing ", filepath);

    Ok(())
} 