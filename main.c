#include <limits.h>
#include <stdlib.h>
#include "cellular_automaton.h"

int main(int argc, char* argv[]) {

    int neighborhood_rule = 30;
    int display_width = 169;
    int nr_iterations = 100;
    int sleep_time_ms = 150;

    if (argc > 1) neighborhood_rule = atoi(argv[1]);
    if (argc > 2) display_width = atoi(argv[2]);
    if (argc > 3) nr_iterations = atoi(argv[3]);
    if (argc > 4) sleep_time_ms = atoi(argv[4]);
    if (nr_iterations == 0) nr_iterations = INT_MAX;

    simulate_cellular_automaton(neighborhood_rule, display_width, nr_iterations, sleep_time_ms);

    return 0;
}
