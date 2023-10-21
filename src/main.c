#include <flecs.h>
#include <raylib.h>
#include <string.h>

#include "scripting.h"

typedef struct {
  float x, y;
} Position, Velocity;

Camera camera = {0};

const int screenWidth = 800;
const int screenHeight = 450;

void move_entity(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 1);
  Velocity *v = ecs_field(it, Velocity, 2);

  for (int i = 0; i < it->count; i++) {
    p[i].x += v[i].x;
    p[i].y += v[i].y;
  }
}

ecs_world_t *init_ecs() { return ecs_init(); }

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
  ecs_world_t *ecs = init_ecs();

  Model model =
      LoadModel("raylib/build/examples/resources/models/obj/castle.obj");
  Texture2D texture = LoadTexture(
      "raylib/build/examples/resources/models/obj/castle_diffuse.png");
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  Vector3 position = {0.0f, 0.0f, 0.0f};

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
