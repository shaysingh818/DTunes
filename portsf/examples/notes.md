# Concepts and Examples


## Amplitude Factor
1. Increadsing volume and scale of a wav file
2. Ampltidue with decibal scalin

## Stereo Panning
1. Stereo panning left and right speakers with simple panning function
2. Stereo panning with constant power function
3. Adding breakpoint file support for stereo panning


## Adding Breakpoints for Envelope Processing
1. Revisit the rest of chapter 2, once audio streaming works


## Audio Streaming

1. Real time player doesn't work. 
	* Error with Jack and ALSA dev kit
	* Jack server can't connect so none of the tiny audio lib files work
	* Use minaudio header file as a workaround for playing audio files

2. Output wav file with specifying start/end time duration. 
	* Play certain time slices of wav files
	* Output time slice using portsf


## Digital Audio Signals

**Signals**: Information carriers, functions that can describe information. 
	* Time is continous variable through the function
	* Can build function tables out of a signal

**Quantization**: Finding a numerical value that will represent the input signal
	* Digital conversion: Audio coding

**Sinusoids**: Simplest waveform
	* given to waves that have a particular shape
	* outputs are sine and cosine


