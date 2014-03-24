

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "gps_reader.h"
#include <math.h>

void start() {
    FILE * file1;
    FILE * file2;
    stream_t strm_1,strm_2;// stream 1 and stream 2
    strm_1.satelitesOK = -1 ;
    strm_2.satelitesOK = -1 ;

    file1 = fopen(FILE_NAME_1, "r");
    file2 = fopen(FILE_NAME_2, "r");
    if (file1 == NULL || file2 == NULL) {
        printf("Invalid file name(s).");
        return;
    }
    synchronize(file1, file2,&strm_1,&strm_2);
    printf("%d",strm_1.satelitesOK);
    printf("%d",strm_2.satelitesOK);
     

}

void synchronize(FILE * file1, FILE * file2, stream_t * strm_1, stream_t * strm_2) {
    char line_read_1[STRING_SIZE] = "";   

    while (strcmp(line_read_1, GPS_TIME) != 0) {
        strcpy(line_read_1, read_line(file1, strm_1));
    }
    char line_read_2[STRING_SIZE] = "";
    while (strcmp(line_read_2, GPS_TIME) != 0) {
        strcpy(line_read_2, read_line(file2, strm_2));
    }
    
    time_t time_1 = mktime(&strm_1->location.time);
    time_t time_2 = mktime(&strm_2->location.time);

    double time_difference = difftime(time_1, time_2);

    if (time_difference < 0) {
        while (time_difference != 0) {
            line_read_1[0] = '\0';
            while (strcmp(line_read_1, GPS_TIME) != 0) {
                strcpy(line_read_1, read_line(file1, strm_1));
            }
            time_1 = mktime(&strm_1->location.time);
            time_2 = mktime(&strm_2->location.time);
            time_difference = difftime(time_1, time_2);
        }
    } else if (time_difference > 0) {
        while (time_difference != 0) {
            line_read_2[0] = '\0';
            while (strcmp(line_read_2, GPS_TIME) != 0) {
                strcpy(line_read_2, read_line(file2, strm_2));
            }
            time_1 = mktime(&strm_1->location.time);
            time_2 = mktime(&strm_2->location.time);
            time_difference = difftime(time_1, time_2);
        }
    }


}

char * read_line(FILE * file, stream_t * strm) {
    char buffer[BUFFER_SIZE];
    char current_line[BUFFER_SIZE];

    char * status = fgets(buffer, BUFFER_SIZE, file);
    strcpy(current_line, buffer);
    if (status != NULL) {
        char * current = strtok(buffer, ",");
        if (strcmp(current, "$GPRMC") == 0) {
            proccessRMC(buffer, strm);
            return GPS_TIME;
        } else if (strcmp(current, "$GPGSV") == 0) {

            char * current = strtok(NULL, ",");
            int num_lines = atoi(current);
            char lines[num_lines][BUFFER_SIZE];

            strcpy(lines[0], current_line);
            int i;
            for (i = 1; i < num_lines; i++) {

                fgets(buffer, BUFFER_SIZE, file);
                strcpy(lines[i], buffer);

            }
            proccessGSV(lines, num_lines, strm);
            return SATELITE;


        }


    }
    return _EOF;

}

void proccessGSV(char lines[][BUFFER_SIZE], int num_lines, stream_t * data) {
    int num_satelites;
    int i = 0, o = 0, j = 0, k = 0;

    char first_line[BUFFER_SIZE];
    strcpy(first_line, lines[0]);
    char * current = strtok(first_line, ",");
    for (i = 0; i < 3; ++i) {
        current = strtok(NULL, ",");
    }
    num_satelites = atoi(current);
    int count = 0;
    for (i = 0; i < num_lines; ++i) {
        char * asterix_getter = strtok(lines[i], "*");
        char * current = strtok(asterix_getter, ",");
        for (o = 0; o < 3; ++o) {
            current = strtok(NULL, ",");
        }
        for (k = 0; k < 4 && current != NULL && num_satelites > 0; ++k) {
            for (j = 0; j < 4 && current != NULL && num_satelites > 0; ++j) {
                current = strtok(NULL, ",");
            }
            int satelite_status = atoi(current);           
            if (satelite_status >= 30) {
                count++;
                if (count == 3) {
                    data->satelitesOK = 1;
                    return;
                }
            }
            num_satelites--;
        }
    }
    data->satelitesOK = -1 ;
    return;
}

void proccessRMC(char * buffer, stream_t * data) {
    char tokens[10][STRING_SIZE];
    int i;
    char * current = strtok(NULL, ",");
    for (i = 0; i < 10; ++i) {
        strcpy(tokens[i], current);
        current = strtok(NULL, ",");
    }
    char * the_time = strtok(tokens[0], ".");
    strcat(the_time, tokens[8]);
    // printf("%s", the_time);
    strptime(the_time, "%H%M%S%d%m%y", &(data->location.time));

    data->location.time.tm_isdst = 0;


    degreesToDecimal(&data->location, tokens[2], tokens[4]);
    if (*tokens[3] == 'S') {
        data->location.latitude *= -1;
    }
    if (*tokens[5] == 'W') {
        data->location.longitude *= -1;
    }

}

void degreesToDecimal(loc_t * loc, char * lat, char * lng) {

    double lat_ = atof(lat);
    int lat_degrees = (int) lat_ / 100;
    double lat_minutes = (lat_ - lat_degrees * 100) / 60.0;

    double lng_ = atof(lng);
    int lng_degrees = (int) lng_ / 100;
    double lng_minutes = (lng_ - lng_degrees * 100) / 60.0;

    loc->latitude = ceil((lat_degrees + lat_minutes) * MIL) / MIL;

    loc->longitude = ceil((lng_degrees + lng_minutes) * MIL) / MIL;


}





