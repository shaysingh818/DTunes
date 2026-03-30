pub mod dtunes_api;
pub mod handlers;

use std::sync::Mutex;
use tauri::{Builder, Manager}; 

use crate::handlers::player::{
    PlayerState,
    play_audio,
    pause_audio,
    stop_audio,
    is_playing,
    is_paused,
    curr_duration,
    forward_duration,
    rewind_duration,
};

use crate::handlers::audio_file::{
    create_audio_file, 
    delete_audio_file, 
    edit_audio_file, 
    play_audio_file, 
    search_audio_files,
    view_audio_file, 
    view_audio_files,
};

use crate::handlers::pomodoro::{
    add_audio_file_pomodoro, 
    create_pomodoro, 
    delete_pomodoro_session, 
    edit_pomodoro,
    remove_audio_file_pomodoro, 
    search_pomodoro_audio_files, 
    search_pomodoro_sessions,
    view_pomodoro_audio_files, 
    view_pomodoro_session, 
    view_pomodoro_sessions,

    /*
    play_pomodoro_alarm_sound, */ 

    create_pomodoro_tracking_session,
    retrieve_pomodoro_tracking_monthly_usage,
    retrieve_pomodoro_tracking_total_hours, 
    retrieve_pomodoro_tracking_monthly_hours_average,
    retrieve_pomodoro_tracking_weekly_hours_average,
    retrieve_pomodoro_tracking_daily_hours_average,
};

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    Builder::default()
        .setup(|app| {
            app.manage(Mutex::new(PlayerState::new()));
            Ok(())
        })
        .plugin(tauri_plugin_os::init())
        .plugin(tauri_plugin_fs::init())
        .plugin(tauri_plugin_dialog::init())
        .plugin(tauri_plugin_shell::init())
        .invoke_handler(tauri::generate_handler![

            /* endpoints for audio player */ 
            play_audio,
            pause_audio,
            stop_audio,
            is_playing,
            is_paused,
            curr_duration,
            forward_duration,
            rewind_duration,

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
            //play_pomodoro_alarm_sound,

            /* Pomodoro Tracking Session Endpoints */ 
            create_pomodoro_tracking_session,
            retrieve_pomodoro_tracking_monthly_usage,
            retrieve_pomodoro_tracking_total_hours,
            retrieve_pomodoro_tracking_monthly_hours_average,
            retrieve_pomodoro_tracking_weekly_hours_average,
            retrieve_pomodoro_tracking_daily_hours_average,
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
