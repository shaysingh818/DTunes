#ifndef URLS_ENDPOINTS_H
#define URLS_ENDPOINTS_H

#include "../endpoints/endpoints.h"

void insertYoutubeUrlCmd(endpoint_t *e, char* argv[]);
void viewYoutubeUrlsCmd(endpoint_t *e, char* argv[]);
void deleteYoutubeUrlCmd(endpoint_t *e, char* argv[]);
void deleteAllYoutubeUrlsCmd(endpoint_t *e, char* argv[]);

// youtube url backup
void YTBackup(endpoint_t *e, char* argv[]);
void testUrlsLibrary(endpoint_t *e, char* argv[]);

void urlEndpoints(endpoint_t **head);
#endif
