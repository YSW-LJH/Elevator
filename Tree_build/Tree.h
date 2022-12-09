#pragma once
#include <func.h>

//用于记录数据的结构体
typedef struct _Data
{
	int com_data[DATA_SIZE] = { 0 };//数据部分
	int pos = 1;//记录数据的位置
	int size = 0;//记录数据位数
	int count = 1;//统计相同数据出现的次数
	_Data* pre = NULL;
	_Data* next = NULL;
	int _pre[DATA_SIZE] = { 0 };//记录前后两组数据间对应字节位的差值
}Data;
//用于记录ID的结构体
typedef struct _Tree_Root
{
	int ID = -1;
	int total_type = 0;//已经包含的数据类型数量
	int pre_data_type = 0;//前置数据种类
	int count = 0;//统计Data节点数量
	unsigned int* Window[DATA_SIZE] = { NULL };//统计Data下每一个滑窗数据
	int Window_count[DATA_SIZE] = { 0 };
	Data* child = NULL;
	_Tree_Root* next = NULL;
	_Tree_Root* pre = NULL;
}Tree_Root;

void tree_main();
void tree_delete();
