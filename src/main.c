#include <flecs.h>
#include <raylib.h>
#include <string.h>

#define MAX_CALLBACKS 10

typedef struct {
  float x, y;
} Position, Velocity;

EventCallback event_callbacks[MAX_CALLBACKS];
int callback_count = 0;

static int l_game_on(lua_State *L) {
  const char *event_name = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);

void move_entity(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 1);
  Velocity *v = ecs_field(it, Velocity, 2);

  for (int i = 0; i < it->count; i++) {
    p[i].x += v[i].x;
    p[i].y += v[i].y;
  }
}

ecs_world_t *init_ecs() { return ecs_init(); }

  DrawLine(x1, y1, x2, y2, RED);

  return 0; // Number of return values
}

static int l_draw_text(lua_State *L) {
  const char *text = luaL_checkstring(L, 1);
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  int size = luaL_checkinteger(L, 4);

  DrawText(text, x, y, size, RED);

  return 0; // Number of return values
}

int main() {
  ecs_world_t *ecs = init_ecs();

  // Load mods
  if (luaL_dofile(L, "mods/log.lua") != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  if (luaL_dofile(L, "mods/crosshair.lua") != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  if (luaL_dofile(L, "mods/frame.lua") != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  const int screenWidth = 800;
  const int screenHeight = 450;

  SetTraceLogLevel(LOG_ERROR);
  InitWindow(screenWidth, screenHeight, "Hello, World!");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Mods All the Way Down", 190, 200, 20, LIGHTGRAY);
    trigger_event(L, "render", screenWidth, screenHeight);

    EndDrawing();
  }

  ECS_COMPONENT(ecs, Position);
  ECS_COMPONENT(ecs, Velocity);

  ECS_SYSTEM(ecs, move_entity, EcsOnUpdate, Position, Velocity);

  ecs_entity_t e = ecs_new_id(ecs);
  ecs_set(ecs, e, Position, {10, 20});
  ecs_set(ecs, e, Velocity, {1, 2});

  ecs_progress(ecs, 0);
  printf("Position: {%f, %f}\n", ecs_get(ecs, e, Position)->x,
         ecs_get(ecs, e, Position)->y);
  ecs_progress(ecs, 0);
  printf("Position: {%f, %f}\n", ecs_get(ecs, e, Position)->x,
         ecs_get(ecs, e, Position)->y);

  CloseWindow();
  lua_close(L);
  ecs_fini(ecs);
}
