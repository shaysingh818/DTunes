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

2. **Creating Multiple Types of Audio Streams**: For creating an audio stream, the input and output device parameters need to be set along with initializing the portaudio library. The stream should also be started when it's created. We need to create multiple types of streams and play audio frames using different techniques. We'll first want a stream that can utilize a callback function. In the callback function, data parameters from portsf need to be supplied. For iterative streaming, we'll need a stream that doesn't utilize a callback function and uses the blocking API. As a result of the requirements, there's a couple additions needed to phaedra.
	* Create a streaming function that uses a callback
	* Stream audio file iteratively without callback

3. **Accounting for different sample types**: When streaming an audio file, we need to know what bit/sample rate the file is. The wav file can be either of these formats, 8, 16, 24, 32 and 32 floating with IEEE format. If the format does not contain any of these numbers, it should not be streamable to the DAC. There should be hardcoded logic to check the sample rate and prevent the file from being played if it doesn't meet this criteria. 

4. **Duration Tracking**: When the audio file is being streamed, it should indicate it's current time value. As the file is being played, the duration values of the file should be stored locally for access. The user should be able to skip to any time duration in the audio file, and stream the content at the desired time value. These are some basic functionalites that should be available to the user. There should also be basic audio controls included for the player. 
 	* **Stop**: Stop the current queue entirely
	* **Pause**: Pause the wav file
	* **Select Duration**: Rewind to a certain duration point in the file
	* **Play**: Play the audio file
	* **Skip**: Skip to the next song in the queue  


## Audio Queueing
When streaming multiple files, there needs to be data structures that handle the order of how audio files are played. The use of data structures serve as a way to expiriment with optimal memory usage.  This audio queueing component is needed for recieving audo file links from the database and placing them in whatever order the user desires. 


**Singular Queues**: 

**Circular Queues**: Circular queues are continious. The last element of the queue should be connected to the first node in the queue. The purpose for a circular queue is to create a continous autoplay in the background. We'll want our player to keep playing songs in the background until the user manipulates the stream using the audio controls. 
	* Create a circular queue for phaedra
	* If possible, merge circular/singular queue together


### Phaedra Audio Queue
This is the design implementation for the phaedra audio queue. This will cover the implementation for the linked list and array based queue. This queue is responsible for playing audio files in the order specified by the user. 

1. **Operations for AUDIO queue**: These are the types of insert operations that will exist for the linked list and array based queue.
	* **initQueue**: Create initial queue with size of 0
	* **enqueue**: Add audio file to the queue
	* **dequeue**: Remove item from the queue
	* **front**: Get first item in queue
	* **rear**: Get item at the end of the queue


## Server/Client Remote Streaming










