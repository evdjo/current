#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gps_reader.h"

void add_element(node_t ** the_head, loc_t the_loc) {
    if (*the_head == NULL) {
        *the_head = (node_t*) malloc(sizeof (node_t));
        (**the_head).loc = the_loc;
        (**the_head).next = NULL;
    } else {
        node_t * current = *the_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (node_t*) malloc(sizeof (node_t));
        current->next->loc = the_loc;
        current->next->next = NULL;
    }





}

void print_loc(node_t * node) {


    printf("%lf,%lf\n", node->loc.latitude, node->loc.longitude);

}

void loop_through(node_t * head, void (*funcPtr)(node_t*)) {

     
        node_t * current = head;
        while (current != NULL) {
            (*funcPtr)(current);
            current = current->next;
        }
     


}

