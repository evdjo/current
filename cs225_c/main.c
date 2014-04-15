/* 
 * File:   main.c
 * Author: evdjoint
 *
 * Created on 22 March 2014, 00:59
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gps_reader.h"

int main() {

  //  FILE * file_1 = fopen(FILE_NAME_1, "r");
    char buffer[BUFFER_SIZE] = "1,,";


   //  fgets(buffer, BUFFER_SIZE, file_1);
    
     
   // printf("%s",buffer);
    char lol[BUFFER_SIZE];
    char lol_[BUFFER_SIZE];
    
     
    sscanf(buffer,"%[^,],%[^,]",lol,lol_);
    printf("%s,%s",lol,lol_);
    if(lol[0] == '\0') {
        printf("%s", "TRUE");
    }
    

    return (EXIT_SUCCESS);

}

