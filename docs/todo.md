## Short term tasks for V.10

1. Write test cases for every module. There needs to be test cases for the playlist, audiofile, database and url library. Every function in the system should have test cases so it's easier to debug when problems come up. 
	* Changed song test cases to use uuid in memory instance
	* create a function that generates a uuid
	* Create a routine that combines all the test cases for the system. 


2. Document every single function in the system. This should be done, once there are test cases for each function. Most of the stuff that has been written is very trivial (CRUD, Downloading URlS). 

3. Write down future ideas, algorithms you want to add to each library. Keep a document for each module that stores the features that you want to ship. 
	* Stereo Panning
	* Sound Generation

4. Create a modular way of being able to see how many sample frames are in a file
	* Split the portsf boiler plate code up into functions
	* Modularize the template overall

## Sound creation

Audio concepts to implement from this tool: https://www.leshylabs.com/apps/sfMaker/ 

2. Wave Type
	* Sine
	* Square
	* Sawtooth
	* Noise
	* Foo

3. Sample rate 
	* Ranges from 8000-48000

4. Frequency Types
	* Start Frequency
	* Vibrato Frequency
	* Vibrato Depth
	* Tremelo Frequency
	* Tremelo Depth
	* Square/Saw Duty
	* Duty Sweep Frequency
	

	
