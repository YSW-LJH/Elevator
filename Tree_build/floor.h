#pragma once
#include <func.h>

typedef struct _Floor
{
	int ID = 0;
	std::vector<std::vector<int>>data;
	int pre_size = 0;//记录前置字节数
	int count = 0;//记录统计数量
	int byte_pos = 0;//记录字节位置
	int sub_num = 0;//记录增量
	int height = 0;//记录楼层高度
	std::string file_path;
	_Floor* next = NULL;
	_Floor* pre = NULL;
}Floor;

void floor_main();
void floor_delete();