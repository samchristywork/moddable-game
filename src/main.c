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

int screenWidth = 800;
int screenHeight = 450;

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

void add_object(char *model_path, char *texture_path, char *name,
                Vector3 position) {
  Texture2D texture = LoadTexture(texture_path);
  objects[object_count].model = LoadModel(model_path);

  Material material = objects[object_count].model.materials[0];
  material.maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  objects[object_count].name = name;
  objects[object_count].position = position;
  object_count++;
}

int main() {
  lua_State *L = init_lua();
  init_raylib();

  add_object("res/models/castle.obj", "res/textures/castle.png", "castle",
             (Vector3){0.0f, 0.0f, 0.0f});

  while (!WindowShouldClose()) {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    SetMousePosition(screenWidth / 2, screenHeight / 2);

    if (IsKeyDown(KEY_E))
      cameraYaw(0.02f);

    if (IsKeyDown(KEY_Q))
      cameraYaw(-0.02f);

    if (IsKeyDown(KEY_W))
      cameraMove(0.4f, 0.0f, 0.0f);

    if (IsKeyDown(KEY_A))
      cameraMove(0.0f, 0.4f, 0.0f);

    if (IsKeyDown(KEY_S))
      cameraMove(-0.4f, 0.0f, 0.0f);

    if (IsKeyDown(KEY_D))
      cameraMove(0.0f, -0.4f, 0.0f);

    if (IsKeyDown(KEY_SPACE))
      cameraMove(0.0f, 0.0f, 0.4f);

    if (IsKeyDown(KEY_LEFT_SHIFT))
      cameraMove(0.0f, 0.0f, -0.4f);

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
