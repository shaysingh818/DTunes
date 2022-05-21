## Short term tasks for V.10


1. Write test cases for every module. There needs to be test cases for the playlist, audiofile, database and url library. Every function in the system should have test cases so it's easier to debug when problems come up. 
	* Changed song test cases to use uuid in memory instance
	* create a function that generates a uuid


2. Document every single function in the system. This should be done, once there are test cases for each function. Most of the stuff that has been written is very trivial (CRUD, Downloading URlS). 

3. Write down future ideas, algorithms you want to add to each library. Keep a document for each module that stores the features that you want to ship. 


## Sound creation

1. https://www.leshylabs.com/apps/sfMaker/



## Breakpoint Files

1. Create a stable library that can read and write breakpoint files. The library should be able to check for the maximum time value in a breakpoint file. The file should contain only a time and a value of some sort. The library should validate the following. 
	* Format of the breakpoint file. Should only contain time and value
	* Ensure that the time value is increasing each time
	* Make sure there are no negative values in the file. 
	
