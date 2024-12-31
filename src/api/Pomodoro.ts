import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { audioStore, AudioFile } from "./AudioFile";

export class PomodoroTimer {

    private timerId: NodeJS.Timeout | null = null;
    private remainingTime: number | null = null;
    private duration: number | null = null;
    private stringTimerValue: string | null = null; 
    private playing: boolean = false;
    private paused: boolean = false;
    private resumed: boolean = false;
    private updateCallback: (time: string) => void; 

    constructor(duration: number, updateCallback: (time: string) => void) {

      let minutes = Math.floor(duration / 60); 
      let seconds = duration % 60; 

      this.duration = duration;
      this.remainingTime = duration;
      this.stringTimerValue = seconds < 10 
        ? `${minutes}:0${seconds}` 
        : `${minutes}:${seconds}`;

      this.updateCallback = updateCallback;
      this.updateCallback(this.stringTimerValue);
      
    }

    isPlaying(): boolean {
      return this.playing;
    }

    isPaused(): boolean {
      return this.paused;
    }

    isResumed(): boolean {
      return this.resumed;
    }

    pause(): void {
      if(this.timerId !== null) {
        console.log("PAUSING TIMER"); 
        clearInterval(this.timerId); 
        if(this.duration && this.remainingTime) {
          console.log("GETTING REMAINING TIME"); 
          const elapsed = this.duration - this.remainingTime; 
          console.log("ELAPSED: ", elapsed);  
          this.remainingTime = this.duration - elapsed; 
          this.timerId = null;
        }
        this.paused = true;
        this.resumed = true;
      }
    }

    resume(): void {
      if(this.timerId == null) {
        console.log("RESUMING TIMER");
        this.resumed = false; 
        this.paused = false; 
        this.timerId = setInterval(() => this.update(), 1000);  
      }
    }

    start(): void {
      if(this.timerId == null) {
        console.log("STARTING TIMER");
        this.playing = true;
        this.timerId = setInterval(() => this.update(), 1000);  
      }
      
    }

    stop(): void {
      console.log("Called stop timer"); 
      if(this.timerId != null) {
        console.log("STOPPING TIMER"); 
        clearInterval(this.timerId); 
        this.timerId = null;
        this.playing = false; 
        this.resumed = false; 
        this.paused = false;
      }
    }

    update(): void {
      console.log("DURATION: ", this.duration); 
      console.log("REMAINING TIME ", this.remainingTime); 

      if(this.remainingTime && this.duration) {
        let minutes = Math.floor(this.remainingTime / 60); 
        let seconds = this.remainingTime % 60; 

        this.stringTimerValue = seconds < 10 
          ? `${minutes}:0${seconds}` 
          : `${minutes}:${seconds}`;

        this.updateCallback(this.stringTimerValue); 
          
        if(this.remainingTime > 0) {
          console.log("CURR DURATION", this.remainingTime); 
          this.remainingTime--;
        }

        if(this.remainingTime == 0) {
          console.log("TIMER FINISHED"); 
          console.log("PLAYING TIMER SOUND");
          const alarmSound = new Audio('dtunes-alarm-sound.mp3'); 
          alarmSound.play().then(() => {
            console.log("Played sound")
          }).catch((error) => {
            console.error("Autoplay error: ", error); 
          }); 
          
          this.playing = false; 
          this.resumed = false; 
          this.paused = false;  
          if(this.timerId){
            clearInterval(this.timerId); 
          }
        }

    
      }

    }

}


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
  focusDuration: true as Boolean,
  shortBreakDuration: false as Boolean, 
  longBreakDuration: false as Boolean,
  sessionStarted: false as Boolean,
  sessionPaused: false as Boolean,
  sessionFinished: false as Boolean, 
  sessionResume: false as Boolean,
  currentDuration: 0 as Number,
  sessionPlayerInterval: null as NodeJS.Timeout | null,
  pomodoroTimer: null as PomodoroTimer | null,

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

  async queuePomodoroAudioFiles(pomodoroId: string, audioFileId: string) {

    let audioFile = await audioStore.viewAudioFile(audioFileId);
    audioStore.queuedAudioFiles.push(audioFile);

    await this.viewPomodoroAudioFiles(pomodoroId);
    this.audioFiles.forEach( (audioFile) => {
      audioStore.queuedAudioFiles.push(audioFile);
    });

    audioStore.queueIndex = 0; 
  },


  async selectShortBreak(shortBreak: number) {

    if(this.sessionPlayerInterval) {
      clearInterval(this.sessionPlayerInterval);   
    }

    this.currentDuration = shortBreak; 
    this.sessionPaused = false; 
    this.sessionStarted = false; 
    this.focusDuration = false;
    this.longBreakDuration = false; 
    this.shortBreakDuration = true; 
  },

  async selectLongBreak(longBreak: number) {

    if(this.sessionPlayerInterval) {
      clearInterval(this.sessionPlayerInterval);   
    }

    this.currentDuration = longBreak; 
    this.sessionPaused = false; 
    this.sessionStarted = false; 
    this.focusDuration = false;
    this.shortBreakDuration = false; 
    this.longBreakDuration = true; 
  },

  async selectDurationBreak(duration: number) {

    if(this.sessionPlayerInterval) {
      clearInterval(this.sessionPlayerInterval);   
    }

    this.currentDuration = duration; 
    this.sessionPaused = false; 
    this.sessionStarted = false; 
    this.shortBreakDuration = false; 
    this.longBreakDuration = false;
    this.focusDuration = true;
  },

}); 
