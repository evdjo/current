/* 
 * File:   Location.cpp
 * Author: evdjoint
 * 
 * Created on 25 March 2014, 22:21
 */
#include <time.h>
#include "Location.h"

Location::Location() {
    latitude = 0;
    longitude = 0;
    the_time.tm_hour = 0;
    the_time.tm_mday = 0;
    the_time.tm_min = 0;
    the_time.tm_sec = 0;
    the_time.tm_year = 0;
    the_time.tm_mon = 0;
    the_time.tm_gmtoff = 0;
}
Location& Location::operator=(const Location& orig) {    
    latitude = orig.latitude;
    longitude = orig.longitude;
    the_time = orig.the_time;          
}

Location::Location(const Location& orig) {
    latitude = orig.latitude;
    longitude = orig.longitude;
    the_time = orig.the_time;
}

Location::~Location() {
}

