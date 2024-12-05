use dtunes_audio_app_lib::dtunes_api; 

#[cfg(test)]
mod genre_instance {

    use crate::dtunes_api::audio_file::AudioFile;
    use crate::dtunes_api::genre::Genre;
    use rusqlite::{Connection, Result};

    /* test database instance */
    static DB_PATH: &str = "../database/tests/dtunes-test-genres.sqlite3";

    #[test]
    fn test_create_and_view_genre() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;

        for i in 0..5 {
            let genre_value = format!("genre_{}", i);
            let mut my_genre: Genre = Genre::new(&genre_value, "storage_path");
            my_genre.insert(&conn)?;
        }

        let file_names = vec!["genre_0", "genre_1", "genre_2", "genre_3", "genre_4"];

        let genres: Vec<Genre> = Genre::retrieve(&conn)?;
        assert_eq!(genres.len(), 5);

        let mut counter = 0;
        for file in genres {
            assert_eq!(&file.genre_name, &file_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM GENRE", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_update_genre() -> Result<()> {
        /* Create connection and insert playlist into db  */
        let conn = Connection::open(DB_PATH)?;
        let mut my_genre: Genre = Genre::new("genre_name", "thumbnail");
        let modified_time = my_genre.last_modified.clone();
        my_genre.insert(&conn)?;

        /* update playlist */
        my_genre.genre_name = String::from("genre_update_1");
        my_genre.update(&conn, "1")?;

        /* ensure that modified times are different */
        let recent_modified_time = my_genre.last_modified;
        assert_ne!(recent_modified_time, modified_time);
        conn.execute("DELETE FROM GENRE", [])?;
        Ok(())
    }

    #[test]
    fn test_view_genre_by_id() -> Result<()> {
        /* Create connection and insert playlist into db  */
        let conn = Connection::open(DB_PATH)?;
        let mut my_genre: Genre = Genre::new("genre_name", "thumbnail");
        my_genre.insert(&conn)?;

        let my_retrieved_genre = Genre::view(&conn, "1")?;
        assert_eq!(my_retrieved_genre.genre_id, 1);
        assert_eq!(my_retrieved_genre.genre_name, "genre_name");
        conn.execute("DELETE FROM GENRE", [])?;
        Ok(())
    }

    #[test]
    fn test_delete_genre_by_id() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_genre: Genre = Genre::new("genre_name", "thumbnail");
        my_genre.insert(&conn)?;
        let genre = Genre::view(&conn, "1")?;

        assert_eq!(genre.genre_id, 1);
        assert_eq!(genre.genre_name, "genre_name");
        assert_eq!(genre.genre_thumbnail, "thumbnail");

        Genre::delete(&conn, "1")?;
        let result = conn.execute("SELECT * FROM GENRE where GENRE_ID =?", [&"1"])?;
        assert_eq!(result, 1);
        Ok(())
    }

    #[test]
    fn test_add_genre_audio_file() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_genre: Genre = Genre::new("genre_name", "thumbnail");
        my_genre.insert(&conn)?;
        let genre = Genre::view(&conn, "1")?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;

            let audio_file_id = format!("{}", i + 1);
            let audio_file = AudioFile::view(&conn, &audio_file_id)?;
            my_genre.add_audio_file(&conn, audio_file.audio_file_id)?;
        }

        let file_names = vec![
            "audio_file_0",
            "audio_file_1",
            "audio_file_2",
            "audio_file_3",
            "audio_file_4",
        ];

        let mut counter = 0;
        let audio_files = Genre::retrieve_audio_files(&conn, "1")?;
        for item in audio_files {
            assert_eq!(&item.file_name, file_names[counter]);
            assert_eq!(item.audio_file_id, counter + 1);
            counter += 1;
        }

        conn.execute("DELETE FROM GENRE_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM GENRE", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_remove_genre_audio_file() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_genre: Genre = Genre::new("genre_name", "thumbnail");
        my_genre.insert(&conn)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;

            let audio_file_id = format!("{}", i + 1);
            let audio_file = AudioFile::view(&conn, &audio_file_id)?;
            my_genre.add_audio_file(&conn, audio_file.audio_file_id)?;
        }

        my_genre.remove_audio_file(&conn, 3);
        my_genre.remove_audio_file(&conn, 4);
        my_genre.remove_audio_file(&conn, 5);

        let mut counter = 0;
        let audio_files: Vec<AudioFile> = Genre::retrieve_audio_files(&conn, "1")?;
        for item in audio_files {
            assert_eq!(item.audio_file_id, counter + 1);
            counter += 1;
        }

        conn.execute("DELETE FROM GENRE_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM GENRE", [])?;
        let _ = conn.close();
        Ok(())
    }
}
