# migrate new db model changes
sudo rm dtunes.db
touch dtunes.db
cat dtunes.sql | sqlite3 dtunes.db
