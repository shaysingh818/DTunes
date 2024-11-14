use chrono;
use rusqlite::{Connection, Result};


#[derive(Debug, Clone)]
pub struct AudioFile {
    pub audio_file_id: usize,
    pub file_name: String,
    pub file_path: String,
    pub thumbnail: String,
    pub duration: String,
    pub plays: usize,
    pub sample_rate: String,
    pub date_created: String,
    pub last_modified: String
}


impl AudioFile {

    pub fn new(
        file_name: &str,
        file_path: &str,
        thumbnail: &str,
        duration: &str,
        plays: usize,
        sample_rate: &str) -> AudioFile {

        AudioFile {
            audio_file_id: 0, 
            file_name: String::from(file_name),
            file_path: String::from(file_path),
            thumbnail: String::from(thumbnail),
            duration: duration.to_string(), 
            plays: plays,
            sample_rate: sample_rate.to_string(),
            date_created: chrono::offset::Local::now().to_string(),
            last_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
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
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<AudioFile>> {
       let mut stmt = conn.prepare("SELECT * FROM AUDIO_FILE ORDER BY LAST_MODIFIED")?;
       let rows = stmt.query_map([], |row| {
            Ok(AudioFile {
                audio_file_id: row.get(0)?,
                file_name: row.get(1)?,
                file_path: row.get(2)?,
                thumbnail: row.get(3)?,
                duration: row.get(4)?,
                plays: row.get(5)?,
                sample_rate: row.get(6)?,
                date_created: row.get(7)?,
                last_modified: row.get(8)?
            })
       })?;

       let mut audio_files = Vec::new(); 
       for file in rows {
            audio_files.push(file?);
       }

       Ok(audio_files)
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
                id
            ],
        )?;
        Ok(())
    }


    pub fn delete(&mut self, conn: &Connection, id: &str) -> Result<()> {
        conn.execute(
            "DELETE FROM AUDIO_FILE WHERE AUDIO_FILE_ID=?",
            [id],
        )?;
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
                last_modified: row.get(8)?
            })
        })
    }

}

