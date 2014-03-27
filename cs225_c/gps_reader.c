

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __USE_XOPEN
#include <time.h>
#include <math.h>
#include "gps_reader.h"

/* Reads a line from the read, if it is RMC or GSV , the data is processed
 returns integer corresponding to the line read*/
int read_line(FILE * file, stream_t * strm) {

    char buffer[BUFFER_SIZE]; // buffer to store the current line

    char * status = fgets(buffer, BUFFER_SIZE, file); // read a line

    char * temp_buffer = strdup(buffer); // duplicate , will be needed later on

    if (status == NULL) { // check if stream ended
        return _EOF;
    }

    char * temp = strsep(&temp_buffer, ","); // get the type of sentence

    if (strcmp(temp, "$GPRMC") == 0) {

        proccess_rmc(buffer, strm);
        return GPS_TIME; // new cooridnate and time was processed

    } else if (strcmp(temp, "$GPGSV") == 0) {

        char * current = strsep(&temp_buffer, ",");

        int num_lines = atoi(current);

        char lines[num_lines][BUFFER_SIZE]; // array to store the gsv sentences

        strcpy(lines[0], buffer); // store the first, 
        //already read line to the array

        int i;
        for (i = 1; i < num_lines; i++) {// put each gsv sentence to the array

            fgets(buffer, BUFFER_SIZE, file);
            strcpy(lines[i], buffer);
        }

        proccess_gsv(lines, num_lines, strm);

        return SATELITE; // new satellite fix was processed

    } else {
        return SKIP_LINE;
    }

}

/* Read the GSV sentences passed and update the stream passed in */
void proccess_gsv(char lines[][BUFFER_SIZE], int num_lines, stream_t * strm) {
    int num_satellites; // how many satellites in  the lines
    int i = 0, o = 0, j = 0, k = 0; // integers used in the loops below


    // duplicate used to see how many satellites are there
    char * first_line = strdup(lines[0]);
    char * _num_satellites; // to store the read satellites number

    for (i = 0; i < 4; ++i) { // skip the first four tokens
        _num_satellites = strsep(&first_line, ",");
    }
    // now _num_satellites holds the number of satellites , parse that to int
    num_satellites = atoi(_num_satellites);

    // count -  how many good satellites fixes are  there
    int count = 0;


    // loop through the gsv sentences
    for (i = 0; i < num_lines; ++i) {


        // duplicate  the current line
        char * curr_line = strdup(lines[i]);

        // the line without the asterix and the data after it
        char * asterix_getter = strsep(&curr_line, "*");



        char * current;
        for (o = 0; o < 4; ++o) { // skip the first 4 tokens
            current = strsep(&asterix_getter, ",");
        }


        // loop through each satellite SNR
        for (k = 0; k < 4 && current != NULL && num_satellites > 0; ++k) {

            // skip each 4 tokens, to the to the next SNR
            for (j = 0; j < 4 && current != NULL
                    && num_satellites > 0; ++j, --num_satellites) {

                current = strsep(&asterix_getter, ",");
            }

            //current should point to a SNR value
            if (*current != '\0') { // check if is null

                //parse it to integer
                int satelite_status = atoi(current);

                //check if its good
                if (satelite_status >= 30) {
                    ++count;
                    if (count == 3) { // if count reaches 3 
                        strm->satelitesOK = 1; // exit and set
                        //the satellite fix to true
                        return;
                    }
                }
            }
        }
    }
    strm->satelitesOK = 0; // no three satellites met the criteria  
    //  stream has bad fix 
    return;
}

/* Read the RMC sentence passed and update the time and coordinates*/
void proccess_rmc(char * buffer, stream_t * data) {
    char * tokens[10]; // array of pointers to string

    char * temp = strdup(buffer);

    int i;
    for (i = 0; i < 10; ++i) { // store all the tokens to the array
        tokens[i] = strsep(&temp, ",");
    }

    char * temp_time = strdup(tokens[1]); // duplicate to use as a temp time
    char * the_time = strsep(&temp_time, "."); // truncate the milliseconds
    strcat(the_time, tokens[9]); //concatenate  the date and the time

    // parse the time
    strptime(the_time, "%H%M%S%d%m%y", &(data->location.time));

    data->location.time.tm_isdst = 0; // set this to zero, otherwise bugs occur

    //convert the coordinates from degrees to decimal, and  put it into the 
    // stream's location
    degrees_to_decimal(&data->location, tokens[3], tokens[5]);

    if (*tokens[4] == 'S') { // check if coordinate is in south hemisphere
        data->location.latitude *= -1;
    }

    if (*tokens[6] == 'W') {// check if coordinate is in west hemisphere
        data->location.longitude *= -1;
    }



}

/* Transforms coordinates from degrees to a decimal, and puts the coordinates
 * to the loc passed in */
void degrees_to_decimal(loc_t * loc, char * lat, char * lng) {

    double lat_ = atof(lat); // parse the latitude
    int lat_degrees = (int) lat_ / 100; // truncate the degrees

    // turn the minutes to decimal
    double lat_minutes = (lat_ - lat_degrees * 100) / 60.0;

    // repeat for the longitude 
    double lng_ = atof(lng);
    int lng_degrees = (int) lng_ / 100;
    double lng_minutes = (lng_ - lng_degrees * 100) / 60.0;


    //place the values to the loc  
    loc->latitude = round((lat_degrees + lat_minutes) * MIL) / MIL;
    loc->longitude = round((lng_degrees + lng_minutes) * MIL) / MIL;


}





