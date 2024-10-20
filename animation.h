#ifndef ANIMATION_H
#define ANIMATION_H true
#include "types.h"

void step_frame(animation_data *data);
bool step_time(animation_data *data, unsigned long delta);
char* get_curr_frame(animation_data *data);
animation_data new_animation_data(char **frames, int *frame_transistions, int max_frame, unsigned long frame_delay);
#endif
