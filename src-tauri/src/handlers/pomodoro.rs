use crate::dtunes_api::audio_file::*;
use crate::dtunes_api::pomodoro::*;
use rusqlite::{Connection, Result};
use std::ffi::OsStr;
use std::fs;
use std::path::Path;
use uuid::Uuid;

#[tauri::command]
pub fn create_pomodoro(
    user_db_path: &str,
    session_name: &str,
    duration: usize,
    duration_limit: usize,
    short_break: usize,
    long_break: usize,
) -> String {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    let mut pomodoro = Pomodoro::new(
        session_name,
        duration,
        duration_limit,
        short_break,
        long_break,
    );

    match pomodoro.insert(&conn) {
        Ok(()) => {
            let _ = conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert playlist metadata: {}", e),
    }
}

#[tauri::command]
pub fn edit_pomodoro(
    user_db_path: &str,
    session_id: &str,
    session_name: &str,
    duration: usize,
    duration_limit: usize,
    short_break: usize,
    long_break: usize,
) -> String {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return e.to_string();
        }
    };

    match Pomodoro::view(&conn, session_id) {
        Ok(mut pomodoro) => {
            /* update audio file name */
            pomodoro.session_name = session_name.to_string();
            pomodoro.duration = duration;
            pomodoro.duration_limit = duration_limit;
            pomodoro.short_break = short_break;
            pomodoro.long_break = long_break;

            match pomodoro.update(&conn, session_id) {
                Ok(()) => {
                    let _ = conn.close();
                    return "Success".to_string();
                }
                Err(e) => format!("Failed to update playlist metadata: {}", e),
            }
        }
        Err(e) => {
            return "failure".to_string();
        }
    }
}

#[tauri::command]
pub fn view_pomodoro_sessions(user_db_path: &str) -> Vec<Pomodoro> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Pomodoro::retrieve(&conn) {
        Ok(sessions) => sessions,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}

#[tauri::command]
pub fn delete_pomodoro_session(user_db_path: &str, session_id: &str) -> String {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return format!("Database connection error: {:?}", e);
        }
    };

    match Pomodoro::view(&conn, session_id) {
        Ok(mut pomodoro) => {
            Pomodoro::delete(&conn, session_id);
            return format!("Success");
        }
        Err(e) => {
            return format!("Error retrieving playlist with id: {:?}", session_id);
        }
    }
}

#[tauri::command]
pub fn view_pomodoro_session(user_db_path: &str, session_id: &str) -> Result<Pomodoro, String> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string());
        }
    };

    match Pomodoro::view(&conn, session_id) {
        Ok(mut session) => {
            return Ok(session);
        }
        Err(e) => {
            let err_msg = format!("Error retrieving session with id: {:?}", session_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn view_pomodoro_audio_files(
    user_db_path: &str,
    session_id: &str,
) -> Result<Vec<AudioFile>, String> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string());
        }
    };

    match Pomodoro::retrieve_audio_files(&conn, session_id) {
        Ok(mut audio_files) => {
            return Ok(audio_files);
        }
        Err(e) => {
            let err_msg = format!("Error retrieving pomodoro audio files: {:?}", session_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn add_audio_file_pomodoro(
    user_db_path: &str,
    session_id: &str,
    audio_file_id: usize,
) -> Result<String, String> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string());
        }
    };

    match Pomodoro::view(&conn, session_id) {
        Ok(mut pomodoro) => {
            pomodoro.add_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        }
        Err(e) => {
            let err_msg = format!("Error retrieving pomodoro with id: {:?}", session_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn remove_audio_file_pomodoro(
    user_db_path: &str,
    session_id: &str,
    audio_file_id: usize,
) -> Result<String, String> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string());
        }
    };

    match Pomodoro::view(&conn, session_id) {
        Ok(mut pomodoro) => {
            pomodoro.remove_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        }
        Err(e) => {
            let err_msg = format!("Error retrieving pomodoro with id: {:?}", session_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn search_pomodoro_sessions(user_db_path: &str, search_term: &str) -> Vec<Pomodoro> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Pomodoro::search(&conn, search_term) {
        Ok(sessions) => sessions,
        Err(e) => {
            println!("Error searching pomodoro: {:?}", e);
            Vec::new()
        }
    }
}

#[tauri::command]
pub fn search_pomodoro_audio_files(
    user_db_path: &str,
    session_id: &str,
    search_term: &str,
) -> Vec<AudioFile> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Pomodoro::search_audio_files(&conn, session_id, search_term) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("Error searching audio files for pomodoro sessions: {:?}", e);
            Vec::new()
        }
    }
}
