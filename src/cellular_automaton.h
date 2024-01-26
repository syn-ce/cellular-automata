#ifndef CELLULAR_AUTOMATON
#define CELLULAR_AUTOMATON

void simulate_cellular_automaton(char *neighborhood_rule, int neighborhood_radius, int display_width, int nr_iterations, int sleep_time_ms);

void print_rules(char *neighborhood_rule, int neighborhood_radius);

#endif // !CELLULAR_AUTOMATON
