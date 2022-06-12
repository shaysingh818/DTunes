# Phaedra
Phaedra is low latency audio player designed for queueing, listening and proccessing audio files in the background. Phaedra is a submodule to DTunes, and is the heart of playing audio files. 

## Audio File Streaming
These are the main requirements for being able to stream/play and audio file. This design requirement is for handeling audio signals with various formats. 

1. Audio file frequencies will be read and passed to the portaudio library. 
	* Deconstruct the tinyaudiolib module
	* Create a user defined function to select default DAC device
	* If needed, handle scenarious where various file formats are supplied (MP3, WAV, AIFF)
	* Stream the audio file to the DAC

2. Capture real time streaming values
	* As the file is being played, keep track of the current time duration
	* Use breakpoint files/signals to keep transparency around the time duration. 


## Queueing




## Autoplay




