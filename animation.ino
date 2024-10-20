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
  
  if(data->curr_delay >= data->frame_delay){
    step_frame(data);
    data->curr_delay = 0UL;
    transitioned = true;
  }

  return transitioned;
}

char* get_curr_frame(animation_data *data) {
  return data->curr_frame;
}

animation_data new_animation_data(char **frames, int *frame_transitions, int max_frame, unsigned long frame_delay) {
  animation_data result;
  result.frame_transitions = frame_transitions;
  result.frames = frames;
  result.max_frame = max_frame;

  result.frame = 0;
  result.curr_frame = frames[0];
}
