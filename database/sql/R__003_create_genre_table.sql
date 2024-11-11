CREATE TABLE IF NOT EXISTS GENRE (
    GENRE_ID INTEGER PRIMARY KEY,
    GENRE_NAME VARCHAR,
    DATE_CREATED DATETIME,
    LAST_MODIFIED DATETIME
);

CREATE TABLE IF NOT EXISTS GENRE_AUDIO_FILE (
    GENRE_AUDIO_FILE_ID INTEGER PRIMARY KEY,
    GENRE_ID INTEGER,
    AUDIO_FILE_ID INTEGER,
    FOREIGN KEY (GENRE_ID) REFERENCES GENRE(GENRE_ID), 
    FOREIGN KEY (AUDIO_FILE_ID) REFERENCES AUDIO_FILE(AUDIO_FILE_ID)
);