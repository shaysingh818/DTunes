use crate::dtunes_api::audio_file::*;
use rusqlite::{Connection, Result};
use std::ffi::OsStr;
use std::fs;
use std::path::Path;
use uuid::Uuid;

#[tauri::command]
pub fn create_audio_file(
    alias_file_name: &str,
    audio_file_name: &str,
    thumbnail_file_name: &str,
    user_local_file_path: &str,
    user_local_thumbnail_path: &str,
    user_thumbnail_path: &str,
    user_audio_file_path: &str,
    user_db_path: &str,
) -> String {
    let file_uuid = Uuid::new_v4();
    let thumbnail_uuid = Uuid::new_v4();

    let file_ext = Path::new(audio_file_name)
        .extension()
        .and_then(OsStr::to_str);

    let thumbnail_ext = Path::new(thumbnail_file_name)
        .extension()
        .and_then(OsStr::to_str);

    let thumbnail_uuid_format = format!("{}.{}", thumbnail_uuid, thumbnail_ext.unwrap());
    let file_uuid_format = format!("{}.{}", file_uuid, file_ext.unwrap());

    let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, thumbnail_uuid_format);
    let usr_file_path = format!("{}/{}", user_audio_file_path, file_uuid_format);

    // Try to copy the thumbnail file and log any errors
    match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
        Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
        Err(e) => eprintln!("Error copying thumbnail: {}", e),
    }

    // Try to copy the audio file and log any errors
    match fs::copy(user_local_file_path, usr_file_path) {
        Ok(bytes) => println!("Successfully copied audio file with {} bytes", bytes),
        Err(e) => eprintln!("Error copying audio file: {}", e),
    }

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    let mut audio_file: AudioFile = AudioFile::new(
        alias_file_name,
        &file_uuid_format,
        &thumbnail_uuid_format,
        0,
        "2",
    );
    audio_file.set_duration(user_audio_file_path).unwrap();

    match audio_file.insert(&conn) {
        Ok(()) => {
            let _ = conn.close();
            return "Success".to_string();
        }
        Err(e) => format!("Failed to insert audio file metadata: {}", e),
    }
}

#[tauri::command]
pub fn edit_audio_file(
    audio_file_id: &str,
    alias_file_name: &str,
    audio_file_name: &str,
    thumbnail_file_name: &str,
    user_local_file_path: &str,
    user_local_thumbnail_path: &str,
    user_thumbnail_path: &str,
    user_audio_file_path: &str,
    user_db_path: &str,
) -> String {
    let usr_file_path = format!("{}/{}", user_audio_file_path, audio_file_name);

    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return e.to_string();
        }
    };

    match AudioFile::view(&conn, audio_file_id) {
        Ok(mut audio_file) => {
            /* update audio file name */
            audio_file.file_name = alias_file_name.to_string();

            /* copy new thumbnail if one is detected */
            if audio_file.thumbnail != thumbnail_file_name {
                println!("Detected new thumbnail: {:?}", thumbnail_file_name);

                let thumbnail_uuid = Uuid::new_v4();
                let thumbnail_ext = Path::new(thumbnail_file_name)
                    .extension()
                    .and_then(OsStr::to_str);

                let thumbnail_uuid_format =
                    format!("{}.{}", thumbnail_uuid, thumbnail_ext.unwrap());
                let usr_thumbnail_path =
                    format!("{}/{}", user_thumbnail_path, thumbnail_uuid_format);

                match fs::copy(user_local_thumbnail_path, usr_thumbnail_path) {
                    Ok(bytes) => println!("Successfully copied thumbnail with {} bytes", bytes),
                    Err(e) => eprintln!("Error copying thumbnail: {}", e),
                }

                /* remove previous thumbnail */
                let thumbnail_delete_path =
                    format!("{}/{}", user_thumbnail_path, audio_file.thumbnail);
                match fs::remove_file(thumbnail_delete_path.clone()) {
                    Ok(result) => {
                        println!(
                            "Successfully removed audio file thumbnail {:?}",
                            thumbnail_delete_path.clone()
                        );
                    }
                    Err(e) => {
                        println!("Error removing audio file thumbnail {:?}", e);
                    }
                }
                audio_file.thumbnail = thumbnail_uuid_format.to_string();
            }

            /* copy new audio file if one is detected */
            if audio_file.file_path != audio_file_name {
                let file_uuid = Uuid::new_v4();
                let file_ext = Path::new(audio_file_name)
                    .extension()
                    .and_then(OsStr::to_str);

                let file_uuid_format = format!("{}.{}", file_uuid, file_ext.unwrap());
                let usr_file_path = format!("{}/{}", user_audio_file_path, file_uuid_format);

                println!("Detected new audio file: {:?}", audio_file_name);
                match fs::copy(user_local_file_path, usr_file_path.clone()) {
                    Ok(bytes) => println!("Successfully copied audio file with {} bytes", bytes),
                    Err(e) => eprintln!("Error copying audio file: {}", e),
                }

                /* remove previous audio file */
                let file_delete_path = format!("{}/{}", user_audio_file_path, audio_file.file_path);
                match fs::remove_file(file_delete_path.clone()) {
                    Ok(result) => {
                        println!(
                            "Successfully removed audio file storage location {:?}",
                            file_delete_path.clone()
                        );
                    }
                    Err(e) => {
                        println!("Error removing audio file storage location {:?}", e);
                    }
                }

                audio_file.file_path = file_uuid_format.to_string();
                audio_file.set_duration(user_audio_file_path).unwrap();
            }

            match audio_file.update(&conn, audio_file_id) {
                Ok(()) => {
                    let _ = conn.close();
                    return "Success".to_string();
                }
                Err(e) => format!("Failed to update audio file metadata: {}", e),
            }
        }
        Err(e) => {
            return "failure".to_string();
        }
    }
}

