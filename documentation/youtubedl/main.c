#include "libs/db.h"
#include "libs/urls.h"
#include <stdbool.h> 


void exportUrlsToCsv(char *fileName){
	// export db urls to csv file
	FILE *fp; 
	fileName = strcat(fileName, ".csv"); 
	int urlLimit = getUrlTableSize(); 
	url_t **urls = initUrls(urlLimit); 
	url_t ***p = &urls; 
	
	fp = fopen(fileName, "w+");
	fprintf(fp, "%s, %s", "DATE_CREATED", "YOUTUBE_URL"); 
	for(int i = 0; i < urlLimit; i++){
		fprintf(fp, "\n%s, %s", (*p)[i]->dateCreated, (*p)[i]->url); 
	}
	fclose(fp);
}


const char* getCsvField(char *line, int num){
	const char* token; 
	for(token = strtok(line, ",");
		token && *token;
		token = strtok(NULL, ",\n")){
		if(!--num){
			return token; 
		}
	}
	return NULL; 
}


void importCsvToDB(char *fileName){
	FILE *fp; 
	fp = fopen(fileName, "r");
	char row[1024]; 
	while(feof(fp) != true){
		fgets(row, 1024, fp); 
		printf("Row: %s\n", row); 
	}
}


int main(int argc, int **argv){

	char test[40] = "testExport"; 	
	//exportUrlsToCsv(test);
	importCsvToDB("testExport.csv");  
}
