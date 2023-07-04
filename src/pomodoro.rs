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


    pub fn retrieve_audio_files(&mut self, conn: &Connection) -> Result<Vec<AudioFile>> {

        /* many to many query */ 
        let query = "SELECT * FROM AUDIO_FILE WHERE file_name IN ( 
            SELECT audio_file FROM POMODORO_AUDIO_FILE WHERE pomodoro=?);";
        let mut stmt = conn.prepare(query)?; 
            
        /* return audio files */
        let rows = stmt.query_map([&self.name], |row| {
            Ok(AudioFile {
                file_name: row.get(0)?,
                file_type: row.get(1)?,
                duration: row.get(2)?,
                sample_rate: row.get(3)?,
                date_created: row.get(4)?,
                date_modified: row.get(5)?
            })
        })?;

        /*  store files here */ 
       let mut audio_files = Vec::new();
       for audio_file in rows {
            audio_files.push(audio_file?); 
        }
       Ok(audio_files)
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

    /* test database instance */ 
    static DB_PATH: &str = "db/unit/pomodoro_instance.db";

    #[test]
    fn test_create_and_view_sessions() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
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
        let conn = Connection::open(DB_PATH)?;
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
        let conn = Connection::open(DB_PATH)?;
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

    #[test]
    fn test_add_file_to_session() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true;   

        /* insert dummy session */ 
        let mut my_session : Pomodoro = Pomodoro::new(
            &"the_audio_session",
            60,
            60, 
            10,
            15
        );
        my_session.insert(&conn)?;

        /* insert 5 dummy audio files */ 
        for i in 0..5 {
            let file_value = format!("test_audio_session_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", 1000, 2);
            my_file.insert(&conn)?;
            my_session.add_audio_file(&conn, &my_file)?; 
        }

        /* expected playlist names */
        let mut file_names = Vec::new(); 
        file_names.push("test_audio_session_file_0");
        file_names.push("test_audio_session_file_1");
        file_names.push("test_audio_session_file_2");
        file_names.push("test_audio_session_file_3");
        file_names.push("test_audio_session_file_4");

        /* test retrieving audio file vector */ 
        let mut idx_counter = 0; 
        let audio_files : Vec<AudioFile> = my_session.retrieve_audio_files(&conn)?;
        for item in audio_files {
            if file_names[idx_counter] != &item.file_name {
                equality_status = false;
            }
            idx_counter += 1;
        }


        /* delete entries */
        conn.execute("DELETE FROM POMODORO_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM POMODORO", [])?;
        assert_eq!(equality_status, true); 
        Ok(())
    }


    #[test]
    fn test_remove_file_from_session() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy session */ 
        let mut my_session : Pomodoro = Pomodoro::new(
            &"the_audio_session_remove",
            60,
            60, 
            10,
            15
        );
        my_session.insert(&conn)?;

        /* insert 5 dummy audio files */ 
        for i in 0..5 {
            let file_value = format!("test_remove_audio_file_session_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", 1000, 2);
            my_file.insert(&conn)?;
            my_session.add_audio_file(&conn, &my_file)?; 
        }

        /* expected playlist names */
        let mut file_names = Vec::new(); 
        file_names.push("test_remove_audio_file_session_0");
        file_names.push("test_remove_audio_file_session_1");
        file_names.push("test_remove_audio_file_session_2");

        /* remove file from playlist */ 
        my_session.remove_audio_file(&conn, &"test_remove_audio_file_session_4")?; 
        my_session.remove_audio_file(&conn, &"test_remove_audio_file_session_3")?;
        
        let mut idx_counter = 0;
        let audio_files : Vec<AudioFile> = my_session.retrieve_audio_files(&conn)?;
        for item in audio_files {
            if file_names[idx_counter] != &item.file_name {
                equality_status = false;
            }
            idx_counter += 1;
        }

        /* delete entries */
        conn.execute("DELETE FROM POMODORO_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM POMODORO", [])?;
        assert_eq!(equality_status, true); 
        Ok(())
    }


    #[test]
    fn test_delete_session_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy session */ 
        let mut my_session : Pomodoro = Pomodoro::new(
            &"the_audio_session_remove_test",
            60,
            60, 
            10,
            15
        );
        my_session.insert(&conn)?;

        /* delete the playlist by name */ 
        my_session.delete(&conn)?; 

        let result = conn.execute("SELECT * FROM POMODORO where name =?", [&my_session.name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }

}