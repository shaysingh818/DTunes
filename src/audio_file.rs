use soloud::*;
use chrono; 
use rusqlite::{Connection, Result};

#[derive(Debug)]
pub struct AudioFile {
    pub file_name: String, 
    pub file_type: String, 
    pub duration: usize,
    pub sample_rate: usize,
    pub date_created: String,
    pub date_modified: String
}


impl AudioFile {

    pub fn new(
        file_name: &str, file_type: &str, 
        sample_rate: usize, duration: usize) -> AudioFile {
        AudioFile {
            file_name: String::from(file_name),
            file_type: String::from(file_type),
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string(),
            duration: duration, 
            sample_rate: sample_rate,
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO audio_file 
                (file_name, file_type, duration, sample_rate, date_created, date_modified) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6)",
            [
                &self.file_name,
                &self.file_type,
                &self.duration.to_string(), 
                &self.sample_rate.to_string(),
                &self.date_created,
                &self.date_modified, 
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<AudioFile>> {
       let mut stmt = conn.prepare("SELECT * FROM AUDIO_FILE ORDER BY date_modified")?;
       let rows = stmt.query_map([], |row| {
            Ok(AudioFile {
                file_name: row.get(0)?,
                file_type: row.get(1)?,
                duration: row.get(2)?,
                sample_rate: row.get(3)?,
                date_created: row.get(4)?,
                date_modified: row.get(5)?
            })
       })?;

       let mut audio_files = Vec::new(); 
       for file in rows {
            audio_files.push(file?);
       }

       Ok(audio_files)
    }

    pub fn update(&mut self, conn: &Connection) -> Result<()> {

        /* change date modified */ 
        self.date_modified = chrono::offset::Local::now().to_string(); 
        
        conn.execute(
            "UPDATE AUDIO_FILE
                SET file_name=?, date_created=?, date_modified=?,
                file_type=?, duration=?, sample_rate=?
                WHERE file_name=?",
            [
                &self.file_name,
                &self.date_created,
                &self.date_modified, 
                &self.file_type,
                &self.duration.to_string(),
                &self.sample_rate.to_string(),
                &self.file_name
            ],
        )?;
        Ok(())
    }


    pub fn delete(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "DELETE FROM audio_file WHERE file_name=?",
            [&self.file_name],
        )?;
        Ok(())
    }


    fn view(&mut self, conn: &Connection) -> Result<AudioFile> {
        let query = "SELECT * FROM audio_file WHERE file_name = ?";
        conn.query_row(query, &[&self.file_name], |row| {
            Ok(AudioFile {
                file_name: row.get(0)?,
                file_type: row.get(1)?,
                duration: row.get(2)?,
                sample_rate: row.get(3)?,
                date_created: row.get(4)?,
                date_modified: row.get(5)?
            })
        })
    }



    pub fn play_wav(filepath: &str) -> Result<(), Box<dyn std::error::Error>> {

        let sl = Soloud::default()?; 
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



}


#[cfg(test)]
mod audio_file_instance {
 
    use crate::audio_file::AudioFile;
    use rusqlite::{Connection, Result};

    /* test database instance */ 
    static DB_PATH: &str = "db/unit/audio_file_instance.db";

    #[test]
    fn test_create_and_view_audio_file() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert 5 dummy playlists */ 
        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", 1000, 2);
            my_file.insert(&conn)?;
        }

        /* expected playlist names */
        let mut file_names = Vec::new(); 
        file_names.push("audio_file_0");
        file_names.push("audio_file_1");
        file_names.push("audio_file_2");
        file_names.push("audio_file_3");
        file_names.push("audio_file_4");

        /* check playlist data */ 
        let audio_files : Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        let mut counter = 0;  
        for file in audio_files {    
            if &file.file_name != &file_names[counter] {
                equality_status = false;                 
            }
            counter += 1;  
        }

        /* delete playlists and validate */
        conn.execute("DELETE FROM AUDIO_FILE", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 

    }

    #[test]
    fn test_update_audio_file() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_file : AudioFile = AudioFile::new("file_update", "mp3", 1000, 2);
        let modified_time = my_file.date_modified.clone(); 
        my_file.insert(&conn)?;

        /* update playlist */ 
        my_file.file_name = String::from("file_update_1");
        my_file.update(&conn)?;

        /* ensure that modified times are different */ 
        let recent_modified_time = my_file.date_modified;
        if modified_time == recent_modified_time {
            equality_status = false; 
        }

        conn.execute("DELETE FROM AUDIO_FILE", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }


    #[test]
    fn test_view_file_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_file : AudioFile = AudioFile::new("test_file", "mp3", 1000, 2);
        my_file.insert(&conn)?;

        let audio_file = my_file.view(&conn)?; 
        if audio_file.file_name != my_file.file_name {
            equality_status = false;
        }

        conn.execute("DELETE FROM AUDIO_FILE where file_name =?", [&my_file.file_name])?; 
        assert_eq!(equality_status, true); 

        Ok(())
    }


    #[test]
    fn test_delete_file_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */ 
        let mut my_file : AudioFile = AudioFile::new("test_file", "mp3", 1000, 2);
        my_file.insert(&conn)?;

        /* delete the playlist by name */ 
        my_file.delete(&conn)?; 

        let result = conn.execute("SELECT * FROM AUDIO_FILE where file_name =?", [&my_file.file_name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }


}

