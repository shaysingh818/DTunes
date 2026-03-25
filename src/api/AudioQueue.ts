import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
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
  _currentBlobUrl: null as string | null,
})

export const audioQueueStore = reactive({

  // add methods for paused, playing, resume states

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
    this.currentTime = 0;
  },

  queue(audioFiles: AudioFile[]) {
    this.audioFiles = audioFiles;
  },

  setCurrAudioFile() {
    this.currAudioFile = this.audioFiles[this.currentQueueIdx];
  },

  isPlaying() {
    return this.active == true && this.playing == false && this.resume == false;
  },

  isResume() {
    return this.resume == true && this.paused == true && this.playing == false;
  },

  isPaused() {
    return this.playing == true;
  },

  nextAudioFile() {

    if(this.currentQueueIdx >= this.audioFiles.length-1) {
      this.currentQueueIdx = 0;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.playing == true && this.player != null) {
        this.player.pause();  
      }
      this.playAudio(); 
    } else {
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
      this.currentQueueIdx = this.audioFiles.length-1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.player) {
        this.player.pause();
      }
      this.playAudio(); 
    } else {
      this.currentQueueIdx -= 1; 
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.player) {
        this.player.pause(); 
      }
      this.playAudio(); 
    }
  },

  async playAudio() {

    if(this.currAudioFile) {


      this.playing = true;
      this.duration = parseInt(this.currAudioFile.duration); 
      const filePath = this.currAudioFile.file_path;

      if(this.player) {
        this.player.pause();
        this.player.src = '';
        this.player.load(); 
        this.player = null;
      }

      const fileBuffer = await readFile(`dtunes-audio-app/audio_files/${filePath}`, {
          baseDir: BaseDirectory.Data,
        });

      if(this._currentBlobUrl) {
        URL.revokeObjectURL(this._currentBlobUrl);
      }

      const audioUrl = URL.createObjectURL(new Blob([fileBuffer]));
      this._currentBlobUrl = audioUrl;

      try {

        this.player = new Audio(audioUrl);

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

        await new Promise<void>((resolve) => {
            this.player!.oncanplaythrough = () => resolve();
            this.player!.load();
        }); 


        await this.player.play();

      } catch (error) {
        console.error("Could not play audio file: ", error);
      }    

    } else {
      console.error("No current audio file to play"); 
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
      this.player.currentTime = this.currentTime;
      await this.player.play();
      this.playing = true;
      this.paused = false; 
      this.resume = false;
    }
  },

  async forward() {
    if(this.player) {
      const newDuration = this.player.currentTime - 30;
      if(newDuration <= this.duration) {
        this.player.currentTime += 30;
        this.currentTime = this.player.currentTime; 
      }
    }
  },

  async rewind() {
    if(this.player) {
      const newDuration = this.player.currentTime - 30;
      if(newDuration > 0) {
        this.player.currentTime -= 30;
        this.currentTime = this.player.currentTime; 
      }
    }
  },

  async updateRealtimePlayerInformation() {
    const durationElement = document.getElementById("duration-tracker");
    if(durationElement && audioQueueStore.playing == true)  {
      const value = (this.currentTime/this.duration) * 100;
      durationElement.style.width = `${value}%`;
    }
  }, 

});

