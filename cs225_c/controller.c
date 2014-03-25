#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <math.h>
#include "gps_reader.h"

void start() {

    FILE * file_1 = fopen(FILE_NAME_1, "r");
    FILE * file_2 = fopen(FILE_NAME_2, "r");
    if (file_1 == NULL || file_2 == NULL) {
        printf("Invalid file name(s).");
        return;
    }

    stream_t strm_1, strm_2; // stream 1 and stream 2 
    strm_1.satelitesOK = 0;
    strm_2.satelitesOK = 0;


    sync_satelites(file_1, file_2, &strm_1, &strm_2);
    sync_time_gps(file_1, file_2, &strm_1, &strm_2);

    node_t * head = NULL;
    
        while (1) {

            if (strm_1.satelitesOK) {
                add_element(head,strm_1.location);
                


            }


        }
     

/*
    printf("%d", strm_1.satelitesOK);
    printf("%d", strm_2.satelitesOK);

    printf("\n %s", asctime(&strm_1.location.time));
    printf("\n %s", asctime(&strm_2.location.time));
*/
}

int sync_time_gps(FILE * file1, FILE * file2, stream_t * strm_1, stream_t * strm_2) {
    int line_read = 0;

    while (line_read != GPS_TIME) {
        line_read = read_line(file1, strm_1);
    }

    line_read = 0;
    while (line_read != GPS_TIME) {
        line_read = read_line(file2, strm_2);
    }

    time_t time_1 = mktime(&strm_1->location.time);
    time_t time_2 = mktime(&strm_2->location.time);

    double time_difference = difftime(time_1, time_2);

    if (time_difference < 0) {
        while (time_difference != 0) {
            line_read = 0;
            while (line_read != GPS_TIME) {
                line_read = read_line(file1, strm_1);
            }
            time_1 = mktime(&strm_1->location.time);
            time_2 = mktime(&strm_2->location.time);
            time_difference = difftime(time_1, time_2);
        }
    } else if (time_difference > 0) {
        while (time_difference != 0) {
            line_read = 0;
            while (line_read != GPS_TIME) {
                line_read = read_line(file2, strm_2);
            }
            time_1 = mktime(&strm_1->location.time);
            time_2 = mktime(&strm_2->location.time);
            time_difference = difftime(time_1, time_2);
        }
    }
    return EXIT_SUCCESS;

}

int sync_satelites(FILE * file1, FILE * file2, stream_t * strm_1, stream_t * strm_2) {

    int line_read = 0;


    while (line_read != SATELITE) {
        line_read = read_line(file1, strm_1);
        if (strm_1->satelitesOK != 0) {
            break;
        }
    }
    line_read = 0;
    while (line_read != SATELITE) {
        line_read = read_line(file2, strm_2);
        if (strm_2->satelitesOK != 0) {
            break;
        }
    }

    return EXIT_SUCCESS;

}
