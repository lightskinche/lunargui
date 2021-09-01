#include "mainheader.h"
reg_function* registered_functions = NULL;
reg_button* registered_buttons = NULL;
//lua funcs
struct luaL_reg lunar[] = {
	{"register_button",LUAPROC_Register_Button},
	{"load_image",LUAPROC_Load_Image},
	{NULL,NULL}
};
//gui main
int Lunar_Update(SDL_Renderer* renderer) {
	SDL_Event guievent;
	//so we don't interfere up the client's renderer color after we're done
	Uint8 r = 0, g = 0, b = 0, a = 0;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	//get mouse state this frame
	int mousex = 0, mousey = 0, pressed = 0;
	pressed = SDL_BUTTON(SDL_GetMouseState(&mousex, &mousey));
	//important time related variables
	static int pressed_last = 0, down_time = 0;
	if (pressed_last < 0 && pressed > 0)
		down_time = clock();
	//go through and handle all buttons
	for (reg_button* tmp = registered_buttons; tmp != NULL; tmp = tmp->next) {
		SDL_Rect result;
		tmp->hovered = SDL_IntersectRect(&(SDL_Rect) { mousex, mousey, 25, 25 }, & (SDL_Rect){ tmp->xpos, tmp->ypos, tmp->xsize, tmp->ysize }, &result);
		if (tmp->hovered == 1 && tmp->last_hovered == 0)
			tmp->hovered_start = clock();
		else if (tmp->hovered == 0 && tmp->last_hovered == 1)
			tmp->hovered_start = INT32_MAX;

		if (down_time >= tmp->hovered_start) {
			if (pressed > 0 && pressed_last < 0)
				if (tmp->button_down)
					tmp->button_down();
			if (pressed_last == 1 && pressed < 0)
				if (tmp->button_up)
					tmp->button_up();
		    SDL_SetRenderDrawColor(renderer, tmp->press_r, tmp->press_g, tmp->press_b, tmp->press_a);
			SDL_RenderFillRect(renderer, &(SDL_Rect){tmp->xpos, tmp->ypos, tmp->xsize, tmp->ysize});
		}
		else {

			SDL_SetRenderDrawColor(renderer, tmp->unpress_r, tmp->unpress_g, tmp->unpress_b, tmp->unpress_a);
			SDL_RenderFillRect(renderer, &(SDL_Rect){tmp->xpos, tmp->ypos, tmp->xsize, tmp->ysize});
		}
		tmp->last_hovered = tmp->hovered;
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if (pressed_last > 0 && pressed < 0) //we have to do this check down here so we don't interfere with button_up callbacks
		down_time = 0;
	pressed_last = pressed;
}
//so that I can just delete main to turn this into a lib
void Lunar_Init(char* lua_filename, char* resources_loc) {
	//execute lua
	lua_State* L = lua_open();
	luaL_openlibs(L);
	luaL_openlib(L, "lunar", lunar, 0);
	if (luaL_loadfile(L, lua_filename) || lua_pcall(L, 0, 0, 0))
		printf("LUA EXCEPTION: %s\n", luaL_checkstring(L, -1));
}
//everything below this point is the client's program, this is faster than seperating these two programs
void testcall(void) {
	printf("Ran\n");
}
void testcall2(void) {
	printf("Ran other\n");
}
int main(void) {
	SDL_Window* window = SDL_CreateWindow("test", 0, 100, 800, 800, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	REGISTER_FUNC("callback_test", testcall);
	REGISTER_FUNC("callback_test2", testcall2);
	Lunar_Init("lunar.lua", "resources/");
	SDL_Event main_event;
	while (1) {
		while (SDL_PollEvent(&main_event))
			if (main_event.window.event == SDL_WINDOWEVENT_CLOSE)
				exit(0);
		SDL_RenderClear(renderer);
		Lunar_Update(renderer);
		SDL_RenderPresent(renderer);
	}
	return 0;
}