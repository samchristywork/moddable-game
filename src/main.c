#include <flecs.h>
#include <raylib.h>
#include <string.h>

#include "scripting.h"

typedef struct {
  float x, y, z;
} Position, Velocity, Acceleration;

Camera camera = {0};

const int screenWidth = 800;
const int screenHeight = 450;

void move_entity(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 1);
  Velocity *v = ecs_field(it, Velocity, 2);
  Velocity *a = ecs_field(it, Velocity, 3);

  for (int i = 0; i < it->count; i++) {
    p[i].x += v[i].x;
    p[i].y += v[i].y;
    p[i].z += v[i].z;

    v[i].x += a[i].x;
    v[i].y += a[i].y;
    v[i].z += a[i].z;
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

  ECS_COMPONENT(ecs, Position);
  ECS_COMPONENT(ecs, Velocity);
  ECS_COMPONENT(ecs, Acceleration);

  ECS_SYSTEM(ecs, move_entity, EcsOnUpdate, Position, Velocity, Acceleration);

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

    ecs_progress(ecs, 0);
  }

  CloseWindow();
  lua_close(L);
  ecs_fini(ecs);
}
