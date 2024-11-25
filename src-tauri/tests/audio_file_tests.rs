#[cfg(test)]
mod audio_file_instance {

    use dtunes_audio_app_lib::audio_file::{AudioFile};
    use rusqlite::{Connection, Result};

    /* test database instance */
    static DB_PATH: &str = "../database/tests/dtunes-test-audio-files.sqlite3";

    #[test]
    fn test_create_and_view_audio_file() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;
        }

        let file_names = vec![
            "audio_file_0",
            "audio_file_1",
            "audio_file_2",
            "audio_file_3",
            "audio_file_4",
        ];

        let audio_files: Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 5);

        let mut counter = 0;
        for file in audio_files {
            assert_eq!(&file.file_name, &file_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        let _ = conn.close();
        Ok(())
    }

    #[test]
    fn test_update_audio_file() -> Result<()> {
        /* Create connection and insert playlist into db  */
        let conn = Connection::open(DB_PATH)?;
        let mut equality_status = true;

        /* insert dummy playlist */
        let mut my_file: AudioFile =
            AudioFile::new("file_update", "storage_path", "thumbnail", 10, "2");
        let modified_time = my_file.last_modified.clone();
        my_file.insert(&conn)?;

        /* update playlist */
        my_file.file_name = String::from("file_update_1");
        my_file.update(&conn, "1")?;

        /* ensure that modified times are different */
        let recent_modified_time = my_file.last_modified;
        if modified_time == recent_modified_time {
            equality_status = false;
        }

        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        assert_eq!(equality_status, true);

        Ok(())
    }

    #[test]
    fn test_view_file_by_id() -> Result<()> {
        /* Create connection and insert playlist into db  */
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */
        let mut my_file: AudioFile =
            AudioFile::new("file_update", "storage_path", "thumbnail", 10, "2");
        my_file.insert(&conn)?;

        let my_retrieved_file = AudioFile::view(&conn, "1")?;
        assert_eq!(my_retrieved_file.audio_file_id, 1);
        assert_eq!(my_retrieved_file.file_name, "file_update");
        assert_eq!(my_retrieved_file.file_path, "storage_path");
        assert_eq!(my_retrieved_file.thumbnail, "thumbnail");
        assert_eq!(my_retrieved_file.duration, "");
        assert_eq!(my_retrieved_file.plays, 10);
        assert_eq!(my_retrieved_file.sample_rate, "2");

        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        Ok(())
    }

    #[test]
    fn test_delete_file_by_id() -> Result<()> {
        /* Create connection and insert playlist into db  */
        let conn = Connection::open(DB_PATH)?;

        /* insert dummy playlist */
        let mut my_file: AudioFile =
            AudioFile::new("file_update", "storage_path", "thumbnail", 10, "2");
        my_file.insert(&conn)?;
        my_file.delete(&conn, "1")?;

        let audio_files: Vec<AudioFile> = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 0);

        Ok(())
    }

    #[test]
    fn get_audio_file_duration() {

        let mut my_file_mp3: AudioFile = AudioFile::new(
            "bigswag.mp3", 
            "bigswag.mp3", 
            "thumbnail", 
            10, 
            "2"
        );

        let mut my_file_mp4: AudioFile = AudioFile::new(
            "battlecry.mp4", 
            "battlecry.mp4", 
            "thumbnail", 
            10, 
            "2"
        );

        let mut bad_file: AudioFile = AudioFile::new(
            "file_update", 
            "storage_path", 
            "thumbnail", 
            10, 
            "2"
        );

        let downloads_path = "../database/tests/audio_files";
        my_file_mp3.set_duration(downloads_path).unwrap(); 
        my_file_mp4.set_duration(downloads_path).unwrap(); 

        assert_eq!(my_file_mp3.duration, "660.14");
        assert_eq!(my_file_mp4.duration, "180.651");
        assert_eq!(
            bad_file.set_duration(downloads_path).unwrap_err(),
            "Could not parse extension"
        );

    }
}
