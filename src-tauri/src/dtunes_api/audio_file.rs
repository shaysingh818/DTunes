use chrono;
use mp3_duration;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};
use std::ffi::OsStr;
use std::fs::File;
use std::io::BufReader;
use std::path::Path;

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
        let result = conn.execute(
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
        );

        match result {
            Ok(_) => {
                println!("Successfully inserted audio file");
                self.audio_file_id = conn.last_insert_rowid() as usize; 
                return Ok(())
            },
            Err(err) => {
                println!("[audio_file::insert] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<AudioFile>> {
        let mut stmt = conn.prepare("SELECT * FROM AUDIO_FILE ORDER BY PLAYS DESC")?;

        let audio_files: Result<Vec<AudioFile>> = stmt
            .query_map([], |row| {
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
            })?
            .collect();

        audio_files
    }

    pub fn update(&mut self, conn: &Connection, id: &str) -> Result<()> {
        /* change date modified */
        self.last_modified = chrono::offset::Local::now().to_string();

        let result = conn.execute(
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
        );

        match result {
            Ok(_) => {
                println!("Successfully updated audio file");
                return Ok(())
            },
            Err(err) => {
                println!("[audio_file::update] sqlite3 error {:?}", err);
                return Err(err)
            }
        }

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
                let mp4 =
                    mp4::Mp4Reader::read_header(reader, size).expect("unable to get mp4 reader");
                self.duration = mp4.duration().as_secs_f64().to_string();
            }
            Some("mp3") => {
                let path = Path::new(&file_path);
                let duration = mp3_duration::from_path(&path).unwrap();
                let duration_seconds = duration.as_secs_f64().to_string();
                let duration_fmt = format!("{:.6}", duration_seconds);
                self.duration = duration_fmt;
            }
            Some(ext) => {
                let err_msg = format!("Unsupported Extension: {}", ext);
                return Err(err_msg.to_string());
            }
            None => {
                let err_msg = "Could not parse extension";
                return Err(err_msg.to_string());
            }
        }

        Ok(())
    }

    pub fn play(&mut self, conn: &Connection) -> Result<()> {
        
        self.plays += 1;
        let result = conn.execute(
            "UPDATE AUDIO_FILE SET PLAYS=? WHERE AUDIO_FILE_ID=?",
            [&self.plays.to_string(), &self.audio_file_id.to_string()],
        );

        match result {
            Ok(_) => {
                println!("Successfully added play to audio file");
                return Ok(())
            },
            Err(err) => {
                println!("[audio_file::play] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
        
    }

    pub fn search(conn: &Connection, search_term: &str) -> Result<Vec<AudioFile>> {
        let query = format!(
            "SELECT * FROM AUDIO_FILE WHERE FILE_NAME LIKE '%{}%'",
            search_term
        );
        let mut stmt = conn.prepare(&query)?;
        let audio_files: Result<Vec<AudioFile>> = stmt
            .query_map([], |row| {
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
            })?
            .collect();
        audio_files
    }
}
