#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

static int drawLine(lua_State *L) {
  float x1 = luaL_checknumber(L, 1);
  float y1 = luaL_checknumber(L, 2);
  float x2 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);

  DrawLine(x1, y1, x2, y2, RED);

  return 0; // Number of return values
}

int main() {
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  lua_register(L, "drawLine", drawLine);

  if (luaL_dofile(L, "mods/crosshair.lua") != LUA_OK) {
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
