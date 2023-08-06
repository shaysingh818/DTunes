CREATE TABLE PLAYLIST (
	name VARCHAR(255) UNIQUE PRIMARY KEY,
	date_created VARCHAR(255),
	date_modified VARCHAR(255),
	disk_space INT, 
	file_count INT,
	play_count INT
);

CREATE TABLE AUDIO_FILE (	
    file_id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT,
    file_name VARCHAR(255) NOT NULL UNIQUE,
	file_type VARCHAR(255) NOT NULL,
	duration INT,
    sample_rate INT,
    storage_path VARCHAR(1000), 
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);

CREATE TABLE POMODORO (
	name VARCHAR(255) UNIQUE PRIMARY KEY,
	duration INT NOT NULL,
	duration_limit INT NOT NULL,
	short_break INT NOT NULL,
	long_break INT NOT NULL,
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);

CREATE TABLE ARTIST (
    artist_id INTEGER PRIMARY KEY AUTOINCREMENT,
	first_name VARCHAR(255),
	last_name VARCHAR(255),
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);

CREATE TABLE GENRE (
    genre_id INTEGER PRIMARY KEY AUTOINCREMENT,
	genre_name VARCHAR(255) NOT NULL,
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);


/* relationships */ 
CREATE TABLE PLAYLIST_FILE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    playlist VARCHAR(255) NOT NULL,
    audio_file VARCHAR(255) NOT NULL,
    FOREIGN KEY(playlist) REFERENCES PLAYLIST(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(file_name)
);


CREATE TABLE POMODORO_AUDIO_FILE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    pomodoro VARCHAR(100) NOT NULL,
    audio_file VARCHAR(255) NOT NULL,
    FOREIGN KEY(pomodoro) REFERENCES POMODORO(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(file_name)
);


CREATE TABLE AUDIO_FILE_ARTIST (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    audio_file_id INTEGER NOT NULL,
    artist_id INTEGER NOT NULL,
    FOREIGN KEY(audio_file_id) REFERENCES AUDIO_FILE(file_id),
    FOREIGN KEY(artist_id) REFERENCES ARTIST(artist_id)
);


CREATE TABLE AUDIO_FILE_GENRE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    audio_file_id INTEGER NOT NULL,
    genre_id INTEGER NOT NULL,
    FOREIGN KEY(audio_file_id) REFERENCES AUDIO_FILE(file_id),
    FOREIGN KEY(genre_id) REFERENCES GENRE(genre_id)
);