use std::fs;
use std::path::Path; 
use std::ffi::OsStr;
use uuid::Uuid; 
use rusqlite::{Connection, Result};
use crate::dtunes_api::{artist::*, audio_file::AudioFile};


#[tauri::command]
pub fn create_artist(
    artist_name: &str,
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

    let mut artist = Artist::new(artist_name, &thumbnail_uuid_format);
    match artist.insert(&conn) {
        Ok(()) => {
            let _ = conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert artist metadata: {}", e),
    }
}


#[tauri::command]
pub fn edit_artist(
    artist_id: &str, 
    artist_name: &str,
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

    match Artist::view(&conn, artist_id) {

        Ok(mut artist) => {
            /* update audio file name */
            artist.artist_name = artist_name.to_string();

            /* copy new thumbnail if one is detected */
            if artist.artist_thumbnail != thumbnail_file_name {
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
                let thumbnail_delete_path = format!("{}/{}", user_thumbnail_path, artist.artist_thumbnail);
                match fs::remove_file(thumbnail_delete_path.clone()) {
                    Ok(result) => {
                        println!("Successfully removed playlist thumbnail {:?}", thumbnail_delete_path.clone());
                    },
                    Err(e) => {
                        println!("Error removing playlist thumbnail {:?}", e);
                    }
                }
                artist.artist_thumbnail = thumbnail_uuid_format.to_string();

            }

            match artist.update(&conn, artist_id) {
                Ok(()) => {
                    let _ = conn.close();
                    return "Success".to_string();
                }
                Err(e) => format!("Failed to update artist metadata: {}", e),
            }
        },
        Err(e) => {
            return "failure".to_string();
        }

    }

}


#[tauri::command]
pub fn view_artists(user_db_path: &str) -> Vec<Artist> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match Artist::retrieve(&conn) {
        Ok(artists) => artists,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}


#[tauri::command]
pub fn delete_artist(
    user_db_path: &str, 
    artist_id: &str,
    user_thumbnail_path: &str) -> String {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return format!("Database connection error: {:?}", e);
        }
    };
    
    match Artist::view(&conn, artist_id) {
        Ok(mut artist) => {

            let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, artist.artist_thumbnail);

            match fs::remove_file(usr_thumbnail_path.clone()) {
                Ok(result) => {
                    println!("Successfully removed artist thumbnail {:?}", usr_thumbnail_path.clone());
                },
                Err(e) => {
                    println!("Error removing artist thumbnail {:?}", e);
                }
            }
            Artist::delete(&conn, artist_id);

            return format!("Success"); 
        },
        Err(e) => {
            return format!("Error retrieving artist with id: {:?}", artist_id);
        }
    }
}


#[tauri::command]
pub fn view_artist(user_db_path: &str, artist_id: &str) -> Result<Artist, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Artist::view(&conn, artist_id) {
        Ok(mut artist) => {
            return Ok(artist);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving artist with id: {:?}", artist_id);
            return Err(err_msg.to_string());
        }
    }
}


#[tauri::command]
pub fn view_artist_audio_files(user_db_path: &str, artist_id: &str) -> Result<Vec<AudioFile>, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };
    
    match Artist::retrieve_audio_files(&conn, artist_id) {
        Ok(mut audio_files) => {
            return Ok(audio_files);
        },
        Err(e) => {
            let err_msg = format!("Error retrieving artist audio files: {:?}", artist_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn add_audio_file_artist(user_db_path: &str, artist_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Artist::view(&conn, artist_id) {
        Ok(mut artist) => {
            artist.add_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving artist with id: {:?}", artist_id);
            return Err(err_msg.to_string());
        }
    }    
}


#[tauri::command]
pub fn remove_audio_file_artist(user_db_path: &str, artist_id: &str, audio_file_id: usize) -> Result<String, String> {

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string()); 
        }
    };

    match Artist::view(&conn, artist_id) {
        Ok(mut artist) => {
            artist.remove_audio_file(&conn, audio_file_id).unwrap();
            return Ok("Success".to_string());
        },
        Err(e) => {
            let err_msg = format!("Error retrieving artist with id: {:?}", artist_id);
            return Err(err_msg.to_string());
        }
    }    
}