#include "engine.h"

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

#define max_objs 10
static int scene_index = 0;
game_obj_t* scene[max_objs];

const int target_FPS = 30;
static int previous_tick;

uint8_t get_inputs() {

  return 0;
}

void update_game() {
  println("Updating %d objects.", scene_index);
  for (int i = 0; i < scene_index; i++) {
    println("Updating obj: %d", i);
    scene[i]->update(scene[i]);
   }
}

void render(ST7789V2_cfg_t* cfg) {
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

void add_object(game_obj_t* obj) {
  if (scene_index < max_objs) {
    println("Adding object: %d", scene_index);
    scene[scene_index] = obj;
    scene_index++;
  }
}

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
