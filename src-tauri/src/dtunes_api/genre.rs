use chrono;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};
use crate::dtunes_api::audio_file::AudioFile;


#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Genre {
    pub genre_id: usize,
    pub genre_name: String,
    pub genre_thumbnail: String,
    pub date_created: String,
    pub last_modified: String,
}

impl Genre {
    pub fn new(name: &str, thumbnail: &str) -> Genre {
        Genre {
            genre_id: 0,
            genre_name: String::from(name),
            genre_thumbnail: String::from(thumbnail),
            date_created: chrono::offset::Local::now().to_string(),
            last_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO GENRE 
                (GENRE_NAME, GENRE_THUMBNAIL, DATE_CREATED, LAST_MODIFIED) 
                VALUES (?1, ?2, ?3, ?4)",
            [
                &self.genre_name,
                &self.genre_thumbnail,
                &self.date_created,
                &self.last_modified,
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Genre>> {
        let mut stmt = conn.prepare("SELECT * FROM GENRE ORDER BY LAST_MODIFIED")?;
        let rows = stmt.query_map([], |row| {
            Ok(Genre {
                genre_id: row.get(0)?,
                genre_name: row.get(1)?,
                genre_thumbnail: row.get(2)?,
                date_created: row.get(3)?,
                last_modified: row.get(4)?,
            })
        })?;

        let mut genres = Vec::new();
        for genre in rows {
            genres.push(genre?);
        }

        Ok(genres)
    }

    pub fn update(&mut self, conn: &Connection, id: &str) -> Result<()> {
        /* change date modified */
        self.last_modified = chrono::offset::Local::now().to_string();

        conn.execute(
            "UPDATE GENRE
                SET GENRE_NAME=?, GENRE_THUMBNAIL=?, DATE_CREATED=?, LAST_MODIFIED=?
                WHERE GENRE_ID=?",
            [
                &self.genre_name,
                &self.genre_thumbnail,
                &self.date_created,
                &self.last_modified,
                id,
            ],
        )?;
        Ok(())
    }

    pub fn delete(conn: &Connection, id: &str) -> Result<()> {
        conn.execute("DELETE FROM GENRE WHERE GENRE_ID=?", [id])?;
        Ok(())
    }

    pub fn view(conn: &Connection, id: &str) -> Result<Genre> {
        let query = "SELECT * FROM GENRE WHERE GENRE_ID = ?";
        conn.query_row(query, &[&id], |row| {
            Ok(Genre {
                genre_id: row.get(0)?,
                genre_name: row.get(1)?,
                genre_thumbnail: row.get(2)?,
                date_created: row.get(3)?,
                last_modified: row.get(4)?,
            })
        })
    }

    pub fn add_audio_file(&mut self, conn: &Connection, audio_file_id: usize) -> Result<()> {
        conn.execute(
            "INSERT INTO GENRE_AUDIO_FILE
                (GENRE_ID, AUDIO_FILE_ID)
            VALUES (?1, ?2)
            ",
            [&self.genre_id, &audio_file_id],
        )?;

        Ok(())
    }

    pub fn retrieve_audio_files(conn: &Connection, id: &str) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = "SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM GENRE_AUDIO_FILE WHERE GENRE_ID=?);";
        let mut stmt = conn.prepare(query)?;

        /* return audio files */
        let rows = stmt.query_map([id], |row| {
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
        })?;

        /*  store files here */
        let mut audio_files = Vec::new();
        for audio_file in rows {
            audio_files.push(audio_file?);
        }
        Ok(audio_files)
    }

    pub fn search_audio_files(
        conn: &Connection, 
        id: &str,
        search_term: &str) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = format!("SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM GENRE_AUDIO_FILE WHERE GENRE_ID=? AND AUDIO_FILE.FILE_NAME LIKE '%{}%');", search_term);
        let mut stmt = conn.prepare(&query)?;
        let audio_files: Result<Vec<AudioFile>> = stmt.query_map([id], |row| {
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

    pub fn remove_audio_file(&self, conn: &Connection, audio_file_id: usize) -> Result<()> {
        conn.execute(
            "DELETE FROM GENRE_AUDIO_FILE
                WHERE GENRE_ID=? AND AUDIO_FILE_ID=?
            ",
            [&self.genre_id, &audio_file_id],
        )?;
        Ok(())
    }

    pub fn search(conn: &Connection, search_term: &str) -> Result<Vec<Genre>> {
        let query = format!("SELECT * FROM GENRE WHERE GENRE_NAME LIKE '%{}%'", search_term);
        let mut stmt = conn.prepare(&query)?;
        let genres: Result<Vec<Genre>> = stmt.query_map([], |row| {
            Ok(Genre {
                genre_id: row.get(0)?,
                genre_name: row.get(1)?,
                genre_thumbnail: row.get(2)?,
                date_created: row.get(3)?,
                last_modified: row.get(4)?,
            })
        })?.collect(); 
        genres
    }
}
