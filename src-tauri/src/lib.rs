pub mod artist;
pub mod audio_file;
pub mod genre;
pub mod playlist;
pub mod pomodoro;

use crate::audio_file::{
    create_audio_file, 
    view_audio_files, 
    read_image_from_data_dir,
    delete_audio_file
};

// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let port: u16 = 9527;
    tauri::Builder::default()
        .plugin(tauri_plugin_fs::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![
            greet,
            create_audio_file,
            view_audio_files,
            delete_audio_file,
            read_image_from_data_dir
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
