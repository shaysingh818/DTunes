#ifndef PHAEDRA_ENDPOINTS_H
#define PHAEDRA_ENDPOINTS_H

#include "../endpoints/endpoint_refactor/endpoints.h"

// phaedra functions
void playAudioCmd(endpoint_t *e, char* argv[]);
void playAudioCallbackCmd(endpoint_t *e, char* argv[]);
void writeToQueue(endpoint_t *e, char* argv[]); 
void testPhaedra(endpoint_t *e, char* argv[]); 
void phaedraEndpoints(endpoint_t **head); 

#endif


