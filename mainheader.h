//std header stuff
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
//sdl stuff
#include "sdl/SDL.h"
#include "sdl/SDL_image.h"
#include "sdl/SDL_ttf.h"
//lua stuff
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/luaconf.h"
#include "lua/lauxlib.h"
//data structures we'll need (include as we need)
#include "datastructures/utlist.h"
#include "datastructures//uthash.h"
//important defines
#define ERR_THREAD_CREATION_FAILED -11
//custom structs and types we need
typedef void (*button_proto)(void);
typedef struct reg_button reg_button;
typedef struct reg_function reg_function;
struct reg_function {
	void* func; //void so the function can be casted to like a button_proto or something when needed-- it's generic
	char* id;
	UT_hash_handle hh;
};
struct reg_button {
	//stuff for the actual button
	char* displaytext;
	int xsize, ysize, xpos, ypos;
	//stuff for apperance of button
	int press_r, press_g, press_b, press_a;
	int unpress_r, unpress_g, unpress_b, unpress_a;
	//time related stuff for when this button is handled
	int hovered, last_hovered, hovered_start; //so each button can keep track of the last time it was hovered, when the mouse started hovering over it, etc
	//callback
	button_proto button_down, button_up;
	//stuff for linked list
	reg_button* next;
};
//helpful macros
#define REGISTER_FUNC(name,func) {reg_function* tmp_function = calloc(1, sizeof(*tmp_function)); *tmp_function = (reg_function){ func,name }; HASH_ADD_KEYPTR(hh, registered_functions, tmp_function->id, strlen(tmp_function->id), tmp_function);}
//important globals
extern reg_function* registered_functions;
extern reg_button* registered_buttons;
//general prototypes
extern void Lunar_Init(char* lua_filename, char* resources_loc);
extern int Lunar_Update(SDL_Renderer* renderer);
//prototypes for some LUAPROC's
extern int LUAPROC_Register_Button(lua_State* L);
extern int LUAPROC_Load_Image(lua_State* L);