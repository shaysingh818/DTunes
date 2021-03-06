#include "breakpoint.h"
#include "../logging/log.h"




float larger(float first, float second){
    if(first < second){
        return second;
    }else{
        return first;
    }
}


breakpoint_t maxpoint(const breakpoint_t* points, long npoints){
    // create a temp breakpoint
    breakpoint_t point;
    point.time = points[0].time;
    point.value = points[0].value;
    // check for largest point
    for(int i = 0; i < npoints; i++){
        if(point.value < points[i].value){
            point.value = points[i].value;
            point.time = points[i].time;
        }
    }
    return point;
}



void retrieveBreakpoint(breakpoint_t *points, char *line, int count){
    int retrieveStatus;
    retrieveStatus = sscanf(
        line,
        "%lf%lf",
        &points[count].time, 
        &points[count ].value
    );

    switch(retrieveStatus){
        case(0):
            dlog_int("NON NUMERIC DATA AT LINE", count+1); 
            break;
    
        case(1):    
            dlog_int("INCOMPLETE BREAKPOINT FOUND AT LINE", count+1); 
            break; 
        
        case(2):
            dlog_int("RETRIEVED BREAKPOINT", count+1); 
            break;          
    } 
}



breakpoint_t* getBreakpoints(FILE *fp, long* psize){

    int got;
    long npoints = 0, size = 64;
    double lasttime = 0.0;
    breakpoint_t* points = NULL;
    char line[80]; // 80 is the line size
    // check if file pointer is empty
    if(fp==NULL){
        return NULL;
    }

    // allocate breakpoints for size of file
    points = (breakpoint_t*)malloc(sizeof(breakpoint_t) * size);
    if(points == NULL){
        return NULL;
    }

    while(fgets(line, 80, fp)){

        retrieveBreakpoint(points, line, npoints);
        if(points[npoints].time < lasttime){
            dlog_int("TIME NOT INCREASING AT LINE", npoints+1);
            break;
        }

        lasttime = points[npoints].time;
        if(++npoints == size){
            breakpoint_t* tmp;
            size += NPOINTS;
            tmp = (breakpoint_t*)realloc(points, sizeof(breakpoint_t) * size);
            if(tmp == NULL){
                npoints = 0;
                free(points);
                points = NULL;
                break;
            }
            points = tmp;
        }
    }

    if(npoints){
        *psize = npoints;
    }

    return points;
}


