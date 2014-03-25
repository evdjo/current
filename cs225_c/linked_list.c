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

void loop_through(node_t * head) {
    FILE *f = fopen(FILE_NAME_3, "w");

    fprintf(f, "%s", "<?xml version=\"1.0\"?>\n"
            "<gpx\n"
            " version=\"1.0\"\n"
            " creator=\"Evdzhan Mustafa\"\n"
            " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n");

    node_t * current = head;
    while (current != NULL) {
        fprintf(f, "<wpt lat=\"%lf\" lon=\"%lf\">\n<time>%s</time>\n</wpt>\n ",
                current->loc.latitude, current->loc.longitude, asctime(&current->loc.time));

        current = current->next;
    }
    fprintf(f, "%s", "</gpx>");
    fclose(f);



}

