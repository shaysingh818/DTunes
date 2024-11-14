import os
import time
import sqlite3


# Function to read SQL from a file
def read_sql_file(file_path):
    with open(file_path, 'r') as file:
        sql_content = file.read()
    return sql_content


def commit(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if os.path.isfile(file_path):
            if os.path.getsize(file_path) != 0:
                sql_query = read_sql_file(file_path)
                conn = sqlite3.connect('storage/dtunes.sqlite3')
                cursor = conn.cursor()
                cursor.executescript(sql_query)
                print(f'[sqlite3]: Executing {file_path} on dtunes.sqlite3')
                conn.close()


# Config
SCRIPTS_FOLDER = "sql/"
METADATA_STORAGE_PATH = "storage/"

def main():

    # Create database if storage path is empty
    if len(os.listdir(METADATA_STORAGE_PATH)) == 0:
        conn = sqlite3.connect('storage/dtunes.sqlite3')
        conn.close()
        print("[sqlite3]: Creating database file dtunes.sqlite3")
    else:
        print("[sqlite3]: Database file found, running scripts..")

    print(f'[sqlite3] committing scripts in {SCRIPTS_FOLDER}')
    commit(SCRIPTS_FOLDER)


main()


