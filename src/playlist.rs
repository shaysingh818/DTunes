use chrono; 
use rusqlite::{Connection, Result};
use crate::audio_file::AudioFile;
use crate::queue::CircularQueue; 

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
       let mut stmt = conn.prepare("SELECT * FROM PLAYLIST ORDER BY date_modified")?;
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

    pub fn delete(conn: &Connection, name: &str) -> Result<()> {
        conn.execute(
            "DELETE FROM playlist WHERE name=?",
            [&name],
        )?;
        Ok(())
    }


    pub fn view(conn: &Connection, name: &str) -> Result<Playlist> {
        let query = "SELECT * FROM playlist WHERE name = ?";
        conn.query_row(query, &[&name], |row| {
            Ok(Playlist {
                name: row.get(0)?,
                date_created: row.get(1)?,
                date_modified: row.get(2)?,
                disk_space: row.get(3)?,
                file_count: row.get(4)?
            })
        })
    }

    pub fn add_audio_file(&mut self, conn: &Connection, audio_file: &AudioFile) -> Result<()> {
        conn.execute(
            "INSERT INTO PLAYLIST_FILE
                (playlist, audio_file)
            VALUES (?1, ?2)
            ",
            [&self.name, &audio_file.file_name],
        )?;

        Ok(())
    }

    pub fn retrieve_audio_files(&mut self, conn: &Connection) -> Result<Vec<AudioFile>> {

        /* many to many query */ 
        let query = "SELECT * FROM AUDIO_FILE WHERE file_name IN ( 
            SELECT audio_file FROM PLAYLIST_FILE WHERE playlist=?);";
        let mut stmt = conn.prepare(query)?; 
            
        /* return audio files */
        let rows = stmt.query_map([&self.name], |row| {
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

        /*  store files here */ 
       let mut audio_files = Vec::new();
       for audio_file in rows {
            audio_files.push(audio_file?); 
        }
       Ok(audio_files)
    }

    pub fn remove_audio_file(&mut self, conn: &Connection, file_name: &str) -> Result<()> {
        conn.execute(
            "DELETE FROM PLAYLIST_FILE
                WHERE playlist=? AND audio_file=?
            ",
            [&self.name, file_name],
        )?;
        Ok(())
    }

    pub fn queue_playlist_files(&mut self, conn: &Connection) -> Result<()> {

        
        let audio_files : Vec<AudioFile> = self.retrieve_audio_files(&conn).unwrap();  
        let queue_size = audio_files.len(); 
        let mut queue : CircularQueue = CircularQueue::new(queue_size);

        /* load items to queue */ 
        for file in audio_files {
            queue.enqueue(file); 
        }

        println!("Loading {:} files on queue 🔥🔥", queue.items.len()); 

        /* play each song in the queue */ 
        for item in queue.items {
            println!("🎵 Now playing: {:?}", item.file_name); 
            println!("📁 File type: {:?}", item.file_type); 
            println!("📅 Date Created: {:?}", item.date_created);
            println!("✏️ Last Modified: {:?}", item.date_modified);  
            AudioFile::play_wav(&item.storage_path).unwrap();  
        }
        Ok(())
    }

}


#[cfg(test)]
mod playlist_instance {
 
    use crate::playlist::Playlist;
    use crate::audio_file::AudioFile;
    use rusqlite::{Connection, Result};

    /* test database instance */ 
    static DB_PATH: &str = "db/unit/playlist_instance.db";

    #[test]
    fn test_create_and_view_playlists() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
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
        let conn = Connection::open(DB_PATH)?;
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


    #[test]
    fn test_view_playlist_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_playlist : Playlist = Playlist::new("test_view_playlist");
        my_playlist.insert(&conn)?;

        let playlist = Playlist::view(&conn, "test_view_playlist")?; 
        if playlist.name != "test_view_playlist" {
            equality_status = false;
        }

        conn.execute("DELETE FROM PLAYLIST where name =?", [&"test_view_playlist"])?; 
        assert_eq!(equality_status, true); 

        Ok(())
    }


    #[test]
    fn test_delete_playlist_by_name() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */ 
        let mut my_playlist : Playlist = Playlist::new("test_playlist_delete");
        my_playlist.insert(&conn)?;

        /* delete the playlist by name */ 
        Playlist::delete(&conn, "test_playlist_delete")?; 

        let result = conn.execute("SELECT * FROM PLAYLIST where name =?", [&my_playlist.name])?; 
        assert_eq!(result, 1); 

        Ok(())
    }


    #[test]
    fn test_add_file_to_playlist() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_playlist : Playlist = Playlist::new("test_my_playlist");
        my_playlist.insert(&conn)?;

        /* insert 5 dummy audio files */ 
        for i in 0..5 {
            let file_value = format!("test_audio_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
            my_file.insert(&conn)?;
            my_playlist.add_audio_file(&conn, &my_file)?; 
        }

        /* expected playlist names */
        let mut file_names = Vec::new(); 
        file_names.push("test_audio_file_0");
        file_names.push("test_audio_file_1");
        file_names.push("test_audio_file_2");
        file_names.push("test_audio_file_3");
        file_names.push("test_audio_file_4");

        /* test retrieving audio file vector */ 
        let mut idx_counter = 0; 
        let audio_files : Vec<AudioFile> = my_playlist.retrieve_audio_files(&conn)?;
        for item in audio_files {
            if file_names[idx_counter] != &item.file_name {
                equality_status = false;
            }
            idx_counter += 1;
        }


        /* delete entries */
        conn.execute("DELETE FROM PLAYLIST_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM PLAYLIST", [])?;
        assert_eq!(equality_status, true); 
        Ok(())
    }


    #[test]
    fn test_remove_file_from_playlist() -> Result<()> {

        /* Create connection and insert playlist into db  */ 
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true; 

        /* insert dummy playlist */ 
        let mut my_playlist : Playlist = Playlist::new("test_my_playlist_remove");
        my_playlist.insert(&conn)?;

        /* insert 5 dummy audio files */ 
        for i in 0..5 {
            let file_value = format!("test_remove_audio_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
            my_file.insert(&conn)?;
            my_playlist.add_audio_file(&conn, &my_file)?; 
        }

        /* expected playlist names */
        let mut file_names = Vec::new(); 
        file_names.push("test_remove_audio_file_0");
        file_names.push("test_remove_audio_file_1");
        file_names.push("test_remove_audio_file_2");

        /* remove file from playlist */ 
        my_playlist.remove_audio_file(&conn, &"test_remove_audio_file_4")?; 
        my_playlist.remove_audio_file(&conn, &"test_remove_audio_file_3")?;
        
        let mut idx_counter = 0;
        let audio_files : Vec<AudioFile> = my_playlist.retrieve_audio_files(&conn)?;
        for item in audio_files {
            if file_names[idx_counter] != &item.file_name {
                equality_status = false;
            }
            idx_counter += 1;
        }

        /* delete entries */
        conn.execute("DELETE FROM PLAYLIST_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM PLAYLIST", [])?;
        assert_eq!(equality_status, true); 
        Ok(())

    }

}