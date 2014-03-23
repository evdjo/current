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

/*
 * 
 */
int main() {





    start();

    /* loc_t * head = (loc_t*) malloc(sizeof (loc_t));

     add_element(head, 52.5123 , -4.0156);
     add_element(head, 52.7128 , -4.5124);


     loop_through(head, print_loc );*/

    return (EXIT_SUCCESS);
}

void add_element(loc_t * head, double lat, double lng) {

    /*
        loc_t * current = head;

        while (current->next  != NULL) {
            current = current->next;
        }
        current->next = (loc_t*)malloc(sizeof(loc_t));
        current->next->latitude = lat ;
        current->next->longitude= lng ;
    
     */


}

void print_loc(loc_t * node) {

    /*
        printf("%lf,%lf\n", node->latitude,node->longitude);
     */
}

void loop_through(loc_t * head, void (*funcPtr)(loc_t*)) {

    /*
        loc_t * current = head->next;
        while (current != NULL) {
            (*funcPtr)(current);
            current = current->next;
        }
     */


}
