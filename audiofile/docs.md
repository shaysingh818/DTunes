# Phaedra
Phaedra is low latency audio player designed for queueing, listening and proccessing audio files in the background. Phaedra is a submodule to DTunes, and is the heart of playing audio files. Phaedra is a competitor to another library called "TinyAudioLib". Both of these libraries are built on top of PortAudio. 

## Audio File Streaming
These are the main requirements for being able to stream/play and audio file. This design requirement is for handeling audio signals with various formats. 

1. Audio file frequencies will be read and passed to the portaudio library. 
	* Deconstruct the tinyaudiolib module
	* Use audio CallBack and blocking API (Have option to use both)
	* Create a user defined function to select default DAC device
	* If needed, handle scenarious where various file formats are supplied (MP3, WAV, AIFF)
	* Stream the audio file to the DAC

2. Capture real time streaming values
	* Using the block api or callback, remove unwanted terminal output
	* As the file is being played, keep track of the current time duration
	* Use breakpoint files/signals to keep transparency around the time duration. 


## Sound File Formatting and Conversion
Put together a list of all possible audio file formats that we might need. Use the portsf sample type checking function to see what bit rate the file is. 

1. Create a user defined function that tells us the type of wav file
	* Use this website: http://mauvecloud.net/sounds/index.html
	* Make phaedra be able to interpret all these compression formats
	* Cross reference the table and display the information in checkSampleTypeCmd()

2. Find a list of all possible audio file formats
	* MP3, WAV, AIFF etc
	* Learn how to write functions that can interpret these 
	* Make interpreter convert it to float form


## Queueing



## Autoplay


## Endpoint integration
Use the CLI endpoint library and set up test endpoints to use certain functions of the library. Down below is a list of possible endpoints we could use to test the phaedra streaming library. 

1. Play audio file with wav file as argument
	* Take in command line arg with audio file

2. Decode and check audio metadata/sampletype
	* Give the sample rate of the audio file



