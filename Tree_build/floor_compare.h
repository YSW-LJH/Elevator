#pragma once
#include <func.h>

extern struct _Data;

typedef struct _Floor_Data
{
	std::vector<_Data*> data;
}Floor_Data;

void door_main();
void door_delete();