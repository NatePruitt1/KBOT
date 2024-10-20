#ifndef TYPES_H
#define TYPES_H true
//Three modes: 
//scroll through a bunch of love letters.
//a menu with simple commands like brightness up or down
//eyes that look around in a loop to look like bmo.
typedef enum state {
  menu,
  letters,
  eye_animation,
  boot_animation,
} state;

typedef struct {
  unsigned long frame_delay;
  unsigned long curr_delay;
  int frame;
  int max_frame;
  char *curr_frame;
  char **frames;
  int *frame_transitions;
} animation_data;

#endif
