CREATE TABLE PLAYLIST (
	name VARCHAR(255) UNIQUE PRIMARY KEY,
	date_created VARCHAR(255),
	date_modified VARCHAR(255),
	disk_space INT, 
	file_count INT
);

CREATE TABLE AUDIO_FILE (
    file_name VARCHAR(1000) PRIMARY KEY,
	file_type VARCHAR(255),
	duration INT,
	date_modifed VARCHAR(255),
    date_created VARCHAR(1000),
    sample_rate INT
);


CREATE TABLE PLAYLIST_FILE (
    relation_id VARCHAR(37) PRIMARY KEY,
    playlist VARCHAR(255),
    audio_file VARCHAR(255),
    FOREIGN KEY(playlist) REFERENCES PLAYLIST(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(name)
);
