#include <raylib.h>
#include <string.h>

#include "scripting.h"

typedef struct {
  float x, y, z;
} Position, Velocity, Acceleration;

Camera camera = {0};

const int screenWidth = 800;
const int screenHeight = 450;

void init_raylib() {
  // SetTraceLogLevel(LOG_ERROR);
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screenWidth, screenHeight, "Hello, World!");

  camera.position = (Vector3){50.0f, 50.0f, 50.0f};
  camera.target = (Vector3){0.0f, 10.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);
}

int main() {
  lua_State *L = init_lua();
  init_raylib();

  ecs_entity_t e = ecs_new_id(ecs);

  ecs_set(ecs, e, Position, {0, 0});
  ecs_set(ecs, e, Velocity, {-1, 1});
  ecs_set(ecs, e, Acceleration, {0, -.04});

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    trigger_event(L, "render3d", screenWidth, screenHeight);
    EndMode3D();

    DrawText("Mods All the Way Down", 190, 200, 20, LIGHTGRAY);
    trigger_event(L, "render2d", screenWidth, screenHeight);

    EndDrawing();
  }

  CloseWindow();
  lua_close(L);
}
