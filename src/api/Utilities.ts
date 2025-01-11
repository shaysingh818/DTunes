import { mkdir, readDir, writeFile } from '@tauri-apps/plugin-fs';
import { BaseDirectory } from '@tauri-apps/api/path';
import { platform } from '@tauri-apps/plugin-os';

const isNode = typeof process !== 'undefined' && process.versions && process.versions.node;

async function joinPaths(...paths: any[]) {
  if (isNode) {
    // I have no idea why I need this but I'm going to keep just in case
    const path = require('path');
    console.log('Are we node?'); 
    return path.join(...paths); 
  } else {
    console.log("We're not node"); 
    if(await detectWindows()) {
      return paths.join('\\');
    }; 
    return paths.join('/');
  }
}

async function detectWindows(): Promise<boolean> {
  const platformName = await platform();
  if(platformName.toString() == "windows") {
    return true; 
  }
  return false;
}



export async function downloadSQLiteFile() {
  try {

    const RAW_FILE_URL = "https://raw.githubusercontent.com/shaysingh818/dtunes-studio/release/database/build/dtunes-audio-app.sqlite3"
    const response = await fetch(RAW_FILE_URL);
    if (!response.ok) {
      throw new Error(`Failed to fetch the file: ${response.statusText}`);
    }

    const fileBlob = await response.blob();
    const arrayBuffer = await fileBlob.arrayBuffer();
    const uint8Array = new Uint8Array(arrayBuffer);
    const fullPath = await joinPaths('dtunes-audio-app', 'metadata', 'dtunes-audio-app.sqlite3');

    await writeFile(
      fullPath, 
      uint8Array, 
      { baseDir: BaseDirectory.Data }
    );

    console.log(`SQLite file downloaded and saved`);
  } catch (error) {
    console.error("Error downloading the SQLite file:", error);
  }
}

export async function downloadDefaultAlarmSound(fileName: string) {

    const BASE_URL = `https://raw.githubusercontent.com/shaysingh818/dtunes-studio/main/public/${fileName}`
    const response = await fetch(BASE_URL);
    if (!response.ok) {
      throw new Error(`Failed to fetch default sound alarm: ${response.statusText}`);
    }

    const fileBlob = await response.blob();
    const arrayBuffer = await fileBlob.arrayBuffer();
    const uint8Array = new Uint8Array(arrayBuffer);
    const fullPath = await joinPaths('dtunes-audio-app', 'audio_files', fileName);

    await writeFile(
      fullPath, 
      uint8Array, 
      { baseDir: BaseDirectory.Data }
    );
}

export async function downloadDefaultThumbnail(fileName: string) {

    const BASE_URL = `https://raw.githubusercontent.com/shaysingh818/dtunes-studio/main/public/${fileName}`
    const response = await fetch(BASE_URL);
    if (!response.ok) {
      throw new Error(`Failed to fetch default sound alarm: ${response.statusText}`);
    }

    const fileBlob = await response.blob();
    const arrayBuffer = await fileBlob.arrayBuffer();
    const uint8Array = new Uint8Array(arrayBuffer);
    const fullPath = await joinPaths('dtunes-audio-app', 'images', fileName);

    await writeFile(
      fullPath, 
      uint8Array, 
      { baseDir: BaseDirectory.Data }
    );

}

export async function checkAppDataFolders(): Promise<boolean> {

    let check = false;
    const appName = 'dtunes-audio-app';
    const expectedDirectories: string[] = ['images', 'audio_files', 'metadata'];
    const dirs: string[] = [];

    try {

      const entries = await readDir(appName, {baseDir: BaseDirectory.Data});
      entries.forEach((entry) => {
        if(entry.isDirectory) {
          dirs.push(entry.name);
        }
      });

      check = expectedDirectories.every(dir => dirs.includes(dir)); 
    } catch (error) {
      console.error("Error checking app data folders:", error);
    }

    return check; 
}


// Function to create directories if they do not exist
export async function createAppDataFolders() {

  const appName = 'dtunes-audio-app';
  const dirs: string[] = ['images', 'audio_files', 'metadata'];

  try {
    dirs.forEach(async (dir) => {
        const fullPath = await joinPaths(appName, dir);
        mkdir(fullPath, {baseDir: BaseDirectory.Data});
    });
  } catch (error) {
    console.error("Error creating app data folders:", error);
  }
}

export async function syncMetadata() {
  if(await checkAppDataFolders() == false) {
    console.log("No metadata folders found, creating now... ")
    await createAppDataFolders();
    console.log("Created metadata folders"); 
    await downloadSQLiteFile();
    console.log("Downloaded latest database schema from github...");  
    await downloadDefaultThumbnail("default_dtunes_thumbnail.webp");
    console.log("Downloaded default thumbnail for DTunes app...");
    await downloadDefaultAlarmSound("dtunes-alarm-sound.mp3"); 
    console.log("Downloaded pomodoro alarm sound to audio files directory...")  
    alert("First time setup completed.. data can now be stored on the application"); 
  } else {
    alert("Detected first time setup already complete"); 
  }
}
