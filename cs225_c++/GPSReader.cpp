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

int GPSReader::read() {
    string line_read;

    if (!getline(reader, line_read)) {
        return _EOF;
    } else if (line_read.empty() || line_read.compare("\r") == 0) {
        return SKIP_LINE;
    } else if (line_read.substr(3, 3) == "RMC") {

        proccess_rmc(line_read);
        return GPS_TIME;
    } else if (line_read.substr(3, 3) == "GSV") {

        stringstream strstream(line_read.substr(7, 1));
        int num_lines;
        strstream >> num_lines;
        string lines[num_lines];
        lines[0] = line_read;

        int i = 1;
        for (i; i < num_lines; i++) {
            getline(reader, line_read);
            lines[i] = line_read;
        }

        proccess_gsv(lines, num_lines);
        return SATELITE;
    } else {
        return SKIP_LINE;
    }

}

void GPSReader::proccess_gsv(string data[], int num_sentences) {
    int count = 0;
    int num_satelites;
    stringstream strstream(data[0].substr(11, 2));
    strstream >> num_satelites;
    int i = 0;
    for (i = 0; i < num_sentences; ++i) {

        stringstream asterix_getter(data[i]);
        getline(asterix_getter, data[i], '*');

        vector<string> tokens;
        stringstream ss(data[i]);
        string current;
        while (getline(ss, current, ',')) {
            tokens.push_back(current);
        }

        int o = 0;
        for (o = 7; o < tokens.size() && !tokens.at(o).empty()
                && num_satelites > 0; o += 4, num_satelites--) {
            int value;
            stringstream satelitegetter(tokens.at(o));
            satelitegetter >> value;
            if (value >= 30) {
                count++;
                if (count == 3) {
                    satelitesOK = true;
                    return;
                }
            }
        }
    }

    satelitesOK = false;
    return;


}

void GPSReader::proccess_rmc(string& data) {

    stringstream asterix_getter(data);
    getline(asterix_getter, data, '*');

    vector<string> tokens;
    stringstream ss(data);
    string current;
    while (getline(ss, current, ',')) {
        tokens.push_back(current);
    }

    stringstream time_getter(tokens.at(1));
    string time;
    getline(time_getter, time, '.');

    time += tokens.at(9);
    strptime(time.c_str(), "%H%M%S%d%m%y", &this->loc.the_time);
    degrees_to_decimal(tokens.at(3), tokens.at(5));
    if (tokens.at(4) == "S") {
        loc.latitude *= -1;

    }
    if (tokens.at(6) == "W") {
        loc.longitude *= -1;
    }

}

void GPSReader::degrees_to_decimal(string lat, string lng) {

    stringstream convertlat(lat);
    double lat_;
    convertlat >>lat_;
    int lat_degrees = (int) lat_ / 100;
    double lat_minutes = (lat_ - lat_degrees * 100) / 60.0;

    stringstream convertlng(lng);
    double lng_;
    convertlng >> lng_;
    int lng_degrees = (int) lng_ / 100;
    double lng_minutes = (lng_ - lng_degrees * 100) / 60.0;

    loc.latitude = round((lat_degrees + lat_minutes) * MIL) / MIL;

    loc.longitude = round((lng_degrees + lng_minutes) * MIL) / MIL;


}

GPSReader::GPSReader(const GPSReader& orig) {
}

GPSReader::~GPSReader() {
}

