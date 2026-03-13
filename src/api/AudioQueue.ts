import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { AudioFile } from "./AudioFile";

export const audioQueueStore = reactive({

  audioFiles: [] as AudioFile[],
  currentQueueIdx: 0,
  previousQueueIdx: -1,
  nextQueueIdx: 0,
  started: false as boolean,
  playing: false as boolean,
  resume: false as boolean,
  paused: false as boolean,
  currentTime: 0 as number,
  duration: 0 as number,
  currAudioFile: null as AudioFile | null,
  player: null as HTMLAudioElement | null,
  audioPlayerInterval: null as NodeJS.Timeout | null,

  startQueue() {
    console.log(this.audioFiles);
    this.started = true; 
  },

  stopQueue() {
    this.started = false; 
  },

  initPlayer() {
    this.resume = true; 
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
      this.previousQueueIdx = -1;
      this.nextQueueIdx = 1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
    } else {
      console.log("Cycling to next audio file");
      this.currentQueueIdx += 1;
      this.previousQueueIdx += 1;
      this.nextQueueIdx += 1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx]; 
    }

  },

  prevAudioFile() {
    console.log("Going to previous audio file"); 
    this.currentQueueIdx -= 1;
    this.previousQueueIdx -= 1;
    this.nextQueueIdx -= 1;
  },

  async playAudio() {

    try {

      this.player = new Audio(audioUrl);
      this.player.play();

      this.player.onended = () => {
        this.resume = false;  
        this.playing = false;         
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

