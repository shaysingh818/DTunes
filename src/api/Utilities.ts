import { create, exists, mkdir } from '@tauri-apps/plugin-fs';
import { dataDir } from '@tauri-apps/api/path';


export async function checkAppDataFolders(): Promise<boolean> {

    let check = false;
    const appDataDir = await dataDir(); // Get the app's data directory path

    const directoriesToCheck = [
      'dtunes-audio-app/images',
      'dtunes-audio-app/audio_files',
      'dtunes-audio-app/metadata'
    ];

    for (const dir of directoriesToCheck) {
      const fullDirPath = `${appDataDir}/${dir}`;
      const dirExists = await exists(fullDirPath);
      if(dirExists) {
        check = true; 
      }
    }

    return check; 
}

// Function to create directories if they do not exist
export async function createAppDataFolders() {
  try {
    const appDataDir = await dataDir(); // Get the app's data directory path

    // Define paths for the directories you want to create
    const directoriesToCreate = [
      'dtunes-audio-app/images',
      'dtunes-audio-app/audio_files',
      'dtunes-audio-app/metadata'
    ];

    // Loop through and create each directory if it doesn't exist
    for (const dir of directoriesToCreate) {
        const fullDirPath = `${appDataDir}/${dir}`;
        await mkdir(fullDirPath);
    }
  } catch (error) {
    console.error("Error creating app data folders:", error);
  }
}
