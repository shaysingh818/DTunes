use rusqlite::{Connection, Result};
use crate::db::*; 

#[derive(Debug)]
pub struct Playlist {
    id: String, 
    name: String, 
    date: String,
    file_count: String 
}


pub fn create_playlist_table() -> Result<()> {

    let conn = Connection::open(DB_PATH)?;

    conn.execute(
        "CREATE TABLE IF NOT EXISTS playlist (
            id integer primary key, 
            name text not null unique,
            date_created TIMESTAMP default CURRENT_TIMESTAMP,
            file_count integer
        )",
        [],
    )?;
    
    println!("Playlist: table has been created ");

    Ok(())    

}

pub fn insert_playlist(name: &str) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    conn.execute(INSERT_PLAYLIST, &[name, "0"])?; 
    
    match conn {
        Err(e) => println!("{:?}", e),
         _ => () 
    }

    Ok(())    
    
}


pub fn view_playlists(playlist_result: &mut Vec<Playlist>) -> Result<()> {

    let conn = Connection::open(DB_PATH)?;
    let mut stmt = conn.prepare(VIEW_PLAYLISTS)?;

    let playlists = stmt.query_map([], |row| {
        Ok(Playlist {
            id: row.get(0)?,
            name: row.get(1)?,
            date: row.get(2)?, 
            file_count: row.get(3)?
        })
    })?;

    for p in playlists {
        playlist_result.push(p.unwrap()); 
    }

    Ok(())
}



#[cfg(test)]
mod playlist_db {

    use crate::playlist::*;
    use rusqlite::{Connection, Result};

    /* tests for the board instance */

    #[test]
    fn test_create_playlist_table() {

        let result = create_playlist_table(); 

        match result {
            Err(e) => println!("{:?}", e),
            _ => () 
        }

    }

    #[test]
    fn test_insert_playlist() -> Result<()> {

        let playlist_names : Vec<String> = Vec::new(); 
        playlist_names.push("testing"); 
        playlist_names.push("testing1"); 
        playlist_names.push("testing2"); 
        playlist_names.push("testing3"); 

        let result = insert_playlist("testing"); 

        match result {
            Err(e) => println!("{:?}", e),
            _ => () 
        }

        /* query database */
        let conn = Connection::open("db/dtunes.db")?;
        let mut stmt = conn.prepare("SELECT * FROM playlist")?;

        let playlists = stmt.query_map([], |row| {
            Ok(Playlist {
                id: row.get(0)?,
                name: row.get(1)?,
                date: row.get(2)?, 
                file_count: row.get(3)?
            })
        })?;

        for p in playlists {
            let playlist : Playlist = p.unwrap(); 
            assert_eq!(playlist.name, "testing"); 
        }

        Ok(())    
    }

}