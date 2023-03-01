#pragma once
#include <func.h>

struct _File_Data;
struct _Tree_Root;
struct _Data;

//多文件下，用于保存原始文件的结构体
typedef struct _File_Data
{
	std::string file_path;//记录数据来源文件
	std::vector<_Data*> data;//记录原始数据
	//报错  std::vector<std::vector<int>> data(0,std::vector<int>(0,0));
	_File_Data* pre = NULL;//指针，指向前一个元素以及后一个元素
	_File_Data* next = NULL;
	_Tree_Root* tree_root = NULL;//指向建立的树
	int ID_num = 0;//统计ID的种类数
	int Total_data_num = 0;//统计当前文件数据量
}File_Data;
//用于记录ID的结构体
typedef struct _Tree_Root
{
	int ID = -1;
	int total_type = 0;//已经包含的数据类型数量
	int pre_data_type = 0;//前置数据种类
	int count = 0;	//统计Data节点数量
	_Data* child = NULL;
	_Tree_Root* next = NULL;
	_Tree_Root* pre = NULL;
	_File_Data* file = NULL;\
}Tree_Root;
//用于记录数据的结构体
typedef struct _Data
{
	int com_data[DATA_SIZE] = { 0 };//数据部分
	int pos = 1;//记录数据的位置
	int size = 0;//记录数据位数
	int count = 1;//统计相同数据出现的次数
	std::vector<int> tag_every;
	std::vector<int>::iterator tag_each;
	_Data* pre = NULL;//指针，指向前一个元素以及后一个元素
	_Data* next = NULL;
	_Tree_Root* ID = NULL;
	int _pre[DATA_SIZE] = { 0 };//记录前后两组数据间对应字节位的差值
	int tag[5] = { 0 };//标签，用于定义数据类型（楼层、运行、内外召等，具体定义见数组 ）{1.楼层，2.门，3.运行}
}Data;

void tree_main();
void tree_delete();
