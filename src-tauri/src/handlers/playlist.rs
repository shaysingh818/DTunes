use std::fs;
use std::path::Path; 
use std::ffi::OsStr;
use uuid::Uuid; 
use rusqlite::{Connection, Result};
use crate::dtunes_api::playlist::*;
use crate::dtunes_api::audio_file::*; 


#[tauri::command]
pub fn create_playlist(
    playlist_name: &str,
    thumbnail_file_name: &str,
    user_local_thumbnail_path: &str,
    user_thumbnail_path: &str,
    user_db_path: &str,
) -> String {

    let thumbnail_uuid = Uuid::new_v4();
    let thumbnail_ext = Path::new(thumbnail_file_name)
        .extension()
        .and_then(OsStr::to_str);

    let thumbnail_uuid_format = format!("{}.{}", thumbnail_uuid, thumbnail_ext.unwrap());
    let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, thumbnail_uuid_format);

    println!("USR THUMBNAIL PATH: {:?}", usr_thumbnail_path);
    println!("USR LOCAL THUMBNAIL: {:?}", user_local_thumbnail_path);

    // Try to copy the thumbnail file and log any errors
    match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
        Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
        Err(e) => eprintln!("Error copying thumbnail: {}", e),
    }

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    let mut playlist = Playlist::new(playlist_name, &thumbnail_uuid_format);
    match playlist.insert(&conn) {
        Ok(()) => {
            let _ = conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert playlist metadata: {}", e),
    }
}


#[tauri::command]
pub fn edit_playlist(
    playlist_id: &str, 
    playlist_name: &str,
    thumbnail_file_name: &str,
    user_local_thumbnail_path: &str,
    user_thumbnail_path: &str,
    user_db_path: &str,
) -> String {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return e.to_string();
        }
    };

    match Playlist::view(&conn, playlist_id) {

        Ok(mut playlist) => {
            /* update audio file name */
            playlist.playlist_name = playlist_name.to_string();

            /* copy new thumbnail if one is detected */
            if playlist.playlist_thumbnail != thumbnail_file_name {
                println!("Detected new thumbnail: {:?}", thumbnail_file_name);

                let thumbnail_uuid = Uuid::new_v4();
                let thumbnail_ext = Path::new(thumbnail_file_name)
                    .extension()
                    .and_then(OsStr::to_str);

                let thumbnail_uuid_format = format!("{}.{}", thumbnail_uuid, thumbnail_ext.unwrap());
                let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, thumbnail_uuid_format);

                match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
                    Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
                    Err(e) => eprintln!("Error copying thumbnail: {}", e),
                }

                /* remove previous thumbnail */
                let thumbnail_delete_path = format!("{}/{}", user_thumbnail_path, playlist.playlist_thumbnail);
                match fs::remove_file(thumbnail_delete_path.clone()) {
                    Ok(result) => {
                        println!("Successfully removed playlist thumbnail {:?}", thumbnail_delete_path.clone());
                    },
                    Err(e) => {
                        println!("Error removing playlist thumbnail {:?}", e);
                    }
                }
                playlist.playlist_thumbnail = thumbnail_uuid_format.to_string();

            }

            match playlist.update(&conn, playlist_id) {
                Ok(()) => {
                    let _ = conn.close();
                    return "Success".to_string();
                }
                Err(e) => format!("Failed to update playlist metadata: {}", e),
            }
        },
        Err(e) => {
            return "failure".to_string();
        }

    }

}


#[tauri::command]
pub fn view_playlists(user_db_path: &str) -> Vec<Playlist> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Playlist::retrieve(&conn) {
        Ok(playlists) => playlists,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub fn delete_playlist(
    user_db_path: &str, 
    playlist_id: &str,
    user_thumbnail_path: &str) -> String {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return format!("Database connection error: {:?}", e);
        }
    };
    
    match Playlist::view(&conn, playlist_id) {
        Ok(mut playlist) => {

            let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, playlist.playlist_thumbnail);

            match fs::remove_file(usr_thumbnail_path.clone()) {
                Ok(result) => {
                    println!("Successfully removed playlist thumbnail {:?}", usr_thumbnail_path.clone());
                },
                Err(e) => {
                    println!("Error removing playlist thumbnail {:?}", e);
                }
            }
            Playlist::delete(&conn, playlist_id);

            return format!("Success"); 
        },
        Err(e) => {
            return format!("Error retrieving playlist with id: {:?}", playlist_id);
        }
    }
}


#[tauri::command]
pub fn view_playlist(user_db_path: &str, playlist_id: &str) -> Result<Playlist, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Playlist::view(&conn, playlist_id) {
        Ok(mut playlist) => {
            return Ok(playlist);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving playlist with id: {:?}", playlist_id);
            return Err(err_msg.to_string());
        }
    }
}


#[tauri::command]
pub fn view_playlist_audio_files(user_db_path: &str, playlist_id: &str) -> Result<Vec<AudioFile>, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Playlist::retrieve_audio_files(&conn, playlist_id) {
        Ok(mut audio_files) => {
            return Ok(audio_files);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving playlist audio files: {:?}", playlist_id);
            return Err(err_msg.to_string());
        }
    }
}


#[tauri::command]
pub fn add_audio_file_playlist(user_db_path: &str, playlist_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Playlist::view(&conn, playlist_id) {
        Ok(mut playlist) => {
            playlist.add_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving playlist with id: {:?}", playlist_id);
            return Err(err_msg.to_string());
        }
    }    
}


#[tauri::command]
pub fn remove_audio_file_playlist(user_db_path: &str, playlist_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Playlist::view(&conn, playlist_id) {
        Ok(mut playlist) => {
            playlist.remove_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving playlist with id: {:?}", playlist_id);
            return Err(err_msg.to_string());
        }
    }    
}


#[tauri::command]
pub fn search_playlists(user_db_path: &str, search_term: &str) -> Vec<Playlist> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Playlist::search(&conn, search_term) {
        Ok(playlists) => playlists,
        Err(e) => {
            println!("Error searching playlists: {:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub fn search_playlist_audio_files(
    user_db_path: &str, 
    playlist_id: &str, 
    search_term: &str) -> Vec<AudioFile> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Playlist::search_audio_files(&conn, playlist_id, search_term) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("Error searching audio files for playlist: {:?}", e);
            Vec::new()
        }
    }
}