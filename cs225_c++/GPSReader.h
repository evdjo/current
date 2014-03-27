/* 
 * File:   GPSReader.h
 * Author: evdjoint
 *
 * Created on 25 March 2014, 16:39
 */

#ifndef GPSREADER_H
#define	GPSREADER_H
using namespace std;
class GPSReader {
public:



    const static int SKIP_LINE = 1;
    const static int GPS_TIME = 2;
    const static int SATELITE = 3;
    const static int _EOF = -1;
    const static double MIL = 1000000.0;


    GPSReader(const string& file);
    int read();
    void degrees_to_decimal(string lat, string lng);
    void proccess_gsv(string data[], int num_sentences);
    void proccess_rmc(string& data);
    GPSReader(const GPSReader& orig);
    virtual ~GPSReader();

    bool satelitesOK;
    Location loc;

private:
    ifstream reader;
};

#endif	/* GPSREADER_H */

