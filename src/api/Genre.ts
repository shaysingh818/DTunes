import { invoke } from "@tauri-apps/api/core";
import { dataDir} from '@tauri-apps/api/path';
import { reactive } from 'vue';
import { audioStore, AudioFile } from "./AudioFile";

// Define the type for the AudioFile object
export interface Genre {
  genreId: number;
  genreName: string;
  genreThumbnail: string;
  dateCreated: string;
  lastModified: string;
}

export class Genre {
  genreId: number;
  genreName: string;
  genreThumbnail: string;
  dateCreated: string;
  lastModified: string;

  constructor({
    genreId,
    genreName,
    genreThumbnail,
    dateCreated,
    lastModified,
  }: {
    genreId: number;
    genreName: string;
    genreThumbnail: string;
    dateCreated: string;
    lastModified: string;
  }) {
    this.genreId = genreId;
    this.genreName = genreName;
    this.genreThumbnail = genreThumbnail;
    this.dateCreated = dateCreated;
    this.lastModified = lastModified;
  }
}

export const genreStore = reactive({

  genres: [] as Genre[],
  audioFiles: [] as AudioFile[],

  async loadGenres() {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const genres = await invoke<Genre[]>('view_genres', { userDbPath});
        this.genres = genres;

    } catch(error) {
        console.error("Error loading genres: ", error); 
    }
  },

  async createGenre(genreName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("create_genre", { 
        genreName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async editGenre(genreId: string, genreName: string, userLocalThumbnailPath: string): Promise<string> {

    const dataDirPath = await dataDir(); 
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 
    const thumbnailFileName = userLocalThumbnailPath.replace(/^.*[\\/]/, '');

    return await invoke("edit_genre", { 
        genreId,
        genreName,
        thumbnailFileName,
        userLocalThumbnailPath,
        userThumbnailPath,
        userDbPath
    });
  },

  async deleteGenre(genreId: string): Promise<string> {

    const dataDirPath = await dataDir();
    const userThumbnailPath = `${dataDirPath}/dtunes-audio-app/images`; 
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`; 

    return await invoke("delete_genre", { 
      userDbPath, 
      userThumbnailPath, 
      genreId
    });
  },

  async viewGenre(genreId: string): Promise<Genre> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("view_genre", { userDbPath, genreId });
  },

  async viewGenreAudioFiles(genreId: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>('view_genre_audio_files', { userDbPath, genreId});
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files: ", error); 
    }
  },

  async addAudioFileGenre(genreId: string, audioFileId: Number): Promise<Genre> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("add_audio_file_genre", { 
        userDbPath, 
        genreId, 
        audioFileId 
    });
  },

  async removeAudioFileGenre(genreId: string, audioFileId: Number): Promise<Genre> {
    const dataDirPath = await dataDir();
    const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
    return await invoke("remove_audio_file_genre", { 
        userDbPath, 
        genreId, 
        audioFileId 
    });
  },

  async searchGenres(searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const genres = await invoke<Genre[]>('search_genres', { userDbPath, searchTerm});
        this.genres = genres;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },

  async searchGenreAudioFiles(genreId: string, searchTerm: string) {
    try {
        const dataDirPath = await dataDir(); 
        const userDbPath = `${dataDirPath}/dtunes-audio-app/metadata/dtunes-audio-app.sqlite3`;
        const audioFiles = await invoke<AudioFile[]>(
          'search_genre_audio_files', 
          { userDbPath, genreId, searchTerm}
        );
        this.audioFiles = audioFiles;

    } catch(error) {
        console.error("Error loading audio files from search term: ", error); 
    }
  },


  async queueGenreAudioFiles(genreId: string, audioFileId: string) {

    let audioFile = await audioStore.viewAudioFile(audioFileId);
    audioStore.queuedAudioFiles.push(audioFile);

    await this.viewGenreAudioFiles(genreId);
    this.audioFiles.forEach( (audioFile) => {
      audioStore.queuedAudioFiles.push(audioFile);
    });

    audioStore.queueIndex = 0; 
  },

}); 