#pragma once
#include <func.h>

struct _Data;

typedef struct _Floor_Data
{
	vector<_Data*> data;
}Floor_Data;

void floor_compare_main();
void floor_compare_delete(Floor_Data* floor_data);