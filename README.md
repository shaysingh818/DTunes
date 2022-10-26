# DTunes: 
DTunes is an audio engine for researching audio composition and processing techniques. The purpose of this engine is to understand music production/theory concepts and combine it with computational algorithms. A list of features include reading audio frequencies and perform common audio processing techniques, audio composition (composing music), general audio feature extraction, and experimenting with ways to play audio files stored in queuing data structures.  The purpose of this project is to understand music composition and create the ultimate audio experience from the perspective of a computer scientist or researcher.

## Setup

1. Run the install.sh file. 
```
bash install.sh
```

2. Run the make file
```
make
```

3. Execute the CLI tool and use the help command.
```
./dtunes help
```

```
 (                      )       (     
 )\ )   *   )        ( /(       )\ )  
(()/( ` )  /(    (   )\()) (   (()/(  
 /(_)) ( )(_))   )\ ((_)\  )\   /(_)) 
(_))_ (_(_()) _ ((_) _((_)((_) (_))   
 |   \|_   _|| | | || \| || __|/ __|  
 | |) | | |  | |_| || .` || _| \__ \  
 |___/  |_|   \___/ |_|\_||___||___/  


                              

============================================================
collections               Collections to store audio files in DTunes
phaedra                   Audio player module for DTunes

```

4. You should see two modules, collections and phaedra. To get help on how to use the modules, type “help” after the module name.

```
./dtunes collections help
./dtunes phaedra help
```


## Bulk Syncing Audio Files to DTunes

1. Create a collection with a name of your choice. 
```
./dtunes collections create testing
```


2. Change directory to ```adms/scripts```. Go into to the data.json file and paste in the following. 
```json
{
    "collections": {
        "collection_testing": {
            "path": "../../collections/collection-testing",
            "videos": [
                "https://www.youtube.com/watch?v=tLJH_NtkWBw",
                "https://www.youtube.com/watch?v=5wrKDFIJZuA",
                "https://www.youtube.com/watch?v=-heXdwWVr3I",
                "https://www.youtube.com/watch?v=oj8_wufhE28"
            ]
        }
    }
}
```

3. Change directory to the dtunes binary, check if there are files in the testing collection. 
```
./dtunes collections vcf testing
```

4. If files show up in that collection, queue the files with phaedra
```
./dtunes collections queue testing
```





