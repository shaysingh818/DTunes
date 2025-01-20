pub mod dtunes_api;
pub mod handlers;

use crate::handlers::audio_file::{
    create_audio_file, delete_audio_file, edit_audio_file, play_audio_file, search_audio_files,
    view_audio_file, view_audio_files,
};

use crate::handlers::playlist::{
    add_audio_file_playlist, create_playlist, delete_playlist, edit_playlist,
    remove_audio_file_playlist, search_playlist_audio_files, search_playlists, view_playlist,
    view_playlist_audio_files, view_playlists,
};

use crate::handlers::artist::{
    add_audio_file_artist, create_artist, delete_artist, edit_artist, remove_audio_file_artist,
    search_artist_audio_files, search_artists, view_artist, view_artist_audio_files, view_artists,
};

use crate::handlers::genre::{
    add_audio_file_genre, create_genre, delete_genre, edit_genre, remove_audio_file_genre,
    search_genre_audio_files, search_genres, view_genre, view_genre_audio_files, view_genres,
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
            /* Endpoints for playlist module  */
            create_playlist,
            view_playlists,
            delete_playlist,
            view_playlist,
            edit_playlist,
            view_playlist_audio_files,
            add_audio_file_playlist,
            remove_audio_file_playlist,
            search_playlists,
            search_playlist_audio_files,
            /* Endpoints for artist module  */
            create_artist,
            view_artists,
            delete_artist,
            view_artist,
            edit_artist,
            view_artist_audio_files,
            add_audio_file_artist,
            remove_audio_file_artist,
            search_artists,
            search_artist_audio_files,
            /* Endpoints for genre module  */
            create_genre,
            view_genres,
            delete_genre,
            view_genre,
            edit_genre,
            view_genre_audio_files,
            add_audio_file_genre,
            remove_audio_file_genre,
            search_genres,
            search_genre_audio_files,
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
