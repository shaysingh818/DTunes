
pub const DB_PATH : &str = "db/dtunes.db";

/* playlist queries */ 
pub const INSERT_PLAYLIST : &str = "INSERT INTO playlist (name, file_count) values (?1, ?2)"; 
pub const VIEW_PLAYLISTS : &str = "SELECT * FROM playlist"; 


