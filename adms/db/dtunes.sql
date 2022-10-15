/** Current DB schema for DTUNES */

CREATE TABLE COLLECTION (
    name VARCHAR(255) UNIQUE PRIMARY KEY,
    date_created VARCHAR(255),
	disk_space VARCHAR(255), 
	file_count VARCHAR(255),
	collection_path VARCHAR(1000)
);


CREATE TABLE AUDIO_FILE (
    name VARCHAR(255) UNIQUE PRIMARY KEY,
    date_created VARCHAR(255),
    streaming_path VARCHAR(255)
);


CREATE TABLE COLLECTION_FILE (
    relation_id VARCHAR(37) PRIMARY KEY,
    collection VARCHAR(255),
    audio_file VARCHAR(255),
    FOREIGN KEY(collection) REFERENCES COLLECTION(name),
    FOREIGN KEY(audio_file) REFERENCES AUDIO_FILE(name)
);


