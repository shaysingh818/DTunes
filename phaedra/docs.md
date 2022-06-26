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
When streaming multiple files, there needs to be data structures that handle the order of how audio files are played. The use of data structures serve as a way to expiriment with optimal memory usage.  This audio queueing component is needed for recieving audo file links from the database and placing them in whatever order the user desires. For each data structure, there will be a contiguous and noncontiguous version created. 

**Queue Data Structures**: For queueing multiple audio files on phaedra, we'll need to utilize some data stuctures. A portion of the data structures will be soley reposible for in memory storage whereas others will be used for optimization and reccomendation. These are some the data structures we'll use for queueing multiple audio files
	* **Linked List Based Queue**: Noncontiguous data structure
	* **Array Based Queue**: Contiguous data structure
	* **Graphs**: For reccomendation and playing audio files with realationships
	* **Matrices**: For state vector represenation of the audio environment

**Input Files for Queueing Structures**: The queue operations and commands will be saved in csv files containing the queue operation, wav file link, and date/time values of command. For each data structure, csv file will be generated for storing queue commands. When a specific data structure is chosen for queueing, it's corresponding csv file will be used as input to populate the queue. 


2. **Loading data into the Queue**: Using the input queue files, the system should read all the file streaming links from the input file and populate the queue/buffer accordingly. The purpose of the file is to test the in memory audio queueing operations. 

### Phaedra Audio Queue
This is the design implementation for the phaedra audio queue. This will cover the implementation for the linked list and array based queue. This queue is responsible for playing audio files in the order specified by the user. 

1. **Operations for AUDIO queue**: These are the types of insert operations that will exist for the linked list and array based queue.
	* **initQueue**: Create initial queue with size of 0
	* **enqueue**: Add audio file to the queue
	* **dequeue**: Remove item from the queue
	* **front**: Get first item in queue
	* **rear**: Get item at the end of the queue
	* **writeCommand**: Operation for writting queue commands to a file

2. **Playing files through the queue**: Once the queue is populated from the input file, the audio library should start streaming files in the specified order. There needs to be method that iterates through every item in the queue and plays each file. When a song is done playing, we should check the current commands in the file in case anything changes and repopulate the queue. 

## Accounting for multiple compression formats
Put together a list of all possible audio file formats that we might need. Use the portsf sample type checking function to see what bit rate the file is. Refer to this website for all possible compression formats: http://mauvecloud.net/sounds/index.html










