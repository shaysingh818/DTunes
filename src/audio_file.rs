use soloud::*;
use chrono;
use std::io; 
use std::fs;
use rusqlite::{Connection, Result};

#[derive(Debug)]
pub struct AudioFile {
    pub file_name: String, 
    pub file_type: String, 
    pub duration: usize,
    pub sample_rate: usize,
    pub storage_path: String,
    pub date_created: String,
    pub date_modified: String
}


impl AudioFile {

    pub fn new(
        file_name: &str, file_type: &str, storage_path: &str,
        sample_rate: usize, duration: usize) -> AudioFile {
        AudioFile {
            file_name: String::from(file_name),
            file_type: String::from(file_type),
            duration: duration, 
            sample_rate: sample_rate,
            storage_path: String::from(storage_path),
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string(),
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO audio_file 
                (file_name, file_type, duration, sample_rate, storage_path, date_created, date_modified) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7)",
            [
                &self.file_name,
                &self.file_type,
                &self.duration.to_string(), 
                &self.sample_rate.to_string(),
                &self.storage_path,
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
                file_name: row.get(1)?,
                file_type: row.get(2)?,
                duration: row.get(3)?,
                sample_rate: row.get(4)?,
                storage_path: row.get(5)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
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
                file_type=?, duration=?, sample_rate=?, storage_path=?
                WHERE file_name=?",
            [
                &self.file_name,
                &self.date_created,
                &self.date_modified, 
                &self.file_type,
                &self.duration.to_string(),
                &self.sample_rate.to_string(),
                &self.storage_path,
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


    pub fn view(conn: &Connection, file_name: &str) -> Result<AudioFile> {
        let query = "SELECT * FROM audio_file WHERE file_name = ?";
        conn.query_row(query, &[&file_name], |row| {
            Ok(AudioFile {
                file_name: row.get(1)?,
                file_type: row.get(2)?,
                duration: row.get(3)?,
                sample_rate: row.get(4)?,
                storage_path: row.get(5)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
            })
        })
    }

    pub fn prepare_raw_file_names(dir: &str) -> io::Result<Vec<(String, String)>>  {

        let mut file_names : Vec<(String, String)> = Vec::new(); 
        for entry in fs::read_dir(dir)? {
            let entry = entry?;
            let path = entry.path();
            let path_str = path.display().to_string(); 

            /* remove data/raw prefix */ 
            let removed_path = path_str.replace(dir, "");
            let extension_extract = removed_path.split(".");
            let split_results = extension_extract.collect::<Vec<&str>>(); 
            let extension_index = split_results.len();
            let extension = split_results[extension_index-1]; 
            let file_name = split_results[0]; 
            file_names.push((file_name.to_string(), extension.to_string()));
        }

        Ok(file_names)
    }

    pub fn raw_file_insertion(
        conn: &Connection, 
        files: Vec<(String, String)>,
        input_path: &str,
        output_path: &str) -> Result<()> {

        for item in files { 

            let processed_file_name = item.0.replace(" ", "");
            let old_path = format!("{}{}.{}", input_path, item.0, item.1); 
            let file_name = format!("{}.{}", processed_file_name, item.1); 
            let storage_path = format!("{}{}", output_path, file_name); 

            /* audio file instance */ 
            let mut my_file : AudioFile = AudioFile::new(
                &processed_file_name, 
                &item.1,
                &storage_path, 
                2, 0
            );

            /* move files from raw to storage path */ 
            fs::copy(old_path, storage_path).unwrap(); 
            my_file.insert(&conn)?; 
        }

        Ok(())
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
    use std::path::Path;
    use std::io;

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
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
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
        let mut my_file : AudioFile = AudioFile::new(
            "file_update", 
            "mp3", 
            "storage_path", 1000, 2
        );

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

        /* insert dummy playlist */ 
        let mut my_file : AudioFile = AudioFile::new(
            "test_file", 
            "mp3", 
            "storage_path", 
            1000, 2
        );
        my_file.insert(&conn)?;

        let audio_file = AudioFile::view(&conn, "test_file")?; 
        assert_eq!(audio_file.file_name, my_file.file_name);

        conn.execute("DELETE FROM AUDIO_FILE where file_name =?", [&my_file.file_name])?; 
        Ok(())
    }


    #[test]
    fn test_delete_file_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */ 
        let mut my_file : AudioFile = AudioFile::new(
            "test_file", 
            "mp3", "storage_path",
            1000, 2
        );
        my_file.insert(&conn)?;

        /* delete the playlist by name */ 
        my_file.delete(&conn)?; 

        let result = conn.execute("SELECT * FROM AUDIO_FILE where file_name =?", [&my_file.file_name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }


    #[test]
    fn test_process_raw_file_names() -> io::Result<()>  {

        let expected_file_names = vec![
            "Test 2 -file", 
            "2CELLOS - Whole Lotta Love vs Beethoven 5th Symphony [OFFICIAL VIDEO](720p)",
            "2CELLOS - Pirates Of The Caribbean [OFFICIAL VIDEO]", 
            "Test 1 file"
        ];
        let expected_exts = vec!["txt", "mp3", "wav", "txt"];

        /* test raw file ingestion */ 
        let file_names = AudioFile::prepare_raw_file_names("data/unit/raw/")?;
        assert_eq!(file_names.len(), 4); 

        let mut index = 0; 
        for name in file_names {
            assert_eq!(name.0, expected_file_names[index]);
            assert_eq!(name.1, expected_exts[index]);
            index += 1;  
        }
        
        Ok(())
    }


    #[test]
    fn test_raw_file_ingestion() -> Result<()>  {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let file_names = AudioFile::prepare_raw_file_names("data/unit/raw/").unwrap();  
        assert_eq!(file_names.len(), 4); 

        let input_path = "data/unit/raw/";
        let output_path = "data/audio_files/"; 
       
        /* call raw file ingestion method */ 
        AudioFile::raw_file_insertion(
            &conn, 
            file_names.clone(), 
            input_path,
            output_path
        )?;

        /* check audio files in db */
        let mut idx = 0;  
        let audio_files : Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        for item in &audio_files {
            let expected_file_name = file_names[idx].0.replace(" ", ""); 
            let expected_storage_path = format!(
                "{}{}.{}", 
                output_path, 
                expected_file_name,
                file_names[idx].1
            ); 
            assert_eq!(item.file_name, expected_file_name);
            assert_eq!(item.storage_path, expected_storage_path);
            assert_eq!(item.duration, 0);
            assert_eq!(item.sample_rate, 2);
            
            /* open storage path and make sure it exists */ 
            let result = Path::new(&item.storage_path).exists(); 
            assert_eq!(result, true); 

            idx += 1;  
        }
  
        conn.execute("DELETE FROM AUDIO_FILE", [])?;


        /* delete all files under old path */ 
        //fs::remove_dir_all(input_path).unwrap(); 
        //fs::create_dir(input_path).unwrap(); 

        Ok(())
    }

}

