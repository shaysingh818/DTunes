use chrono;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};
use crate::dtunes_api::audio_file::AudioFile;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Pomodoro {
    pub session_id: usize,
    pub session_name: String,
    pub duration: usize,
    pub duration_limit: usize,
    pub short_break: usize,
    pub long_break: usize,
    pub date_created: String,
    pub last_modified: String,
}

impl Pomodoro {
    pub fn new(
        name: &str,
        duration: usize,
        duration_limit: usize,
        short_break: usize,
        long_break: usize,
    ) -> Pomodoro {
        Pomodoro {
            session_id: 0,
            session_name: String::from(name),
            duration: duration,
            duration_limit: duration_limit,
            short_break: short_break,
            long_break: long_break,
            date_created: chrono::offset::Local::now().to_string(),
            last_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO POMODORO_SESSION 
                (SESSION_NAME, DURATION, DURATION_LIMIT, SHORT_BREAK, LONG_BREAK, DATE_CREATED, LAST_MODIFIED) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7)",
            [
                &self.session_name,
                &self.duration.to_string(),
                &self.duration_limit.to_string(), 
                &self.short_break.to_string(),
                &self.long_break.to_string(),
                &self.date_created,
                &self.last_modified
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Pomodoro>> {
        let mut stmt = conn.prepare("SELECT * FROM POMODORO_SESSION ORDER BY LAST_MODIFIED")?;
        let rows = stmt.query_map([], |row| {
            Ok(Pomodoro {
                session_id: row.get(0)?,
                session_name: row.get(1)?,
                duration: row.get(2)?,
                duration_limit: row.get(3)?,
                short_break: row.get(4)?,
                long_break: row.get(5)?,
                date_created: row.get(6)?,
                last_modified: row.get(7)?,
            })
        })?;

        let mut pomo_sessions = Vec::new();
        for pomo in rows {
            pomo_sessions.push(pomo?);
        }
        Ok(pomo_sessions)
    }

    pub fn update(&mut self, conn: &Connection, id: &str) -> Result<()> {
        /* change date modified */
        self.last_modified = chrono::offset::Local::now().to_string();

        conn.execute(
            "UPDATE POMODORO_SESSION
                SET SESSION_NAME=?, DURATION=?, DURATION_LIMIT=?,
                SHORT_BREAK=?, LONG_BREAK=?, DATE_CREATED=?, LAST_MODIFIED=?
                WHERE SESSION_ID=?",
            [
                &self.session_name,
                &self.duration.to_string(),
                &self.duration_limit.to_string(),
                &self.short_break.to_string(),
                &self.long_break.to_string(),
                &self.date_created,
                &self.last_modified,
                id,
            ],
        )?;
        Ok(())
    }

    pub fn delete(conn: &Connection, id: &str) -> Result<()> {
        conn.execute("DELETE FROM POMODORO_SESSION WHERE SESSION_ID=?", [id])?;
        Ok(())
    }

    pub fn view(conn: &Connection, id: &str) -> Result<Pomodoro> {
        let query = "SELECT * FROM POMODORO_SESSION WHERE SESSION_ID = ?";
        conn.query_row(query, &[&id], |row| {
            Ok(Pomodoro {
                session_id: row.get(0)?,
                session_name: row.get(1)?,
                duration: row.get(2)?,
                duration_limit: row.get(3)?,
                short_break: row.get(4)?,
                long_break: row.get(5)?,
                date_created: row.get(6)?,
                last_modified: row.get(7)?,
            })
        })
    }

    pub fn add_audio_file(&mut self, conn: &Connection, audio_file_id: usize) -> Result<()> {
        conn.execute(
            "INSERT INTO POMODORO_AUDIO_FILE
                (SESSION_ID, AUDIO_FILE_ID)
            VALUES (?1, ?2)
            ",
            [&self.session_id, &audio_file_id],
        )?;

        Ok(())
    }

    pub fn retrieve_audio_files(conn: &Connection, id: &str) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = "SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM POMODORO_AUDIO_FILE WHERE SESSION_ID=?);";
        let mut stmt = conn.prepare(query)?;
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

    pub fn search_audio_files(
        conn: &Connection, 
        id: &str,
        search_term: &str) -> Result<Vec<AudioFile>> {

        /* many to many query */
        let query = format!("SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM POMODORO_AUDIO_FILE WHERE SESSION_ID=? AND AUDIO_FILE.FILE_NAME LIKE '%{}%');", search_term);
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
            "DELETE FROM POMODORO_AUDIO_FILE
                WHERE SESSION_ID=? AND AUDIO_FILE_ID=?
            ",
            [&self.session_id, &audio_file_id],
        )?;
        Ok(())
    }

    pub fn search(conn: &Connection, search_term: &str) -> Result<Vec<Pomodoro>> {
        let query = format!("SELECT * FROM POMODORO_SESSION WHERE SESSION_NAME LIKE '%{}%'", search_term);
        let mut stmt = conn.prepare(&query)?;
        let sessions: Result<Vec<Pomodoro>> = stmt.query_map([], |row| {
            Ok(Pomodoro {
                session_id: row.get(0)?,
                session_name: row.get(1)?,
                duration: row.get(2)?,
                duration_limit: row.get(3)?,
                short_break: row.get(4)?,
                long_break: row.get(5)?,
                date_created: row.get(6)?,
                last_modified: row.get(7)?,
            })
        })?.collect(); 
        sessions
    }
}