#[tauri::command]
pub fn view_audio_files(user_db_path: &str) -> Vec<AudioFile> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match AudioFile::retrieve(&conn) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("{:?}", e);
            Vec::new()
        }
    }
}

#[tauri::command]
pub fn delete_audio_file(
    user_db_path: &str,
    audio_file_id: &str,
    user_thumbnail_path: &str,
    user_audio_file_path: &str,
) -> String {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return format!("Database connection error: {:?}", e);
        }
    };

    match AudioFile::view(&conn, audio_file_id) {
        Ok(mut audio_file) => {
            let usr_thumbnail_path = format!("{}/{}", user_thumbnail_path, audio_file.thumbnail);
            let usr_file_path = format!("{}/{}", user_audio_file_path, audio_file.file_path);

            match fs::remove_file(usr_thumbnail_path.clone()) {
                Ok(result) => {
                    println!(
                        "Successfully removed audio file thumbnail {:?}",
                        usr_thumbnail_path.clone()
                    );
                }
                Err(e) => {
                    println!("Error removing audio file thumbnail {:?}", e);
                }
            }

            match fs::remove_file(usr_file_path.clone()) {
                Ok(result) => {
                    println!(
                        "Successfully removed audio file storage location {:?} ",
                        usr_file_path.clone()
                    );
                }
                Err(e) => {
                    println!("Error removing audio file storage location {:?}", e);
                }
            }

            audio_file.delete(&conn, audio_file_id);

            return format!("Success");
        }
        Err(e) => {
            return format!("Error retrieving audio file with id: {:?}", audio_file_id);
        }
    }
}

#[tauri::command]
pub fn view_audio_file(user_db_path: &str, audio_file_id: &str) -> Result<AudioFile, String> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            let err_msg = format!("Database connection error: {:?}", e);
            return Err(err_msg.to_string());
        }
    };

    match AudioFile::view(&conn, audio_file_id) {
        Ok(mut audio_file) => {
            return Ok(audio_file);
        }
        Err(e) => {
            let err_msg = format!("Error retrieving audio file with id: {:?}", audio_file_id);
            return Err(err_msg.to_string());
        }
    }
}

#[tauri::command]
pub fn search_audio_files(user_db_path: &str, search_term: &str) -> Vec<AudioFile> {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => {
            println!("{:?}", e);
            return Vec::new();
        }
    };

    match AudioFile::search(&conn, search_term) {
        Ok(audio_files) => audio_files,
        Err(e) => {
            println!("Error searching audio file: {:?}", e);
            Vec::new()
        }
    }
}

#[tauri::command]
pub fn play_audio_file(user_db_path: &str, audio_file_id: &str) -> String {
    let conn = match Connection::open(user_db_path) {
        Ok(connection) => connection,
        Err(e) => return format!("Failed to open database: {}", e),
    };

    match AudioFile::view(&conn, audio_file_id) {
        Ok(mut audio_file) => {
            audio_file.play(&conn);
            return "Added play".to_string();
        }
        Err(e) => {
            let err_msg = format!("Error retrieving audio file with id: {:?}", audio_file_id);
            return err_msg.to_string();
        }
    }
}
