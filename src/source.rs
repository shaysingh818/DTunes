use chrono; 
use rusqlite::{Connection, Result};
use serde::{Deserialize};
use std::fs::File; 
use std::io::{Read};


#[derive(Debug)]
pub struct Source {
    pub source_name: String, 
    pub url: String,
    pub api_key: String,
    pub storage_path: String,
    pub description: String,
    pub status: usize,
    pub date_created: String,
    pub date_modified: String
}

#[derive(Debug)]
pub struct FileType {
    pub extension_name: String, 
    pub date_created: String,
    pub date_modified: String
}

#[derive(Debug, Deserialize)]
struct DataSource {
    pub name: String,
    pub description: String,
    pub base_path: String,
    pub sources: Vec<ConfigSource>,
}

#[derive(Debug, Deserialize)]
struct ConfigSource {
    pub source_name: String,
    pub url: String,
    pub api_key: String,
    pub description: String,
    pub storage_location: String,
    pub file_types: Vec<String>,
}


impl Source {

    pub fn new(
        source_name: &str, 
        url: &str,
        api_key: &str,
        storage_path: &str,
        description: &str) -> Source {

        Source {
            source_name: String::from(source_name),
            url: String::from(url),
            api_key: String::from(api_key),
            storage_path: String::from(storage_path), 
            description: String::from(description),
            status: 0,
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string()
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO SOURCE 
                (source_name, url, api_key, storage_path, description, status, date_created, date_modified) 
                VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8)",
            [
                &self.source_name,
                &self.url,
                &self.api_key, 
                &self.storage_path,
                &self.description,
                &self.status.to_string(),
                &self.date_created,
                &self.date_modified
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Source>> {
       let mut stmt = conn.prepare("SELECT * FROM SOURCE ORDER BY date_modified")?;
       let rows = stmt.query_map([], |row| {
            Ok(Source {
                source_name: row.get(0)?,
                url: row.get(1)?,
                api_key: row.get(2)?,
                storage_path: row.get(3)?,
                description: row.get(4)?,
                status: row.get(5)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
            })
       })?;

       let mut sources = Vec::new(); 
       for source in rows {
            sources.push(source?);
       }

       Ok(sources)
    }

    pub fn update(&mut self, conn: &Connection) -> Result<()> {

        /* change date modified */ 
        self.date_modified = chrono::offset::Local::now().to_string(); 
        
        conn.execute(
            "UPDATE SOURCE
                SET source_name=?, url=?, api_key=?, storage_path=?,
                description=?, status=?, date_created=?, date_modified=?
                WHERE source_name=?",
            [
                &self.source_name,
                &self.url,
                &self.api_key, 
                &self.storage_path,
                &self.description,
                &self.status.to_string(),
                &self.date_created,
                &self.date_modified,
                &self.source_name
            ],
        )?;
        Ok(())
    }

    pub fn delete(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "DELETE FROM SOURCE WHERE source_name=?",
            [&self.source_name],
        )?;
        Ok(())
    }

    pub fn view(conn: &Connection, source_name: &str) -> Result<Source> {
        let query = "SELECT * FROM SOURCE WHERE source_name = ?";
        conn.query_row(query, &[&source_name], |row| {
            Ok(Source { 
                source_name: row.get(0)?,
                url: row.get(1)?,
                api_key: row.get(2)?,
                storage_path: row.get(3)?,
                description: row.get(4)?,
                status: row.get(5)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
            })
        })
    }

    pub fn exists(conn: &Connection, source_name: &str) -> Result<bool> {  
        let query = "SELECT * FROM SOURCE where source_name = ?"; 
        let mut stmt = conn.prepare(query)?;
        let rows = stmt.query_map([&source_name], |row| {
            Ok(Source {
                source_name: row.get(0)?,
                url: row.get(1)?,
                api_key: row.get(2)?,
                storage_path: row.get(3)?,
                description: row.get(4)?,
                status: row.get(5)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
            })
        })?;

        if rows.count() > 0 {
            return Ok(true)
        }
        Ok(false)
    }


    pub fn add_file_type(&mut self, conn: &Connection, extension_name: &str) -> Result<()> {
        conn.execute(
            "INSERT INTO SOURCE_FILE_TYPE
                (source, file_type)
            VALUES (?1, ?2)
            ",
            [&self.source_name, extension_name],
        )?;

        Ok(())
    }


    pub fn sync_config_file(conn: &Connection, config: &str) -> Result<()>  {

        /* read config file */ 
        let mut file = File::open(config).expect("Failed to open the file");
        let mut contents = String::new();
        file.read_to_string(&mut contents).expect("Failed to read the file");

        /* read into data sources schema */ 
        let data_sources: DataSource = serde_yaml::from_str(&contents).expect("Failed to deserialize");
        let name_condition = data_sources.name.len() > 0; 
        let description_condition = data_sources.description.len() > 0;
        let base_path_condition = data_sources.base_path.len() > 0;  
        
        if !name_condition || !description_condition || !base_path_condition {
            println!("Name and description missing from config file\n");    
        }

        /* Read sources from file line by line */ 
        for source in data_sources.sources {

            /* Insert unique sources */ 
            if Source::exists(&conn, &source.source_name) == Ok(false) {
                let mut db_source : Source = Source::new(
                    &source.source_name,
                    &source.url,
                    &source.api_key,
                    &source.storage_location,
                    &source.description
                );

                db_source.insert(&conn)?;

                /* Check for duplicate file types */ 
                for file_type in source.file_types {
                    if FileType::exists(&conn, &file_type) == Ok(false)  {
                        let mut db_file_type : FileType = FileType::new(&file_type);
                        db_file_type.insert(&conn)?;
                    }

                    let mut source_instance = Source::view(&conn, &db_source.source_name)?;
                    source_instance.add_file_type(&conn, &file_type)?; 
                }
            }
        }

        Ok(())

    }

}

impl FileType {

