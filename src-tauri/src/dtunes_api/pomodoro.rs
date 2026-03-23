use crate::dtunes_api::audio_file::AudioFile;
use rusqlite::{Connection, Result};
use serde::{Deserialize, Serialize};
use chrono;

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

        let result = conn.execute(
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
        );

        match result {
            Ok(_) => {
                println!("Successfully inserted pomodoro session");
                self.session_id = conn.last_insert_rowid() as usize; 
                return Ok(())
            },
            Err(err) => {
                println!("[pomodoro::insert] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
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

        let result = conn.execute(
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
        );

        match result {
            Ok(_) => {
                println!("Successfully updated pomodoro session");
                return Ok(())
            },
            Err(err) => {
                println!("[pomodoro::update] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
    }

    pub fn delete(conn: &Connection, id: &str) -> Result<()> {
        let result = conn.execute("DELETE FROM POMODORO_SESSION WHERE SESSION_ID=?", [id]);
        match result {
            Ok(_) => {
                println!("Succesfully deleted pomodoro session"); 
                return Ok(());
            },
            Err(err) => {
                println!("Error Deleting pomodoro session {:?}", err); 
                return Err(err);
            }
        }
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

        let result = conn.execute(
            "INSERT INTO POMODORO_AUDIO_FILE
                (SESSION_ID, AUDIO_FILE_ID)
            VALUES (?1, ?2)
            ",
            [&self.session_id, &audio_file_id],
        );

        match result {
            Ok(_) => {
                println!("Successfully added audio file to pomodoro session");
                return Ok(())
            },
            Err(err) => {
                println!("[pomodoro::add_audio_file] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
    }

    pub fn retrieve_audio_files(conn: &Connection, id: &str) -> Result<Vec<AudioFile>> {
        /* many to many query */
        let query = "SELECT * FROM AUDIO_FILE WHERE AUDIO_FILE_ID IN ( 
            SELECT AUDIO_FILE_ID FROM POMODORO_AUDIO_FILE WHERE SESSION_ID=?);";
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
            SELECT AUDIO_FILE_ID FROM POMODORO_AUDIO_FILE WHERE SESSION_ID=? AND AUDIO_FILE.FILE_NAME LIKE '%{}%');", search_term);
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
            "DELETE FROM POMODORO_AUDIO_FILE
                WHERE SESSION_ID=? AND AUDIO_FILE_ID=?
            ",
            [&self.session_id, &audio_file_id],
        );

        match result {
            Ok(_) => {
                println!("Successfully removed audio file from pomodoro session");
                return Ok(())
            },
            Err(err) => {
                println!("[pomodoro::remove_audio_file] sqlite3 error {:?}", err);
                return Err(err)
            }
        }

    }

    pub fn search(conn: &Connection, search_term: &str) -> Result<Vec<Pomodoro>> {
        let query = format!(
            "SELECT * FROM POMODORO_SESSION WHERE SESSION_NAME LIKE '%{}%'",
            search_term
        );
        let mut stmt = conn.prepare(&query)?;
        let sessions: Result<Vec<Pomodoro>> = stmt
            .query_map([], |row| {
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
            })?
            .collect();
        sessions
    }
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PomodoroSessionTracking {
    pub tracking_session_id: usize,
    pub duration: usize,
    pub date_created: String,
}


#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PomodoroMonthlyUsageResult {
    pub day: String,
    pub hours_per_day: f64,
}


impl PomodoroSessionTracking {

    pub fn new(duration: usize) -> PomodoroSessionTracking {
        PomodoroSessionTracking {
            tracking_session_id: 0,
            duration: duration,
            date_created: chrono::offset::Local::now().to_string(),
        }
    }


    pub fn insert(&mut self, conn: &Connection) -> Result<()> {

        let result = conn.execute(
            "INSERT INTO POMODORO_SESSION_TRACKING 
                (DURATION, DATE_CREATED) 
                VALUES (?1, ?2)",
            [
                &self.duration.to_string(),
                &self.date_created,
            ],
        );

        match result {
            Ok(_) => {
                println!("Successfully inserted pomodoro tracking session");
                return Ok(())
            },
            Err(err) => {
                println!("[pomodoro_tracking_session::insert] sqlite3 error {:?}", err);
                return Err(err)
            }
        }
    }


    pub fn retrieve_monthly_usage(conn: &Connection) -> Result<Vec<PomodoroMonthlyUsageResult>> {
        
        let query = "
            SELECT
                DATE(DATE_CREATED) as DAY,
                ROUND(CAST(SUM(DURATION) AS REAL) / 60.0, 2) AS HOURS_PER_DAY
            FROM POMODORO_SESSION_TRACKING
            WHERE DATE(DATE_CREATED) >= DATE('now', '-30 days')
            GROUP BY DATE(DATE_CREATED)
            ORDER BY DAY;
        ";

        let mut stmt = conn.prepare(query)?;
        let results: Result<Vec<PomodoroMonthlyUsageResult>> = stmt
            .query_map([], |row| {
                Ok(PomodoroMonthlyUsageResult {
                    day: row.get(0)?,
                    hours_per_day: row.get(1)?,
                })
            })?
            .collect();
        results
    }

    pub fn retrieve_total_hours(conn: &Connection) -> Result<f64> {   
        let query = "SELECT (SELECT SUM(DURATION) / 60 FROM POMODORO_SESSION_TRACKING) as total_minutes;";
        conn.query_row(query, [], |row| row.get(0))
    }


    pub fn retrieve_weekly_hours_average(conn: &Connection) -> Result<f64> {
        let query = "
            WITH WEEKLY_HOURS AS (
	            SELECT
	                strftime('%Y-%W', DATE_CREATED) as WEEK,
	                ROUND(CAST(SUM(DURATION) AS REAL) / 60.0, 2) AS HOURS_PER_WEEK
	            FROM POMODORO_SESSION_TRACKING
	            GROUP BY strftime('%Y-%W', DATE_CREATED)
	            ORDER BY WEEK
            )
            SELECT (SELECT ROUND(AVG(HOURS_PER_WEEK), 2) FROM WEEKLY_HOURS) AS WEEKLY_HOURS_AVERAGE; 
        "; 
        conn.query_row(query, [], |row| row.get(0))
    }

    pub fn retrieve_monthly_hours_average(conn: &Connection) -> Result<f64> { 
        let query = "
            WITH MONTHLY_HOURS AS (
	            SELECT
	                strftime('%Y-%m', DATE_CREATED) as MONTH,
	                ROUND(CAST(SUM(DURATION) AS REAL) / 60.0, 2) AS HOURS_PER_MONTH
	            FROM POMODORO_SESSION_TRACKING
	            GROUP BY strftime('%Y-%m', DATE_CREATED)
	            ORDER BY MONTH
            )
            SELECT (SELECT ROUND(AVG(HOURS_PER_MONTH), 2) FROM MONTHLY_HOURS) AS MONTHLY_HOURS_AVERAGE;
        "; 
        conn.query_row(query, [], |row| row.get(0))
    }


    pub fn retrieve_daily_hours_average(conn: &Connection) -> Result<f64> { 
        let query = "
            WITH DAILY_HOURS AS (
                SELECT
                    DATE(DATE_CREATED) as DAY,
                    ROUND(CAST(SUM(DURATION) AS REAL) / 60.0, 2) AS HOURS_PER_DAY
                FROM POMODORO_SESSION_TRACKING
                GROUP BY DATE(DATE_CREATED)
                ORDER BY DAY
            )
            SELECT (SELECT ROUND(AVG(HOURS_PER_DAY), 2) FROM DAILY_HOURS) AS DAILY_HOURS_AVERAGE;
        "; 
        conn.query_row(query, [], |row| row.get(0))
    }

}
