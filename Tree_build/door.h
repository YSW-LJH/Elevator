#pragma once
#include <func.h>

typedef struct _Door
{
	std::string data_sequence = "\0";
	struct _Tree_Root* root = NULL;
	_Door* next = NULL;
	_Door* pre = NULL;
}Door;

void door_main();
void door_delete();