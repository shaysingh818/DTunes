pub mod dtunes_api; 
pub mod handlers; 

use crate::handlers::audio_file::{
    create_audio_file, 
    view_audio_files, 
    delete_audio_file,
    view_audio_file,
    edit_audio_file
};

use crate::handlers::playlist::{
    create_playlist, 
    view_playlists, 
    delete_playlist,
    edit_playlist,
    view_playlist,
    view_playlist_audio_files, 
    add_audio_file_playlist,
    remove_audio_file_playlist
};

use crate::handlers::artist::{
    create_artist, 
    view_artists, 
    delete_artist,
    edit_artist,
    view_artist,
    view_artist_audio_files, 
    add_audio_file_artist,
    remove_audio_file_artist
};

use crate::handlers::genre::{
    create_genre, 
    view_genres, 
    delete_genre,
    edit_genre,
    view_genre,
    view_genre_audio_files, 
    add_audio_file_genre,
    remove_audio_file_genre
};


#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
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

            /* Endpoints for playlist module  */
            create_playlist, 
            view_playlists, 
            delete_playlist,
            view_playlist,
            edit_playlist,
            view_playlist_audio_files, 
            add_audio_file_playlist,
            remove_audio_file_playlist,

            /* Endpoints for artist module  */
            create_artist, 
            view_artists, 
            delete_artist,
            view_artist,
            edit_artist,
            view_artist_audio_files, 
            add_audio_file_artist,
            remove_audio_file_artist,

            /* Endpoints for genre module  */
            create_genre, 
            view_genres, 
            delete_genre,
            view_genre,
            edit_genre,
            view_genre_audio_files, 
            add_audio_file_genre,
            remove_audio_file_genre
            
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
