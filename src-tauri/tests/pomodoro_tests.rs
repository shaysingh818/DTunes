

#[cfg(test)]
mod pomodoro_instance {

    use dtunes_audio_app_lib::pomodoro::Pomodoro;
    use dtunes_audio_app_lib::audio_file::AudioFile;
    use rusqlite::{Connection, Result};

    /* test database instance */ 
    static DB_PATH: &str = "../database/storage/dtunes.sqlite3";

    #[test]
    fn test_create_and_view_pomodoro() -> Result<()> {

        let conn = Connection::open(DB_PATH)?;
        
        for i in 0..5 {
            let session_name = format!("pomodoro_{}", i);
            let mut my_session : Pomodoro = Pomodoro::new(&session_name, 0, 0, 0, 0);
            my_session.insert(&conn)?;
        }

        let session_names = vec![
            "pomodoro_0",
            "pomodoro_1",
            "pomodoro_2",
            "pomodoro_3",
            "pomodoro_4"
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

}