#include <LiquidCrystal.h>
#include "types.h"
#include "animation.h"

#define RS 12
#define ENABLE 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

#define WIDTH 16
#define HEIGHT 4

unsigned long last_milli;
unsigned long delta_milli;

void update_time() {
  unsigned long curr_time = millis();
  
  if(curr_time < last_milli) {
    //overflow occured, set delta time to 1.
    delta_milli = 1;
  } else {
    delta_milli = curr_time - last_milli;
  }
  
  last_milli = curr_time;
}

LiquidCrystal screen(RS, ENABLE, D4, D5, D6, D7);

char *screen_buffer;
bool update_screen;

void check_screen_update() {
  if(update_screen) {
    screen.clear();
    screen.setCursor(0, 0);
    screen.print(screen_buffer);
    update_screen = false;
  }
}

state curr_state = boot_animation;

animation_data boot_anim_data;

//Changes from one state to another state
state change_state(state new_state, state curr_state) {
  return new_state;
}

//Applies the state to the buffer.
void process_state(state curr_state) {
  if(curr_state == boot_animation) {
    if(step_time(&boot_anim_data, delta_milli)){
      screen_buffer = get_curr_frame(&boot_anim_data);
      update_screen = true;
    }
  }
}

void setup() {
  curr_state = boot_animation;
  screen.begin(WIDTH, HEIGHT);
}

void loop() {
  update_time();
  process_state(curr_state);
  check_screen_update(); 
}