    pub fn new(extension_name: &str) -> FileType {
        FileType {
            extension_name: String::from(extension_name),
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string()
        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO FILE_TYPE 
                (extension_name, date_created, date_modified) 
                VALUES (?1, ?2, ?3)",
            [
                &self.extension_name,
                &self.date_created,
                &self.date_modified
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<FileType>> {
       let mut stmt = conn.prepare("SELECT * FROM FILE_TYPE ORDER BY date_modified")?;
       let rows = stmt.query_map([], |row| {
            Ok(FileType {
                extension_name: row.get(0)?,
                date_created: row.get(1)?,
                date_modified: row.get(2)?
            })
       })?;

       let mut file_types = Vec::new(); 
       for file_type in rows {
            file_types.push(file_type?);
       }

       Ok(file_types)
    }

    pub fn update(&mut self, conn: &Connection) -> Result<()> {

        /* change date modified */ 
        self.date_modified = chrono::offset::Local::now().to_string(); 
        
        conn.execute(
            "UPDATE FILE_TYPE
                SET extension_name=?, date_created=?, date_modified=?
                WHERE extension_name=?",
            [
                &self.extension_name,
                &self.date_created,
                &self.date_modified,
                &self.extension_name
            ],
        )?;
        Ok(())
    }

    pub fn delete(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "DELETE FROM FILE_TYPE WHERE extension_name=?",
            [&self.extension_name],
        )?;
        Ok(())
    }

    pub fn view(&mut self, conn: &Connection) -> Result<FileType> {
        let query = "SELECT * FROM FILE_TYPE WHERE extension_name = ?";
        conn.query_row(query, &[&self.extension_name], |row| {
            Ok(FileType { 
                extension_name: row.get(0)?,
                date_created: row.get(1)?,
                date_modified: row.get(2)?
            })
        })
    }

    pub fn exists(conn: &Connection, extension_name: &str) -> Result<bool> {  
        let query = "SELECT * FROM FILE_TYPE where extension_name = ?"; 
        let mut stmt = conn.prepare(query)?;    
        let rows = stmt.query_map([&extension_name], |row| {
            Ok(FileType {
                extension_name: row.get(0)?,
                date_created: row.get(1)?,
                date_modified: row.get(2)?
            })
        })?;

        if rows.count() > 0 {
            return Ok(true)
        }
        Ok(false)
    }


}



#[cfg(test)]
mod source_instance {
 
    use crate::source::{Source, FileType};
    use rusqlite::{Connection, Result};

    /* test database instance */ 
    static DB_PATH: &str = "db/unit/source_instance.db";


    #[test]
    fn test_create_and_view_sources() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert 5 dummy playlists */ 
        for i in 0..5 {
            let source_value = format!("source_{}", i);
            let mut my_source : Source = Source::new(
                &source_value,
                &"www.youtube.com",
                &"378429738294",
                &"dtunes/sources/youtube",
                &"Files coming from youtube",
            );
            my_source.insert(&conn)?;
        }

        /* expected playlist names */
        let mut source_names = Vec::new(); 
        source_names.push("source_0");
        source_names.push("source_1");
        source_names.push("source_2");
        source_names.push("source_3");
        source_names.push("source_4");

        /* check playlist data */ 
        let sources : Vec<Source> = Source::retrieve(&conn)?;
        let mut counter = 0;  
        for s in sources {    
            assert_eq!(&s.source_name, &source_names[counter]); 
            counter += 1;  
        }

        /* delete playlists and validate */
        conn.execute("DELETE FROM SOURCE", [])?; 
        Ok(()) 

    }


    #[test]
    fn test_update_source() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy source */ 
        let mut my_source : Source = Source::new(
            &"youtube-test",
            &"www.youtube.com",
            &"378429738294",
            &"dtunes/sources/youtube",
            &"Files coming from youtube",
        );
        let modified_time = my_source.date_modified.clone(); 
        my_source.insert(&conn)?;

        /* update playlist */ 
        my_source.source_name = String::from("youtube");
        my_source.update(&conn)?;

        /* ensure that modified times are different */ 
        let recent_modified_time = my_source.date_modified;
        if modified_time == recent_modified_time {
            equality_status = false; 
        }

        conn.execute("DELETE FROM SOURCE", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }

    #[test]
    fn test_view_source_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy source */ 
        let mut my_source : Source = Source::new(
            &"spotify-test-2",
            &"www.youtube.com",
            &"378429738294",
            &"dtunes/sources/spotify",
            &"Files coming from spotify",
        );
        my_source.insert(&conn)?;

        let source = Source::view(&conn, "spotify-test-2")?; 
        assert_eq!(source.source_name, my_source.source_name); 

        conn.execute("DELETE FROM SOURCE where source_name =?", [&source.source_name])?; 
        Ok(())
    }


