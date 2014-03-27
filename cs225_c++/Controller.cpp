/* 
 * File:   Controller.cpp
 * Author: evdjoint
 * 
 * Created on 27 March 2014, 11:49
 */
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include "Location.h"
#include "GPSReader.h"
#include "Controller.h"
using namespace std;

const string Controller::FILE_1 = "gps_1.dat";
const string Controller::FILE_2 = "gps_2.dat";
const string Controller::FILE_3 = "data.gpx";

bool Controller::sync_satelite() {
    do {

        int status = strm_1.read();

        if (status == GPSReader::_EOF)
            return false;

    } while (!strm_1.satelitesOK);


    do {

        int status = strm_2.read();

        if (status == GPSReader::_EOF)
            return false;

    } while (!strm_2.satelitesOK);


    return true;

}

bool Controller::sync_time_gps() {
    int status = 0;
    do {
        status = strm_1.read();
        if (status == GPSReader::_EOF)
            return false;

    } while (status != GPSReader::GPS_TIME);
    status = 0;
    do {
        status = strm_2.read();
        if (status == GPSReader::_EOF)
            return false;

    } while (status != GPSReader::GPS_TIME);

    time_t time_1 = mktime(&strm_1.loc.the_time);
    time_t time_2 = mktime(&strm_2.loc.the_time);

    double time_difference = difftime(time_1, time_2);

    if (time_difference < 0) {
        while (time_difference != 0) {
            status = 0;
            while (status != GPSReader::GPS_TIME) {
                status = strm_1.read();
                if (status == GPSReader::_EOF)
                    return false;
            }
            time_1 = mktime(&strm_1.loc.the_time);
            time_2 = mktime(&strm_2.loc.the_time);
            time_difference = difftime(time_1, time_2);
        }
    } else if (time_difference > 0) {
        while (time_difference != 0) {
            status = 0;
            while (status != GPSReader::GPS_TIME) {
                status = strm_2.read();
                if (status == GPSReader::_EOF)
                    return false;
            }
            time_1 = mktime(&strm_1.loc.the_time);
            time_2 = mktime(&strm_2.loc.the_time);
            time_difference = difftime(time_1, time_2);
        }
    }
    return true;
}

void Controller::go() {
    Controller ctrl(FILE_1, FILE_2);
    if (!(ctrl.sync_satelite() && ctrl.sync_time_gps()))
        return;
    vector<Location> locs;

    int i = 0;

    while (true) {

        if (ctrl.strm_1.satelitesOK) {
            locs.push_back(ctrl.strm_1.loc);

            if (ctrl.strm_2.satelitesOK) {
                ctrl.get_offset(ctrl.strm_1.loc, ctrl.strm_2.loc);
            } else {
                ctrl.add_offset(ctrl.strm_1.loc, ctrl.strm_2.loc);
            }
        } else if (ctrl.strm_2.satelitesOK) {
            locs.push_back(ctrl.strm_2.loc);
            i = locs.size();
            ctrl.add_offset(ctrl.strm_2.loc, ctrl.strm_1.loc);
        }

        int line_read = 0;

        while (line_read != GPSReader::GPS_TIME && line_read != GPSReader::_EOF) {
            line_read = ctrl.strm_1.read();
        }
        if (line_read == GPSReader::_EOF)
            break;

        line_read = 0;

        while (line_read != GPSReader::GPS_TIME && line_read != GPSReader::_EOF) {
            line_read = ctrl.strm_2.read();
        }
        if (line_read == GPSReader::_EOF)
            break;


    }

    ctrl.write(locs);




}

void Controller::write(vector<Location> locs) {
    writer.open(FILE_3.c_str());



    writer << "<?xml version=\"1.0\"?>\n";
    writer << "<gpx version=\"1.0\"";
    writer << "\ncreator=\"Evdzhan Mustafa\"";
    writer << "\nxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";



    for (int i = 0; i < locs.size(); ++i) {
       

        writer << setprecision(10) <<  "<wpt lat=\"" << locs.at(i).latitude << "\" ";
        writer << setprecision(10) << "lon=\"" << locs.at(i).longitude << "\">";
        writer << "\n<time>" << asctime(&locs.at(i).the_time) << "</time>\n</wpt>\n";



    }
    writer << "</gpx>";



}

void Controller::get_offset(const Location& one, const Location& two) {

    lat_offset = (long) (one.latitude * MIL) - (long) (two.latitude * MIL);
    lng_offset = (long) (one.longitude * MIL) -(long) (two.longitude * MIL);

}

void Controller::add_offset(const Location& good, Location& bad) {

    bad.latitude = (round(good.latitude * MIL) + lat_offset) / MIL;
    bad.longitude = (round(good.longitude * MIL) + lng_offset) / MIL;

}

Controller
::Controller(const string& file_1, const string& file_2)
: strm_1(file_1), strm_2(file_2) {

    strm_1.satelitesOK = 0;
    strm_2.satelitesOK = 0;
}

Controller::~Controller() {
}

