import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';

// Define the type for the AudioFile object
export interface AudioFile {
  audio_file_id: number;
  date_created: string;
  duration: string;
  file_name: string;
  file_path: string;
  last_modified: string;
  plays: number;
  sample_rate: string;
  thumbnail: string;
}

export class AudioFile {
  audio_file_id: number;
  date_created: string;
  duration: string;
  file_name: string;
  file_path: string;
  last_modified: string;
  plays: number;
  sample_rate: string;
  thumbnail: string;

  constructor({
    audio_file_id,
    date_created,
    duration,
    file_name,
    file_path,
    last_modified,
    plays,
    sample_rate,
    thumbnail,
  }: {
    audio_file_id: number;
    date_created: string;
    duration: string;
    file_name: string;
    file_path: string;
    last_modified: string;
    plays: number;
    sample_rate: string;
    thumbnail: string;
  }) {
    this.audio_file_id = audio_file_id;
    this.date_created = date_created;
    this.duration = duration;
    this.file_name = file_name;
    this.file_path = file_path;
    this.last_modified = last_modified;
    this.plays = plays;
    this.sample_rate = sample_rate;
    this.thumbnail = thumbnail;
  }
}

export const audioStore = reactive({

  audioFiles: [] as AudioFile[],

  async loadAudioFiles() {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('view_audio_files', { userDbPath});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files: ", error); 
    }
  },

  async viewAudioFile(audioFileId: string): Promise<AudioFile> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("view_audio_file", { userDbPath, audioFileId });
  },

  async createAudioFile(aliasFileName: string, userLocalFilePath: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userAudioFilePath = `${dataDirPath}/dtunes-audio-app/audio_files`;
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    const audioFileName = userLocalFilePath.replace(/^.*[\\/]/, '');
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("create_audio_file", { 
        aliasFileName,
        audioFileName,
        thumbnailFileName,
        userLocalFilePath,
        userLocalThumbnailPath,
        userThumbnailPath,
        userAudioFilePath,
        userDbPath
    });
  },

  async editAudioFile(audioFileId: string, aliasFileName: string, userLocalFilePath: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userAudioFilePath = `${dataDirPath}/dtunes-audio-app/audio_files`;
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    const audioFileName = userLocalFilePath.replace(/^.*[\\/]/, '');
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("edit_audio_file", { 
        audioFileId,
        aliasFileName,
        audioFileName,
        thumbnailFileName,
        userLocalFilePath,
        userLocalThumbnailPath,
        userThumbnailPath,
        userAudioFilePath,
        userDbPath
    });
  },

  async deleteAudioFile(audioFileId: string): Promise<string> {

    const dataDirPath = await dataDir();
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userAudioFilePath = `${dataDirPath}/dtunes-audio-app/audio_files`;
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("delete_audio_file", { 
      userDbPath, 
      userThumbnailPath, 
      userAudioFilePath, 
      audioFileId
    });
  },

  async searchAudioFiles(searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('search_audio_files', { userDbPath, searchTerm});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

  async addPlay(audioFileId: string) {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("play_audio_file", {
      userDbPath,
      audioFileId
    });
  },


  async convertSecondsToMinutes(seconds: number) {
    const hours = Math.floor(seconds / 3600); // Get full hours
    const minutes = Math.floor(seconds/60) // Get full minutes
    const remainingSeconds = Math.floor(seconds % 60); // Get the remaining seconds 
    const formattedMinutes = minutes < 10 ? '0' + minutes : minutes;
    const formattedSeconds = remainingSeconds < 10 ? '0' + remainingSeconds : remainingSeconds;
    return `${hours}:${formattedMinutes}:${formattedSeconds}`;
  },


})