    #[test]
    fn test_delete_source_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */ 
        let mut my_source : Source = Source::new(
            &"spotify-test",
            &"www.youtube.com",
            &"378429738294",
            &"dtunes/sources/spotify",
            &"Files coming from spotify",
        );
        my_source.insert(&conn)?;

        /* delete the playlist by name */ 
        my_source.delete(&conn)?; 

        let result = conn.execute("SELECT * FROM SOURCE where source_name =?", [&my_source.source_name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }


    #[test]
    fn test_create_and_view_file_types() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert 5 dummy playlists */ 
        for i in 0..5 {
            let file_value = format!("mp3_{}", i);
            let mut file_type : FileType = FileType::new(&file_value);
            file_type.insert(&conn)?;
        }

        /* expected playlist names */
        let mut type_names = Vec::new(); 
        type_names.push("mp3_0");
        type_names.push("mp3_1");
        type_names.push("mp3_2");
        type_names.push("mp3_3");
        type_names.push("mp3_4");

        /* check playlist data */ 
        let types : Vec<FileType> = FileType::retrieve(&conn)?;
        let mut counter = 0;  
        for t in types {    
            if &t.extension_name != &type_names[counter] {
                equality_status = false;                 
            }
            counter += 1;  
        }

        /* delete playlists and validate */
        conn.execute("DELETE FROM FILE_TYPE", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }


    #[test]
    fn test_update_file_type() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy file type */ 
        let mut file_type : FileType = FileType::new("mp4");
        let modified_time = file_type.date_modified.clone(); 
        file_type.insert(&conn)?;

        /* update playlist */ 
        file_type.extension_name = String::from("wav");
        file_type.update(&conn)?;

        /* ensure that modified times are different */ 
        let recent_modified_time = file_type.date_modified;
        if modified_time == recent_modified_time {
            equality_status = false; 
        }

        conn.execute("DELETE FROM FILE_TYPE", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }

    #[test]
    fn test_view_file_type_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy file type */ 
        let mut file_type : FileType = FileType::new("mp5");
        file_type.insert(&conn)?;

        let file = file_type.view(&conn)?; 
        if file.extension_name != file_type.extension_name {
            equality_status = false;
        }

        conn.execute("DELETE FROM FILE_TYPE where extension_name =?", [&file.extension_name])?; 
        assert_eq!(equality_status, true); 

        Ok(())
    }

    #[test]
    fn test_delete_file_type_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy file type */ 
        let mut file_type : FileType = FileType::new("mp5");
        file_type.insert(&conn)?;

        /* delete the playlist by name */ 
        file_type.delete(&conn)?; 

        let result = conn.execute("SELECT * FROM FILE_TYPE WHERE extension_name =?", [&file_type.extension_name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }

    #[test]
    fn test_sync_config_file() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        Source::sync_config_file(&conn, "config/data_sources.yaml")?;  
        
        let yt_source = Source::exists(&conn, &"YouTube")?;
        let soundcloud_source = Source::exists(&conn, &"SoundCloud")?;

        assert_eq!(yt_source, true); 
        assert_eq!(soundcloud_source, true); 

        let expcted_file_types = vec!["mp3", "mp4", "wav", "Flacc"]; 
        for file_type in expcted_file_types {
            let file_type_check = FileType::exists(&conn, &file_type)?;
            assert_eq!(file_type_check, true);        
        }
        
        conn.execute("DELETE FROM SOURCE_FILE_TYPE", [])?; 
        conn.execute("DELETE FROM FILE_TYPE", [])?; 
        conn.execute("DELETE FROM SOURCE", [])?; 

        Ok(())
    }

}

