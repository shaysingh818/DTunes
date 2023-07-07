pub mod playlist;
pub mod audio_file;
pub mod pomodoro;
pub mod source; 


extern crate clap;
use clap::{App, Arg};

fn main() {

    println!("");
 
    let _matches = App::new("DTunes CLI")
        .author("Shay Singh")
        .about("Command line audio player with pomodoro timer")
        .arg(
            Arg::new("playlist")
                .short('p')
                .long("playlist")
                .help("work with playlist on DTunes")
        )
        .arg(
            Arg::new("pomodoro")
                .short('o')
                .long("pomodoro")
                .help("Create pomodoro session on DTunes")
        )
        .arg(
            Arg::new("file")
                .short('f')
                .long("file")
                .help("View audio files on DTunes")
        )
        .arg(
            Arg::new("source")
                .short('s')
                .long("source")
                .help("Source audio files on DTunes")
        )
        .version("0.1.0")
        .get_matches();



}