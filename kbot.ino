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
#define HEIGHT 2

byte eye_middle[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
};

byte eye_left[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
};

byte eye_right[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
};

byte eye_close[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
};

byte smile[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
};

byte smile_left[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10010,
  0b01100,
};

byte smile_right[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01001,
  0b00110,
};

//all of the data for the boot animation.
char **boot_anim_frames = (char *[]){
  "  Loading MEM\n   [       ]", 
  "  Loading MEM\n   [-      ]", 
  "  Loading MEM\n   [--     ]", 
  "  Loading MEM\n   [---    ]", 
  "  Loading MEM\n   [----   ]", 
  "  Loading MEM\n   [-----  ]", 
  "  Loading MEM\n   [------ ]", 
  "  Loading MEM\n   [-------]", 
  "   INIT KBOT\n      [|]",
  "   INIT KBOT\n      [-]",
  "KBOT INITIALIZED\n  LOVE YOU :)"
};

int boot_anim_transitions[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 10, 10, 10, 10, 10, 10, 10};

char **eye_anim_frames = (char *[]){
  "\5     \1 \1\n       \6",
  "\5    \3 \3\n      \6",
  "\5      \2 \2\n        \6",
  "\5     \4 \4\n       \6",
};

int eye_anim_trans[] = {0, 3, 0, 0, 1, 1, 1, 1, 0, 3, 0, 0, 2, 2, 0, 0, 3, 0, 0};

char **letter_anim_frames = (char *[]) {
  " \5I love you!\5",
  "You are my best\nfriend",
  "and I could not\nask for a better",
  "person to spend\nmy time with.",
  "You are\nbeautiful, sexy",
  "and so\nintelligent,",
  "I love you",
  "more than I can\nexpress.",
  "I wish you\nan amazing year!!",
  "Nate \5",
};

int letter_anim_trans[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 8, 9, 9, 9};

animation_data boot_anim_data;
animation_data eye_anim_data;
animation_data love_letter_data;

static unsigned long last_milli;
static unsigned long delta_milli;
static bool assigned = false;

void update_time() {
  unsigned long curr_time = millis();
  if(!assigned) {
    delta_milli = 0;
    assigned = true;
  } else {
    if(curr_time < last_milli) {
      //overflow occured, set delta time to 1.
      delta_milli = 1;
    } else {
      delta_milli = curr_time - last_milli;
    }
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
    int i = 0;
    int row = 0;
    while(screen_buffer[i] != '\0') {
      if(screen_buffer[i] == '\n'){
        row += 1;
        screen.setCursor(0, row);
      } else {
        screen.print(screen_buffer[i]);
      }
      i += 1; 
    }
    update_screen = false;
  }
}

static state curr_state;
animation_data *current_anim_data;

//Changes from one state to another state
state change_state(state new_state, state curr_state) {
  Serial.print("Transition from ");
  Serial.print(curr_state);
  Serial.print(" -> ");
  Serial.print(new_state);
  Serial.print("\n");
  if(new_state == eye_animation) {
    screen_buffer = eye_anim_data.frames[0];
    update_screen = true;
  } else if(new_state == letter_animation) {
    screen_buffer = love_letter_data.frames[0];
    update_screen = true;
  }
  return new_state;
}

//Applies the state to the buffer.
void process_state(state *curr_state) {
  if(*curr_state == boot_animation) {
    if(step_time(&boot_anim_data, delta_milli)){
      screen_buffer = get_curr_frame(&boot_anim_data);
      update_screen = true;
    }

    if(boot_anim_data.frame == 29) {
      *curr_state = change_state(eye_animation, *curr_state);
    }
  } else if (*curr_state == eye_animation) {
    if(step_time(&eye_anim_data, delta_milli)){
      screen_buffer = get_curr_frame(&eye_anim_data);
      update_screen = true;
    }
  } else if (*curr_state == letter_animation) {
    if(step_time(&love_letter_data, delta_milli)){
      screen_buffer = get_curr_frame(&love_letter_data);
      update_screen = true;
    }
  }
}

int last_volt = 0;
int curr_volt = 0;
int threshold = 800;
bool switch_pressed(int pin) {
  curr_volt = analogRead(pin);

  bool last_pressed = last_volt < curr_volt;
  bool is_pressed = curr_volt >= threshold;
  last_volt = curr_volt;
  return last_pressed && is_pressed;
}

void setup() {
  new_animation_data(&boot_anim_data, boot_anim_frames, boot_anim_transitions, sizeof(boot_anim_transitions) / 2, 500UL);
  new_animation_data(&eye_anim_data, eye_anim_frames, eye_anim_trans, 19, 500UL);
  new_animation_data(&love_letter_data, letter_anim_frames, letter_anim_trans, 15, 2500UL);
  curr_state = boot_animation;
  screen.begin(WIDTH, HEIGHT);
  screen.createChar(1, eye_middle);
  screen.createChar(2, eye_right);
  screen.createChar(3, eye_left);
  screen.createChar(4, eye_close);
  screen.createChar(5, heart);
  screen.createChar(6, smile);
  check_screen_update();

  Serial.begin(9600);
  Serial.print("Hello!!");
}

void loop() {
  update_time();
  process_state(&curr_state);
  check_screen_update(); 
  if(switch_pressed(A0)) {
    if(curr_state != boot_animation) {
      if(curr_state == eye_animation) {
        eye_anim_data.frame = 0;
        curr_state = change_state(letter_animation, curr_state);
      } else if(curr_state == letter_animation) {
        love_letter_data.frame = 0;
        curr_state = change_state(eye_animation, curr_state);
      }
    }
  }
}
