#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <unistd.h>

// https://stackoverflow.com/a/7660837
void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);
}

// https://stackoverflow.com/a/28827188
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
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

char get_value_from_neighborhood(char * state, int state_length, int index_of_cell, int *neighborhood_map) {
    int nr = 0;
    for (int i = index_of_cell - 1; i <= index_of_cell + 1; i++) {
        if (i == -1) nr = nr * 2 + (state[state_length - 1] == '#' ? 1 : 0);
        else nr = nr * 2 + (state[i % state_length] == '#' ? 1 : 0);
    }

    //printf("%c%c%c: %i -> %i\n", state[index_of_cell-1], state[index_of_cell], state[index_of_cell+1], nr, neighborhood_map[nr]);

    return neighborhood_map[nr] == 1 ? '#' : ' ';
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
      printf("Please specify the rule-number.\n");
      exit(1);
    }

    int neighborhood_nr = atoi(argv[1]);
    int display_width = 21;
    if (argc > 2) display_width = atoi(argv[2]);
    int nr_iterations = 20;
    if (argc > 3) nr_iterations = atoi(argv[3]);
    if (nr_iterations == 0) nr_iterations = INT_MAX;

    int neighborhood_map[8];
    for (int i = 0; i <= 7; i++) {
        neighborhood_map[i] = neighborhood_nr % 2;
        neighborhood_nr /= 2;
    }

    char current_state[display_width + 1];
    char next_state[display_width + 1];
    current_state[display_width] = '\0';
    next_state[display_width] = '\0';
    for (int i = 0; i < display_width; i++) {
        current_state[i] = ' ';
    }
    current_state[display_width / 2] = '#';
    next_state[0] = current_state[0];
    next_state[display_width - 1] = current_state[display_width - 1];
    
    for (int _ = 0; _ < nr_iterations; _++) {
        //if ((_ + 1) % 100 == 0) clearScreen();
        printf("%s\n", current_state);
        sleep_ms(150);
        for (int i = 0; i < display_width; i++) {
          next_state[i] = get_value_from_neighborhood(current_state, display_width, i, neighborhood_map);
        }
        for (int i = 0; i < display_width; i++) current_state[i] = next_state[i];
    }
}
