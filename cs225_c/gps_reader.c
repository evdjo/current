

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __USE_XOPEN
#include <time.h>
#include <math.h>
#include "gps_reader.h"




int read_line(FILE * file, stream_t * strm) {

    char buffer[BUFFER_SIZE];

    char * status = fgets(buffer, BUFFER_SIZE, file);

    char * temp_buffer = strdup(buffer);

    if (status == NULL) {
        return _EOF;
    }

    char * temp = strsep(&temp_buffer, ",");

    if (strcmp(temp, "$GPRMC") == 0) {

        proccess_rmc(buffer, strm);
        return GPS_TIME;

    } else if (strcmp(temp, "$GPGSV") == 0) {

        char * current = strsep(&temp_buffer, ",");

        int num_lines = atoi(current);

        char lines[num_lines][BUFFER_SIZE];

        strcpy(lines[0], buffer);

        int i;
        for (i = 1; i < num_lines; i++) {

            fgets(buffer, BUFFER_SIZE, file);
            strcpy(lines[i], buffer);
        }

        proccess_gsv(lines, num_lines, strm);
        return SATELITE;

    } else {
        return SKIP_LINE;
    }

}

void proccess_gsv(char lines[][BUFFER_SIZE], int num_lines, stream_t * data) {
    int num_satelites;
    int i = 0, o = 0, j = 0, k = 0;

    char * first_line = strdup(lines[0]);
    char * token;

    for (i = 0; i < 4; ++i) {
        token = strsep(&first_line, ",");
    }

    num_satelites = atoi(token);
    int count = 0;

    for (i = 0; i < num_lines; ++i) {

        char * curr_line = strdup(lines[i]);

        char * asterix_getter = strsep(&curr_line, "*");


        char * current;
        for (o = 0; o < 4; ++o) {
            current = strsep(&asterix_getter, ",");
        }

        for (k = 0; k < 4 && current != NULL && num_satelites > 0; ++k) {

            for (j = 0; j < 4 && current != NULL && num_satelites > 0; ++j) {

                current = strsep(&asterix_getter, ",");

            }
            if (*current != '\0') {

                int satelite_status = atoi(current);

                if (satelite_status >= 30) {
                    ++count;
                    if (count == 3) {
                        data->satelitesOK = 1;
                        return;
                    }
                }
            }
            num_satelites--;
        }
    }
    data->satelitesOK = 0;
    return;
}

void proccess_rmc(char * buffer, stream_t * data) {
    char * tokens[10]; // array of pointers to string

    char * temp = strdup(buffer);

    int i;
    for (i = 0; i < 10; ++i) {
        tokens[i] = strsep(&temp, ",");
    }

    char * the_time = strsep(&tokens[1], ".");
    strcat(the_time, tokens[9]);

    strptime(the_time, "%H%M%S%d%m%y", &(data->location.time));
    data->location.time.tm_isdst = 0;

    degrees_to_decimal(&data->location, tokens[3], tokens[5]);

    if (*tokens[4] == 'S') {
        data->location.latitude *= -1;
    }

    if (*tokens[6] == 'W') {
        data->location.longitude *= -1;
    }



}

void degrees_to_decimal(loc_t * loc, char * lat, char * lng) {

    double lat_ = atof(lat);
    int lat_degrees = (int) lat_ / 100;
    double lat_minutes = (lat_ - lat_degrees * 100) / 60.0;

    double lng_ = atof(lng);
    int lng_degrees = (int) lng_ / 100;
    double lng_minutes = (lng_ - lng_degrees * 100) / 60.0;

    loc->latitude = ceil((lat_degrees + lat_minutes) * MIL) / MIL;

    loc->longitude = ceil((lng_degrees + lng_minutes) * MIL) / MIL;


}





