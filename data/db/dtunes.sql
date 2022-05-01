/** Current DB schema for DTUNES */

CREATE TABLE YOUTUBE_URL (
    url_uuid VARCHAR(37) PRIMARY KEY,
    yt_url VARCHAR(255) UNIQUE,
    date_created VARCHAR(255)
);

CREATE TABLE PLAYLIST (
    playlist_uuid VARCHAR(37) PRIMARY KEY,
    name VARCHAR(255),
    date_created VARCHAR(255)
);


CREATE TABLE SONG (
    song_uuid VARCHAR(37) PRIMARY KEY,
    name VARCHAR(255),
    date_created VARCHAR(255),
    file_path VARCHAR(255),
    subtitles VARCHAR(1000),
    plays INT
);


CREATE TABLE PLAYLIST_SONG (
    relation_id INTEGER PRIMARY KEY,
    playlist VARCHAR(37),
    song VARCHAR(37),
    FOREIGN KEY(playlist) REFERENCES PLAYLIST(playlist_uuid),
    FOREIGN KEY(song) REFERENCES SONG(song_uuid)
);


