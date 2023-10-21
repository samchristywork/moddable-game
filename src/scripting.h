#ifndef SCRIPTING_H
#define SCRIPTING_H

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

static int l_draw_line(lua_State *L) {
  float x1 = luaL_checknumber(L, 1);
  float y1 = luaL_checknumber(L, 2);
  float x2 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);

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

static int l_dependency(lua_State *L) {
  const char *text = luaL_checkstring(L, 1);

  char filename[100];
  strcpy(filename, "mods/");
  strcat(filename, text);
  strcat(filename, ".lua");

  if (luaL_dofile(L, filename) != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  return 0; // Number of return values
}

void create_game_object(lua_State *L) {
  lua_newtable(L);

  lua_pushstring(L, "on");
  lua_pushcfunction(L, l_game_on);
  lua_settable(L, -3);

  lua_setglobal(L, "game");
}

void trigger_event(lua_State *L, const char *event_name, int screenWidth,
                   int screenHeight) {
  for (int i = 0; i < callback_count; i++) {
    if (strcmp(event_callbacks[i].event_name, event_name) == 0) {
      lua_rawgeti(L, LUA_REGISTRYINDEX, event_callbacks[i].callback_ref);
      lua_pushinteger(L, screenWidth);
      lua_pushinteger(L, screenHeight);
      lua_pcall(L, 2, 0, 0); // Call with 2 arguments, expect 0 results
    }
  }
}

lua_State *init_lua() {
  lua_State *L = luaL_newstate();

  luaL_openlibs(L);

  create_game_object(L);

  // API functions
  lua_register(L, "drawLine", l_draw_line);
  lua_register(L, "drawText", l_draw_text);
  lua_register(L, "dependency", l_dependency);

  // Load mods
  if (luaL_dofile(L, "mods/all.lua") != LUA_OK) {
    fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // pop error message from the stack
  }

  return L;
}

#endif
