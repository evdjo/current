/* 
 * File:   GPSReader.cpp
 * Author: evdjoint
 * 
 * Created on 25 March 2014, 16:39
 */
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Location.h"
#include "GPSReader.h"
using namespace std;

GPSReader::GPSReader(const std::string& file) {
    reader.open(file.c_str());
}

/* Reads a line from the stream, 
 * returns integer based on what was read.*/
int GPSReader::read() {

    string line_read; // string to place the line

    if (!getline(reader, line_read)) {

        return _EOF; // stream ended

    } else if (line_read.empty() || line_read.compare("\r") == 0) {

        return SKIP_LINE; // empty line, skip it

    } else if (line_read.substr(3, 3) == "RMC") {

        process_rmc(line_read); // process the new coordinates and time

        return GPS_TIME;


    } else if (line_read.substr(3, 3) == "GSV") {

        // see how many gsv sentences are there
        stringstream strstream(line_read.substr(7, 1));

        int num_lines;
        strstream >> num_lines;

        //create container array to put the gsv sentences
        string lines[num_lines];


        lines[0] = line_read; // the first sentences is already in line_read

        int i = 1;
        for (i; i < num_lines; i++) { // put  the sentences in the container
            getline(reader, line_read);
            lines[i] = line_read;
        }

        process_gsv(lines, num_lines); // process the sentences
        return SATELITE;

    } else {

        return SKIP_LINE; // skip if anything else occurred 
    }

}

/* Read the gsv sentences in the data array.
 * Determines whether the satellite fix is ok. */
void GPSReader::process_gsv(string data[], int num_sentences) {

    int count = 0;
    int num_satelites;


    stringstream strstream(data[0].substr(11, 2));

    strstream >> num_satelites; // parse the number of satellites 
    int i = 0;

    // loop through the sentences 
    for (i = 0; i < num_sentences; ++i) {

        stringstream asterix_getter(data[i]);

        // get the sentence up to the asterix
        getline(asterix_getter, data[i], '*');

        vector<string> tokens; // vector to place the tokens

        stringstream ss(data[i]); // treat the sentence as string stream
        string current;

        // split the current sentence to tokens
        while (getline(ss, current, ',')) {
            tokens.push_back(current);
        }

        int o = 0;
        //loop through the satellites 
        for (o = 7; o < tokens.size() && !tokens.at(o).empty()
                && num_satelites > 0; o += 4, num_satelites--) {
            
            int value; 
            stringstream satelitegetter(tokens.at(o));
            satelitegetter >> value;  // parse the SNR
            
            if (value >= 30) { // if SNR good increment count
                count++;
                if (count == 3) { // at least 3 satellites had SNR of 30 or more
                    satelitesOK = true;
                    return; //  we are done here
                }
            }
        }
    }
    satelitesOK = false; //if the code reaches here, no 3 satellites were  good
    return;


}

void GPSReader::process_rmc(string& data) {

    stringstream asterix_getter(data);
    getline(asterix_getter, data, '*'); // get the line up to the asterix 

    vector<string> tokens; //vector to store the tokens

    stringstream ss(data);
    string current;

    while (getline(ss, current, ',')) { // split the string to tokens
        tokens.push_back(current);
    }

    stringstream time_getter(tokens.at(1)); // get the time

    string time;

    getline(time_getter, time, '.');

    time += tokens.at(9); // concatenate the time with the date

    // take the time and transform it into struct tm time
    strptime(time.c_str(), "%H%M%S%d%m%y", &this->loc.the_time);

    degrees_to_decimal(tokens.at(3), tokens.at(5)); // transform the degrees

    if (tokens.at(4) == "S") {// if the coordinate is in the southern hemisphere
        loc.latitude *= -1;

    }
    if (tokens.at(6) == "W") {// if the coordinate is in the western hemisphere
        loc.longitude *= -1;
    }

}

void GPSReader::degrees_to_decimal(string lat, string lng) {

    stringstream convertlat(lat);
    double lat_;
    convertlat >>lat_; // parse the latitude
    int lat_degrees = (int) lat_ / 100; // truncate the degrees

    //turn minutes to decimal
    double lat_minutes = (lat_ - lat_degrees * 100) / 60.0;


    //now repeat for the longitude
    stringstream convertlng(lng);
    double lng_;
    convertlng >> lng_;
    int lng_degrees = (int) lng_ / 100;
    double lng_minutes = (lng_ - lng_degrees * 100) / 60.0;



    //round both values and put them to the location
    loc.latitude = round((lat_degrees + lat_minutes) * MIL) / MIL;
    loc.longitude = round((lng_degrees + lng_minutes) * MIL) / MIL;


}

GPSReader::GPSReader(const GPSReader& orig) {
}

GPSReader::~GPSReader() {
}

