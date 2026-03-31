use tauri::{State}; 
use std::sync::Mutex;
use std::error::Error;
use std::time::Duration;
use rodio::queue::SourcesQueueOutput; 
use rodio::{
    DeviceSinkBuilder, 
    MixerDeviceSink,
    Player, 
    Decoder,
};


pub struct PlayerState {
    mixer: MixerDeviceSink,
    player: Player,
    queue: SourcesQueueOutput,
    total_duration: f64
}

impl PlayerState {

    pub fn new() -> PlayerState {

        let (new_player, queue) = Player::new();

        PlayerState {
            mixer: DeviceSinkBuilder::open_default_sink().unwrap(),
            player: new_player,
            queue: queue,
            total_duration: 0.0,
        }

    }

    pub fn is_playing(&self) -> bool {
        self.player.len() > 0
    }
 
    pub fn is_paused(&self) -> bool {
        self.player.is_paused()
    }

    pub fn get_current_duration(&self) -> u64 {
        let duration = self.player.get_pos();
        duration.as_secs()
    }

    pub fn set_total_duration(&mut self, duration: f64) {
        self.total_duration = duration;
    }

    pub fn forward_duration(&mut self) {

        let curr_durr = self.get_current_duration();
        
        if curr_durr as f64 > self.total_duration {
            println!("Hitting this condition (forward)");
            self.stop_audio();
        } else {
            let next = curr_durr + 30;
            let durr_obj = Duration::from_secs(next); 
            match self.player.try_seek(durr_obj) {
                Ok(()) => println!("Seeked to {:?}", next),
                Err(e) => eprintln!("Seek failed: {}", e),
            }
        }
    }

    pub fn rewind_duration(&mut self) {

        let curr_durr = self.get_current_duration();

        if curr_durr < 30 {
            println!("Hitting this condition");
            self.stop_audio();
        } else {
            let next = curr_durr - 30;
            let durr_obj = Duration::from_secs(next); 
            match self.player.try_seek(durr_obj) {
                Ok(()) => println!("Seeked to {:?}", next),
                Err(e) => eprintln!("Seek failed: {}", e),
            }
        }
    }

    pub fn queue_audio_file(&mut self, file_name: &str) {
        println!("queueing audio file: {:?}", file_name); 
        self.player = Player::connect_new(self.mixer.mixer());
        let file = std::fs::File::open(file_name).unwrap(); 
        self.player.append(Decoder::try_from(file).unwrap());
    }

    pub fn play_audio(&mut self, file_name: &str, duration: f64) {
        if !self.is_playing() {
            self.queue_audio_file(file_name); 
            self.set_total_duration(duration);
        }
        self.player.play();
    }

    pub fn pause_audio(&mut self) {
        self.player.pause(); 
    }

    pub fn stop_audio(&mut self) {
        self.player.stop();
    }

}


#[tauri::command]
pub fn play_audio(file_path: &str, duration: f64, state: State<'_, Mutex<PlayerState>>) -> Result<(), String> {

    let mut state = state.lock().unwrap();
    state.play_audio(file_path, duration); 

    Ok(())
}


#[tauri::command]
pub fn pause_audio(state: State<'_, Mutex<PlayerState>>) -> Result<(), String> {

    let mut state = state.lock().unwrap();
    state.pause_audio(); 

    Ok(())
}


#[tauri::command]
pub fn stop_audio(state: State<'_, Mutex<PlayerState>>) -> Result<(), String> {

    let mut state = state.lock().unwrap();
    state.stop_audio(); 

    Ok(())
}


#[tauri::command]
pub fn is_playing(state: State<'_, Mutex<PlayerState>>) -> bool {

    let mut state = state.lock().unwrap();
    state.is_playing() 
}


#[tauri::command]
pub fn is_paused(state: State<'_, Mutex<PlayerState>>) -> bool {

    let mut state = state.lock().unwrap();
    state.is_paused() 
}


#[tauri::command]
pub fn curr_duration(state: State<'_, Mutex<PlayerState>>) -> u64 {

    let mut state = state.lock().unwrap();
    state.get_current_duration() 
}


#[tauri::command]
pub fn forward_duration(state: State<'_, Mutex<PlayerState>>) {

    let mut state = state.lock().unwrap();
    state.forward_duration() 
}


#[tauri::command]
pub fn rewind_duration(state: State<'_, Mutex<PlayerState>>) {

    let mut state = state.lock().unwrap();
    state.rewind_duration() 
}


