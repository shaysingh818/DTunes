use chrono; 
use rusqlite::{Connection, Result};

#[derive(Debug)]
pub struct Playlist {
    pub name: String, 
    pub date_created: String,
    pub date_modified: String,
    pub file_count: usize,
    pub disk_space: usize 
}


impl Playlist {

    pub fn new(name: &str) -> Playlist {
        Playlist {
            name: String::from(name),
            date_created: chrono::offset::Local::now().to_string(),
            date_modified: chrono::offset::Local::now().to_string(),
            file_count: 0, 
            disk_space: 0

        }
    }

    pub fn insert(&mut self, conn: &Connection) -> Result<()> {
        conn.execute(
            "INSERT INTO playlist 
                (name, date_created, date_modified, file_count, disk_space) 
                VALUES (?1, ?2, ?3, ?4, ?5)",
            [
                &self.name,
                &self.date_created,
                &self.date_modified, 
                &self.file_count.to_string(),
                &self.disk_space.to_string()
            ],
        )?;
        Ok(())
    }

    pub fn retrieve(conn: &Connection) -> Result<Vec<Playlist>> {
       let mut stmt = conn.prepare("SELECT * FROM PLAYLIST")?;
       let rows = stmt.query_map([], |row| {
            Ok(Playlist {
                name: row.get(0)?,
                date_created: row.get(1)?,
                date_modified: row.get(2)?,
                disk_space: row.get(3)?,
                file_count: row.get(4)?
            })
       })?;

       let mut playlists = Vec::new(); 
       for playlist in rows {
            playlists.push(playlist?);
       }

       Ok(playlists)
    }

    pub fn update(&mut self, conn: &Connection) -> Result<()> {

        /* change date modified */ 
        self.date_modified = chrono::offset::Local::now().to_string(); 
        
        conn.execute(
            "UPDATE playlist
                SET name=?, date_created=?, date_modified=?,
                file_count=?, disk_space=?
                WHERE name=?",
            [
                &self.name,
                &self.date_created,
                &self.date_modified, 
                &self.file_count.to_string(),
                &self.disk_space.to_string(),
                &self.name
            ],
        )?;
        Ok(())
    }



}


#[cfg(test)]
mod playlist_instance {
 
    use crate::playlist::Playlist;
    use rusqlite::{Connection, Result};

    #[test]
    fn test_create_and_view_playlists() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open("db/dtunes.db")?;
        let mut equality_status = true; 

        /* insert 5 dummy playlists */ 
        for i in 0..5 {
            let playlist_value = format!("playlist_{}", i);
            let mut my_playlist : Playlist = Playlist::new(&playlist_value);
            my_playlist.insert(&conn)?;
        }

        /* expected playlist names */
        let mut playlist_names = Vec::new(); 
        playlist_names.push("playlist_0");
        playlist_names.push("playlist_1");
        playlist_names.push("playlist_2");
        playlist_names.push("playlist_3");
        playlist_names.push("playlist_4");

        /* check playlist data */ 
        let playlists : Vec<Playlist> = Playlist::retrieve(&conn)?;
        let mut counter = 0;  
        for p in playlists {    
            if &p.name != &playlist_names[counter] {
                equality_status = false;                 
            }
            counter += 1;  
        }

        /* delete playlists and validate */
        conn.execute("DELETE FROM PLAYLIST", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 

    }

    #[test]
    fn test_update_playlist() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open("db/dtunes.db")?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_playlist : Playlist = Playlist::new("playlist_update");
        let modified_time = my_playlist.date_modified.clone(); 
        my_playlist.insert(&conn)?;

        /* update playlist */ 
        my_playlist.name = String::from("playlist_update_1");
        my_playlist.update(&conn)?;

        /* ensure that modified times are different */ 
        let recent_modified_time = my_playlist.date_modified;
        if modified_time == recent_modified_time {
            equality_status = false; 
        }

        conn.execute("DELETE FROM PLAYLIST", [])?; 
        assert_eq!(equality_status, true); 

        Ok(()) 
    }


}