use chrono; 
use rusqlite::{Connection, Result};
use crate::audio_file::AudioFile;

#[derive(Debug)]
pub struct Pomodoro {
    pub name: String, 
    pub duration: usize,
    pub duration_limit: usize,
    pub short_break: usize,
    pub long_break: usize,
    pub date_created: String,
    pub date_modified: String
}


impl Pomodoro {

    pub fn new(
        name: &str, 
        duration: usize, 
        duration_limit: usize,
        short_break: usize,
        long_break: usize) -> Pomodoro {

        Pomodoro {
            name: String::from(name),
            duration: duration,
            duration_limit: duration_limit,
            short_break: short_break, 
            long_break: long_break,
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string()
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO POMODORO 
                (name, duration, duration_limit, short_break, long_break, date_created, date_modified) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7)",
            [
                &self.name,
                &self.duration.to_string(),
                &self.duration_limit.to_string(), 
                &self.short_break.to_string(),
                &self.long_break.to_string(),
                &self.date_created,
                &self.date_modified
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Pomodoro>> {
       let mut stmt = conn.prepare("SELECT * FROM POMODORO")?;
       let rows = stmt.query_map([], |row| {
            Ok(Pomodoro {
                name: row.get(0)?,
                duration: row.get(1)?,
                duration_limit: row.get(2)?,
                short_break: row.get(3)?,
                long_break: row.get(4)?,
                date_created: row.get(5)?,
                date_modified: row.get(6)?
            })
       })?;

       let mut pomo_sessions = Vec::new(); 
       for pomo in rows {
            pomo_sessions.push(pomo?);
       }

       Ok(pomo_sessions)
    }

    pub fn update(&mut self, conn: &Connection) -> Result<()> {

        /* change date modified */ 
        self.date_modified = chrono::offset::Local::now().to_string(); 
        
        conn.execute(
            "UPDATE pomodoro
                SET name=?, duration=?, duration_limit=?,
                short_break=?, long_break=?, date_created=?, date_modified=?
                WHERE name=?",
            [
                &self.name,
                &self.duration.to_string(),
                &self.duration_limit.to_string(), 
                &self.short_break.to_string(),
                &self.long_break.to_string(),
                &self.date_created,
                &self.date_modified,
                &self.name
            ],
        )?;
        Ok(())
    }

    pub fn delete(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "DELETE FROM pomodoro WHERE name=?",
            [&self.name],
        )?;
        Ok(())
    }

    pub fn view(&mut self, conn: &Connection) -> Result<Pomodoro> {
        let query = "SELECT * FROM pomodoro WHERE name = ?";
        conn.query_row(query, &[&self.name], |row| {
            Ok(Pomodoro {
                name: row.get(0)?,
                duration: row.get(1)?,
                duration_limit: row.get(2)?,
                short_break: row.get(3)?,
                long_break: row.get(4)?,
                date_created: row.get(5)?,
                date_modified: row.get(6)?
            })
        })
    }

    pub fn add_audio_file(&mut self, conn: &Connection, audio_file: &AudioFile) -> Result<()> {
        conn.execute(
            "INSERT INTO POMODORO_AUDIO_FILE
                (pomodoro, audio_file)
            VALUES (?1, ?2)
            ",
            [&self.name, &audio_file.file_name],
        )?;

        Ok(())
    }

    pub fn remove_audio_file(&mut self, conn: &Connection, file_name: &str) -> Result<()> {
        conn.execute(
            "DELETE FROM POMODORO_AUDIO_FILE
                WHERE pomodoro=? AND audio_file=?
            ",
            [&self.name, file_name],
        )?;
        Ok(())
    }

}


#[cfg(test)]
mod pomodoro_instance {
 
    use crate::pomodoro::Pomodoro;
    use crate::audio_file::AudioFile;
    use rusqlite::{Connection, Result};

    #[test]
    fn test_create_and_view_playlists() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open("db/dtunes.db")?;
        let mut equality_status = true; 

        /* insert 5 dummy playlists */ 
        for i in 0..5 {
            let name_value = format!("pomodoro_{}", i);
            let mut my_session : Pomodoro = Pomodoro::new(
                &name_value,
                60,
                60, 
                10,
                15
            );
            my_session.insert(&conn)?;
        }

        /* expected playlist names */
        let mut session_names = Vec::new(); 
        session_names.push("pomodoro_0");
        session_names.push("pomodoro_1");
        session_names.push("pomodoro_2");
        session_names.push("pomodoro_3");
        session_names.push("pomodoro_4");

        /* check playlist data */ 
        let sessions : Vec<Pomodoro> = Pomodoro::retrieve(&conn)?;
        let mut counter = 0;  
        for p in sessions {    
            if &p.name != &session_names[counter] {
                equality_status = false;                 
            }
            counter += 1;  
        }


        /* delete playlists and validate */
        conn.execute("DELETE FROM POMODORO", [])?; 
        assert_eq!(equality_status, true); 

        Ok(())

    }

    #[test]
    fn test_update_pomodoro() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open("db/dtunes.db")?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_session : Pomodoro = Pomodoro::new(
            &"session_update",
            60,
            60, 
            10,
            15
        );
        let modified_time = my_session.date_modified.clone(); 
        my_session.insert(&conn)?;

        /* update playlist */ 
        my_session.name = String::from("session_update_1");
        my_session.update(&conn)?;

        /* ensure that modified times are different */ 
        let recent_modified_time = my_session.date_modified;
        if modified_time == recent_modified_time {
            equality_status = false; 
        }

        conn.execute("DELETE FROM POMODORO", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }

    #[test]
    fn test_view_session_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open("db/dtunes.db")?;
        let mut equality_status = true; 

        /* insert dummy session */ 
        let mut my_session : Pomodoro = Pomodoro::new(
            &"the_session",
            60,
            60, 
            10,
            15
        );
        my_session.insert(&conn)?;

        let session = my_session.view(&conn)?; 
        if session.name != my_session.name {
            equality_status = false;
        }

        conn.execute("DELETE FROM POMODORO where name =?", [&my_session.name])?; 
        assert_eq!(equality_status, true); 

        Ok(())
    }

}