/* 
 * File:   main.cpp
 * Author: evdjoint
 *
 * Created on 09 June 2014, 14:44
 */

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    string buffer;    
    cout << "sq shta testvam kolko si umen...\nKolko e 5 po 5 ?\n";
    while (buffer.compare("20") == 0 || buffer.compare("25") == 0) {
        cin >> buffer;
        cout << " greshka moi proay sa pak \nKolko e 5 po 5 ?";
        
    }
        
    
    
    return 0;
}

