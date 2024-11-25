use chrono;
use std::fs;
use std::io;
use std::path::Path; 
use std::io::Read; 
use std::fs::File;
use std::ffi::OsStr;
use std::io::{BufReader}; 
use base64::encode; 
use mp3_duration;
use mp4::{Result as MP4Result};
use directories::BaseDirs;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};


#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AudioFile {
    pub audio_file_id: usize,
    pub file_name: String,
    pub file_path: String,
    pub thumbnail: String,
    pub duration: String,
    pub plays: usize,
    pub sample_rate: String,
    pub date_created: String,
    pub last_modified: String,
}

impl AudioFile {
    pub fn new(
        file_name: &str,
        file_path: &str,
        thumbnail: &str,
        plays: usize,
        sample_rate: &str,
    ) -> AudioFile {
        AudioFile {
            audio_file_id: 0,
            file_name: String::from(file_name),
            file_path: String::from(file_path),
            thumbnail: String::from(thumbnail),
            duration: String::from(""),
            plays: plays,
            sample_rate: sample_rate.to_string(),
            date_created: chrono::offset::Local::now().to_string(),
            last_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute("BEGIN TRANSACTION", [])?;
        conn.execute(
            "INSERT INTO AUDIO_FILE 
                (FILE_NAME, FILE_PATH, THUMBNAIL, DURATION, PLAYS, SAMPLE_RATE, DATE_CREATED, LAST_MODIFIED) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8)",
            [
                &self.file_name,
                &self.file_path,
                &self.thumbnail,
                &self.duration, 
                &self.plays.to_string(),
                &self.sample_rate.to_string(),
                &self.date_created,
                &self.last_modified 
            ],
        )?;
        conn.execute("COMMIT", [])?; 
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<AudioFile>> {
        let mut stmt = conn.prepare("SELECT * FROM AUDIO_FILE")?;

        let audio_files: Result<Vec<AudioFile>> = stmt.query_map([], |row| {
            Ok(AudioFile {
                audio_file_id: row.get(0)?,
                file_name: row.get(1)?,
                file_path: row.get(2)?,
                thumbnail: row.get(3)?,
                duration: row.get(4)?,
                plays: row.get(5)?,
                sample_rate: row.get(6)?,
                date_created: row.get(7)?,
                last_modified: row.get(8)?,
            })
        })?.collect(); 

        audio_files
    }

    pub fn update(&mut self, conn: &Connection, id: &str) -> Result<()> {
        /* change date modified */
        self.last_modified = chrono::offset::Local::now().to_string();

        conn.execute(
            "UPDATE AUDIO_FILE
                SET FILE_NAME=?, FILE_PATH=?, THUMBNAIL=?, DURATION=?, PLAYS=?, SAMPLE_RATE=?, 
                DATE_CREATED=?, LAST_MODIFIED=?
                WHERE AUDIO_FILE_ID=?",
            [
                &self.file_name,
                &self.file_path,
                &self.thumbnail,
                &self.duration,
                &self.plays.to_string(),
                &self.sample_rate,
                &self.date_created,
                &self.last_modified,
                id,
            ],
        )?;
        Ok(())
    }

    pub fn delete(&mut self, conn: &Connection, id: &str) -> Result<()> {
        conn.execute("DELETE FROM AUDIO_FILE WHERE AUDIO_FILE_ID=?", [id])?;
        Ok(())
    }

    pub fn view(conn: &Connection, id: &str) -> Result<AudioFile> {
        let query = "SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID = ?";
        conn.query_row(query, &[&id], |row| {
            Ok(AudioFile {
                audio_file_id: row.get(0)?,
                file_name: row.get(1)?,
                file_path: row.get(2)?,
                thumbnail: row.get(3)?,
                duration: row.get(4)?,
                plays: row.get(5)?,
                sample_rate: row.get(6)?,
                date_created: row.get(7)?,
                last_modified: row.get(8)?,
            })
        })
    }

    pub fn set_duration(&mut self, user_data_path: &str) -> Result<(), String> {

        let file_path = format!("{}/{}", user_data_path, self.file_path);
        let extension = Path::new(&self.file_path)
            .extension()
            .and_then(OsStr::to_str);

        println!("FILE PATH: {:?}", file_path); 

        match extension {
            Some("mp4") => {
                let f = File::open(file_path).unwrap();
                let size = f.metadata().unwrap().len();
                let reader = BufReader::new(f);
                let mp4 = mp4::Mp4Reader::read_header(reader, size).expect("unable to get mp4 reader");
                self.duration = mp4.duration().as_secs_f64().to_string();
            },
            Some("mp3") => {
                let path = Path::new(&file_path);
                let duration = mp3_duration::from_path(&path).unwrap();
                let duration_seconds = duration.as_secs_f64().to_string();
                let duration_fmt = format!("{:.6}", duration_seconds);
                self.duration = duration_fmt;  
            },
            Some(ext) => {
                let err_msg = format!("Unsupported Extension: {}", ext);
                return Err(err_msg.to_string())
            },
            None => {
                let err_msg = "Could not parse extension";
                return Err(err_msg.to_string())
            }
        }

        Ok(())
    }
}

#[tauri::command]
pub fn create_audio_file(
    alias_file_name: &str,
    audio_file_name: &str,
    thumbnail_file_name: &str,
    user_local_file_path: &str,
    user_local_thumbnail_path: &str,
    user_thumbnail_path: &str,
    user_audio_file_path: &str,
    user_db_path: &str,
) -> String {

    let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, thumbnail_file_name);
    let usr_file_path = format!("{}/{}", user_audio_file_path, audio_file_name);
    // get_audio_duration(user_audio_file_path.to_string());

    println!("USR THUMBNAIL PATH: {:?}", usr_thumbnail_path);
    println!("USR LOCAL THUMBNAIL: {:?}", user_local_thumbnail_path);
    // println!("AUDIO DURATION")

    // Try to copy the thumbnail file and log any errors
    match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
        Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
        Err(e) => eprintln!("Error copying thumbnail: {}", e),
    }

    // Try to copy the audio file and log any errors
    match fs::copy(user_local_file_path, usr_file_path) {
        Ok(bytes) => println!("Successfully copied audio file with {} bytes", bytes),
        Err(e) => eprintln!("Error copying audio file: {}", e),
    }

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    let mut audio_file: AudioFile = AudioFile::new(
        alias_file_name,
        audio_file_name,
        thumbnail_file_name,
        0,
        "2",
    );
    audio_file.set_duration(user_audio_file_path).unwrap();

    match audio_file.insert(&conn) {
        Ok(()) => {
            conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert audio file metadata: {}", e),
    }
}

#[tauri::command]
pub fn view_audio_files(user_db_path: &str) -> Vec<AudioFile> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match AudioFile::retrieve(&conn) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub async fn read_image_from_data_dir(image_name: String) -> Result<String, String> {
    // Define the path to your image file
    let image_path = format!("/home/dsm001-primary/.local/share/dtunes-audio-app/images/{}", image_name);

    // Open the file and read it into a vector of bytes
    let mut file = match File::open(&image_path) {
        Ok(f) => f,
        Err(_) => return Err("Failed to open the image file.".into()),
    };

    let mut buffer = Vec::new();
    match file.read_to_end(&mut buffer) {
        Ok(_) => {
            // Convert the binary data to a base64 string
            let base64_image = encode(&buffer);
            Ok(base64_image)
        },
        Err(_) => Err("Failed to read the image file.".into()),
    }
}
