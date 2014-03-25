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


#define STRING_SIZE 64
#define BUFFER_SIZE 512
#define FILE_NAME_1 "gps_1.dat"
#define FILE_NAME_2 "gps_2.dat"
#define SKIP_LINE 1
#define GPS_TIME 2 
#define SATELITE 3
#define _EOF -1

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

    typedef struct node {
        loc_t loc;
        struct node * next;
    } node_t;





    void add_element(node_t ** head, loc_t loc);
    void loop_through(node_t * head, void (*funcPtr)(node_t*));
    void print_loc(node_t * node);


    void start();

    int sync_time_gps(FILE * file1, FILE * file2,
            stream_t * strm_1, stream_t * strm_2);

    int sync_satelites(FILE * file1, FILE * file2,
            stream_t * strm_1, stream_t * strm_2);


    int read_line(FILE * file, stream_t * data);
    void proccess_rmc(char * buffer, stream_t * data);
    void proccess_gsv(char lines[][BUFFER_SIZE], int num_lines, stream_t * data);
    void degrees_to_decimal(loc_t * loc, char * lat, char * lng);


    void get_offset(long * lat_offset, long * lng_offset, loc_t loc_1, loc_t loc_2);
    void add_offset(long lat_offset, long lng_offset, loc_t good, loc_t * bad);


#ifdef	__cplusplus
}
#endif

#endif	/* GPS_READER_H */

