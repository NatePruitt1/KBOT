#include "types.h"
#include "animation.h"

//TODO: Add relative frame movement using negative numbers. -

void step_frame(animation_data *data) {
  data->frame += 1;
  if(data->frame >= data->max_frame){
    data->frame = 0;
  }

  int transition = data->frame_transitions[data->frame];
  if(transition != -1) {
    data->curr_frame = data->frames[transition];
  }
}

bool step_time(animation_data *data, unsigned long delta) {
  bool transitioned = false;
  data->curr_delay += delta;
  if(data->curr_delay > data->frame_delay){
    step_frame(data);
    data->curr_delay = 0;
    transitioned = true;
  }

  return transitioned;
}

bool last_frame(animation_data *data) {
  return data->frame == data->max_frame - 1;
}

char* get_curr_frame(animation_data *data) {
  return data->curr_frame;
}

void new_animation_data(animation_data *data, char **frames, int *frame_transitions, int max_frame, unsigned long frame_delay) {
  data->frame_transitions = frame_transitions;
  data->frames = frames;
  data->max_frame = max_frame;
  data->frame_delay = frame_delay;
  data->frame = 0;
  data->curr_frame = frames[0];
}
