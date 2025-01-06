use crate::dtunes_api::audio_file::AudioFile;
use chrono;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Playlist {
    pub playlist_id: usize,
    pub playlist_name: String,
    pub playlist_thumbnail: String,
    pub date_created: String,
    pub last_modified: String,
}

impl Playlist {
    pub fn new(name: &str, thumbnail: &str) -> Playlist {
        Playlist {
            playlist_id: 0,
            playlist_name: String::from(name),
            playlist_thumbnail: String::from(thumbnail),
            date_created: chrono::offset::Local::now().to_string(),
            last_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {

        let result = conn.execute(
            "INSERT INTO PLAYLIST 
                (PLAYLIST_NAME, PLAYLIST_THUMBNAIL, DATE_CREATED, LAST_MODIFIED) 
                VALUES (?1, ?2, ?3, ?4)",
            [
                &self.playlist_name,
                &self.playlist_thumbnail,
                &self.date_created,
                &self.last_modified,
            ],
        );

        match result {
            Ok(_) => {
                println!("Successfully inserted playlist");
                self.playlist_id = conn.last_insert_rowid() as usize; 
                return Ok(())
            },
            Err(err) => {
                println!("[playlist::insert] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Playlist>> {
        let mut stmt = conn.prepare("SELECT * FROM PLAYLIST ORDER BY LAST_MODIFIED")?;
        let rows = stmt.query_map([], |row| {
            Ok(Playlist {
                playlist_id: row.get(0)?,
                playlist_name: row.get(1)?,
                playlist_thumbnail: row.get(2)?,
                date_created: row.get(3)?,
                last_modified: row.get(4)?,
            })
        })?;

        let mut playlists = Vec::new();
        for playlist in rows {
            playlists.push(playlist?);
        }

        Ok(playlists)
    }

    pub fn update(&mut self, conn: &Connection, id: &str) -> Result<()> {
        /* change date modified */
        self.last_modified = chrono::offset::Local::now().to_string();

        let result = conn.execute(
            "UPDATE PLAYLIST
                SET PLAYLIST_NAME=?, PLAYLIST_THUMBNAIL=?, DATE_CREATED=?, LAST_MODIFIED=?
                WHERE PLAYLIST_ID=?",
            [
                &self.playlist_name,
                &self.playlist_thumbnail,
                &self.date_created,
                &self.last_modified,
                id,
            ],
        );

        match result {
            Ok(_) => {
                println!("Successfully updated playlist");
                return Ok(())
            },
            Err(err) => {
                println!("[playlist::update] sqlite3 error {:?}", err);
                return Err(err)
            }
        }

    }

    pub fn delete(conn: &Connection, id: &str) -> Result<()> {
        conn.execute("DELETE FROM PLAYLIST WHERE PLAYLIST_ID=?", [id])?;
        conn.execute("DELETE FROM PLAYLIST_AUDIO_FILE WHERE PLAYLIST_ID=?", [id])?;
        Ok(())
    }

    pub fn view(conn: &Connection, id: &str) -> Result<Playlist> {
        let query = "SELECT * FROM PLAYLIST WHERE PLAYLIST_ID = ?";
        conn.query_row(query, &[&id], |row| {
            Ok(Playlist {
                playlist_id: row.get(0)?,
                playlist_name: row.get(1)?,
                playlist_thumbnail: row.get(2)?,
                date_created: row.get(3)?,
                last_modified: row.get(4)?,
            })
        })
    }

    pub fn add_audio_file(&mut self, conn: &Connection, audio_file_id: usize) -> Result<()> {

        let result = conn.execute(
            "INSERT INTO PLAYLIST_AUDIO_FILE
                (PLAYLIST_ID, AUDIO_FILE_ID)
            VALUES (?1, ?2)
            ",
            [&self.playlist_id, &audio_file_id],
        );

        match result {
            Ok(_) => {
                println!("Successfully added audio file playlist");
                return Ok(())
            },
            Err(err) => {
                println!("[playlist::add_audio_file] sqlite3 error {:?}", err);
                return Err(err)
            }
        }

    }

    pub fn retrieve_audio_files(conn: &Connection, id: &str) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = "SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM PLAYLIST_AUDIO_FILE WHERE PLAYLIST_ID=?);";
        let mut stmt = conn.prepare(query)?;

        let audio_files: Result<Vec<AudioFile>> = stmt
            .query_map([id], |row| {
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

    pub fn search_audio_files(
        conn: &Connection,
        id: &str,
        search_term: &str,
    ) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = format!("SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM PLAYLIST_AUDIO_FILE WHERE PLAYLIST_ID=? AND AUDIO_FILE.FILE_NAME LIKE '%{}%');", search_term);
        let mut stmt = conn.prepare(&query)?;
        let audio_files: Result<Vec<AudioFile>> = stmt
            .query_map([id], |row| {
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

    pub fn remove_audio_file(&self, conn: &Connection, audio_file_id: usize) -> Result<()> {

        let result = conn.execute(
            "DELETE FROM PLAYLIST_AUDIO_FILE
                WHERE PLAYLIST_ID=? AND AUDIO_FILE_ID=?
            ",
            [&self.playlist_id, &audio_file_id],
        );

        match result {
            Ok(_) => {
                println!("Successfully removed audio file from playlist");
                return Ok(())
            },
            Err(err) => {
                println!("[playlist::remove_audio_file] sqlite3 error {:?}", err);
                return Err(err)
            }
        }

        Ok(())
    }

    pub fn search(conn: &Connection, search_term: &str) -> Result<Vec<Playlist>> {
        let query = format!(
            "SELECT * FROM PLAYLIST WHERE PLAYLIST_NAME LIKE '%{}%'",
            search_term
        );
        let mut stmt = conn.prepare(&query)?;
        let playlists: Result<Vec<Playlist>> = stmt
            .query_map([], |row| {
                Ok(Playlist {
                    playlist_id: row.get(0)?,
                    playlist_name: row.get(1)?,
                    playlist_thumbnail: row.get(2)?,
                    date_created: row.get(3)?,
                    last_modified: row.get(4)?,
                })
            })?
            .collect();
        playlists
    }
}
