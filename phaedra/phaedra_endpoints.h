#ifndef PHAEDRA_ENDPOINTS_H
#define PHAEDRA_ENDPOINTS_H

#include "endpoints/endpoints.h"

// phaedra functions
void playAudioCmd(endpoint_t *e, char* argv[]);
void playAudioCallbackCmd(endpoint_t *e, char* argv[]);
void checkSampleTypeCmd(endpoint_t *e, char* argv[]);
void testSDLQueue(endpoint_t *e, char* argv[]); 
void phaedraEndpoints(endpoint_t **head); 

#endif


