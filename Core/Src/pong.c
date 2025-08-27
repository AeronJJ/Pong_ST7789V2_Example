#include "pong.h"
#include <usart.h>
#include <stdarg.h>


const uint8_t SPRITEZOMBIE0[42][27] = {
    { 1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1 },
    { 1,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1 },
    { 1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1 },
    { 1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1 },
    { 1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1 },
    { 1,0,0,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1 },
    { 0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1 },
    { 0,1,0,1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,1 },
    { 0,1,1,1,0,1,0,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1 },
    { 1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1 },
    { 1,1,0,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,0,0,0,0,1,1,0,1,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,0,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1 },
    { 1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1 },
    { 1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,0,0,0 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,1,1,1,1,0 },
    { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0 },
    { 1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 }
    };

#define VERBOSE 0 // Change to 1 for verbose debugging
#define DEBUG 1

void println(const char *fmt, ...) {
  #if VERBOSE
    char buffer[128];  // Adjust size as needed
    va_list args;

    // Format string into buffer
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Clamp if vsnprintf would overflow
    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
        buffer[len] = '\0';
    }

    // Append newline
    if (len < sizeof(buffer) - 2) {
        buffer[len++] = '\r';
        buffer[len++] = '\n';
        buffer[len] = '\0';
    }

    // Transmit over UART
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, HAL_MAX_DELAY);
  #endif
}

void println_debug(const char *fmt, ...) {
  #if DEBUG
    char buffer[128];  // Adjust size as needed
    va_list args;

    // Format string into buffer
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    // Clamp if vsnprintf would overflow
    if (len >= sizeof(buffer)) {
        len = sizeof(buffer) - 1;
        buffer[len] = '\0';
    }

    // Append newline
    if (len < sizeof(buffer) - 2) {
        buffer[len++] = '\r';
        buffer[len++] = '\n';
        buffer[len] = '\0';
    }

    // Transmit over UART
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, HAL_MAX_DELAY);
  #endif
}

static const uint8_t background_colour = 1;

typedef struct coords {
  int x;
  int y;
} coords_t;

typedef struct vector2d {
  int x;
  int y;
} vector2d;

typedef struct sprite {
  uint16_t width;
  uint16_t height;
  uint8_t* sprite_data;
} sprite_t;

struct game_obj;

typedef void (*update_obj)(struct game_obj*);

typedef struct game_obj {
  coords_t   coord;
  coords_t   prev_coord;
  vector2d   velocity;
  uint8_t    speed;
  coords_t   size;
  uint8_t    colour;
  update_obj update;
  struct game_obj* target;
  sprite_t* sprite;
} game_obj_t;

typedef enum collision { // Onehot
  COLLISION_X0  = 0x01,
  COLLISION_X1  = 0x02,
  COLLISION_Y0  = 0x04,
  COLLISION_Y1  = 0x08,
  COLLISION_OBJ = 0x10
} collision_t;

#define max_objs 10
static int scene_index = 0;
game_obj_t* scene[max_objs];

uint8_t edge_collision(game_obj_t* obj) {
  uint8_t collision = 0;
  if (obj->coord.y + obj->size.y >= 280) {
    println("\nY1 Collision!");
    println("Obj position: x=%d, y=%d", obj->coord.x, obj->coord.y);

    obj->coord.y = 279 - obj->size.y;
    collision |= COLLISION_Y1; 
  }
  else if (obj->coord.y <= 0) {
    println("\nY0 Collision!");
    println("Obj position: x=%d, y=%d", obj->coord.x, obj->coord.y);

    obj->coord.y = 0;
    collision |= COLLISION_Y0; 
  }

  if (obj->coord.x + obj->size.x >= 240) {
    println("\nX1 Collision!");
    println("Obj position: x=%d, y=%d", obj->coord.x, obj->coord.y);

    obj->coord.x = 239 - obj->size.x;
    collision |= COLLISION_X1; 
  }
  else if (obj->coord.x <= 0) {
    println("\nX0 Collision!");
    println("Obj position: x=%d, y=%d", obj->coord.x, obj->coord.y);

    obj->coord.x = 0;
    collision |= COLLISION_X0; 
  }
  return collision;
}

uint8_t obj_collision(game_obj_t* obj) {
  for (int i = 0; i < scene_index; i++) {
    if (scene[i] != obj) {
      if (obj->coord.x + obj->size.x < scene[i]->coord.x || scene[i]->coord.x + scene[i]->size.x < obj->coord.x) {
        continue;
      }
      if (obj->coord.y + obj->size.y < scene[i]->coord.y || scene[i]->coord.y + scene[i]->size.y < obj->coord.y) {
        continue;
      }
      println("\nObj Collision!");
      println("Obj1 position: x=%d, y=%d", obj->coord.x, obj->coord.y);
      println("Obj2 position: x=%d, y=%d", scene[i]->coord.x, scene[i]->coord.y);
      return COLLISION_OBJ;
    }
  }
  return 0;
}

uint8_t move_obj(game_obj_t* obj, uint32_t ticks) {
  println("Moving object");

  obj->prev_coord = obj->coord;
  // Add collision check here?
  obj->coord.y += obj->velocity.y * ticks;
  obj->coord.x += obj->velocity.x * ticks;

  uint8_t collision = edge_collision(obj);
  collision |= obj_collision(obj);

  println("Moving object done");

  return collision;
}

