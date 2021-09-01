#include "mainheader.h"

int LUAPROC_Register_Button(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	reg_button* tmp = calloc(1, sizeof(*tmp));
	//a little janky but it should work just fine
	lua_rawgeti(L, 1, 1);//pushes displaytext onto top of stack
	tmp->displaytext = luaL_checkstring(L, -1);
	lua_rawgeti(L, 1, 2);//pushes xpos
	tmp->xpos = luaL_checknumber(L, -1);
	lua_rawgeti(L, 1, 3);//pushes ypos
	tmp->ypos = luaL_checknumber(L, -1);
	lua_rawgeti(L, 1, 4);//pushes xsize
	tmp->xsize = luaL_checknumber(L, -1);
	lua_rawgeti(L, 1, 5);//pushes ysize
	tmp->ysize = luaL_checknumber(L, -1);
	//now we need to get values from the other two tables about the button's apperance
	lua_rawgeti(L, 2, 1);
	tmp->press_r = luaL_checknumber(L, -1);
	lua_rawgeti(L, 2, 2);
	tmp->press_g = luaL_checknumber(L, -1);
	lua_rawgeti(L, 2, 3);
	tmp->press_b = luaL_checknumber(L, -1);
	lua_rawgeti(L, 2, 4);
	tmp->press_a = luaL_checknumber(L, -1);
	//wow this is really long, should probably make a helper macro to make this easier
	lua_rawgeti(L, 3, 1);
	tmp->unpress_r = luaL_checknumber(L, -1);
	lua_rawgeti(L, 3, 2);
	tmp->unpress_g = luaL_checknumber(L, -1);
	lua_rawgeti(L, 3, 3);
	tmp->unpress_b = luaL_checknumber(L, -1);
	lua_rawgeti(L, 3, 4);
	tmp->unpress_a = luaL_checknumber(L, -1);

	tmp->hovered_start = INT32_MAX; //very important for handling buttons later
	if (lua_type(L, 5) == LUA_TSTRING) {
		reg_function* tmp_func = NULL;
		HASH_FIND_STR(registered_functions, luaL_checkstring(L, 5), tmp_func);
		tmp->button_up = tmp_func->func;
	}
	if (lua_type(L, 4) == LUA_TSTRING) {
		reg_function* tmp_func = NULL;
		HASH_FIND_STR(registered_functions, luaL_checkstring(L, 4), tmp_func);
		tmp->button_down = tmp_func->func;
	}
	LL_APPEND(registered_buttons, tmp);
	return 0;
}

int LUAPROC_Load_Image(lua_State* L) {

	return 0;
}