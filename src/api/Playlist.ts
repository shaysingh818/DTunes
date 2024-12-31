import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { audioStore, AudioFile } from "./AudioFile";

// Define the type for the AudioFile object
export interface Playlist {
  playlistId: number;
  playlistName: string;
  playlistThumbnail: string;
  dateCreated: string;
  lastModified: string;
}

export class Playlist {
  playlistId: number;
  playlistName: string;
  playlistThumbnail: string;
  dateCreated: string;
  lastModified: string;

  constructor({
    playlistId,
    playlistName,
    playlistThumbnail,
    dateCreated,
    lastModified,
  }: {
    playlistId: number;
    playlistName: string;
    playlistThumbnail: string;
    dateCreated: string;
    lastModified: string;
  }) {
    this.playlistId = playlistId;
    this.playlistName = playlistName;
    this.playlistThumbnail = playlistThumbnail;
    this.dateCreated = dateCreated;
    this.lastModified = lastModified;
  }
}

export const playlistStore = reactive({

  playlists: [] as Playlist[],
  audioFiles: [] as AudioFile[],

  async loadPlaylists() {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const playlists = await invoke<Playlist[]>('view_playlists', { userDbPath});
        this.playlists = playlists;

    } catch(error) {
        console.error("Error loading playlists: ", error); 
    }
  },

  async createPlaylist(playlistName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("create_playlist", { 
        playlistName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async editPlaylist(playlistId: string, playlistName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("edit_playlist", { 
        playlistId,
        playlistName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async deletePlaylist(playlistId: string): Promise<string> {

    const dataDirPath = await dataDir();
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("delete_playlist", { 
      userDbPath, 
      userThumbnailPath, 
      playlistId
    });
  },

  async viewPlaylist(playlistId: string): Promise<Playlist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("view_playlist", { userDbPath, playlistId });
  },

  async viewPlaylistAudioFiles(playlistId: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('view_playlist_audio_files', { userDbPath, playlistId});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files: ", error); 
    }
  },

  async addAudioFilePlaylist(playlistId: string, audioFileId: Number): Promise<Playlist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("add_audio_file_playlist", { 
        userDbPath, 
        playlistId, 
        audioFileId 
    });
  },

  async removeAudioFilePlaylist(playlistId: string, audioFileId: Number): Promise<Playlist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("remove_audio_file_playlist", { 
        userDbPath, 
        playlistId, 
        audioFileId 
    });
  },

  async searchPlaylists(searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const playlists = await invoke<Playlist[]>('search_playlists', { userDbPath, searchTerm});
        this.playlists = playlists;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

  async searchPlaylistAudioFiles(playlistId: string, searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>(
          'search_playlist_audio_files', 
          { userDbPath, playlistId, searchTerm}
        );
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

  async queuePlaylistAudioFiles(playlistId: string, audioFileId: string) {

    let audioFile = await audioStore.viewAudioFile(audioFileId);
    audioStore.queuedAudioFiles.push(audioFile);

    await this.viewPlaylistAudioFiles(playlistId);
    this.audioFiles.forEach( (audioFile) => {
      audioStore.queuedAudioFiles.push(audioFile);
    });

    audioStore.queueIndex = 0; 
  },

}); 