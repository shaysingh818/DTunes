import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { AudioFile, audioStore } from "./AudioFile";

// Define the type for the AudioFile object
export interface Artist {
  artistId: number;
  artistName: string;
  artistThumbnail: string;
  dateCreated: string;
  lastModified: string;
}

export class Artist {
  artistId: number;
  artistName: string;
  artistThumbnail: string;
  dateCreated: string;
  lastModified: string;

  constructor({
    artistId,
    artistName,
    artistThumbnail,
    dateCreated,
    lastModified,
  }: {
    artistId: number;
    artistName: string;
    artistThumbnail: string;
    dateCreated: string;
    lastModified: string;
  }) {
    this.artistId = artistId;
    this.artistName = artistName;
    this.artistThumbnail = artistThumbnail;
    this.dateCreated = dateCreated;
    this.lastModified = lastModified;
  }
}

export const artistStore = reactive({

  artists: [] as Artist[],
  audioFiles: [] as AudioFile[],

  async loadArtists() {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const artists = await invoke<Artist[]>('view_artists', { userDbPath});
        this.artists = artists;

    } catch(error) {
        console.error("Error loading artists: ", error); 
    }
  },

  async createArtist(artistName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("create_artist", { 
        artistName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async editArtist(artistId: string, artistName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("edit_artist", { 
        artistId,
        artistName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async deleteArtist(artistId: string): Promise<string> {

    const dataDirPath = await dataDir();
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("delete_artist", { 
      userDbPath, 
      userThumbnailPath, 
      artistId
    });
  },

  async viewArtist(artistId: string): Promise<Artist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("view_artist", { userDbPath, artistId });
  },

  async viewArtistAudioFiles(artistId: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('view_artist_audio_files', { userDbPath, artistId});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files: ", error); 
    }
  },

  async addAudioFileArtist(artistId: string, audioFileId: Number): Promise<Artist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("add_audio_file_artist", { 
        userDbPath, 
        artistId, 
        audioFileId 
    });
  },

  async removeAudioFileArtist(artistId: string, audioFileId: Number): Promise<Artist> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("remove_audio_file_artist", { 
        userDbPath, 
        artistId, 
        audioFileId 
    });
  },

  async searchArtist(searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const artists = await invoke<Artist[]>('search_artists', { userDbPath, searchTerm});
        this.artists = artists;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },


  async searchArtistAudioFiles(artistId: string, searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>(
          'search_artist_audio_files', 
          { userDbPath, artistId, searchTerm}
        );
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },


  async queueArtistAudioFiles(artistId: string, audioFileId: string) {

    let audioFile = await audioStore.viewAudioFile(audioFileId);
    audioStore.queuedAudioFiles.push(audioFile);

    await this.viewArtistAudioFiles(artistId);
    this.audioFiles.forEach( (audioFile) => {
      audioStore.queuedAudioFiles.push(audioFile);
    });

    audioStore.queueIndex = 0; 
  },

}); 