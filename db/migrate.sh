
# Migrate main database instance
sudo rm dtunes.db
touch dtunes.db
cat dtunes.sql | sqlite3 dtunes.db

# Migrate audio file instance
sudo rm unit/audio_file_instance.db
touch unit/audio_file_instance.db
cat dtunes.sql | sqlite3 unit/audio_file_instance.db

# Migrate audio file instance
sudo rm unit/playlist_instance.db
touch unit/playlist_instance.db
cat dtunes.sql | sqlite3 unit/playlist_instance.db

# Migrate audio file instance
sudo rm unit/pomodoro_instance.db
touch unit/pomodoro_instance.db
cat dtunes.sql | sqlite3 unit/pomodoro_instance.db

# Migrate audio file instance
sudo rm unit/source_instance.db
touch unit/source_instance.db
cat dtunes.sql | sqlite3 unit/source_instance.db