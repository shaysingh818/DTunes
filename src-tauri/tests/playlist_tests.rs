use dtunes_audio_app_lib::dtunes_api;

#[cfg(test)]
mod playlist_instance {

    use crate::dtunes_api::audio_file::AudioFile;
    use crate::dtunes_api::playlist::Playlist;
    use rusqlite::{Connection, Result};

    /* test database instance */
    static DB_PATH: &str = "../database/tests/dtunes-test-playlists.sqlite3";

    #[test]
    fn test_create_and_view_playlist() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;

        for i in 0..5 {
            let playlist_name = format!("playlist_{}", i);
            let mut my_playlist: Playlist = Playlist::new(&playlist_name, "thumbnail");
            my_playlist.insert(&conn)?;
        }

        let playlist_names = vec![
            "playlist_0",
            "playlist_1",
            "playlist_2",
            "playlist_3",
            "playlist_4",
        ];

        let playlists: Vec<Playlist> = Playlist::retrieve(&conn)?;
        assert_eq!(playlists.len(), 5);

        let mut counter = 0;
        for playlist in playlists {
            assert_eq!(&playlist.playlist_name, &playlist_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM PLAYLIST", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_view_playlist_by_id() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_playlist: Playlist = Playlist::new("test_playlist", "thumbnail");
        my_playlist.insert(&conn)?;
        let playlist = Playlist::view(&conn, &my_playlist.playlist_id.to_string())?;

        assert_eq!(playlist.playlist_name, "test_playlist");

        conn.execute("DELETE FROM PLAYLIST", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_delete_playlist_by_id() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_playlist: Playlist = Playlist::new("test_playlist", "thumbnail");
        my_playlist.insert(&conn)?;

        let playlist = Playlist::view(&conn, &my_playlist.playlist_id.to_string())?;
        assert_eq!(playlist.playlist_name, "test_playlist");

        Playlist::delete(&conn, &my_playlist.playlist_id.to_string())?;

        let playlists = Playlist::retrieve(&conn)?; 
        assert_eq!(playlists.len(), 0); 
        Ok(())
    }

    #[test]
    fn test_add_playlist_audio_file() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        let mut my_playlist: Playlist = Playlist::new("test_playlist", "thumbnail");
        my_playlist.insert(&conn)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;
        }

        let audio_files = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 5);  
        
        for audio_file in audio_files {
            my_playlist.add_audio_file(&conn, audio_file.audio_file_id);
        }

        let file_names = vec![
            "audio_file_0",
            "audio_file_1",
            "audio_file_2",
            "audio_file_3",
            "audio_file_4",
        ];

        let mut counter = 0;
        let audio_files = Playlist::retrieve_audio_files(&conn, &my_playlist.playlist_id.to_string())?;
        for item in audio_files {
            assert_eq!(&item.file_name, file_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM PLAYLIST_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM PLAYLIST", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_remove_playlist_audio_file() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_playlist: Playlist = Playlist::new("test_playlist", "thumbnail");
        my_playlist.insert(&conn)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;
        }

        let audio_files = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 5);  
        
        for audio_file in &audio_files {
            my_playlist.add_audio_file(&conn, audio_file.audio_file_id);
        }

        my_playlist.remove_audio_file(&conn, audio_files[0].audio_file_id);
        my_playlist.remove_audio_file(&conn, audio_files[1].audio_file_id);

        let audio_files: Vec<AudioFile> = Playlist::retrieve_audio_files(&conn, &my_playlist.playlist_id.to_string())?;
        assert_eq!(audio_files.len(), 3); 

        conn.execute("DELETE FROM PLAYLIST_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM PLAYLIST", [])?;
        let _ = conn.close();
        Ok(())
    }
}
