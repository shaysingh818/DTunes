# MP3 Dataset Generation Library Documentation

```c 
struct ThreadArg {
    url_t** urlSubArr;
    int urlLimit;
    int threadId;
    pthread_t urlThread;
    struct ThreadArg* nextThread;
};

typedef struct ThreadArg thread_arg;
```

The thread argument structure is responsible for storing information that we want to pass to multiple threads. There are five important fields that it holds. It stores the subarray of urls that we want to download from the database, the size of the subarray, the current thread id, the pthread instance that we want to spawn, and lastly, the pointer reference to the next thread argument in the linked list. 

```c 
thread_arg *buildThreadArgument(url_t **subArr, int size, int tid)
``` 

**Purpose**: The build thread argument method creates a memory instance of thread_arg structure. The user provides parameters to the method that will populate the structure field. 
