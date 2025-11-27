#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdarg.h>
#include <usart.h>
#include "LCD.h"
#include "stm32l4xx_hal.h"

#ifndef VERBOSE
#define VERBOSE 0 // Change to 1 for verbose debugging
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

void println(const char *fmt, ...);
void println_debug(const char *fmt, ...);

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

uint8_t get_inputs();
void update_game();
void render(ST7789V2_cfg_t* cfg);
void add_object(game_obj_t* obj);
uint8_t edge_collision(game_obj_t* obj);
uint8_t obj_collision(game_obj_t* obj);
uint8_t move_obj(game_obj_t* obj, uint32_t ticks);



#endif