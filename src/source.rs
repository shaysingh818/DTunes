use chrono; 
use rusqlite::{Connection, Result};

#[derive(Debug)]
pub struct Source {
    pub source_name: String, 
    pub url: String,
    pub api_key: String,
    pub storage_path: String,
    pub description: String,
    pub status: bool,
    pub date_created: String,
    pub date_modified: String
}


#[derive(Debug)]
pub struct FileType {
    pub extension_name: String, 
    pub date_created: String,
    pub date_modified: String
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
            url: url,
            api_key: api_key,
            storage_path: storage_path, 
            description: description,
            status: false,
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
                &self.status,
                &self.date_created,
                &self.date_modified
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Source>> {
       let mut stmt = conn.prepare("SELECT * FROM SOURCE")?;
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
                &self.status,
                &self.date_created,
                &self.date_modified,
                &self.name
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

    pub fn view(&mut self, conn: &Connection) -> Result<Source> {
        let query = "SELECT * FROM SOURCE WHERE source_name = ?";
        conn.query_row(query, &[&self.name], |row| {
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
       let mut stmt = conn.prepare("SELECT * FROM FILE_TYPE")?;
       let rows = stmt.query_map([], |row| {
            Ok(FileType {
                extension_name: row.get(0)?,
                date_created: row.get(6)?,
                date_modified: row.get(7)?
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

}