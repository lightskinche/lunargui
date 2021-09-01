#include "mainheader.h"

int* HELPER_Load_IntTable(lua_State* L, int idx, int length, int offset) {
	int* returnarray = calloc(length, sizeof(*returnarray));
	for (int i = offset; i < length; ++i) {
		lua_rawgeti(L, idx, i + 1);
		returnarray[i] = lua_tonumber(L, -1);
	}
	return returnarray;
}