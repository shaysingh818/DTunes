use std::fs;
use std::path::Path; 
use std::ffi::OsStr;
use uuid::Uuid; 
use rusqlite::{Connection, Result};
use crate::dtunes_api::{audio_file::AudioFile, genre::*};


#[tauri::command]
pub fn create_genre(
    genre_name: &str,
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

    // Try to copy the thumbnail file and log any errors
    match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
        Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
        Err(e) => eprintln!("Error copying thumbnail: {}", e),
    }

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    let mut genre = Genre::new(genre_name, &thumbnail_uuid_format);
    match genre.insert(&conn) {
        Ok(()) => {
            let _ = conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert genre metadata: {}", e),
    }
}


#[tauri::command]
pub fn edit_genre(
    genre_id: &str, 
    genre_name: &str,
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

    match Genre::view(&conn, genre_id) {

        Ok(mut genre) => {
            /* update audio file name */
            genre.genre_name = genre_name.to_string();

            /* copy new thumbnail if one is detected */
            if genre.genre_thumbnail != thumbnail_file_name {
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
                let thumbnail_delete_path = format!("{}/{}", user_thumbnail_path, genre.genre_thumbnail);
                match fs::remove_file(thumbnail_delete_path.clone()) {
                    Ok(result) => {
                        println!("Successfully removed genre thumbnail {:?}", thumbnail_delete_path.clone());
                    },
                    Err(e) => {
                        println!("Error removing genre thumbnail {:?}", e);
                    }
                }
                genre.genre_thumbnail = thumbnail_uuid_format.to_string();

            }

            match genre.update(&conn, genre_id) {
                Ok(()) => {
                    let _ = conn.close();
                    return "Success".to_string();
                }
                Err(e) => format!("Failed to update genre metadata: {}", e),
            }
        },
        Err(e) => {
            return "failure".to_string();
        }

    }

}


#[tauri::command]
pub fn view_genres(user_db_path: &str) -> Vec<Genre> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Genre::retrieve(&conn) {
        Ok(genres) => genres,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub fn delete_genre(
    user_db_path: &str, 
    genre_id: &str,
    user_thumbnail_path: &str) -> String {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return format!("Database connection error: {:?}", e);
        }
    };
    
    match Genre::view(&conn, genre_id) {
        Ok(mut genre) => {

            let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, genre.genre_thumbnail);

            match fs::remove_file(usr_thumbnail_path.clone()) {
                Ok(result) => {
                    println!("Successfully removed genre thumbnail {:?}", usr_thumbnail_path.clone());
                },
                Err(e) => {
                    println!("Error removing genre thumbnail {:?}", e);
                }
            }
            Genre::delete(&conn, genre_id);

            return format!("Success"); 
        },
        Err(e) => {
            return format!("Error retrieving genre with id: {:?}", genre_id);
        }
    }
}


#[tauri::command]
pub fn view_genre(user_db_path: &str, genre_id: &str) -> Result<Genre, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Genre::view(&conn, genre_id) {
        Ok(mut genre) => {
            return Ok(genre);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving genre with id: {:?}", genre_id);
            return Err(err_msg.to_string());
        }
    }
}


#[tauri::command]
pub fn view_genre_audio_files(user_db_path: &str, genre_id: &str) -> Result<Vec<AudioFile>, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Genre::retrieve_audio_files(&conn, genre_id) {
        Ok(mut audio_files) => {
            return Ok(audio_files);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving genre audio files: {:?}", genre_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn add_audio_file_genre(user_db_path: &str, genre_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Genre::view(&conn, genre_id) {
        Ok(mut genre) => {
            genre.add_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving genre with id: {:?}", genre_id);
            return Err(err_msg.to_string());
        }
    }    
}


#[tauri::command]
pub fn remove_audio_file_genre(user_db_path: &str, genre_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Genre::view(&conn, genre_id) {
        Ok(mut genre) => {
            genre.remove_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving genre with id: {:?}", genre_id);
            return Err(err_msg.to_string());
        }
    }    
}

#[tauri::command]
pub fn search_genres(user_db_path: &str, search_term: &str) -> Vec<Genre> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Genre::search(&conn, search_term) {
        Ok(genres) => genres,
        Err(e) => {
            println!("Error searching genres: {:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub fn search_genre_audio_files(
    user_db_path: &str, 
    genre_id: &str, 
    search_term: &str) -> Vec<AudioFile> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Genre::search_audio_files(&conn, genre_id, search_term) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("Error searching audio files for genre: {:?}", e);
            Vec::new()
        }
    }
}