use dtunes_audio_app_lib::dtunes_api;

#[cfg(test)]
mod pomodoro_instance {

    use crate::dtunes_api::audio_file::AudioFile;
    use crate::dtunes_api::pomodoro::Pomodoro;
    use rusqlite::{Connection, Result};

    /* test database instance */
    static DB_PATH: &str = "../database/tests/dtunes-test-pomodoro.sqlite3";

    #[test]
    fn test_create_and_view_pomodoro() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;

        for i in 0..5 {
            let session_name = format!("pomodoro_{}", i);
            let mut my_session: Pomodoro = Pomodoro::new(&session_name, 0, 0, 0, 0);
            my_session.insert(&conn)?;
        }

        let session_names = vec![
            "pomodoro_0",
            "pomodoro_1",
            "pomodoro_2",
            "pomodoro_3",
            "pomodoro_4",
        ];

        let sessions: Vec<Pomodoro> = Pomodoro::retrieve(&conn)?;
        assert_eq!(sessions.len(), 5);

        let mut counter = 0;
        for session in sessions {
            assert_eq!(&session.session_name, &session_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM POMODORO_SESSION", [])?;
        conn.close();
        Ok(())
    }

    #[test]
    fn test_view_pomdoro_by_id() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        let mut my_session: Pomodoro = Pomodoro::new("test_session", 0, 0, 0, 0);
        my_session.insert(&conn)?;

        let session = Pomodoro::view(&conn, &my_session.session_id.to_string())?;

        assert_eq!(session.session_name, "test_session");

        conn.execute("DELETE FROM POMODORO_SESSION", [])?;
        conn.close();
        Ok(())
    }

    #[test]
    fn test_delete_pomdoro_by_id() -> Result<()> {
        let conn = Connection::open(DB_PATH)?;
        let mut my_session: Pomodoro = Pomodoro::new("test_session", 0, 0, 0, 0);
        my_session.insert(&conn)?;

        let session = Pomodoro::view(&conn, &my_session.session_id.to_string())?;
        assert_eq!(session.session_name, "test_session");

        Pomodoro::delete(&conn, &my_session.session_id.to_string())?;

        let sessions = Pomodoro::retrieve(&conn)?; 
        assert_eq!(sessions.len(), 0); 
        Ok(())
    }

    #[test]
    fn test_add_pomodoro_audio_file() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        let mut my_session: Pomodoro = Pomodoro::new("test_session", 0, 0, 0, 0);
        my_session.insert(&conn)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;
        }

        let audio_files = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 5);        

        for audio_file in audio_files {
            my_session.add_audio_file(&conn, audio_file.audio_file_id);
        }

        let file_names = vec![
            "audio_file_0",
            "audio_file_1",
            "audio_file_2",
            "audio_file_3",
            "audio_file_4",
        ];

        let mut counter = 0;
        let audio_files = Pomodoro::retrieve_audio_files(&conn, &my_session.session_id.to_string())?;
        for item in audio_files {
            assert_eq!(&item.file_name, file_names[counter]);
            counter += 1;
        }

        conn.execute("DELETE FROM POMODORO_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM POMODORO_SESSION", [])?;
        Ok(())
    }

    #[test]
    fn test_remove_pomodoro_audio_file() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        let mut my_session: Pomodoro = Pomodoro::new("test_session", 0, 0, 0, 0);
        my_session.insert(&conn)?;

        for i in 0..5 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file: AudioFile =
                AudioFile::new(&file_value, "storage_path", "thumbnail", 0, "2");
            my_file.insert(&conn)?;
        }

        let audio_files = AudioFile::retrieve(&conn)?;
        assert_eq!(audio_files.len(), 5);        

        for audio_file in &audio_files {
            my_session.add_audio_file(&conn, audio_file.audio_file_id);
        }

        my_session.remove_audio_file(&conn, audio_files[0].audio_file_id);
        my_session.remove_audio_file(&conn, audio_files[1].audio_file_id);

        let audio_files: Vec<AudioFile> = Pomodoro::retrieve_audio_files(&conn, &my_session.session_id.to_string())?;
        assert_eq!(audio_files.len(), 3); 

        conn.execute("DELETE FROM POMODORO_AUDIO_FILE", [])?;
        conn.execute("DELETE FROM AUDIO_FILE", [])?;
        conn.execute("DELETE FROM POMODORO_SESSION", [])?;
        Ok(())
    }
}
