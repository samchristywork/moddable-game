#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

int main() {
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  if (luaL_dofile(L, "crosshair.lua") != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Hello, World!");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Hello, World!", 190, 200, 20, LIGHTGRAY);
    trigger_event(L, "render", 800, 600);

    EndDrawing();
  }

  CloseWindow();
  lua_close(L);
}
