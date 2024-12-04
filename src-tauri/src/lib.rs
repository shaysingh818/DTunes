pub mod dtunes_api; 
pub mod handlers; 

use crate::handlers::audio_file::{
    create_audio_file, 
    view_audio_files, 
    delete_audio_file,
    view_audio_file,
    edit_audio_file
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
            view_audio_file,
            edit_audio_file
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