void ball_update(game_obj_t* ball) {
  println("Ball update start");
  uint8_t collision = move_obj(ball, 1);

  if (collision & (COLLISION_X0 | COLLISION_X1)) {
    ball->velocity.x *= -1;
  }

  if (collision & (COLLISION_Y0 | COLLISION_Y1)) {
    ball->velocity.y *= -1;
    ball->coord.x = 120;
    ball->coord.y = 140;
  }

  if (collision & COLLISION_OBJ) {
    ball->velocity.y *= -1;
  }

  // if (ball->coord.y >= 279 - ball->size.y) {
  //   ball->coord.y = 279 - ball->size.y;
  //   ball->velocity.y *= -1; // Reverse direction
  // }
  // if (ball->coord.y <= 0) {
  //   ball->coord.y = 0;
  //   ball->velocity.y *= -1; // Reverse direction
  // }
  println("Ball update done");
}

void bat_update(game_obj_t* bat) {
  println("Bat update start");
  uint8_t collision = move_obj(bat, 1);

  // Find ball object
  if (bat->target == NULL) {
    if (collision & (COLLISION_X0 | COLLISION_X1)) {
      bat->velocity.x *= -1;
    }
  }
  else {
    if (bat->coord.x + bat->size.x/2 == bat->target->coord.x + bat->target->size.x) {
      bat->velocity.x = 0;
    }
    else if (bat->coord.x + bat->size.x/2 > bat->target->coord.x + bat->target->size.x) {
      bat->velocity.x = bat->speed * -1;
    }
    else {
      bat->velocity.x = bat->speed;
    }
  }

  println("Bat update done");
}

void add_object(game_obj_t* obj) {
  if (scene_index < max_objs) {
    println("Adding object: %d", scene_index);
    scene[scene_index] = obj;
    scene_index++;
  }
}

const int target_FPS = 30;
static int previous_tick;

static void get_inputs() {

}

static void update_game() {
  println("Updating %d objects.", scene_index);
  for (int i = 0; i < scene_index; i++) {
    println("Updating obj: %d", i);
    scene[i]->update(scene[i]);
   }
}

static void render(ST7789V2_cfg_t* cfg) {
  println("Starting render");
  int current_tick = HAL_GetTick();
  int delay = 1000/target_FPS - (current_tick - previous_tick);
  if (delay > 0) {
    HAL_Delay(delay);
  }
  previous_tick = current_tick;

  println("Undrawing previous sprites");
  // Undraw previous sprites
  for (int i = 0; i < scene_index; i++) {
    game_obj_t* obj = scene[i];
    if (obj->sprite == NULL) {
      LCD_Draw_Rect(obj->prev_coord.x, obj->prev_coord.y, obj->size.x, obj->size.y, background_colour, 1);
    }
    else {
      LCD_Draw_Rect(obj->prev_coord.x, obj->prev_coord.y, obj->sprite->width, obj->sprite->height, background_colour, 1);
    }
  }

  println("Drawing new sprites");
  // Draw new sprites
  for (int i = 0; i < scene_index; i++) {
    game_obj_t* obj = scene[i];
    if (obj->sprite == NULL) {
      LCD_Draw_Rect(obj->coord.x, obj->coord.y, obj->size.x, obj->size.y, obj->colour, 1);
    }
    else {
      LCD_Draw_Sprite(obj->coord.x, obj->coord.y, obj->sprite->height, obj->sprite->width, obj->sprite->sprite_data);
    }
  }

  println("Refreshing display");
  LCD_Refresh(cfg);
}

void pong_run(ST7789V2_cfg_t* cfg) {

  sprite_t zombie = {
    .height = 42,
    .width = 27,
    .sprite_data = (uint8_t*)SPRITEZOMBIE0
  };

  game_obj_t left_bat = {
    .coord = {
      .x = 120,
      .y = 1
    },
    .prev_coord = {
      .x = 120,
      .y = 1
    },
    .velocity = {
      .x = 1,
      .y = 0
    },
    .speed = 3,
    .size = {
      .x = 50,
      .y = 5
    },
    .colour = 0,
    .update = bat_update,
    .target = NULL,
    .sprite = NULL
  };
  game_obj_t right_bat = {
    .coord = {
      .x = 120,
      .y = 273
    },
    .prev_coord = {
      .x = 120,
      .y = 273
    },
    .velocity = {
      .x = -2,
      .y = 0
    },
    .speed = 3,
    .size = {
      .x = 50,
      .y = 5
    },
    .colour = 0,
    .update = bat_update,
    .target = NULL,
    .sprite = NULL
  };
  game_obj_t ball = {
    .coord = {
      .x = 120,
      .y = 140
    },
    .prev_coord = {
      .x = 120,
      .y = 140
    },
    .velocity = {
      .x = 3,
      .y = 1
    },
    .speed = 1,
    .size = {
      .x = 27,
      .y = 42
    },
    .colour = 0,
    .update = ball_update,
    .target = NULL,
    .sprite = &zombie
  };

  left_bat.target = &ball;
  right_bat.target = &ball;

  add_object(&left_bat);
  add_object(&right_bat);
  add_object(&ball);

  LCD_Fill_Buffer(background_colour);

  LCD_Draw_Circle(150, 140, 20, 9, 0);

  while (1) 
  {
    println("\nWhile loop, objs: %d", scene_index);

    get_inputs();
    println("Inputs done");

    update_game();
    println("Update done");

    render(cfg);
    println("Render done");
  }
}