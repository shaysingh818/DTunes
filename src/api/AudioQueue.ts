import { BaseDirectory, readFile } from '@tauri-apps/plugin-fs';
import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { AudioFile } from "./AudioFile";

export const audioQueueStore = reactive({

  audio_files: [] as AudioFile[],
  current_queue_idx: 0,
  previous_queue_idx: -1,
  next_queue_idx: 0,

  queue(audio_files: AudioFile[]) {
    this.audio_files = audio_files;
  },

  next() {
    this.current_queue_idx += 1;
    this.previous_queue_idx += 1;
    this.next_queue_idx += 1;
  },

  prev() {
    this.current_queue_idx -= 1;
    this.previous_queue_idx -= 1;
    this.next_queue_idx -= 1;
  },


});

