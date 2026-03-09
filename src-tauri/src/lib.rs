pub mod dtunes_api;
pub mod handlers;

use crate::handlers::audio_file::{
    create_audio_file, delete_audio_file, edit_audio_file, play_audio_file, search_audio_files,
    view_audio_file, view_audio_files,
};

use crate::handlers::pomodoro::{
    add_audio_file_pomodoro, create_pomodoro, delete_pomodoro_session, edit_pomodoro,
    remove_audio_file_pomodoro, search_pomodoro_audio_files, search_pomodoro_sessions,
    view_pomodoro_audio_files, view_pomodoro_session, view_pomodoro_sessions, play_pomodoro_alarm_sound
};

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_os::init())
        .plugin(tauri_plugin_fs::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![

            /* endpoints for audio file module */
            create_audio_file,
            view_audio_files,
            delete_audio_file,
            view_audio_file,
            edit_audio_file,
            search_audio_files,
            play_audio_file,

            /* Pomodoro Endpoints */
            create_pomodoro,
            edit_pomodoro,
            view_pomodoro_sessions,
            delete_pomodoro_session,
            view_pomodoro_session,
            view_pomodoro_audio_files,
            add_audio_file_pomodoro,
            remove_audio_file_pomodoro,
            search_pomodoro_sessions,
            search_pomodoro_audio_files,
            play_pomodoro_alarm_sound
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
