#include "db.h"
#include "db_endpoints.h"
#include "db_test.h"


// view playlist
void testDbFunctions(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
		runDbTests(); 
    }
}


void dbEndpoints(endpoint_t **head){
	// testing database repository endpoint
    endpoint_t *e1 = createEndpoint(
        "test-database-repository",
        "tdb",
        "test database repository"
    );
    e1->endpointLogic = testDbFunctions;
    appendEndpoint(head, e1);

}
