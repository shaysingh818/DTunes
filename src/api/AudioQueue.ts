import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { AudioFile } from "./AudioFile";


const initialState = () => ({
  active: false as boolean,
  playing: false as boolean,
  resume: false as boolean,
  paused: false as boolean,
  duration: 0 as number,
  player: null as HTMLAudioElement | null,
  audioFiles: [] as AudioFile[],
  currentQueueIdx: 0, 
  currentTime: 0 as number, 
  currAudioFile: null as AudioFile | null,
  audioPlayerInterval: null as NodeJS.Timeout | null,
})

export const audioQueueStore = reactive({

  ...initialState(),

  reset() {
    
    if(this.audioPlayerInterval) {
      clearInterval(this.audioPlayerInterval);
    }

    if(this.player) {
      this.player.pause(); 
      this.player.src = ''; 
      this.player = null;
    }

    const fresh = initialState();
    for (const key in fresh) {
      (this as any)[key] = fresh[key as keyof typeof fresh]; 
    }

  },

  initPlayer() {
    this.active = true;
    this.currentTime = -1;
  },

  queue(audioFiles: AudioFile[]) {
    this.audioFiles = audioFiles;
  },

  setCurrAudioFile() {
    this.currAudioFile = this.audioFiles[this.currentQueueIdx];
    console.log("Set current audio file");
  },

  nextAudioFile() {

    if(this.currentQueueIdx >= this.audioFiles.length-1) {
      console.log("Going back to beginning");
      this.currentQueueIdx = 0;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.playing == true && this.player != null) {
        this.player.pause();  
      }
      this.playAudio(); 
    } else {
      console.log("Cycling to next audio file");
      this.currentQueueIdx += 1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx]; 
      if(this.playing == true && this.player != null) {
        this.player.pause();
      }
      this.playAudio(); 
    }

  },

  prevAudioFile() {
    if(this.currentQueueIdx <= 0) {
      console.log("Going to end of queue");
      this.currentQueueIdx = this.audioFiles.length-1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      this.player.pause();
      this.playAudio(); 
    } else {
      console.log("Cycling to previous audio file");
      this.currentQueueIdx -= 1; 
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      this.player.pause(); 
      this.playAudio(); 
    }
  },

  async playAudio() {

    this.playing = true;
    this.duration = parseInt(this.currAudioFile.duration); 
    const filePath = this.currAudioFile.file_path;

    const fileBuffer = await readFile(`dtunes-audio-app/audio_files/${filePath}`, {
        baseDir: BaseDirectory.Data,
      });

    const audioUrl = URL.createObjectURL(new Blob([fileBuffer]));

    try {

      this.player = new Audio(audioUrl);
      this.player.play();

      this.player.onended = () => {
        this.resume = false;  
        this.playing = false; 
        console.log("does this get hit?"); 
      }

      this.player.onpause = () => {
        if(this.player) {
          this.currentTime = this.player.currentTime;
        }
        this.paused = true;
      }

      this.player.ontimeupdate = () => {
        if(this.player) {
          this.currentTime  = this.player.currentTime;
        }
      }

    } catch (error) {
      console.error("Could not play audio file: ", error);
    }    
  },

  async pauseAudio() {
    console.log("Paused audio"); 
    this.playing = false;
    if(this.player) {
        this.player.pause();
        this.resume = true;
    }    
  },

  async resumeAudio() {
    console.log("resume audio being called"); 
    if(this.paused && this.player) {
      this.player.play();
      this.player.currentTime = this.currentTime;
      this.playing = true;
      this.paused = false; 
      this.resume = false;
    }
  },

  async forward() {
    console.log("Forward audio being called"); 
    if(this.player) {
      const newDuration = this.player.currentTime - 30;
      if(newDuration <= this.duration) {
        this.player.currentTime += 30;
        this.currentTime = this.player.currentTime; 
      }
    }
  },

  async rewind() {
    console.log("Rewind audio being called"); 
    if(this.player) {
      const newDuration = this.player.currentTime - 30;
      if(newDuration > 0) {
        this.player.currentTime -= 30;
        this.currentTime = this.player.currentTime; 
      }
    }
  },

});

