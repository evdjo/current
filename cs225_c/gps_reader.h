/* 
 * File:   gps_reader.h
 * Author: evdjoint
 *
 * Created on 22 March 2014, 01:05
 */

#ifndef GPS_READER_H
#define	GPS_READER_H

#ifdef	__cplusplus
extern "C" {
#endif
#define STRING_SIZE 50
#define BUFFER_SIZE 500
#define FILE_NAME_1 "gps_1.dat"
#define FILE_NAME_2 "gps_2.dat"
#define GPS_TIME "GPS_TIME"
#define SATELITE "SATELITE"
#define _EOF "EOF"
#define MIL 1000000.0

    typedef struct location {
        double latitude;
        double longitude;
        struct tm time;
    } loc_t;

    typedef struct stream {
        loc_t location;
        _Bool satelitesOK;

    } stream_t;




    char** str_split(char* a_str, const char a_delim);
    void add_element(loc_t * head, double lat, double lng);
    void loop_through(loc_t * head, void (*funcPtr)(loc_t*));
    void print_loc(loc_t * node);
    void start();
    void synchronize(FILE * file1, FILE * file2);
    
    char * read_line(FILE * file, stream_t * data);
    void proccessRMC(char * buffer, stream_t * data);
    void degreesToDecimal(loc_t * loc, char * lat, char * lng);
#ifdef	__cplusplus
}
#endif

#endif	/* GPS_READER_H */

