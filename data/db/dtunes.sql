/**
Currently not using unique ID
Don't know how to generate unique id from database
*/

CREATE TABLE YOUTUBE_URL (
	yt_url VARCHAR(255),	
	date_created VARCHAR(255),
	desired_path VARCHAR(255)
);

CREATE TABLE PLAYLIST (
	name VARCHAR(255) PRIMARY KEY,
	date_created VARCHAR(255)
); 


CREATE TABLE SONG (
	name VARCHAR(255) PRIMARY KEY,
	date_created VARCHAR(255),
	file_path VARCHAR(255),
	subtitles VARCHAR(1000),
	plays INT
); 



