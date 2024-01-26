#include <stdio.h>
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

char get_value_from_neighborhood(char * state, int state_length, int index_of_cell, int *neighborhood_map) {
    int nr = 0;
    for (int i = index_of_cell - 1; i <= index_of_cell + 1; i++) {
        if (i == -1) nr = nr * 2 + (state[state_length - 1] == '#' ? 1 : 0); // Wrap around edges
        else nr = nr * 2 + (state[i % state_length] == '#' ? 1 : 0);
    }

    return neighborhood_map[nr] == 1 ? '#' : ' ';
}

void fill_neighborhood_map(int *neighborhood_map, int neighorhood_rule) {
    for (int i = 0; i <= 7; i++) {
        neighborhood_map[i] = neighorhood_rule % 2;
        neighorhood_rule /= 2;
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

void simulate_cellular_automaton_state(char *current_state, char *next_state, int *neighborhood_map, int display_width, int nr_iterations, int sleep_time_ms) { 
     for (int _ = 0; _ < nr_iterations; _++) {
        printf("%s\n", current_state);
        sleep_ms(sleep_time_ms);
        for (int i = 0; i < display_width; i++) {
          next_state[i] = get_value_from_neighborhood(current_state, display_width, i, neighborhood_map);
        }
        for (int i = 0; i < display_width; i++) current_state[i] = next_state[i];
    }   
}

void simulate_cellular_automaton(int neighborhood_rule, int display_width, int nr_iterations, int sleep_time_ms) {
  int neighborhood_map[8];
    fill_neighborhood_map(neighborhood_map, neighborhood_rule);

    char current_state[display_width + 1];
    char next_state[display_width + 1];
    initialize_states(current_state, next_state, display_width);

    simulate_cellular_automaton_state(current_state, next_state, neighborhood_map, display_width, nr_iterations, sleep_time_ms);
}
