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
    console.log("Cycling to next audio file"); 
    this.currentQueueIdx += 1;
    this.previousQueueIdx += 1;
    this.nextQueueIdx += 1;
  },

  prevAudioFile() {
    console.log("Going to previous audio file"); 
    this.currentQueueIdx -= 1;
    this.previousQueueIdx -= 1;
    this.nextQueueIdx -= 1;
  },

  async playAudio() {

    console.log("PLAYING AUDIO FILE"); 
    console.log(this.curr_audio_file); 


    /*
    this.playing = true;
    this.duration = parseInt(this.curr_audio_file.duration);

    const fileBuffer = await readFile(`dtunes-audio-app/audio_files/${this.curr_audio_file.file_path}`, {
        baseDir: BaseDirectory.Data,
    });

    const audioUrl = URL.createObjectURL(new Blob([fileBuffer]));
    console.log("Attempting to play:", audioUrl);

    await updateAudioPlayerInformation(
      this.curr_audio_file.audio_file_id.toString(), 
      this.curr_audio_file.thumbnail,
      parseInt(this.curr_audio_file.duration)
    ); 

    if(this.audio_player_interval == null) {
      console.log("Creating new interval...", this.audio_player_interval); 
      this.audioPlayerInterval = setInterval(this.updateRealtimePlayerInformation, 1000); 
    }

    if(this.current_queue_idx == this.audio_files.length) {
      console.log("Killing interval..."); 
      clearInterval(this.audioPlayerInterval); 
    }

    console.log("QUEUE LENGTH", this.queuedAudioFiles.length); 

    try {
        this.player = new Audio(audioUrl);
        this.player.play();

        this.player.onended = () => {
          this.addPlay(audioFile.audio_file_id.toString()); 
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
    } */
    
  },

  async pauseAudio() {
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

