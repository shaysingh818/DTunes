import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { AudioFile } from "./AudioFile";

// Define the type for the AudioFile object
export interface Pomodoro {
  sessionId: number;
  sessionName: string;
  duration: number;
  durationLimit: number;
  shortBreak: number;
  longBreak: number;
  dateCreated: string;
  lastModified: string;
}

export class Pomodoro {
  sessionId: number;
  sessionName: string;
  duration: number;
  durationLimit: number;
  shortBreak: number;
  longBreak: number;
  dateCreated: string;
  lastModified: string;

  constructor({
    sessionId,
    sessionName,
    duration,
    durationLimit,
    shortBreak, 
    longBreak,
    dateCreated,
    lastModified,
  }: {
    sessionId: number;
    sessionName: string;
    duration: number;
    durationLimit: number;
    shortBreak: number; 
    longBreak: number;
    dateCreated: string;
    lastModified: string;
  }) {
    this.sessionId = sessionId;
    this.sessionName = sessionName;
    this.duration = duration;
    this.durationLimit = durationLimit;
    this.shortBreak = shortBreak;
    this.longBreak = longBreak;
    this.dateCreated = dateCreated; 
    this.lastModified = lastModified;
  }
}

export const pomodoroStore = reactive({

  sessions: [] as Pomodoro[],
  audioFiles: [] as AudioFile[],

  async loadSessions() {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const sessions = await invoke<Pomodoro[]>('view_pomodoro_sessions', { userDbPath});
        this.sessions = sessions;

    } catch(error) {
        console.error("Error loading artists: ", error); 
    }
  },

  async createSession(
    sessionName: string, 
    duration: number,
    durationLimit: number,
    shortBreak: number,
    longBreak: number): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("create_pomodoro", { 
        userDbPath,
        sessionName,
        duration,
        durationLimit,
        shortBreak,
        longBreak
    });
  },

  async editSession(
    sessionId: string, 
    sessionName: string, 
    duration: number,
    durationLimit: number,
    shortBreak: number,
    longBreak: number): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("edit_pomodoro", { 
        userDbPath,
        sessionId,
        sessionName,
        duration,
        durationLimit,
        shortBreak,
        longBreak
    });
  },

  async deleteSession(sessionId: string): Promise<string> {

    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("delete_pomodoro_session", { 
      userDbPath, 
      sessionId
    });
  },

  async viewSession(sessionId: string): Promise<Pomodoro> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("view_pomodoro_session", { userDbPath, sessionId });
  },

  async viewPomodoroAudioFiles(sessionId: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('view_pomodoro_audio_files', { userDbPath, sessionId});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files: ", error); 
    }
  },

  async addAudioFilePomodoro(sessionId: string, audioFileId: Number) {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("add_audio_file_pomodoro", { 
        userDbPath, 
        sessionId, 
        audioFileId 
    });
  },

  async removeAudioFilePomodoro(sessionId: string, audioFileId: Number) {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("remove_audio_file_pomodoro", { 
        userDbPath, 
        sessionId, 
        audioFileId 
    });
  },

  async searchPomodoro(searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const sessions = await invoke<Pomodoro[]>('search_pomodoro_sessions', { userDbPath, searchTerm});
        this.sessions = sessions;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

  async searchPomodoroAudioFiles(sessionId: string, searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>(
          'search_pomdoro_audio_files', 
          { userDbPath, sessionId, searchTerm}
        );
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

}); 