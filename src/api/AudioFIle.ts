import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue'

// Define the type for the AudioFile object
export interface AudioFile {
  audioFileId: number;
  dateCreated: string;
  duration: string;
  fileName: string;
  filePath: string;
  lastModified: string;
  plays: number;
  sampleRate: string;
  thumbnail: string;
}

export class AudioFile {
  audioFileId: number;
  dateCreated: string;
  duration: string;
  fileName: string;
  filePath: string;
  lastModified: string;
  plays: number;
  sampleRate: string;
  thumbnail: string;

  constructor({
    audioFileId,
    dateCreated,
    duration,
    fileName,
    filePath,
    lastModified,
    plays,
    sampleRate,
    thumbnail,
  }: {
    audioFileId: number;
    dateCreated: string;
    duration: string;
    fileName: string;
    filePath: string;
    lastModified: string;
    plays: number;
    sampleRate: string;
    thumbnail: string;
  }) {
    this.audioFileId = audioFileId;
    this.dateCreated = dateCreated;
    this.duration = duration;
    this.fileName = fileName;
    this.filePath = filePath;
    this.lastModified = lastModified;
    this.plays = plays;
    this.sampleRate = sampleRate;
    this.thumbnail = thumbnail;
  }
}

export const audioStore = reactive({

  audioFiles: [] as AudioFile[],
  audioFilePlaying: {} as AudioFile,
  player: null as HTMLAudioElement | null,
  playing: false as boolean,
  paused: false as boolean, 
  resume: false as boolean, 
  currentTime: 0 as number,
  duration: 0 as number,


  async setCurrentPlaying(audioFile: AudioFile) {
    this.audioFilePlaying = audioFile;
    this.resume = false;
    this.currentTime = 0;
  },

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

  async playAudio(audioFile: AudioFile) {

    this.audioFilePlaying = audioFile;
    this.playing = true;

    const fileBuffer = await readFile(`dtunes-audio-app/audio_files/${audioFile.filePath}`, {
        baseDir: BaseDirectory.Data,
    });

    const audioUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("Attempting to play:", audioUrl);

    try {
        this.player = new Audio(audioUrl);
        this.player.play();
        this.player.onpause = () => {
          if(this.player) {
            this.currentTime = this.player.currentTime;
          }
          this.paused = true;
        }

    } catch (error) {
        console.error("Could not play audio file: ", error); 
    }
    
  },

  async streamDuration() {
    if(this.player) {
      this.player.ontimeupdate = () => {
        if(this.player) {
          this.currentTime  = this.player.currentTime;
        }
      }
    }
  },

  async pauseAudio() {

    this.playing = false;
    if(this.player) {
        this.player.pause();
        this.resume = true;
    }
    
  },

  async resumeAudio() {
    if(this.paused && this.player) {
      this.player.play();
      this.player.currentTime = this.currentTime;
      this.playing = true;
      this.paused = false; 
      this.resume = false;
    }
  }
})

