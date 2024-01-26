#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// https://stackoverflow.com/a/28827188 // -- cross-platform sleep function
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}
// --

char get_value_from_neighborhood(char *state, int state_length, int index_of_cell, int *neighborhood_map, int neighborhood_radius) {
    int nr = 0;
    for (int i = index_of_cell - neighborhood_radius; i <= index_of_cell + neighborhood_radius; i++) {
        int ind = i % state_length;
        if (i < 0) ind = i + state_length;
        nr = nr * 2 + (state[ind] == '#' ? 1 : 0);
    }

    return neighborhood_map[nr] == 1 ? '#' : ' ';
}

void fill_neighborhood_map(int *neighborhood_map, char *neighborhood_rule, int neighborhood_radius) {
    int nr_neighborhood_entries = (int)pow(2, neighborhood_radius * 2 + 1);
    for (int i = 0; i < nr_neighborhood_entries; i++) {
        if (neighborhood_rule[i] == '\0') {fprintf(stderr, "Not enough Bits in string; Got %i, need %i\n", i, nr_neighborhood_entries); exit(1);}
        if (neighborhood_rule[i] != '0' && neighborhood_rule[i] != '1') {fprintf(stderr, "Invalid Bits in string; Need 0 or 1, got %c\n", neighborhood_rule[i]); exit(1);}
        neighborhood_map[i] = neighborhood_rule[i] - '0';
    }
}

void initialize_states(char *current_state, char *next_state, int display_width) {
    current_state[display_width] = '\0';
    next_state[display_width] = '\0';
    for (int i = 0; i < display_width; i++) {
        current_state[i] = ' ';
    }
    current_state[display_width / 2] = '#';
    next_state[0] = current_state[0];
    next_state[display_width - 1] = current_state[display_width - 1];
}

void simulate_cellular_automaton_state(char *current_state, char *next_state, int *neighborhood_map, int neighborhood_radius, int display_width, int nr_iterations, int sleep_time_ms) {
     for (int _ = 0; _ < nr_iterations; _++) {
        printf("%s\n", current_state);
        sleep_ms(sleep_time_ms);
        for (int i = 0; i < display_width; i++) {
          next_state[i] = get_value_from_neighborhood(current_state, display_width, i, neighborhood_map, neighborhood_radius);
        }
        for (int i = 0; i < display_width; i++) current_state[i] = next_state[i];
    }
}

int get_string_len(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void simulate_cellular_automaton(char *neighborhood_rule, int neighborhood_radius, int display_width, int nr_iterations, int sleep_time_ms) {
    int nr_neighborhood_entries = (int)pow(2.0, (double)(neighborhood_radius * 2 + 1));
    if (nr_neighborhood_entries != get_string_len(neighborhood_rule)) {fprintf(stderr, "Unexpected length of bits-string; Was %i, should have been %i\n", get_string_len(neighborhood_rule), nr_neighborhood_entries); exit(1);}
    int neighborhood_map[nr_neighborhood_entries];

    fill_neighborhood_map(neighborhood_map, neighborhood_rule, neighborhood_radius);

    char current_state[display_width + 1];
    char next_state[display_width + 1];
    initialize_states(current_state, next_state, display_width);

    simulate_cellular_automaton_state(current_state, next_state, neighborhood_map, neighborhood_radius, display_width, nr_iterations, sleep_time_ms);
}

void fill_curr_binary_nr(int nr, char *curr_binary_nr, int nr_digits) {
    for (int i = 0; i < nr_digits; i++) {
        int pow_2 = (int)pow(2.0, (double) nr_digits - 1 - i);
        curr_binary_nr[i] = nr / pow_2 + '0';
        nr = nr % pow_2;
    }
}

void print_rules(char *neighborhood_rule, int neighborhood_radius) {
    int nr_digits_per_nr = neighborhood_radius * 2 + 1;
    int nr_neighborhood_entries = (int)pow(2.0, (double)nr_digits_per_nr);
    int neighborhood_map[nr_neighborhood_entries];
    fill_neighborhood_map(neighborhood_map, neighborhood_rule, neighborhood_radius);

    char curr_binary_nr[nr_digits_per_nr+1];
    curr_binary_nr[nr_digits_per_nr] = '\0';

    for (int i = 0; i < nr_neighborhood_entries; i++) {
        fill_curr_binary_nr(i, curr_binary_nr, nr_digits_per_nr);
        printf("%s -> %i\n", curr_binary_nr, neighborhood_map[i]);
    }
}
