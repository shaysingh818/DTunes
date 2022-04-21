# SQLITE3 Object Relational Mapper
To avoid writting the same database methods tediously, I'll be creating a ORM bult for sqlite3 in C. This is meant to act as a requirements document for what features/methods need to be implemented in order for the ORM to be fully modular. 

## How it will work
The orm will allow CRUD (Create, Update, Delete, Read) methods to be inherited across all structural methods defined. The user can define a struct for the database model, and then wrap it using specific database methods. This will make it so that CRUD methods won't ahave to be implemented for each model in the system. The goal is to eliminate having files with too many lines for one entity. In the sections below, we'll look at some functionalities that are tedious and how this library can remove extra definitions. 


## Creating An Abstract "Model"
1. We need a model structure that can take in any type, the CRUD methods can be applied to this model. 

# Create
The method to create an instance requires, a connection to the database file, a query to create an instance of the model, and fields to bind the needed parameters. 

1. The create method needs to be able to generate the INSERT query for whatever structure we define. If we define a song model with the fields, name, date, artist, the ORM needs to convert all those attributes to an insert query. The name of the table that gets generated should be the name of the typedef struct defined in the headerfile. 

2. The create method should also be able to generate the ```sqlite3Bind()``` commands needed to bind the fields from the structure. The method should be able to know and detect the datatypes of all the fields. 


# Read
1. The read method will generate a SELECT ALL query similar to the INSERT query for the create method. When the model is defined, it should automatically allocate the required space needed from the database. It will use the generated DB query for selecting all instances. Similar to the insert method, it will detect the datatypes for all the fields and render them to the array of structures allocated. 

2. The read method will also return struct arrays based on different data structures. We might want to use a doubly/singly linked list for traversing items in a certain order. For priority or scheduling, we can use different types of queues when rendering the struct array from the read method. 

# Update





# Delete All



# Delete

