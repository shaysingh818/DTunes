import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { reactive } from 'vue';
import { AudioFile } from "./AudioFile";
import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';

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

  ...initialState(),

  reset() {
    
    if(this.audioPlayerInterval) {
      clearInterval(this.audioPlayerInterval);
    }

    if(this.playerActive()) {
      this.stopAudio();
    }

    const fresh = initialState();
    for (const key in fresh) {
      (this as any)[key] = fresh[key as keyof typeof fresh]; 
    }

  },

  initPlayer() {
    this.active = true;
    this.currentTime = 0;

    this.audioPlayerInterval = setInterval(() => {
      this.updateRealtimePlayerInformation();
    }, 500); 
  },

  async playerActive(): Promise<boolean> {
    return await invoke("is_playing", {});
  },

  async queue(audioFiles: AudioFile[]) {
    if(this.playerActive()) {
      this.stopAudio();
    }
    this.audioFiles = audioFiles;
  },

  setCurrAudioFile() {
    this.currAudioFile = this.audioFiles[this.currentQueueIdx];
  },

  isPlaying() {
    const playFirst = this.playing === false && this.paused === false && this.active === true;
    const resume = this.playing === false && this.paused === true && this.active == true;
    return playFirst || resume; 
  },

  isPaused() {
    return this.playing == true;
  },

  nextAudioFile() {

    if(this.currentQueueIdx >= this.audioFiles.length-1) {
      this.currentQueueIdx = 0;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.playerActive()) { this.stopAudio(); }
      this.playAudio(); 
    } else {
      this.currentQueueIdx += 1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx]; 
      if(this.playerActive()) { this.stopAudio(); }
      this.playAudio(); 
    }

  },

  prevAudioFile() {
    if(this.currentQueueIdx <= 0) {
      this.currentQueueIdx = this.audioFiles.length-1;
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.playerActive()) { this.stopAudio(); }
      this.playAudio(); 
    } else {
      this.currentQueueIdx -= 1; 
      this.currAudioFile = this.audioFiles[this.currentQueueIdx];
      if(this.playerActive()) { this.stopAudio(); }
      this.playAudio(); 
    }
  },

  async playAudio() {
    if(this.currAudioFile) {

      const dataDirPath = await dataDir();
      const fileName = this.currAudioFile.file_path;
      const duration = this.currAudioFile.duration;
      const filePath = `${dataDirPath}/dtunes-audio-app/audio_files/${fileName}`;
      await invoke("play_audio", {  filePath, duration: parseFloat(duration) });
      this.playing = true;
      this.paused = false;
      this.duration = duration; 
    }
  },

  async currDuration(): number {
    return await invoke("curr_duration", {});
  },

  async pauseAudio() {
    await invoke("pause_audio", {});
    this.playing = false;
    this.resume = true;
    this.paused = true; 
  },

  async stopAudio() {
    await invoke("stop_audio", {});
    this.playing = false; 
    this.paused = false;
  },

  async forward() {
    await invoke("forward_duration", {});
  },

  async rewind() {
    await invoke("rewind_duration", {}); 
  },

  async updateRealtimePlayerInformation() {
    const durationElement = document.getElementById("duration-tracker");
    console.log(await this.currDuration());
    console.log(this.duration); 
    if(durationElement && audioQueueStore.playing == true)  {
      const currTime = await this.currDuration(); 
      const value = (currTime/this.duration) * 100;
      durationElement.style.width = `${value}%`;
    }
  }, 

});

