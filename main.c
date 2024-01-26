#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include "cellular_automaton.h"
#include <stdio.h>

void nr_to_bits(int nr, char *bits) {
    for (int i = 0; bits[i] != '\0'; i++) {
        bits[i] = (nr & 1) + '0';
        nr >>= 1;
    }
}

void reverse_string(char *s) {
    int len = 0;
    while (s[len] != '\0') len++;
    for (int i = 0; i < (len + 1) / 2; i++) {
        char tmp = s[len - 1 - i];
        s[len - 1 - i] = s[i];
        s[i] = tmp;
    }
}

int main(int argc, char* argv[]) {

    int neighborhood_rule = 30;
    int neighborhood_radius = 1;
    int display_width = 169;
    int nr_iterations = 100;
    int sleep_time_ms = 150;

    if (argc > 1) neighborhood_rule = atoi(argv[1]);
    if (argc > 2) neighborhood_radius = atoi(argv[2]);
    if (argc > 3) display_width = atoi(argv[3]);
    if (argc > 4) nr_iterations = atoi(argv[4]);
    if (nr_iterations == 0) nr_iterations = INT_MAX;
    if (argc > 5) sleep_time_ms = atoi(argv[5]);

    size_t nr_bits = pow(2, neighborhood_radius * 2 + 1);
    char n_r_bits[nr_bits + 1];
    n_r_bits[nr_bits] = '\0';
    for (int i = 0; i < nr_bits; i++) n_r_bits[i] = ' ';
    nr_to_bits(neighborhood_rule, n_r_bits);
    char *neighborhood_rule_bits = n_r_bits;

    if (argc > 6) neighborhood_rule_bits = argv[6], reverse_string(neighborhood_rule_bits); // Need to reverse; Most significant bit should determine highest nr pattern

    print_rules(neighborhood_rule_bits, neighborhood_radius);
    simulate_cellular_automaton(neighborhood_rule_bits, neighborhood_radius, display_width, nr_iterations, sleep_time_ms);

    return 0;
}
