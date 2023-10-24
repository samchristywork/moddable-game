#include <math.h>
#include <raylib.h>
#include <string.h>

struct Object {
  char *name;
  Model model;
  Vector3 position;
};

struct Object objects[100];
int object_count = 0;

#include "scripting.h"

Camera camera = {0};

const int screenWidth = 800;
const int screenHeight = 450;

void cameraYaw(float angle) {
  float currentAngle = atan2(camera.target.z - camera.position.z,
                             camera.target.x - camera.position.x);
  float currentDistance = sqrt(pow(camera.target.z - camera.position.z, 2) +
                               pow(camera.target.x - camera.position.x, 2));

  camera.target.x =
      camera.position.x + currentDistance * cos(currentAngle + angle);
  camera.target.z =
      camera.position.z + currentDistance * sin(currentAngle + angle);
}

void cameraMove(float x, float y, float z) {
  float currentAngle = atan2(camera.target.z - camera.position.z,
                             camera.target.x - camera.position.x);
  float currentDistance = sqrt(pow(camera.target.z - camera.position.z, 2) +
                               pow(camera.target.x - camera.position.x, 2));

  camera.position.x += x * cos(currentAngle);
  camera.position.z += x * sin(currentAngle);

  camera.position.x += y * sin(currentAngle);
  camera.position.z -= y * cos(currentAngle);

  camera.target.x = camera.position.x + currentDistance * cos(currentAngle);
  camera.target.z = camera.position.z + currentDistance * sin(currentAngle);

  camera.position.y += z;
  camera.target.y += z;
}

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

  objects[0].model = LoadModel("res/models/castle.obj");
  Texture2D texture = LoadTexture("res/textures/castle.png");
  objects[0].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
  objects[0].name = "castle";
  objects[0].position = (Vector3){0.0f, 0.0f, 0.0f};
  object_count++;

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
