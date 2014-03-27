/* 
 * File:   Controller.h
 * Author: evdjoint
 *
 * Created on 27 March 2014, 11:49
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H
using namespace std;

class Controller {
public:
    const static double MIL = 1000000.0;
    const static string FILE_1;
    const static string FILE_2;
    const static string FILE_3;
    static void go();
    void start();
    bool sync_satelite();
    bool sync_time_gps();
    void write(vector<Location> locs);
    void get_offset(const Location& one, const Location& two);
    void add_offset(const Location& good, Location& bad);
    Controller(const string& file_1, const string& file_2);
    virtual ~Controller();
private:
    GPSReader strm_1;
    GPSReader strm_2;
    long lat_offset;
    long lng_offset;
    ofstream writer;

};

#endif	/* CONTROLLER_H */

