# Phaedra
Phaedra is low latency audio player designed for queueing, listening and proccessing audio files in the background. Similar to TinyAudioLib, Phaedra is built on top of portaudio and uses the portsf for decoding and streaming audio frames. With this library, the user has the option to use an audio callback or use the blocking API. When using TinyAudioLib, there were issues with streaming Mono/Stereo files and the the sample type isn't automatically detected. Phaedra is made to avoid these issues and automatically detect the proper framereading functions for wav files. 

## Audio File Streaming
These are the main requirements for being able to stream/play and audio file. This design requirement is for handeling audio signals with various formats. 

1. **Setting Input/Output Device**: To stream a wav file, the input device needs to be set. Phaedra will set the input device to the default. When selecting the input device, there needs to be logic to select the suggested latency. For the input device, there are specific fields that need to be filled. The fields below are for the input device, for the output device, they stay the same but use the default output device instead of input. 

| IO Parameter              | Value                                                       |
|---------------------------|-------------------------------------------------------------|
| device                    | Uses the default input device from portaudio                |
| channelCount              | Set to 2 for stereo, 1 for mono wav files.                  |
| sampleFormat              | Set to float32 for now, varies based on the file contents   |
| suggestedLatency          | selected based on logic, default is 0.2 (not entirely sure) |
| hostApiSpecificStreamInfo | Set to NULL for now                                         |

2. **Creating A Stream**: For creating an audio stream, the input and output device parameters need to be set along with initializing the portaudio library. Once the default input and output device are chosen, there is logic to handle whether the use wants to stream audio using a callback, or the iterative method. If the user wants to stream using a callback, a callback function and  data parameters must be supplied when opening the stream. At the moment, there isn't a more effiecient way to handle a streaming function without a callback. If a user wants to stream iterativley, they supply no data and set the callback boolean to "false".  	

3. **Accounting for different sample types**: When streaming an audio file, we need to know what bit/sample rate the file is. The wav file can be either of these formats, 8, 16, 24, 32 and 32 floating with IEEE format. If the format does not contain any of these numbers, it should not be streamable to the DAC. There should be hardcoded logic to check the sample rate and prevent the file from being played if it doesn't meet this criteria. 

4. **Duration Tracking**: When the audio file is being streamed, it should indicate it's current time value. As the file is being played, the duration values of the file should be stored locally for access. The user should be able to skip to any time duration in the audio file, and stream the content at the desired time value. These are some basic functionalites that should be available to the user
	* Grab the entire time duration of the wav file
	* Play audio file at certain time value
	* Skip any amount of time beind or ahead of the real time stream
	* Pause the stream at certain time duration
	* Stop at certain time duration 

## Audio Queueing
When streaming multiple files, there needs to be data structures that handle the order of how audio files are played. The use of data structures serve as a way to expiriment with optimal memory usage.  This audio queueing component is needed for recieving audo file links from the database and placing them in whatever order the user desires. 




## Accounting for multiple compression formats
Put together a list of all possible audio file formats that we might need. Use the portsf sample type checking function to see what bit rate the file is. Refer to this website for all possible compression formats: http://mauvecloud.net/sounds/index.html







## Autoplay




