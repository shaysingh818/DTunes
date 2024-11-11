import os
import time
import duckdb


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
                conn = duckdb.connect('storage/dtunes.duckdb')
                conn.execute(sql_query)
                print(f'[duckdb]: Executing {file_path} on dtunes.duckdb')


# Config
SCRIPTS_FOLDER = "sql/"
METADATA_STORAGE_PATH = "storage/"

def main():

    # Create database if storage path is empty
    if len(os.listdir(METADATA_STORAGE_PATH)) == 0:
        conn = duckdb.connect('storage/dtunes.duckdb')
        conn.close()
        print("[duckdb]: Creating database file dtunes.duckdb")
    else:
        print("[duckdb]: Database file found, running scripts..")

    print(f'[duckdb] committing scripts in {SCRIPTS_FOLDER}')
    commit(SCRIPTS_FOLDER)




