#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <string.h>

#define MAX_CALLBACKS 10

typedef struct {
  const char *event_name;
  int callback_ref;
} EventCallback;

EventCallback event_callbacks[MAX_CALLBACKS];
int callback_count = 0;

static int l_game_log(lua_State *L) {
  const char *msg = luaL_checkstring(L, 1);
  printf("[Game Log]: %s\n", msg);
  return 0;
}

static int l_game_on(lua_State *L) {
  const char *event_name = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  // Store the callback function in the registry
  lua_pushvalue(L, 2);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  // Add to our callback array
  event_callbacks[callback_count].event_name = event_name;
  event_callbacks[callback_count].callback_ref = ref;
  callback_count++;

  return 0;
}

void create_game_object(lua_State *L) {
  lua_newtable(L);

  lua_pushstring(L, "log");
  lua_pushcfunction(L, l_game_log);
  lua_settable(L, -3);

  lua_pushstring(L, "on");
  lua_pushcfunction(L, l_game_on);
  lua_settable(L, -3);

  lua_setglobal(L, "game");
}

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

  create_game_object(L);

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
