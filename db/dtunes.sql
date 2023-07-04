CREATE TABLE PLAYLIST (
	name VARCHAR(255) UNIQUE PRIMARY KEY,
	date_created VARCHAR(255),
	date_modified VARCHAR(255),
	disk_space INT, 
	file_count INT
);

CREATE TABLE AUDIO_FILE (
    file_name VARCHAR(255) UNIQUE PRIMARY KEY,
	file_type VARCHAR(255),
	duration INT,
    sample_rate INT,
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);


CREATE TABLE PLAYLIST_FILE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    playlist VARCHAR(255),
    audio_file VARCHAR(255),
    FOREIGN KEY(playlist) REFERENCES PLAYLIST(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(file_name)
);

CREATE TABLE POMODORO (
	name VARCHAR(100),
	duration INT,
	duration_limit INT,
	short_break INT,
	long_break INT,
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);

CREATE TABLE POMODORO_AUDIO_FILE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    pomodoro VARCHAR(100),
    audio_file VARCHAR(255),
    FOREIGN KEY(pomodoro) REFERENCES POMODORO(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(file_name)
);

CREATE TABLE SOURCE (
	source_name VARCHAR(100),
	url VARCHAR(100), 
	api_key VARCHAR(100),
	storage_path VARCHAR(100),
	description VARCHAR(1000),
	status INT,
	date_modified VARCHAR(255),
    date_created VARCHAR(255)
);

CREATE TABLE FILE_TYPE (
	extension_name VARCHAR(100) UNIQUE PRIMARY KEY,
    date_created VARCHAR(255),
	date_modified VARCHAR(255)
);

CREATE TABLE SOURCE_FILE_TYPE (
    relation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    source VARCHAR(100),
   	file_type VARCHAR(255),
    FOREIGN KEY(source) REFERENCES SOURCE(source_name),
    FOREIGN KEY(file_type) REFERENCES FILE_TYPE(extension_name)
);

