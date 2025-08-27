#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "ST7789V2_Driver.h"

typedef struct joystick_cfg_struct{
   uint8_t setup_done;
   GPIO_Pin_t SW, VRx, VRy;
} joystick_cfg_t;

// this value can be tuned to alter tolerance of joystick movement
#define TOL 0.1f
#define RAD2DEG 57.2957795131f

typedef enum Direction_enum {
    CENTRE,  // 0
    N,       // 1
    NE,      // 2
    E,       // 3
    SE,      // 4
    S,       // 5
    SW,      // 6
    W,       // 7
    NW      // 8
} Direction;

typedef struct Vector2D_struct {
  float x;
  float y;  
} Vector2D;

typedef struct Polar_struct {
    float mag;
    float angle;
} Polar;


void joystick_init();  // needs to be called at start with joystick centred
float joystick_get_mag();              // polar
float joystick_get_angle();            // polar
Vector2D joystick_get_coord();         // cartesian co-ordinates x,y
Vector2D joystick_get_mapped_coord();  // x,y mapped to circle
Direction joystick_get_direction();    // N,NE,E,SE etc.
Polar joystick_get_polar();            // mag and angle in struct form

// centred x,y values 
float _x0;
float _y0;


#endif